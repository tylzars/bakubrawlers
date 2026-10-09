#include "Graphics/Model.hpp"
#include "Graphics/ModelInternal.hpp"
#include <nitro/os.h>
#include <nitro/mi.h>
#include <stdio.h>

extern "C" NNSG3dResTex *func_020335a8(void *file);
extern "C" u32 func_02031840(NNSG3dResTex *tex);
extern "C" u32 func_02031854(NNSG3dResTex *tex);
extern "C" u32 func_02031980(NNSG3dResTex *tex);
extern "C" void func_02031868(NNSG3dResTex *tex, u32 texKey, u32 tex4x4Key);
extern "C" void func_02031994(NNSG3dResTex *tex, u32 plttKey);

extern "C" void *func_0203324c(void *dict, const char *name);

void NitroTexture::func_02028280(const char *name) {
    void *file = func_02023504(name, 0, 0, 0, 0, 0);
    mOwnsFile  = 1;
    func_020282c4(file);
}

BOOL UnkStruct_02028308::func_020282c4(void *file) {
    if (file == NULL) {
        return FALSE;
    }
    if (mFile != NULL) {
        func_0202832c();
    }
    mFile = file;
    mTex  = func_020335a8(file);
    func_020283f8();
    return TRUE;
}

UnkStruct_02028308::UnkStruct_02028308() {
    mTex      = NULL;
    mFile     = NULL;
    mLoaded   = 0;
    mTexHandle   = 0;
    m4x4Handle = 0;
    mPlttHandle  = 0;
    mOwnsFile = 0;
}

void UnkStruct_02028308::func_0202832c() {
    func_02028388();
    func_02028344();
}

void UnkStruct_02028308::func_02028344() {
    if (mFile == NULL) {
        return;
    }
    if (mOwnsFile) {
        OS_Free(mFile);
        mOwnsFile = 0;
    }
    mTex  = NULL;
    mFile = NULL;
}

void UnkStruct_02028308::func_02028388() {
    if (mLoaded == 0) {
        return;
    }
    if (mPlttHandle != 0) {
        data_020a588c(mPlttHandle);
    }
    if (m4x4Handle != 0) {
        data_020a5884(m4x4Handle);
    }
    if (mTexHandle != 0) {
        data_020a5884(mTexHandle);
    }
    mLoaded = 0;
}

void UnkStruct_02028308::func_020283f8() {
    u32 texSize   = func_02031840(mTex);
    u32 size4x4   = func_02031854(mTex);
    u32 plttSize  = func_02031980(mTex);
    u32 texKey    = 0;
    if (texSize != 0) {
        texKey = data_020a5880(texSize, 0, 0);
    }
    u32 tex4x4Key = 0;
    if (size4x4 != 0) {
        tex4x4Key = data_020a5880(size4x4, 1, 0);
    }
    u32 plttKey = 0;
    if (plttSize != 0) {
        plttKey = data_020a5888(plttSize, 0, 1);
    }
    func_02031868(mTex, texKey, tex4x4Key);
    func_02031994(mTex, plttKey);
    func_020284e8();
    func_0202866c();
    mTexHandle    = texKey;
    m4x4Handle = tex4x4Key;
    mPlttHandle   = plttKey;
    mLoaded    = 1;
}

void UnkStruct_02028308::func_020284e8() {
    u32 ofs;
    NNSG3dResTex *tex = mTex;
    u32 size          = tex->mTexBlocks << 3;
    if (size != 0) {
        ofs       = tex->mTexData;
        u32 addr  = (tex->mTexHandle & 0xffff) << 3;
        void *buf = OS_Alloc(size);
        func_01ff9510((u8 *) tex + ofs, buf, size);
        func_02005e9c(buf, size);
        func_02025c1c(0, 2, addr, buf, size, 1);
        mTex->mTexState |= 1;
    }
    u32 idx;
    u32 ofs2;
    NNSG3dResTex *tex2 = mTex;
    u32 addr2;
    u32 size2 = tex2->m4x4Blocks << 3;
    if (size2 == 0) {
        return;
    }
    ofs2        = tex2->m4x4Data;
    addr2       = (tex2->m4x4Handle & 0xffff) << 3;
    idx         = tex2->m4x4IndexData;
    void *buf2  = OS_Alloc(size2);
    func_01ff9510((u8 *) tex2 + ofs2, buf2, size2);
    func_02005e9c(buf2, size2);
    func_02025c1c(0, 2, addr2, buf2, size2, 1);
    size2 >>= 1;
    void *buf3 = OS_Alloc(size2);
    func_01ff9510((u8 *) tex2 + idx, buf3, size2);
    func_02005e9c(buf3, size2);
    func_02025c1c(0, 2, 0x20000 + ((addr2 & 0x1ffff) >> 1) + ((addr2 & 0x40000) >> 2), buf3, size2, 1);
    mTex->m4x4State |= 1;
}

