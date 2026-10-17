#include <nitro/private/fs_dev.h>

static inline BOOL FS_IsBusy(FSArchive *a) { return FS_ArcTest(a, 0x200) ? TRUE : FALSE; }
int RunFileCommand(FSFile *f, int cmd);
void FinishFileCommand(FSFile *f, int result);


u32 PackArchiveName(const u8 *s, int len);
void FS_InitFile(FSFile *f);
BOOL FS_OpenFileRange(FSFile *f, FSArchive *arc, u32 start, u32 end, int id);
BOOL FS_CloseFile(FSFile *f);
int FS_ReadFile(FSFile *f, void *buf, int len);
FSFile *func_02009fa4(FSArchive *arc);
void RunFileQueue(FSFile *f);
BOOL func_0200a24c(FSFile *f);
BOOL RequestFileCommand(FSFile *f, u32 cmd);
FSDirPos gCurrentDirPos;      // gCurrentDirPos
extern char data_020a4f3c[];
FSArchive *data_arc_list;   // data_020bfc88
int ReadFromAbsoluteAddress(FSArchive *arc, void *dst, u32 pos, u32 len);
int ReadFromArchiveBase(FSArchive *arc, void *dst, u32 pos, u32 len);
int WriteToArchiveBase(FSArchive *arc, void *src, u32 pos, u32 len);
int CompareIgnoringCase(const u8 *a, const u8 *b, u32 n)
{
    u32 i;
    for (i = 0; i < n; i++) {
        int x = a[i] - 'A';
        int y = b[i] - 'A';
        if ((u32) x <= 25) x += 32;
        if ((u32) y <= 25) y += 32;
        if (x != y) return x - y;
    }
    return 0;
}

int ReadTable(FSTablePos *p, void *dst, u32 len)
{
    FSArchive *arc = p->arc;
    int r;
    arc->flags |= 0x200;
    r = arc->read_cur(arc, dst, p->pos, len);
    switch (r) {
    case 0:
    case 1:
        arc->flags &= ~0x200;
        break;
    case 6: {
        u32 e = OS_DisableInterrupts();
        while (FS_ArcTest(arc, 0x200)) {
            OS_SleepThread(&arc->q0);
        }
        OS_RestoreInterrupts(e);
        r = arc->list->error;
        break;
    }
    }
    p->pos += len;
    return r;
}

int SeekDir(FSFile *f, u32 id)
{
    f->stat |= 4;
    f->f30 = (u32) f->arc;
    f->f38 = 0;
    FH(f, 0x36) = 0;
    FH(f, 0x34) = id;
    return RunFileCommand(f, 2);
}

int ReadCommand(FSFile *f)
{
    u32 pos = f->f2c;
    u32 len = f->f38;
    FSArchive *a = f->arc;
    void *buf = (void *) f->f30;
    f->f2c = pos + len;
    return a->read_func(a, buf, pos, len);
}

int WriteCommand(FSFile *f)
{
    u32 pos = f->f2c;
    u32 len = f->f38;
    FSArchive *a = f->arc;
    void *buf = (void *) f->f30;
    f->f2c = pos + len;
    return a->write_func(a, buf, pos, len);
}

int SeekDirCommand(FSFile *f)
{
    FSArchive *arc = f->arc;
    struct { u32 a; u16 b; u16 c; } e;
    FSTablePos tp;
    FSDirPos *p = (FSDirPos *) &f->f30;
    int r;
    tp.arc = arc;
    tp.pos = arc->fnt + p->idx * 8;
    r = ReadTable(&tp, &e, 8);
    if (r == 0) {
        *(FSDirPos *) &f->f20 = *p;
        if (p->sub == 0 && p->pos == 0) {
            FH(f, 0x26) = e.b;
            f->f28 = arc->fnt + e.a;
        }
        f->f2c = e.c & 0xfff;
    }
    return r;
}

