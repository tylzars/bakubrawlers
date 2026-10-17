#include "Text/TextManager.hpp"
#include "UI/StorytalkWindow.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"
#include "Text/TextManager.hpp"
#include <nitro/mi.h>
#include <nitro/os.h>

void func_02087d68(StorytalkWindow *self);

void func_020880f4(StorytalkWindow *self);

void func_0208813c(StorytalkWindow *self, s32 unk1);

void func_020882c4(StorytalkWindow *self);

void func_020889d8(StorytalkWindow *self, s32 unk1);

void func_02088a24(StorytalkWindow *self);

void func_02088b30(StorytalkWindow *self);

void func_02088cd8(StorytalkWindow *self);

StorytalkWindow::StorytalkWindow() :
    SceneBase(0x3e8, 0) {
    mMessages = NULL;
    mStoryCharaNames = NULL;
    mState = 0;
    mUnk_31 = 0;
    mMessageIndex = -1;
    mUnk_36 = 0;
    mTextPage = 0;
    mLastTextPage = 0;
    mUnk_39 = 0;
    mUnk_3a = 0x14;
    mUnk_3b = 0x40;
    mUnk_3c = 0;
    mUnk_3e = 0;
    mCharacterId = -1;
    mHasMessage = 0;
    mCell1Visible = 1;
    mCloseRequested = 0;
    mBlink = 0;
    mBlinkHidden = 0;
    mUnk_4a = 0;
    mUnk_4b = 0;
    MI_CpuClear32(mCellAnims, 0xc);
    MI_CpuClear32(mTextBuffers, 8);
    MI_CpuClear32(mTextBufferSizes, 8);
}

StorytalkWindow::~StorytalkWindow() {
    for (u8 i = 0; i < 3; i++) {
        delete mCellAnims[i];
    }
    if (mMessages != NULL) {
        OS_Free(mMessages);
    }
    if (mStoryCharaNames != NULL) {
        OS_Free(mStoryCharaNames);
    }
    for (u8 i = 0; i < 2; i++) {
        if (mTextBuffers[i] != NULL) {
            OS_Free(mTextBuffers[i]);
        }
    }
}

void StorytalkWindow::Update() {
    if (mCloseRequested != 0) {
        mState = 6;
    }
    if (mBlink != 0) {
        mBlinkHidden = (mBlinkHidden + 1) & 1;
    }
    switch (mState) {
        case 0:
            func_02087d68(this);
            LoadGraphics();
            mState++;
            break;
        case 1:
            if (mMessages != NULL) {
                mState++;
            }
            break;
        case 2:
            if (mHasMessage != 0) {
                if (mUnk_4b == 0) {
                    SoundManager::Get().StartSeqArc(0x18);
                }
                mSlideTimer = 0xc;
                mState++;
            }
            break;
        case 3:
            func_02088cd8(this);
            if (mSlideTimer != 0) {
                mSlideTimer--;
            } else {
                mState++;
            }
            break;
        case 4:
            func_02088a24(this);
            func_02088cd8(this);
            func_020882c4(this);
            mLastPageDone = 0;
            mState++;
            break;
        case 5:
            func_02088a24(this);
            func_02088b30(this);
            func_02088cd8(this);
            if (mLastPageDone != 0 && mUnk_4a == 0) {
                mHasMessage = 0;
                mState = 2;
            }
            break;
        case 6:
            RequestScene(1);
            mFinished = 1;
            break;
    }
    if (mUnk_3c != 0) {
        mUnk_3c--;
    }
}

extern u16 data_020c1a4c[];


extern const u8 data_020a41cc[];

extern const s8 data_020a41c8[];


void func_02087ccc(StorytalkWindow *self, void *data);
void func_02087d68(StorytalkWindow *self);
void func_020880f4(StorytalkWindow *self);
void func_0208813c(StorytalkWindow *self, s32 unk1);
void func_020889d8(StorytalkWindow *self, s32 unk1);
void func_02088a24(StorytalkWindow *self);
void func_02088b30(StorytalkWindow *self);
void func_02088cd8(StorytalkWindow *self);
void func_02088ec4(StorytalkWindow *self);
void func_02088f28(StorytalkWindow *self, u8 unk1);
u8 func_02088f38();

BOOL StorytalkWindow::func_02087cac() {
    return mState > 2;
}

