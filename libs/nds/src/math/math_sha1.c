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
            MIi_CpuCopyFast(data, buf + c->fill, n);
            len -= n;
            data += n;
            data_020a4f50(c, buf, 0x40);
            c->fill = 0;
        } else {
            MIi_CpuCopyFast(data, buf + c->fill, len);
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
                MIi_CpuCopyFast(data, buf, 0x40);
                data += 0x40;
                data_020a4f50(c, buf, 0x40);
                m -= 0x40;
            } while (m > 0);
        }
    }
    c->fill = len;
    if (len == 0) return;
    MIi_CpuCopyFast(data, buf, len);
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