int ReadDirEntryCommand(FSFile *f)
{
    FSDirEntry *ent = (FSDirEntry *) f->f30;
    u8 b;
    u16 id;
    FSTablePos tp;
    u32 pos;
    int r;
    tp.arc = f->arc;
    tp.pos = f->f28;
    r = ReadTable(&tp, &b, 1);
    if (r != 0) return r;
    ent->name_len = b & 0x7f;
    ent->is_dir = (b >> 7) & 1;
    if (ent->name_len == 0) return 1;
    if (f->f34 == 0) {
        r = ReadTable(&tp, ent->name, ent->name_len);
        if (r != 0) return r;
        ent->name[ent->name_len] = 0;
    } else {
        tp.pos += ent->name_len;
    }
    if (ent->is_dir != 0) {
        r = ReadTable(&tp, &id, 2);
        if (r != 0) return r;
        ent->arc = f->arc;
        FH(ent, 4) = id & 0xfff;
        FH(ent, 6) = 0;
        ent->pos = 0;
    } else {
        ent->arc = f->arc;
        ent->id = FH(f, 0x26);
        FH(f, 0x26)++;
    }
    f->f28 = tp.pos;
    return r;
}

static inline BOOL IsPathNameChar(u32 c)
{
    BOOL r = FALSE;
    if (c != 0 && c != '/' && c != '\\') r = TRUE;
    return r;
}

int FindPathCommand(FSFile *f)
{
    FSDirEntry ent;
    const u8 *path = (const u8 *) f->f3c;
    u32 wantdir = f->f40;
    RunFileCommand(f, 2);
    while (*path != 0) {
        int len = 0;
        u32 term;
        while (IsPathNameChar(term = path[len])) {
            len++;
        }
        if (term != 0 || wantdir != 0) term = 1;
        if (len == 0) return 1;
        if (*path == '.') {
            if (len == 1) {
                path += 1;
                goto next;
            }
            if ((len == 2) & (path[1] == '.')) {
                if (FH(f, 0x24) != 0) SeekDir(f, f->f2c);
                path += 2;
                goto next;
            }
        }
        if (len > 0x7f) return 1;
        *(FSDirEntry **) &f->f30 = &ent;
        f->f34 = 0;
        do {
            if (RunFileCommand(f, 3) != 0) return 1;
        } while (term != ent.is_dir || len != ent.name_len || CompareIgnoringCase(path, (u8 *) ent.name, len) != 0);
        if (term != 0) {
            *(FSDirPos *) &f->f30 = *(FSDirPos *) &ent;
            path += len;
            RunFileCommand(f, 2);
        } else {
            if (wantdir != 0) return 1;
            *(FSFileID *) f->f44 = *(FSFileID *) &ent;
            return 0;
        }
    next:
        path += (*path != 0);
    }
    if (wantdir == 0) return 1;
    *(FSDirPos *) f->f44 = *(FSDirPos *) &f->f20;
    return 0;
}

