#include <stdio.h>
#include "immintrin.h"

void sum()
{
    // 初始化两个矢量 ，8个32位整数
    __m256i a = _mm256_set_epi32(20, 30, 40, 60, 342, 34523, 474, 123);
    __m256i b = _mm256_set_epi32(234, 234, 456, 78, 2345, 213, 76, 88);

    // 向量加法
    // vpaddd	%ymm3, %ymm2, %ymm2
    __m256i sum = _mm256_add_epi32(a, b);

    // 打印每个值
    int32_t *s = (int32_t *)&sum;
    for (int i = 0; i < 8; i++)
    {
        printf("s[%d] : %d\n", i, s[i]);
    }
}

int main(int argc, char const *argv[])
{
    sum();
    return 0;
}
