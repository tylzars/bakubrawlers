# ARM9 main module layout

A map of what lives where in the main module (`.text` 0x02000000-0x0209d490), from a survey of the disassembly.
Confidence varies; treat library boundaries marked "approx." as starting points. Library code (NitroSDK, MSL,
middleware) is not decompiled as game source; it will be matched or linked separately.

## Libraries and runtime

| Range | Contents |
|---|---|
| 0x02000000-0x02000800 | Secure area, BIOS call thunks |
| 0x02000800-0x02000c04 | crt0, build info strings (NitroSDK, Mobiclip SDK) |
| 0x02000c04-0x02000cb4 | `main` (game) |
| 0x02000cb4-0x02001c6c | FX / MTX math (approx.) |
| 0x02001c6c-0x02004b50 | GX / G2 / G3 |
| 0x02004b64-0x02008000 | OS (init, interrupts, cache, heap, tick/alarm) |
| 0x02008000-0x02009050 | SND command layer |
| 0x02009050-0x02009558 | PXI |
| 0x02009558-0x0200bb88 | FS (approx.) |
| 0x0200be84-0x0200d3b4 | TP, MIC, PM |
| 0x0200da80-0x0200e310 | RTC (approx.) |
| 0x0200e310-0x02011c34 | CARD and CARD backup (save memory) |
| 0x02011c34-0x020122b0 | CTRDG (approx.) |
| 0x020122b0-0x020202bc | MSL C runtime: printf family, string, math, soft float, integer division (`_s32_div_f` at 0x0201f970) |
| 0x020202bc-0x0202030c | C++ runtime glue (`__init_cpp`, `__register_global_object`) |
| 0x02036000-0x0203a52c | NNS sound (approx.) |
| 0x0203a544-0x0203c1d8 | Mobiclip video wrapper; its ARM kernels are at 0x02090b24-0x0209d490 |
| 0x0203dee8-0x02052320 | CRI audio middleware (ADX/AHX and friends) |
| 0x0208b4bc-0x02090b24 | Tables (sine/cosine and others) |

## Game code

Roughly 0x0202030c-0x0208b4a4 minus the library blocks above (about 1370 functions). `operator new`/`delete`
(0x02021e50/0x02021e80) are game wrappers over `OS_AllocFromHeap`/`OS_FreeToHeap`. The 3D code at
0x02031478-0x02035e00 works on NNS G3d data but keeps its render state in an object owned by `main`, so it is
treated as game code. Proposed source files follow the class boundaries (vtables, constructors and the `.data`/
`.bss` each range uses), under `src/Main/<Category>/`.
