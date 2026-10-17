#include "types.h"
typedef struct { u32 compParam : 4; u32 compType : 4; u32 destSize : 24; } MICompressionHeader;
typedef struct {
    u8 *destp; s32 destCount; s32 length; u16 unk0c; u8 unk0e; u8 flags; u8 flagCount; u8 lengthState; u8 exFormat;
} MIUncompContextLZ;
void MI_InitUncompContextLZ(MIUncompContextLZ *context, u8 *dest, const MICompressionHeader *header) {
    context->destp = dest;
    context->destCount = header->destSize;
    context->flags = 0;
    context->flagCount = 0;
    context->length = 0;
    context->lengthState = 3;
    context->unk0c = 0;
    context->unk0e = 0;
    context->exFormat = header->compParam != 0;
}

// Resumable decoder. r0 -> state block: +0 output pointer, +4 bytes still to produce, +8 pending
// back-reference word, +0xf current flag byte, +0x10 flag bits left, +0x11 phase of the
// back-reference header parse, +0x12 header format (1 = extended form). r1 = input pointer,
// r2 = input bytes available. Each flag byte is consumed top bit first: a clear bit copies one
// literal byte; a set bit starts a back-reference whose header is 2..4 bytes: one length byte in
// the basic format (count = high nibble + 3), or one to three length bytes in the extended format
// (chosen by the first byte's top bits), then a distance low byte. The low nibble of the last
// length byte supplies the distance's top four bits (distance = 12-bit value + 1), and the
// referenced bytes are copied one at a time.
// All output bytes are written with an atomic byte swap. Runs until the output count reaches zero
// or the input runs out; the state is stored back and the remaining output count is returned in r0.
asm int MI_UncompLZChunk(void *state, const unsigned char *in, unsigned avail) {
    stmfd sp!, {r4, r5, r6, r7, r8, r9, r10, r11}
    ldr r3, [r0, #0]                    // output pointer
    ldr r4, [r0, #4]                    // output bytes left
    ldrb r5, [r0, #0xf]                 // flag byte
    ldrb r6, [r0, #0x10]                // flag bits left
    ldr r7, [r0, #8]                    // pending reference word
    ldrb r8, [r0, #0x11]                // header phase
    ldrb r11, [r0, #0x12]               // header format
L_top:
    cmp r4, #0
    ble L_save                          // nothing left to produce
    cmp r6, #0
    beq L_newflag
L_bit:
    cmp r2, #0
    beq L_save                          // out of input
    tst r5, #0x80
    bne L_ref
    ldrb r9, [r1], #1                   // literal byte
    sub r4, r4, #1
    sub r2, r2, #1
    swpb r9, r9, [r3]
    add r3, r3, #1
    b L_next
L_ref:
    cmp r8, #0
    beq L_copyhdr
    cmp r11, #1
    bne L_fmt0
    subs r8, r8, #1                     // extended format: step the header phase
    beq L_ph1
    cmp r8, #1
    beq L_ph2
    ldrb r7, [r1], #1                   // first header byte
    tst r7, #0xe0
    beq L_long
    add r7, r7, #0x10                   // short form (top 3 bits non-zero): count = high nibble + 1
    mov r8, #0
    b L_hdrdone
L_long:
    mov r10, #0x110
    tst r7, #0x10
    beq L_mid
    and r7, r7, #0xf
    add r10, r10, #0x1000
    add r7, r10, r7, lsl #16            // longest form: two more bytes follow
    b L_more
L_mid:
    and r7, r7, #0xf
    add r7, r10, r7, lsl #8             // middle form: one more byte follows
    mov r8, #1
    b L_more
L_ph2:
    ldrb r10, [r1], #1                  // next byte goes into bits 8..15
    add r7, r7, r10, lsl #8
    b L_more
L_ph1:
    ldrb r10, [r1], #1                  // last byte goes into bits 0..7
    add r7, r7, r10
    b L_hdrdone
L_more:
    subs r2, r2, #1
    beq L_save
    b L_ref
L_fmt0:
    ldrb r7, [r1], #1                   // basic format: one byte, length word = byte + 0x30
    add r7, r7, #0x30
    mov r8, #0
L_hdrdone:
    subs r2, r2, #1
    beq L_save
L_copyhdr:
    and r9, r7, #0xf                    // distance high bits from the length word
    mov r10, r9, lsl #8
    ldrb r9, [r1], #1                   // distance low byte
    mov r8, #3
    sub r2, r2, #1
    orr r9, r9, r10
    add r9, r9, #1                      // distance
    movs r7, r7, asr #4                 // copy count (not limited by bytes left)
    beq L_next
L_copy:
    ldrb r10, [r3, -r9]                 // byte from earlier output
    sub r4, r4, #1
    swpb r10, r10, [r3]
    add r3, r3, #1
    subs r7, r7, #1
    bgt L_copy
L_next:
    cmp r4, #0
    beq L_save
    mov r5, r5, lsl #1                  // advance to the next flag bit
    subs r6, r6, #1
    bne L_bit
L_newflag:
    cmp r2, #0
    beq L_save
    ldrb r5, [r1], #1                   // fetch a new flag byte
    mov r6, #8
    sub r2, r2, #1
    b L_top
L_save:
    str r3, [r0, #0]
    str r4, [r0, #4]
    strb r5, [r0, #0xf]
    strb r6, [r0, #0x10]
    str r7, [r0, #8]
    strb r8, [r0, #0x11]
    strb r11, [r0, #0x12]
    mov r0, r4
    ldmfd sp!, {r4, r5, r6, r7, r8, r9, r10, r11}
    bx lr
}