int GetPathCommand(FSFile *f)
{
    FSArchive *arc = f->arc;
    FSDirEntry ent;
    FSFile tmp;
    int pos;
    u8 *buf;
    u32 target;
    int r;
    u32 i;
    u32 ndirs;
    u32 j;
    int len;
    u32 dir;
    FS_InitFile(&tmp);
    tmp.arc = f->arc;
    if (FS_FileTest(f, 0x20)) {
        dir = FH(f, 0x24);
        target = 0x10000;
    } else {
        target = f->f20;
        if (FH(f, 0x38) != 0) {
            dir = FH(f, 0x3a);
        } else {
            i = 0;
            ndirs = 0;
            dir = 0x10000;
            do {
                SeekDir(&tmp, i);
                if (i == 0) ndirs = tmp.f2c;
                *(FSDirEntry **) &tmp.f30 = &ent;
                tmp.f34 = 1;
                if (RunFileCommand(&tmp, 3) == 0) {
                    do {
                        if (ent.is_dir == 0 && ent.id == target) {
                            dir = FH(&tmp, 0x24);
                            break;
                        }
                    } while (RunFileCommand(&tmp, 3) == 0);
                }
                if (dir != 0x10000) break;
                i++;
            } while (i < ndirs);
        }
    }
    if (dir == 0x10000) {
        FH(f, 0x38) = 0;
        return 1;
    }
    if (FH(f, 0x38) == 0) {
        u32 name;
        len = 0;
        name = arc->name;
        if (name <= 0xff) len += 1;
        else if (name <= 0xff00) len += 2;
        else len += 3;
        len += 2;
        if (target != 0x10000) len += ent.name_len;
        j = dir;
        if (j != 0) {
            SeekDir(&tmp, dir);
            do {
                SeekDir(&tmp, tmp.f2c);
                *(FSDirEntry **) &tmp.f30 = &ent;
                tmp.f34 = 1;
                if (RunFileCommand(&tmp, 3) == 0) {
                    do {
                        if (ent.is_dir != 0 && FH(&ent, 4) == j) {
                            len += ent.name_len + 1;
                            break;
                        }
                    } while (RunFileCommand(&tmp, 3) == 0);
                }
                j = FH(&tmp, 0x24);
            } while (j != 0);
        }
        FH(f, 0x38) = len + 1;
        FH(f, 0x3a) = dir;
    }
    buf = (u8 *) f->f30;
    if (buf == NULL) return 0;
    pos = FH(f, 0x38);
    if (f->f34 < pos) return 1;
    {
        u32 name = arc->name;
        u32 off = 0;
        if (name <= 0xff) len = 1;
        else if (name <= 0xff00) len = 2;
        else len = 3;
        MI_CpuCopy8(arc, buf, len);
        off += len;
        MI_CpuCopy8(data_020a4f3c, buf + off, 2);
    }
    SeekDir(&tmp, dir);
    if (target != 0x10000) {
        *(FSDirEntry **) &tmp.f30 = &ent;
        tmp.f34 = 0;
        if (RunFileCommand(&tmp, 3) == 0) {
            do {
                if (ent.is_dir == 0 && ent.id == target) break;
            } while (RunFileCommand(&tmp, 3) == 0);
        }
        r = ent.name_len + 1;
        MI_CpuCopy8(ent.name, buf + pos - r, r);
        pos -= r;
    } else {
        *(buf + pos - 1) = 0;
        pos -= 1;
    }
    while (dir != 0) {
        SeekDir(&tmp, tmp.f2c);
        *(FSDirEntry **) &tmp.f30 = &ent;
        tmp.f34 = 0;
        *(buf + pos - 1) = '/';
        pos--;
        if (RunFileCommand(&tmp, 3) == 0) {
            do {
                if (ent.is_dir != 0 && FH(&ent, 4) == dir) {
                    dir = ent.name_len;
                    MI_CpuCopy8(ent.name, buf + pos - dir, dir);
                    pos -= dir;
                    break;
                }
            } while (RunFileCommand(&tmp, 3) == 0);
        }
        dir = FH(&tmp, 0x24);
    }
    return 0;
}

int OpenFileByIdCommand(FSFile *f)
{
    FSArchive *arc = f->arc;
    u32 idx = f->f34;
    u32 pos = idx * 8;
    u32 ent[2];
    FSTablePos tp;
    int r;
    if (pos >= arc->fat_size) return 1;
    tp.arc = arc;
    tp.pos = arc->fat + pos;
    r = ReadTable(&tp, ent, 8);
    if (r != 0) return r;
    f->f30 = ent[0];
    f->f34 = ent[1];
    f->f38 = idx;
    return RunFileCommand(f, 7);
}

int OpenFileRangeCommand(FSFile *f)
{
    f->f24 = f->f30;
    f->f2c = f->f30;
    f->f28 = f->f34;
    f->f20 = f->f38;
    return 0;
}

int CloseFileCommand(void)
{
    return 0;
}

u32 PackArchiveName(const u8 *s, int len)
{
    u32 v = 0;
    int i, sh;
    if (len <= 3) {
      i = 0;
      if (len > 0) {
        sh = 0;
        do {
            u32 c = s[i];
            u32 d;
            if (c == 0) break;
            d = c - 'A';
            if (d <= 25) c = d + 'a'; else c = d + 'A';
            i++;
            v |= c << sh;
            sh += 8;
        } while (i < len);
    }
    }
    return v;
}

int ReadFromArchiveBase(FSArchive *arc, void *dst, u32 pos, u32 len)
{
    MI_CpuCopy8((u8 *) arc->base + pos, dst, len);
    return 0;
}

int WriteToArchiveBase(FSArchive *arc, void *src, u32 pos, u32 len)
{
    MI_CpuCopy8(src, (u8 *) arc->base + pos, len);
    return 0;
}

int ReadFromAbsoluteAddress(FSArchive *arc, void *dst, u32 pos, u32 len)
{
    MI_CpuCopy8((void *) pos, dst, len);
    return 0;
}

