#include "Game/Random.hpp"

extern u32 func_02006ba4();

void func_020219ec(u32 seed) {
    MATH_InitRand32(&data_020d42e8, seed);
}

void func_020219dc() {
    func_020219ec(func_02006ba4());
}

s8 func_02021a30(u8 count, const u8 *weights) {
    u8 i = 0;
    s16 total = 0;
    do {
        total += weights[i];
        i++;
    } while (i < count);
    if (total == 0) {
        return -1;
    }
    s16 roll = MATH_Rand32(&data_020d42e8, total);
    i = 0;
    do {
        roll -= weights[i];
        if (roll < 0) {
            return i;
        }
        i++;
    } while (i < count);
    return count - 1;
}
