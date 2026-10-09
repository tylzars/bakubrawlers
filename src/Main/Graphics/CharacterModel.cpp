#include "Graphics/CharacterModel.hpp"
#include "System/FileSystem.hpp"
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

// the original passes a pointer here (the declaration in FileSystem.hpp takes u32)
const char *data_020b24e4[6] = {"head", "up", "down", "hair", "glasse", "cap"}; // part names, .data 0x020b24bc-0x020b24fc
extern MtxFx43 data_0220e86c;
extern MtxFx43 data_022191d4;
extern "C" void func_02031838(void *renderObj, void (*callback)(NNSG3dRS *));
extern "C" MtxFx43 *func_020323b8();
extern "C" void func_01ff89c0(MtxFx43 *a, MtxFx43 *b, MtxFx43 *out);
void func_01ff96c4(const void *src, void *dst, u32 size);
void func_02053fc0(NNSG3dRS *rs);
void func_02053fd8(NNSG3dRS *rs);

CharacterModel::CharacterModel() {
    mUnk_60 = NULL;
    mUnk_64 = 0;
    mUnk_76 = 0;
    mUnk_77 = 0;
    for (u8 i = 0; i < 6; i++) {
        mParts[i] = NULL;
    }
    mUnk_40 = -1;
    mUnk_41 = -1;
    mUnk_44 = 2;
    MI_CpuFill8(mUnk_38, 0, 6);
    mUnk_2c = NULL;
    mUnk_30 = NULL;
    mUnk_34 = NULL;
}

CharacterModel::~CharacterModel() {
    if (mParts[0] != NULL) {
        func_02031824((u8 *) mParts[0] + 4);
    }
    func_020531fc();
    if (mUnk_2c != NULL) {
        OS_Free(mUnk_2c);
    }
    if (mUnk_30 != NULL) {
        OS_Free(mUnk_30);
    }
    if (mUnk_34 != NULL) {
        OS_Free(mUnk_34);
    }
}

void CharacterModel::func_02052894(u32 a, u32 b) {
    if (mParts[0] == NULL) {
        return;
    }
    mParts[0]->func_02028e54(a, b, 0x1000);
}

void CharacterModel::func_020528b0(u32 part, u32 sex) {
    char buf[0x20];
    switch (part) {
        case 0: {
            func_02052da4(sex);
            u32 async = 0;
            while (!func_02052e50(async)) {
            }
            func_020530e8();
            break;
        }
        case 1: {
            mUnk_38[1] = mUnk_6f;
            ChangeDir("/Game/Character/EditedCharacter/Male/Up");
            sprintf(buf, "edit_%s_M%02d.nsbtx", data_020b24e4[1], mUnk_38[1]);
            UnkStruct_02028308 *texture = new NitroTexture(buf);
            sprintf(buf, "edit_%s_M%02d.nsbmd", data_020b24e4[1], mUnk_38[1]);
            NitroModel *model = new NitroModel(buf, texture);
            mParts[1]         = model->func_02028b78(NULL);
            func_0205340c(1);
            texture->func_02028344();
            if (mUnk_60 != NULL) {
                mParts[1]->mUnk_38 = mUnk_60;
            }
            mParts[1]->mUnk_69 = 0;
            break;
        }
        case 2: {
            mUnk_38[2] = mUnk_71 * 3 + mUnk_73;
            ChangeDir("/Game/Character/EditedCharacter/Male/Down");
            sprintf(buf, "edit_%s_M%02d.nsbtx", data_020b24e4[2], mUnk_38[2]);
            UnkStruct_02028308 *texture = new NitroTexture(buf);
            sprintf(buf, "edit_%s_M%02d.nsbmd", data_020b24e4[2], mUnk_38[2]);
            NitroModel *model = new NitroModel(buf, texture);
            mParts[2]         = model->func_02028b78(NULL);
            func_0205340c(2);
            texture->func_02028344();
            if (mUnk_60 != NULL) {
                mParts[2]->mUnk_38 = mUnk_60;
            }
            mParts[2]->mUnk_69 = 0;
            break;
        }
        case 3: {
            mUnk_38[3] = mUnk_68;
            ChangeDir("/Game/Character/EditedCharacter/Male/Hair");
            sprintf(buf, "edit_%s_M%02d.nsbtx", data_020b24e4[3], mUnk_38[3]);
            UnkStruct_02028308 *texture = new NitroTexture(buf);
            sprintf(buf, "edit_%s_M%02d.nsbmd", data_020b24e4[3], mUnk_38[3]);
            NitroModel *model = new NitroModel(buf, texture);
            mParts[3]         = model->func_02028b78(NULL);
            func_0205340c(3);
            texture->func_02028344();
            mParts[3]->mUnk_69 = 0;
            break;
        }
        case 4: {
            mUnk_38[4] = mUnk_6c;
            if (mUnk_38[4] != 0) {
                ChangeDir("/Game/Character/EditedCharacter/Male/Glasse");
                sprintf(buf, "edit_%s_M%02d.nsbtx", data_020b24e4[4], mUnk_38[4] - 1);
                UnkStruct_02028308 *texture = new NitroTexture(buf);
                sprintf(buf, "edit_%s_M%02d.nsbmd", data_020b24e4[4], mUnk_38[4] - 1);
                NitroModel *model = new NitroModel(buf, texture);
                mParts[4]         = model->func_02028b78(NULL);
                func_0205340c(4);
                texture->func_02028344();
                mParts[4]->mUnk_69 = 0;
            }
            break;
        }
        case 5: {
            mUnk_38[5] = mUnk_6a;
            if (mUnk_38[5] != 0) {
                ChangeDir("/Game/Character/EditedCharacter/Male/Cap");
                sprintf(buf, "edit_%s_M%02d.nsbtx", data_020b24e4[5], mUnk_38[5] - 1);
                UnkStruct_02028308 *texture = new NitroTexture(buf);
                sprintf(buf, "edit_%s_M%02d.nsbmd", data_020b24e4[5], mUnk_38[5] - 1);
                NitroModel *model = new NitroModel(buf, texture);
                mParts[5]         = model->func_02028b78(NULL);
                func_0205340c(5);
                texture->func_02028344();
                mParts[5]->mUnk_69 = 0;
            }
            break;
        }
    }
    ChangeDir("/");
}

