// Atomic word swap (0x02007cd0), hand-written assembly.

// MI_SwapWord(value, addr): atomically exchanges the word at addr with value
// and returns the old word.
asm unsigned int MI_SwapWord(unsigned int value, volatile unsigned int *addr) {
    swp r0, r0, [r1]
    bx lr
}
