#include "Text/TextManager.hpp"
#include "Unknown/UnkStruct_02087898.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"
#include "Text/TextManager.hpp"
#include <nitro/mi.h>
#include <nitro/os.h>

void func_02087d68(UnkStruct_02087898 *self);

void func_020880f4(UnkStruct_02087898 *self);

void func_0208813c(UnkStruct_02087898 *self, s32 unk1);

void func_020882c4(UnkStruct_02087898 *self);

void func_020889d8(UnkStruct_02087898 *self, s32 unk1);

void func_02088a24(UnkStruct_02087898 *self);

void func_02088b30(UnkStruct_02087898 *self);

void func_02088cd8(UnkStruct_02087898 *self);

UnkStruct_02087898::UnkStruct_02087898() :
    SceneBase(0x3e8, 0) {
    mUnk_18 = NULL;
    mUnk_1c = NULL;
    mUnk_30 = 0;
    mUnk_31 = 0;
    mUnk_34 = -1;
    mUnk_36 = 0;
    mUnk_37 = 0;
    mUnk_38 = 0;
    mUnk_39 = 0;
    mUnk_3a = 0x14;
    mUnk_3b = 0x40;
    mUnk_3c = 0;
    mUnk_3e = 0;
    mUnk_42 = -1;
    mUnk_44 = 0;
    mUnk_46 = 1;
    mUnk_47 = 0;
    mUnk_48 = 0;
    mUnk_49 = 0;
    mUnk_4a = 0;
    mUnk_4b = 0;
    MI_CpuClear32(mUnk_0c, 0xc);
    MI_CpuClear32(mUnk_20, 8);
    MI_CpuClear32(mUnk_28, 8);
}

UnkStruct_02087898::~UnkStruct_02087898() {
    for (u8 i = 0; i < 3; i++) {
        delete mUnk_0c[i];
    }
    if (mUnk_18 != NULL) {
        OS_Free(mUnk_18);
    }
    if (mUnk_1c != NULL) {
        OS_Free(mUnk_1c);
    }
    for (u8 i = 0; i < 2; i++) {
        if (mUnk_20[i] != NULL) {
            OS_Free(mUnk_20[i]);
        }
    }
}

