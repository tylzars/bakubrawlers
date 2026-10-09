#include <nitro/private/fs_dev.h>

int func_020093c8(FSFile *f, int cmd);
void func_0200936c(FSFile *f, int result);
int func_02009668(FSFile *f, u32 id);
u32 func_02009ef8(const u8 *s, int len);
int func_02009f50(FSArchive *arc, void *dst, u32 pos, u32 len);
int func_02009f6c(FSArchive *arc, void *src, u32 pos, u32 len);
int func_02009f8c(FSArchive *arc, void *dst, u32 pos, u32 len);
void func_0200a7b4(FSFile *f);
int func_0200a9dc(FSFile *f, FSArchive *arc, u32 a, u32 b, int c);


extern FSDirPos data_020bfc8c;      // data_020bfc8c
extern FSArchive *data_arc_list;   // data_020bfc88
static int fs_initialized;   // data_020bfc98
FSArchive *func_0200a460(const char *name, int len);
BOOL func_0200a290(FSFile *f, u32 cmd);
BOOL func_0200a24c(FSFile *f);
FSFile *func_02009fa4(FSArchive *arc);
void func_0200a1b8(FSFile *f);
void func_0200adf4(u32 a);
int func_02009f8c(FSArchive *arc, void *dst, u32 pos, u32 len);
BOOL func_0200ab18(FSFile *f);
BOOL func_0200aadc(FSFile *f);
int func_0200ac30(FSFile *f, void *buf, int len);
BOOL func_0200a9dc(FSFile *f, FSArchive *arc, u32 start, u32 end, int id);
void func_0200a7b4(FSFile *f);
void func_0200a78c(u32 a)
{
    if (fs_initialized) return;
    fs_initialized = 1;
    func_0200adf4(a);
}

void func_0200a7b4(FSFile *f)
{
    f->prev = NULL;
    f->next = NULL;
    f->queue.head = f->queue.tail = NULL;
    f->arc = NULL;
    f->command = 0xe;
    f->stat = 0;
}

BOOL func_0200a7dc(FSFile *f, const u8 *path, u32 *idout, u32 *dirout)
{
    FSDirPos pos;
    int i;
    if (path[0] == '/' || path[0] == '\\') {
        pos.idx = 0;
        pos.arc = data_020bfc8c.arc;
        pos.pos = 0;
        pos.sub = 0;
        path++;
    } else {
        pos = data_020bfc8c;
        for (i = 0; i <= 3; i++) {
            u32 c = path[i];
            if (c == 0 || c == '/' || c == '\\') break;
            if (c == ':') {
                FSArchive *a = func_0200a460((const char *) path, i);
                if (a == NULL) return FALSE;
                if (!FS_ArcTest(a, 2)) return FALSE;
                pos.arc = a;
                pos.pos = 0;
                pos.sub = 0;
                pos.idx = 0;
                path += i + 1;
                if (*path == '/' || *path == '\\') path++;
                break;
            }
        }
    }
    f->arc = pos.arc;
    f->f3c = (u32) path;
    *(FSDirPos *) &f->f30 = pos;
    if (dirout != NULL) {
        f->f40 = 1;
        f->f44 = (u32) dirout;
    } else {
        f->f40 = 0;
        f->f44 = (u32) idout;
    }
    return func_0200a290(f, 4);
}

int func_0200a920(FSFile *f, void *buf, int len, BOOL async)
{
    u32 pos = f->f2c;
    int n = len;
    int remain = f->f28 - pos;
    f->f30 = (u32) buf;
    if (n > remain) n = remain;
    if (n < 0) n = 0;
    f->f34 = len;
    f->f38 = n;
    if (!async) f->stat |= 4;
    func_0200a290(f, 0);
    if (!async) {
        if (func_0200ab18(f)) {
            n = f->f2c - pos;
        } else {
            n = -1;
        }
    }
    return n;
}

BOOL FS_ConvertPathToFileID(FSFileID *id, const char *path)
{
    FSFile f;
    func_0200a7b4(&f);
    return func_0200a7dc(&f, (const u8 *) path, (u32 *) id, NULL) ? TRUE : FALSE;
}

BOOL func_0200a9dc(FSFile *f, FSArchive *arc, u32 start, u32 end, int id)
{
    f->arc = arc;
    f->f38 = id;
    f->f30 = start;
    f->f34 = end;
    if (!func_0200a290(f, 7)) return FALSE;
    f->stat = (f->stat | 0x10) & ~0x20;
    return TRUE;
}

BOOL func_0200aa24(FSFile *f, FSFileID id)
{
    if (id.arc == NULL) return FALSE;
    f->arc = id.arc;
    f->f30 = (u32) id.arc;
    f->f34 = id.file_id;
    if (!func_0200a290(f, 6)) return FALSE;
    f->stat = (f->stat | 0x10) & ~0x20;
    return TRUE;
}

BOOL func_0200aa94(FSFile *f, const char *path)
{
    FSFileID id;
    if (FS_ConvertPathToFileID(&id, path) && func_0200aa24(f, id)) return TRUE;
    return FALSE;
}

BOOL func_0200aadc(FSFile *f)
{
    if (!func_0200a290(f, 8)) return FALSE;
    f->arc = NULL;
    f->command = 0xe;
    f->stat &= ~0x30;
    return TRUE;
}

BOOL func_0200ab18(FSFile *f)
{
    BOOL sync = FALSE;
    u32 e = OS_DisableInterrupts();
    if (FS_FileTest(f, 1)) {
        sync = !(f->stat & 0x44);
        if (sync) {
            f->stat |= 4;
            do {
                OS_SleepThread(&f->queue);
            } while (!(f->stat & 0x40));
        } else {
            do {
                OS_SleepThread(&f->queue);
            } while (FS_FileTest(f, 1));
        }
    }
    OS_RestoreInterrupts(e);
    if (sync) return func_0200a24c(f);
    return f->error == 0;
}

void func_0200abd8(FSFile *f)
{
    u32 e = OS_DisableInterrupts();
    if (FS_FileTest(f, 1)) {
        f->stat |= 2;
        f->arc->flags |= 0x20;
    }
    OS_RestoreInterrupts(e);
}

int func_0200ac30(FSFile *f, void *buf, int len);
BOOL func_0200aadc(FSFile *f);
BOOL func_0200ab18(FSFile *f);
void func_0200a7b4(FSFile *f);
void func_0200a7b4(FSFile *f);

int func_0200ac20(FSFile *f, void *buf, int len)
{
    return func_0200a920(f, buf, len, 1);
}

int func_0200ac30(FSFile *f, void *buf, int len)
{
    return func_0200a920(f, buf, len, 0);
}

BOOL func_0200ac40(FSFile *f, int offset, int origin)
{
    switch (origin) {
    case 0: offset += f->f24; break;
    case 1: offset += f->f2c; break;
    case 2: offset += f->f28; break;
    default: return FALSE;
    }
    if (offset < (int) f->f24) offset = f->f24;
    if (offset > (int) f->f28) offset = f->f28;
    f->f2c = offset;
    return TRUE;
}

BOOL func_0200acac(const char *path)
{
    FSFile tmp;
    FSDirPos pos;
    func_0200a7b4(&tmp);
    if (!func_0200a7dc(&tmp, (const u8 *) path, NULL, (u32 *) &pos)) return FALSE;
    data_020bfc8c = pos;
    return TRUE;
}
