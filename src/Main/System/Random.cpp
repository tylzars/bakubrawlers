#include "Game/Random.hpp"

extern "C" u32 OS_GetTick();

void SeedRandom(u32 seed);

void SeedRandomFromTick() {
    SeedRandom(OS_GetTick());
}

void SeedRandom(u32 seed) {
    MATH_InitRand32(&gRandom, seed);
}

s8 PickWeighted(u8 count, const u8 *weights) {
    u8 i = 0;
    s16 total = 0;
    do {
        total += weights[i];
        i++;
    } while (i < count);
    if (total == 0) {
        return -1;
    }
    s16 roll = MATH_Rand32(&gRandom, total);
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
