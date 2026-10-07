#pragma once

#include "global.h"
#include "types.h"

// Frame clock singleton (instance 0x020c1884, destructor 0x020219c0); mFrame is the running frame count.
class UnkStruct_020c1884 {
public:
    /* 00 */ PAD(0x00, 0x10);
    /* 10 */ u32 mFrame;
    /* 14 */ u8 mUnk_14;
    /* 15 */ PAD(0x15, 0x18);
    /* 18 */

    ~UnkStruct_020c1884();

    static UnkStruct_020c1884 &Get() {
        static UnkStruct_020c1884 instance;
        return instance;
    }
    const char *func_0202030c(); // language suffix of file names
};
