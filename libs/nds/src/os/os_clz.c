// Count leading zeros (0x0200745c), hand-written assembly.

// OS_CountLeadingZeros(x): returns the number of leading zero bits in x (32 for 0).
asm unsigned int OS_CountLeadingZeros(unsigned int x) {
    clz r0, r0
    bx lr
}