FSFile *func_02009fa4(FSArchive *arc)
{
    u32 e = OS_DisableInterrupts();
    FSFile *p, *next;
    if (FS_ArcTest(arc, 0x20)) {
        arc->flags &= ~0x20;
        p = arc->list;
        if (p != NULL) {
            do {
                next = p->next;
                if (FS_FileTest(p, 2)) {
                    if (arc->list == p) arc->list = next;
                    FinishFileCommand(p, 3);
                    if (next == NULL) next = arc->list;
                }
                p = next;
            } while (p != NULL);
        }
    }
    if (!FS_ArcTest(arc, 0x40) && !FS_ArcTest(arc, 0x8) && (p = arc->list) != NULL) {
        BOOL first = !FS_ArcTest(arc, 0x10);
        if (first) arc->flags |= 0x10;
        OS_RestoreInterrupts(e);
        if (first && (arc->proc_mask & 0x200)) {
            arc->proc(p, 9);
        }
        e = OS_DisableInterrupts();
        p->stat |= 0x40;
        if (FS_FileTest(p, 4)) {
            OS_WakeupThread(&p->queue);
            OS_RestoreInterrupts(e);
            return NULL;
        }
        OS_RestoreInterrupts(e);
        return p;
    }
    if (FS_ArcTest(arc, 0x10)) {
        arc->flags &= ~0x10;
        if (arc->proc_mask & 0x400) {
            FSFile tmp;
            FS_InitFile(&tmp);
            tmp.arc = arc;
            arc->proc(&tmp, 10);
        }
    }
    if (FS_ArcTest(arc, 0x40)) {
        arc->flags = (arc->flags & ~0x40) | 0x8;
        OS_WakeupThread(&arc->q1);
    }
    OS_RestoreInterrupts(e);
    return NULL;
}

void RunFileQueue(FSFile *f)
{
    FSArchive *arc = f->arc;
    u32 e;
    if (f == NULL) return;
    do {
        e = OS_DisableInterrupts();
        f->stat |= 0x40;
        if (FS_FileTest(f, 4)) {
            OS_WakeupThread(&f->queue);
            OS_RestoreInterrupts(e);
            return;
        }
        f->stat |= 8;
        OS_RestoreInterrupts(e);
        if (RunFileCommand(f, f->command) == 6) return;
        f = func_02009fa4(arc);
    } while (f != NULL);
}

BOOL func_0200a24c(FSFile *f)
{
    FSFile *n;
    FinishFileCommand(f, RunFileCommand(f, f->command));
    n = func_02009fa4(f->arc);
    if (n != NULL) RunFileQueue(n);
    return f->error == 0;
}

BOOL RequestFileCommand(FSFile *f, u32 cmd)
{
    FSArchive *arc = f->arc;
    u32 bit = 1 << cmd;
    u32 e;
    FSFile *t;
    FSFile *prev;
    FSFile *next;
    f->command = cmd;
    f->error = 2;
    f->stat |= 1;
    e = OS_DisableInterrupts();
    if (arc->flags & 0x80) {
        FinishFileCommand(f, 3);
        OS_RestoreInterrupts(e);
        return FALSE;
    }
    if (bit & 0x1fc) f->stat |= 4;
    t = (FSFile *) &arc->list_prev;
    prev = f->prev;
    next = f->next;
    if (prev != NULL) prev->next = next;
    if (next != NULL) next->prev = prev;
    while (t->next != NULL) t = t->next;
    t->next = f;
    f->prev = t;
    f->next = NULL;
    if (!FS_ArcTest(arc, 8) && !FS_ArcTest(arc, 0x10)) {
        arc->flags |= 0x10;
        OS_RestoreInterrupts(e);
        if (arc->proc_mask & 0x200) {
            arc->proc(f, 9);
        }
        e = OS_DisableInterrupts();
        f->stat |= 0x40;
        if (!FS_FileTest(f, 4)) {
            OS_RestoreInterrupts(e);
            RunFileQueue(f);
            return TRUE;
        }
        OS_RestoreInterrupts(e);
    } else {
        if (!FS_FileTest(f, 4)) {
            OS_RestoreInterrupts(e);
            return TRUE;
        }
        do {
            OS_SleepThread(&f->queue);
        } while (!(f->stat & 0x40));
        OS_RestoreInterrupts(e);
    }
    return func_0200a24c(f);
}

