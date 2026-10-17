#include <nitro/private/fs_dev.h>

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
    u32 target;     // 0x20
    u32 start;      // 0x24
    u32 length;     // 0x28
} FSOverlayInfo;

typedef struct { u32 ptr; u32 size; } FSOvtLoc;
typedef struct { u32 off9, size9, off7, size7; } FSOvtHdr;

extern FSOvtLoc data_020bfca4;
extern FSOvtLoc data_020bfcac;
extern FSArchive data_020bfcb4;
extern u32 data_020baef8[];   // digest table start
extern u32 data_020baef8_end[];

void FS_InitFile(FSFile *f);
BOOL FS_OpenFileRange(FSFile *f, FSArchive *arc, u32 start, u32 end, int id);
BOOL FS_OpenFileById(FSFile *f, FSFileID id);
BOOL FS_CloseFile(FSFile *f);
int FS_ReadFile(FSFile *f, void *buf, int len);
u32 GetOverlayReadSize(FSOverlayInfo *ov);
void FS_InvalidateOverlayAndClearBss(FSOverlayInfo *ov);
FSFileID FS_GetOverlayFileID(FSOverlayInfo *ov);
BOOL ReadOverlayInfoEntry(FSOverlayInfo *out, int target, int id, FSArchive *arc, u32 off9, u32 size9, u32 off7, u32 size7);
BOOL FS_LoadOverlayInfo(FSOverlayInfo *out, int target, int id);
BOOL FS_ReadOverlayFile(FSOverlayInfo *ov);
void MATH_HmacSHA1(u8 *digest, const void *data, u32 len, const void *key, u32 keylen);
BOOL CompareDigest(const u8 *expected, const void *data, u32 len);
void func_0200b2f4(FSOverlayInfo *ov);
void RunOverlayDestructors(FSOverlayInfo *ov);
BOOL func_0200b4d8(FSOverlayInfo *ov);
void OS_Terminate(void);
void UnpackBackwards(void *p);
void DC_FlushRange(void *p, u32 n);

BOOL ReadOverlayInfoEntry(FSOverlayInfo *out, int target, int id, FSArchive *arc, u32 off9, u32 size9, u32 off7, u32 size7)
{
    FSOvtLoc pr;
    FSFile tmp;
    FSFileID fid;
    u32 pos;
    if (target == 0) {
        pr.ptr = off9;
        pr.size = size9;
    } else {
        pr.ptr = off7;
        pr.size = size7;
    }
    pos = (u32) id * 0x20;
    if (pos >= pr.size) return FALSE;
    FS_InitFile(&tmp);
    if (!FS_OpenFileRange(&tmp, arc, pr.ptr + pos, pr.ptr + pr.size, -1)) return FALSE;
    if (FS_ReadFile(&tmp, out, 0x20) != 0x20) {
        FS_CloseFile(&tmp);
        return FALSE;
    }
    FS_CloseFile(&tmp);
    out->target = target;
    fid = FS_GetOverlayFileID(out);
    if (!FS_OpenFileById(&tmp, fid)) return FALSE;
    out->start = tmp.f24;
    out->length = tmp.f28 - tmp.f24;
    FS_CloseFile(&tmp);
    return TRUE;
}

// data_020bfca4 / data_020bfcac are FSRomWork's w8 / wc and w10 / w14 (fs_rom.c): FS_Init stores ptr and size 0,
// or ptr -1 and size 0 when the u16 at 0x027ffc40 is 2; then the loc->ptr != 0 branch returns FALSE at the size test.
BOOL FS_LoadOverlayInfo(FSOverlayInfo *out, int target, int id)
{
    FSOvtLoc *loc = target == 0 ? &data_020bfca4 : &data_020bfcac;
    FSFile tmp;
    FSFileID fid;
    if (loc->ptr != 0) {
        u32 pos = (u32) id * 0x20;
        if (pos >= loc->size) return FALSE;
        MI_CpuCopy8((void *) (loc->ptr + pos), out, 0x20);
        out->target = target;
        FS_InitFile(&tmp);
        fid = FS_GetOverlayFileID(out);
        if (!FS_OpenFileById(&tmp, fid)) return FALSE;
        out->start = tmp.f24;
        out->length = tmp.f28 - tmp.f24;
        FS_CloseFile(&tmp);
        return TRUE;
    }
    return ReadOverlayInfoEntry(out, target, id, &data_020bfcb4, ((FSOvtHdr *) 0x27ffe50)->off9, ((FSOvtHdr *) 0x27ffe50)->size9, ((FSOvtHdr *) 0x27ffe50)->off7, ((FSOvtHdr *) 0x27ffe50)->size7);
}

