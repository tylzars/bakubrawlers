#include <nitro/private/snd_h.h>

typedef struct {
    void (*callback)(void *);
    void *arg;
    u8 count;
} AlarmEntry;

// 0x020bfba0
static AlarmEntry alarmTable[8];

void ClearAlarmTable(void) {
    int i;
    for (i = 0; i < 8; i++) {
        alarmTable[i].callback = NULL;
        alarmTable[i].arg = NULL;
        alarmTable[i].count = 0;
    }
}

void IncrementAlarmCount(int idx) {
    AlarmEntry *e = &alarmTable[idx];
    e->count++;
}

u32 SetAlarmCallback(int idx, void (*cb)(void *), void *arg) {
    AlarmEntry *e = &alarmTable[idx];
    e->callback = cb;
    e->arg = arg;
    e->count++;
    return e->count;
}

void CallAlarmCallback(u32 data) {
    AlarmEntry *e = &alarmTable[data & 0xff];
    if (((int) data >> 8 & 0xff) != e->count) {
        return;
    }
    if (e->callback == NULL) {
        return;
    }
    e->callback(e->arg);
}
