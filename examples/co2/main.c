#include <stdio.h>

// TODO 定义一个简单的协程结构体，然后通过汇编 swap context 来交换执行
// https://cfsamson.gitbook.io/green-threads-explained-in-200-lines-of-rust/final-200-lines-of-code

extern long co_swap(long);

int main(int argc, char const *argv[])
{
    int count = 1000;
    for (int i = 0; i < count; i++)
    {
        long ret = co_swap(i);
        printf("%ld\n", ret);
    }

    return 0;
}
