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

void func_0200a7b4(FSFile *f);
BOOL func_0200a9dc(FSFile *f, FSArchive *arc, u32 start, u32 end, int id);
BOOL func_0200aa24(FSFile *f, FSFileID id);
BOOL func_0200aadc(FSFile *f);
int func_0200ac30(FSFile *f, void *buf, int len);
u32 func_0200af5c(FSOverlayInfo *ov);
void func_0200af78(FSOverlayInfo *ov);
FSFileID func_0200afb4(FSOverlayInfo *ov);
BOOL func_0200afdc(FSOverlayInfo *out, int target, int id, FSArchive *arc, FSOvtHdr hdr);
BOOL func_0200b0d4(FSOverlayInfo *out, int target, int id);
BOOL func_0200b1d0(FSOverlayInfo *ov);
void func_0200b928(u8 *digest, const void *data, u32 len, const void *key, u32 keylen);
BOOL func_0200b260(const u32 *expected, const void *data, u32 len);
void func_0200b2f4(FSOverlayInfo *ov);
void func_0200b3e8(FSOverlayInfo *ov);
BOOL func_0200b4d8(FSOverlayInfo *ov);
void OS_Terminate(void);
void func_02000970(void *p);
void DC_FlushRange(void *p, u32 n);

BOOL func_0200afdc(FSOverlayInfo *out, int target, int id, FSArchive *arc, FSOvtHdr hdr)
{
    FSFile tmp;
    FSFileID fid;
    u32 off, size;
    u32 ofs = id << 5;
    if (target == 0) {
        off = hdr.off9;
        size = hdr.size9;
    } else {
        off = hdr.off7;
        size = hdr.size7;
    }
    if (size <= id * 0x20) return FALSE;
    func_0200a7b4(&tmp);
    if (!func_0200a9dc(&tmp, arc, off + ofs, off + size, -1)) return FALSE;
    if (func_0200ac30(&tmp, out, 0x20) != 0x20) {
        func_0200aadc(&tmp);
        return FALSE;
    }
    func_0200aadc(&tmp);
    out->target = target;
    fid = func_0200afb4(out);
    if (!func_0200aa24(&tmp, fid)) return FALSE;
    out->start = tmp.f24;
    out->length = tmp.f28 - tmp.f24;
    func_0200aadc(&tmp);
    return TRUE;
}

BOOL func_0200b0d4(FSOverlayInfo *out, int target, int id)
{
    FSOvtLoc *loc = target == 0 ? &data_020bfca4 : &data_020bfcac;
    FSFile tmp;
    FSFileID fid;
    if (loc->ptr != 0) {
        if (loc->size <= id * 0x20) return FALSE;
        MIi_CpuCopyFast((void *) (loc->ptr + id * 0x20), out, 0x20);
        out->target = target;
        func_0200a7b4(&tmp);
        fid = func_0200afb4(out);
        if (!func_0200aa24(&tmp, fid)) return FALSE;
        out->start = tmp.f24;
        out->length = tmp.f28 - tmp.f24;
        func_0200aadc(&tmp);
        return TRUE;
    }
    return func_0200afdc(out, target, id, &data_020bfcb4, *(FSOvtHdr *) 0x27ffe50);
}

BOOL func_0200b1d0(FSOverlayInfo *ov)
{
    FSFile tmp;
    FSFileID fid;
    u32 size, n;
    func_0200a7b4(&tmp);
    fid = func_0200afb4(ov);
    if (!func_0200aa24(&tmp, fid)) return FALSE;
    size = func_0200af5c(ov);
    func_0200af78(ov);
    n = func_0200ac30(&tmp, (void *) ov->ram_addr, size);
    if (size != n) {
        func_0200aadc(&tmp);
        return FALSE;
    }
    func_0200aadc(&tmp);
    return TRUE;
}

typedef struct { const u32 *key; u32 keylen; } FSHmacKey;
extern FSHmacKey data_020a4f44;

void MI_CpuCopy8(const void *src, void *dst, u32 n);

BOOL func_0200b260(const u32 *expected, const void *data, u32 len)
{
    u32 digest[5];
    u8 key[0x40];
    u32 i;
    u32 *p;
    MI_CpuFill8(digest, 0, 0x14);
    MI_CpuCopy8(data_020a4f44.key, key, data_020a4f44.keylen);
    func_0200b928((u8 *) digest, data, len, key, data_020a4f44.keylen);
    p = digest;
    for (i = 0; i < 0x14; i += 4) {
        if (*p++ != *(u32 *) ((u8 *) expected + i)) break;
    }
    return i == 0x14;
}

void func_0200b2f4(FSOverlayInfo *ov)
{
    u32 size = func_0200af5c(ov);
    void (**p)(void);
    void (**end)(void);
    if (*(u16 *) 0x027ffc40 == 2) {
        BOOL ok = FALSE;
        if ((ov->flags & 2) != 0) {
            u32 n = ((u32) data_020baef8_end - (u32) data_020baef8) / 0x14;
            if (ov->id < n) {
                ok = func_0200b260((u32 *) ((u8 *) data_020baef8 + ov->id * 0x14), (void *) ov->ram_addr, size);
            }
        }
        if (!ok) {
            MI_CpuFill8((void *) ov->ram_addr, 0, size);
            OS_Terminate();
            return;
        }
    }
    if ((ov->flags & 1) != 0) {
        func_02000970((void *) (ov->ram_addr + size));
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

void func_0200b3e8(FSOverlayInfo *ov)
{
    u32 base;
    u32 end;
    DtorNode *removed = NULL;
    DtorNode *tail = NULL;
    DtorNode *prev = NULL;
    DtorNode *head;
    DtorNode *n;
    DtorNode *next;
    u32 e;
    base = ov->ram_addr;
    end = base + (ov->ram_size + ov->bss_size);
    e = OS_DisableInterrupts();
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
    for (n = removed; n != NULL; n = next) {
        next = n->next;
        if (n->func != NULL) n->func(n->arg);
    }
}

BOOL func_0200b4d8(FSOverlayInfo *ov)
{
    func_0200b3e8(ov);
    return TRUE;
}

BOOL func_0200b4e8(int target, int id)
{
    FSOverlayInfo ov;
    if (!func_0200b0d4(&ov, target, id) || !func_0200b1d0(&ov)) return FALSE;
    func_0200b2f4(&ov);
    return TRUE;
}

BOOL func_0200b53c(int target, int id)
{
    FSOverlayInfo ov;
    if (!func_0200b0d4(&ov, target, id) || !func_0200b4d8(&ov)) return FALSE;
    return TRUE;
}
