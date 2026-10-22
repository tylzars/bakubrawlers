// Count leading zeros (0x0200745c), hand-written assembly.

// OS_CountLeadingZeros(x): returns the number of leading zero bits in x (32 for 0).
// Not compiled: clz, which this compiler never emits: it has no clz intrinsic, and `__builtin_clz` and `__clz`
// compile as calls to an external function.
asm unsigned int OS_CountLeadingZeros(unsigned int x) {
    clz r0, r0
    bx lr
}
