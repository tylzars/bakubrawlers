#include <nitro/private/fs_dev.h>

typedef struct {
    u32 h[5];       // 0x00
    u32 len_lo;     // 0x14
    u32 len_hi;     // 0x18
    u32 fill;       // 0x1c
    u8 buf[0x40];   // 0x20
    u32 pad[2];
} SHA1Ctx;

typedef struct {
    int digest_len;
    int block_len;
    void *ctx;
    void *dig;
    void (*init)(void *);
    void (*update)(void *, const void *, u32);
    void (*final)(void *, void *);
} HashDesc;

extern void (*data_020a4f50)(SHA1Ctx *, const void *, u32);   // block transform pointer
extern const u32 data_020902d4[7];                              // HMAC descriptor template

void MIi_CpuClear32(u32 d, void *p, u32 n);
#define MI_CpuClear32(p, n) MIi_CpuClear32(0, p, n)
void func_0200b9bc(u8 *out, const u8 *data, u32 len, const u8 *key, int keylen, HashDesc *d);

void func_0200b588(SHA1Ctx *c)
{
    c->h[0] = 0x67452301;
    c->h[1] = 0xefcdab89;
    c->h[2] = 0x98badcfe;
    c->h[3] = 0x10325476;
    c->h[4] = 0xc3d2e1f0;
    c->len_lo = 0;
    c->len_hi = 0;
    c->fill = 0;
}

void func_0200b5d8(SHA1Ctx *c, const u8 *data, u32 len)
{
    u8 *buf = c->buf;
    u32 n, sum;
    if (len == 0) return;
    sum = c->len_lo + (len << 3);
    if (sum < c->len_lo) c->len_hi++;
    c->len_hi += len >> 29;
    c->len_lo = sum;
    if (c->fill != 0) {
        if (c->fill + len >= 0x40) {
            n = 0x40 - c->fill;
            MI_CpuCopy8(data, buf + c->fill, n);
            len -= n;
            data += n;
            data_020a4f50(c, buf, 0x40);
            c->fill = 0;
        } else {
            MI_CpuCopy8(data, buf + c->fill, len);
            c->fill += len;
            return;
        }
    }
    if (len >= 0x40) {
        int m = len & ~0x3f;
        len -= m;
        if (((u32) data & 3) == 0) {
            data_020a4f50(c, data, m);
            data += m;
        } else {
            do {
                MI_CpuCopy8(data, buf, 0x40);
                data += 0x40;
                data_020a4f50(c, buf, 0x40);
                m -= 0x40;
            } while (m > 0);
        }
    }
    c->fill = len;
    if (len == 0) return;
    MI_CpuCopy8(data, buf, len);
}

void func_0200b72c(SHA1Ctx *c, u8 *out)
{
    u32 *w = (u32 *) c->buf;
    int i;
    int fill = c->fill;
    u8 *b;
    i = fill >> 2;
    if ((fill & 3) == 0) w[i] = 0;
    b = c->buf;
    b[fill] = 0x80;
    fill++;
    while ((fill & 3) != 0) {
        b[fill] = 0;
        fill++;
    }
    i++;
    if ((int) c->fill >= 0x38) {
        for (; i < 16; i++) w[i] = 0;
        data_020a4f50(c, w, 0x40);
        i = 0;
    }
    for (; i < 14; i++) w[i] = 0;
    {
        u32 lo = c->len_lo;
        b[0x3f] = lo;
        b[0x3e] = lo >> 8;
        b[0x3d] = lo >> 16;
        b[0x3c] = lo >> 24;
    }
    {
        u32 hi = c->len_hi;
        b[0x3b] = hi;
        b[0x3a] = hi >> 8;
        b[0x39] = hi >> 16;
        b[0x38] = hi >> 24;
    }
    data_020a4f50(c, w, 0x40);
    {
        u32 v = c->h[0];
        out[0] = v >> 24;
        out[1] = v >> 16;
        out[2] = v >> 8;
        out[3] = v;
    }
    {
        u32 v = c->h[1];
        out[4] = v >> 24;
        out[5] = v >> 16;
        out[6] = v >> 8;
        out[7] = v;
    }
    {
        u32 v = c->h[2];
        out[8] = v >> 24;
        out[9] = v >> 16;
        out[10] = v >> 8;
        out[11] = v;
    }
    {
        u32 v = c->h[3];
        out[12] = v >> 24;
        out[13] = v >> 16;
        out[14] = v >> 8;
        out[15] = v;
    }
    {
        u32 v = c->h[4];
        out[16] = v >> 24;
        out[17] = v >> 16;
        out[18] = v >> 8;
        out[19] = v;
    }
    c->fill = 0;
    MI_CpuClear32(&c, 4);
}

