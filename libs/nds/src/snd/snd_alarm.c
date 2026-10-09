#include <nitro/private/snd_h.h>

typedef struct {
    void (*callback)(void *);
    void *arg;
    u8 count;
} AlarmEntry;

// 0x020bfba0
static AlarmEntry alarmTable[8];

void func_02008a58(void) {
    int i;
    for (i = 0; i < 8; i++) {
        alarmTable[i].callback = NULL;
        alarmTable[i].arg = NULL;
        alarmTable[i].count = 0;
    }
}

void func_02008a88(int idx) {
    AlarmEntry *e = &alarmTable[idx];
    e->count++;
}

u32 func_02008aa8(int idx, void (*cb)(void *), void *arg) {
    AlarmEntry *e = &alarmTable[idx];
    e->callback = cb;
    e->arg = arg;
    e->count++;
    return e->count;
}

void func_02008ad8(u32 data) {
    AlarmEntry *e = &alarmTable[data & 0xff];
    if (((int) data >> 8 & 0xff) != e->count) {
        return;
    }
    if (e->callback == NULL) {
        return;
    }
    e->callback(e->arg);
}
