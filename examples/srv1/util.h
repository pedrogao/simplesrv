/**
 * @file util.h
 * @author pedrogao
 * @brief
 * @version 0.1
 * @date 2022-05-16
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <cstdio>
#include <cstdlib>

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