void CharacterModel::func_02052da4(u32 sex) {
    mUnk_78 = sex;
    mUnk_79 = 0;
    mUnk_7a = 0;
}

BOOL CharacterModel::func_02052db8() {
    BOOL result = FALSE;
    if (mUnk_7a == 0) {
        if (func_02052e50(1)) {
            mUnk_79 = 0;
            result  = TRUE;
        }
    }
    return result;
}

void CharacterModel::func_02052df0() {
    func_020530e8();
    func_020528b0(1, mUnk_78);
    func_020528b0(2, mUnk_78);
    func_020528b0(3, mUnk_78);
    func_020528b0(4, mUnk_78);
    func_020528b0(5, mUnk_78);
}

BOOL CharacterModel::func_02052e50(u32 async) {
    char buf[0x20];
    BOOL result = FALSE;
    switch (mUnk_79) {
        case 0:
            mUnk_38[0] = 0;
            ChangeDir("/Game/Character/EditedCharacter/Male/Head");
            sprintf(buf, "edit_%s_M%02d.nsbtx", data_020b24e4[0], mUnk_38[0]);
            if (async) {
                func_02023708(buf, 0, 0, 0, 0, 0);
            } else {
                mUnk_2c = func_02023504(buf, 0, 0, 0, 0, 0);
            }
            ChangeDir("/");
            mUnk_79++;
            break;
        case 1:
            if (func_02023874() == 0) {
                ChangeDir("/Game/Character/EditedCharacter/Male/Head");
                sprintf(buf, "edit_%s_M%02d.nsbmd", data_020b24e4[0], mUnk_38[0]);
                if (async) {
                    mUnk_2c = func_02023864();
                    func_02023708(buf, 0, 0, 0, 0, 0);
                } else {
                    mUnk_30 = func_02023504(buf, 0, 0, 0, 0, 0);
                }
                ChangeDir("/");
                mUnk_79++;
            }
            break;
        case 2:
            if (func_02023874() == 0) {
                ChangeDir("/Game/Character/EditedCharacter/Anm");
                if (mUnk_78 == 0) {
                    sprintf(buf, "edit_motion_m_cut_data.bin");
                } else {
                    sprintf(buf, "edit_motion_m_data.bin");
                }
                if (async) {
                    mUnk_30 = func_02023864();
                    func_02023708(buf, 0, 0, 0, 0, 0);
                } else {
                    mUnk_34 = func_02023504(buf, 0, 0, 0, 0, 0);
                    result  = TRUE;
                }
                ChangeDir("/");
                mUnk_79++;
            }
            break;
        case 3:
            if (func_02023874() == 0) {
                mUnk_34 = func_02023864();
                result  = TRUE;
            }
            break;
    }
    return result;
}