BOOL FS_ReadOverlayFile(FSOverlayInfo *ov)
{
    FSFile tmp;
    FSFileID fid;
    u32 size, n;
    FS_InitFile(&tmp);
    fid = FS_GetOverlayFileID(ov);
    if (!FS_OpenFileById(&tmp, fid)) return FALSE;
    size = GetOverlayReadSize(ov);
    FS_InvalidateOverlayAndClearBss(ov);
    n = FS_ReadFile(&tmp, (void *) ov->ram_addr, size);
    if (size != n) {
        FS_CloseFile(&tmp);
        return FALSE;
    }
    FS_CloseFile(&tmp);
    return TRUE;
}

typedef struct { const u32 *key; u32 keylen; } FSHmacKey;
extern FSHmacKey data_020a4f44;

void MI_CpuCopy8(const void *src, void *dst, u32 n);

BOOL CompareDigest(const u8 *expected, const void *data, u32 len)
{
    int i;
    u8 digest[20];
    u8 key[0x40];
    MI_CpuFill8(digest, 0, sizeof(digest));
    MI_CpuCopy8(data_020a4f44.key, key, data_020a4f44.keylen);
    MATH_HmacSHA1(digest, data, len, key, data_020a4f44.keylen);
    for (i = 0; i < sizeof(digest); i += sizeof(u32)) {
        if (*(const u32 *) (digest + i) != *(const u32 *) (expected + i)) break;
    }
    return i == sizeof(digest);
}

void func_0200b2f4(FSOverlayInfo *ov)
{
    u32 size = GetOverlayReadSize(ov);
    void (**p)(void);
    void (**end)(void);
    if (*(u16 *) 0x027ffc40 == 2) {
        BOOL ok = FALSE;
        if ((ov->flags & 2) != 0) {
            int n = ((int) data_020baef8_end - (int) data_020baef8) / 0x14;
            if (ov->id < n) {
                ok = CompareDigest(((const u8 *) data_020baef8 + ov->id * 0x14), (void *) ov->ram_addr, size);
            }
        }
        if (!ok) {
            MI_CpuFill8((void *) ov->ram_addr, 0, size);
            OS_Terminate();
            return;
        }
    }
    if ((ov->flags & 1) != 0) {
        UnpackBackwards((void *) (ov->ram_addr + size));
    }
    DC_FlushRange((void *) ov->ram_addr, ov->ram_size);
    p = (void (**)(void)) ov->sinit_start;
    end = (void (**)(void)) ov->sinit_end;
    for (; p < end; p++) {
        if (*p != NULL) (*p)();
    }
}

typedef struct DtorNode { struct DtorNode *next; void (*func)(void *); void *arg; } DtorNode;
extern DtorNode *data_020c17a4;

void RunOverlayDestructors(FSOverlayInfo *ov)
{
    while (1) {
        DtorNode *removed = NULL;
        DtorNode *tail = NULL;
        u32 base;
        u32 end;
        DtorNode *next;
        u32 e;
        DtorNode *prev;
        DtorNode *head;
        DtorNode *n;
        base = ov->ram_addr;
        end = base + (ov->ram_size + ov->bss_size);
        e = OS_DisableInterrupts();
        prev = NULL;
        head = data_020c17a4;
        n = head;
        if (head != NULL) {
            do {
                DtorNode *nx = n->next;
                u32 f = (u32) n->func;
                u32 a = (u32) n->arg;
                if ((a == 0 && f >= base && f < end) || (a >= base && a < end)) {
                    if (tail != NULL) tail->next = n; else removed = n;
                    if (head == n) {
                        data_020c17a4 = nx;
                        head = nx;
                    }
                    n->next = NULL;
                    tail = n;
                    if (prev != NULL) prev->next = nx;
                } else {
                    prev = n;
                }
                n = nx;
            } while (n != NULL);
        }
        OS_RestoreInterrupts(e);
        if (removed == NULL) return;
        n = removed;
        do {
            next = n->next;
            if (n->func != NULL) n->func(n->arg);
            n = next;
        } while (n != NULL);
    }
}

BOOL func_0200b4d8(FSOverlayInfo *ov)
{
    RunOverlayDestructors(ov);
    return TRUE;
}

BOOL FS_LoadOverlay(int target, int id)
{
    FSOverlayInfo ov;
    if (!FS_LoadOverlayInfo(&ov, target, id) || !FS_ReadOverlayFile(&ov)) return FALSE;
    func_0200b2f4(&ov);
    return TRUE;
}

BOOL FS_RunOverlayDestructorsById(int target, int id)
{
    FSOverlayInfo ov;
    if (!FS_LoadOverlayInfo(&ov, target, id) || !func_0200b4d8(&ov)) return FALSE;
    return TRUE;
}
