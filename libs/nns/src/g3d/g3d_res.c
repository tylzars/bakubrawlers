// NitroSystem G3d: binary resource file access. 0x02033574-0x0203382c
#include <nnsys/private/nns_g3d.h>
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define U8(p, o) (*((u8 *)(p) + (o)))

void *func_02033574(u8 *file, u32 idx) {   // data block by index
    if (idx >= U16(file, 0xe)) return 0;
    return file + ((u32 *)(file + U16(file, 0xc)))[idx];
}
void *func_02033598(u8 *file) { return file + U32(file, U16(file, 0xc)); }   // model set block
void *func_020335a8(u8 *file) {   // texture block
    u8 *blocks = file + U16(file, 0xc);
    if (U16(file, 0xe) == 1) {
        if (U32(file, 0) == 0x30585442) return file + U32(blocks, 0);
        return 0;
    }
    return file + U32(blocks, 4);
}
void *func_020335e8(u8 *file, u32 idx) {   // model by index
    if (file != 0) {
        ResMdlSet *set = (ResMdlSet *)(file + U32(file, U16(file, 0xc)));
        u32 *e = DictEntry(&set->dict, idx);
        if (e != 0) return (u8 *)set + *e;
    }
    return 0;
}
void *func_02033644(u8 *file, u32 magic, u32 kind) {   // find a block of an animation file
    if (file != 0 && U32(file, 0) == magic) {
        u32 i;
        for (i = 0; i < U16(file, 0xe); i++) {
            u8 *b = func_02033574(file, i);
            if (U32(b, 0) == kind) return b;
        }
    }
    return 0;
}
void *func_020336a0(u8 *file) { return func_02033644(file, 0x30415642, 0x30534956); }   // BVA0 / VIS0
void *func_020336bc(u8 *p, u32 idx) {
    if (p != 0 && idx < U8(p, 6)) return p + U16(p, 8) + idx * 16;
    return 0;
}
void *func_020336e8(u8 *p, u32 idx) {
    if (p != 0 && idx < U8(p, 7)) return p + U16(p, 0xa) + idx * 16;
    return 0;
}
void *func_02033788(u8 *p, u32 idx);
typedef struct { u16 f; u16 v; } KeyE;
u16 *func_02033714(u8 *p, u32 unused, u32 frame) {
    u8 *e = func_02033788(p, unused);
    KeyE *keys = (KeyE *)(p + U16(e, 6));
    u32 pos = ((s16)U16(e, 4) * frame) >> 12;
    while (pos > 0 && keys[pos].f >= frame) pos--;
    while (pos + 1 < U16(e, 0) && keys[pos + 1].f <= frame) pos++;
    return (u16 *)&keys[pos];
}
void *func_02033788(u8 *p, u32 idx) {   // entry of the dictionary at +0xc
    u8 *d = p + 0xc;
    if (d != 0 && idx < U8(p, 0xd)) {
        u8 *e = d + U16(p, 0x12);
        return e + 4 + *(u16 *)e * idx;
    }
    return 0;
}
void *func_020337bc(u8 *file) { return func_02033644(file, 0x30505442, 0x30544150); }   // BTP0 / PAT0
void *func_020337d8(u8 *file) { return func_02033644(file, 0x30415442, 0x30545253); }   // BTA0 / SRT0
void *func_020337f4(u8 *file) { return func_02033644(file, 0x30414d42, 0x3054414d); }   // BMA0 / MAT0
void *func_02033810(u8 *file) { return func_02033644(file, 0x30414342, 0x30544e4a); }   // BCA0 / JNT0
