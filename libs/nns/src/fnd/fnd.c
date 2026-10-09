// NitroSystem foundation: lists and heap heads. 0x02030880-0x02030b7c
typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;
typedef struct { void *prev; void *next; } NNSFndLink;
typedef struct { void *head; void *tail; u16 count; u16 offset; } NNSFndList;
typedef struct {
    u32 magic; NNSFndLink sibling; NNSFndList children;
    void *start; void *end; u32 opts;
} NNSFndHeapHead;
#define LNK(l,o) ((NNSFndLink*)((u8*)(o)+(l)->offset))

static inline u32 SetOpt(u32 attr, u16 opt) { return (attr & ~0xff) | (opt & 0xff); }
extern u32 data_02211364;
extern NNSFndList data_02211368;

void func_02030880(NNSFndList *list, u16 offset) {   // NNS_FndInitList
    list->head = 0; list->tail = 0; list->count = 0; list->offset = offset;
}
void func_02030898(NNSFndList *list, void *obj) {    // first-object insert
    NNSFndLink *l = LNK(list, obj);
    l->next = 0; l->prev = 0;
    list->head = obj; list->tail = obj;
    list->count++;
}
void func_020308c4(NNSFndList *list, void *obj) {    // NNS_FndAppendListObject
    if (list->head == 0) { func_02030898(list, obj); return; }
    {
        NNSFndLink *l = LNK(list, obj);
        l->prev = list->tail;
        l->next = 0;
        LNK(list, list->tail)->next = obj;
        list->tail = obj;
        list->count++;
    }
}
void func_02030918(NNSFndList *list, void *obj) {    // NNS_FndPrependListObject
    if (list->head == 0) { func_02030898(list, obj); return; }
    {
        NNSFndLink *l = LNK(list, obj);
        l->prev = 0;
        l->next = list->head;
        LNK(list, list->head)->prev = obj;
        list->head = obj;
        list->count++;
    }
}
void func_02030968(NNSFndList *list, void *target, void *obj) {   // NNS_FndInsertListObject
    if (target == 0) { func_020308c4(list, obj); return; }
    if (target == list->head) { func_02030918(list, obj); return; }
    {
        u16 off = list->offset;
        NNSFndLink *l = (NNSFndLink *)((u8 *)obj + off);
        NNSFndLink *pl;
        void *prev = *(void **)((u8 *)target + off);
        l->prev = prev;
        l->next = target;
        pl = (NNSFndLink *)((u8 *)prev + off);
        pl->next = obj;
        LNK(list, target)->prev = obj;
        list->count++;
    }
}
void func_020309cc(NNSFndList *list, void *obj) {    // NNS_FndRemoveListObject
    NNSFndLink *l = LNK(list, obj);
    if (l->prev == 0) list->head = l->next;
    else LNK(list, l->prev)->next = l->next;
    if (l->next == 0) list->tail = l->prev;
    else LNK(list, l->next)->prev = l->prev;
    l->prev = 0; l->next = 0;
    list->count--;
}
void *func_02030a2c(NNSFndList *list, void *obj) {   // NNS_FndGetNextListObject
    if (obj == 0) return list->head;
    return LNK(list, obj)->next;
}
void *func_02030a44(NNSFndList *list, void *obj) {   // NNS_FndGetPrevListObject
    if (obj == 0) return list->tail;
    return LNK(list, obj)->prev;
}
NNSFndHeapHead *func_02030a58(NNSFndList *list, void *ptr) {   // find innermost heap containing ptr
    NNSFndHeapHead *h = 0;
    while ((h = func_02030a2c(list, h)) != 0) {
        if ((u32)h->start <= (u32)ptr && (u32)ptr < (u32)h->end) {
            NNSFndHeapHead *c = func_02030a58(&h->children, ptr);
            return c ? c : h;
        }
    }
    return 0;
}
NNSFndList *func_02030ac0(void *ptr) {   // list that holds the heap containing ptr
    NNSFndList *list = &data_02211368;
    NNSFndHeapHead *h = func_02030a58(list, ptr);
    if (h) list = &h->children;
    return list;
}
void func_02030ae8(NNSFndHeapHead *h, u32 sig, void *start, void *end, u16 opt) {   // NNS_FndInitHeapHead
    u32 attr = 0;
    h->magic = sig;
    h->start = start;
    h->end = end;
    attr &= ~0xff; attr |= opt & 0xff;
    h->opts = attr;
    func_02030880(&h->children, 4);
    if (data_02211364 == 0) {
        func_02030880(&data_02211368, 4);
        data_02211364 = 1;
    }
    func_020308c4(func_02030ac0(h), h);
}
void func_02030b64(NNSFndHeapHead *h) {   // heap finalize: unlink from parent list
    func_020309cc(func_02030ac0(h), h);
}
