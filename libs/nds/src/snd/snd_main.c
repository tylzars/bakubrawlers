#include <nitro/private/snd_h.h>

static BOOL sndInitialized;     // 0x020be088
static OSMutex sndMutex;        // 0x020be08c

void func_020082e8(void) {
    if (sndInitialized != 0) {
        return;
    }
    sndInitialized = TRUE;
    OS_InitMutex(&sndMutex);
    SND_CommandInit();
    func_02008a58();
}

void SND_LockMutex(void) {
    OS_LockMutex(&sndMutex);
}

void SND_UnlockMutex(void) {
    OS_UnlockMutex(&sndMutex);
}

