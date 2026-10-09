#include <nitro/private/fs_dev.h>

static inline BOOL FS_IsBusy(FSArchive *a) { return FS_ArcTest(a, 0x200) ? TRUE : FALSE; }
int func_020093c8(FSFile *f, int cmd);
void func_0200936c(FSFile *f, int result);


u32 func_02009ef8(const u8 *s, int len);
void func_0200a7b4(FSFile *f);
BOOL func_0200a9dc(FSFile *f, FSArchive *arc, u32 start, u32 end, int id);
BOOL func_0200aadc(FSFile *f);
int func_0200ac30(FSFile *f, void *buf, int len);
FSFile *func_02009fa4(FSArchive *arc);
void func_0200a1b8(FSFile *f);
BOOL func_0200a24c(FSFile *f);
BOOL func_0200a290(FSFile *f, u32 cmd);
FSDirPos data_020bfc8c;      // data_020bfc8c
extern char data_020a4f3c[];
FSArchive *data_arc_list;   // data_020bfc88
int func_02009f8c(FSArchive *arc, void *dst, u32 pos, u32 len);
int func_02009f50(FSArchive *arc, void *dst, u32 pos, u32 len);
int func_02009f6c(FSArchive *arc, void *src, u32 pos, u32 len);
int func_02009558(const u8 *a, const u8 *b, u32 n)
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

int func_020095a8(FSTablePos *p, void *dst, u32 len)
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

int func_02009668(FSFile *f, u32 id)
{
    f->stat |= 4;
    f->f30 = (u32) f->arc;
    f->f38 = 0;
    FH(f, 0x36) = 0;
    FH(f, 0x34) = id;
    return func_020093c8(f, 2);
}

int func_0200969c(FSFile *f)
{
    u32 pos = f->f2c;
    u32 len = f->f38;
    FSArchive *a = f->arc;
    void *buf = (void *) f->f30;
    f->f2c = pos + len;
    return a->read_func(a, buf, pos, len);
}

int func_020096c8(FSFile *f)
{
    u32 pos = f->f2c;
    u32 len = f->f38;
    FSArchive *a = f->arc;
    void *buf = (void *) f->f30;
    f->f2c = pos + len;
    return a->write_func(a, buf, pos, len);
}

