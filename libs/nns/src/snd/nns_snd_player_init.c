// NitroSystem sound library: sound player setup from the archive and sound start helpers. 0x02038fac-0x0203913c
#include <nnsys/private/nns_snd_arc.h>

typedef struct { u8 maxSounds; u8 pad1; u16 channels; u32 heapSize; } SndPlayerInfo;   // INFO player record
// INFO sequence record as used here: bank at 4; StartSeqSound gets byte 9 as the player and byte 8 as the priority
// (both for AllocHandleSound) and passes byte 6 to SetHandleVolA and byte 7 to func_02036b68.
typedef struct { u32 file; u16 bank; u8 volA; u8 b7; u8 prio; u8 player; } SndSeqRec;

extern void SetPlayerMaxSounds(int player, int maxSounds);
extern void func_02036938(int player, int channels);
extern int CreatePlayerHeap(int player, SndHeap *heap, u32 size);
extern void *func_0203a460(u32 seq, u32 id);
extern int StartSeqSound(int a, int b, int c, int d, void *rec, int id);
extern int func_02039238(int a, int b, int c, int d, void *rec, u32 seq, int e, int f);

int InitPlayersFromArc(SndHeap *heap) {   // set up the sound players from the archive
    int i;
    SndPlayerInfo *p;
    int j;
    GetCurrentSndArc();
    for (i = 0; i < 32; i++) {
        p = GetPlayerInfo(i);
        if (p == 0) continue;
        SetPlayerMaxSounds(i, p->maxSounds);
        func_02036938(i, p->channels);
        if (p->heapSize != 0 && heap != 0) {
            for (j = 0; j < p->maxSounds; j++) {
                if (CreatePlayerHeap(i, heap, p->heapSize) == 0) return 0;
            }
        }
    }
    return 1;
}

int StartSeqFromInfo(int a, int id) {   // start sequence `id` with its default parameters
    u8 *rec = GetSeqInfo(id);
    if (rec == 0) return 0;
    return StartSeqSound(a, rec[9], *(u16 *)(rec + 4), rec[8], rec, id);
}

int func_02039090(int a, int b, int c, int d, int e, int f) {   // start a sequence of a sequence archive; negative parameters use the defaults
    u32 *arc = func_02037ddc(e);
    u32 seq;
    u8 *rec;
    if (arc == 0) return 0;
    seq = GetFatEntryCache(arc[0]);
    if (seq == 0) return 0;
    rec = func_0203a460(seq, f);
    if (rec == 0) return 0;
    if (d < 0) d = rec[8];
    if (c < 0) c = *(u16 *)(rec + 4);
    if (b < 0) b = rec[9];
    return func_02039238(a, b, c, d, rec, seq, e, f);
}
