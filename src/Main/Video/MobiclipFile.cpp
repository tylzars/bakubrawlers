#include "Video/MobiclipDecoder.hpp"
typedef struct { u8 pad[0x24]; u32 o24, o28, o2c; } FSFile;
extern "C" {
void MIi_CpuCopyFast(const void *src, void *dst, u32 n);
void FS_WaitFile(FSFile *);
int FS_SeekFile(FSFile *, u32, u32);
int FS_ReadFile(FSFile *, void *, u32);
int FS_ReadFileAsync(FSFile *, void *, u32);
void *OS_AllocFromHeap(int, int, u32);
void OS_FreeToHeap(int, int, void *);
struct F { u32 o0; u32 o4; u32 o8; FSFile *file; u8 flag; };
int ReaderInit(F *f, FSFile *file) {
    f->file = file;
    f->o4 = file->o28 - file->o24;
    f->o8 = f->file->o2c - f->file->o24;
    f->flag = 0;
    return 1;
}
int ReaderSeek(F *f, u32 pos) {
    if (f->flag == 1) {
        FS_WaitFile(f->file);
        f->flag = 0;
    }
    if (FS_SeekFile(f->file, pos, 0) == 0) return 0;
    f->o8 = pos;
    return 1;
}
int ReaderRead(F *f, void *buf, u32 n) {
    if (f->flag == 1) {
        FS_WaitFile(f->file);
        FS_SeekFile(f->file, f->o8, 0);
        f->flag = 0;
    }
    if (FS_ReadFile(f->file, buf, n) == -1) return 0;
    f->o8 += n;
    return 1;
}
int ReaderStartAlignedRead(F *f, void *buf, u32 n) {
    u32 aligned = f->o8 & -0x200;
    u32 off = f->o8 - aligned;
    u32 len = n + off;
    if (len & 0x1ff) len = (len & -0x200) + 0x200;
    FS_SeekFile(f->file, aligned, 0);
    if (FS_ReadFileAsync(f->file, buf, len) == -1) return 0;
    f->o8 += n;
    f->flag = 1;
    return off;
}
void func_0203b7c8(F *f) {
    if (f->flag == 1) FS_WaitFile(f->file);
    f->flag = 0;
}
void ReaderClose(Src *s) { s->adv(); }
void func_0203b804() {}
// YUV-to-RGB555 conversion (hand-written assembly): args points at {Y, UV, dst, stride, width, height, table}.
// Y rows are 0x100 bytes apart; each U/V row (0x100 bytes, U in the first 0x80, V in the second) serves two Y
// rows, one U/V pair per 2x2 pixels. Output pixels are 16-bit with bit 15 set. The loops take 16 pixels and two
// rows per pass.
asm void func_0203b808(void *args) {
    // save the registers; unpack the argument block
    stmfd sp!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}
    ldr r8, [r0, #0xc]              // stride: bytes per output row
    ldr r9, [r0, #0x10]             // width in pixels
    ldr r10, [r0, #0x14]            // height in pixels
    ldr r3, [r0, #0x18]             // lookup table
    mov r4, #0x100
    rsb r5, r9, r4, lsl #0x1        // Y step to the next row pair: 0x200 - width
    sub r6, r4, r9, lsr #0x1        // U/V step to the next row: 0x100 - width / 2
    sub r7, r8, r9
    mov r7, r7, lsl #0x1            // output step over the second row: 2 * (stride - width)
    stmfd sp!, {r5, r6, r7, r8, r9, r10} // kept on the stack for the whole call
    ldr r1, [r0, #0x4]              // r1 = U/V
    ldr r2, [r0, #0x8]              // r2 = output, first row of the pair
    ldr r0, [r0, #0x0]              // r0 = Y
    add r3, r3, #0x100              // r3 = table + 0x100, indexed with signed offsets
    add lr, r2, r8                  // lr = output, second row of the pair
    // one pass per pair of rows
L0203b848:
    str r10, [sp, #-4]!             // save the rows left
    ldr r10, [sp, #0x14]            // r10 = pixels left in this row pair
L0203b850:
    // 16 pixels per pass, as eight 2x2 blocks. Each block shares one U and one V byte: three table bases
    // are formed from them, each indexed by a Y byte, and the three results are packed into one 16-bit
    // pixel. Two of the four pixels use Y - 4.
    ldrb r5, [r1], #0x80            // U
    ldrb r6, [r1], #-0x7f           // V, 0x80 bytes after U; r1 moves on to the next U
    ldrb r4, [r0], #0x1             // Y, first row, left pixel
    sub r5, r5, #0x80               // u = U - 0x80
    sub r6, r6, #0x80               // v = V - 0x80
    add r7, r3, r6                  // r7 = table + v
    sub r6, r5, r6
    add r6, r6, r3                  // r6 = table + u - v
    sub r5, r6, r5, lsl #0x1        // r5 = table - u - v
    ldrb r8, [r7, r4]               // each base indexed by Y
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    ldrb r4, [r0], #0xff            // Y, first row, right pixel; r0 moves to the second row
    add r8, r11, r8, lsl #0x5       // r6 lookup in bits 0-4, r7 in bits 5-9
    add r8, r8, ip, lsl #0xa        // r5 in bits 10-14
    orr r8, r8, #0x8000             // and bit 15
    sub r4, r4, #0x4                // this pixel uses Y - 4
    ldrb r9, [r7, r4]
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    ldrb r4, [r0], #0x1             // Y, second row, left pixel
    add r8, r8, r9, lsl #0x15       // same packing for the upper halfword
    add r8, r8, r11, lsl #0x10
    add r8, r8, ip, lsl #0x1a
    orr r8, r8, #0x80000000         // bit 31
    str r8, [r2], #0x4              // two pixels to the first row
    sub r4, r4, #0x4                // this pixel uses Y - 4 too
    ldrb r8, [r7, r4]
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    ldrb r4, [r0], #-0xff           // Y, second row, right pixel; r0 back to the first row, next pair of columns
    add r8, r11, r8, lsl #0x5
    add r8, r8, ip, lsl #0xa
    orr r8, r8, #0x8000
    ldrb r9, [r7, r4]
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    add r8, r8, r9, lsl #0x15
    add r8, r8, r11, lsl #0x10
    add r8, r8, ip, lsl #0x1a
    orr r8, r8, #0x80000000
    str r8, [lr], #0x4              // two pixels to the second row
    // block 2: the same, two columns on
    ldrb r5, [r1], #0x80
    ldrb r6, [r1], #-0x7f
    ldrb r4, [r0], #0x1
    sub r5, r5, #0x80
    sub r6, r6, #0x80
    add r7, r3, r6
    sub r6, r5, r6
    add r6, r6, r3
    sub r5, r6, r5, lsl #0x1
    ldrb r8, [r7, r4]
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    ldrb r4, [r0], #0xff
    add r8, r11, r8, lsl #0x5
    add r8, r8, ip, lsl #0xa
    orr r8, r8, #0x8000
    sub r4, r4, #0x4
    ldrb r9, [r7, r4]
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    ldrb r4, [r0], #0x1
    add r8, r8, r9, lsl #0x15
    add r8, r8, r11, lsl #0x10
    add r8, r8, ip, lsl #0x1a
    orr r8, r8, #0x80000000
    str r8, [r2], #0x4
    sub r4, r4, #0x4
    ldrb r8, [r7, r4]
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    ldrb r4, [r0], #-0xff
    add r8, r11, r8, lsl #0x5
    add r8, r8, ip, lsl #0xa
    orr r8, r8, #0x8000
    ldrb r9, [r7, r4]
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    add r8, r8, r9, lsl #0x15
    add r8, r8, r11, lsl #0x10
    add r8, r8, ip, lsl #0x1a
    orr r8, r8, #0x80000000
    str r8, [lr], #0x4
    // block 3: the same, two columns on
    ldrb r5, [r1], #0x80
    ldrb r6, [r1], #-0x7f
    ldrb r4, [r0], #0x1
    sub r5, r5, #0x80
    sub r6, r6, #0x80
    add r7, r3, r6
    sub r6, r5, r6
    add r6, r6, r3
    sub r5, r6, r5, lsl #0x1
    ldrb r8, [r7, r4]
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    ldrb r4, [r0], #0xff
    add r8, r11, r8, lsl #0x5
    add r8, r8, ip, lsl #0xa
    orr r8, r8, #0x8000
    sub r4, r4, #0x4
    ldrb r9, [r7, r4]
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    ldrb r4, [r0], #0x1
    add r8, r8, r9, lsl #0x15
    add r8, r8, r11, lsl #0x10
    add r8, r8, ip, lsl #0x1a
    orr r8, r8, #0x80000000
    str r8, [r2], #0x4
    sub r4, r4, #0x4
    ldrb r8, [r7, r4]
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    ldrb r4, [r0], #-0xff
    add r8, r11, r8, lsl #0x5
    add r8, r8, ip, lsl #0xa
    orr r8, r8, #0x8000
    ldrb r9, [r7, r4]
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    add r8, r8, r9, lsl #0x15
    add r8, r8, r11, lsl #0x10
    add r8, r8, ip, lsl #0x1a
    orr r8, r8, #0x80000000
    str r8, [lr], #0x4
    // block 4: the same, two columns on
    ldrb r5, [r1], #0x80
    ldrb r6, [r1], #-0x7f
    ldrb r4, [r0], #0x1
    sub r5, r5, #0x80
    sub r6, r6, #0x80
    add r7, r3, r6
    sub r6, r5, r6
    add r6, r6, r3
    sub r5, r6, r5, lsl #0x1
    ldrb r8, [r7, r4]
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    ldrb r4, [r0], #0xff
    add r8, r11, r8, lsl #0x5
    add r8, r8, ip, lsl #0xa
    orr r8, r8, #0x8000
    sub r4, r4, #0x4
    ldrb r9, [r7, r4]
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    ldrb r4, [r0], #0x1
    add r8, r8, r9, lsl #0x15
    add r8, r8, r11, lsl #0x10
    add r8, r8, ip, lsl #0x1a
    orr r8, r8, #0x80000000
    str r8, [r2], #0x4
    sub r4, r4, #0x4
    ldrb r8, [r7, r4]
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    ldrb r4, [r0], #-0xff
    add r8, r11, r8, lsl #0x5
    add r8, r8, ip, lsl #0xa
    orr r8, r8, #0x8000
    ldrb r9, [r7, r4]
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    add r8, r8, r9, lsl #0x15
    add r8, r8, r11, lsl #0x10
    add r8, r8, ip, lsl #0x1a
    orr r8, r8, #0x80000000
    str r8, [lr], #0x4
    // block 5: the same, two columns on
    ldrb r5, [r1], #0x80
    ldrb r6, [r1], #-0x7f
    ldrb r4, [r0], #0x1
    sub r5, r5, #0x80
    sub r6, r6, #0x80
    add r7, r3, r6
    sub r6, r5, r6
    add r6, r6, r3
    sub r5, r6, r5, lsl #0x1
    ldrb r8, [r7, r4]
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    ldrb r4, [r0], #0xff
    add r8, r11, r8, lsl #0x5
    add r8, r8, ip, lsl #0xa
    orr r8, r8, #0x8000
    sub r4, r4, #0x4
    ldrb r9, [r7, r4]
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    ldrb r4, [r0], #0x1
    add r8, r8, r9, lsl #0x15
    add r8, r8, r11, lsl #0x10
    add r8, r8, ip, lsl #0x1a
    orr r8, r8, #0x80000000
    str r8, [r2], #0x4
    sub r4, r4, #0x4
    ldrb r8, [r7, r4]
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    ldrb r4, [r0], #-0xff
    add r8, r11, r8, lsl #0x5
    add r8, r8, ip, lsl #0xa
    orr r8, r8, #0x8000
    ldrb r9, [r7, r4]
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    add r8, r8, r9, lsl #0x15
    add r8, r8, r11, lsl #0x10
    add r8, r8, ip, lsl #0x1a
    orr r8, r8, #0x80000000
    str r8, [lr], #0x4
    // block 6: the same, two columns on
    ldrb r5, [r1], #0x80
    ldrb r6, [r1], #-0x7f
    ldrb r4, [r0], #0x1
    sub r5, r5, #0x80
    sub r6, r6, #0x80
    add r7, r3, r6
    sub r6, r5, r6
    add r6, r6, r3
    sub r5, r6, r5, lsl #0x1
    ldrb r8, [r7, r4]
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    ldrb r4, [r0], #0xff
    add r8, r11, r8, lsl #0x5
    add r8, r8, ip, lsl #0xa
    orr r8, r8, #0x8000
    sub r4, r4, #0x4
    ldrb r9, [r7, r4]
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    ldrb r4, [r0], #0x1
    add r8, r8, r9, lsl #0x15
    add r8, r8, r11, lsl #0x10
    add r8, r8, ip, lsl #0x1a
    orr r8, r8, #0x80000000
    str r8, [r2], #0x4
    sub r4, r4, #0x4
    ldrb r8, [r7, r4]
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    ldrb r4, [r0], #-0xff
    add r8, r11, r8, lsl #0x5
    add r8, r8, ip, lsl #0xa
    orr r8, r8, #0x8000
    ldrb r9, [r7, r4]
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    add r8, r8, r9, lsl #0x15
    add r8, r8, r11, lsl #0x10
    add r8, r8, ip, lsl #0x1a
    orr r8, r8, #0x80000000
    str r8, [lr], #0x4
    // block 7: the same, two columns on
    ldrb r5, [r1], #0x80
    ldrb r6, [r1], #-0x7f
    ldrb r4, [r0], #0x1
    sub r5, r5, #0x80
    sub r6, r6, #0x80
    add r7, r3, r6
    sub r6, r5, r6
    add r6, r6, r3
    sub r5, r6, r5, lsl #0x1
    ldrb r8, [r7, r4]
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    ldrb r4, [r0], #0xff
    add r8, r11, r8, lsl #0x5
    add r8, r8, ip, lsl #0xa
    orr r8, r8, #0x8000
    sub r4, r4, #0x4
    ldrb r9, [r7, r4]
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    ldrb r4, [r0], #0x1
    add r8, r8, r9, lsl #0x15
    add r8, r8, r11, lsl #0x10
    add r8, r8, ip, lsl #0x1a
    orr r8, r8, #0x80000000
    str r8, [r2], #0x4
    sub r4, r4, #0x4
    ldrb r8, [r7, r4]
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    ldrb r4, [r0], #-0xff
    add r8, r11, r8, lsl #0x5
    add r8, r8, ip, lsl #0xa
    orr r8, r8, #0x8000
    ldrb r9, [r7, r4]
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    add r8, r8, r9, lsl #0x15
    add r8, r8, r11, lsl #0x10
    add r8, r8, ip, lsl #0x1a
    orr r8, r8, #0x80000000
    str r8, [lr], #0x4
    // block 8: the same, two columns on
    ldrb r5, [r1], #0x80
    ldrb r6, [r1], #-0x7f
    ldrb r4, [r0], #0x1
    sub r5, r5, #0x80
    sub r6, r6, #0x80
    add r7, r3, r6
    sub r6, r5, r6
    add r6, r6, r3
    sub r5, r6, r5, lsl #0x1
    ldrb r8, [r7, r4]
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    ldrb r4, [r0], #0xff
    add r8, r11, r8, lsl #0x5
    add r8, r8, ip, lsl #0xa
    orr r8, r8, #0x8000
    sub r4, r4, #0x4
    ldrb r9, [r7, r4]
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    ldrb r4, [r0], #0x1
    add r8, r8, r9, lsl #0x15
    add r8, r8, r11, lsl #0x10
    add r8, r8, ip, lsl #0x1a
    orr r8, r8, #0x80000000
    str r8, [r2], #0x4
    sub r4, r4, #0x4
    ldrb r8, [r7, r4]
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    ldrb r4, [r0], #-0xff
    add r8, r11, r8, lsl #0x5
    add r8, r8, ip, lsl #0xa
    orr r8, r8, #0x8000
    ldrb r9, [r7, r4]
    ldrb r11, [r6, r4]
    ldrb ip, [r5, r4]
    add r8, r8, r9, lsl #0x15
    add r8, r8, r11, lsl #0x10
    add r8, r8, ip, lsl #0x1a
    orr r8, r8, #0x80000000
    str r8, [lr], #0x4
    subs r10, r10, #0x10            // 16 pixels done
    bgt L0203b850
    ldr r10, [sp], #4               // rows left
    // next row pair: advance Y, U/V and both output rows by the saved steps
    ldr r5, [sp, #0x0]
    ldr r6, [sp, #0x4]
    ldr r7, [sp, #0x8]
    add r0, r0, r5
    add r1, r1, r6
    add r2, r2, r7
    add lr, lr, r7
    subs r10, r10, #0x2             // two rows done
    bgt L0203b848
    add sp, sp, #0x18               // drop the six saved values
    ldmfd sp!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, pc}
}

struct G { u32 a, b, c, s0c; u8 *p10; u32 s14; u8 *p18; u32 pad; u32 d; };
extern G data_02215860;
u32 func_0203bdc8() { return 0x659c; }
u32 func_0203bdd4() { return 0x2580; }
void func_0203bddc(u8 *p, u32 n) {
    data_02215860.p18 = p;
    data_02215860.s14 = n & ~3;
    data_02215860.c = 0;
}
void func_0203bdfc(u8 *p, u32 n) {
    data_02215860.p10 = p;
    data_02215860.s0c = n & ~3;
    data_02215860.b = 0;
    data_02215860.a = 0;
    data_02215860.d = 0;
}
#define ALLOC(name, field, size, src) \
void *name() { \
    if (data_02215860.field == 0) { \
        if (data_02215860.s0c >= size) { \
            data_02215860.field = (u32) data_02215860.p10; \
            MIi_CpuCopyFast((void *) src, data_02215860.p10, size); \
            data_02215860.p10 += size; \
            data_02215860.s0c -= size; \
        } else { \
            return (void *) src; \
        } \
    } \
    return (void *) data_02215860.field; \
}
extern u8 data_02096a90[];
void *func_0203be24() {
    if (data_02215860.c == 0) {
        if (data_02215860.s14 >= 0x659c) {
            data_02215860.c = (u32) data_02215860.p18;
            MIi_CpuCopyFast(data_02096a90, data_02215860.p18, 0x659c);
            data_02215860.p18 += 0x659c;
            data_02215860.s14 -= 0x659c;
        } else {
            return data_02096a90;
        }
    }
    return (void *) data_02215860.c;
}
extern u8 data_02094990[], data_0209d6f8[], data_0209d578[];
ALLOC(func_0203bea0, b, 0x2100, data_02094990)
ALLOC(func_0203bf10, a, 0x300, data_0209d6f8)
ALLOC(func_0203bf80, d, 0x180, data_0209d578)
void func_0203bff0(u32 *s, u8 *p) {
    s[1] = p[0];
    s[0] = *(s16 *) (p + 2);
}
struct AdSt { int pred; int idx; };
extern const s8 data_0209d9f8[];
extern const s16 data_0209da08[];
#define NIB(n) \
    { \
        s16 step = data_0209da08[st->idx]; \
        st->idx += data_0209d9f8[(n) & 7]; \
        if (st->idx < 0) st->idx = 0; \
        if (st->idx > 0x58) st->idx = 0x58; \
        int diff = step >> 3; \
        if ((n) & 4) diff += step; \
        if ((n) & 2) diff += step >> 1; \
        if ((n) & 1) diff += step >> 2; \
        if ((n) & 8) st->pred -= diff; else st->pred += diff; \
        if (st->pred < -0x8000) st->pred = -0x8000; \
        if (st->pred > 0x7fff) st->pred = 0x7fff; \
    }
void func_0203c004(AdSt *st, u8 *in, int n, u32 *out) {
    for (int i = 0; i < n; i++) {
        int b = in[i];
        NIB(b)
        int s1 = st->pred;
        b >>= 4;
        NIB(b)
        *out++ = (u16) s1 | (st->pred << 16);
    }
}

void *MobiclipAlloc(u32 n) { return OS_AllocFromHeap(0, -1, n); }
void MobiclipFree(void *q) { OS_FreeToHeap(0, -1, q); }
}
