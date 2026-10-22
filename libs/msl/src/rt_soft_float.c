// Soft-float and integer-division runtime (main 0x0201d72c-0x020202bc). Every routine here except
// func_0201e7dc (C) is hand-written ARM assembly. A float or int travels in r0 (second operand r1); a double or 64-bit
// integer in the pair r0 (low word) / r1 (high word), the second operand in r2/r3. The C
// prototypes only document register use.


extern int data_020c1790;

// _dadd: double-precision addition, a in r1:r0 (high:low), b in r3:r2, result in r1:r0.
// Hand-written; no compiler produced this. Layout of the working registers:
//   ip   = sign (bit 11) | biased exponent of the larger-magnitude operand
//   r1:r0 = its mantissa, left-justified with the hidden 1 forced into bit 63
//   r4   = exponent of the other operand, r3:r2 = its mantissa in the same form
// Operands whose signs differ are not handled here: b's sign is flipped and control
// jumps into the magnitude-subtract body inside _dsub (the instruction at _dsub+0x10,
// emitted below as a raw branch word because separate asm functions cannot share labels).
// _dsub in turn jumps back to _dadd+0x10 for operands that end up with equal signs.
// Special cases: NaN in either operand gives the all-ones-mantissa NaN 0x7fffffffffffffff,
// Inf + finite = Inf, Inf + Inf (same sign) = Inf, zero + x returns x, denormal inputs are
// treated as exponent 1 without hidden bit. Overflow gives +-Inf. Result is rounded to
// nearest, ties to even, using the 11 bits shifted out plus a sticky bit.
// The `dcd` word below is the cross-routine branch (no label can be shared between asm functions).
// Not compiled: part of one hand-written object with _dsub. The `bmi` written as the `dcd` word branches into the
// middle of _dsub (_dsub+0x10), and _dsub branches back into this routine at _dadd+0x10; a C function can neither
// branch into another function's body nor be entered anywhere but at its start.
asm double _dadd(double a, double b) {
    stmfd sp!, {r4, lr}
    // different signs? then flip b's sign and take the subtract path
    eors ip, r1, r3
    eormi r3, r3, #0x80000000
    dcd 0x4a0002b7
    // order the operands so r1:r0 has the larger magnitude (swap using the difference)
    subs ip, r0, r2
    sbcs lr, r1, r3
    bhs L_0201d758
    adds r2, r2, ip
    adc r3, r3, lr
    subs r0, r0, ip
    sbc r1, r1, lr
    // join point for both add and subtract: r1:r0 holds the larger magnitude. Unpack it
L_0201d758:
    mov lr, #0x80000000
    mov ip, r1, lsr #0x14
    orr r1, lr, r1, lsl #0xb
    orr r1, r1, r0, lsr #0x15
    mov r0, r0, lsl #0xb
    // exponent of a zero or 0x7ff?
    movs r4, ip, lsl #0x15
    cmnne r4, #0x200000
    beq L_0201d854
    // unpack b the same way
    mov r4, r3, lsr #0x14
    orr r3, lr, r3, lsl #0xb
    orr r3, r3, r2, lsr #0x15
    mov r2, r2, lsl #0xb
    movs lr, r4, lsl #0x15
    beq L_0201d89c
    // both operands normal: ip = exponent difference; align the smaller mantissa
L_0201d790:
    subs r4, ip, r4
    beq L_0201d7e8
    cmp r4, #0x20
    ble L_0201d7cc
    cmp r4, #0x38
    movge r4, #0x3f
    sub r4, r4, #0x20
    rsb lr, r4, #0x20
    orrs lr, r2, r3, lsl lr
    mov r2, r3, lsr r4
    orrne r2, r2, #0x1
    adds r0, r0, r2
    adcs r1, r1, #0x0
    blo L_0201d810
    b L_0201d7f4
    // difference 1..32: shift the smaller mantissa right across both words, sticky into bit 0
L_0201d7cc:
    rsb lr, r4, #0x20
    movs lr, r2, lsl lr
    rsb lr, r4, #0x20
    mov r2, r2, lsr r4
    orr r2, r2, r3, lsl lr
    mov r3, r3, lsr r4
    orrne r2, r2, #0x1
    // exponents equal (or aligned): add mantissas; no carry means already normalized
L_0201d7e8:
    adds r0, r0, r2
    adcs r1, r1, r3
    blo L_0201d810
    // carry out: shift right one (keep sticky), exponent + 1, overflow to Inf if exponent is 0x7ff
L_0201d7f4:
    add ip, ip, #0x1
    and r4, r0, #0x1
    movs r1, r1, rrx
    orr r0, r4, r0, rrx
    mov lr, ip, lsl #0x15
    cmn lr, #0x200000
    beq L_0201da20
    // pack: drop 11 low bits (round bits in r2), remove hidden bit, insert exponent; round to nearest even
L_0201d810:
    movs r2, r0, lsl #0x15
    mov r0, r0, lsr #0xb
    orr r0, r0, r1, lsl #0x15
    add r1, r1, r1
    mov r1, r1, lsr #0xc
    orr r1, r1, ip, lsl #0x14
    tst r2, #0x80000000
    ldmeqfd sp!, {r4, lr}
    bxeq lr
    movs r2, r2, lsl #0x1
    andeqs r2, r0, #0x1
    ldmeqfd sp!, {r4, lr}
    bxeq lr
    adds r0, r0, #0x1
    adc r1, r1, #0x0
    ldmfd sp!, {r4, lr}
    bx lr
    // first operand has exponent 0 or 0x7ff; lr = its sign
L_0201d854:
    cmp ip, #0x800
    movge lr, #0x80000000
    movlt lr, #0x0
    bics ip, ip, #0x800
    beq L_0201d8c0
    orrs r4, r0, r1, lsl #0x1
    bne L_0201d9fc
    mov r4, r3, lsr #0x14
    mov r3, r3, lsl #0xb
    orr r3, r3, r2, lsr #0x15
    mov r2, r2, lsl #0xb
    movs r4, r4, lsl #0x15
    beq L_0201d9e8
    cmn r4, #0x200000
    bne L_0201d9e8
    orrs r4, r2, r3, lsl #0x1
    beq L_0201d9e8
    b L_0201d9fc
    // second operand has exponent 0 or 0x7ff; lr = its sign
L_0201d89c:
    cmp r4, #0x800
    movge lr, #0x80000000
    movlt lr, #0x0
    bic ip, ip, #0x800
    bics r4, r4, #0x800
    beq L_0201d92c
    orrs r4, r2, r3, lsl #0x1
    bne L_0201d9fc
    b L_0201d9e8
    // first operand is a denormal (nonzero): use exponent 1, no hidden bit, then unpack the other
L_0201d8c0:
    orrs r4, r0, r1, lsl #0x1
    beq L_0201d900
    mov ip, #0x1
    bic r1, r1, #0x80000000
    mov r4, r3, lsr #0x14
    mov r3, r3, lsl #0xb
    orr r3, r3, r2, lsr #0x15
    mov r2, r2, lsl #0xb
    movs r4, r4, lsl #0x15
    cmnne r4, #0x200000
    mov r4, r4, lsr #0x15
    orr r4, r4, lr, lsr #0x14
    beq L_0201d89c
    orr r3, r3, #0x80000000
    orr ip, ip, lr, lsr #0x14
    b L_0201d790
    // first operand is zero: the result is the other operand (after NaN/Inf checks)
L_0201d900:
    mov ip, r3, lsr #0x14
    mov r1, r3, lsl #0xb
    orr r1, r1, r2, lsr #0x15
    mov r0, r2, lsl #0xb
    movs r4, ip, lsl #0x15
    beq L_0201d9b4
    cmn r4, #0x200000
    bne L_0201d9b4
    orrs r4, r0, r1, lsl #0x1
    beq L_0201d9e8
    b L_0201da00
    // second operand is zero or denormal
L_0201d92c:
    orrs r4, r2, r3, lsl #0x1
    beq L_0201d9c4
    mov r4, #0x1
    bic r3, r3, #0x80000000
    cmp r1, #0x0
    bpl L_0201d950
    orr ip, ip, lr, lsr #0x14
    orr r4, r4, lr, lsr #0x14
    b L_0201d790
    // both denormal-like: add mantissas directly
L_0201d950:
    adds r0, r0, r2
    adcs r1, r1, r3
    blo L_0201d970
    add ip, ip, #0x1
    and r4, r0, #0x1
    movs r1, r1, rrx
    mov r0, r0, rrx
    orr r0, r0, r4
    // pack (exponent 0 unless the sum reached bit 63); returns when the round bits are clear or a
    // tie meets an even result. A denormal sum has no round bits, so the round-up case never
    // occurs; the code would fall through into the next block
L_0201d970:
    cmp r1, #0x0
    subges ip, ip, #0x1
    movs r2, r0, lsl #0x15
    mov r0, r0, lsr #0xb
    orr r0, r0, r1, lsl #0x15
    add r1, r1, r1
    orr r1, lr, r1, lsr #0xc
    orr r1, r1, ip, lsl #0x14
    ldmeqfd sp!, {r4, lr}
    bxeq lr
    tst r2, #0x80000000
    ldmeqfd sp!, {r4, lr}
    bxeq lr
    movs r2, r2, lsl #0x1
    andeqs r2, r0, #0x1
    ldmeqfd sp!, {r4, lr}
    bxeq lr
    // return the second operand unchanged
L_0201d9b4:
    mov r1, r3
    mov r0, r2
    ldmfd sp!, {r4, lr}
    bx lr
    // second operand is zero: repack the first operand
L_0201d9c4:
    cmp r1, #0x0
    subges ip, ip, #0x1
    mov r0, r0, lsr #0xb
    orr r0, r0, r1, lsl #0x15
    add r1, r1, r1
    orr r1, lr, r1, lsr #0xc
    orr r1, r1, ip, lsl #0x14
    ldmfd sp!, {r4, lr}
    bx lr
    // result is a signed infinity (sign in lr)
L_0201d9e8:
    ldr r1, =0x7ff00000
    orr r1, lr, r1
    mov r0, #0x0
    ldmfd sp!, {r4, lr}
    bx lr
    // NaN result: all-ones mantissa, exponent 0x7ff, sign cleared
L_0201d9fc:
    mov r1, r3
    // second entry; both give the same constant (r1 is overwritten)
L_0201da00:
    mvn r0, #0x0
    bic r1, r0, #0x80000000
    ldmfd sp!, {r4, lr}
    bx lr
    // unreachable duplicate of the NaN return
    mvn r0, #0x0
    bic r1, r0, #0x80000000
    ldmfd sp!, {r4, lr}
    bx lr
    // exponent overflow: signed infinity
L_0201da20:
    cmp ip, #0x800
    movge lr, #0x80000000
    movlt lr, #0x0
    ldr r1, =0x7ff00000
    orr r1, lr, r1
    mov r0, #0x0
    ldmfd sp!, {r4, lr}
    bx lr
}

// _d2f: converts a double (r1:r0) to a single (r0).
// Zero/denormal doubles give a signed zero; Inf gives +-Inf; NaN gives 0x7fffffff.
// The exponent is rebiased (1023 -> 127, subtract 0x380). Exponent >= 255 after rebias
// gives +-Inf; a result exponent <= 0 goes to the denormal path, where the mantissa
// (with hidden bit) is shifted right, keeping a sticky bit. The 29 dropped mantissa bits
// are rounded to nearest, ties to even.
// Not compiled: `subs ip, ip, #0x380` is followed by `bls`, a test of the subtract's own carry and zero flags. In
// this build's compiled code and in every form tried, a 32-bit flag-setting subtract is tested only with eq, ne, mi
// or pl (or feeds the carry of a 64-bit subtract): the compiler folds a compare with zero into the subtract for those
// tests only. Written in C, unsigned `if (e <= 0x380)` with `e - 0x380` used afterwards compiles to `cmp rN, #0x380`
// and a separate `sub`, and signed `s = e - 0x380; if (s <= 0)` to `sub; cmp rN, #0`.
asm float _d2f(double a) {
    // r2 = sign; ip = biased exponent
    and r2, r1, #0x80000000
    mov ip, r1, lsr #0x14
    bics ip, ip, #0x800
    beq L_0201dabc
    mov r3, ip, lsl #0x15
    cmn r3, #0x200000
    bhs L_0201daa0
    // rebias 1023 -> 127
    subs ip, ip, #0x380
    bls L_0201dacc
    cmp ip, #0xff
    bge L_0201db3c
    mov r1, r1, lsl #0xc
    orr r3, r2, r1, lsr #0x9
    orr r3, r3, r0, lsr #0x1d
    movs r1, r0, lsl #0x3
    orr r0, r3, ip, lsl #0x17
    bxeq lr
    tst r1, #0x80000000
    bxeq lr
    movs r1, r1, lsl #0x1
    andeqs r1, r0, #0x1
    addne r0, r0, #0x1
    bx lr
    // exponent 0x7ff: Inf (mantissa zero) -> +-Inf, else NaN 0x7fffffff
L_0201daa0:
    orrs r3, r0, r1, lsl #0xc
    bne L_0201dab4
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #0x1
    bx lr
    // NaN
L_0201dab4:
    mvn r0, #0x80000000
    bx lr
    // exponent 0: zero or denormal -> signed zero (denormals underflow)
L_0201dabc:
    orrs r3, r0, r1, lsl #0xc
    bne L_0201db34
    mov r0, r2
    bx lr
    // rebiased exponent <= 0: denormal single or underflow
L_0201dacc:
    cmn ip, #0x17
    beq L_0201db20
    bmi L_0201db34
    mov r1, r1, lsl #0xb
    orr r1, r1, #0x80000000
    mov r3, r1, lsr #0x8
    orr r3, r3, r0, lsr #0x1d
    rsb ip, ip, #0x1
    movs r1, r0, lsl #0x3
    orr r0, r2, r3, lsr ip
    rsb ip, ip, #0x20
    mov r3, r3, lsl ip
    orrne r3, r3, #0x1
    movs r1, r3
    bxeq lr
    tst r1, #0x80000000
    bxeq lr
    movs r1, r1, lsl #0x1
    andeqs r1, r0, #0x1
    addne r0, r0, #0x1
    bx lr
    // rebiased exponent -23: the value is at least half the smallest denormal; any fraction bit
    // set rounds up to it, an exact half rounds to signed zero
L_0201db20:
    orr r0, r0, r1, lsl #0xc
    movs r1, r0
    mov r0, r2
    addne r0, r0, #0x1
    bx lr
    // signed zero
L_0201db34:
    mov r0, r2
    bx lr
    // overflow: signed infinity
L_0201db3c:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #0x1
    bx lr
}

// _dfix: double (r1:r0) to signed 32-bit integer, truncating toward zero.
// shift = 0x41e - biased exponent. shift <= 0 (|x| >= 2^31, Inf, NaN) saturates by sign
// to 0x7fffffff / 0x80000000; shift >= 32 (|x| < 1) gives 0; otherwise the top 32
// mantissa bits (hidden bit included) are shifted right by `shift` and negated if x < 0.
// Not compiled: `subs r2, r2, r3, lsr #0x14` is followed by `ble`, a signed test of the subtract's own flags. In this
// build's compiled code and in every form tried, a 32-bit flag-setting subtract is tested only with eq, ne, mi or pl
// (or feeds the carry of a 64-bit subtract): the compiler folds a compare with zero into the subtract for those tests
// only. Written in C, `s = k - (e >> 20); if (s <= 0)` compiles to `sub; cmp rN, #0`, `if (s < 0)` to `subs` with mi,
// and `if (k <= (e >> 20))` to `cmp` plus a separate `sub`.
asm int _dfix(double a) {
    bic r3, r1, #0x80000000
    ldr r2, =0x41e
    subs r2, r2, r3, lsr #0x14
    ble L_0201db84
    cmp r2, #0x20
    bge L_0201db7c
    mov r3, r1, lsl #0xb
    orr r3, r3, #0x80000000
    orr r3, r3, r0, lsr #0x15
    cmp r1, #0x0
    mov r0, r3, lsr r2
    rsbmi r0, r0, #0x0
    bx lr
    // |x| < 1: zero
L_0201db7c:
    mov r0, #0x0
    bx lr
    // out of range: saturate by sign
L_0201db84:
    mvn r0, r1, asr #0x1f
    add r0, r0, #0x80000000
    bx lr
}

