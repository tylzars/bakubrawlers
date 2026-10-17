#include "Video/MobiclipDecoder.hpp"
extern "C" {
typedef void *(*F4)(void *, void *, u32);
int PlayerNextFrame(P *p) {
    if (p->o98 == p->frames) return 0;
    p->o98++;
    int old = p->cur;
    p->cur = (old + 1) % 2;
    p->src->adv();
    u32 w = *(u32 *) (p->o70[p->cur] + (p->o80[p->cur] + p->o78[p->cur]));
    p->o78[old] = w >> 14;
    p->o88[old] = w & 0x3fff;
    p->ctx->ptr = p->o70[p->cur] + p->o80[p->cur];
    if (p->o98 < p->frames) {
        p->o80[old] = p->src->read(p->o70[old], p->o78[old] + 4);
    }
    for (int i = 0; i < 6; i++) {
        p->ctx->a[i] = p->o5c[p->oac[i]];
        p->ctx->b[i] = p->o60[p->oac[i]];
    }
    p->oc8 = p->o1e * p->o88[p->cur];
    return 1;
}
}

extern "C" {
extern "C" void *MobiclipAlloc(u32 size);
extern "C" void MobiclipFree(void *p);
void func_0209d02c(void *);
extern "C" void *func_0203bf80();
int func_0203b808(void *);
int func_02090b24(Chan *);
void func_020914e4(Chan *);
extern "C" void MobiclipSetAdpcmState(Chan *, u8 *);
extern "C" void MobiclipDecodeAdpcm(Chan *, u8 *, int, u32);
extern "C" void MIi_CpuCopy16(void *, u32, u32);
extern const int data_0209d568[];

int PlayerDecodeFrame(P *p) {
    if (p->o9c > p->oa0) return 0;
    int k;
    if (*(u16 *) p->ctx->ptr & 0x8000) {
        p->oa4 = 1;
        k = 4;
    } else {
        k = 0;
        p->oa4 = k;
    }
    p->ctx->ptr += p->fn38(p->ctx);
    if (p->c8 == 'N' && p->c9 == '3') {
        p->ctx->ptr += k;
    }
    p->o64[p->oac[0]] = p->ctx->o3b4;
    int i = 0;
    p->occ = 0;
    p->od0 = 0;
    for (i = 0; i < 6; i++) {
        p->oac[i]++;
        if (p->oa8 == p->oac[i]) p->oac[i] = 0;
    }
    p->oa0++;
    return 1;
}

struct Conv { void *y, *u, *dst; u32 w; u32 a, b, c; };
int PlayerConvertFrame(P *p, u32 a, u32 b, int c) {
    if (p->o9c >= p->oa0) return 0;
    if (c != 0) {
        if (p->o68 == 0) {
            p->o68 = MobiclipAlloc(p->o14 << 8);
            if (p->o68 == 0) return 0;
        }
        if (p->o6c == 0) {
            p->o6c = MobiclipAlloc((p->o14 >> 1) << 8);
            if (p->o6c == 0) return 0;
        }
        void *s[9];
        s[0] = p->o5c[p->oc4];
        s[1] = p->o60[p->oc4];
        s[2] = p->o68;
        s[3] = p->o6c;
        s[4] = (void *) p->o10;
        s[5] = (void *) p->o14;
        s[6] = p->o64[p->oc4];
        s[7] = func_0203bf80();
        if (c == 2) { s[8] = 0; } else { s[8] = (void *) 1; }
        func_0209d02c(s);
        p->o3c = p->o68;
        p->o40 = p->o6c;
    } else {
        p->o3c = p->o5c[p->oc4];
        p->o40 = p->o60[p->oc4];
    }
    p->o44 = (void *) a;
    p->o48 = b << 1;
    func_0203b808(&p->o3c);
    p->o9c++;
    p->oc4++;
    if (p->oc4 == p->oa8) p->oc4 = 0;
    return 1;
}

int PlayerSkipFrame(P *p) {
    if (p->o9c >= p->oa0) return 0;
    p->o9c++;
    p->oc4++;
    if (p->oc4 == p->oa8) p->oc4 = 0;
    return 1;
}

// blocks per channel in the current frame (oc8 = o1e channels times this)
u32 PlayerGetAudioBlockCount(P *p) { return p->o88[p->cur]; }

#define CH(p) ((Chan *) ((p)->o58 + (p)->od0 * 0x14f8))
int PlayerReadAudioBlock(P *p, u32 v) {
    if (p->occ == p->oc8) return 0;
    u32 t = p->o1c;
    if (t == 0) return 0;
    if (t == 1) {
        *(u8 **) (p->o58 + p->od0 * 0x14f8) = p->ctx->ptr;
        ((u32 *) CH(p))[1] = v;
        p->ctx->ptr += func_02090b24(CH(p));
    } else if (t == 2) {
        *(u8 **) (p->o58 + p->od0 * 0x14f8) = p->ctx->ptr;
        ((u32 *) CH(p))[1] = v;
        func_020914e4(CH(p));
        p->ctx->ptr += 0x28;
    } else if (t == 3) {
        if (p->oa4 == 1) {
            MobiclipSetAdpcmState(CH(p), p->ctx->ptr);
            p->ctx->ptr += 4;
        }
        MobiclipDecodeAdpcm(CH(p), p->ctx->ptr, 0x80, v);
        p->ctx->ptr += 0x80;
    } else {
        MIi_CpuCopy16(p->ctx->ptr, v, 0x200);
        p->ctx->ptr += 0x200;
    }
    p->od0++;
    if (p->od0 == p->o1e) {
        p->od0 = 0;
        p->oa4 = 0;
    }
    p->occ++;
    return 1;
}

// Tests o20 == 0 where PlayerReadAudioBlock tests the type o1c == 0, so a type-0 stream with o20 != 0 skips 0x200.
int PlayerSkipAudioBlock(P *p) {
    if (p->occ == p->oc8) return 0;
    if (p->o20 == 0) return 0;
    u32 t = p->o1c;
    if (t == 1) {
        u8 *q = p->ctx->ptr;
        p->ctx->ptr = q + data_0209d568[((u32) *(u16 *) (q + 2) >> 12) & 3];
    } else if (t == 2) {
        p->ctx->ptr += 0x28;
    } else if (t == 3) {
        if (p->oa4 == 1) {
            p->ctx->ptr += 0x84;
        } else {
            p->ctx->ptr += 0x80;
        }
    } else {
        p->ctx->ptr += 0x200;
    }
    p->od0++;
    if (p->od0 == p->o1e) {
        p->od0 = 0;
        p->oa4 = 0;
    }
    p->occ++;
    return 1;
}
}

// the byte reader Mobiclip.cpp allocates with new
struct Reader {
    static void *operator new(unsigned long size);
};
void *Reader::operator new(unsigned long n) { return MobiclipAlloc(n); }

extern "C" {
void func_0203b64c(void *q) { MobiclipFree(q); }
}
