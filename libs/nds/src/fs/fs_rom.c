#include <nitro/private/fs_dev.h>

typedef struct {
    u32 w0;     // result of OS_GetLockID
    u32 dma;    // dma channel argument of FS_Init
    u32 w8, wc, w10, w14;
} FSRomWork;

typedef struct {
    u32 id;
    u32 ram_addr;
    u32 ram_size;
    u32 bss_size;
    u32 sinit_start;
    u32 sinit_end;
    u32 file_id;
    u32 comp_size : 24;
    u32 flags : 8;
} FSOverlayInfo;

typedef struct { u16 mode; u8 pad[0x1fe]; u32 fnt, fnt_size, fat, fat_size; } RomHdr;
extern FSRomWork data_020bfc9c;
extern FSArchive data_020bfcb4;
extern char data_020a4f40[];

u32 OS_GetLockID(void);
void InitCard(void);
int IsCardPulledOut(void);
void ReadCardRom(u32 dma, u32 src, void *dst, u32 len, void (*cb)(void *), void *arg, int async);
void func_0200e620(u32 v);
void func_0200e63c(u32 v);
void IC_InvalidateRange(void *p, u32 n);
void DC_InvalidateRange(void *p, u32 n);
void FS_InitArchive(FSArchive *a);
BOOL FS_RegisterArchiveName(FSArchive *a, const char *name, int len);
void FS_SetArchiveProc(FSArchive *a, void (*proc)(void *, int), u32 mask);
BOOL FS_SetupArchive(FSArchive *arc, u32 base, u32 fat, u32 fat_size, u32 fnt, u32 fnt_size, FSReadFunc rd, FSReadFunc wr);
u32 FS_LoadArchiveTables(FSArchive *arc, void *mem, u32 size);
void FS_FinishArchiveCommand(FSArchive *arc, int result);

int ReadRomArchive(FSArchive *arc, void *dst, u32 pos, u32 len);
int func_0200ad70(FSArchive *arc, void *dst, u32 pos, u32 len);
int func_0200ade4(FSArchive *arc, void *dst, u32 pos, u32 len);
int func_0200adec(void *arc, int ev);
int RomArchiveProc(void *arc, int ev);

void FinishRomArchiveRead(FSArchive *arc)
{
    FS_FinishArchiveCommand(arc, IsCardPulledOut() ? 5 : 0);
}

int ReadRomArchive(FSArchive *arc, void *dst, u32 pos, u32 len)
{
    ReadCardRom(data_020bfc9c.dma, pos, dst, len, (void (*)(void *)) FinishRomArchiveRead, arc, 1);
    return 6;
}

int func_0200ad70(FSArchive *arc, void *dst, u32 pos, u32 len)
{
    return 1;
}

int RomArchiveProc(void *arc, int ev)
{
    switch (ev) {
    case 9:
        func_0200e620((u16) data_020bfc9c.w0);
        return 0;
    case 10:
        func_0200e63c((u16) data_020bfc9c.w0);
        return 0;
    case 1:
        return 4;
    }
    return 8;
}

int func_0200ade4(FSArchive *arc, void *dst, u32 pos, u32 len)
{
    return 1;
}

int func_0200adec(void *arc, int ev)
{
    return 4;
}

typedef enum { HDR_BASE = 0x027ffc40 } HdrBase;
void FS_Init(u32 dma)
{
    volatile RomHdr *hdr;
    u32 fnt, fat;
    HdrBase base;
    data_020bfc9c.dma = dma;
    data_020bfc9c.w0 = OS_GetLockID();
    data_020bfc9c.w8 = 0;
    data_020bfc9c.wc = 0;
    data_020bfc9c.w10 = 0;
    data_020bfc9c.w14 = 0;
    InitCard();
    FS_InitArchive(&data_020bfcb4);
    FS_RegisterArchiveName(&data_020bfcb4, data_020a4f40, 3);
    base = HDR_BASE;
    hdr = (volatile RomHdr *) base;
    if (hdr->mode == 2) {
        data_020bfc9c.w8 = -1;
        data_020bfc9c.wc = 0;
        data_020bfc9c.w10 = -1;
        data_020bfc9c.w14 = 0;
        FS_SetArchiveProc(&data_020bfcb4, (void (*)(void *, int)) func_0200adec, -1);
        FS_SetupArchive(&data_020bfcb4, 0, 0, 0, 0, 0, func_0200ade4, func_0200ad70);
    } else {
        FS_SetArchiveProc(&data_020bfcb4, (void (*)(void *, int)) RomArchiveProc, 0x602);
        fnt = hdr->fnt;
        if (fnt != -1 && fnt != 0) {
            fat = hdr->fat;
            if (fat != -1 && fat != 0) {
                FS_SetupArchive(&data_020bfcb4, 0, fat, hdr->fat_size, fnt, hdr->fnt_size, ReadRomArchive, func_0200ad70);
            }
        }
    }
}

u32 FS_LoadRomTables(void *mem, u32 size)
{
    return FS_LoadArchiveTables(&data_020bfcb4, mem, size);
}

u32 GetOverlayReadSize(FSOverlayInfo *ov)
{
    if ((ov->flags & 1) != 0) return ov->comp_size;
    return ov->ram_size;
}

void FS_InvalidateOverlayAndClearBss(FSOverlayInfo *ov)
{
    u32 base = ov->ram_addr;
    u32 size = ov->ram_size;
    u32 end = size + ov->bss_size;
    IC_InvalidateRange((void *) base, end);
    DC_InvalidateRange((void *) base, end);
    MI_CpuFill8((void *) (base + size), 0, end - size);
}

FSFileID FS_GetOverlayFileID(FSOverlayInfo *ov)
{
    FSFileID id;
    id.arc = &data_020bfcb4;
    id.file_id = ov->file_id;
    return id;
}
