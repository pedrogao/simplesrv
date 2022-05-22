#ifndef NEBULA_COROUTINE_H_
#define NEBULA_COROUTINE_H_

#include "fiber.h"

#define DEFAULT_STACK_SIZE 1024

class Coroutine
{
public:
    Coroutine(ef_fiber_proc_t proc, void *params);
    ~Coroutine();

    ef_fiber_t *GetFiber();

private:
    ef_fiber_t *fiber_;
};

#endif