int func_020096f4(FSFile *f)
{
    FSArchive *arc = f->arc;
    struct { u32 a; u16 b; u16 c; } e;
    FSTablePos tp;
    FSDirPos *p = (FSDirPos *) &f->f30;
    int r;
    tp.arc = arc;
    tp.pos = arc->fnt + p->idx * 8;
    r = func_020095a8(&tp, &e, 8);
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

int func_0200978c(FSFile *f)
{
    FSDirEntry *ent = (FSDirEntry *) f->f30;
    u8 b;
    u16 id;
    FSTablePos tp;
    u32 pos;
    int r;
    tp.arc = f->arc;
    tp.pos = f->f28;
    r = func_020095a8(&tp, &b, 1);
    if (r != 0) return r;
    ent->name_len = b & 0x7f;
    ent->is_dir = (b >> 7) & 1;
    if (ent->name_len == 0) return 1;
    if (f->f34 == 0) {
        r = func_020095a8(&tp, ent->name, ent->name_len);
        if (r != 0) return r;
        ent->name[ent->name_len] = 0;
    } else {
        tp.pos += ent->name_len;
    }
    if (ent->is_dir != 0) {
        r = func_020095a8(&tp, &id, 2);
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

int func_020098b8(FSFile *f)
{
    FSDirEntry ent;
    const u8 *path = (const u8 *) f->f3c;
    u32 wantdir = f->f40;
    u32 c;
    func_020093c8(f, 2);
    c = *path;
    if (c != 0) {
        do {
            int len = 0;
            u32 term;
            while (IsPathNameChar(term = path[len])) {
                len++;
            }
            if (term != 0 || wantdir != 0) term = 1;
            if (len == 0) return 1;
            if (c == '.') {
                if (len == 1) {
                    path += 1;
                    goto next;
                }
                if ((len == 2) & (path[1] == '.')) {
                    if (FH(f, 0x24) != 0) func_02009668(f, f->f2c);
                    path += 2;
                    goto next;
                }
            }
            if (len > 0x7f) return 1;
            f->f30 = (u32) &ent;
            f->f34 = 0;
            do {
                if (func_020093c8(f, 3) != 0) return 1;
            } while (term != ent.is_dir || len != ent.name_len || func_02009558(path, (u8 *) ent.name, len) != 0);
            if (term != 0) {
                *(FSDirPos *) &f->f30 = *(FSDirPos *) &ent;
                func_020093c8(f, 2);
                path += len;
            } else {
                u32 *out;
                if (wantdir != 0) return 1;
                out = (u32 *) f->f44;
                out[0] = (u32) ent.arc;
                out[1] = ent.id;
                return 0;
            }
        next:
            path += (*path != 0);
            c = *path;
        } while (c != 0);
    }
    if (wantdir == 0) return 1;
    *(FSDirPos *) f->f44 = *(FSDirPos *) &f->f20;
    return 0;
}

int func_02009a9c(FSFile *f)
{
    FSArchive *arc = f->arc;
    FSFile tmp;
    FSDirEntry ent;
    u32 target;
    u32 dir;
    u32 i, ndirs;
    int len, r;
    u8 *buf;
    int pos;
    func_0200a7b4(&tmp);
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
                func_02009668(&tmp, i);
                if (i == 0) ndirs = tmp.f2c;
                tmp.f30 = (u32) &ent;
                tmp.f34 = 1;
                if (func_020093c8(&tmp, 3) == 0) {
                    do {
                        if (ent.is_dir == 0 && ent.id == target) {
                            dir = FH(&tmp, 0x24);
                            break;
                        }
                    } while (func_020093c8(&tmp, 3) == 0);
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
        u32 name = arc->name;
        if (name <= 0xff) len = 1;
        else if (name <= 0xff00) len = 2;
        else len = 3;
        len += 2;
        if (target != 0x10000) len += ent.name_len;
        i = dir;
        if (i != 0) {
            func_02009668(&tmp, dir);
            do {
                func_02009668(&tmp, tmp.f2c);
                tmp.f34 = 1;
                tmp.f30 = (u32) &ent;
                if (func_020093c8(&tmp, 3) == 0) {
                    do {
                        if (ent.is_dir != 0 && FH(&ent, 4) == i) {
                            len += ent.name_len + 1;
                            break;
                        }
                    } while (func_020093c8(&tmp, 3) == 0);
                }
                i = FH(&tmp, 0x24);
            } while (i != 0);
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
        if (name <= 0xff) len = 1;
        else if (name <= 0xff00) len = 2;
        else len = 3;
    }
    MIi_CpuCopyFast(arc, buf, len);
    MIi_CpuCopyFast(data_020a4f3c, buf + len, 2);
    func_02009668(&tmp, dir);
    if (target != 0x10000) {
        tmp.f30 = (u32) &ent;
        tmp.f34 = 0;
        if (func_020093c8(&tmp, 3) == 0) {
            do {
                if (ent.is_dir == 0 && ent.id == target) break;
            } while (func_020093c8(&tmp, 3) == 0);
        }
        r = ent.name_len + 1;
        MIi_CpuCopyFast(ent.name, buf + pos - r, r);
        pos -= r;
    } else {
        buf[pos - 1] = 0;
        pos -= 1;
    }
    while (dir != 0) {
        func_02009668(&tmp, tmp.f2c);
        buf[pos - 1] = '/';
        tmp.f30 = (u32) &ent;
        tmp.f34 = 0;
        pos--;
        if (func_020093c8(&tmp, 3) == 0) {
            do {
                if (ent.is_dir != 0 && FH(&ent, 4) == dir) {
                    dir = ent.name_len;
                    MIi_CpuCopyFast(ent.name, buf + pos - dir, dir);
                    pos -= dir;
                    break;
                }
            } while (func_020093c8(&tmp, 3) == 0);
        }
        dir = FH(&tmp, 0x24);
    }
    return 0;
}

int func_02009e48(FSFile *f)
{
    FSArchive *arc = f->arc;
    u32 idx = f->f34;
    u32 off = idx << 3;
    u32 ent[2];
    FSTablePos tp;
    int r;
    if (arc->fat_size <= idx * 8) return 1;
    tp.arc = arc;
    tp.pos = arc->fat + off;
    r = func_020095a8(&tp, ent, 8);
    if (r != 0) return r;
    f->f30 = ent[0];
    f->f34 = ent[1];
    f->f38 = idx;
    return func_020093c8(f, 7);
}

int func_02009ec8(FSFile *f)
{
    f->f24 = f->f30;
    f->f2c = f->f30;
    f->f28 = f->f34;
    f->f20 = f->f38;
    return 0;
}

int func_02009ef0(void)
{
    return 0;
}

u32 func_02009ef8(const u8 *s, int len)
{
    u32 v = 0;
    int i, sh;
    if (len <= 3 && len > 0) {
        i = 0;
        sh = 0;
        do {
            u32 c = s[i];
            if (c == 0) break;
            c -= 'A';
            if (c <= 25) c = c + 'a'; else c = c + 'A';
            i++;
            v |= c << sh;
            sh += 8;
        } while (i < len);
    }
    return v;
}

int func_02009f50(FSArchive *arc, void *dst, u32 pos, u32 len)
{
    MIi_CpuCopyFast((u8 *) arc->base + pos, dst, len);
    return 0;
}

int func_02009f6c(FSArchive *arc, void *src, u32 pos, u32 len)
{
    MIi_CpuCopyFast(src, (u8 *) arc->base + pos, len);
    return 0;
}

int func_02009f8c(FSArchive *arc, void *dst, u32 pos, u32 len)
{
    MIi_CpuCopyFast((void *) pos, dst, len);
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
                    func_0200936c(p, 3);
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
            func_0200a7b4(&tmp);
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

void func_0200a1b8(FSFile *f)
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
        if (func_020093c8(f, f->command) == 6) return;
        f = func_02009fa4(arc);
    } while (f != NULL);
}

BOOL func_0200a24c(FSFile *f)
{
    FSFile *n;
    func_0200936c(f, func_020093c8(f, f->command));
    n = func_02009fa4(f->arc);
    if (n != NULL) func_0200a1b8(n);
    return f->error == 0;
}

BOOL func_0200a290(FSFile *f, u32 cmd)
{
    FSArchive *arc = f->arc;
    u32 bit = 1 << cmd;
    u32 e;
    FSFile *next;
    FSFile *prev;
    FSFile *t;
    f->command = cmd;
    f->error = 2;
    f->stat |= 1;
    e = OS_DisableInterrupts();
    if (arc->flags & 0x80) {
        func_0200936c(f, 3);
        OS_RestoreInterrupts(e);
        return FALSE;
    }
    if (bit & 0x1fc) f->stat |= 4;
    prev = f->prev;
    next = f->next;
    if (prev != NULL) prev->next = next;
    if (next != NULL) next->prev = prev;
    t = (FSFile *) &arc->list_prev;
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
            func_0200a1b8(f);
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

void func_0200a434(FSArchive *arc)
{
    MI_CpuFill8(arc, 0, 0x5c);
    arc->q0.head = arc->q0.tail = NULL;
    arc->q1.head = arc->q1.tail = NULL;
}

FSArchive *func_0200a460(const char *name, int len)
{
    u32 n = func_02009ef8((const u8 *) name, len);
    u32 e = OS_DisableInterrupts();
    FSArchive *p = data_arc_list;
    while (p != NULL && p->name != n) {
        p = p->next;
    }
    OS_RestoreInterrupts(e);
    return p;
}

BOOL func_0200a4a0(FSArchive *arc, const char *name, int len)
{
    BOOL ok = FALSE;
    u32 e = OS_DisableInterrupts();
    if (func_0200a460(name, len) == NULL) {
        if (data_arc_list == NULL) {
            data_arc_list = arc;
            data_020bfc8c.arc = arc;
            data_020bfc8c.pos = 0;
            data_020bfc8c.sub = 0;
            data_020bfc8c.idx = 0;
        } else {
            FSArchive *t = data_arc_list;
            while (t->next != NULL) t = t->next;
            t->next = arc;
            arc->prev = t;
        }
        arc->name = func_02009ef8((const u8 *) name, len);
        arc->flags |= 1;
        ok = TRUE;
    }
    OS_RestoreInterrupts(e);
    return ok;
}

BOOL func_0200a554(FSArchive *arc, u32 base, u32 fat, u32 fat_size, u32 fnt, u32 fnt_size, FSReadFunc rd, FSReadFunc wr)
{
    arc->base = base;
    arc->fat_size = fat_size;
    arc->fat = arc->fat_bak = fat;
    arc->fnt_size = fnt_size;
    arc->fnt = arc->fnt_bak = fnt;
    arc->read_func = rd != NULL ? rd : func_02009f50;
    arc->write_func = wr != NULL ? wr : func_02009f6c;
    arc->read_cur = arc->read_func;
    arc->load_mem = 0;
    arc->flags |= 2;
    return TRUE;
}

u32 func_0200a5c4(FSArchive *arc, void *mem, u32 size)
{
    FSFile tmp;
    u32 need = (arc->fat_size + arc->fnt_size + 0x3f) & ~0x1f;
    u32 base;
    if (need <= size) {
        base = ((u32) mem + 0x1f) & ~0x1f;
        func_0200a7b4(&tmp);
        if (func_0200a9dc(&tmp, arc, arc->fat, arc->fat + arc->fat_size, -1)) {
            if (func_0200ac30(&tmp, (void *) base, arc->fat_size) < 0) {
                MI_CpuFill8((void *) base, 0, arc->fat_size);
            }
            func_0200aadc(&tmp);
        }
        arc->fat = base;
        base += arc->fat_size;
        if (func_0200a9dc(&tmp, arc, arc->fnt, arc->fnt + arc->fnt_size, -1)) {
            if (func_0200ac30(&tmp, (void *) base, arc->fnt_size) < 0) {
                MI_CpuFill8((void *) base, 0, arc->fnt_size);
            }
            func_0200aadc(&tmp);
        }
        arc->fnt = base;
        arc->load_mem = (u32) mem;
        arc->read_cur = func_02009f8c;
        arc->flags |= 4;
    }
    return need;
}

void func_0200a6e8(FSArchive *arc, void (*proc)(void *, int), u32 mask)
{
    if (mask == 0) {
        proc = NULL;
    } else if (proc == NULL) {
        mask = 0;
    }
    arc->proc = proc;
    arc->proc_mask = mask;
}

void func_0200a708(FSArchive *arc, int result)
{
    FSFile *p, *n;
    u32 e;
    if (FS_ArcTest(arc, 0x100)) {
        arc->flags &= ~0x100;
        func_0200936c(arc->list, result);
        n = func_02009fa4(arc);
        if (n != NULL) func_0200a1b8(n);
    } else {
        p = arc->list;
        e = OS_DisableInterrupts();
        p->error = result;
        arc->flags &= ~0x200;
        OS_WakeupThread(&arc->q0);
        OS_RestoreInterrupts(e);
    }
}
