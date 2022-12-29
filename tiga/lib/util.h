#ifndef __TIGA_UTIL_H__
#define __TIGA_UTIL_H__

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

namespace tiga {

pid_t GetThreadId();
uint32_t GetFiberId();

} // namespace tiga

#endif