void CharacterModel::func_020530e8() {
    UnkStruct_02028308 *texture = new NitroTextureData(mUnk_2c);
    NitroModel *model           = new NitroModelData(mUnk_30, texture);
    ModelAnimData *animData     = new ModelAnimData(mUnk_34);
    ModelAnim *anim             = new ModelAnim(animData, model);
    mParts[0]                   = model->func_02028b78(anim);
    func_0205340c(0);
    texture->func_02028344();
    OS_Free(mUnk_2c);
    mUnk_2c = NULL;
    if (mUnk_60 == NULL) {
        s16 count = *(u8 *) (*(u8 **) ((u8 *) mParts[0] + 8) + 0x17);
        mUnk_60   = OS_Alloc(count * 0x58);
    }
    if (mUnk_60 != NULL) {
        mParts[0]->mUnk_38 = mUnk_60;
    }
    mParts[0]->mUnk_69 = 0;
}


void CharacterModel::func_020531fc() {
    for (u8 i = 0; i < 6; i++) {
        ModelInstance *inst = mParts[i];
        if (inst != NULL) {
            if (inst->mUnk_3c == 0) {
                *(u32 *) ((u8 *) inst + 4) &= ~1;
            }
            inst->mUnk_38 = NULL;
            inst = mParts[i];
            inst->mUnk_68 = 0xf;
            delete inst;
            mParts[i] = NULL;
        }
    }
    if (mUnk_60 != NULL) {
        OS_Free(mUnk_60);
        mUnk_60 = NULL;
    }
}

void CharacterModel::func_02053298() {
    MtxFx43 mtx = data_0220e86c;
    if (mParts[0] != NULL) {
        mParts[0]->func_02028d80();
    }
    if (mUnk_64 == 0) {
        return;
    }
    func_01ffcf64(&mtx, &mPosition);
    func_01ffd088(&mtx, mRotX);
    func_01ffd0e0(&mtx, mRotY);
    func_01ffd138(&mtx, mRotZ);
    if (mParts[0] != NULL) {
        func_02031838((u8 *) mParts[0] + 4, func_02053fc0);
        *(u32 *) ((u8 *) mParts[0] + 4) |= 1;
        mParts[0]->func_02029174(&mtx, &mScale);
        if (mUnk_77 != 0) {
            mParts[0]->func_02028da4();
        }
    }
    if (mParts[1] != NULL) {
        mParts[1]->func_02029174(&mtx, &mScale);
    }
    if (mParts[2] != NULL) {
        mParts[2]->func_02029174(&mtx, &mScale);
    }
    func_01ffd0e0(&data_022191d4, 0x8000);
    func_01ffd138(&data_022191d4, 0x4000);
    if (mParts[5] != NULL) {
        mParts[5]->func_02029174(&data_022191d4, &mScale);
    } else if (mParts[3] != NULL) {
        mParts[3]->func_02029174(&data_022191d4, &mScale);
    }
    if (mParts[4] != NULL) {
        mParts[4]->func_02029174(&data_022191d4, &mScale);
    }
}

