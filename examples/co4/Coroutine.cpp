#include "Coroutine.h"

extern ef_fiber_sched_t *ef_fiber_sched;

Coroutine::Coroutine(ef_fiber_proc_t proc, void *params)
{
    fiber_ = ef_fiber_create(ef_fiber_sched, DEFAULT_STACK_SIZE, sizeof(Coroutine), proc, params);
}

Coroutine::~Coroutine()
{
    ef_fiber_delete(fiber_);
    delete fiber_;
}

ef_fiber_t *Coroutine::GetFiber()
{
    return fiber_;
}