void FS_InitArchive(FSArchive *arc)
{
    MI_CpuFill8(arc, 0, 0x5c);
    arc->q0.head = arc->q0.tail = NULL;
    arc->q1.head = arc->q1.tail = NULL;
}

FSArchive *FS_FindArchive(const char *name, int len)
{
    u32 n = PackArchiveName((const u8 *) name, len);
    u32 e = OS_DisableInterrupts();
    FSArchive *p = data_arc_list;
    while (p != NULL && p->name != n) {
        p = p->next;
    }
    OS_RestoreInterrupts(e);
    return p;
}

BOOL FS_RegisterArchiveName(FSArchive *arc, const char *name, int len)
{
    BOOL ok = FALSE;
    u32 e = OS_DisableInterrupts();
    if (FS_FindArchive(name, len) == NULL) {
        if (data_arc_list == NULL) {
            data_arc_list = arc;
            gCurrentDirPos.arc = arc;
            gCurrentDirPos.pos = 0;
            gCurrentDirPos.sub = 0;
            gCurrentDirPos.idx = 0;
        } else {
            FSArchive *t = data_arc_list;
            while (t->next != NULL) t = t->next;
            t->next = arc;
            arc->prev = t;
        }
        arc->name = PackArchiveName((const u8 *) name, len);
        arc->flags |= 1;
        ok = TRUE;
    }
    OS_RestoreInterrupts(e);
    return ok;
}

BOOL FS_SetupArchive(FSArchive *arc, u32 base, u32 fat, u32 fat_size, u32 fnt, u32 fnt_size, FSReadFunc rd, FSReadFunc wr)
{
    arc->base = base;
    arc->fat_size = fat_size;
    arc->fat = arc->fat_bak = fat;
    arc->fnt_size = fnt_size;
    arc->fnt = arc->fnt_bak = fnt;
    arc->read_func = rd != NULL ? rd : ReadFromArchiveBase;
    arc->write_func = wr != NULL ? wr : WriteToArchiveBase;
    arc->read_cur = arc->read_func;
    arc->load_mem = 0;
    arc->flags |= 2;
    return TRUE;
}

u32 FS_LoadArchiveTables(FSArchive *arc, void *mem, u32 size)
{
    FSFile tmp;
    u32 need = (arc->fat_size + arc->fnt_size + 0x3f) & ~0x1f;
    u32 base;
    if (need <= size) {
        base = ((u32) mem + 0x1f) & ~0x1f;
        FS_InitFile(&tmp);
        if (FS_OpenFileRange(&tmp, arc, arc->fat, arc->fat + arc->fat_size, -1)) {
            if (FS_ReadFile(&tmp, (void *) base, arc->fat_size) < 0) {
                MI_CpuFill8((void *) base, 0, arc->fat_size);
            }
            FS_CloseFile(&tmp);
        }
        arc->fat = base;
        base += arc->fat_size;
        if (FS_OpenFileRange(&tmp, arc, arc->fnt, arc->fnt + arc->fnt_size, -1)) {
            if (FS_ReadFile(&tmp, (void *) base, arc->fnt_size) < 0) {
                MI_CpuFill8((void *) base, 0, arc->fnt_size);
            }
            FS_CloseFile(&tmp);
        }
        arc->fnt = base;
        arc->load_mem = (u32) mem;
        arc->read_cur = ReadFromAbsoluteAddress;
        arc->flags |= 4;
    }
    return need;
}

void FS_SetArchiveProc(FSArchive *arc, void (*proc)(void *, int), u32 mask)
{
    if (mask == 0) {
        proc = NULL;
    } else if (proc == NULL) {
        mask = 0;
    }
    arc->proc = proc;
    arc->proc_mask = mask;
}

void FS_FinishArchiveCommand(FSArchive *arc, int result)
{
    FSFile *p, *n;
    u32 e;
    if (FS_ArcTest(arc, 0x100)) {
        FSFile *l = arc->list;
        arc->flags &= ~0x100;
        FinishFileCommand(l, result);
        n = func_02009fa4(arc);
        if (n != NULL) RunFileQueue(n);
    } else {
        p = arc->list;
        e = OS_DisableInterrupts();
        p->error = result;
        arc->flags &= ~0x200;
        OS_WakeupThread(&arc->q0);
        OS_RestoreInterrupts(e);
    }
}