void CharacterModel::func_0205340c(u32 part) {
    char bufA[0x7f];
    char bufB[0x7f];
    char bufC[0x7f];
    char bufD[0x7f];
    switch (part) {
        case 0: {
            ChangeDir("/Game/Character/EditedCharacter/Male/Tex/Skin");
            sprintf(bufA, "Skin_00");
            sprintf(bufB, "Skin_00_%03d", mUnk_65);
            func_02053d9c(part, bufA, bufB);
            ChangeDir("/Game/Character/EditedCharacter/Male/Tex/Mouth");
            sprintf(bufA, "Mouth_00");
            sprintf(bufB, "Mouth_00_%03d", mUnk_65);
            func_02053d9c(part, bufA, bufB);
            ChangeDir("/Game/Character/EditedCharacter/Male/Tex/Face");
            sprintf(bufA, "Face_00");
            sprintf(bufB, "Face_%02d_%03d", mUnk_66, mUnk_67);
            sprintf(bufC, "Face_%02d_%03d", mUnk_66, mUnk_65);
            u8 table[4] = {3, 2, 2, 3};
            u8 offset = table[mUnk_66];
            func_02053d74(part, bufA, bufB);
            func_02053dc4(part, bufA, bufB, bufC, offset);
            break;
        }
        case 1: {
            ChangeDir("/Game/Character/EditedCharacter/Male/Tex/Skin");
            sprintf(bufA, "Skin_00");
            sprintf(bufB, "Skin_00_%03d", mUnk_65);
            func_02053d9c(part, bufA, bufB);
            ChangeDir("/Game/Character/EditedCharacter/Male/Tex/Mobile");
            sprintf(bufA, "Mobile_00");
            sprintf(bufB, "Mobile_00_%03d", mUnk_75);
            func_02053d9c(part, bufA, bufB);
            ChangeDir("/Game/Character/EditedCharacter/Male/Tex/Gloves");
            sprintf(bufA, "Gloves_00");
            sprintf(bufB, "Gloves_00_%03d", mUnk_75);
            func_02053d9c(part, bufA, bufB);
            ChangeDir("/Game/Character/EditedCharacter/Male/Tex/Top");
            sprintf(bufA, "Top_%02d", mUnk_6f);
            sprintf(bufB, "Top_%02d_%03d", mUnk_6f, mUnk_75);
            sprintf(bufC, "Top_%02d_%03d", mUnk_6f, mUnk_70);
            u8 table[6] = {4, 8, 8, 7, 6, 0xe};
            u8 offset = table[mUnk_6f];
            func_02053d74(part, bufA, bufB);
            func_02053dc4(part, bufA, bufB, bufC, offset);
            break;
        }
        case 2: {
            if (mUnk_71 == 0) {
                ChangeDir("/Game/Character/EditedCharacter/Male/Tex/Item");
                sprintf(bufA, "item_00");
                sprintf(bufB, "item_00_%03d", mUnk_75);
                sprintf(bufC, "item_00_%03d", mUnk_72);
                func_02053dc4(part, bufA, bufB, bufC, 9);
            }
            if (mUnk_71 == 1 || mUnk_71 == 5) {
                ChangeDir("/Game/Character/EditedCharacter/Male/Tex/Skin");
                sprintf(bufA, "Skin_00");
                sprintf(bufB, "Skin_00_%03d", mUnk_65);
                func_02053d9c(part, bufA, bufB);
            }
            ChangeDir("/Game/Character/EditedCharacter/Male/Tex/Shoes");
            sprintf(bufA, "Shoes_%02d", mUnk_73);
            sprintf(bufB, "Shoes_%02d_%03d", mUnk_73, mUnk_74);
            func_02053d74(part, bufA, bufB);
            func_02053d9c(part, bufA, bufB);
            ChangeDir("/Game/Character/EditedCharacter/Male/Tex/Down");
            sprintf(bufA, "Down_%02d", mUnk_71);
            sprintf(bufB, "Down_%02d_%03d", mUnk_71, mUnk_75);
            sprintf(bufC, "Down_%02d_%03d", mUnk_71, mUnk_72);
            u8 table[6] = {0, 4, 5, 4, 2, 0xe};
            u8 offset = table[mUnk_71];
            func_02053d74(part, bufA, bufB);
            func_02053dc4(part, bufA, bufB, bufC, offset);
            break;
        }
        case 3: {
            ChangeDir("/Game/Character/EditedCharacter/Male/Tex/Hair");
            sprintf(bufA, "Hair_%02d", mUnk_68);
            sprintf(bufB, "Hair_%02d_%03d", 0, mUnk_69);
            func_02053d9c(part, bufA, bufB);
            break;
        }
        case 4: {
            ChangeDir("/Game/Character/EditedCharacter/Male/Tex/Glasse");
            sprintf(bufA, "Glasse_%02d", mUnk_6c - 1);
            if (mUnk_6e == 0) {
                sprintf(bufD, "Glasse_%02d_%03d", mUnk_6c - 1, 0);
                sprintf(bufB, "Glasse_%02d_%03d", mUnk_6c - 1, 0);
                sprintf(bufC, "Glasse_%02d_%03d", mUnk_6c - 1, mUnk_6d);
            } else {
                sprintf(bufD, "Glasse_%02d_%03d", mUnk_6c - 1, 1);
                sprintf(bufB, "Glasse_%02d_%03d", mUnk_6c - 1, mUnk_6e - 1);
                sprintf(bufC, "Glasse_%02d_%03d", mUnk_6c - 1, mUnk_6d);
            }
            u8 table[4] = {0xf, 0xf, 0xf, 0xf};
            u8 offset = table[mUnk_6c - 1];
            func_02053d74(part, bufA, bufD);
            func_02053dc4(part, bufA, bufB, bufC, offset);
            break;
        }
        case 5: {
            ChangeDir("/Game/Character/EditedCharacter/Male/Tex/Cap");
            sprintf(bufA, "Cap_%02d", mUnk_6a - 1);
            sprintf(bufB, "Cap_%02d_%03d", mUnk_6a - 1, mUnk_69);
            sprintf(bufC, "Cap_%02d_%03d", mUnk_6a - 1, mUnk_6b);
            u8 table[5] = {3, 5, 0xc, 5, 0xc};
            u8 offset = table[mUnk_6a - 1];
            func_02053d74(part, bufA, bufB);
            func_02053dc4(part, bufA, bufB, bufC, offset);
            break;
        }
    }
    ChangeDir("/");
}