void func_0200b928(u8 *out, const u8 *data, u32 len, const u8 *key, int keylen)
{
    SHA1Ctx ctx;
    u8 dig[20];
    HashDesc d = { 20, 64, NULL, NULL, NULL, NULL, NULL };
    d.ctx = &ctx;
    d.dig = dig;
    d.init = (void (*)(void *)) func_0200b588;
    d.update = (void (*)(void *, const void *, u32)) func_0200b5d8;
    d.final = (void (*)(void *, void *)) func_0200b72c;
    func_0200b9bc(out, data, len, key, keylen, &d);
}

void func_0200b9bc(u8 *out, const u8 *data, u32 len, const u8 *key, int keylen, HashDesc *d)
{
    u8 kh[0x40];
    u8 ipad[0x40];
    u8 opad[0x40];
    int i;
    if (out == NULL || data == NULL || len == 0 || key == NULL || keylen == 0 || d == NULL) return;
    if (keylen > d->block_len) {
        d->init(d->ctx);
        d->update(d->ctx, key, keylen);
        d->final(d->ctx, kh);
        keylen = d->digest_len;
        key = kh;
    }
    for (i = 0; i < keylen; i++) ipad[i] = key[i] ^ 0x36;
    for (; i < d->block_len; i++) ipad[i] = 0x36;
    d->init(d->ctx);
    d->update(d->ctx, ipad, d->block_len);
    d->update(d->ctx, data, len);
    d->final(d->ctx, d->dig);
    for (i = 0; i < keylen; i++) opad[i] = key[i] ^ 0x5c;
    for (; i < d->block_len; i++) opad[i] = 0x5c;
    d->init(d->ctx);
    d->update(d->ctx, opad, d->block_len);
    d->update(d->ctx, d->dig, d->digest_len);
    d->final(d->ctx, out);
}

// The block transform below (with its five-word constant block, 0x0200bb74-0x0200be08) is hand-written assembly.
// Constant block (0x0200bb74, 20 bytes): a byte-lane mask 0x00ff00ff, then four round constants
// 0x5a827999, 0x6ed9eba1, 0x8f1bbcdc, 0xca62c1d6. Not a function.
asm void data_0200bb74(void) {
    dcd 0x00ff00ff
    dcd 0x5a827999
    dcd 0x6ed9eba1
    dcd 0x8f1bbcdc
    dcd 0xca62c1d6
}

