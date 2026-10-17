// NitroSystem Gfd: frame-texture VRAM state save/restore and the link-list VRAM region manager (free-list helpers).
// main 0x020360c4-0x020364a8
typedef unsigned int u32;
typedef struct { u32 start; u32 end; u32 enabled; u32 mapped; u32 pad[2]; } TexSlot;   // as gfd_vram.c (unused here)
typedef struct Node { u32 start; u32 size; struct Node *prev; struct Node *next; } Node;
typedef struct { Node *head; } Man;
typedef struct { Node *head; } Pool;

int func_020360c4(u32 key) { return 0; }

typedef struct { u32 a, b, c[4]; } Ent;   // data_020a58ac seen as here: a / b are TexSlot start / end
extern Ent data_020a58ac[5];
void SaveTexSlotRanges(u32 *out) {
    int i;
    for (i = 0; i < 5; i++) {
        out[i * 2] = data_020a58ac[i].a;
        out[i * 2 + 1] = data_020a58ac[i].b;
    }
}

void RestoreTexSlotRanges(u32 *in) {
    int i;
    for (i = 0; i < 5; i++) {
        data_020a58ac[i].a = in[i * 2];
        data_020a58ac[i].b = in[i * 2 + 1];
    }
}

#define UNLINK(man, n) \
    { Node *nx; Node *pv; pv = (n)->prev; nx = (n)->next; \
      if (pv) pv->next = nx; else (man)->head = nx; \
      if (nx) nx->prev = pv; }
#define PUSH(list, n) \
    { if ((list)->head) (list)->head->prev = (n); (n)->next = (list)->head; (n)->prev = 0; (list)->head = (n); }

int MergeAdjacentFreeRegions(Man *man, Pool *pool, u32 *range) {
    Node *n = man->head;
    Node *next;
    int ret = 0;
    while (n) {
        next = n->next;
        if (n->start == range[1]) {
            ret |= 1;
            range[1] = n->start + n->size;
            UNLINK(man, n);
            PUSH(pool, n);
        }
        if (range[0] == n->start + n->size) {
            range[0] = n->start;
            UNLINK(man, n);
            ret |= 1;
            PUSH(pool, n);
        }
        n = next;
    }
    return ret;
}

void ClearFreeRegions(Man *man) { man->head = 0; }

Node *ChainNodes(Node *mem, u32 n) {
    u32 i;
    Node *prev;
    for (i = 0; i < n - 1; i++) {
        mem[i].next = &mem[i + 1];
        mem[i + 1].prev = &mem[i];
    }
    mem[0].prev = 0;
    (mem + n)[-1].next = 0;
    return mem;
}

int AddFreeRegion(Man *man, Pool *pool, u32 start, u32 size) {
    Node *n = pool->head;
    if (n) pool->head = n->next;
    if (n != 0) {
        n->start = start;
        n->size = size;
        n->prev = 0;
        n->next = 0;
        PUSH(man, n);
        return 1;
    }
    return 0;
}

int AllocFromFreeRegions(Man *man, Pool *pool, u32 *out, u32 size, u32 align) {
    u32 start;
    u32 need;
    u32 pad;
    Node *best = 0;
    Node *n = man->head;
    for (; n; n = n->next) {
        if (align > 1) {
            start = (n->start + (align - 1)) & ~(align - 1);
            pad = start - n->start;
            need = size + pad;
        } else {
            start = n->start;
            pad = 0;
            need = size;
        }
        if (n->size >= need) {
            best = n;
            break;
        }
    }
    if (best == 0) goto fail;
    if (pad != 0) {
        Node *a = pool->head;
        if (a) pool->head = a->next;
        if (a == 0) goto fail;
        a->start = best->start;
        a->size = pad;
        a->prev = 0;
        a->next = 0;
        PUSH(man, a);
    }
    best->size -= need;
    best->start += need;
    if (best->size == 0) {
        UNLINK(man, best);
        PUSH(pool, best);
    }
    *out = start;
    return 1;
fail:
    *out = 0;
    return 0;
}

void MergeAllFreeRegions(Man *man, Pool *pool) {
    u32 r[2];
    Node *n = man->head;
    if (n == 0) return;
    do {
        r[0] = n->start;
        r[1] = n->start + n->size;
        if (MergeAdjacentFreeRegions(man, pool, r) == 0) {
            n = n->next;
        } else {
            n->start = r[0];
            n->size = r[1] - r[0];
            n = man->head;
        }
    } while (n);
}

int MergeAndAddFreeRegion(Man *man, Pool *pool, u32 start, u32 size) {
    u32 r[2];
    Node *n;
    r[0] = start;
    r[1] = start + size;
    MergeAdjacentFreeRegions(man, pool, r);
    n = pool->head;
    if (n) pool->head = n->next;
    if (n == 0) return 0;
    n->start = r[0];
    n->size = r[1] - r[0];
    n->prev = 0;
    n->next = 0;
    PUSH(man, n);
    return 1;
}
