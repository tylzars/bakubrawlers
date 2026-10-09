#include <nitro/private/os_types.h>

extern void MI_CpuCopy8(const void *src, void *dest, u32 size);       // 0x01ff96c4 (ITCM)
extern void func_01ff94e0(const void *src, void *dest, u32 size);    // ITCM

typedef struct {
    u8 favoriteColor;      // 0x00
    u8 birthMonth;         // 0x01
    u8 birthDay;           // 0x02
    u8 unk03;              // 0x03
    u16 nickName[10];      // 0x04
    u16 nickNameEnd;       // 0x18
    u16 nickNameLength;    // 0x1a
    u16 comment[26];       // 0x1c
    u16 commentEnd;        // 0x50
    u16 commentLength;     // 0x52
} OSOwnerInfo;

void OS_GetMacAddress(u8 *macAddress) {
    MI_CpuCopy8((const void *) 0x27ffcf4, macAddress, 6);
}

void OS_GetOwnerInfo(OSOwnerInfo *info) {
    vu8 *src = (vu8 *) 0x27ffc80;
    info->favoriteColor = ((u32) *(vu16 *) (src + 0x64) << 29) >> 29;
    info->birthMonth = ((u32) src[2] << 28) >> 28;
    info->birthDay = src[3];
    info->unk03 = src[4];
    info->nickNameLength = src[0x1a];
    info->commentLength = src[0x50];
    func_01ff94e0((const void *) (src + 6), info->nickName, 0x14);
    func_01ff94e0((const void *) (src + 0x1c), info->comment, 0x34);
    info->nickNameEnd = 0;
    info->commentEnd = 0;
}