void UnkStruct_02028308::func_0202866c() {
    u32 ofs;
    NNSG3dResTex *tex = mTex;
    u32 size          = tex->mPlttBlocks << 3;
    ofs           = tex->mPlttData;
    u32 addr          = (tex->mPlttHandle & 0xffff) << 3;
    void *buf         = OS_Alloc(size);
    func_01ff9510((u8 *) tex + ofs, buf, size);
    func_02005e9c(buf, size);
    func_02025c1c(0, 3, addr, buf, size, 1);
    mTex->mPlttState |= 1;
}

BOOL UnkStruct_02028308::func_020286fc(const char *name, const char *file, u32 size) {
    char path[0x20];
    u32 outSize;
    sprintf(path, "%s.ntft", file);
    void *data = func_02023504(path, 0, 0, 0, &outSize, size);
    BOOL result = func_02028778(name, data, outSize);
    OS_Free(data);
    return result;
}

BOOL UnkStruct_02028308::func_02028778(const char *name, void *data, u32 size) {
    char buf[0x14];
    MI_CpuFill8(buf, 0, 0x14);
    sprintf(buf, name);
    u32 *entry;
    if (mTex == NULL) {
        entry = NULL;
    } else {
        entry = (u32 *) func_0203324c((u8 *) mTex + 0x3c, buf);
    }
    if (entry != NULL) {
        u32 addr  = (mTex->mTexHandle << 16 >> 13) + (*entry << 16 >> 13);
        void *dst = OS_Alloc(size);
        func_01ff9510(data, dst, size);
        func_02005e9c(dst, size);
        func_02025c1c(0, 2, addr, dst, size, 1);
    } else {
        return FALSE;
    }
    return TRUE;
}

BOOL UnkStruct_02028308::func_02028858(const char *name, const char *file, u32 size) {
    char path[0x20];
    u32 outSize;
    sprintf(path, "%s.ntfp", file);
    void *data = func_02023504(path, 0, 0, 0, &outSize, size);
    BOOL result = func_020288d4(name, data, outSize);
    OS_Free(data);
    return result;
}

// non-matching: 6 words; the original keeps `aligned` in r7 and `addr` in r6 (here swapped), with the
// lsl/bic pair in the other order. Everything else (loads, calls, stack) matches.
BOOL UnkStruct_02028308::func_020288d4(const char *name, void *data, u32 size) {
    char buf[0x14];
    MI_CpuFill8(buf, 0, 0x14);
    sprintf(buf, "%s_pl", name);
    u16 *entry;
    NNSG3dResTex *tex = mTex;
    if (tex == NULL || tex->mPlttNames == 0) {
        entry = NULL;
    } else {
        entry = (u16 *) func_0203324c((u8 *) tex + tex->mPlttNames, buf);
    }
    if (entry != NULL) {
        u32 key     = mTex->mPlttHandle;
        u32 addr    = (*entry << 3) + ((key << 19) >> 16);
        u32 aligned = (size + 3) & ~3;
        void *dst   = OS_Alloc(aligned);
        func_01ff9510(data, dst, aligned);
        func_02005e9c(dst, aligned);
        func_02025c1c(0, 3, addr, dst, aligned, 1);
    } else {
        return FALSE;
    }
    return TRUE;
}