// Hashes whole 64-byte blocks. r0 -> five-word running state, r1 -> input bytes, r2 = byte count
// (consumed 64 at a time while positive). Per block: the first 16 words are loaded big-endian
// (byte-swapped through the 0x00ff00ff mask) into a 64-word schedule kept on the stack (a
// 16-word window is stored twice, 0x40 apart, so the later words read a flat run); the other
// words are the rotate-by-1 of the xor of the words 16, 14, 8 and 3 positions back. Four stages
// of 20 steps follow, each step mixing the five state words with a different boolean function
// (select, parity, majority, parity) and its constant, rotating the first word left by 5 and the
// third right by 2. The block result is added back into the state at r0.
asm void func_0200bb88(unsigned *state, const void *data, int len) {
    stmfd sp!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}
    ldmia r0, {r3, r9, r10, r11, ip}    // load the five state words
    sub sp, sp, #0x84
    str r2, [sp, #0x80]                 // remaining length
L_block:
    ldr r8, [pc, #-40]                  // stage constant 1
    ldr r7, [pc, #-48]                  // byte-lane mask
    mov r6, sp
    mov r5, #0
L_load:                                 // stage 1, steps 0-15: words come from the input
    ldr r4, [r1], #4
    add r2, r8, ip
    add r2, r2, r3, ror #0x1b
    and lr, r4, r7
    and r4, r7, r4, ror #0x18
    orr r4, r4, lr, ror #8              // byte-reversed word
    str r4, [r6, #0x40]
    str r4, [r6], #4
    add r2, r2, r4
    eor r4, r10, r11
    and r4, r4, r9
    eor r4, r4, r11                     // select function
    add r2, r2, r4
    mov r9, r9, ror #2
    mov ip, r11
    mov r11, r10
    mov r10, r9
    mov r9, r3
    mov r3, r2
    add r5, r5, #4
    cmp r5, #0x40
    blt L_load
    mov r7, #0
    mov r6, sp
L_sel:                                  // stage 1, steps 16-19: expanded words, select function
    ldr r2, [r6, #0]
    ldr r5, [r6, #8]
    ldr r4, [r6, #0x20]
    ldr lr, [r6, #0x34]
    eor r2, r2, r5
    eor r4, r4, lr
    eor r2, r2, r4
    mov r2, r2, ror #0x1f
    str r2, [r6, #0x40]
    str r2, [r6], #4
    add r2, r2, ip
    add r2, r2, r8
    add r2, r2, r3, ror #0x1b
    eor r4, r10, r11
    and r4, r4, r9
    eor r4, r4, r11
    add r2, r2, r4
    mov r9, r9, ror #2
    mov ip, r11
    mov r11, r10
    mov r10, r9
    mov r9, r3
    mov r3, r2
    add r7, r7, #4
    cmp r7, #0x10
    blt L_sel
    ldr r8, [pc, #-252]                 // stage constant 2
    mov r7, #0
L_par1:                                 // stage 2: parity function, 20 steps
    ldr r2, [r6, #0]
    ldr r4, [r6, #8]
    ldr lr, [r6, #0x20]
    ldr r5, [r6, #0x34]
    eor r2, r2, r4
    eor lr, lr, r5
    eor r2, r2, lr
    mov r2, r2, ror #0x1f
    str r2, [r6, #0x40]
    str r2, [r6], #4
    add r2, r2, ip
    add r2, r2, r8
    add r2, r2, r3, ror #0x1b
    eor lr, r9, r10
    eor lr, lr, r11
    add r2, r2, lr
    mov r9, r9, ror #2
    mov ip, r11
    mov r11, r10
    mov r10, r9
    mov r9, r3
    mov r3, r2
    add r7, r7, #1
    cmp r7, #0xc
    moveq r6, sp                        // wrap the schedule window
    cmp r7, #0x14
    blt L_par1
    ldr r8, [pc, #-364]                 // stage constant 3
    mov r7, #0
L_maj:                                  // stage 3: majority function, 20 steps
    ldr r2, [r6, #0]
    ldr lr, [r6, #8]
    ldr r5, [r6, #0x20]
    ldr r4, [r6, #0x34]
    eor r2, r2, lr
    eor r5, r5, r4
    eor r2, r2, r5
    mov r2, r2, ror #0x1f
    str r2, [r6, #0x40]
    str r2, [r6], #4
    add r2, r2, ip
    add r2, r2, r8
    add r2, r2, r3, ror #0x1b
    orr r5, r9, r10
    and r5, r5, r11
    and r4, r9, r10
    orr r5, r5, r4
    add r2, r2, r5
    mov r9, r9, ror #2
    mov ip, r11
    mov r11, r10
    mov r10, r9
    mov r9, r3
    mov r3, r2
    add r7, r7, #1
    cmp r7, #8
    moveq r6, sp
    cmp r7, #0x14
    blt L_maj
    ldr r8, [pc, #-484]                 // stage constant 4
    mov r7, #0
L_par2:                                 // stage 4: parity function, 20 steps
    ldr r2, [r6, #0]
    ldr r5, [r6, #8]
    ldr r4, [r6, #0x20]
    ldr lr, [r6, #0x34]
    eor r2, r2, r5
    eor r4, r4, lr
    eor r2, r2, r4
    mov r2, r2, ror #0x1f
    str r2, [r6, #0x40]
    str r2, [r6], #4
    add r2, r2, ip
    add r2, r2, r8
    add r2, r2, r3, ror #0x1b
    eor r4, r9, r10
    eor r4, r4, r11
    add r2, r2, r4
    mov r9, r9, ror #2
    mov ip, r11
    mov r11, r10
    mov r10, r9
    mov r9, r3
    mov r3, r2
    add r7, r7, #1
    cmp r7, #4
    moveq r6, sp
    cmp r7, #0x14
    blt L_par2
    ldmia r0, {r2, r4, r6, r7, lr}      // add the block result into the state
    add r3, r3, r2
    add r9, r9, r4
    add r10, r10, r6
    add r11, r11, r7
    add ip, ip, lr
    stmia r0, {r3, r9, r10, r11, ip}
    ldr lr, [sp, #0x80]
    subs lr, lr, #0x40                  // next block while data remains
    str lr, [sp, #0x80]
    bgt L_block
    add sp, sp, #0x84
    ldmfd sp!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, pc}
}