BOOL CharacterModel::func_02053d74(u32 part, const char *name, const char *file) {
    return mParts[part]->mUnk_58->mUnk_08->func_020286fc(name, file, 0);
}

BOOL CharacterModel::func_02053d9c(u32 part, const char *name, const char *file) {
    return mParts[part]->mUnk_58->mUnk_08->func_02028858(name, file, 0);
}

void CharacterModel::func_02053dc4(u32 part, const char *name, const char *file1, const char *file2, u8 offset) {
    UnkStruct_02028308 *texture = mParts[part]->mUnk_58->mUnk_08;
    char buf1[0x20];
    char buf2[0x20];
    u32 size;
    u8 *data2;
    u8 *data1;
    sprintf(buf1, "%s.ntfp", file1);
    data1 = (u8 *) func_02023504(buf1, 0, 0, 0, &size, 0);
    sprintf(buf2, "%s.ntfp", file2);
    data2 = (u8 *) func_02023504(buf2, 0, 0, 0, &size, 0);
    func_01ff96c4(data2 + offset * 2, data1 + offset * 2, size - offset * 2);
    texture->func_020288d4(name, data1, size);
    OS_Free(data1);
    OS_Free(data2);
}

void CharacterModel::func_02053ea0(u32 a, u32 b) {
    if (mParts[0] == NULL) {
        return;
    }
    mParts[0]->func_02028e54(a, 1, 0x1000);
    mParts[0]->func_02028e54(b, 1, 0x1000);
    mUnk_76 = a;
    mUnk_77 = 0;
}

// non-matching: 0x90 vs 0x98, the original keeps this in r4 and does not hoist the branch.
void CharacterModel::func_02053eec(u32 flag) {
    if (mParts[0] == NULL) {
        return;
    }
    mUnk_77 = flag;
    if (!(flag & 0xff)) {
        ModelAnim *anim;
        u32 loop;
        u8 channel;
        NNSG3dAnmObj *obj;
        s32 frame;
        anim    = mParts[0]->mAnim;
        channel = anim->mUnk_5d;
        loop    = anim->mUnk_08;
        obj     = anim->func_02029a48(0);
        if (obj == NULL) {
            frame = 0;
        } else {
            frame = obj->mFrame;
        }
        mParts[0]->func_02028e54(channel, loop, 0x1000);
        mParts[0]->mAnim->func_020298c0(frame, 0);
    } else {
        NNSG3dAnmObj *obj = mParts[0]->mAnim->func_02029a58(4, mUnk_76);
        mParts[0]->func_02028f54(obj);
    }
}

s32 CharacterModel::func_02053f84() {
    NNSG3dAnmObj *obj = mParts[0]->mAnim->func_02029a48(0);
    if (obj == NULL) {
        return 0;
    }
    return obj->mFrame;
}

void CharacterModel::func_02053fa8(u32 frame) {
    mParts[0]->mAnim->func_020298c0(frame, 0);
}

void func_02053fc0(NNSG3dRS *rs) {
    rs->mCallback = func_02053fd8;
    rs->mUnk_92   = 3;
}

void func_02053fd8(NNSG3dRS *rs) {
    int id;
    if (rs->mFlags & 0x10) {
        id = rs->mUnk_ae;
    } else {
        id = -1;
    }
    if (id != 0x10) {
        return;
    }
    func_02032cc8(&data_022191d4, NULL);
    MtxFx43 *cam = func_020323b8();
    func_01ff89c0(&data_022191d4, cam, &data_022191d4);
}
