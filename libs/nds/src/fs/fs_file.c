#include <nitro/private/fs_dev.h>

int RunFileCommand(FSFile *f, int cmd);
void FinishFileCommand(FSFile *f, int result);
int SeekDir(FSFile *f, u32 id);
u32 PackArchiveName(const u8 *s, int len);
int ReadFromArchiveBase(FSArchive *arc, void *dst, u32 pos, u32 len);
int WriteToArchiveBase(FSArchive *arc, void *src, u32 pos, u32 len);
int ReadFromAbsoluteAddress(FSArchive *arc, void *dst, u32 pos, u32 len);
void FS_InitFile(FSFile *f);
int FS_OpenFileRange(FSFile *f, FSArchive *arc, u32 a, u32 b, int c);


extern FSDirPos gCurrentDirPos;      // gCurrentDirPos
extern FSArchive *data_arc_list;   // data_020bfc88
static int fs_initialized;   // data_020bfc98
FSArchive *FS_FindArchive(const char *name, int len);
BOOL RequestFileCommand(FSFile *f, u32 cmd);
BOOL func_0200a24c(FSFile *f);
FSFile *func_02009fa4(FSArchive *arc);
void RunFileQueue(FSFile *f);
void FS_Init(u32 a);
int ReadFromAbsoluteAddress(FSArchive *arc, void *dst, u32 pos, u32 len);
BOOL FS_WaitFile(FSFile *f);
BOOL FS_CloseFile(FSFile *f);
int FS_ReadFile(FSFile *f, void *buf, int len);
BOOL FS_OpenFileRange(FSFile *f, FSArchive *arc, u32 start, u32 end, int id);
void FS_InitFile(FSFile *f);
void FS_InitOnce(u32 a)
{
    if (fs_initialized) return;
    fs_initialized = 1;
    FS_Init(a);
}

void FS_InitFile(FSFile *f)
{
    f->prev = NULL;
    f->next = NULL;
    f->queue.head = f->queue.tail = NULL;
    f->arc = NULL;
    f->command = 0xe;
    f->stat = 0;
}

BOOL FS_FindPath(FSFile *f, const u8 *path, u32 *idout, u32 *dirout)
{
    FSDirPos pos;
    int i;
    if (path[0] == '/' || path[0] == '\\') {
        pos.idx = 0;
        pos.arc = gCurrentDirPos.arc;
        pos.pos = 0;
        pos.sub = 0;
        path++;
    } else {
        pos = gCurrentDirPos;
        for (i = 0; i <= 3; i++) {
            u32 c = path[i];
            if (c == 0 || c == '/' || c == '\\') break;
            if (c == ':') {
                FSArchive *a = FS_FindArchive((const char *) path, i);
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
    return RequestFileCommand(f, 4);
}

int ReadFileCore(FSFile *f, void *buf, int len, BOOL async)
{
    u32 pos = f->f2c;
    int remain = f->f28 - pos;
    int org = len;
    f->f30 = (u32) buf;
    if (len > remain) len = remain;
    if (len < 0) len = 0;
    f->f34 = org;
    f->f38 = len;
    if (!async) f->stat |= 4;
    RequestFileCommand(f, 0);
    if (!async) {
        if (FS_WaitFile(f)) { len = f->f2c - pos; } else { len = -1; }
    }
    return len;
}

BOOL FS_ConvertPathToFileID(FSFileID *id, const char *path)
{
    FSFile f;
    FS_InitFile(&f);
    return FS_FindPath(&f, (const u8 *) path, (u32 *) id, NULL) ? TRUE : FALSE;
}

BOOL FS_OpenFileRange(FSFile *f, FSArchive *arc, u32 start, u32 end, int id)
{
    f->arc = arc;
    f->f38 = id;
    f->f30 = start;
    f->f34 = end;
    if (!RequestFileCommand(f, 7)) return FALSE;
    f->stat = (f->stat | 0x10) & ~0x20;
    return TRUE;
}

BOOL FS_OpenFileById(FSFile *f, volatile FSFileID id)
{
    FSArchive *arc = id.arc;
    if (arc == NULL) return FALSE;
    f->arc = arc;
    f->f30 = (u32) arc;
    f->f34 = id.file_id;
    if (!RequestFileCommand(f, 6)) return FALSE;
    f->stat = (f->stat | 0x10) & ~0x20;
    return TRUE;
}

BOOL FS_OpenFile(FSFile *f, const char *path)
{
    FSFileID id;
    if (FS_ConvertPathToFileID(&id, path) && FS_OpenFileById(f, id)) return TRUE;
    return FALSE;
}

BOOL FS_CloseFile(FSFile *f)
{
    if (!RequestFileCommand(f, 8)) return FALSE;
    f->arc = NULL;
    f->command = 0xe;
    f->stat &= ~0x30;
    return TRUE;
}

BOOL FS_WaitFile(FSFile *f)
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

void FS_CancelFile(FSFile *f)
{
    u32 e = OS_DisableInterrupts();
    if (FS_FileTest(f, 1)) {
        f->stat |= 2;
        f->arc->flags |= 0x20;
    }
    OS_RestoreInterrupts(e);
}

int FS_ReadFile(FSFile *f, void *buf, int len);
BOOL FS_CloseFile(FSFile *f);
BOOL FS_WaitFile(FSFile *f);
void FS_InitFile(FSFile *f);
void FS_InitFile(FSFile *f);

int FS_ReadFileAsync(FSFile *f, void *buf, int len)
{
    return ReadFileCore(f, buf, len, 1);
}

int FS_ReadFile(FSFile *f, void *buf, int len)
{
    return ReadFileCore(f, buf, len, 0);
}

BOOL FS_SeekFile(FSFile *f, int offset, int origin)
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

BOOL FS_ChangeDir(const char *path)
{
    FSFile tmp;
    FSDirPos pos;
    FS_InitFile(&tmp);
    if (!FS_FindPath(&tmp, (const u8 *) path, NULL, (u32 *) &pos)) return FALSE;
    gCurrentDirPos = pos;
    return TRUE;
}