void UnkStruct_02087898::Update() {
    if (mUnk_47 != 0) {
        mUnk_30 = 6;
    }
    if (mUnk_48 != 0) {
        mUnk_49 = (mUnk_49 + 1) & 1;
    }
    switch (mUnk_30) {
        case 0:
            func_02087d68(this);
            func_02087eec();
            mUnk_30++;
            break;
        case 1:
            if (mUnk_18 != NULL) {
                mUnk_30++;
            }
            break;
        case 2:
            if (mUnk_44 != 0) {
                if (mUnk_4b == 0) {
                    SoundManager::Get().func_020246f8(0x18);
                }
                mUnk_32 = 0xc;
                mUnk_30++;
            }
            break;
        case 3:
            func_02088cd8(this);
            if (mUnk_32 != 0) {
                mUnk_32--;
            } else {
                mUnk_30++;
            }
            break;
        case 4:
            func_02088a24(this);
            func_02088cd8(this);
            func_020882c4(this);
            mUnk_45 = 0;
            mUnk_30++;
            break;
        case 5:
            func_02088a24(this);
            func_02088b30(this);
            func_02088cd8(this);
            if (mUnk_45 != 0 && mUnk_4a == 0) {
                mUnk_44 = 0;
                mUnk_30 = 2;
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

// non-matching: 2 words, the original loads mUnk_20[0] (r6) before mUnk_18 (r5); the compiler loads them in the other order

extern const u8 data_020a41cc[];

extern const s8 data_020a41c8[];

// non-matching: register allocation only (original: i r6, SpriteManager init pointer r7, x r8, y r9; the compiler puts x r7, y r8 and the init pointer r9)

void func_02087ccc(UnkStruct_02087898 *self, void *data);
void func_02087d68(UnkStruct_02087898 *self);
void func_020880f4(UnkStruct_02087898 *self);
void func_0208813c(UnkStruct_02087898 *self, s32 unk1);
void func_020889d8(UnkStruct_02087898 *self, s32 unk1);
void func_02088a24(UnkStruct_02087898 *self);
void func_02088b30(UnkStruct_02087898 *self);
void func_02088cd8(UnkStruct_02087898 *self);
void func_02088ec4(UnkStruct_02087898 *self);
void func_02088f28(UnkStruct_02087898 *self, u8 unk1);
u8 func_02088f38();

BOOL UnkStruct_02087898::func_02087cac() {
    return mUnk_30 > 2;
}

void UnkStruct_02087898::func_02087cc0() {
    mUnk_47 = 1;
}

void func_02087ccc(UnkStruct_02087898 *self, void *data) {
    if (self->mUnk_18 != NULL) {
        OS_Free(self->mUnk_18);
    }
    self->mUnk_18 = data;
}

void UnkStruct_02087898::func_02087cf8(const char *path) {
    if (mUnk_18 != NULL) {
        OS_Free(mUnk_18);
    }
    ChangeDir("/");
    mUnk_18 = func_02023504(path, 0, 0, 0, 0, 0);
}

void func_02087d68(UnkStruct_02087898 *self) {
    ChangeDir("/");
    self->mUnk_1c = func_02023504("/font/mes_StoryCharaName.mes", 0, 0, 0, 0, 0);
    SpriteManager::Get().Screen(0).func_02027624(3);
    self->mUnk_28[0] = TextManager::Get().BufferSize(0x18, 8);
    self->mUnk_20[0] = OS_Alloc(self->mUnk_28[0]);
    MI_CpuClear16(self->mUnk_20[0], self->mUnk_28[0]);
    self->mUnk_28[1] = TextManager::Get().BufferSize(0x10, 4);
    self->mUnk_20[1] = OS_Alloc(self->mUnk_28[1]);
    MI_CpuClear16(self->mUnk_20[1], self->mUnk_28[1]);
    func_020880f4(self);
}

void UnkStruct_02087898::func_02087eec() {
    char path[0x80];
    ChangeDir("/story/");
    SpriteScreen &screen = SpriteManager::Get().Screen(0);
    screen.func_02027624(1);
    sprintf(path, "storytalk_ob.nce_ncg1D.bin");
    screen.LoadCharacters(path, 0x4000, 0);
    sprintf(path, "storytalk_ob_ncl.bin");
    screen.LoadPalette(path, 0x180, 0);
    for (u8 i = 0; i <= 2; i++) {
        delete mUnk_0c[i];
        mUnk_0c[i]            = new CellAnimation("storytalk_ob_nce.bin", 0, 1, 1);
        mUnk_0c[i]->mCharBase = 0x200;
        mUnk_0c[i]->mUnk_11   = 12;
    }
    mUnk_0c[0]->SetAnimation(0);
    mUnk_0c[1]->SetAnimation(1);
    mUnk_0c[2]->SetAnimation(2);
    ChangeDir("/");
    TextManager::Get().SetColor(0, 0, 0xf);
}

void func_020880f4(UnkStruct_02087898 *self) {
    MI_CpuClearFast((void *) 0x6406000, 0x1800);
    MI_CpuClear16(self->mUnk_20[0], self->mUnk_28[0]);
    self->mUnk_36 = 1;
    self->mUnk_39 = 0;
    self->mUnk_34 = -1;
}

void func_0208813c(UnkStruct_02087898 *self, s32 unk1) {
    if (self->mUnk_34 == unk1) {
        return;
    }
    func_020880f4(self);
    self->mUnk_34 = unk1;
    self->mUnk_36 = 0;
    self->mUnk_37 = 0;
    self->mUnk_38 = 0;
    if (self->mUnk_34 < 0) {
        self->mUnk_30 = 2;
        self->mUnk_44 = 0;
        return;
    }
    self->mUnk_44 = 1;
    if (self->func_02087cac()) {
        self->mUnk_30 = 4;
    }
    TextManager &text = TextManager::Get();
    s16 y;
    s16 x;
    x = 0;
    y = 0x20;
    u16 index         = self->mUnk_34;
    MessageFile *file;
    void *buffer = self->mUnk_20[0];
    file         = (MessageFile *) self->mUnk_18;
    MI_CpuClear16(data_020c1a4c, 0x20);
    text.mUnk_58 = 1;
    text.Print(file->GetText(index), file->GetLength(index), buffer, 0x20, 6, &y, &x, NULL, 0, 0, 0x21, 0x10, 0, 1, 0, 1, 1);
    for (u8 i = 1; i < 0x10; i++) {
        if (text.mPalette[i] == 0) {
            text.mPalette[i] = text.mUnk_36[i];
        }
    }
}

// non-matching: same size; in the first mUnk_39 print the text/buffer registers are swapped (r5/r6) and file + index*4 is computed twice here, once in the original.
extern char data_020d441a[];

void func_020882c4(UnkStruct_02087898 *self) {
    s16 h;
    s16 w;
    if (self->mUnk_34 < 0) {
        return;
    }
    u32 a = TextManager::Get().func_0202c1a8((u8) ((self->mUnk_38 + 1) * 3));
    u32 b = TextManager::Get().func_0202c1a8((u8) ((self->mUnk_38 + 2) * 3));
    if (a < b) {
        self->mUnk_39 = 1;
    }
    if (self->mUnk_39 != 0) {
        self->mUnk_38++;
    }
    if (self->mUnk_39 != 0) {
        TextManager &first = TextManager::Get();
        TextManager &t1 = TextManager::Get();
        TextManager &text = first;


        s16 y;
        s16 x;
        x = -(self->mUnk_37 * 0x30);
        y = 0x20;
        void *buffer      = self->mUnk_20[0];
        u16 count         = t1.func_0202c1a8((u8) ((self->mUnk_37 + 1) * 3));
        MessageFile *file = (MessageFile *) self->mUnk_18;
        u16 index         = self->mUnk_34;
        u16 len           = file->GetLength(index);
        const u8 *str     = file->GetText(index);
        u16 n             = count;
        if (len < count * 2) {
            n = len >> 1;
        }
        text.mUnk_58 = 0x10;
        text.Print(str, n * 2, buffer, 0x20, 6, &y, &x, NULL, 0, 2, 0x21, 0x10, 0, 0, 1, 1, 1);
        self->mUnk_3e = 2;
    } else {
        TextManager &text = TextManager::Get();
        s16 y;
        s16 x;
        x = -(self->mUnk_37 * 0x30);
        y = 0x20;
        MessageFile *file = (MessageFile *) self->mUnk_18;
        u16 index         = self->mUnk_34;
        text.Print(file->GetText(index), file->GetLength(index), self->mUnk_20[0], 0x20, 6, &y, &x, NULL, 0, 2, 0x21, 0x10, 0, 0, 1, 1, 1);
        s32 w = ((u16) x - 0x10) / 16;
        if (w <= 0) {
            w = 0;
        }
        self->mUnk_3e = w;
        self->mUnk_36 = 1;
    }
    func_02025c1c(SpriteManager::Get().mScreens[0].mBgIndex, 4, 0x6000, self->mUnk_20[0], 0x1800, 0);
    if (self->mUnk_37 != 0) {
        TextManager &first = TextManager::Get();
        TextManager &text = TextManager::Get();
        u16 a2            = first.func_0202c1a8((u8) ((self->mUnk_37 + 1) * 3));

        u16 b2            = text.func_0202c1a8((u8) (self->mUnk_37 * 3));
        self->mUnk_3c     = a2 - b2;
    } else {
        self->mUnk_3c = TextManager::Get().func_0202c1a8((u8) ((self->mUnk_37 + 1) * 3));
    }
    self->mUnk_3c = self->mUnk_3c * 2 / 3;
    if (self->mUnk_34 < 0 || self->mUnk_42 < 0) {
        return;
    }
    TextManager::Get().mUnk_5b = 1;
    if (self->mUnk_42 == 0x15) {
        h = 0;
        w = 0;
        TextManager::Get().Print(data_020d441a, 0x18, self->mUnk_20[1], 0x20, 2, &h, &w, NULL, 0, 2, 0x21, 0, 0, 0, 0, 1, 1);
    } else {
        TextManager &text = TextManager::Get();
        s16 y;
        s16 x;
        x = 0;
        y = 0;

        MessageFile *file = (MessageFile *) self->mUnk_1c;
        u16 index         = self->mUnk_42;
        text.Print(
file->GetText(index), file->GetLength(index), self->mUnk_20[1], 0x20, 2, &y, &x, NULL, 0, 2, 0x21, 0, 0, 0, 0, 1, 1);
    }
    TextManager::Get().mUnk_5b = 0;
    func_02025c1c(SpriteManager::Get().mScreens[0].mBgIndex, 4, 0x7800, self->mUnk_20[1], self->mUnk_28[1], 0);
}

void func_020889d8(UnkStruct_02087898 *self, s32 unk1) {
    if (unk1 == self->mUnk_42) {
        return;
    }
    self->mUnk_42 = unk1;
    MI_CpuClearFast((void *) 0x6407800, 0x800);
    MI_CpuClear16(self->mUnk_20[1], self->mUnk_28[1]);
}

void UnkStruct_02087898::func_02088a18(u32 unk1, u32 unk2) {
    mUnk_3f = unk1;
    mUnk_40 = unk2;
}

void func_02088a24(UnkStruct_02087898 *self) {
    if (self->mUnk_48 != 0 && self->mUnk_49 != 0) {
        return;
    }
    if (self->mUnk_4b != 0) {
        return;
    }
    u8 i = 0;
    do {
        SpriteManager::Get().Screen(0).func_02027778((void *) data_020a41cc, 1, (s16) (i % 8 * 0x20 + 0x12), 0x82, 0, (u16) (i * 8 + 0x3c0), 0xf, 0);
        i++;
    } while (i < 8);
}

void func_02088b30(UnkStruct_02087898 *self) {
    if (self->mUnk_48 != 0 && self->mUnk_49 != 0) {
        return;
    }
    s16 shift = -(self->mUnk_32 * self->mUnk_32) / 8;
    u8 i;
    s32 x;
    s32 y;
    if (self->mUnk_4b != 0) {
        x = self->mUnk_3a;
        y = self->mUnk_3b;
    } else {
        x = 0x14;
        y = (s16) (data_020a41c8[self->mUnk_3e] + 0x96);
    }
    i = 0;
    do {
        SpriteManager::Get().Screen(0).func_02027778((void *) data_020a41cc, 1, (s16) (x + i % 8 * 0x20 - 0x20), (s16) (y + i / 8 * 12 + shift), 0, (u16) (i * 8 + 0x300), 0xf, 0);
        i++;
    } while (i < 0x18);
    if (self->mUnk_46 == 0) {
        return;
    }
    if (self->mUnk_4b != 0) {
        return;
    }
    self->mUnk_0c[1]->Update();
    self->mUnk_0c[1]->Draw(0, 0xf0, (s16) (y + 0x1c), 0);
}

void func_02088cd8(UnkStruct_02087898 *self) {
    if (self->mUnk_48 != 0 && self->mUnk_49 != 0) {
        return;
    }
    if (self->mUnk_4b != 0) {
        return;
    }
    s16 shift = -(self->mUnk_32 * self->mUnk_32) / 8;
    if (self->mUnk_34 >= 0) {
        s16 x = self->mUnk_40;
        if ((x < 0xe8) & (x > 0x18)) {
            if (self->mUnk_3f < 3) {
                self->mUnk_0c[2]->func_0202a3ec(0, self->mUnk_3f, x, (s16) (shift + 0x60), 0);
            }
        }
    }
    self->mUnk_0c[0]->Draw(0, 0x80, (s16) (shift + 0x60), 0);
}

BOOL UnkStruct_02087898::func_02088dac(u32 unk1) {
    if (unk1 != 0) {
        SoundManager::Get().func_020246f8(0x6b);
    }
    if (mUnk_37 >= mUnk_38) {
        mUnk_45 = 1;
        return FALSE;
    }
    MI_CpuClear16(mUnk_20[0], mUnk_28[0]);
    mUnk_37++;
    mUnk_38 = mUnk_37;
    mUnk_39 = 0;
    mUnk_30 = 4;
    return TRUE;
}

void UnkStruct_02087898::func_02088e64(s32 unk1, s32 unk2, u32 unk3, u32 unk4) {
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
        mUnk_30 = 2;
    }
}

void UnkStruct_02087898::func_02088ebc(u32 unk1) {
    mUnk_46 = unk1;
}

void func_02088ec4(UnkStruct_02087898 *self) {
    self->func_02088e64(-1, -1, 0, 0);
    self->mUnk_3c = 0;
}

u32 UnkStruct_02087898::func_02088ef4() {
    return mUnk_3c;
}

BOOL UnkStruct_02087898::func_02088efc() {
    return mUnk_37 < mUnk_38;
}

BOOL UnkStruct_02087898::func_02088f14() {
    return mUnk_30 == 5;
}

void func_02088f28(UnkStruct_02087898 *self, u8 unk1) {
    self->mUnk_48 = unk1;
    self->mUnk_49 = 0;
}
