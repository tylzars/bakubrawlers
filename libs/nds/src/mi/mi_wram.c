#include "types.h"
void MI_SetWramBank(u8 bank) {
    *(vu8 *) 0x4000247 = bank;
}
