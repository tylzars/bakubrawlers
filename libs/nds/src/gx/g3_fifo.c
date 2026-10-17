#include <nitro/internal.h>

#define REG_GXFIFO ((vu32 *) 0x04000400)

extern u32 *data_01ffd1a0; // current command buffer: [0] = word count, then the words
extern volatile s32 data_01ffd1a4; // nonzero while a DMA to the GX FIFO is running
extern s32 data_01ffd1a8; // 0: use the first DMA helper, nonzero: the second

void WaitFifoBlockSend(void);
void FifoBlockSendDoneCallback(void *arg);
void MI_CpuSend32(const void *src, volatile void *dest, u32 size);
void SendFifoCommand(u32 cmd, const void *params, u32 count);
void MI_SendGXCommandAsyncFast(s32 ch, const void *src, u32 size, void (*cb)(void *), void *arg);
void MI_SendGXCommandAsync(s32 ch, const void *src, u32 size, void (*cb)(void *), void *arg);

void SetFifoCommandBufferIfNone(u32 *buf) {
    if (data_01ffd1a0 == 0) {
        *buf = 0;
        data_01ffd1a0 = buf;
    }
}

void FlushFifoCommandBuffer(void) {
    u32 n;
    if (data_01ffd1a4 != 0) {
        WaitFifoBlockSend();
    }
    if (data_01ffd1a0 == 0 || (n = *data_01ffd1a0) == 0) {
        return;
    }
    MI_CpuSend32(data_01ffd1a0 + 1, REG_GXFIFO, n << 2);
    *data_01ffd1a0 = 0;
}

void WaitFifoBlockSend(void) {
    while (data_01ffd1a4 != 0) {
    }
}

void FifoBlockSendDoneCallback(void *arg) {
    *(s32 *) arg = 0;
}

void SendFifoCommandBlock(const u32 *src, u32 size) {
    if (size < 0x100 || data_020a4f24 == -1) {
        SendFifoCommand(src[0], src + 1, (size >> 2) - 1);
        return;
    }
    FlushFifoCommandBuffer();
    data_01ffd1a4 = 1;
    if (data_01ffd1a8 != 0) {
        MI_SendGXCommandAsyncFast(data_020a4f24, src, size, FifoBlockSendDoneCallback, (void *) &data_01ffd1a4);
    } else {
        MI_SendGXCommandAsync(data_020a4f24, src, size, FifoBlockSendDoneCallback, (void *) &data_01ffd1a4);
    }
}

void SendFifoCommand(u32 cmd, const void *params, u32 count) {
    u32 *buf = data_01ffd1a0;
    if (buf != 0) {
        if (data_01ffd1a4 != 0) {
            u32 n = buf[0];
            if (n + 1 + count <= 0xc0) {
                buf[0] = n + 1;
                (data_01ffd1a0 + 1)[n] = cmd;
                if (count == 0) {
                    return;
                }
                MIi_CpuCopyFast(params, data_01ffd1a0 + 1 + data_01ffd1a0[0], count << 2);
                data_01ffd1a0[0] += count;
                return;
            }
        }
        if (buf[0] != 0) {
            FlushFifoCommandBuffer();
        } else if (data_01ffd1a4 != 0) {
            WaitFifoBlockSend();
        }
    } else if (data_01ffd1a4 != 0) {
        WaitFifoBlockSend();
    }
    *REG_GXFIFO = cmd;
    MI_CpuSend32(params, REG_GXFIFO, count << 2);
}
