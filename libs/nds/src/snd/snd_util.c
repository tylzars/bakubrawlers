#include <nitro/private/snd_h.h>

extern const u8 sndVolumeTable[];   // 0x0208ff9c .. (rodata table indexed from -723)

u16 func_02008bec(int x) {
    int shift;
    u32 vol;
    if (x < -723) {
        x = -723;
    } else if (x > 0) {
        x = 0;
    }
    vol = sndVolumeTable[x + 723];
    if (x < -240) {
        shift = 3;
    } else if (x < -120) {
        shift = 2;
    } else if (x < -60) {
        shift = 1;
    } else {
        shift = 0;
    }
    return vol | (shift << 8);
}
