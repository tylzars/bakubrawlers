#include <nitro/private/os_types.h>

typedef struct FSArchiveLike FSArchiveLike;
typedef struct FSFileLike FSFileLike;

struct FSArchiveLike {
    u8 pad0[0x0c];
    OSThreadQueue queue;     // 0x0c
    u8 pad1[0x08];
    u32 flag;                // 0x1c
    u8 pad2[0x34];
    int (*func)(FSFileLike *, int);   // 0x54
    u32 mask;                // 0x58
};

struct FSFileLike {
    FSFileLike *next;        // 0x00
    FSFileLike *prev;        // 0x04
    FSArchiveLike *archive;  // 0x08
    u32 flag;                // 0x0c
    u32 pad10;
    int result;              // 0x14
    OSThreadQueue queue;     // 0x18
};

extern int (*const fsCommandTable[])(FSFileLike *);   // 0x02090270
extern void func_0200936c(FSFileLike *file, int result);

void func_0200936c(FSFileLike *file, int result) {
    OSIntrMode e = OS_DisableInterrupts();
    FSFileLike *next = file->next;
    FSFileLike *prev = file->prev;
    if (next != NULL) {
        next->prev = prev;
    }
    if (prev != NULL) {
        prev->next = next;
    }
    file->next = NULL;
    file->prev = NULL;
    file->flag &= ~0x4f;
    file->result = result;
    OS_WakeupThread(&file->queue);
    OS_RestoreInterrupts(e);
}

int func_020093c8(FSFileLike *file, int cmd) {
    FSArchiveLike *ar = file->archive;
    BOOL async = (file->flag & 4) ? TRUE : FALSE;
    u32 bit = 1 << cmd;
    int result;
    if (async) {
        ar->flag |= 0x200;
    } else {
        ar->flag |= 0x100;
    }
    if (ar->mask & bit) {
        result = ar->func(file, cmd);
        switch (result) {
        case 0:
        case 1:
        case 4:
            file->result = result;
            break;
        case 8:
            ar->mask &= ~bit;
            result = 7;
            break;
        default:
            break;
        }
    } else {
        result = 7;
    }
    if (result == 7) {
        result = fsCommandTable[cmd](file);
    }
    if (result == 6) {
        if ((file->flag & 4) ? TRUE : FALSE) {
            OSIntrMode e = OS_DisableInterrupts();
            if ((ar->flag & 0x200) ? TRUE : FALSE) {
                do {
                    OS_SleepThread(&ar->queue);
                } while ((ar->flag & 0x200) ? TRUE : FALSE);
            }
            result = file->result;
            OS_RestoreInterrupts(e);
        }
    } else if (!((file->flag & 4) ? TRUE : FALSE)) {
        ar->flag &= ~0x100;
        func_0200936c(file, result);
    } else {
        ar->flag &= ~0x200;
        file->result = result;
    }
    return result;
}
