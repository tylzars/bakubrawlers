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
