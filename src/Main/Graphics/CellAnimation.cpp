#include "Graphics/CellAnimation.hpp"
#include "Graphics/SpriteManager.hpp"

#include <nitro/fx.h>


CellAnimation::CellAnimation(CellAnimFile *file, u32 unk2) {
    func_02029df8(file->mData->mFrames, unk2);
    mFile = NULL;
}

CellAnimation::CellAnimation(const char *path, u32 unk2, u32 unk3, u32 unk4) {
    mFile = new CellAnimFile(path, unk3, *(u16 *) &unk4);
    func_02029df8(mFile->mData->mFrames, unk2);
}

void CellAnimation::func_02029df8(CellAnimFrame *entries, u32 index) {
    mFrames  = entries;
    mUnk_0e   = index;
    mCharBase = 0;
    mUnk_11   = 0;
    func_02029f6c();
}

CellAnimation::~CellAnimation() {
    CellAnimFile *file = mFile;
    if (file != NULL) {
        file->~CellAnimFile();
        operator delete(file);
    }
}

void CellAnimation::Update() {
    CellAnimFrame *entry = &mFrames[mUnk_0e];
    if (mUnk_10 == 5) {
        return;
    }
    mTimer = mTimer - mStep;
    if (mTimer > 0) {
        return;
    }
    switch (mUnk_10) {
    case 0: {
        u32 next = mUnk_0f + 1;
        if (next < entry->mUnk_10) {
            mUnk_0f = next;
        } else {
            mUnk_12 = 1;
        }
        break;
    }
    case 1:
        mUnk_0f++;
        mUnk_0f = mUnk_0f % entry->mUnk_10;
        break;
    case 2:
        if (mUnk_0f != 0) {
            mUnk_0f--;
        } else {
            mUnk_12 = 1;
        }
        break;
    case 3:
        mUnk_0f--;
        if (mUnk_0f >= entry->mUnk_10) {
            mUnk_0f = entry->mUnk_10 - 1;
        }
        break;
    case 4:
        mUnk_0f++;
        if (mUnk_0f == entry->mUnk_10) {
            mUnk_0f = entry->mUnk_10 - 1;
            mUnk_12 = 1;
        }
        break;
    }
    mTimer = entry->mCels[mUnk_0f].mDuration << 12;
}

void CellAnimation::func_02029f6c() {
    CellAnimFrame *entry = &mFrames[mUnk_0e];
    mUnk_12              = 0;
    mUnk_10              = entry->mMode;
    mStep                = (s32)(4096.0f * entry->mSpeed);
    func_02029fc0();
}

void CellAnimation::func_02029fb8(u32 unk1) {
    mUnk_10 = unk1;
}

void CellAnimation::func_02029fc0() {
    CellAnimFrame *entry = &mFrames[mUnk_0e];
    mUnk_0f              = entry->mStartFrame;
    mTimer               = entry->mCels[mUnk_0f].mDuration << 12;
}

void CellAnimation::func_02029ffc(SpriteScreen *screen, s32 x, s32 y, u8 unk4) {
    CellAnimCel *frame = &mFrames[mUnk_0e].mCels[mUnk_0f];
    if (frame->mRotation != 0 || frame->mScaleX != 0x1000 || frame->mScaleY != 0x1000) {
        SpriteTransform transform(frame->mScaleX, frame->mScaleY, frame->mRotation);
        screen->func_02027778(frame->mOam, frame->mOamCount, (s16)(x + frame->mOffsetX), (s16)(y + frame->mOffsetY), unk4,
                              mCharBase, mUnk_11, (u32)&transform);
    } else {
        screen->func_02027778(frame->mOam, frame->mOamCount, (s16)(x + frame->mOffsetX), (s16)(y + frame->mOffsetY), unk4,
                              mCharBase, mUnk_11, 0);
    }
}

void CellAnimation::func_0202a120(SpriteScreen *screen, u32 frameIndex, s32 x, s16 y, u8 unk5) {
    CellAnimCel *frame = &mFrames[mUnk_0e].mCels[frameIndex];
    if (frame->mRotation != 0 || frame->mScaleX != 0x1000 || frame->mScaleY != 0x1000) {
        SpriteTransform transform(frame->mScaleX, frame->mScaleY, frame->mRotation);
        screen->func_02027778(frame->mOam, frame->mOamCount, (s16)(x + frame->mOffsetX), (s16)(y + frame->mOffsetY), unk5,
                              mCharBase, mUnk_11, (u32)&transform);
    } else {
        screen->func_02027778(frame->mOam, frame->mOamCount, (s16)(x + frame->mOffsetX), (s16)(y + frame->mOffsetY), unk5,
                              mCharBase, mUnk_11, 0);
    }
}

void CellAnimation::func_0202a244(SpriteScreen *screen, s32 x, s32 y, u8 unk4, s32 scaleX, s32 scaleY, u16 rotation) {
    CellAnimCel *frame = &mFrames[mUnk_0e].mCels[mUnk_0f];
    if (frame->mRotation != 0 || frame->mScaleX != 0x1000 || frame->mScaleY != 0x1000) {
        scaleX   = FX_Mul(scaleX, frame->mScaleX);
        scaleY   = FX_Mul(scaleY, frame->mScaleY);
        rotation = rotation + frame->mRotation;
    }
    SpriteTransform transform(scaleX, scaleY, rotation);
    screen->func_02027778(frame->mOam, frame->mOamCount, (s16)(x + frame->mOffsetX), (s16)(y + frame->mOffsetY), unk4,
                          mCharBase, mUnk_11, (u32)&transform);
}

void CellAnimation::Draw(u8 unk1, s32 x, s32 y, u32 unk4) {
    func_02029ffc(&SpriteManager::Get().Screen(unk1), x, y, *(u8 *) &unk4);
}

void CellAnimation::func_0202a3ec(u8 unk1, u8 frame, s16 x, s16 y, u32 unk5) {
    func_0202a120(&SpriteManager::Get().Screen(unk1), frame, x, y, *(u8 *) &unk5);
}

void CellAnimation::func_0202a494(u32 screen, s32 x, s32 y, u32 mode, s32 scaleX, s32 scaleY, u32 unk7) {
    func_0202a244(&SpriteManager::Get().Screen(screen), x, y, *(u8 *) &mode, scaleX, scaleY, *(u16 *) &unk7);
}

u32 CellAnimation::func_0202a54c() {
    CellAnimFrame *entry = &mFrames[mUnk_0e];
    CellAnimCel *frame = entry->mCels;
    u32 total            = 0;
    u16 i                = 0;
    do {
        total += frame->mDuration;
        frame++;
        i++;
    } while (entry->mUnk_10 > i);
    return total;
}