void StorytalkWindow::Close() {
    mCloseRequested = 1;
}

void func_02087ccc(StorytalkWindow *self, void *data) {
    if (self->mMessages != NULL) {
        OS_Free(self->mMessages);
    }
    self->mMessages = data;
}

void StorytalkWindow::LoadMessageFile(const char *path) {
    if (mMessages != NULL) {
        OS_Free(mMessages);
    }
    ChangeDir("/");
    mMessages = LoadFile(path, 0, 0, 0, 0, 0);
}

void func_02087d68(StorytalkWindow *self) {
    ChangeDir("/");
    self->mStoryCharaNames = LoadFile("/font/mes_StoryCharaName.mes", 0, 0, 0, 0, 0);
    SpriteManager::Get().Screen(0).SetObjVramMode(3);
    self->mTextBufferSizes[0] = TextManager::Get().BufferSize(0x18, 8);
    self->mTextBuffers[0] = OS_Alloc(self->mTextBufferSizes[0]);
    MI_CpuClear16(self->mTextBuffers[0], self->mTextBufferSizes[0]);
    self->mTextBufferSizes[1] = TextManager::Get().BufferSize(0x10, 4);
    self->mTextBuffers[1] = OS_Alloc(self->mTextBufferSizes[1]);
    MI_CpuClear16(self->mTextBuffers[1], self->mTextBufferSizes[1]);
    func_020880f4(self);
}

// Re-entrant: deletes and recreates the cells; also called by the scenes that reload it (UnkStruct_02065c40.cpp,
// BattleConfrontCard, BattleCommand, BattleObj00, BattleHud, BattleShoot).
void StorytalkWindow::LoadGraphics() {
    char path[0x80];
    ChangeDir("/story/");
    SpriteScreen &screen = SpriteManager::Get().Screen(0);
    screen.SetObjVramMode(1);
    sprintf(path, "storytalk_ob.nce_ncg1D.bin");
    screen.LoadCharacters(path, 0x4000, 0);
    sprintf(path, "storytalk_ob_ncl.bin");
    screen.LoadPalette(path, 0x180, 0);
    for (u8 i = 0; i <= 2; i++) {
        delete mCellAnims[i];
        mCellAnims[i]            = new CellAnimation("storytalk_ob_nce.bin", 0, 1, 1);
        mCellAnims[i]->mCharBase = 0x200;
        mCellAnims[i]->mPaletteBase   = 12;
    }
    mCellAnims[0]->SetAnimation(0);
    mCellAnims[1]->SetAnimation(1);
    mCellAnims[2]->SetAnimation(2);
    ChangeDir("/");
    TextManager::Get().SetColor(0, 0, 0xf);
}

void func_020880f4(StorytalkWindow *self) {
    MI_CpuClearFast((void *) 0x6406000, 0x1800);
    MI_CpuClear16(self->mTextBuffers[0], self->mTextBufferSizes[0]);
    self->mUnk_36 = 1;
    self->mUnk_39 = 0;
    self->mMessageIndex = -1;
}

void func_0208813c(StorytalkWindow *self, s32 unk1) {
    if (self->mMessageIndex == unk1) {
        return;
    }
    func_020880f4(self);
    self->mMessageIndex = unk1;
    self->mUnk_36 = 0;
    self->mTextPage = 0;
    self->mLastTextPage = 0;
    if (self->mMessageIndex < 0) {
        self->mState = 2;
        self->mHasMessage = 0;
        return;
    }
    self->mHasMessage = 1;
    if (self->func_02087cac()) {
        self->mState = 4;
    }
    TextManager &text = TextManager::Get();
    s16 y;
    s16 x;
    x = 0;
    y = 0x20;
    u16 index         = self->mMessageIndex;
    MessageFile *file;
    void *buffer = self->mTextBuffers[0];
    file         = (MessageFile *) self->mMessages;
    MI_CpuClear16(data_020c1a4c, 0x20);
    text.mLineStartCount = 1;
    text.Print(file->GetText(index), file->GetLength(index), buffer, 0x20, 6, &y, &x, NULL, 0, 0, 0x21, 0x10, 0, 1, 0, 1, 1);
    for (u8 i = 1; i < 0x10; i++) {
        if (text.mLineStarts[i] == 0) {
            text.mLineStarts[i] = text.mUnk_36[i];
        }
    }
}

