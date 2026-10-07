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

## Game files found so far

| Range | File | Notes |
|---|---|---|
| 0x020219dc-0x02021af8 | System/Random.cpp | seeding and weighted picks on `data_020d42e8` |
| 0x02021af8-0x02021c60 | System/InputManager.cpp | key state with auto-repeat |
| 0x02021c60-0x02021eb8 | System/Memory.cpp | game heap, `operator new`/`delete` |
| 0x02021eb8-0x020224f8 | Scene/SceneBase.cpp | `SceneManager` (scene list with layers) and the `SceneBase` constructor |
| 0x020224f8-0x02022b0c | System/TouchManager.cpp | touch sampling, hit tests |
| 0x02022b0c-0x020234c8 | Game/GameData.cpp | helpers over data_020d433c/data_020d43f0 (the save data) |
| 0x020234c8-0x02023b80 | System/FileSystem.cpp | file loading (`func_02023504`), compressed files, path buffer |
| 0x02023b80-0x020244cc | System/FileLoader.cpp | backup (save) read/write scenes and CRC |
| 0x020244cc-0x02024818 | System/SoundManager.cpp | sound archive, BGM and SE |
| 0x02024818-0x0202537c | System/SceneRunner.cpp | loads each scene's overlay and runs it |
| 0x0202537c-0x02025668 | System/FrameUpdate.cpp | frame callbacks and HBlank hooks |
| 0x02025668-0x020257c8 | Graphics/SpriteFade.cpp | screen fade state |
| 0x020257c8-0x02025e18 | Graphics/UploadQueue.cpp | queue of VRAM uploads (`func_02025b64`, `func_02025c1c`) |
| 0x02025e18-0x020275bc | Graphics/SpriteManager.cpp | identity matrices, Camera, CameraManager, SpriteManager methods |
| 0x020275bc-0x02028280 | Graphics/SpriteScreen.cpp | |
| 0x02028280-0x020289c8 | Graphics/NitroTexture.cpp | texture/palette resource and VRAM upload |
| 0x020289c8-0x02028bb4 | Graphics/NitroModel.cpp | model resource |
| 0x02028bb4-0x02029248 | Graphics/ModelInstance.cpp | render object, matrices, drawing |
| 0x02029248-0x020294e8 | Graphics/ModelAnimData.cpp | animation files per channel |
| 0x020294e8-0x02029b5c | Graphics/ModelAnim.cpp | animation player |
| 0x02029b5c-0x02029d7c | Graphics/ModelLight.cpp | the four 3D lights |
| 0x020305ac-0x02030880 | Graphics/ModelResource.cpp | model + texture + animation bundle |
| 0x02052548-0x020527a4 | Graphics/ModelTransform.cpp | model with a transform (CharacterModel follows at 0x020527a4) |
| 0x02029d7c-0x0202a594 | Graphics/CellAnimation.cpp | |
| 0x0202a594-0x0202ab24 | Graphics/CellAnimFile.cpp | cached cell-animation files |
| 0x0202ab24-0x0202c02c | Graphics/EffectManager.cpp | effect script interpreter (Effect) and EffectManager |
| 0x0202c02c-0x0202d794 | Text/TextManager.cpp | |
| 0x0202d794-0x0202db04 | System/MicManager.cpp | microphone sampling ("blow" detection), not text |
| 0x0202f420-0x020305ac | UI/SelectWindowCursor.cpp | |
| 0x0203dbb0-0x0203de40 | System/FrameTask.cpp | sound/voice task over the CRI library (`TaskBase` has no virtual destructor) |
| 0x0205b288-0x0205c8f0 | UI/SelectWindow.cpp | |
| 0x02065158-0x02065c40 | UI/MessageWindow.cpp | also the four-character-icon class `UnkStruct_020651bc` |

0x02030880-0x02030b64 is NitroSystem foundation code (list and heap helpers), not game code.
