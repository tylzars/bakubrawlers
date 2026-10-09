#ifndef FS_DEV_H
#define FS_DEV_H
#include "types.h"

typedef struct { void *head; void *tail; } OSThrQ;
typedef struct FSFile FSFile;
typedef struct FSArchive FSArchive;
typedef struct { FSArchive *arc; u32 file_id; } FSFileID;

typedef struct { FSArchive *arc; u16 idx; u16 sub; u32 pos; } FSDirPos;
typedef struct { FSArchive *arc; u32 pos; } FSTablePos;

typedef int (*FSReadFunc)(FSArchive *arc, void *dst, u32 pos, u32 len);

struct FSArchive {
    u32 name;               // 0x00
    FSArchive *next;        // 0x04
    FSArchive *prev;        // 0x08
    OSThrQ q0;              // 0x0c
    OSThrQ q1;              // 0x14
    volatile u32 flags;     // 0x1c
    u32 list_prev;          // 0x20
    FSFile *list;           // 0x24
    u32 base;               // 0x28
    u32 fat;                // 0x2c
    u32 fat_size;           // 0x30
    u32 fnt;                // 0x34
    u32 fnt_size;           // 0x38
    u32 fat_bak;            // 0x3c
    u32 fnt_bak;            // 0x40
    u32 load_mem;           // 0x44
    FSReadFunc read_func;   // 0x48
    FSReadFunc write_func;  // 0x4c
    FSReadFunc read_cur;    // 0x50
    void (*proc)(void *, int); // 0x54
    u32 proc_mask;          // 0x58
};

struct FSFile {
    FSFile *prev;   // 0x00
    FSFile *next;   // 0x04
    FSArchive *arc; // 0x08
    volatile u32 stat;      // 0x0c
    u32 command;    // 0x10
    u32 error;      // 0x14
    OSThrQ queue;   // 0x18
    u32 f20, f24, f28, f2c, f30, f34, f38, f3c, f40, f44;
};
typedef struct { FSArchive *arc; u32 id; u32 pos; u32 is_dir; u32 name_len; char name[0x80]; } FSDirEntry;
static inline BOOL FS_ArcTest(FSArchive *a, u32 m) { return (a->flags & m) != 0; }
static inline BOOL FS_FileTest(FSFile *f, u32 m) { return (f->stat & m) != 0; }
#define FH(p, o) (*(u16 *) ((u8 *) (p) + (o)))
#define FW(p, o) (*(u32 *) ((u8 *) (p) + (o)))

u32 OS_DisableInterrupts(void);   // 0x0200710c
u32 OS_RestoreInterrupts(u32 e);  // 0x02007120
void OS_SleepThread(void *q);     // 0x02005610
void OS_WakeupThread(void *q);    // 0x02005660
void MIi_CpuCopyFast(const void *src, void *dst, u32 n);
void MI_CpuFill8(void *dst, u8 v, u32 n);
#endif