// _dfixu: double (r1:r0) to unsigned 32-bit integer, truncating.
// Negative inputs give 0, except a negative NaN which gives 0xffffffff. For positive
// inputs shift = 0x41e - biased exponent: shift < 0 (>= 2^32, +Inf, NaN) gives
// 0xffffffff, shift >= 32 (< 1) gives 0, otherwise the mantissa is shifted right.
// Not compiled: `subs r2, r2, r1, lsr #0x14` is followed by `blt`, a signed test of the subtract's own flags. In this
// build's compiled code and in every form tried, a 32-bit flag-setting subtract is tested only with eq, ne, mi or pl
// (or feeds the carry of a 64-bit subtract): the compiler folds a compare with zero into the subtract for those tests
// only. Written in C, `s = k - (e >> 20); if (s < 0)` compiles to `subs` with mi, `if (s <= 0)` to `sub; cmp rN, #0`,
// and `if (k < (e >> 20))` to `cmp` plus a separate `sub`.
asm unsigned int _dfixu(double a) {
    tst r1, #0x80000000
    bne L_0201dbcc
    ldr r2, =0x41e
    subs r2, r2, r1, lsr #0x14
    blt L_0201dbe0
    cmp r2, #0x20
    bge L_0201dbc4
    mov r3, r1, lsl #0xb
    orr r3, r3, #0x80000000
    orr r3, r3, r0, lsr #0x15
    mov r0, r3, lsr r2
    bx lr
    // |x| < 1: zero
L_0201dbc4:
    mov r0, #0x0
    bx lr
    // negative input: 0, or 0xffffffff for a negative NaN
L_0201dbcc:
    cmn r1, #0x100000
    cmpeq r0, #0x0
    movls r0, #0x0
    mvnhi r0, #0x0
    bx lr
    // too large: saturate to 0xffffffff
L_0201dbe0:
    mvn r0, #0x0
    bx lr
}

// func_0201dbec: double (r1:r0) to unsigned 64-bit integer (result r1:r0), truncating.
// Same scheme as _dfixu with limit 2^64: shift = 0x43e - biased exponent. Negative
// inputs give 0 (a negative NaN gives all ones); shift < 0 saturates to all ones;
// shift >= 64 gives 0; otherwise the 53-bit mantissa, left-justified in a 64-bit value,
// is shifted right by `shift`.
// Not compiled: `subs r2, r2, r1, lsr #0x14` is followed by `blt`, a signed test of the subtract's own flags. In this
// build's compiled code and in every form tried, a 32-bit flag-setting subtract is tested only with eq, ne, mi or pl
// (or feeds the carry of a 64-bit subtract): the compiler folds a compare with zero into the subtract for those tests
// only. Written in C, `s = k - (e >> 20); if (s < 0)` compiles to `subs` with mi, `if (s <= 0)` to `sub; cmp rN, #0`,
// and `if (k < (e >> 20))` to `cmp` plus a separate `sub`.
asm unsigned long long func_0201dbec(double a) {
    tst r1, #0x80000000
    bne L_0201dc50
    ldr r2, =0x43e
    subs r2, r2, r1, lsr #0x14
    blt L_0201dc68
    cmp r2, #0x40
    bge L_0201dc44
    mov ip, r1, lsl #0xb
    orr ip, ip, #0x80000000
    orr ip, ip, r0, lsr #0x15
    cmp r2, #0x20
    ble L_0201dc2c
    sub r2, r2, #0x20
    mov r1, #0x0
    mov r0, ip, lsr r2
    bx lr
    // shift <= 32: two-word shift
L_0201dc2c:
    mov r3, r0, lsl #0xb
    mov r1, ip, lsr r2
    mov r0, r3, lsr r2
    rsb r2, r2, #0x20
    orr r0, r0, ip, lsl r2
    bx lr
    // |x| < 1: zero
L_0201dc44:
    mov r1, #0x0
    mov r0, #0x0
    bx lr
    // negative input: 0 unless NaN
L_0201dc50:
    cmn r1, #0x100000
    cmpeq r0, #0x0
    bhi L_0201dc68
    mov r1, #0x0
    mov r0, #0x0
    bx lr
    // saturate to all ones
L_0201dc68:
    mvn r1, #0x0
    mvn r0, #0x0
    bx lr
}

// _dflt: signed 32-bit integer (r0) to double (r1:r0).
// Takes the magnitude, returns +0 for zero, normalizes with a leading-zero count, and
// builds the exponent as 0x41e - clz. The 32-bit value always fits in the 53-bit
// mantissa, so no rounding is needed; the hidden bit is dropped when packing.
// Not compiled: normalises with `clz`, which this compiler never emits: it has no clz intrinsic, and `__builtin_clz`
// and `__clz` compile as calls to an external function.
asm double _dflt(int a) {
    // r2 = sign; take magnitude
    ands r2, r0, #0x80000000
    rsbmi r0, r0, #0x0
    cmp r0, #0x0
    mov r1, #0x0
    bxeq lr
    mov r3, #0x400
    add r3, r3, #0x1e
    // normalize: shift left by the leading-zero count, exponent = 0x41e - count
    clz ip, r0
    movs r0, r0, lsl ip
    sub r3, r3, ip
    movs r1, r0
    mov r0, r1, lsl #0x15
    add r1, r1, r1
    orr r1, r2, r1, lsr #0xc
    orr r1, r1, r3, lsl #0x14
    bx lr
}

// _dfltu: unsigned 32-bit integer (r0) to double (r1:r0). Like _dflt but with no sign
// handling; if the top bit is already set the normalization step is skipped.
// Not compiled: normalises with `clz`, which this compiler never emits: it has no clz intrinsic, and `__builtin_clz`
// and `__clz` compile as calls to an external function.
asm double _dfltu(unsigned int a) {
    cmp r0, #0x0
    mov r1, #0x0
    bxeq lr
    mov r3, #0x400
    add r3, r3, #0x1e
    bmi L_0201dcdc
    clz ip, r0
    movs r0, r0, lsl ip
    sub r3, r3, ip
    // pack: low word from the shifted-out bits, high word without hidden bit, plus exponent
L_0201dcdc:
    mov r1, r0
    mov r0, r1, lsl #0x15
    add r1, r1, r1
    mov r1, r1, lsr #0xc
    orr r1, r1, r3, lsl #0x14
    bx lr
}

// _dmul: double-precision multiplication, a in r1:r0, b in r3:r2, result in r1:r0.
// lr = sign of the result (xor of the input signs). Both mantissas are unpacked
// left-justified with the hidden bit; the 64x64 -> 128-bit product is formed from four
// 32x32 umull partial products, the top 64 bits are kept and any nonzero low half is
// folded into bit 0 as a sticky bit. The exponent is ea + eb - 0x3fe; if the product's
// top bit is clear it is shifted left once and the exponent decremented.
// Special cases: NaN operand -> NaN (0x7fffffffffffffff); Inf * 0 -> the same NaN;
// Inf * nonzero -> signed Inf; zero * finite -> signed zero; denormal operands are
// normalized first with a leading-zero count. Exponent overflow -> signed Inf; exponent
// underflow shifts into a denormal (smallest denormal / zero when far enough down).
// Rounding is to nearest, ties to even.
// Not compiled: normalises denormal operands with `clz`, which this compiler never emits: it has no clz intrinsic,
// and `__builtin_clz` and `__clz` compile as calls to an external function.
asm double _dmul(double a, double b) {
    stmfd sp!, {r4, r5, r6, r7, lr}
    // sign of the product
    eor lr, r1, r3
    and lr, lr, #0x80000000
    mov ip, r1, lsr #0x14
    mov r1, r1, lsl #0xb
    orr r1, r1, r0, lsr #0x15
    mov r0, r0, lsl #0xb
    movs r6, ip, lsl #0x15
    cmnne r6, #0x200000
    beq L_0201ddfc
    orr r1, r1, #0x80000000
    bic ip, ip, #0x800
    mov r4, r3, lsr #0x14
    mov r3, r3, lsl #0xb
    orr r3, r3, r2, lsr #0x15
    mov r2, r2, lsl #0xb
    movs r5, r4, lsl #0x15
    cmnne r5, #0x200000
    beq L_0201de44
    orr r3, r3, #0x80000000
    bic r4, r4, #0x800
    // 128-bit product of r1:r0 and r3:r2 (four 32x32 partial products); ip = ea + eb
L_0201dd48:
    add ip, r4, ip
    umull r5, r4, r0, r2
    umull r7, r6, r0, r3
    adds r4, r7, r4
    adc r6, r6, #0x0
    umull r7, r0, r1, r2
    adds r4, r7, r4
    adcs r0, r0, r6
    umull r7, r2, r1, r3
    adc r1, r2, #0x0
    adds r0, r0, r7
    adc r1, r1, #0x0
    orrs r4, r4, r5
    orrne r0, r0, #0x1
    cmp r1, #0x0
    blt L_0201dd94
    sub ip, ip, #0x1
    adds r0, r0, r0
    adc r1, r1, r1
    // rebias exponent; <= 0 means underflow, >= 0x7ff means overflow
L_0201dd94:
    add ip, ip, #0x2
    subs ip, ip, #0x400
    bmi L_0201df30
    beq L_0201df30
    mov r6, ip, lsl #0x14
    cmn r6, #0x100000
    bmi L_0201e030
    movs r2, r0, lsl #0x15
    mov r0, r0, lsr #0xb
    orr r0, r0, r1, lsl #0x15
    add r1, r1, r1
    orr r1, lr, r1, lsr #0xc
    orr r1, r1, ip, lsl #0x14
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    tst r2, #0x80000000
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    movs r2, r2, lsl #0x1
    andeqs r2, r0, #0x1
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    adds r0, r0, #0x1
    adc r1, r1, #0x0
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
    // first operand exponent is 0 or 0x7ff
L_0201ddfc:
    // special-case path: strip sign from the exponent
    bics ip, ip, #0x800
    beq L_0201de58
    orrs r6, r0, r1, lsl #0x1
    bne L_0201dfe4
    mov r4, r3, lsr #0x14
    mov r3, r3, lsl #0xb
    orr r3, r3, r2, lsr #0x15
    mov r2, r2, lsl #0xb
    movs r5, r4, lsl #0x15
    beq L_0201de38
    cmn r5, #0x200000
    bne L_0201dfd0
    orrs r5, r2, r3, lsl #0x1
    beq L_0201dfd0
    b L_0201dfe4
    // second operand is zero/denormal: zero -> NaN (Inf * 0), else Inf
L_0201de38:
    orrs r5, r3, r2
    beq L_0201dff8
    b L_0201dfd0
    // second operand exponent is 0 or 0x7ff
L_0201de44:
    bics r4, r4, #0x800
    beq L_0201deec
    orrs r6, r2, r3, lsl #0x1
    bne L_0201dfe4
    b L_0201dfd0
    // first operand is zero or denormal
L_0201de58:
    orrs r6, r0, r1, lsl #0x1
    beq L_0201dec0
    mov ip, #0x1
    cmp r1, #0x0
    bne L_0201de7c
    sub ip, ip, #0x20
    movs r1, r0
    mov r0, #0x0
    bmi L_0201de98
    // normalize first operand's denormal mantissa with a leading-zero count
L_0201de7c:
    clz r6, r1
    movs r1, r1, lsl r6
    rsb r6, r6, #0x20
    orr r1, r1, r0, lsr r6
    rsb r6, r6, #0x20
    mov r0, r0, lsl r6
    sub ip, ip, r6
    // unpack second operand
L_0201de98:
    mov r4, r3, lsr #0x14
    mov r3, r3, lsl #0xb
    orr r3, r3, r2, lsr #0x15
    mov r2, r2, lsl #0xb
    movs r5, r4, lsl #0x15
    cmnne r5, #0x200000
    beq L_0201de44
    orr r3, r3, #0x80000000
    bic r4, r4, #0x800
    b L_0201dd48
    // first operand is zero: signed zero unless second is Inf/NaN
L_0201dec0:
    mov r4, r3, lsr #0x14
    mov r3, r3, lsl #0xb
    orr r3, r3, r2, lsr #0x15
    mov r2, r2, lsl #0xb
    movs r5, r4, lsl #0x15
    beq L_0201e044
    cmn r5, #0x200000
    bne L_0201e044
    orrs r6, r2, r3, lsl #0x1
    beq L_0201dff8
    b L_0201dfe4
    // second operand is zero or denormal
L_0201deec:
    orrs r5, r2, r3, lsl #0x1
    beq L_0201e044
    mov r4, #0x1
    cmp r3, #0x0
    bne L_0201df10
    sub r4, r4, #0x20
    movs r3, r2
    mov r2, #0x0
    bmi L_0201dd48
    // normalize second operand's denormal mantissa
L_0201df10:
    clz r6, r3
    movs r3, r3, lsl r6
    rsb r6, r6, #0x20
    orr r3, r3, r2, lsr r6
    rsb r6, r6, #0x20
    mov r2, r2, lsl r6
    sub r4, r4, r6
    b L_0201dd48
    // result exponent <= 0: shift right into a denormal, rounding to nearest even
L_0201df30:
    cmn ip, #0x34
    beq L_0201dfc8
    bmi L_0201e020
    mov r2, r1
    mov r3, r0
    add r4, ip, #0x34
    cmp r4, #0x20
    movge r2, r3
    movge r3, #0x0
    subge r4, r4, #0x20
    rsb r5, r4, #0x20
    mov r2, r2, lsl r4
    orr r2, r2, r3, lsr r5
    movs r3, r3, lsl r4
    orrne r2, r2, #0x1
    rsb ip, ip, #0xc
    cmp ip, #0x20
    movge r0, r1
    movge r1, #0x0
    subge ip, ip, #0x20
    rsb r4, ip, #0x20
    mov r0, r0, lsr ip
    orr r0, r0, r1, lsl r4
    orr r1, lr, r1, lsr ip
    cmp r2, #0x0
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    tst r2, #0x80000000
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    movs r2, r2, lsl #0x1
    andeqs r2, r0, #0x1
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    adds r0, r0, #0x1
    adc r1, r1, #0x0
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
    // half the smallest denormal: round up only if any other bit is set
L_0201dfc8:
    orr r0, r0, r1, lsl #0x1
    b L_0201e008
    // signed infinity
L_0201dfd0:
    ldr r1, =0x7ff00000
    orr r1, lr, r1
    mov r0, #0x0
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
    // NaN (all-ones mantissa)
L_0201dfe4:
    mov r1, r3
    mvn r0, #0x0
    bic r1, r0, #0x80000000
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
    // NaN (Inf * 0)
L_0201dff8:
    mvn r0, #0x0
    bic r1, r0, #0x80000000
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
    // return smallest denormal if r0 nonzero, else signed zero
L_0201e008:
    movs r2, r0
    mov r1, lr
    mov r0, #0x0
    addne r0, r0, #0x1
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
    // underflow to signed zero
L_0201e020:
    mov r1, lr
    mov r0, #0x0
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
    // overflow: signed infinity
L_0201e030:
    ldr r1, =0x7ff00000
    orr r1, lr, r1
    mov r0, #0x0
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
    // signed zero
L_0201e044:
    mov r1, lr
    mov r0, #0x0
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
}

