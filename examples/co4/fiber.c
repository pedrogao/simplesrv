#include "fiber.h"
#include <signal.h>
#include <sys/mman.h>
#include <unistd.h>

/*
 * 页大小
 */
static long ef_page_size = 0;

/*
 * 全局调度器，开辟在 .data 中
 */
ef_fiber_sched_t *ef_fiber_sched = NULL;

/*
 * 协程栈交换执行
 */
long ef_fiber_internal_swap(void *new_sp, void **old_sp_ptr, long retval);

/*
 * 协程栈内部初始化
 */
void *ef_fiber_internal_init(ef_fiber_t *fiber, ef_fiber_proc_t fiber_proc, void *param);

/*
 * 新建协程栈等元数据
 */
ef_fiber_t *ef_fiber_create(ef_fiber_sched_t *rt, size_t stack_size, size_t header_size, ef_fiber_proc_t fiber_proc,
                            void *param)
{
  ef_fiber_t *fiber;
  void *stack; // 协程栈内存
  long page_size = ef_page_size;

  if (stack_size == 0)
  { // 栈最小为 page_size
    stack_size = (size_t)page_size;
  }

  /*
   * make the stack_size an integer multiple of page_size 栈大小
   */
  stack_size = (size_t)((stack_size + page_size - 1) & ~(page_size - 1));

  /*
   * reserve the stack area, no physical pages here
   */
  stack = mmap(NULL, stack_size, PROT_NONE, MAP_PRIVATE | MAP_ANON, -1, 0);
  if (MAP_FAILED == stack)
  {
    return NULL;
  }

  /*
   * map the highest page in the stack area
   */
  if (mprotect((char *)stack + stack_size - page_size, page_size, PROT_READ | PROT_WRITE) < 0)
  {
    munmap(stack, stack_size);
    return NULL;
  }

  /*
   * the topmost header_size bytes used by ef_fiber_t and
   * maybe some outter struct which ef_fiber_t nested in
   */
  fiber = (ef_fiber_t *)((char *)stack + stack_size - header_size); // 内存高位
  fiber->stack_size = stack_size;
  fiber->stack_area = stack;                                     // 栈底
  fiber->stack_upper = (char *)stack + stack_size - header_size; // 栈顶
  fiber->stack_lower = (char *)stack + stack_size - page_size;
  fiber->sched = rt;
  ef_fiber_init(fiber, fiber_proc, param);
  return fiber;
}

void ef_fiber_init(ef_fiber_t *fiber, ef_fiber_proc_t fiber_proc, void *param)
{
  // https://en.wikipedia.org/wiki/X86_calling_conventions
  // stack_ptr = fiber_proc
  // 设置栈指针，协程从栈指针处开始执行
  fiber->stack_ptr = ef_fiber_internal_init(fiber, fiber_proc, (param != NULL) ? param : fiber);
}

void ef_fiber_delete(ef_fiber_t *fiber)
{
  /*
   * free the stack area, contains the ef_fiber_t
   * of course the fiber cannot delete itself
   * 释放内存
   */
  munmap(fiber->stack_area, fiber->stack_size);
}

int ef_fiber_resume(ef_fiber_sched_t *rt, ef_fiber_t *to, long sndval, long *retval)
{
  long ret;
  ef_fiber_t *current;

  /*
   * ensure the fiber is initialized and not exited
   */
  if (to->status != FIBER_STATUS_INITED)
  {
    if (to->status == FIBER_STATUS_EXITED)
    {
      return ERROR_FIBER_EXITED;
    }
    return ERROR_FIBER_NOT_INITED;
  }

  current = rt->current_fiber;
  to->parent = current;
  rt->current_fiber = to;
  // 交换栈指针
  ret = ef_fiber_internal_swap(to->stack_ptr, &current->stack_ptr, sndval);

  if (retval)
  {
    *retval = ret;
  }
  return 0;
}

long ef_fiber_yield(ef_fiber_sched_t *rt, long sndval)
{
  ef_fiber_t *current = rt->current_fiber;
  rt->current_fiber = current->parent;
  // 将 cpu 执行权交给新的 fiber
  return ef_fiber_internal_swap(current->parent->stack_ptr, &current->stack_ptr, sndval);
}

int ef_fiber_expand_stack(ef_fiber_t *fiber, void *addr)
{
  int retval = -1;

  /*
   * align to the nearest lower page boundary
   */
  char *lower = (char *)((long)addr & ~(ef_page_size - 1));

  /*
   * the last one page keep unmaped for guard
   */
  if (lower - (char *)fiber->stack_area >= ef_page_size && lower < (char *)fiber->stack_lower)
  {
    size_t size = (char *)fiber->stack_lower - lower;
    retval = mprotect(lower, size, PROT_READ | PROT_WRITE); // 再开辟一个物理页
    if (retval >= 0)
    {
      fiber->stack_lower = lower; // 变更 lower
    }
  }
  return retval;
}

void ef_fiber_sigsegv_handler(int sig, siginfo_t *info, void *ucontext)
{
  /*
   * we need core dump if failed to expand fiber stack
   */
  if ((SIGSEGV != sig && SIGBUS != sig) || ef_fiber_expand_stack(ef_fiber_sched->current_fiber, info->si_addr) < 0)
  {
    raise(SIGABRT);
  }
}

/*
 * ef_fiber_init_sched 初始化协程调度器
 * 使用信号处理的备用栈
 */
int ef_fiber_init_sched(ef_fiber_sched_t *rt, int handle_sigsegv)
{
  stack_t ss; // see: http://runxinzhi.com/tongyishu-p-12194308.html
  struct sigaction sa = {0};

  /*
   * the global pointer used by SIGSEGV handler
   */
  ef_fiber_sched = rt; // 全局调度器

  rt->current_fiber = &rt->thread_fiber; // 当前协程为线程
  ef_page_size = sysconf(_SC_PAGESIZE);  // 得到系统页大小
  if (ef_page_size < 0)
  {
    return -1;
  }
  // 如果没有自定义 segment fault 处理器，那么使用默认的
  if (!handle_sigsegv)
  {
    return 0;
  }

  /*
   * use alt stack, when SIGSEGV caused by fiber stack, user stack maybe invalid
   */
  ss.ss_sp = mmap(NULL, SIGSTKSZ, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
  if (ss.ss_sp == NULL)
  {
    return -1;
  }
  ss.ss_size = SIGSTKSZ;
  ss.ss_flags = 0;
  // 设置 signal 栈
  if (sigaltstack(&ss, NULL) == -1)
  {
    return -1;
  }

  /*
   * register SIGSEGV handler for fiber stack expanding
   */
  sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
  sa.sa_sigaction = ef_fiber_sigsegv_handler; // 信号处理器
  if (sigaction(SIGSEGV, &sa, NULL) < 0)
  {
    return -1;
  }

  /*
   * maybe SIGBUS on macos
   */
  if (sigaction(SIGBUS, &sa, NULL) < 0)
  {
    return -1;
  }
  return 0;
}
