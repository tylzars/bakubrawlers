#include "Graphics/Model.hpp"
#include "Graphics/ModelInternal.hpp"
#include <nitro/os.h>
#include <nitro/mi.h>
#include <stdio.h>

extern "C" NNSG3dResTex *GetResTex(void *file);
extern "C" u32 GetTexSize(NNSG3dResTex *tex);
extern "C" u32 GetTex4x4Size(NNSG3dResTex *tex);
extern "C" u32 GetPlttSize(NNSG3dResTex *tex);
extern "C" void SetTexKeys(NNSG3dResTex *tex, u32 texKey, u32 tex4x4Key);
extern "C" void SetPlttKey(NNSG3dResTex *tex, u32 plttKey);

extern "C" void *FindDictEntryByName(void *dict, const char *name);

void NitroTexture::Load(const char *name) {
    void *file = LoadFile(name, 0, 0, 0, 0, 0);
    mOwnsFile  = 1;
    LoadData(file);
}

BOOL TextureFile::LoadData(void *file) {
    if (file == NULL) {
        return FALSE;
    }
    if (mFile != NULL) {
        Release();
    }
    mFile = file;
    mTex  = GetResTex(file);
    AllocVramAndUpload();
    return TRUE;
}

TextureFile::TextureFile() {
    mTex      = NULL;
    mFile     = NULL;
    mLoaded   = 0;
    mTexHandle   = 0;
    m4x4Handle = 0;
    mPlttHandle  = 0;
    mOwnsFile = 0;
}

void TextureFile::Release() {
    FreeVram();
    FreeFile();
}

void TextureFile::FreeFile() {
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

void TextureFile::FreeVram() {
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

void TextureFile::AllocVramAndUpload() {
    u32 texSize   = GetTexSize(mTex);
    u32 size4x4   = GetTex4x4Size(mTex);
    u32 plttSize  = GetPlttSize(mTex);
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
    SetTexKeys(mTex, texKey, tex4x4Key);
    SetPlttKey(mTex, plttKey);
    UploadTextures();
    UploadPalettes();
    mTexHandle    = texKey;
    m4x4Handle = tex4x4Key;
    mPlttHandle   = plttKey;
    mLoaded    = 1;
}

void TextureFile::UploadTextures() {
    u32 ofs;
    NNSG3dResTex *tex = mTex;
    u32 size          = tex->mTexBlocks << 3;
    if (size != 0) {
        ofs       = tex->mTexData;
        u32 addr  = (tex->mTexHandle & 0xffff) << 3;
        void *buf = OS_Alloc(size);
        MIi_CpuCopy32((u8 *) tex + ofs, buf, size);
        DC_FlushRange(buf, size);
        QueueUpload(0, UPLOAD_KIND_TEX, addr, buf, size, 1);
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
    MIi_CpuCopy32((u8 *) tex2 + ofs2, buf2, size2);
    DC_FlushRange(buf2, size2);
    QueueUpload(0, UPLOAD_KIND_TEX, addr2, buf2, size2, 1);
    size2 >>= 1;
    void *buf3 = OS_Alloc(size2);
    MIi_CpuCopy32((u8 *) tex2 + idx, buf3, size2);
    DC_FlushRange(buf3, size2);
    QueueUpload(0, UPLOAD_KIND_TEX, 0x20000 + ((addr2 & 0x1ffff) >> 1) + ((addr2 & 0x40000) >> 2), buf3, size2, 1);
    mTex->m4x4State |= 1;
}

void TextureFile::UploadPalettes() {
    u32 ofs;
    NNSG3dResTex *tex = mTex;
    u32 size          = tex->mPlttBlocks << 3;
    ofs           = tex->mPlttData;
    u32 addr          = (tex->mPlttHandle & 0xffff) << 3;
    void *buf         = OS_Alloc(size);
    MIi_CpuCopy32((u8 *) tex + ofs, buf, size);
    DC_FlushRange(buf, size);
    QueueUpload(0, UPLOAD_KIND_TEX_PLTT, addr, buf, size, 1);
    mTex->mPlttState |= 1;
}

BOOL TextureFile::LoadTextureFile(const char *name, const char *file, u32 size) {
    char path[0x20];
    u32 outSize;
    sprintf(path, "%s.ntft", file);
    void *data = LoadFile(path, 0, 0, 0, &outSize, size);
    BOOL result = UploadNamedTexture(name, data, outSize);
    OS_Free(data);
    return result;
}

BOOL TextureFile::UploadNamedTexture(const char *name, void *data, u32 size) {
    char buf[0x14];
    MI_CpuFill8(buf, 0, 0x14);
    sprintf(buf, name);
    u32 *entry;
    if (mTex == NULL) {
        entry = NULL;
    } else {
        entry = (u32 *) FindDictEntryByName((u8 *) mTex + 0x3c, buf);
    }
    if (entry != NULL) {
        u32 addr  = (mTex->mTexHandle << 16 >> 13) + (*entry << 16 >> 13);
        void *dst = OS_Alloc(size);
        MIi_CpuCopy32(data, dst, size);
        DC_FlushRange(dst, size);
        QueueUpload(0, UPLOAD_KIND_TEX, addr, dst, size, 1);
    } else {
        return FALSE;
    }
    return TRUE;
}

BOOL TextureFile::LoadPaletteFile(const char *name, const char *file, u32 size) {
    char path[0x20];
    u32 outSize;
    sprintf(path, "%s.ntfp", file);
    void *data = LoadFile(path, 0, 0, 0, &outSize, size);
    BOOL result = UploadNamedPalette(name, data, outSize);
    OS_Free(data);
    return result;
}

BOOL TextureFile::UploadNamedPalette(const char *name, void *data, u32 size) {
    char buf[0x14];
    MI_CpuFill8(buf, 0, 0x14);
    sprintf(buf, "%s_pl", name);
    u16 *entry;
    NNSG3dResTex *tex = mTex;
    if (tex == NULL || tex->mPlttNames == 0) {
        entry = NULL;
    } else {
        entry = (u16 *) FindDictEntryByName((u8 *) tex + tex->mPlttNames, buf);
    }
    if (entry != NULL) {
        u32 addr    = (*entry << 3) + (u16) (mTex->mPlttHandle << 3);
        u32 aligned = (size + 3) & ~3;
        void *dst   = OS_Alloc(aligned);
        MIi_CpuCopy32(data, dst, aligned);
        DC_FlushRange(dst, aligned);
        QueueUpload(0, UPLOAD_KIND_TEX_PLTT, addr, dst, aligned, 1);
    } else {
        return FALSE;
    }
    return TRUE;
}