// sqrt: square root of a double (r1:r0), result r1:r0.
// Inputs with high word >= 0x7ff00000 (negatives, Inf, NaN): -0 is returned unchanged,
// +Inf is returned unchanged, anything else is an invalid operation: the result is the
// input with the quiet-NaN bits 0x7ff80000 or'ed in, and the value 0x21 is stored to the
// global data_020c1790 (a domain-error code). Zero returns itself; denormals are
// normalized with a leading-zero count. Otherwise the biased exponent is halved, rounding
// down; when it is even (odd unbiased exponent) the half is lowered by one and the mantissa
// shifted left one extra place. The root is produced bit by bit: 22 steps for the high word
// (root bits in r2, remainder in r1:r3, r4 = twice the partial root), then one step for bit 31
// and 31 more for the rest of the low word (r0). The last root bit is the round bit: it is
// cleared when the remainder is zero, then added in after the final shift right by one, and
// the exponent is rebiased.
// Not compiled: normalises with `clz`, which this compiler never emits: it has no clz intrinsic, and `__builtin_clz`
// and `__clz` compile as calls to an external function.
asm double sqrt(double x) {
    stmfd sp!, {r4, r5, r6, lr}
    ldr r2, =0x7ff00000
    cmp r1, r2
    bhs L_0201e1a4
    movs ip, r1, lsr #0x14
    beq L_0201e150
    bic r1, r1, r2
    orr r1, r1, #0x100000
    // ip = biased exponent, r1:r0 = mantissa with hidden bit (bit 52); halve the exponent
L_0201e078:
    movs ip, ip, asr #0x1
    bhs L_0201e08c
    sub ip, ip, #0x1
    movs r0, r0, lsl #0x1
    adc r1, r1, r1
    // high-word root, bit by bit
L_0201e08c:
    movs r3, r0, lsl #0x1
    adc r1, r1, r1
    mov r2, #0x0
    mov r4, #0x0
    mov lr, #0x200000
    // restoring step: try adding the next bit to the partial root
L_0201e0a0:
    add r6, r4, lr
    cmp r6, r1
    addle r4, r6, lr
    suble r1, r1, r6
    addle r2, r2, lr
    movs r3, r3, lsl #0x1
    adc r1, r1, r1
    movs lr, lr, lsr #0x1
    bne L_0201e0a0
    mov r0, #0x0
    mov r5, #0x0
    cmp r1, r4
    cmpeq r3, #0x80000000
    blo L_0201e0e8
    subs r3, r3, #0x80000000
    sbc r1, r1, r4
    add r4, r4, #0x1
    mov r0, #0x80000000
    // low-word root, bit by bit
L_0201e0e8:
    movs r3, r3, lsl #0x1
    adc r1, r1, r1
    mov lr, #0x40000000
    // restoring step for the low word
L_0201e0f4:
    add r6, r5, lr
    cmp r4, r1
    cmpeq r6, r3
    bhi L_0201e114
    add r5, r6, lr
    subs r3, r3, r6
    sbc r1, r1, r4
    add r0, r0, lr
    // loop tail: shift the remainder, move to the next bit
L_0201e114:
    movs r3, r3, lsl #0x1
    adc r1, r1, r1
    movs lr, lr, lsr #0x1
    bne L_0201e0f4
    // round, merge the two halves, rebias the exponent
    orrs r1, r1, r3
    biceq r0, r0, #0x1
    movs r1, r2, lsr #0x1
    movs r0, r0, rrx
    adcs r0, r0, #0x0
    adc r1, r1, #0x0
    add r1, r1, #0x20000000
    sub r1, r1, #0x100000
    add r1, r1, ip, lsl #0x14
    ldmfd sp!, {r4, r5, r6, lr}
    bx lr
    // exponent field zero: zero returns itself, denormals are normalized
L_0201e150:
    cmp r1, #0x0
    bne L_0201e180
    cmp r0, #0x0
    ldmeqfd sp!, {r4, r5, r6, lr}
    bxeq lr
    mvn ip, #0x13
    clz r5, r0
    movs r0, r0, lsl r5
    sub ip, ip, r5
    mov r1, r0, lsr #0xb
    mov r0, r0, lsl #0x15
    b L_0201e078
    // denormal with nonzero high word
L_0201e180:
    clz r2, r1
    movs r1, r1, lsl r2
    rsb r2, r2, #0x2b
    mov r1, r1, lsr #0xb
    orr r1, r1, r0, lsr r2
    rsb r2, r2, #0x20
    mov r0, r0, lsl r2
    rsb ip, r2, #0x1
    b L_0201e078
    // negative, Inf or NaN input
L_0201e1a4:
    tst r1, #0x80000000
    beq L_0201e1c0
    bics r3, r1, #0x80000000
    cmpeq r0, #0x0
    ldmeqfd sp!, {r4, r5, r6, lr}
    bxeq lr
    b L_0201e1cc
    // positive: Inf (mantissa zero) returns itself
L_0201e1c0:
    orrs r2, r0, r1, lsl #0xc
    ldmeqfd sp!, {r4, r5, r6, lr}
    bxeq lr
    // invalid operation: quiet NaN, store domain-error code 0x21
L_0201e1cc:
    ldr r2, =0x7ff80000
    orr r1, r1, r2
    ldr r3, =data_020c1790
    mov r4, #0x21
    str r4, [r3, #0x0]
    ldmfd sp!, {r4, r5, r6, lr}
    bx lr
}

// func_0201e1f4: reverse-operand entry for _dsub: swaps the two double operands with three
// exclusive-ors each and falls through into _dsub, which follows directly. Nothing in
// the program calls it, so the link keeps it with a force_active entry.
// Not compiled: has no return. After swapping the operands it runs on into _dsub, which is placed directly after it.
// A compiled function always ends in a return or a branch: `return _dsub(b, a);` moves the operands through spare
// registers and calls _dsub.
asm void func_0201e1f4(void) {
    eor r1, r1, r3
    eor r3, r1, r3
    eor r1, r1, r3
    eor r0, r0, r2
    eor r2, r0, r2
    eor r0, r0, r2
}

// _dsub: double-precision subtraction a - b (a in r1:r0, b in r3:r2), result in r1:r0.
// Mirror image of _dadd: when the signs differ, b's sign is flipped and control jumps
// into _dadd's magnitude-add body (the instruction at _dadd+0x10, emitted as a raw branch
// word). Otherwise magnitudes are subtracted after ordering the operands so that the
// larger is in r1:r0 (swapping them also flips the result sign). The smaller mantissa is
// aligned by shifting right with a sticky bit; the difference is renormalized with a
// leading-zero count (zero result -> +0), denormal results are shifted back down, and
// the result is rounded to nearest-even. NaN/Inf/zero/denormal inputs are handled as in
// _dadd (NaN -> 0x7fffffffffffffff; Inf - Inf of the same sign and Inf with NaN -> NaN).
// The `dcd` word below is the cross-routine branch (no label can be shared between asm functions).
// Not compiled: part of one hand-written object with _dadd. The `bmi` written as the `dcd` word branches into the
// middle of _dadd (_dadd+0x10), _dadd branches into this routine at _dsub+0x10, and func_0201e1f4 runs into its first
// instruction; a C function can neither branch into another function's body nor be entered anywhere but at its start.
// It also normalises with `clz`, which this compiler never emits: it has no clz intrinsic, and `__builtin_clz` and
// `__clz` compile as calls to an external function.
asm double _dsub(double a, double b) {
    stmfd sp!, {r4, lr}
    // different signs? then flip b's sign and take the add path
    eors ip, r1, r3
    eormi r3, r3, #0x80000000
    dcd 0x4afffd47
    // order the operands by magnitude; a swap also flips the result sign
    subs ip, r0, r2
    sbcs lr, r1, r3
    bhs L_0201e23c
    eor lr, lr, #0x80000000
    adds r2, r2, ip
    adc r3, r3, lr
    subs r0, r0, ip
    sbc r1, r1, lr
    // join point for both subtract and add: unpack the larger magnitude
L_0201e23c:
    mov lr, #0x80000000
    mov ip, r1, lsr #0x14
    orr r1, lr, r1, lsl #0xb
    orr r1, r1, r0, lsr #0x15
    mov r0, r0, lsl #0xb
    movs r4, ip, lsl #0x15
    cmnne r4, #0x200000
    beq L_0201e440
    mov r4, r3, lsr #0x14
    orr r3, lr, r3, lsl #0xb
    orr r3, r3, r2, lsr #0x15
    mov r2, r2, lsl #0xb
    movs lr, r4, lsl #0x15
    beq L_0201e488
    // both normal: align the smaller mantissa and subtract
L_0201e274:
    subs r4, ip, r4
    beq L_0201e31c
    cmp r4, #0x20
    ble L_0201e2b0
    cmp r4, #0x38
    movge r4, #0x3f
    sub r4, r4, #0x20
    rsb lr, r4, #0x20
    orrs lr, r2, r3, lsl lr
    mov r2, r3, lsr r4
    orrne r2, r2, #0x1
    subs r0, r0, r2
    sbcs r1, r1, #0x0
    bmi L_0201e2d8
    b L_0201e3c8
    // difference 1..32
L_0201e2b0:
    rsb lr, r4, #0x20
    movs lr, r2, lsl lr
    rsb lr, r4, #0x20
    mov r2, r2, lsr r4
    orr r2, r2, r3, lsl lr
    mov r3, r3, lsr r4
    orrne r2, r2, #0x1
    subs r0, r0, r2
    sbcs r1, r1, r3
    bpl L_0201e3c8
    // pack: drop 11 low bits, remove hidden bit, insert exponent; round to nearest even
L_0201e2d8:
    movs r2, r0, lsl #0x15
    mov r0, r0, lsr #0xb
    orr r0, r0, r1, lsl #0x15
    add r1, r1, r1
    mov r1, r1, lsr #0xc
    orr r1, r1, ip, lsl #0x14
    tst r2, #0x80000000
    ldmeqfd sp!, {r4, lr}
    bxeq lr
    movs r2, r2, lsl #0x1
    andeqs r2, r0, #0x1
    ldmeqfd sp!, {r4, lr}
    bxeq lr
    adds r0, r0, #0x1
    adc r1, r1, #0x0
    ldmfd sp!, {r4, lr}
    bx lr
    // exponents equal: subtract; zero result gives +0, else renormalize
L_0201e31c:
    subs r0, r0, r2
    sbc r1, r1, r3
    orrs lr, r1, r0
    beq L_0201e5ac
    mov lr, ip, lsl #0x14
    and lr, lr, #0x80000000
    bic ip, ip, #0x800
    cmp r1, #0x0
    bmi L_0201e3a4
    bne L_0201e354
    sub ip, ip, #0x20
    movs r1, r0
    mov r0, #0x0
    bmi L_0201e370
    // leading-zero normalization of the difference (high word nonzero)
L_0201e354:
    clz r4, r1
    movs r1, r1, lsl r4
    rsb r4, r4, #0x20
    orr r1, r1, r0, lsr r4
    rsb r4, r4, #0x20
    mov r0, r0, lsl r4
    sub ip, ip, r4
    // exponent > 0: pack; otherwise the result is denormal: shift right by 12 - exponent
L_0201e370:
    cmp ip, #0x0
    bgt L_0201e3ac
    rsb ip, ip, #0xc
    cmp ip, #0x20
    movge r0, r1
    movge r1, #0x0
    subge ip, ip, #0x20
    rsb r4, ip, #0x20
    mov r0, r0, lsr ip
    orr r0, r0, r1, lsl r4
    orr r1, lr, r1, lsr ip
    ldmfd sp!, {r4, lr}
    bx lr
    // top bit set: already normalized (reached only with r1 negative, so the decrement
    // never applies)
L_0201e3a4:
    cmp r1, #0x0
    subges ip, ip, #0x1
    // pack (positive exponent): remove hidden bit, insert exponent and sign
L_0201e3ac:
    mov r0, r0, lsr #0xb
    orr r0, r0, r1, lsl #0x15
    add r1, r1, r1
    orr r1, lr, r1, lsr #0xc
    orr r1, r1, ip, lsl #0x14
    ldmfd sp!, {r4, lr}
    bx lr
    // result needs renormalization: leading-zero count shift
L_0201e3c8:
    mov lr, ip, lsl #0x14
    and lr, lr, #0x80000000
    bic ip, ip, #0x800
    cmp r1, #0x0
    bne L_0201e3ec
    sub ip, ip, #0x20
    movs r1, r0
    mov r0, #0x0
    bmi L_0201e408
    // leading-zero normalization (high word nonzero)
L_0201e3ec:
    clz r4, r1
    movs r1, r1, lsl r4
    rsb r4, r4, #0x20
    orr r1, r1, r0, lsr r4
    rsb r4, r4, #0x20
    mov r0, r0, lsl r4
    sub ip, ip, r4
    // exponent > 0: pack via the common rounding tail; else denormal shift
L_0201e408:
    cmp ip, #0x0
    orrgt ip, ip, lr, lsr #0x14
    bgt L_0201e2d8
    rsb ip, ip, #0xc
    cmp ip, #0x20
    movge r0, r1
    movge r1, #0x0
    subge ip, ip, #0x20
    rsb r4, ip, #0x20
    mov r0, r0, lsr ip
    orr r0, r0, r1, lsl r4
    orr r1, lr, r1, lsr ip
    ldmfd sp!, {r4, lr}
    bx lr
    // first operand exponent 0 or 0x7ff
L_0201e440:
    cmp ip, #0x800
    movge lr, #0x80000000
    movlt lr, #0x0
    bics ip, ip, #0x800
    beq L_0201e4ac
    orrs r4, r0, r1, lsl #0x1
    bne L_0201e588
    mov r4, r3, lsr #0x14
    mov r3, r3, lsl #0xb
    orr r3, r3, r2, lsr #0x15
    mov r2, r2, lsl #0xb
    movs r4, r4, lsl #0x15
    beq L_0201e574
    cmn r4, #0x200000
    bne L_0201e574
    orrs r4, r2, r3, lsl #0x1
    beq L_0201e59c
    b L_0201e588
    // second operand exponent 0 or 0x7ff
L_0201e488:
    cmp r4, #0x800
    movge lr, #0x80000000
    movlt lr, #0x0
    bic ip, ip, #0x800
    bics r4, r4, #0x800
    beq L_0201e524
    orrs r4, r2, r3, lsl #0x1
    bne L_0201e588
    b L_0201e574
    // first operand denormal or zero
L_0201e4ac:
    orrs r4, r0, r1, lsl #0x1
    beq L_0201e4ec
    mov ip, #0x1
    bic r1, r1, #0x80000000
    mov r4, r3, lsr #0x14
    mov r3, r3, lsl #0xb
    orr r3, r3, r2, lsr #0x15
    mov r2, r2, lsl #0xb
    movs r4, r4, lsl #0x15
    cmnne r4, #0x200000
    mov r4, r4, lsr #0x15
    orr r4, r4, lr, lsr #0x14
    beq L_0201e488
    orr r3, r3, #0x80000000
    orr ip, ip, lr, lsr #0x14
    b L_0201e274
    // first operand is zero: a zero second operand gives +0, a finite nonzero one is returned
    // as is, Inf/NaN give +-Inf/NaN
L_0201e4ec:
    mov ip, r3, lsr #0x14
    mov r1, r3, lsl #0xb
    orr r1, r1, r2, lsr #0x15
    mov r0, r2, lsl #0xb
    movs r4, ip, lsl #0x15
    beq L_0201e518
    cmn r4, #0x200000
    bne L_0201e540
    orrs r4, r0, r1, lsl #0x1
    bne L_0201e58c
    b L_0201e574
    // second operand exponent 0: zero gives +0, denormal is returned as is
L_0201e518:
    orrs r4, r0, r1, lsl #0x1
    beq L_0201e5ac
    b L_0201e540
    // second operand zero or denormal
L_0201e524:
    orrs r4, r2, r3, lsl #0x1
    beq L_0201e550
    mov r4, #0x1
    bic r3, r3, #0x80000000
    orr ip, ip, lr, lsr #0x14
    orr r4, r4, lr, lsr #0x14
    b L_0201e274
    // return the second operand unchanged
L_0201e540:
    mov r1, r3
    mov r0, r2
    ldmfd sp!, {r4, lr}
    bx lr
    // second operand is zero: repack first
L_0201e550:
    cmp r1, #0x0
    subges ip, ip, #0x1
    mov r0, r0, lsr #0xb
    orr r0, r0, r1, lsl #0x15
    add r1, r1, r1
    orr r1, lr, r1, lsr #0xc
    orr r1, r1, ip, lsl #0x14
    ldmfd sp!, {r4, lr}
    bx lr
    // signed infinity
L_0201e574:
    ldr r1, =0x7ff00000
    orr r1, lr, r1
    mov r0, #0x0
    ldmfd sp!, {r4, lr}
    bx lr
    // NaN
L_0201e588:
    mov r1, r3
    // NaN (all-ones mantissa)
L_0201e58c:
    mvn r0, #0x0
    bic r1, r0, #0x80000000
    ldmfd sp!, {r4, lr}
    bx lr
    // NaN (Inf - Inf)
L_0201e59c:
    mvn r0, #0x0
    bic r1, r0, #0x80000000
    ldmfd sp!, {r4, lr}
    bx lr
    // +0
L_0201e5ac:
    mov r1, #0x0
    mov r0, #0x0
    ldmfd sp!, {r4, lr}
    bx lr
}

// _fadd: single-precision addition, a in r0, b in r1, result r0.
// Operands with different signs get b's sign flipped and control jumps into the
// magnitude-subtract body of _fsub (at _fsub+0xc, emitted as a raw branch word). Otherwise
// the larger magnitude is placed in r0, both mantissas are left-justified with the hidden
// bit at bit 31, b is shifted right by the exponent difference keeping a sticky bit,
// the mantissas are added (a carry shifts right by one and bumps the exponent; exponent
// 255 gives +-Inf), and the 8 dropped bits are rounded to nearest, ties to even.
// NaN -> 0x7fffffff, Inf + finite or Inf + Inf -> Inf, a zero operand returns the other
// one, denormal inputs are treated as exponent 1 without hidden bit.
// The `dcd` word below is the cross-routine branch (no label can be shared between asm functions).
// Not compiled: part of one hand-written object with _fsub. The `bmi` written as the `dcd` word branches into the
// middle of _fsub (_fsub+0xc), and _fsub branches back into this routine at _fadd+0xc; a C function can neither
// branch into another function's body nor be entered anywhere but at its start.
asm float _fadd(float a, float b) {
    // different signs? flip b and take the subtract path
    eors r2, r0, r1
    eormi r1, r1, #0x80000000
    dcd 0x4a0003cb
    // order by magnitude (swap when |a| < |b|)
    subs ip, r0, r1
    sublo r0, r0, ip
    addlo r1, r1, ip
    // unpack a: r3 = sign|exponent, r0 = mantissa with hidden bit at bit 31
    mov r2, #0x80000000
    mov r3, r0, lsr #0x17
    orr r0, r2, r0, lsl #0x8
    ands ip, r3, #0xff
    cmpne ip, #0xff
    beq L_0201e660
    mov ip, r1, lsr #0x17
    orr r1, r2, r1, lsl #0x8
    ands r2, ip, #0xff
    beq L_0201e6a0
    // both normal: b's mantissa shifted right by the exponent difference, with sticky
L_0201e600:
    subs ip, r3, ip
    beq L_0201e618
    rsb r2, ip, #0x20
    movs r2, r1, lsl r2
    mov r1, r1, lsr ip
    orrne r1, r1, #0x1
    // add mantissas; carry shifts right by one and bumps the exponent
L_0201e618:
    adds r0, r0, r1
    blo L_0201e638
    and r1, r0, #0x1
    orr r0, r1, r0, rrx
    add r3, r3, #0x1
    and r2, r3, #0xff
    cmp r2, #0xff
    beq L_0201e7a8
    // pack: remove hidden bit, insert exponent, round to nearest even using the low 8 bits
L_0201e638:
    ands r1, r0, #0xff
    add r0, r0, r0
    mov r0, r0, lsr #0x9
    orr r0, r0, r3, lsl #0x17
    tst r1, #0x80
    bxeq lr
    ands r1, r1, #0x7f
    andeqs r1, r0, #0x1
    addne r0, r0, #0x1
    bx lr
    // a has exponent 0 or 255; r2 = its sign
L_0201e660:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0x0
    ands r3, r3, #0xff
    beq L_0201e6c4
    movs r0, r0, lsl #0x1
    bne L_0201e7d4
    mov ip, r1, lsr #0x17
    mov r1, r1, lsl #0x9
    ands ip, ip, #0xff
    beq L_0201e7c8
    cmp ip, #0xff
    blt L_0201e7c8
    cmp r1, #0x0
    beq L_0201e7c8
    b L_0201e7d4
    // b has exponent 0 or 255
L_0201e6a0:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0x0
    and r3, r3, #0xff
    ands ip, ip, #0xff
    beq L_0201e720
    // b is Inf/NaN: NaN if its mantissa is nonzero else Inf
L_0201e6b8:
    movs r1, r1, lsl #0x1
    bne L_0201e7d4
    b L_0201e7c8
    // a is zero or denormal
L_0201e6c4:
    movs r0, r0, lsl #0x1
    beq L_0201e6fc
    mov r3, #0x1
    mov r0, r0, lsr #0x1
    mov ip, r1, lsr #0x17
    mov r1, r1, lsl #0x8
    ands ip, ip, #0xff
    beq L_0201e720
    cmp ip, #0xff
    beq L_0201e6b8
    orr r1, r1, #0x80000000
    orr r3, r3, r2, lsr #0x17
    orr ip, ip, r2, lsr #0x17
    b L_0201e600
    // a is zero: result is b
L_0201e6fc:
    mov r3, r1, lsr #0x17
    mov r0, r1, lsl #0x9
    ands r3, r3, #0xff
    beq L_0201e788
    cmp r3, #0xff
    blt L_0201e788
    cmp r0, #0x0
    beq L_0201e7c8
    b L_0201e7c0
    // b is zero or denormal
L_0201e720:
    movs r1, r1, lsl #0x1
    beq L_0201e790
    mov r1, r1, lsr #0x1
    mov ip, #0x1
    orr r3, r3, r2, lsr #0x17
    orr ip, ip, r2, lsr #0x17
    cmp r0, #0x0
    bmi L_0201e600
    adds r0, r0, r1
    blo L_0201e754
    and r1, r0, #0x1
    orr r0, r1, r0, rrx
    add ip, ip, #0x1
    // pack (exponent 0 unless the sum reached bit 31), round to nearest even
L_0201e754:
    cmp r0, #0x0
    subge ip, ip, #0x1
    ands r1, r0, #0xff
    add r0, r0, r0
    mov r0, r0, lsr #0x9
    orr r0, r0, ip, lsl #0x17
    bxeq lr
    tst r1, #0x80
    bxeq lr
    ands r1, r1, #0x7f
    andeqs r1, r0, #0x1
    addne r0, r0, #0x1
    bx lr
    // return b
L_0201e788:
    mov r0, r1
    bx lr
    // b is zero: repack a
L_0201e790:
    cmp r0, #0x0
    subges r3, r3, #0x1
    add r0, r0, r0
    orr r0, r2, r0, lsr #0x9
    orr r0, r0, r3, lsl #0x17
    bx lr
    // exponent overflow: signed Inf
L_0201e7a8:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0x0
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #0x1
    bx lr
    // NaN
L_0201e7c0:
    mvn r0, #0x80000000
    bx lr
    // signed infinity
L_0201e7c8:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #0x1
    bx lr
    // NaN
L_0201e7d4:
    mvn r0, #0x80000000
    bx lr
}

// func_0201e7dc: returns the constant 0x7fffffff (the NaN pattern used by the float
// routines above); no arguments are read.
int func_0201e7dc(void) {
    return 0x7fffffff;
}

// _dgeq: returns 1 if a >= b (doubles a in r1:r0, b in r3:r2), else 0.
// A NaN in either operand gives 0 (unordered) and clears the C flag; +0 and -0 compare
// equal (C set). Non-negative operands compare as unsigned 64-bit values; if either is
// negative the raw bits are compared in reverse order, which is correct for two negatives
// and for mixed signs. The flags are left as the result of the final comparison.
// Not compiled: sets or clears the C flag with `mrs ip, cpsr` / `msr cpsr_f, ip`, which C has no expression for. The
// result is returned in the flags as well as in r0, which no C calling convention provides.
asm int _dgeq(double a, double b) {
    mov ip, #0x200000
    cmn ip, r1, lsl #0x1
    bhs L_0201e858
    cmn ip, r3, lsl #0x1
    bhs L_0201e86c
    // ordered: compare
L_0201e7f8:
    orrs ip, r3, r1
    bmi L_0201e828
    cmp r1, r3
    cmpeq r0, r2
    movhs r0, #0x1
    movlo r0, #0x0
    bx lr
    // unordered (NaN)
L_0201e814:
    mov r0, #0x0
    mrs ip, cpsr
    bic ip, ip, #0x20000000
    msr cpsr_f, ip
    bx lr
    // at least one negative: equal zeros check, then reversed compare
L_0201e828:
    orr ip, r0, ip, lsl #0x1
    orrs ip, ip, r2
    moveq r0, #0x1
    mrs ip, cpsr
    orr ip, ip, #0x20000000
    msr cpsr_f, ip
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    movhs r0, #0x1
    movlo r0, #0x0
    bx lr
    // first operand has exponent 0x7ff: NaN if mantissa nonzero
L_0201e858:
    bne L_0201e814
    cmp r0, #0x0
    bhi L_0201e814
    cmn ip, r3, lsl #0x1
    blo L_0201e7f8
    // second operand has exponent 0x7ff: NaN if mantissa nonzero
L_0201e86c:
    bne L_0201e814
    cmp r2, #0x0
    bhi L_0201e814
    b L_0201e7f8
}

// _dgr: returns 1 if a > b (doubles), else 0. Same structure as _dgeq; unordered gives 0
// with C cleared, and the +0 == -0 case gives 0 with C cleared.
// Not compiled: clears the C flag with `mrs ip, cpsr` / `msr cpsr_f, ip`, which C has no expression for. The result
// is returned in the flags as well as in r0, which no C calling convention provides.
asm int _dgr(double a, double b) {
    mov ip, #0x200000
    cmn ip, r1, lsl #0x1
    bhs L_0201e8f0
    cmn ip, r3, lsl #0x1
    bhs L_0201e904
    // ordered: compare
L_0201e890:
    orrs ip, r3, r1
    bmi L_0201e8c0
    cmp r1, r3
    cmpeq r0, r2
    movhi r0, #0x1
    movls r0, #0x0
    bx lr
    // unordered (NaN)
L_0201e8ac:
    mov r0, #0x0
    mrs ip, cpsr
    bic ip, ip, #0x20000000
    msr cpsr_f, ip
    bx lr
    // at least one negative: equal zeros check, then reversed compare
L_0201e8c0:
    orr ip, r0, ip, lsl #0x1
    orrs ip, ip, r2
    moveq r0, #0x0
    mrs ip, cpsr
    bic ip, ip, #0x20000000
    msr cpsr_f, ip
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    movhi r0, #0x1
    movls r0, #0x0
    bx lr
    // first operand has exponent 0x7ff: NaN if mantissa nonzero
L_0201e8f0:
    bne L_0201e8ac
    cmp r0, #0x0
    bhi L_0201e8ac
    cmn ip, r3, lsl #0x1
    blo L_0201e890
    // second operand has exponent 0x7ff: NaN if mantissa nonzero
L_0201e904:
    bne L_0201e8ac
    cmp r2, #0x0
    bhi L_0201e8ac
    b L_0201e890
}

// _dleq: returns 1 if a <= b (doubles), else 0. Same structure as _dgeq.
// Unordered gives 0 with Z cleared and C set; equal zeros give 1 with Z set and C cleared.
// Not compiled: sets or clears the C and Z flags with `mrs ip, cpsr` / `msr cpsr_f, ip`, which C has no expression
// for. The result is returned in the flags as well as in r0, which no C calling convention provides.
asm int _dleq(double a, double b) {
    mov ip, #0x200000
    cmn ip, r1, lsl #0x1
    bhs L_0201e994
    cmn ip, r3, lsl #0x1
    bhs L_0201e9a8
    // ordered: compare
L_0201e928:
    orrs ip, r3, r1
    bmi L_0201e95c
    cmp r1, r3
    cmpeq r0, r2
    movls r0, #0x1
    movhi r0, #0x0
    bx lr
    // unordered (NaN)
L_0201e944:
    mov r0, #0x0
    mrs ip, cpsr
    bic ip, ip, #0x40000000
    orr ip, ip, #0x20000000
    msr cpsr_f, ip
    bx lr
    // at least one negative: equal zeros check, then reversed compare
L_0201e95c:
    orr ip, r0, ip, lsl #0x1
    orrs ip, ip, r2
    moveq r0, #0x1
    bne L_0201e980
    mrs ip, cpsr
    bic ip, ip, #0x20000000
    orr ip, ip, #0x40000000
    msr cpsr_f, ip
    bxeq lr
    // compare reversed (negative operands)
L_0201e980:
    cmp r3, r1
    cmpeq r2, r0
    movls r0, #0x1
    movhi r0, #0x0
    bx lr
    // first operand has exponent 0x7ff: NaN if mantissa nonzero
L_0201e994:
    bne L_0201e944
    cmp r0, #0x0
    bhi L_0201e944
    cmn ip, r3, lsl #0x1
    blo L_0201e928
    // second operand has exponent 0x7ff: NaN if mantissa nonzero
L_0201e9a8:
    bne L_0201e944
    cmp r2, #0x0
    bhi L_0201e944
    b L_0201e928
}

// _dls: returns 1 if a < b (doubles), else 0. Same structure as _dgeq.
// Unordered and the +0/-0 case give 0 with C set.
// Not compiled: sets the C flag with `mrs ip, cpsr` / `msr cpsr_f, ip`, which C has no expression for. The result is
// returned in the flags as well as in r0, which no C calling convention provides.
asm int _dls(double a, double b) {
    mov ip, #0x200000
    cmn ip, r1, lsl #0x1
    bhs L_0201ea30
    cmn ip, r3, lsl #0x1
    bhs L_0201ea44
    // ordered: compare
L_0201e9cc:
    orrs ip, r3, r1
    bmi L_0201e9fc
    cmp r1, r3
    cmpeq r0, r2
    movlo r0, #0x1
    movhs r0, #0x0
    bx lr
    // unordered (NaN)
L_0201e9e8:
    mov r0, #0x0
    mrs ip, cpsr
    orr ip, ip, #0x20000000
    msr cpsr_f, ip
    bx lr
    // at least one negative: equal zeros check, then reversed compare
L_0201e9fc:
    orr ip, r0, ip, lsl #0x1
    orrs ip, ip, r2
    moveq r0, #0x0
    bne L_0201ea1c
    mrs ip, cpsr
    orr ip, ip, #0x20000000
    msr cpsr_f, ip
    bxeq lr
    // compare reversed (negative operands)
L_0201ea1c:
    cmp r3, r1
    cmpeq r2, r0
    movlo r0, #0x1
    movhs r0, #0x0
    bx lr
    // first operand has exponent 0x7ff: NaN if mantissa nonzero
L_0201ea30:
    bne L_0201e9e8
    cmp r0, #0x0
    bhi L_0201e9e8
    cmn ip, r3, lsl #0x1
    blo L_0201e9cc
    // second operand has exponent 0x7ff: NaN if mantissa nonzero
L_0201ea44:
    bne L_0201e9e8
    cmp r2, #0x0
    bhi L_0201e9e8
    b L_0201e9cc
}

// _deq: returns 1 if a == b (doubles), else 0. NaN gives 0 with Z cleared; +0 equals -0;
// otherwise both words must be equal.
// Not compiled: clears the Z flag with `mrs ip, cpsr` / `msr cpsr_f, ip`, which C has no expression for. The result
// is returned in the flags as well as in r0, which no C calling convention provides.
asm int _deq(double a, double b) {
    mov ip, #0x200000
    cmn ip, r1, lsl #0x1
    bhs L_0201eabc
    cmn ip, r3, lsl #0x1
    bhs L_0201ead0
    // ordered: compare
L_0201ea68:
    orrs ip, r3, r1
    bmi L_0201ea98
    cmp r1, r3
    cmpeq r0, r2
    moveq r0, #0x1
    movne r0, #0x0
    bx lr
    // unordered (NaN)
L_0201ea84:
    mov r0, #0x0
    mrs ip, cpsr
    bic ip, ip, #0x40000000
    msr cpsr_f, ip
    bx lr
    // at least one negative: equal zeros check, then reversed compare
L_0201ea98:
    orr ip, r0, ip, lsl #0x1
    orrs ip, ip, r2
    moveq r0, #0x1
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    moveq r0, #0x1
    movne r0, #0x0
    bx lr
    // first operand has exponent 0x7ff: NaN if mantissa nonzero
L_0201eabc:
    bne L_0201ea84
    cmp r0, #0x0
    bhi L_0201ea84
    cmn ip, r3, lsl #0x1
    blo L_0201ea68
    // second operand has exponent 0x7ff: NaN if mantissa nonzero
L_0201ead0:
    bne L_0201ea84
    cmp r2, #0x0
    bhi L_0201ea84
    b L_0201ea68
}

// _dneq: returns 1 if a != b (doubles), else 0. NaN gives 1 with Z cleared;
// +0 and -0 are equal (0).
// Not compiled: clears the Z flag with `mrs ip, cpsr` / `msr cpsr_f, ip`, which C has no expression for. The result
// is returned in the flags as well as in r0, which no C calling convention provides.
asm int _dneq(double a, double b) {
    mov ip, #0x200000
    cmn ip, r1, lsl #0x1
    bhs L_0201eb48
    cmn ip, r3, lsl #0x1
    bhs L_0201eb5c
    // ordered: compare
L_0201eaf4:
    orrs ip, r3, r1
    bmi L_0201eb24
    cmp r1, r3
    cmpeq r0, r2
    movne r0, #0x1
    moveq r0, #0x0
    bx lr
    // unordered (NaN)
L_0201eb10:
    mov r0, #0x1
    mrs ip, cpsr
    bic ip, ip, #0x40000000
    msr cpsr_f, ip
    bx lr
    // at least one negative: equal zeros check, then reversed compare
L_0201eb24:
    orr ip, r0, ip, lsl #0x1
    orrs ip, ip, r2
    moveq r0, #0x0
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    movne r0, #0x1
    moveq r0, #0x0
    bx lr
    // first operand has exponent 0x7ff: NaN if mantissa nonzero
L_0201eb48:
    bne L_0201eb10
    cmp r0, #0x0
    bhi L_0201eb10
    cmn ip, r3, lsl #0x1
    blo L_0201eaf4
    // second operand has exponent 0x7ff: NaN if mantissa nonzero
L_0201eb5c:
    bne L_0201eb10
    cmp r2, #0x0
    bhi L_0201eb10
    b L_0201eaf4
}

// _fgeq: returns 1 if a >= b (floats a in r0, b in r1), else 0; C cleared when unordered.
// A NaN (magnitude above 0xff000000 when shifted left once) in either operand gives 0.
// Otherwise each operand is converted from sign-magnitude to two's complement and the two
// are compared as signed integers, so +0 and -0 compare equal. The flags are left set to
// match the returned condition.
// Not compiled: sets or clears the C flag with `mrs ip, cpsr` / `msr cpsr_f, ip`, which C has no expression for. The
// result is returned in the flags as well as in r0, which no C calling convention provides.
asm int _fgeq(float a, float b) {
    mov r3, #0xff000000
    cmp r3, r0, lsl #0x1
    cmphs r3, r1, lsl #0x1
    blo L_0201ebb4
    // sign-magnitude to two's complement for each operand
    cmp r0, #0x0
    bicmi r0, r0, #0x80000000
    rsbmi r0, r0, #0x0
    cmp r1, #0x0
    bicmi r1, r1, #0x80000000
    rsbmi r1, r1, #0x0
    cmp r0, r1
    movge r0, #0x1
    movlt r0, #0x0
    mrs ip, cpsr
    biclt ip, ip, #0x20000000
    orrge ip, ip, #0x20000000
    msr cpsr_f, ip
    bx lr
    // unordered (NaN) result
L_0201ebb4:
    mov r0, #0x0
    mrs ip, cpsr
    bic ip, ip, #0x20000000
    msr cpsr_f, ip
    bx lr
}

// _fgr: returns 1 if a > b (floats a in r0, b in r1), else 0; C cleared when unordered.
// A NaN (magnitude above 0xff000000 when shifted left once) in either operand gives 0.
// Otherwise each operand is converted from sign-magnitude to two's complement and the two
// are compared as signed integers, so +0 and -0 compare equal. The flags are left set to
// match the returned condition.
// Not compiled: sets or clears the C flag with `mrs ip, cpsr` / `msr cpsr_f, ip`, which C has no expression for. The
// result is returned in the flags as well as in r0, which no C calling convention provides.
asm int _fgr(float a, float b) {
    mov r3, #0xff000000
    cmp r3, r0, lsl #0x1
    cmphs r3, r1, lsl #0x1
    blo L_0201ec10
    // sign-magnitude to two's complement for each operand
    cmp r0, #0x0
    bicmi r0, r0, #0x80000000
    rsbmi r0, r0, #0x0
    cmp r1, #0x0
    bicmi r1, r1, #0x80000000
    rsbmi r1, r1, #0x0
    cmp r0, r1
    movgt r0, #0x1
    movle r0, #0x0
    mrs ip, cpsr
    bicle ip, ip, #0x20000000
    orrgt ip, ip, #0x20000000
    msr cpsr_f, ip
    bx lr
    // unordered (NaN) result
L_0201ec10:
    mov r0, #0x0
    mrs ip, cpsr
    bic ip, ip, #0x20000000
    msr cpsr_f, ip
    bx lr
}

// _fleq: returns 1 if a <= b (floats a in r0, b in r1), else 0; unordered: C set, Z cleared.
// A NaN (magnitude above 0xff000000 when shifted left once) in either operand gives 0.
// Otherwise each operand is converted from sign-magnitude to two's complement and the two
// are compared as signed integers, so +0 and -0 compare equal. The flags are left set to
// match the returned condition.
// Not compiled: sets or clears the C and Z flags with `mrs ip, cpsr` / `msr cpsr_f, ip`, which C has no expression
// for. The result is returned in the flags as well as in r0, which no C calling convention provides.
asm int _fleq(float a, float b) {
    mov r3, #0xff000000
    cmp r3, r0, lsl #0x1
    cmphs r3, r1, lsl #0x1
    blo L_0201ec74
    // sign-magnitude to two's complement for each operand
    cmp r0, #0x0
    bicmi r0, r0, #0x80000000
    rsbmi r0, r0, #0x0
    cmp r1, #0x0
    bicmi r1, r1, #0x80000000
    rsbmi r1, r1, #0x0
    cmp r0, r1
    movle r0, #0x1
    movgt r0, #0x0
    mrs ip, cpsr
    orrgt ip, ip, #0x20000000
    bicgt ip, ip, #0x40000000
    bicle ip, ip, #0x20000000
    orrle ip, ip, #0x40000000
    msr cpsr_f, ip
    bx lr
    // unordered (NaN) result
L_0201ec74:
    mov r0, #0x0
    mrs ip, cpsr
    bic ip, ip, #0x40000000
    orr ip, ip, #0x20000000
    msr cpsr_f, ip
    bx lr
}

// _fls: returns 1 if a < b (floats a in r0, b in r1), else 0; unordered: C set.
// A NaN (magnitude above 0xff000000 when shifted left once) in either operand gives 0.
// Otherwise each operand is converted from sign-magnitude to two's complement and the two
// are compared as signed integers, so +0 and -0 compare equal. The flags are left set to
// match the returned condition.
// Not compiled: sets or clears the C flag with `mrs ip, cpsr` / `msr cpsr_f, ip`, which C has no expression for. The
// result is returned in the flags as well as in r0, which no C calling convention provides.
asm int _fls(float a, float b) {
    mov r3, #0xff000000
    cmp r3, r0, lsl #0x1
    cmphs r3, r1, lsl #0x1
    blo L_0201ecd4
    // sign-magnitude to two's complement for each operand
    cmp r0, #0x0
    bicmi r0, r0, #0x80000000
    rsbmi r0, r0, #0x0
    cmp r1, #0x0
    bicmi r1, r1, #0x80000000
    rsbmi r1, r1, #0x0
    cmp r0, r1
    movlt r0, #0x1
    movge r0, #0x0
    mrs ip, cpsr
    orrge ip, ip, #0x20000000
    biclt ip, ip, #0x20000000
    msr cpsr_f, ip
    bx lr
    // unordered (NaN) result
L_0201ecd4:
    mov r0, #0x0
    mrs ip, cpsr
    orr ip, ip, #0x20000000
    msr cpsr_f, ip
    bx lr
}

// _feq: returns 1 if a == b (floats), else 0. NaN -> 0 with Z cleared. Two zeros of either
// sign are equal (Z set); otherwise the raw bit patterns are compared.
// Not compiled: sets or clears the Z flag with `mrs ip, cpsr` / `msr cpsr_f, ip`, which C has no expression for. The
// result is returned in the flags as well as in r0, which no C calling convention provides.
asm int _feq(float a, float b) {
    mov r3, #0xff000000
    cmp r3, r0, lsl #0x1
    blo L_0201ed3c
    cmp r3, r1, lsl #0x1
    blo L_0201ed3c
    // both zero (ignoring sign)?
    orr r3, r0, r1
    movs r3, r3, lsl #0x1
    moveq r0, #0x1
    bne L_0201ed1c
    mrs ip, cpsr
    orr ip, ip, #0x40000000
    msr cpsr_f, ip
    bx lr
    // not both zero: compare raw bits
L_0201ed1c:
    cmp r0, r1
    moveq r0, #0x1
    movne r0, #0x0
    mrs ip, cpsr
    orreq ip, ip, #0x40000000
    bicne ip, ip, #0x40000000
    msr cpsr_f, ip
    bx lr
    // unordered (NaN) result
L_0201ed3c:
    mov r0, #0x0
    mrs ip, cpsr
    bic ip, ip, #0x40000000
    msr cpsr_f, ip
    bx lr
}

// _fneq: returns 1 if a != b (floats), else 0. NaN -> 1 with Z cleared. Two zeros of either
// sign are equal (returns 0); otherwise the raw bit patterns are compared.
// Not compiled: sets or clears the Z flag with `mrs ip, cpsr` / `msr cpsr_f, ip`, which C has no expression for. The
// result is returned in the flags as well as in r0, which no C calling convention provides.
asm int _fneq(float a, float b) {
    mov r3, #0xff000000
    cmp r3, r0, lsl #0x1
    blo L_0201eda4
    cmp r3, r1, lsl #0x1
    blo L_0201eda4
    // both zero (ignoring sign)?
    orr r3, r0, r1
    movs r3, r3, lsl #0x1
    moveq r0, #0x0
    bne L_0201ed84
    mrs ip, cpsr
    orr ip, ip, #0x40000000
    msr cpsr_f, ip
    bx lr
    // not both zero: compare raw bits
L_0201ed84:
    cmp r0, r1
    movne r0, #0x1
    moveq r0, #0x0
    mrs ip, cpsr
    bicne ip, ip, #0x40000000
    orreq ip, ip, #0x40000000
    msr cpsr_f, ip
    bx lr
    // unordered (NaN): not equal
L_0201eda4:
    mov r0, #0x1
    mrs ip, cpsr
    bic ip, ip, #0x40000000
    msr cpsr_f, ip
    bx lr
}

// func_0201edb8: reverse-operand entry for _fdiv: swaps the two float operands with three
// exclusive-ors each and falls through into _fdiv, which follows directly. Nothing in
// the program calls it, so the link keeps it with a force_active entry.
// Not compiled: has no return. After swapping the operands it runs on into _fdiv, which is placed directly after it.
// A compiled function always ends in a return or a branch: `return _fdiv(b, a);` moves the operands through a spare
// register and ends in a tail call (`ldr ip, =_fdiv; bx ip`).
asm void func_0201edb8(void) {
    eor r0, r0, r1
    eor r1, r0, r1
    eor r0, r0, r1
}

// Single-precision divide/convert/multiply/subtract, the 64-bit and 32-bit integer dividers and
// the double-precision divide.
//
// Notes:
//  - A few routines branch into the middle of a neighbouring routine (_fsub into _fadd+0xc,
//    func_0201f768 into _ll_sdiv+0x10, _ll_udiv into _ull_mod+0x8, and _ull_mod into the
//    64-bit divide inside _ll_sdiv and into _u32_div_f past its zero test).
//    Labels cannot be shared between asm functions, so those branches are written as raw
//    instruction words (`dcd`) with the encoded displacement; the addresses are fixed.
//  - func_0201f768 and _ll_udiv are short entry stubs: they set a mode flag in r4 and branch
//    into the bodies of _ll_sdiv and _ull_mod.
//  - _fdiv and _ddiv carry a 256-byte lookup table (initial reciprocal estimates) in the
//    middle of the code, read with a pc-relative address.

extern int _s32_div_f(int a, int b);

// _fdiv: single-precision divide, r0 = a / b (a in r0, b in r1), round to nearest even.
// Result sign is a^b. Normal case: both mantissas get the implicit bit; if a's mantissa is
// smaller than b's it is doubled and the exponent drops by one. The quotient is formed from a
// reciprocal of b: an initial estimate comes from a 256-entry byte table indexed by the top
// bits of b's mantissa, then refined with a few multiply steps, and finally multiplied by a's
// mantissa (umull). Exponent = ea - eb + bias; underflow goes to the denormal path, overflow to
// infinity. When the low bits of the product are in the ambiguous range, the remainder is
// recomputed exactly to decide the last rounding bit.
// Special cases: NaN operand, 0/0 and inf/inf return the quiet NaN 0x7fffffff; x/0 and
// inf/finite return a signed infinity; 0/finite and finite/inf return a signed zero. Denormal
// operands are normalised with clz first.
// Not compiled: keeps its 256-byte reciprocal table inside its own code and addresses it with `sub r0, pc, #0x94` (pc
// read as data), and normalises denormal operands with `clz`, which this compiler never emits: it has no clz
// intrinsic, and `__builtin_clz` and `__clz` compile as calls to an external function. func_0201edb8 runs into its
// first instruction.
asm float _fdiv(float a, float b) {
    stmdb sp!, {lr}
    mov ip, #0xff
    ands r3, ip, r0, lsr #0x17
    cmpne r3, #0xff
    beq L_0201ef98
    ands ip, ip, r1, lsr #0x17
    cmpne ip, #0xff
    beq L_0201efd4
    orr r1, r1, #0x800000
    orr r0, r0, #0x800000
    bic r2, r0, #0xff000000
    bic lr, r1, #0xff000000
    // both operands are now normalised mantissas (a in r2, b in lr) with exponents in r3 / ip
L_0201edf4:
    cmp r2, lr
    movlo r2, r2, lsl #0x1
    sublo r3, r3, #0x1
    teq r0, r1
    sub r0, pc, #0x94
    ldrb r1, [r0, lr, lsr #0xf]
    rsb lr, lr, #0x0
    mov r0, lr, asr #0x1
    mul r0, r1, r0
    add r0, r0, #0x80000000
    mov r0, r0, lsr #0x6
    mul r0, r1, r0
    mov r0, r0, lsr #0xe
    mul r1, lr, r0
    sub ip, r3, ip
    mov r1, r1, lsr #0xc
    mul r1, r0, r1
    mov r0, r0, lsl #0xe
    add r0, r0, r1, lsr #0xf
    umull r1, r0, r2, r0
    mov r3, r0
    orrmi r0, r0, #0x80000000
    adds ip, ip, #0x7e
    bmi L_0201f09c
    cmp ip, #0xfe
    bge L_0201f150
    add r0, r0, ip, lsl #0x17
    mov ip, r1, lsr #0x1c
    cmp ip, #0x7
    beq L_0201ef78
    add r0, r0, r1, lsr #0x1f
    ldmia sp!, {lr}
    bx lr
    // reciprocal-estimate table (256 bytes), addressed pc-relative above
L_0201ee78:
    dcd 0xfdfeffff
    dcd 0xf9fafbfc
    dcd 0xf5f6f7f8
    dcd 0xf1f2f3f4
    dcd 0xeeeff0f0
    dcd 0xeaebeced
    dcd 0xe7e8e9ea
    dcd 0xe4e5e6e6
    dcd 0xe1e2e2e3
    dcd 0xdedfdfe0
    dcd 0xdbdcdcdd
    dcd 0xd8d9d9da
    dcd 0xd5d6d7d7
    dcd 0xd2d3d4d4
    dcd 0xd0d0d1d2
    dcd 0xcdcececf
    dcd 0xcbcbcccc
    dcd 0xc8c9c9ca
    dcd 0xc6c6c7c8
    dcd 0xc3c4c5c5
    dcd 0xc1c2c2c3
    dcd 0xbfbfc0c0
    dcd 0xbdbdbebe
    dcd 0xbabbbcbc
    dcd 0xb8b9b9ba
    dcd 0xb6b7b7b8
    dcd 0xb4b5b5b6
    dcd 0xb2b3b3b4
    dcd 0xb0b1b1b2
    dcd 0xafafafb0
    dcd 0xadadaeae
    dcd 0xababacac
    dcd 0xa9aaaaaa
    dcd 0xa7a8a8a9
    dcd 0xa6a6a7a7
    dcd 0xa4a4a5a5
    dcd 0xa2a3a3a4
    dcd 0xa1a1a2a2
    dcd 0x9fa0a0a0
    dcd 0x9e9e9e9f
    dcd 0x9c9d9d9d
    dcd 0x9b9b9b9c
    dcd 0x999a9a9a
    dcd 0x98989999
    dcd 0x96979798
    dcd 0x95959696
    dcd 0x94949495
    dcd 0x92939393
    dcd 0x91919292
    dcd 0x90909191
    dcd 0x8f8f8f90
    dcd 0x8d8e8e8e
    dcd 0x8c8c8d8d
    dcd 0x8b8b8c8c
    dcd 0x8a8a8a8b
    dcd 0x8989898a
    dcd 0x88888888
    dcd 0x86878787
    dcd 0x85868686
    dcd 0x84858585
    dcd 0x83838484
    dcd 0x82828383
    dcd 0x81818282
    dcd 0x80808181
    // near-tie: check the exact remainder to decide whether to round up
L_0201ef78:
    mov r1, r3, lsl #0x1
    add r1, r1, #0x1
    rsb lr, lr, #0x0
    mul r1, lr, r1
    cmp r1, r2, lsl #0x18
    addmi r0, r0, #0x1
    ldmia sp!, {lr}
    bx lr
    // a has exponent 0 or 255
L_0201ef98:
    eor lr, r0, r1
    and lr, lr, #0x80000000
    cmp r3, #0x0
    beq L_0201eff0
    movs r0, r0, lsl #0x9
    bne L_0201f138
    mov ip, r1, lsr #0x17
    mov r1, r1, lsl #0x9
    ands ip, ip, #0xff
    beq L_0201f128
    cmp ip, #0xff
    blt L_0201f128
    cmp r1, #0x0
    beq L_0201f144
    b L_0201f120
    // a is normal, b has exponent 0 or 255
L_0201efd4:
    eor lr, r0, r1
    and lr, lr, #0x80000000
    cmp ip, #0x0
    beq L_0201f054
L_0201efe4:
    movs r1, r1, lsl #0x9
    bne L_0201f120
    b L_0201f170
    // a has exponent 0: zero or denormal
L_0201eff0:
    movs r2, r0, lsl #0x9
    beq L_0201f024
    clz r3, r2
    movs r2, r2, lsl r3
    rsb r3, r3, #0x0
    mov r2, r2, lsr #0x8
    ands ip, ip, r1, lsr #0x17
    beq L_0201f07c
    cmp ip, #0xff
    beq L_0201efe4
    orr r1, r1, #0x800000
    bic lr, r1, #0xff000000
    b L_0201edf4
    // a is zero: result is zero unless b is zero or NaN
L_0201f024:
    mov ip, r1, lsr #0x17
    mov r1, r1, lsl #0x9
    ands ip, ip, #0xff
    beq L_0201f048
    cmp ip, #0xff
    blt L_0201f170
    cmp r1, #0x0
    beq L_0201f170
    b L_0201f120
L_0201f048:
    cmp r1, #0x0
    beq L_0201f144
    b L_0201f170
    // b is zero or denormal: zero gives infinity, else normalise b
L_0201f054:
    movs ip, r1, lsl #0x9
    beq L_0201f128
    mov lr, ip
    clz ip, lr
    movs lr, lr, lsl ip
    rsb ip, ip, #0x0
    mov lr, lr, lsr #0x8
    orr r0, r0, #0x800000
    bic r2, r0, #0xff000000
    b L_0201edf4
    // a already normalised, b is zero or denormal
L_0201f07c:
    movs ip, r1, lsl #0x9
    beq L_0201f128
    mov lr, ip
    clz ip, lr
    movs lr, lr, lsl ip
    rsb ip, ip, #0x0
    mov lr, lr, lsr #0x8
    b L_0201edf4
    // exponent below range: build a denormal (or zero) result; the shifted quotient is
    // corrected and rounded (nearest, ties to even) with exact remainder checks
L_0201f09c:
    and r0, r0, #0x80000000
    cmn ip, #0x18
    beq L_0201f110
    bmi L_0201f168
    add r1, ip, #0x17
    mov r2, r2, lsl r1
    rsb ip, ip, #0x0
    mov r3, r3, lsr ip
    orr r0, r0, r3
    rsb lr, lr, #0x0
    mul r1, lr, r3
    cmp r1, r2
    ldmeqia sp!, {lr}
    bxeq lr
    add r1, r1, lr
    cmp r1, r2
    beq L_0201f104
    addmi r0, r0, #0x1
    subpl r1, r1, lr
    add r1, lr, r1, lsl #0x1
    cmp r1, r2, lsl #0x1
    and r3, r0, #0x1
    addmi r0, r0, #0x1
    addeq r0, r0, r3
    ldmia sp!, {lr}
    bx lr
    // quotient was one too low and is exact after the increment
L_0201f104:
    add r0, r0, #0x1
    ldmia sp!, {lr}
    bx lr
    // exponent -24: rounds up to the smallest denormal unless the mantissas are equal
    // (an exact half, which rounds to the even zero)
L_0201f110:
    cmn r2, lr
    addne r0, r0, #0x1
    ldmia sp!, {lr}
    bx lr
    // NaN
L_0201f120:
    mov r0, r1
    b L_0201f138
    // return signed infinity
L_0201f128:
    mov r0, #0xff000000
    orr r0, lr, r0, lsr #0x1
    ldmia sp!, {lr}
    bx lr
    // return quiet NaN
L_0201f138:
    mvn r0, #0x80000000
    ldmia sp!, {lr}
    bx lr
L_0201f144:
    mvn r0, #0x80000000
    ldmia sp!, {lr}
    bx lr
    // exponent overflow: return signed infinity
L_0201f150:
    tst r0, #0x80000000
    mov r0, #0xff000000
    movne r0, r0, asr #0x1
    moveq r0, r0, lsr #0x1
    ldmia sp!, {lr}
    bx lr
    // result underflows to zero keeping only the sign
L_0201f168:
    ldmia sp!, {lr}
    bx lr
    // return signed zero (sign in lr)
L_0201f170:
    mov r0, lr
    ldmia sp!, {lr}
    bx lr
}

// _f2d: widen a float (r0) to a double (r1:r0).
// Normal numbers: rebias the exponent (+0x380) and shift the 23-bit mantissa into the 52-bit
// field. Zero maps to signed zero; a float denormal is normalised with clz so it becomes a
// normal double; infinity becomes the double infinity; any NaN becomes 0x7fffffff_ffffffff.
// Not compiled: normalises a denormal with `clz`, which this compiler never emits: it has no clz intrinsic, and
// `__builtin_clz` and `__clz` compile as calls to an external function.
asm double _f2d(float a) {
    and r2, r0, #0x80000000
    mov ip, r0, lsr #0x17
    mov r3, r0, lsl #0x9
    ands ip, ip, #0xff
    beq L_0201f1ac
    cmp ip, #0xff
    beq L_0201f1d8
    // assemble sign | exponent | mantissa
L_0201f198:
    add ip, ip, #0x380
    mov r0, r3, lsl #0x14
    orr r1, r2, r3, lsr #0xc
    orr r1, r1, ip, lsl #0x14
    bx lr
    // exponent 0: zero or denormal
L_0201f1ac:
    cmp r3, #0x0
    bne L_0201f1c0
    mov r1, r2
    mov r0, #0x0
    bx lr
L_0201f1c0:
    mov r3, r3, lsr #0x1
    clz ip, r3
    movs r3, r3, lsl ip
    rsb ip, ip, #0x1
    add r3, r3, r3
    b L_0201f198
    // exponent 255: infinity or NaN
L_0201f1d8:
    cmp r3, #0x0
    bhi L_0201f1f0
    ldr r1, =0x7ff00000
    orr r1, r1, r2
    mov r0, #0x0
    bx lr
L_0201f1f0:
    mvn r0, #0x0
    bic r1, r0, #0x80000000
    bx lr
}

// _ffix: float (r0) to signed int, truncating toward zero.
// The shift is 0x9e minus the biased exponent; if it is not positive (|a| >= 2^31, infinity,
// NaN) the result saturates to 0x7fffffff for a non-negative input or 0x80000000 for a negative
// one. Otherwise the mantissa with its implicit bit is shifted right and negated if a < 0.
// Not compiled: `subs r2, r2, r1, lsr #0x17` is followed by `ble`, a signed test of the subtract's own flags. In this
// build's compiled code and in every form tried, a 32-bit flag-setting subtract is tested only with eq, ne, mi or pl
// (or feeds the carry of a 64-bit subtract): the compiler folds a compare with zero into the subtract for those tests
// only. Written in C, `s = 0x9e - (m >> 23); if (s <= 0)` compiles to `lsr; rsb; cmp rN, #0`, `if (s < 0)` to `rsbs`
// with mi, and `if (0x9e <= (m >> 23))` to `cmp` plus a separate `rsb`.
asm int _ffix(float a) {
    bic r1, r0, #0x80000000
    mov r2, #0x9e
    subs r2, r2, r1, lsr #0x17
    ble L_0201f228
    mov r1, r1, lsl #0x8
    orr r1, r1, #0x80000000
    cmp r0, #0x0
    mov r0, r1, lsr r2
    rsbmi r0, r0, #0x0
    bx lr
    // out of range: saturate
L_0201f228:
    mvn r0, r0, asr #0x1f
    add r0, r0, #0x80000000
    bx lr
}

// _ffixu: float (r0) to unsigned int, truncating.
// Negative inputs give 0, except NaN which gives 0xffffffff. Non-negative inputs too large for
// 32 bits (including infinity and NaN) give 0xffffffff; otherwise the mantissa with its
// implicit bit is shifted right by (0x9e - exponent).
// Not compiled: `subs r1, r1, r0, lsr #0x17` is followed by `blt`, a signed test of the subtract's own flags. In this
// build's compiled code and in every form tried, a 32-bit flag-setting subtract is tested only with eq, ne, mi or pl
// (or feeds the carry of a 64-bit subtract): the compiler folds a compare with zero into the subtract for those tests
// only. Written in C, `s = 0x9e - (a >> 23); if (s < 0)` compiles to `rsbs` with mi, `if (s <= 0)` to
// `rsb; cmp rN, #0`, and `if (0x9e < (a >> 23))` to `cmp` plus a separate `rsb`.
asm unsigned _ffixu(float a) {
    tst r0, #0x80000000
    bne L_0201f258
    mov r1, #0x9e
    subs r1, r1, r0, lsr #0x17
    blt L_0201f26c
    mov r2, r0, lsl #0x8
    orr r0, r2, #0x80000000
    mov r0, r0, lsr r1
    bx lr
    // negative input: 0, or all ones for NaN
L_0201f258:
    mov r2, #0xff000000
    cmp r2, r0, lsl #0x1
    movhs r0, #0x0
    mvnlo r0, #0x0
    bx lr
    // too large: saturate to 0xffffffff
L_0201f26c:
    mvn r0, #0x0
    bx lr
}

// _fflt: signed int (r0) to float, round to nearest even.
// Takes the magnitude (remembering the sign), returns 0 for 0, normalises with clz, sets the
// exponent to 0x9e - clz, packs sign | exponent | top 23 mantissa bits, and uses the 8 bits
// shifted out below the mantissa to round: exact or below half truncates, above half rounds up,
// exactly half rounds to even.
// Not compiled: normalises with `clz`, which this compiler never emits: it has no clz intrinsic, and `__builtin_clz`
// and `__clz` compile as calls to an external function.
asm float _fflt(int a) {
    ands r2, r0, #0x80000000
    rsbmi r0, r0, #0x0
    cmp r0, #0x0
    bxeq lr
    clz r3, r0
    movs r0, r0, lsl r3
    rsb r3, r3, #0x9e
    ands r1, r0, #0xff
    add r0, r0, r0
    orr r0, r2, r0, lsr #0x9
    orr r0, r0, r3, lsl #0x17
    bxeq lr
    tst r1, #0x80
    bxeq lr
    ands r3, r1, #0x7f
    andeqs r3, r0, #0x1
    addne r0, r0, #0x1
    bx lr
}

// _ffltu: unsigned int (r0) to float, round to nearest even.
// Same as _fflt without the sign handling; a value with bit 31 set needs no normalisation.
// Not compiled: normalises with `clz`, which this compiler never emits: it has no clz intrinsic, and `__builtin_clz`
// and `__clz` compile as calls to an external function.
asm float _ffltu(unsigned a) {
    cmp r0, #0x0
    bxeq lr
    mov r3, #0x9e
    bmi L_0201f2d8
    clz ip, r0
    movs r0, r0, lsl ip
    sub r3, r3, ip
    // pack exponent and mantissa, then round using the low 8 bits
L_0201f2d8:
    ands r2, r0, #0xff
    add r0, r0, r0
    mov r0, r0, lsr #0x9
    orr r0, r0, r3, lsl #0x17
    bxeq lr
    tst r2, #0x80
    bxeq lr
    ands r1, r2, #0x7f
    andeqs r1, r0, #0x1
    addne r0, r0, #0x1
    bx lr
}

// _fmul: single-precision multiply, r0 = a * b, round to nearest even.
// Sign is a^b. Both mantissas are left-aligned with the implicit bit at bit 31 and multiplied
// with umull; if the product's top bit is clear it is doubled and the exponent reduced. The
// exponent is ea + eb - 0x7f (less one when doubled); packing then adds the implicit bit into
// the exponent field. Too large gives a signed infinity; too small goes to a denormal
// path (shift right with a sticky bit, round to even) or to signed zero. Rounding uses the 8
// bits below the mantissa plus a sticky bit from the low product word.
// Special cases: NaN operand or 0 * inf return 0x7fffffff; inf * finite gives signed infinity;
// zero * finite gives signed zero; denormal inputs are normalised with clz.
// Not compiled: normalises denormal operands with `clz`, which this compiler never emits: it has no clz intrinsic,
// and `__builtin_clz` and `__clz` compile as calls to an external function.
asm float _fmul(float a, float b) {
    eor r2, r0, r1
    and r2, r2, #0x80000000
    mov ip, #0xff
    ands r3, ip, r0, lsr #0x17
    mov r0, r0, lsl #0x8
    cmpne r3, #0xff
    beq L_0201f380
    orr r0, r0, #0x80000000
    ands ip, ip, r1, lsr #0x17
    mov r1, r1, lsl #0x8
    cmpne ip, #0xff
    beq L_0201f3c0
    orr r1, r1, #0x80000000
    // operands normalised: a in r0, b in r1 (bit 31 set), exponents in r3 / ip
L_0201f338:
    add ip, r3, ip
    umull r1, r3, r0, r1
    movs r0, r3
    addpl r0, r0, r0
    subpl ip, ip, #0x1
    subs ip, ip, #0x7f
    bmi L_0201f44c
    cmp ip, #0xfe
    bge L_0201f4b8
    ands r3, r0, #0xff
    orr r0, r2, r0, lsr #0x8
    add r0, r0, ip, lsl #0x17
    tst r3, #0x80
    bxeq lr
    orrs r1, r1, r3, lsl #0x19
    andeqs r3, r0, #0x1
    addne r0, r0, #0x1
    bx lr
    // a has exponent 0 or 255
L_0201f380:
    cmp r3, #0x0
    beq L_0201f3d4
    movs r0, r0, lsl #0x1
    bne L_0201f4a8
    mov ip, r1, lsr #0x17
    mov r1, r1, lsl #0x9
    ands ip, ip, #0xff
    beq L_0201f3b4
    cmp ip, #0xff
    blt L_0201f49c
    cmp r1, #0x0
    beq L_0201f49c
    b L_0201f4a8
L_0201f3b4:
    cmp r1, #0x0
    beq L_0201f4b0
    b L_0201f49c
    // b has exponent 0 or 255
L_0201f3c0:
    cmp ip, #0x0
    beq L_0201f430
L_0201f3c8:
    movs r1, r1, lsl #0x1
    bne L_0201f4a8
    b L_0201f49c
    // a is zero or denormal
L_0201f3d4:
    movs r0, r0, lsl #0x1
    beq L_0201f40c
    mov r0, r0, lsr #0x1
    clz r3, r0
    movs r0, r0, lsl r3
    rsb r3, r3, #0x1
    mov ip, r1, lsr #0x17
    mov r1, r1, lsl #0x8
    ands ip, ip, #0xff
    beq L_0201f430
    cmp ip, #0xff
    beq L_0201f3c8
    orr r1, r1, #0x80000000
    b L_0201f338
    // a is zero: result is zero unless b is infinity/NaN
L_0201f40c:
    mov ip, r1, lsr #0x17
    mov r1, r1, lsl #0x9
    ands ip, ip, #0xff
    beq L_0201f4dc
    cmp ip, #0xff
    blt L_0201f4dc
    cmp r1, #0x0
    beq L_0201f4b0
    b L_0201f4a8
    // b is zero or denormal
L_0201f430:
    movs r1, r1, lsl #0x1
    beq L_0201f4dc
    mov r1, r1, lsr #0x1
    clz ip, r1
    movs r1, r1, lsl ip
    rsb ip, ip, #0x1
    b L_0201f338
    // exponent underflow: denormal / zero result
L_0201f44c:
    cmn ip, #0x18
    beq L_0201f494
    bmi L_0201f4d4
    cmp r1, #0x0
    orrne r0, r0, #0x1
    mov r3, r0
    mov r0, r0, lsr #0x8
    rsb ip, ip, #0x0
    orr r0, r2, r0, lsr ip
    rsb ip, ip, #0x18
    movs r1, r3, lsl ip
    bxeq lr
    tst r1, #0x80000000
    bxeq lr
    movs r1, r1, lsl #0x1
    andeqs r1, r0, #0x1
    addne r0, r0, #0x1
    bx lr
    // exponent -24: the top bit is the round bit; rounds up to the smallest denormal if any
    // other bit of the high product word is set (the low word is not consulted)
L_0201f494:
    mov r0, r0, lsl #0x1
    b L_0201f4c4
    // return signed infinity
L_0201f49c:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #0x1
    bx lr
    // return quiet NaN
L_0201f4a8:
    mvn r0, #0x80000000
    bx lr
L_0201f4b0:
    mvn r0, #0x80000000
    bx lr
    // overflow: signed infinity
L_0201f4b8:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #0x1
    bx lr
L_0201f4c4:
    movs r1, r0
    mov r0, r2
    addne r0, r0, #0x1
    bx lr
L_0201f4d4:
    mov r0, r2
    bx lr
    // return signed zero
L_0201f4dc:
    mov r0, r2
    bx lr
}

// func_0201f4e4: reverse-operand entry for _fsub: swaps r0 and r1 with three exclusive-ors
// and falls through into _fsub, which follows directly. Nothing in the program calls it, so
// the link keeps it with a force_active entry.
// Not compiled: has no return. After swapping the operands it runs on into _fsub, which is placed directly after it.
// A compiled function always ends in a return or a branch: `return _fsub(b, a);` moves the operands through a spare
// register and ends in a tail call (`ldr ip, =_fsub; bx ip`).
asm void func_0201f4e4(void) {
    eor r0, r0, r1
    eor r1, r0, r1
    eor r0, r0, r1
}

// _fsub: single-precision subtract, r0 = a - b.
// If the signs differ the operation is really an addition of magnitudes: b's sign is flipped
// and control goes to the add routine (the instruction at _fadd+0xc). With equal signs the
// operands are ordered so |a| >= |b| (swapping and flipping the result sign if needed), b's
// mantissa is shifted right by the exponent difference keeping a sticky bit, the mantissas are
// subtracted, and the result is renormalised with clz (exact cancellation returns +0) and
// rounded to nearest even. Denormals, infinities and NaNs are handled in the tail blocks:
// inf - inf and NaN operands give 0x7fffffff, otherwise the infinity or the other operand is
// returned.
// Not compiled: part of one hand-written object with _fadd. The `bmi` written as the `dcd` word branches into the
// middle of _fadd (_fadd+0xc), _fadd branches into this routine at _fsub+0xc, and func_0201f4e4 runs into its first
// instruction; a C function can neither branch into another function's body nor be entered anywhere but at its start.
// It also normalises with `clz`, which this compiler never emits: it has no clz intrinsic, and `__builtin_clz` and
// `__clz` compile as calls to an external function.
asm float _fsub(float a, float b) {
    eors r2, r0, r1
    eormi r1, r1, #0x80000000
    dcd 0x4afffc33                      // bmi _fadd+0xc (raw word: target inside _fadd)
    subs ip, r0, r1
    eorlo ip, ip, #0x80000000
    sublo r0, r0, ip
    addlo r1, r1, ip
    mov r2, #0x80000000
    mov r3, r0, lsr #0x17
    orr r0, r2, r0, lsl #0x8
    ands ip, r3, #0xff
    cmpne ip, #0xff
    beq L_0201f618
    mov ip, r1, lsr #0x17
    orr r1, r2, r1, lsl #0x8
    ands r2, ip, #0xff
    beq L_0201f658
    // exponents in r3 (a) / ip (b), mantissas left-aligned in r0 / r1
L_0201f534:
    subs ip, r3, ip
    beq L_0201f57c
    rsb r2, ip, #0x20
    movs r2, r1, lsl r2
    mov r1, r1, lsr ip
    orrne r1, r1, #0x1
    subs r0, r0, r1
    bpl L_0201f5c0
    ands r1, r0, #0xff
    add r0, r0, r0
    mov r0, r0, lsr #0x9
    orr r0, r0, r3, lsl #0x17
    tst r1, #0x80
    bxeq lr
    ands r1, r1, #0x7f
    andeqs r1, r0, #0x1
    addne r0, r0, #0x1
    bx lr
    // equal exponents: subtract directly
L_0201f57c:
    subs r0, r0, r1
    beq L_0201f724
    mov r2, r3, lsl #0x17
    and r2, r2, #0x80000000
    bic r3, r3, #0x100
    clz ip, r0
    movs r0, r0, lsl ip
    sub r3, r3, ip
    cmp r3, #0x0
    bgt L_0201f5b0
    rsb r3, r3, #0x9
    orr r0, r2, r0, lsr r3
    bx lr
L_0201f5b0:
    add r0, r0, r0
    orr r0, r2, r0, lsr #0x9
    orr r0, r0, r3, lsl #0x17
    bx lr
    // different exponents, top bit cleared by the subtraction: renormalise with clz; an
    // exponent <= 0 gives an unrounded denormal, otherwise pack and round to nearest even
L_0201f5c0:
    mov r2, r3, lsl #0x17
    and r2, r2, #0x80000000
    bic r3, r3, #0x100
    clz ip, r0
    movs r0, r0, lsl ip
    sub r3, r3, ip
    cmp r3, #0x0
    bgt L_0201f5ec
    rsb r3, r3, #0x9
    orr r0, r2, r0, lsr r3
    bx lr
L_0201f5ec:
    ands r1, r0, #0xff
    add r0, r0, r0
    orr r0, r2, r0, lsr #0x9
    orr r0, r0, r3, lsl #0x17
    bxeq lr
    tst r1, #0x80
    bxeq lr
    ands r1, r1, #0x7f
    andeqs r1, r0, #0x1
    addne r0, r0, #0x1
    bx lr
    // a has exponent 0 or 255
L_0201f618:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0x0
    ands r3, r3, #0xff
    beq L_0201f680
    movs r0, r0, lsl #0x1
    bne L_0201f758
    mov ip, r1, lsr #0x17
    mov r1, r1, lsl #0x9
    ands ip, ip, #0xff
    beq L_0201f74c
    cmp ip, #0xff
    blt L_0201f74c
    cmp r1, #0x0
    beq L_0201f760
    b L_0201f758
    // b has exponent 0 or 255
L_0201f658:
    cmp ip, #0x100
    movge r2, #0x80000000
    movlt r2, #0x0
    and r3, r3, #0xff
    ands ip, ip, #0xff
    beq L_0201f6e8
L_0201f670:
    eor r2, r2, #0x80000000
    movs r1, r1, lsl #0x1
    bne L_0201f758
    b L_0201f74c
    // a is zero or denormal
L_0201f680:
    movs r0, r0, lsl #0x1
    beq L_0201f6b8
    mov r0, r0, lsr #0x1
    mov r3, #0x1
    mov ip, r1, lsr #0x17
    mov r1, r1, lsl #0x8
    ands ip, ip, #0xff
    beq L_0201f6e8
    cmp ip, #0xff
    beq L_0201f670
    orr r1, r1, #0x80000000
    orr r3, r3, r2, lsr #0x17
    orr ip, ip, r2, lsr #0x17
    b L_0201f534
    // a is zero: a zero b gives +0, a finite nonzero b is returned
L_0201f6b8:
    mov r3, r1, lsr #0x17
    mov r0, r1, lsl #0x9
    ands r2, r3, #0xff
    beq L_0201f6dc
    cmp r2, #0xff
    blt L_0201f704
    cmp r0, #0x0
    bne L_0201f744
    b L_0201f74c
L_0201f6dc:
    cmp r0, #0x0
    beq L_0201f724
    b L_0201f704
    // b is zero or denormal
L_0201f6e8:
    movs r1, r1, lsl #0x1
    beq L_0201f70c
    mov r1, r1, lsr #0x1
    mov ip, #0x1
    orr ip, ip, r2, lsr #0x17
    orr r3, r3, r2, lsr #0x17
    b L_0201f534
L_0201f704:
    mov r0, r1
    bx lr
    // b is zero: result is a, repacked (exponent 0 if a was denormal)
L_0201f70c:
    cmp r0, #0x0
    subges r3, r3, #0x1
    add r0, r0, r0
    orr r0, r2, r0, lsr #0x9
    orr r0, r0, r3, lsl #0x17
    bx lr
    // return +0
L_0201f724:
    mov r0, #0x0
    bx lr
    // unreachable repack sequence (no label leads here)
    cmp r0, #0x0
    subges r3, r3, #0x1
    add r0, r0, r0
    mov r0, r0, lsr #0x9
    orr r0, r0, r3, lsl #0x17
    bx lr
    // return quiet NaN
L_0201f744:
    mvn r0, #0x80000000
    bx lr
    // return signed infinity
L_0201f74c:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #0x1
    bx lr
    // return quiet NaN
L_0201f758:
    mvn r0, #0x80000000
    bx lr
    // return quiet NaN
L_0201f760:
    mvn r0, #0x80000000
    bx lr
}

// func_0201f768: signed 64-bit remainder, a % b (a in r1:r0, b in r3:r2); the remainder takes
// the sign of a. It sets r4 = (high word of a) | 1 -- bit 0 selects "return remainder" -- and
// branches into the shared code in _ll_sdiv (0x0201f788). _ll_sdiv, directly after it, is the
// entry for the signed 64-bit quotient: it sets r4 = (ahi ^ bhi) with bit 0 cleared (sign =
// quotient sign) and runs the shared code.
// Division by zero returns immediately with the registers unchanged. If both operands fit in 32
// bits (high words equal to the sign of the low word) the 32-bit divider _s32_div_f does the work and the
// result is sign-extended. Otherwise: both operands are made positive, the divisor is shifted
// left until its top bit is set (r5 counts the shifts), the dividend is shifted similarly to
// set the loop count (r6), and a bit-at-a-time non-restoring long division runs with the
// quotient in r11:ip (high:low) and the partial remainder in r7:r1:r0. The remainder case shifts
// the partial remainder back right by r5; finally the result is negated if bit 31 of r4 is set.
// L_0201f7ec in _ll_sdiv is also the target of a branch in _ull_mod (code shared with _ll_udiv).
// Not compiled: an entry of the hand-written 64-bit divider. It pushes registers, puts a mode flag in r4 and branches
// into the middle of _ll_sdiv (_ll_sdiv+0x10, the `dcd` word), whose code then finishes and returns for it; a C
// function can't branch into another function's body, and no C calling convention passes r4.
asm long long func_0201f768(long long a, long long b) {
    stmdb sp!, {r4, r5, r6, r7, r11, ip, lr}
    mov r4, r1
    orr r4, r4, #0x1
    dcd 0xea000003                      // b _ll_sdiv+0x10, the shared code (raw word)
}

// _ll_sdiv: signed 64-bit quotient, a / b; also the shared body of func_0201f768 (see above).
// Not compiled: entered in the middle by two other routines, which push the registers and set the mode flag in r4
// themselves: func_0201f768 branches to _ll_sdiv+0x10 and _ull_mod to _ll_sdiv+0x74. A compiled function can be
// entered only at its start, and no C calling convention passes r4.
asm long long _ll_sdiv(long long a, long long b) {
    stmdb sp!, {r4, r5, r6, r7, r11, ip, lr}
    eor r4, r1, r3
    mov r4, r4, asr #0x1
    mov r4, r4, lsl #0x1
    // common code: zero divisor returns
L_0201f788:
    orrs r5, r3, r2
    bne L_0201f798
    ldmia sp!, {r4, r5, r6, r7, r11, ip, lr}
    bx lr
    // both operands fit in 32 bits? use the 32-bit divider
L_0201f798:
    mov r5, r0, lsr #0x1f
    add r5, r5, r1
    mov r6, r2, lsr #0x1f
    add r6, r6, r3
    orrs r6, r5, r6
    bne L_0201f7cc
    mov r1, r2
    bl _s32_div_f
    ands r4, r4, #0x1
    movne r0, r1
    mov r1, r0, asr #0x1f
    ldmia sp!, {r4, r5, r6, r7, r11, ip, lr}
    bx lr
    // take absolute values of a and b
L_0201f7cc:
    cmp r1, #0x0
    bge L_0201f7dc
    rsbs r0, r0, #0x0
    rsc r1, r1, #0x0
L_0201f7dc:
    cmp r3, #0x0
    bge L_0201f7ec
    rsbs r2, r2, #0x0
    rsc r3, r3, #0x0
    // dividend zero -> result zero; else align divisor and count steps
L_0201f7ec:
    orrs r5, r1, r0
    beq L_0201f910
    mov r5, #0x0
    mov r6, #0x1
    cmp r3, #0x0
    bmi L_0201f818
L_0201f804:
    add r5, r5, #0x1
    adds r2, r2, r2
    adcs r3, r3, r3
    bpl L_0201f804
    add r6, r6, r5
L_0201f818:
    cmp r1, #0x0
    blt L_0201f838
L_0201f820:
    cmp r6, #0x1
    beq L_0201f838
    sub r6, r6, #0x1
    adds r0, r0, r0
    adcs r1, r1, r1
    bpl L_0201f820
    // long division loop (subtract / add back, one quotient bit per pass)
L_0201f838:
    mov r7, #0x0
    mov ip, #0x0
    mov r11, #0x0
    b L_0201f860
L_0201f848:
    orr ip, ip, #0x1
    subs r6, r6, #0x1
    beq L_0201f8b8
    adds r0, r0, r0
    adcs r1, r1, r1
    adcs r7, r7, r7
L_0201f860:
    subs r0, r0, r2
    sbcs r1, r1, r3
    sbcs r7, r7, #0x0
    adds ip, ip, ip
    adc r11, r11, r11
    cmp r7, #0x0
    bge L_0201f848
L_0201f87c:
    subs r6, r6, #0x1
    beq L_0201f8b0
    adds r0, r0, r0
    adcs r1, r1, r1
    adc r7, r7, r7
    adds r0, r0, r2
    adcs r1, r1, r3
    adc r7, r7, #0x0
    adds ip, ip, ip
    adc r11, r11, r11
    cmp r7, #0x0
    bge L_0201f848
    b L_0201f87c
L_0201f8b0:
    adds r0, r0, r2
    adc r1, r1, r3
    // select quotient or remainder and undo the alignment shift
L_0201f8b8:
    ands r7, r4, #0x1
    moveq r0, ip
    moveq r1, r11
    beq L_0201f8f0
    subs r7, r5, #0x20
    movge r0, r1, lsr r7
    bge L_0201f914
    rsb r7, r5, #0x20
    mov r0, r0, lsr r5
    orr r0, r0, r1, lsl r7
    mov r1, r1, lsr r5
    b L_0201f8f0
    // unreachable (no label leads here)
    mov r0, r1, lsr r7
    mov r1, #0x0
    // apply the result sign
L_0201f8f0:
    cmp r4, #0x0
    blt L_0201f900
    ldmia sp!, {r4, r5, r6, r7, r11, ip, lr}
    bx lr
L_0201f900:
    rsbs r0, r0, #0x0
    rsc r1, r1, #0x0
    ldmia sp!, {r4, r5, r6, r7, r11, ip, lr}
    bx lr
    // zero dividend: result 0
L_0201f910:
    mov r0, #0x0
L_0201f914:
    mov r1, #0x0
    cmp r4, #0x0
    blt L_0201f900
    ldmia sp!, {r4, r5, r6, r7, r11, ip, lr}
    bx lr
}

// _ll_udiv: unsigned 64-bit divide, quotient of a / b (a in r1:r0, b in r3:r2). It sets r4 = 0
// and branches into the shared code in _ull_mod, which follows; _ull_mod's own entry sets r4 = 1
// to return the remainder instead. Zero divisor returns a unchanged. If both high words are zero
// it is a 32-bit divide: the compare/divide part of _u32_div_f (_u32_div_f+0x8) is called, r0
// takes the quotient (or the remainder) and r1 is cleared. Otherwise it jumps into the 64-bit
// divide in _ll_sdiv at L_0201f7ec (0x0201f7ec), past the absolute-value step.
// Not compiled: an entry of the hand-written 64-bit divider. It pushes registers, puts a mode flag in r4 and branches
// into the middle of _ull_mod (_ull_mod+0x8, the `dcd` word), whose code then finishes and returns for it; a C
// function can't branch into another function's body, and no C calling convention passes r4.
asm unsigned long long _ll_udiv(unsigned long long a, unsigned long long b) {
    stmdb sp!, {r4, r5, r6, r7, r11, ip, lr}
    mov r4, #0x0
    dcd 0xea000001                      // b _ull_mod+0x8, the shared code (raw word)
}

// _ull_mod: unsigned 64-bit remainder, a % b; also the shared body of _ll_udiv (see above).
// Not compiled: part of the hand-written 64-bit divider. _ll_udiv enters it at _ull_mod+0x8 with r4 already set; it
// branches into the middle of _ll_sdiv (_ll_sdiv+0x74) and calls _u32_div_f past its zero test (_u32_div_f+0x8), both
// written as `dcd` words, and takes the remainder from r1 after that call. A compiled function can't branch or call
// into another function's body or be entered anywhere but at its start.
asm unsigned long long _ull_mod(unsigned long long a, unsigned long long b) {
    stmdb sp!, {r4, r5, r6, r7, r11, ip, lr}
    mov r4, #0x1
    // common code
L_0201f93c:
    orrs r5, r3, r2
    bne L_0201f94c
    ldmia sp!, {r4, r5, r6, r7, r11, ip, lr}
    bx lr
L_0201f94c:
    orrs r5, r1, r3
    dcd 0x1affffa5                      // bne L_0201f7ec in _ll_sdiv, the 64-bit divide (raw word)
    mov r1, r2
    dcd 0xeb000089                      // bl _u32_div_f+0x8, past its zero test (raw word)
    cmp r4, #0x0
    movne r0, r1
    mov r1, #0x0
    ldmia sp!, {r4, r5, r6, r7, r11, ip, lr}
    bx lr
}

// _s32_div_f: signed 32-bit divide, a / b, quotient in r0 and remainder in r1.
// The sign flags go in ip (bit 31: quotient negative; bit 0: dividend negative, so the
// remainder follows the dividend). Both operands are made positive. Divisor zero, or dividend
// smaller than divisor, skips the loop (divisor zero leaves r0 = a and r1 = 0 once the signs
// are applied; a smaller dividend gives r0 = 0 and r1 = dividend). Otherwise
// the number of significant quotient bits is found by comparing against shifted copies of the
// dividend in steps of 16, 8 and 4 bits; the dividend is shifted up by that count, and a
// computed jump (add pc, pc, r2*12) lands in an unrolled sequence of 32 three-instruction
// steps, each of which shifts the partial remainder in r3, subtracts the divisor when it fits,
// and shifts the new quotient bit into r0. The signs are then applied.
// Not compiled: `add pc, pc, r2, lsl #0x2` (r2 = 3 * shift count) jumps straight into an unrolled run of 32
// three-instruction steps. Every computed write of pc in this build's compiled code is a switch's conditional
// `add pc, pc, rN, lsl #2` (ls, pl or ge) into a table of branches. The steps pass each quotient bit through the
// carry flag (`adcs`, `sublo`, `adcs`): written as `q = q + q + (r >= d)` or `q = (q << 1) | (r >= d)`, the compiler
// gives `cmp; movcs rN, #1; movcc rN, #0` and an add/orr, never adc. The remainder is also returned in r1 beside the
// int result, and _ll_sdiv reads it there.
asm int _s32_div_f(int a, int b) {
    eor ip, r0, r1
    and ip, ip, #0x80000000
    cmp r0, #0x0
    rsblt r0, r0, #0x0
    addlt ip, ip, #0x1
    cmp r1, #0x0
    rsblt r1, r1, #0x0
    beq L_0201fb68
    cmp r0, r1
    movlo r1, r0
    movlo r0, #0x0
    blo L_0201fb68
    mov r2, #0x1c
    mov r3, r0, lsr #0x4
    cmp r1, r3, lsr #0xc
    suble r2, r2, #0x10
    movle r3, r3, lsr #0x10
    cmp r1, r3, lsr #0x4
    suble r2, r2, #0x8
    movle r3, r3, lsr #0x8
    cmp r1, r3
    suble r2, r2, #0x4
    movle r3, r3, lsr #0x4
    mov r0, r0, lsl r2
    rsb r1, r1, #0x0
    adds r0, r0, r0
    add r2, r2, r2, lsl #0x1
    add pc, pc, r2, lsl #0x2
    mov r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    mov r1, r3
    // apply the signs to quotient and remainder
L_0201fb68:
    ands r3, ip, #0x80000000
    rsbne r0, r0, #0x0
    ands r3, ip, #0x1
    rsbne r1, r1, #0x0
    bx lr
}

// _u32_div_f: unsigned 32-bit divide, quotient in r0 and remainder in r1. Divisor zero returns
// immediately. The part after the zero test (0x0201fb84, also called from the code _ll_udiv and
// _ull_mod share) is the same
// shift-count search and unrolled 32-step computed-jump divide as in _s32_div_f, without sign
// handling. If a < b the quotient is 0 and the remainder is a.
// Not compiled: _ull_mod calls into it at _u32_div_f+0x8, past the zero test, and a compiled function can be entered
// only at its start. Like _s32_div_f it also jumps with `add pc, pc, r2, lsl #0x2` straight into an unrolled run of
// steps that pass each quotient bit through the carry flag (`adcs`), and returns the remainder in r1 beside the
// result.
asm unsigned _u32_div_f(unsigned a, unsigned b) {
    cmp r1, #0x0
    bxeq lr
    // entry that skips the zero-divisor test
L_0201fb84:
    cmp r0, r1
    movlo r1, r0
    movlo r0, #0x0
    bxlo lr
    mov r2, #0x1c
    mov r3, r0, lsr #0x4
    cmp r1, r3, lsr #0xc
    suble r2, r2, #0x10
    movle r3, r3, lsr #0x10
    cmp r1, r3, lsr #0x4
    suble r2, r2, #0x8
    movle r3, r3, lsr #0x8
    cmp r1, r3
    suble r2, r2, #0x4
    movle r3, r3, lsr #0x4
    mov r0, r0, lsl r2
    rsb r1, r1, #0x0
    adds r0, r0, r0
    add r2, r2, r2, lsl #0x1
    add pc, pc, r2, lsl #0x2
    mov r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    adcs r3, r1, r3, lsl #0x1
    sublo r3, r3, r1
    adcs r0, r0, r0
    mov r1, r3
    bx lr
}

// func_0201fd60: reverse-operand entry for _ddiv: swaps the two double operands (r1:r0 with
// r3:r2) with three exclusive-ors per word and falls through into _ddiv, which follows
// directly. Nothing in the program calls it, so the link keeps it with a force_active entry.
// Not compiled: has no return. After swapping the operands it runs on into _ddiv, which is placed directly after it.
// A compiled function always ends in a return or a branch: `return _ddiv(b, a);` moves the operands through spare
// registers and calls _ddiv.
asm void func_0201fd60(void) {
    eor r1, r1, r3
    eor r3, r1, r3
    eor r1, r1, r3
    eor r0, r0, r2
    eor r2, r0, r2
    eor r0, r0, r2
}

// _ddiv: double-precision divide, a / b (a in r1:r0, b in r3:r2), round to nearest even.
// Sign is a^b (computed in r4, then carried in bit 0 of ip). Exponent fields are extracted with
// the mask 0xffe (a literal-pool constant), so each exponent is held doubled in the working
// registers; mantissas get the implicit bit. The quotient is built from a reciprocal of b: an
// 8-bit table estimate (256-byte table in the middle of the code, read pc-relative) is refined
// with several multiplies, producing a quotient estimate in r4:r5 plus a guard word in lr. If
// the guard word's top bit is set the result is rounded up directly; otherwise the low bits of
// b*(2q+1) are compared with a to decide the round bit. Overflow gives a signed infinity;
// underflow builds a denormal, corrected and rounded (nearest, ties to even) with exact
// remainder checks, or a signed zero.
// Special cases: a signaling NaN in a is returned quieted; otherwise a quiet NaN in b is
// returned, else a quiet NaN in a. The other NaN cases (signaling NaN in b, 0/0, inf/inf) or
// 0x7ff80000 into the working high word with r0 unchanged, which for 0/0 and inf/inf gives
// 0x7ff80000_00000000 with a's sign. x/0 and inf/finite give a signed infinity; 0/finite and
// finite/inf give signed zero; denormal operands are normalised with clz first.
// Not compiled: keeps its 256-byte reciprocal table inside its own code and addresses it with `sub r4, pc, #0x24` (pc
// read as data), and normalises denormal operands with `clz`, which this compiler never emits: it has no clz
// intrinsic, and `__builtin_clz` and `__clz` compile as calls to an external function. func_0201fd60 runs into its
// first instruction.
asm double _ddiv(double a, double b) {
    stmdb sp!, {r4, r5, r6, lr}
    ldr lr, =0xffe
    eor r4, r1, r3
    ands ip, lr, r1, lsr #0x13
    cmpne ip, lr
    beq L_02020124
    bic r1, r1, lr, lsl #0x14
    orr r1, r1, #0x100000
    add ip, ip, r4, lsr #0x1f
    // a is normalised; now unpack b
L_0201fd9c:
    ands r4, lr, r3, lsr #0x13
    cmpne r4, lr
    beq L_020201bc
    bic r3, r3, lr, lsl #0x14
    orr r3, r3, #0x100000
    // both normalised: ip = exponent difference, a/b mantissas in r1:r0 / r3:r2
L_0201fdb0:
    sub ip, ip, r4
    cmp r1, r3
    cmpeq r0, r2
    bhs L_0201fdcc
    adds r0, r0, r0
    adc r1, r1, r1
    sub ip, ip, #0x2
    // compute reciprocal estimate and refine it
L_0201fdcc:
    sub r4, pc, #0x24
    ldrb lr, [r4, r3, lsr #0xc]
    rsbs r2, r2, #0x0
    rsc r3, r3, #0x0
    mov r4, #0x20000000
    mla r5, lr, r3, r4
    mov r6, r3, lsl #0xa
    mov r5, r5, lsr #0x7
    mul lr, r5, lr
    orr r6, r6, r2, lsr #0x16
    mov lr, lr, lsr #0xd
    mul r5, lr, r6
    mov r6, r1, lsl #0xa
    orr r6, r6, r0, lsr #0x16
    mov r5, r5, lsr #0x10
    mul r5, lr, r5
    mov lr, lr, lsl #0xe
    add lr, lr, r5, lsr #0x10
    umull r5, r6, lr, r6
    umull r4, r5, r6, r2
    mla r5, r3, r6, r5
    mov r4, r4, lsr #0x1a
    orr r4, r4, r5, lsl #0x6
    add r4, r4, r0, lsl #0x2
    umull lr, r5, r4, lr
    mov r4, #0x0
    adds r5, r5, r6, lsl #0x18
    adc r4, r4, r6, lsr #0x8
    cmp ip, #0x800
    bge L_0201ffb0
    add ip, ip, #0x7f0
    adds ip, ip, #0xc
    bmi L_0201ffc8
    orr r1, r4, ip, lsl #0x1f
    bic ip, ip, #0x1
    add r1, r1, ip, lsl #0x13
    tst lr, #0x80000000
    bne L_0201fea0
    rsbs r2, r2, #0x0
    mov r4, r4, lsl #0x1
    add r4, r4, r5, lsr #0x1f
    mul lr, r2, r4
    mov r6, #0x0
    mov r4, r5, lsl #0x1
    orr r4, r4, #0x1
    umlal r6, lr, r4, r2
    rsc r3, r3, #0x0
    mla lr, r4, r3, lr
    cmp lr, r0, lsl #0x15
    bmi L_0201fea0
    mov r0, r5
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
    // round up: add 1 to the 64-bit result
L_0201fea0:
    adds r0, r5, #0x1
    adc r1, r1, #0x0
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
    // reciprocal-estimate table (256 bytes)
L_0201feb0:
    dcd 0xfdfeffff
    dcd 0xf9fafbfc
    dcd 0xf5f6f7f8
    dcd 0xf1f2f3f4
    dcd 0xeeeff0f0
    dcd 0xeaebeced
    dcd 0xe7e8e9ea
    dcd 0xe4e5e6e6
    dcd 0xe1e2e2e3
    dcd 0xdedfdfe0
    dcd 0xdbdcdcdd
    dcd 0xd8d9d9da
    dcd 0xd5d6d7d7
    dcd 0xd2d3d4d4
    dcd 0xd0d0d1d2
    dcd 0xcdcececf
    dcd 0xcbcbcccc
    dcd 0xc8c9c9ca
    dcd 0xc6c6c7c8
    dcd 0xc3c4c5c5
    dcd 0xc1c2c2c3
    dcd 0xbfbfc0c0
    dcd 0xbdbdbebe
    dcd 0xbabbbcbc
    dcd 0xb8b9b9ba
    dcd 0xb6b7b7b8
    dcd 0xb4b5b5b6
    dcd 0xb2b3b3b4
    dcd 0xb0b1b1b2
    dcd 0xafafafb0
    dcd 0xadadaeae
    dcd 0xababacac
    dcd 0xa9aaaaaa
    dcd 0xa7a8a8a9
    dcd 0xa6a6a7a7
    dcd 0xa4a4a5a5
    dcd 0xa2a3a3a4
    dcd 0xa1a1a2a2
    dcd 0x9fa0a0a0
    dcd 0x9e9e9e9f
    dcd 0x9c9d9d9d
    dcd 0x9b9b9b9c
    dcd 0x999a9a9a
    dcd 0x98989999
    dcd 0x96979798
    dcd 0x95959696
    dcd 0x94949495
    dcd 0x92939393
    dcd 0x91919292
    dcd 0x90909191
    dcd 0x8f8f8f90
    dcd 0x8d8e8e8e
    dcd 0x8c8c8d8d
    dcd 0x8b8b8c8c
    dcd 0x8a8a8a8b
    dcd 0x8989898a
    dcd 0x88888888
    dcd 0x86878787
    dcd 0x85868686
    dcd 0x84858585
    dcd 0x83838484
    dcd 0x82828383
    dcd 0x81818282
    dcd 0x80808181
    // exponent too large: signed infinity
L_0201ffb0:
    movs r1, ip, lsl #0x1f
    orr r1, r1, #0x7f000000
    orr r1, r1, #0xf00000
    mov r0, #0x0
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
    // exponent underflow: build a denormal
L_0201ffc8:
    mvn r6, ip, asr #0x1
    cmp r6, #0x34
    bgt L_02020114
    beq L_020200f0
    cmp r6, #0x14
    bge L_02020010
    rsb r6, r6, #0x13
    mov lr, r0, lsl r6
    rsb r6, r6, #0x14
    mov r0, r5, lsr r6
    rsb r6, r6, #0x20
    orr r0, r0, r4, lsl r6
    rsb r6, r6, #0x20
    mov r4, r4, lsr r6
    orr r1, r4, ip, lsl #0x1f
    mov ip, lr
    mov lr, #0x0
    b L_02020040
L_02020010:
    rsb r6, r6, #0x33
    mov lr, r1, lsl r6
    mov r1, ip, lsl #0x1f
    rsb r6, r6, #0x20
    orr ip, lr, r0, lsr r6
    rsb r6, r6, #0x20
    mov lr, r0, lsl r6
    mov r5, r5, lsr #0x15
    orr r5, r5, r4, lsl #0xb
    rsb r6, r6, #0x1f
    mov r0, r5, lsr r6
    mov r4, #0x0
    // check the remainder of the denormal quotient
L_02020040:
    rsbs r2, r2, #0x0
    mul r4, r2, r4
    mov r5, #0x0
    umlal r5, r4, r2, r0
    rsc r3, r3, #0x0
    mla r4, r0, r3, r4
    cmp r4, ip
    cmpeq r5, lr
    ldmeqia sp!, {r4, r5, r6, lr}
    bxeq lr
    adds r5, r5, r2
    adc r4, r4, r3
    cmp r4, ip
    bmi L_020200e4
    bne L_02020088
    cmp r5, lr
    beq L_020200d4
    blo L_020200e4
L_02020088:
    subs r5, r5, r2
    sbc r4, r4, r3
    // tie check: compare doubled remainder with the divisor
L_02020090:
    adds r5, r5, r5
    adc r4, r4, r4
    adds r5, r5, r2
    adc r4, r4, r3
    adds lr, lr, lr
    adc ip, ip, ip
    cmp r4, ip
    bmi L_020200d4
    ldmneia sp!, {r4, r5, r6, lr}
    bxne lr
    cmp r5, lr
    blo L_020200d4
    ldmneia sp!, {r4, r5, r6, lr}
    bxne lr
    tst r0, #0x1
    ldmeqia sp!, {r4, r5, r6, lr}
    bxeq lr
    // round up
L_020200d4:
    adds r0, r0, #0x1
    adc r1, r1, #0x0
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
L_020200e4:
    adds r0, r0, #0x1
    adc r1, r1, #0x0
    b L_02020090
    // shift is exactly 52 bits: result is 0 or the smallest denormal
L_020200f0:
    rsbs r2, r2, #0x0
    rsc r3, r3, #0x0
    cmp r1, r3
    cmpeq r0, r2
    mov r1, ip, lsl #0x1f
    mov r0, #0x0
    movne r0, #0x1
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
    // result underflows to signed zero
L_02020114:
    mov r1, ip, lsl #0x1f
    mov r0, #0x0
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
    // a has exponent 0 or 0x7ff
L_02020124:
    orrs r5, r0, r1, lsl #0x1
    beq L_02020248
    cmp ip, lr
    beq L_0202018c
    movs r1, r1, lsl #0xc
    beq L_02020168
    clz r5, r1
    movs r1, r1, lsl r5
    sub ip, ip, r5
    add r5, ip, #0x1f
    mov r1, r1, lsr #0xb
    orr r1, r1, r0, lsr r5
    rsb r5, r5, #0x20
    mov r0, r0, lsl r5
    mov ip, ip, lsl #0x1
    orr ip, ip, r4, lsr #0x1f
    b L_0201fd9c
L_02020168:
    mvn ip, #0x13
    clz r5, r0
    movs r0, r0, lsl r5
    sub ip, ip, r5
    mov r1, r0, lsr #0xb
    mov r0, r0, lsl #0x15
    mov ip, ip, lsl #0x1
    orr ip, ip, r4, lsr #0x1f
    b L_0201fd9c
    // a is infinity or NaN
L_0202018c:
    orrs r5, r0, r1, lsl #0xc
    bne L_02020270
    bic r5, r3, #0x80000000
    cmp r5, lr, lsl #0x13
    bhs L_020201b0
    and r5, r3, #0x80000000
    eor r1, r5, r1
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
L_020201b0:
    orrs r5, r2, r3, lsl #0xc
    bne L_02020290
    b L_020202a8
    // b has exponent 0 or 0x7ff
L_020201bc:
    orrs r5, r2, r3, lsl #0x1
    beq L_02020234
    cmp r4, lr
    beq L_0202021c
    movs r3, r3, lsl #0xc
    beq L_020201fc
    clz r5, r3
    movs r3, r3, lsl r5
    sub r4, r4, r5
    add r5, r4, #0x1f
    mov r3, r3, lsr #0xb
    orr r3, r3, r2, lsr r5
    rsb r5, r5, #0x20
    mov r2, r2, lsl r5
    mov r4, r4, lsl #0x1
    b L_0201fdb0
L_020201fc:
    mvn r4, #0x13
    clz r5, r2
    movs r2, r2, lsl r5
    sub r4, r4, r5
    mov r3, r2, lsr #0xb
    mov r2, r2, lsl #0x15
    mov r4, r4, lsl #0x1
    b L_0201fdb0
    // b is infinity or NaN
L_0202021c:
    orrs r5, r2, r3, lsl #0xc
    bne L_02020290
    mov r1, ip, lsl #0x1f
    mov r0, #0x0
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
    // b is zero: signed infinity
L_02020234:
    mov r1, ip, lsl #0x1f
    orr r1, r1, lr, lsl #0x13
    mov r0, #0x0
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
    // a is zero
L_02020248:
    orrs r5, r2, r3, lsl #0x1
    beq L_020202a8
    bic r5, r3, #0x80000000
    cmp r5, lr, lsl #0x13
    cmpeq r2, #0x0
    bhi L_02020290
    eor r1, r1, r3
    and r1, r1, #0x80000000
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
    // a is NaN: a signaling one is quieted; a quiet one is returned unless b is a NaN
L_02020270:
    tst r1, #0x80000
    beq L_020202a8
    bic r5, r3, #0x80000000
    cmp r5, lr, lsl #0x13
    cmpeq r2, #0x0
    bhi L_02020290
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
    // b is NaN: return it if quiet, else take the exit below
L_02020290:
    tst r3, #0x80000
    beq L_020202a8
    mov r1, r3
    mov r0, r2
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
    // or 0x7ff80000 into the high word (quiet NaN), r0 unchanged
L_020202a8:
    orr r1, r1, #0x7f000000
    orr r1, r1, #0xf80000
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
}

