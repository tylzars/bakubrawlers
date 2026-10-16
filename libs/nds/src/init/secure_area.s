; The first 0x800 bytes of the ARM9 binary (0x02000000-0x02000800), the cartridge's secure area.
; Apart from the BIOS call thunks, its contents are filler bytes that are not code. They are not
; written out here: each run is taken from the ARM9 binary extracted from the user's own ROM at
; build time (.incbin, offset and length from 0x02000000), so the thunks between them stay at their
; addresses, which are only halfword aligned.
; Each thunk issues one BIOS software interrupt with the caller's arguments still in r0-r3 and returns
; the BIOS result in r0.

	.text
	.thumb
	.global SoftReset
	.global CpuSet
	.global Mod
	.global CpuFastSet
	.global IsDebugger
	.global VBlankIntrWait
	.global Sqrt
	.global IntrWait
	.global LZ77UnCompReadNormalWrite8bit
	.global RLUnCompReadNormalWrite8bit
	.global Halt
	.global WaitByLoop
	.global Div
	.global BitUnPack
	.global LZ77UnCompReadByCallbackWrite16bit
	.global RLUnCompReadByCallbackWrite16bit
	.global GetCRC16
	.global HuffUnCompReadByCallback

; filler 0x02000000-0x02000086
	.incbin "extract/usa/arm9/arm9.bin", 0x0, 0x86

	.type SoftReset, @function
SoftReset:
	swi 0         ; BIOS call 0
	bx lr         ; return
	.size SoftReset, 4

; filler 0x0200008a-0x020000ea
	.incbin "extract/usa/arm9/arm9.bin", 0x8a, 0x60

	.type CpuSet, @function
CpuSet:
	swi 11        ; BIOS call 11
	bx lr         ; return
	.size CpuSet, 4

; filler 0x020000ee-0x02000156
	.incbin "extract/usa/arm9/arm9.bin", 0xee, 0x68

; Mod: the BIOS division call leaves the remainder in r1; return it.
	.type Mod, @function
Mod:
	swi 9         ; BIOS call 9
	mov r0, r1    ; result from r1
	bx lr         ; return
	.size Mod, 6

; filler 0x0200015c-0x020001b8
	.incbin "extract/usa/arm9/arm9.bin", 0x15c, 0x5c

	.type CpuFastSet, @function
CpuFastSet:
	swi 12        ; BIOS call 12
	bx lr         ; return
	.size CpuFastSet, 4

; filler 0x020001bc-0x0200020a
	.incbin "extract/usa/arm9/arm9.bin", 0x1bc, 0x4e

	.type IsDebugger, @function
IsDebugger:
	swi 15        ; BIOS call 15
	bx lr         ; return
	.size IsDebugger, 4

; filler 0x0200020e-0x02000278
	.incbin "extract/usa/arm9/arm9.bin", 0x20e, 0x6a

; VBlankIntrWait: wait for vertical blank (r2 = 0) through the BIOS interrupt-wait call.
	.type VBlankIntrWait, @function
VBlankIntrWait:
	mov r2, #0    ; r2 = 0
	swi 5         ; BIOS call 5
	bx lr         ; return
	.size VBlankIntrWait, 6

; filler 0x0200027e-0x020002e0
	.incbin "extract/usa/arm9/arm9.bin", 0x27e, 0x62

	.type Sqrt, @function
Sqrt:
	swi 13        ; BIOS call 13
	bx lr         ; return
	.size Sqrt, 4

; filler 0x020002e4-0x02000366
	.incbin "extract/usa/arm9/arm9.bin", 0x2e4, 0x82

	.type IntrWait, @function
IntrWait:
	swi 4         ; BIOS call 4
	bx lr         ; return
	.size IntrWait, 4

; filler 0x0200036a-0x020003d6
	.incbin "extract/usa/arm9/arm9.bin", 0x36a, 0x6c

	.type LZ77UnCompReadNormalWrite8bit, @function
LZ77UnCompReadNormalWrite8bit:
	swi 17        ; BIOS call 17
	bx lr         ; return
	.size LZ77UnCompReadNormalWrite8bit, 4

; filler 0x020003da-0x0200043c
	.incbin "extract/usa/arm9/arm9.bin", 0x3da, 0x62

	.type RLUnCompReadNormalWrite8bit, @function
RLUnCompReadNormalWrite8bit:
	swi 20        ; BIOS call 20
	bx lr         ; return
	.size RLUnCompReadNormalWrite8bit, 4

; filler 0x02000440-0x020004a4
	.incbin "extract/usa/arm9/arm9.bin", 0x440, 0x64

	.type Halt, @function
Halt:
	swi 6         ; BIOS call 6
	bx lr         ; return
	.size Halt, 4

; filler 0x020004a8-0x020004fe
	.incbin "extract/usa/arm9/arm9.bin", 0x4a8, 0x56

	.type WaitByLoop, @function
WaitByLoop:
	swi 3         ; BIOS call 3
	bx lr         ; return
	.size WaitByLoop, 4

; filler 0x02000502-0x0200056a
	.incbin "extract/usa/arm9/arm9.bin", 0x502, 0x68

	.type Div, @function
Div:
	swi 9         ; BIOS call 9
	bx lr         ; return
	.size Div, 4

; filler 0x0200056e-0x020005ea
	.incbin "extract/usa/arm9/arm9.bin", 0x56e, 0x7c

	.type BitUnPack, @function
BitUnPack:
	swi 16        ; BIOS call 16
	bx lr         ; return
	.size BitUnPack, 4

; filler 0x020005ee-0x0200065c
	.incbin "extract/usa/arm9/arm9.bin", 0x5ee, 0x6e

	.type LZ77UnCompReadByCallbackWrite16bit, @function
LZ77UnCompReadByCallbackWrite16bit:
	swi 18        ; BIOS call 18
	bx lr         ; return
	.size LZ77UnCompReadByCallbackWrite16bit, 4

; filler 0x02000660-0x020006ca
	.incbin "extract/usa/arm9/arm9.bin", 0x660, 0x6a

	.type RLUnCompReadByCallbackWrite16bit, @function
RLUnCompReadByCallbackWrite16bit:
	swi 21        ; BIOS call 21
	bx lr         ; return
	.size RLUnCompReadByCallbackWrite16bit, 4

; filler 0x020006ce-0x02000730
	.incbin "extract/usa/arm9/arm9.bin", 0x6ce, 0x62

	.type GetCRC16, @function
GetCRC16:
	swi 14        ; BIOS call 14
	bx lr         ; return
	.size GetCRC16, 4

; filler 0x02000734-0x020007b6
	.incbin "extract/usa/arm9/arm9.bin", 0x734, 0x82

	.type HuffUnCompReadByCallback, @function
HuffUnCompReadByCallback:
	swi 19        ; BIOS call 19
	bx lr         ; return
	.size HuffUnCompReadByCallback, 4

; filler 0x020007ba-0x02000800
	.incbin "extract/usa/arm9/arm9.bin", 0x7ba, 0x46