extern char data_020d441a[];

void func_020882c4(StorytalkWindow *self) {
    s16 h;
    s16 w;
    if (self->mMessageIndex < 0) {
        return;
    }
    u32 a = TextManager::Get().GetLineStart((u8) ((self->mLastTextPage + 1) * 3));
    u32 b = TextManager::Get().GetLineStart((u8) ((self->mLastTextPage + 2) * 3));
    if (a < b) {
        self->mUnk_39 = 1;
    }
    if (self->mUnk_39 != 0) {
        self->mLastTextPage++;
    }
    if (self->mUnk_39 != 0) {
        TextManager &first = TextManager::Get();
        TextManager &t1 = TextManager::Get();
        TextManager &text = first;


        s16 y;
        s16 x;
        x = -(self->mTextPage * 0x30);
        y = 0x20;
        void *buffer      = self->mTextBuffers[0];
        u16 count         = t1.GetLineStart((u8) ((self->mTextPage + 1) * 3));
        MessageFile *file = (MessageFile *) self->mMessages;
        u16 index         = self->mMessageIndex;
        u16 len           = file->GetLength(index);
        const u8 *str     = file->GetText(index);
        u16 n             = count;
        if (len < count * 2) {
            n = len >> 1;
        }
        text.mLineStartCount = 0x10;
        text.Print(str, n * 2, buffer, 0x20, 6, &y, &x, NULL, 0, 2, 0x21, 0x10, 0, 0, 1, 1, 1);
        self->mUnk_3e = 2;
    } else {
        TextManager &text = TextManager::Get();
        s16 y;
        s16 x;
        x = -(self->mTextPage * 0x30);
        y = 0x20;
        MessageFile *file = (MessageFile *) self->mMessages;
        u16 index         = self->mMessageIndex;
        text.Print(file->GetText(index), file->GetLength(index), self->mTextBuffers[0], 0x20, 6, &y, &x, NULL, 0, 2, 0x21, 0x10, 0, 0, 1, 1, 1);
        s32 w = ((u16) x - 0x10) / 16;
        if (w <= 0) {
            w = 0;
        }
        self->mUnk_3e = w;
        self->mUnk_36 = 1;
    }
    QueueUpload(SpriteManager::Get().mScreens[0].mBgIndex, 4, 0x6000, self->mTextBuffers[0], 0x1800, 0);
    if (self->mTextPage != 0) {
        TextManager &first = TextManager::Get();
        TextManager &text = TextManager::Get();
        u16 a2            = first.GetLineStart((u8) ((self->mTextPage + 1) * 3));

        u16 b2            = text.GetLineStart((u8) (self->mTextPage * 3));
        self->mUnk_3c     = a2 - b2;
    } else {
        self->mUnk_3c = TextManager::Get().GetLineStart((u8) ((self->mTextPage + 1) * 3));
    }
    self->mUnk_3c = self->mUnk_3c * 2 / 3;
    if (self->mMessageIndex < 0 || self->mCharacterId < 0) {
        return;
    }
    TextManager::Get().mUse4bppFont = 1;
    if (self->mCharacterId == 0x15) {
        h = 0;
        w = 0;
        TextManager::Get().Print(data_020d441a, 0x18, self->mTextBuffers[1], 0x20, 2, &h, &w, NULL, 0, 2, 0x21, 0, 0, 0, 0, 1, 1);
    } else {
        TextManager &text = TextManager::Get();
        s16 y;
        s16 x;
        x = 0;
        y = 0;

        MessageFile *file = (MessageFile *) self->mStoryCharaNames;
        u16 index         = self->mCharacterId;
        text.Print(
file->GetText(index), file->GetLength(index), self->mTextBuffers[1], 0x20, 2, &y, &x, NULL, 0, 2, 0x21, 0, 0, 0, 0, 1, 1);
    }
    TextManager::Get().mUse4bppFont = 0;
    QueueUpload(SpriteManager::Get().mScreens[0].mBgIndex, 4, 0x7800, self->mTextBuffers[1], self->mTextBufferSizes[1], 0);
}

void func_020889d8(StorytalkWindow *self, s32 unk1) {
    if (unk1 == self->mCharacterId) {
        return;
    }
    self->mCharacterId = unk1;
    MI_CpuClearFast((void *) 0x6407800, 0x800);
    MI_CpuClear16(self->mTextBuffers[1], self->mTextBufferSizes[1]);
}

