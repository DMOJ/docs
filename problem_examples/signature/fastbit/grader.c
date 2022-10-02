#include "fastbit.h"
#include <stdlib.h>
#include <stdio.h>

static unsigned x = 1, y = 2, z = 3;

static unsigned xorshf96(void) {
    unsigned t;
    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

    t = x;
    x = y;
    y = z;
    z = t ^ x ^ y;

    return z;
}

__attribute__((noinline)) int setbits(unsigned long long);

int main() {
    int total;
    scanf("%d", &total);

    for (int i = 0; i < total; ++i) {
        unsigned long long k = ((unsigned long long) xorshf96() << 32) | xorshf96();
        if (setbits(k) != __builtin_popcountll(k))
            return 0;
        if (i == 0) putchar('C');
    }
    puts("orrect.");
}
