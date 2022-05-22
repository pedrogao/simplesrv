#include "util.h"

void PanicIf(bool condition, const char *errmsg)
{
    if (condition)
    {
        perror(errmsg);
        exit(EXIT_FAILURE);
    }
}

void Panic(const char *errmsg)
{
    PanicIf(true, errmsg);
}