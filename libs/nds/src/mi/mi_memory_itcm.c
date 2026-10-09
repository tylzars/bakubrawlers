// MI_CpuFill8 and the byte copy (ITCM). non-matching: MI_CpuFill8 differs in scheduling (14 words), func_01ff96c4
// is 0x134 vs 0x130 (register choice); same on every 2.0 build.
// ITCM NitroSDK memory routines 0x01ff94c8-0x01ff9802 (compiler 2.0/sp1p5, -O4,p -ipa file).
// Hand-written asm (not reproducible from C: conditional-execution loops, single-register ldm/stm, 8-register stm runs):
//   MIi_CpuClear16 0x01ff94c8, func_01ff94e0 0x01ff94e0, MIi_CpuClear32 0x01ff94fc, func_01ff9510, func_01ff9528,
//   MIi_CpuClearFast 0x01ff9540, MI_CpuCopy8(fast, ldm/stm x8) 0x01ff958c, func_01ff95c4, func_01ff95e0, func_01ff9604,
//   func_01ff97f4 (thumb, stm x3).
// Not matching (scheduling/register allocation only): MI_CpuFill8, func_01ff96c4 (byte copy).
#include <nitro/internal.h>

void MI_CpuFill8(void *dstp, u32 data, u32 size) {
    if (size == 0) {
        return;
    }
    if ((u32) dstp & 1) {
        u16 *dst = (u16 *) ((u32) dstp - 1);
        u32 lo = *dst & 0xff;
        *dst = (u16) (lo | (data << 8));
        dstp = (u8 *) dstp + 1;
        size--;
        if (size == 0) {
            return;
        }
    }
    if (size >= 2) {
        data = data | (data << 8);
        if ((u32) dstp & 2) {
            *(u16 *) dstp = (u16) data;
            dstp = (u16 *) dstp + 1;
            size -= 2;
            if (size == 0) {
                return;
            }
        }
        data = data | (data << 16);
        {
            u32 n = size & ~3;
            if (n) {
                u32 *w = (u32 *) dstp;
                u32 *e = (u32 *) ((u8 *) dstp + n);
                size -= n;
                do {
                    *w++ = data;
                } while (w < e);
                dstp = w;
            }
        }
        if (size & 2) {
            *(u16 *) dstp = (u16) data;
            dstp = (u16 *) dstp + 1;
        }
    }
    if (size & 1) {
        u32 hi = *(u16 *) dstp & 0xff00;
        *(u16 *) dstp = (u16) ((data & 0xff) | hi);
    }
}

void func_01ff96c4(const void *srcp, void *dstp, u32 size) {
    if (size == 0) {
        return;
    }
    if ((u32) dstp & 1) {
        u32 lo = *(u16 *) ((u8 *) dstp - 1) & 0xff;
        u32 b;
        if ((u32) srcp & 1) {
            b = (u32) *(u16 *) ((u8 *) srcp - 1) >> 8;
        } else {
            b = *(u16 *) srcp;
        }
        *(u16 *) ((u8 *) dstp - 1) = (u16) (lo | (b << 8));
        srcp = (u8 *) srcp + 1;
        dstp = (u8 *) dstp + 1;
        size--;
        if (size == 0) {
            return;
        }
    }
    if (((u32) dstp ^ (u32) srcp) & 1) {
        u32 t;
        u32 v;
        srcp = (const void *) ((u32) srcp & ~1);
        v = *(const u16 *) srcp;
        srcp = (const u16 *) srcp + 1;
        t = v >> 8;
        size -= 2;
        if ((s32) size >= 0) {
            do {
                v = *(const u16 *) srcp;
                srcp = (const u16 *) srcp + 1;
                v = t | (v << 8);
                *(u16 *) dstp = (u16) v;
                dstp = (u16 *) dstp + 1;
                t = v >> 16;
                size -= 2;
            } while ((s32) size >= 0);
        }
        if (size & 1) {
            *(u16 *) dstp = (u16) ((*(u16 *) dstp & 0xff00) | t);
        }
        return;
    }
    if (((u32) dstp ^ (u32) srcp) & 2) {
        u32 n = size & ~1;
        if (n) {
            const u16 *s = (const u16 *) srcp;
            u16 *d = (u16 *) dstp;
            u16 *e = (u16 *) ((u8 *) dstp + n);
            size -= n;
            do {
                *d++ = *s++;
            } while (d < e);
            srcp = s;
            dstp = d;
        }
    } else if (size >= 2) {
        if ((u32) dstp & 2) {
            *(u16 *) dstp = *(const u16 *) srcp;
            srcp = (const u16 *) srcp + 1;
            dstp = (u16 *) dstp + 1;
            size -= 2;
            if (size == 0) {
                return;
            }
        }
        {
            u32 n = size & ~3;
            if (n) {
                const u32 *s = (const u32 *) srcp;
                u32 *d = (u32 *) dstp;
                u32 *e = (u32 *) ((u8 *) dstp + n);
                size -= n;
                do {
                    *d++ = *s++;
                } while (d < e);
                srcp = s;
                dstp = d;
            }
        }
        if (size & 2) {
            *(u16 *) dstp = *(const u16 *) srcp;
            srcp = (const u16 *) srcp + 1;
            dstp = (u16 *) dstp + 1;
        }
    }
    if (size & 1) {
        *(u16 *) dstp = (u16) ((*(u16 *) dstp & 0xff00) | (*(const u16 *) srcp & 0xff));
    }
}
