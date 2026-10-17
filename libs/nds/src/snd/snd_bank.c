#include <nitro/private/snd_h.h>

typedef struct SNDLink SNDLink;
typedef struct SNDWaveArc SNDWaveArc;

struct SNDLink {
    SNDWaveArc *arc;   // 0x00
    SNDLink *next;     // 0x04
};

struct SNDWaveArc {
    u8 pad[0x18];
    SNDLink *list;     // 0x18
};

typedef struct {
    u8 pad[0x18];
    SNDLink link[4];   // 0x18
    u32 count;         // 0x38
    u32 inst[1];       // 0x3c
} SNDBank;

typedef struct {
    u32 index;
    u32 sub;
} SNDInstPos;

void LinkBankToWaveArc(SNDBank *bank, int idx, SNDWaveArc *arc) {
    SNDWaveArc *cur;
    SNDLink *self;
    SNDLink *p;
    SNDLink *q;
    SNDLink *n;
    SND_LockMutex();
    cur = bank->link[idx].arc;
    if (cur != NULL) {
        if (arc == cur) {
            SND_UnlockMutex();
            return;
        }
        self = &bank->link[idx];
        p = cur->list;
        if (self == p) {
            cur->list = bank->link[idx].next;
            DC_StoreRange(bank->link[idx].arc, 0x3c);
        } else {
            q = p;
            if (q != NULL) {
                do {
                    n = q->next;
                    if (self == n) {
                        break;
                    }
                    q = n;
                } while (n != NULL);
            }
            q->next = bank->link[idx].next;
            DC_StoreRange(q, 8);
        }
    }
    {
        SNDLink *old = arc->list;
        arc->list = &bank->link[idx];
        bank->link[idx].next = old;
        bank->link[idx].arc = arc;
    }
    SND_UnlockMutex();
    DC_StoreRange(bank, 0x3c);
    DC_StoreRange(arc, 0x3c);
}

void UnlinkBankFromWaveArcs(SNDBank *bank) {
    int i;
    SNDWaveArc *cur;
    SNDLink *self;
    SNDLink *p;
    SNDLink *q;
    SNDLink *n;
    SND_LockMutex();
    self = bank->link;
    for (i = 0; i < 4; i++, self++) {
        cur = bank->link[i].arc;
        if (cur == NULL) {
            continue;
        }
        p = cur->list;
        if (self == p) {
            cur->list = bank->link[i].next;
            DC_StoreRange(cur, 0x3c);
        } else {
            q = p;
            if (q != NULL) {
                do {
                    n = q->next;
                    if (self == n) {
                        break;
                    }
                    q = n;
                } while (n != NULL);
            }
            q->next = bank->link[i].next;
            DC_StoreRange(q, 8);
        }
    }
    SND_UnlockMutex();
}

void ClearWaveArcLinks(SNDWaveArc *arc) {
    SNDLink *l;
    SNDLink *next;
    SND_LockMutex();
    l = arc->list;
    if (l != NULL) {
        do {
            next = l->next;
            l->arc = NULL;
            l->next = NULL;
            DC_StoreRange(l, 8);
            l = next;
        } while (next != NULL);
    }
    SND_UnlockMutex();
}

void ClearInstPos(u32 *p) {
    volatile u32 zero[2];
    p[0] = 0;
    zero[0] = 0;
    zero[1] = 0;
    p[1] = 0;
}

typedef struct { u16 v[5]; } SndU16x5;
typedef struct { u16 v[6]; } SndU16x6;

BOOL GetNextInstData(SNDBank *bank, u16 *out, SNDInstPos *pos) {
    u32 sub = 0;
    if (pos->index < bank->count) {
        do {
            int type = bank->inst[pos->index] & 0xff;
            u32 entry = bank->inst[pos->index];
            *(u8 *) out = (u8) entry;
            entry >>= 8;
            switch (type) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5: {
                u16 *rec = (u16 *) ((u8 *) bank + entry);
                *(SndU16x5 *) (out + 1) = *(SndU16x5 *) rec;
                pos->index++;
                return TRUE;
            }
            case 0x10: {
                u8 *base = (u8 *) bank + entry;
                while (pos->sub < (u32) (base[1] - base[0] + 1)) {
                    u16 *rec = (u16 *) (base + pos->sub * 12);
                    *(SndU16x6 *) out = *(SndU16x6 *) (rec + 1);
                    pos->sub++;
                    return TRUE;
                }
                break;
            }
            case 0x11: {
                u8 *base = (u8 *) bank + entry;
                while (pos->sub < 8) {
                    u32 k = pos->sub;
                    u16 *rec;
                    if (base[k] == 0) {
                        break;
                    }
                    rec = (u16 *) (base + k * 12);
                    *(SndU16x6 *) out = *(SndU16x6 *) (rec + 4);
                    pos->sub++;
                    return TRUE;
                }
                break;
            }
            default:
                break;
            }
            pos->index++;
            pos->sub = sub;
        } while (pos->index < bank->count);
    }
    return FALSE;
}

u32 GetWaveArcEntryCount(SNDBank *bank) {
    return bank->count;
}

void SetWaveArcEntry(SNDBank *bank, int idx, u32 value) {
    SND_LockMutex();
    bank->inst[idx] = value;
    DC_StoreRange(&bank->inst[idx], 4);
    SND_UnlockMutex();
}

void *GetWaveArcEntry(SNDBank *bank, int idx) {
    u32 v;
    void *r;
    SND_LockMutex();
    v = bank->inst[idx];
    if (v != 0) {
        if (v < 0x2000000) {
            r = (u8 *) bank + v;
        } else {
            r = (void *) v;
        }
    } else {
        r = NULL;
    }
    SND_UnlockMutex();
    return r;
}
