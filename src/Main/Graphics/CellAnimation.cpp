#include "Graphics/CellAnimation.hpp"
#include "Graphics/SpriteManager.hpp"

#include <nitro/fx.h>


CellAnimation::CellAnimation(CellAnimFile *file, u32 unk2) {
    Init(file->mData->mFrames, unk2);
    mFile = NULL;
}

CellAnimation::CellAnimation(const char *path, u32 unk2, u16 unk3, u32 unk4) {
    mFile = new CellAnimFile(path, unk3, *(u16 *) &unk4);
    Init(mFile->mData->mFrames, unk2);
}

void CellAnimation::Init(CellAnimFrame *entries, u32 index) {
    mFrames  = entries;
    mAnimIndex   = index;
    mCharBase = 0;
    mPaletteBase   = 0;
    ResetAnimation();
}

CellAnimation::~CellAnimation() {
    CellAnimFile *file = mFile;
    if (file != NULL) {
        file->~CellAnimFile();
        operator delete(file);
    }
}

void CellAnimation::Update() {
    CellAnimFrame *entry = &mFrames[mAnimIndex];
    if (mMode == 5) {
        return;
    }
    mTimer = mTimer - mStep;
    if (mTimer > 0) {
        return;
    }
    switch (mMode) {
    case 0: {
        u32 next = mFrameIndex + 1;
        if (next < entry->mFrameCount) {
            mFrameIndex = next;
        } else {
            mFinished = 1;
        }
        break;
    }
    case 1:
        mFrameIndex++;
        mFrameIndex = mFrameIndex % entry->mFrameCount;
        break;
    case 2:
        if (mFrameIndex != 0) {
            mFrameIndex--;
        } else {
            mFinished = 1;
        }
        break;
    case 3:
        mFrameIndex--;
        if (mFrameIndex >= entry->mFrameCount) {
            mFrameIndex = entry->mFrameCount - 1;
        }
        break;
    case 4:
        mFrameIndex++;
        if (mFrameIndex == entry->mFrameCount) {
            mFrameIndex = entry->mFrameCount - 1;
            mFinished = 1;
        }
        break;
    }
    mTimer = entry->mCels[mFrameIndex].mDuration << 12;
}

void CellAnimation::ResetAnimation() {
    CellAnimFrame *entry = &mFrames[mAnimIndex];
    mFinished              = 0;
    mMode              = entry->mMode;
    mStep                = (s32)(4096.0f * entry->mSpeed);
    Rewind();
}

void CellAnimation::SetMode(u32 unk1) {
    mMode = unk1;
}

void CellAnimation::Rewind() {
    CellAnimFrame *entry = &mFrames[mAnimIndex];
    mFrameIndex              = entry->mStartFrame;
    mTimer               = entry->mCels[mFrameIndex].mDuration << 12;
}

void CellAnimation::DrawToScreen(SpriteScreen *screen, s32 x, s32 y, u8 unk4) {
    CellAnimCel *frame = &mFrames[mAnimIndex].mCels[mFrameIndex];
    if (frame->mRotation != 0 || frame->mScaleX != 0x1000 || frame->mScaleY != 0x1000) {
        SpriteTransform transform(frame->mScaleX, frame->mScaleY, frame->mRotation);
        screen->AddOam(frame->mOam, frame->mOamCount, (s16)(x + frame->mOffsetX), (s16)(y + frame->mOffsetY), unk4,
                              mCharBase, mPaletteBase, (u32)&transform);
    } else {
        screen->AddOam(frame->mOam, frame->mOamCount, (s16)(x + frame->mOffsetX), (s16)(y + frame->mOffsetY), unk4,
                              mCharBase, mPaletteBase, 0);
    }
}

void CellAnimation::DrawFrameToScreen(SpriteScreen *screen, u32 frameIndex, s32 x, s16 y, u8 unk5) {
    CellAnimCel *frame = &mFrames[mAnimIndex].mCels[frameIndex];
    if (frame->mRotation != 0 || frame->mScaleX != 0x1000 || frame->mScaleY != 0x1000) {
        SpriteTransform transform(frame->mScaleX, frame->mScaleY, frame->mRotation);
        screen->AddOam(frame->mOam, frame->mOamCount, (s16)(x + frame->mOffsetX), (s16)(y + frame->mOffsetY), unk5,
                              mCharBase, mPaletteBase, (u32)&transform);
    } else {
        screen->AddOam(frame->mOam, frame->mOamCount, (s16)(x + frame->mOffsetX), (s16)(y + frame->mOffsetY), unk5,
                              mCharBase, mPaletteBase, 0);
    }
}

void CellAnimation::DrawTransformedToScreen(SpriteScreen *screen, s32 x, s32 y, u8 unk4, s32 scaleX, s32 scaleY, u16 rotation) {
    CellAnimCel *frame = &mFrames[mAnimIndex].mCels[mFrameIndex];
    if (frame->mRotation != 0 || frame->mScaleX != 0x1000 || frame->mScaleY != 0x1000) {
        scaleX   = FX_Mul(scaleX, frame->mScaleX);
        scaleY   = FX_Mul(scaleY, frame->mScaleY);
        rotation = rotation + frame->mRotation;
    }
    SpriteTransform transform(scaleX, scaleY, rotation);
    screen->AddOam(frame->mOam, frame->mOamCount, (s16)(x + frame->mOffsetX), (s16)(y + frame->mOffsetY), unk4,
                          mCharBase, mPaletteBase, (u32)&transform);
}

void CellAnimation::Draw(u8 unk1, s16 x, s16 y, u8 unk4) {
    DrawToScreen(&SpriteManager::Get().Screen(unk1), x, y, unk4);
}

void CellAnimation::DrawFrame(u8 unk1, u8 frame, s16 x, s16 y, u8 unk5) {
    DrawFrameToScreen(&SpriteManager::Get().Screen(unk1), frame, x, y, unk5);
}

void CellAnimation::DrawTransformed(u32 screen, s32 x, s32 y, u32 mode, s32 scaleX, s32 scaleY, u32 unk7) {
    DrawTransformedToScreen(&SpriteManager::Get().Screen(screen), x, y, *(u8 *) &mode, scaleX, scaleY, *(u16 *) &unk7);
}

u32 CellAnimation::GetDuration() {
    CellAnimFrame *entry = &mFrames[mAnimIndex];
    CellAnimCel *frame = entry->mCels;
    u32 total            = 0;
    u16 i                = 0;
    do {
        total += frame->mDuration;
        frame++;
        i++;
    } while (entry->mFrameCount > i);
    return total;
}
