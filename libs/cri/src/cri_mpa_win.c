/* CRI MPEG audio: synthesis windowing, 0x0204c40c-0x0204d78c.
   The 16 functions are one routine specialised for the 16 ring-buffer start positions; a tap whose offset
   would run past the 1024-sample ring wraps to its start. */
#pragma dont_inline on
#define WIN_E(t) ((((t) >> 1) * 128) + (((t) & 1) * 96))
#define WIN_X(t, r) ((WIN_E(t) + 64 * (r)) >= 1024 ? WIN_E(t) - 1024 : WIN_E(t))
#define WIN_PAIR(k, r)                      \
    q = p;                                  \
    s += w[2 * (k)] * q[WIN_X(2 * (k), r)]; \
    s += w[2 * (k) + 1] * q[WIN_X(2 * (k) + 1, r)];

#define WIN_FUNC(name, r)                                        \
    void name(short *v, short *w, short *out) {                  \
        int lo = -0x8000;                                        \
        int hi = (unsigned)lo >> 17;                             \
        int i;                                                   \
        for (i = 32; i > 0; i--) {                               \
            short *p = v;                                        \
            short *q;                                            \
            int s = 0;                                           \
            v++;                                                 \
            WIN_PAIR(0, r) WIN_PAIR(1, r) WIN_PAIR(2, r) WIN_PAIR(3, r) \
            WIN_PAIR(4, r) WIN_PAIR(5, r) WIN_PAIR(6, r) WIN_PAIR(7, r) \
            w += 16;                                             \
            if (s > 0x3fff800) *out++ = hi;                      \
            else if (s < -0x4000000) *out++ = lo;                \
            else *out++ = s >> 11;                               \
        }                                                        \
    }

WIN_FUNC(func_0204c40c, 0)
WIN_FUNC(func_0204c544, 1)
WIN_FUNC(func_0204c67c, 2)
WIN_FUNC(func_0204c7b4, 3)
WIN_FUNC(func_0204c8ec, 4)
WIN_FUNC(func_0204ca24, 5)
WIN_FUNC(func_0204cb5c, 6)
WIN_FUNC(func_0204cc94, 7)
WIN_FUNC(func_0204cdcc, 8)
WIN_FUNC(func_0204cf04, 9)
WIN_FUNC(func_0204d03c, 10)
WIN_FUNC(func_0204d174, 11)
WIN_FUNC(func_0204d2ac, 12)
WIN_FUNC(func_0204d3e4, 13)
WIN_FUNC(func_0204d51c, 14)
WIN_FUNC(func_0204d654, 15)
