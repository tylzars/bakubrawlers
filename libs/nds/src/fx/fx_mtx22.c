#include <nitro/internal.h>

// MTX_Identity22_: writes a 2x2 fixed-point identity matrix (four words) to the
// buffer in r0: 0x1000, 0, 0, 0x1000. Returns nothing.
// Not compiled: stores with `stmia r0!, {r2, r3}` and `stmia r0!, {r1, r2}` (base written back). In this build's
// compiled code an ldm/stm writes back a base other than sp only in struct copies and fills, which carry the data in
// r0-r3 (r0-r1 for a fill) and never use r0 or r1 as the base. In the forms tried, stores merged into one stm keep
// the pointer step as a separate `add` (`p[0] = a; p[1] = b; p[2] = c; p += 3;` gives `stm r0, {r1, r2, r3}` and
// `add r0, r0, #12`). Element stores compile to `str` at offsets from r0, one register per constant.
asm void MTX_Identity22_(MtxFx22 *m) {
    mov r1, #0
    mov r2, #0x1000
    mov r3, #0
    stmia r0!, {r2, r3}     // elements 0 and 1 = 1.0, 0
    stmia r0!, {r1, r2}     // elements 2 and 3 = 0, 1.0
    bx lr
}

// MTX_Scale22 (Thumb): fills a four-word block at r0 with { a, 0, 0, b }.
// The pointer is advanced by one word after the first store, so the final
// offset store of b lands on the fourth word.
#pragma thumb on
// Not compiled: Thumb `stmia r0!, {r1}` and `stmia r0!, {r1, r2}` on the incoming pointer. In every form tried the
// compiler emits a Thumb stmia only in struct copies and fills, which first move the pointer off r0
// (`adds r3, r0, #0`) and store pairs from r0 and r1; element stores compile to `str` at offsets from r0.
asm void MTX_Scale22(int *m, int a, int b) {
    stmia r0!, {r1}         // word 0 = a
    mov r1, #0
    str r2, [r0, #8]        // word 3 = b (r0 already advanced by one word)
    mov r2, #0
    stmia r0!, {r1, r2}     // words 1 and 2 = 0
    bx lr
}
#pragma thumb off

void MTX_Concat22(const MtxFx22 *a, const MtxFx22 *b, MtxFx22 *ab) {
    MtxFx22 tmp;
    MtxFx22 *p = (ab == b) ? &tmp : ab;
    fx32 x, y;
    x = a->_[0];
    y = a->_[1];
    p->_[0] = (fx32) (((fx64) x * b->_[0] + (fx64) y * b->_[2]) >> 12);
    p->_[1] = (fx32) (((fx64) x * b->_[1] + (fx64) y * b->_[3]) >> 12);
    x = a->_[2];
    y = a->_[3];
    p->_[2] = (fx32) (((fx64) x * b->_[0] + (fx64) y * b->_[2]) >> 12);
    p->_[3] = (fx32) (((fx64) x * b->_[1] + (fx64) y * b->_[3]) >> 12);
    if (p == &tmp) {
        *ab = tmp;
    }
}

// MTX_Identity44_: writes a 4x4 fixed-point identity matrix (sixteen words,
// 0x1000 at words 0, 5, 10 and 15, zero elsewhere) to the buffer in r0.
// Returns nothing.
// Not compiled: fills the matrix with seven `stmia r0!` of two or three registers (base written back). In this
// build's compiled code an ldm/stm writes back a base other than sp only in struct copies and fills, which carry the
// data in r0-r3 (r0-r1 for a fill) and never use r0 or r1 as the base. In the forms tried, stores merged into one stm
// keep the pointer step as a separate `add` (`p[0] = a; p[1] = b; p[2] = c; p += 3;` gives `stm r0, {r1, r2, r3}` and
// `add r0, r0, #12`). Element stores compile to `str` at offsets from r0, one register per constant.
asm void MTX_Identity44_(void *m) {
    mov r2, #0x1000
    mov r3, #0
    stmia r0!, {r2, r3}         // m[0]=1.0, m[1]=0
    mov r1, #0
    stmia r0!, {r1, r3}         // m[2]=0, m[3]=0
    stmia r0!, {r1, r2, r3}     // m[4]=0, m[5]=1.0, m[6]=0
    stmia r0!, {r1, r3}         // m[7]=0, m[8]=0
    stmia r0!, {r1, r2, r3}     // m[9]=0, m[10]=1.0, m[11]=0
    stmia r0!, {r1, r3}         // m[12]=0, m[13]=0
    stmia r0!, {r1, r2}         // m[14]=0, m[15]=1.0
    bx lr
}

// MTX_Copy44To43: copies twelve words from r0 to r1 as four groups of three,
// skipping one source word after each group (a 4-wide source row layout becomes
// a 3-wide destination row layout). Returns nothing; r0/r1 are scratch.
// Not compiled: copies with `ldmia r0!, {r2, r3, ip}` and `stmia r1!, {r2, r3, ip}` (base written back, data outside
// r0-r3). In this build's compiled code an ldm/stm writes back a base other than sp only in struct copies and fills,
// which carry the data in r0-r3 (r0-r1 for a fill) and never use r0 or r1 as the base. Three-word struct copies have
// no writeback (`ldm rN, {r0, r1, r2}`, each next row's address formed with `add`).
asm void MTX_Copy44To43(const int *src, int *dst) {
    ldmia r0!, {r2, r3, ip}     // copy 3 words
    add r0, r0, #4              // skip the fourth source word
    stmia r1!, {r2, r3, ip}
    ldmia r0!, {r2, r3, ip}
    add r0, r0, #4
    stmia r1!, {r2, r3, ip}
    ldmia r0!, {r2, r3, ip}
    add r0, r0, #4
    stmia r1!, {r2, r3, ip}
    ldmia r0!, {r2, r3, ip}
    add r0, r0, #4
    stmia r1!, {r2, r3, ip}
    bx lr
}