void StorytalkWindow::func_02088a18(u32 unk1, u32 unk2) {
    mUnk_3f = unk1;
    mUnk_40 = unk2;
}

void func_02088a24(StorytalkWindow *self) {
    if (self->mBlink != 0 && self->mBlinkHidden != 0) {
        return;
    }
    if (self->mUnk_4b != 0) {
        return;
    }
    u8 i = 0;
    do {
        SpriteManager::Get().Screen(0).AddOam((void *) data_020a41cc, 1, (s16) (i % 8 * 0x20 + 0x12), 0x82, 0, (u16) (i * 8 + 0x3c0), 0xf, 0);
        i++;
    } while (i < 8);
}

void func_02088b30(StorytalkWindow *self) {
    if (self->mBlink != 0 && self->mBlinkHidden != 0) {
        return;
    }
    s16 shift = -(self->mSlideTimer * self->mSlideTimer) / 8;
    u8 i;
    s16 x;
    s16 y;
    if (self->mUnk_4b != 0) {
        x = self->mUnk_3a;
        y = self->mUnk_3b;
    } else {
        x = 0x14;
        y = (s16) (data_020a41c8[self->mUnk_3e] + 0x96);
    }
    i = 0;
    do {
        SpriteManager::Get().Screen(0).AddOam((void *) data_020a41cc, 1, (s16) (x + i % 8 * 0x20 - 0x20), (s16) (y + i / 8 * 12 + shift), 0, (u16) (i * 8 + 0x300), 0xf, 0);
        i++;
    } while (i < 0x18);
    if (self->mCell1Visible == 0) {
        return;
    }
    if (self->mUnk_4b != 0) {
        return;
    }
    self->mCellAnims[1]->Update();
    self->mCellAnims[1]->Draw(0, 0xf0, (s16) (y + 0x1c), 0);
}

void func_02088cd8(StorytalkWindow *self) {
    if (self->mBlink != 0 && self->mBlinkHidden != 0) {
        return;
    }
    if (self->mUnk_4b != 0) {
        return;
    }
    s16 shift = -(self->mSlideTimer * self->mSlideTimer) / 8;
    if (self->mMessageIndex >= 0) {
        s16 x = self->mUnk_40;
        if ((x < 0xe8) & (x > 0x18)) {
            if (self->mUnk_3f < 3) {
                self->mCellAnims[2]->DrawFrame(0, self->mUnk_3f, x, (s16) (shift + 0x60), 0);
            }
        }
    }
    self->mCellAnims[0]->Draw(0, 0x80, (s16) (shift + 0x60), 0);
}

BOOL StorytalkWindow::NextPage(u32 unk1) {
    if (unk1 != 0) {
        SoundManager::Get().StartSeqArc(0x6b);
    }
    if (mTextPage >= mLastTextPage) {
        mLastPageDone = 1;
        return FALSE;
    }
    MI_CpuClear16(mTextBuffers[0], mTextBufferSizes[0]);
    mTextPage++;
    mLastTextPage = mTextPage;
    mUnk_39 = 0;
    mState = 4;
    return TRUE;
}

void StorytalkWindow::SetMessage(s32 unk1, s32 unk2, u32 unk3, u32 unk4) {
    mUnk_4b = unk4;
    func_0208813c(this, unk1);
    if (unk2 == 0x64) {
        if (data_020d433c.mUnk_82 != 0) {
            unk2 = 0;
        } else {
            unk2 = 5;
        }
    }
    func_020889d8(this, unk2);
    if (unk3 != 0) {
        mState = 2;
    }
}

void StorytalkWindow::SetCell1Visible(u32 unk1) {
    mCell1Visible = unk1;
}

void func_02088ec4(StorytalkWindow *self) {
    self->SetMessage(-1, -1, 0, 0);
    self->mUnk_3c = 0;
}

u32 StorytalkWindow::func_02088ef4() {
    return mUnk_3c;
}

BOOL StorytalkWindow::HasMorePages() {
    return mTextPage < mLastTextPage;
}

BOOL StorytalkWindow::IsTextShown() {
    return mState == 5;
}

void func_02088f28(StorytalkWindow *self, u8 unk1) {
    self->mBlink = unk1;
    self->mBlinkHidden = 0;
}
