// NitroSystem sound library: loading sequences, sequence archives, banks and wave archives (singly and by group). 0x020385b8-0x02038984
#include <nnsys/private/nns_snd_arc.h>

typedef struct { u8 type; u8 flags; u16 pad; u32 id; } SndGroupEnt;   // 0 sequence, 1 bank, 2 wave archive, 3 sequence archive
typedef struct { u32 count; SndGroupEnt ent[1]; } SndGroup;
typedef struct { u32 file; u16 bank; } SndSeqInfo;      // INFO sequence record (fileId at 0, bank at 4)
typedef struct { u32 file; u16 wave[4]; } SndBankInfo;  // INFO bank record
typedef struct { u32 file:24; u32 split:8; } SndFileInfo;   // file id; split bit 0 = wave data is stored per entry

extern u32 func_02038a24(u32 id, void *heap, int flag);
extern u32 func_02038a90(u32 id, void *heap, int flag);
extern u32 LoadBankFile(u32 id, void *heap, int flag);
extern u32 LoadSplitWaveArcHeaderAndTable(u32 id, void *heap, int flag);
extern u32 LoadWaveArcFile(u32 id, void *heap, int flag);
extern int LoadBankWaveArcEntries(u32 wave, u32 bank, int idx, u32 file, void *heap);
extern void LinkBankToWaveArc(u32 bank, int idx, u32 wave);
int LoadSeqAndBank(int id, u32 flags, void *heap, int flag, u32 *out);
int func_02038758(int id, u32 flags, void *heap, int flag, u32 *out);
int LoadBankAndWaveArcs(int id, u32 flags, void *heap, int flag, u32 *out);
int LoadWaveArc(int id, u32 flags, void *heap, int flag, u32 *out);
int LoadGroupEntries(int group, void *heap);

// 1 when every item loaded (LoadGroupEntries returns 0 then): the opposite polarity of LoadGroupEntries.
int LoadGroup(int group, void *heap) {
    return LoadGroupEntries(group, heap) == 0 ? 1 : 0;
}

int LoadGroupEntries(int group, void *heap) {   // load every item of a group: 0 when all loaded, else the error
    SndGroup *g = GetGroupInfo(group);
    u32 i;
    SndGroupEnt *e;
    int r;
    if (g == 0) return 1;
    for (i = 0; i < g->count; i++) {
        e = &g->ent[i];
        switch (e->type) {
        case 0:
            r = LoadSeqAndBank(e->id, e->flags, heap, 1, 0);
            if (r != 0) return r;
            break;
        case 3:
            r = func_02038758(e->id, e->flags, heap, 1, 0);
            if (r != 0) return r;
            break;
        case 1:
            r = LoadBankAndWaveArcs(e->id, e->flags, heap, 1, 0);
            if (r != 0) return r;
            break;
        case 2:
            r = LoadWaveArc(e->id, e->flags, heap, 1, 0);
            if (r != 0) return r;
            break;
        }
    }
    return 0;
}

int LoadSeqAndBank(int id, u32 flags, void *heap, int flag, u32 *out) {   // load a sequence and its bank
    SndSeqInfo *info = GetSeqInfo(id);
    u32 seq;
    int r;
    if (info == 0) return 2;
    r = LoadBankAndWaveArcs(info->bank, flags, heap, flag, 0);
    if (r != 0) return r;
    if ((flags & 1) != 0) {
        seq = func_02038a24(info->file, heap, flag);
        if (seq == 0) return 6;
    } else {
        seq = GetFatEntryCache(info->file);
    }
    if (out != 0) *out = seq;
    return 0;
}

int func_02038758(int id, u32 flags, void *heap, int flag, u32 *out) {   // load a sequence archive
    SndSeqInfo *info = func_02037ddc(id);
    u32 seq;
    if (info == 0) return 3;
    if ((flags & 8) != 0) {
        seq = func_02038a90(info->file, heap, flag);
        if (seq == 0) return 7;
    } else {
        seq = GetFatEntryCache(info->file);
    }
    if (out != 0) *out = seq;
    return 0;
}

int LoadBankAndWaveArcs(int id, u32 flags, void *heap, int flag, u32 *out) {   // load a bank and its wave archives
    SndBankInfo *bank = GetBankInfo(id);
    SndFileInfo *wave;
    u32 file, tmp;
    int i, r;
    u32 loadWave;
    if (bank == 0) return 4;
    if ((flags & 2) != 0) {
        file = LoadBankFile(bank->file, heap, flag);
        if (file == 0) return 8;
    } else {
        file = GetFatEntryCache(bank->file);
    }
    loadWave = flags & 4;
    for (i = 0; i < 4; i++) {
        if (bank->wave[i] == 0xffff) continue;
        wave = GetWaveArcInfo(bank->wave[i]);
        if (wave == 0) return 5;
        r = LoadWaveArc(bank->wave[i], flags, heap, flag, &tmp);
        if (r != 0) return r;
        if (((wave->split & 1)) != 0 && loadWave != 0) {
            if (LoadBankWaveArcEntries(tmp, file, i, wave->file, heap) == 0) return 9;
        }
        if (file != 0 && tmp != 0) LinkBankToWaveArc(file, i, tmp);
    }
    if (out != 0) *out = file;
    return 0;
}

int LoadWaveArc(int id, u32 flags, void *heap, int flag, u32 *out) {   // load a wave archive
    SndFileInfo *wave = GetWaveArcInfo(id);
    u32 w;
    if (wave == 0) return 5;
    if ((flags & 4) != 0) {
        if (((wave->split & 1)) != 0) {
            w = LoadSplitWaveArcHeaderAndTable(wave->file, heap, flag);
        } else {
            w = LoadWaveArcFile(wave->file, heap, flag);
        }
        if (w == 0) return 9;
    } else {
        w = GetFatEntryCache(wave->file);
    }
    if (out != 0) *out = w;
    return 0;
}
