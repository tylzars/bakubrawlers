#pragma once

#include "types.h"
#include "nitro/hw/ioreg.h"

// Display and 2D engine controls (NitroSDK GX/G2 API). Main-engine functions use the G2_/GX_ prefix, sub-engine
// functions G2S_/GXS_. Bodies are written to reproduce the original's codegen exactly.

#define GX_POWCNT_DSEL_SHIFT 15
#define GX_DISPCNT_PLANE_MASK 0x1f00
#define GX_DISPCNT_PLANE_SHIFT 8
#define GX_DISPCNT_WND_MASK (0x2000 | 0x4000 | 0x8000)
#define GX_DISPCNT_WND_SHIFT 13
#define G2_BGCNT_KEEP_MASK (0x3 | 0x40) // priority and mosaic survive a control write
#define G2_BGCNT_PRIORITY_MASK 0x3
#define G2_BGOFS_H_MASK 0x1ff
#define G2_BGOFS_V_MASK 0x1ff0000
#define G2_WINOUT_PLANE_MASK 0x3f
#define G2_WINOUT_EFFECT 0x20

static inline void GX_SetDispSelect(int sel) {
    reg_GX_POWCNT = (u16) ((reg_GX_POWCNT & ~0x8000) | (sel << GX_POWCNT_DSEL_SHIFT));
}

static inline void GX_SetVisiblePlane(int plane) {
    reg_GX_DISPCNT = (u32) ((reg_GX_DISPCNT & ~GX_DISPCNT_PLANE_MASK) | (plane << GX_DISPCNT_PLANE_SHIFT));
}
static inline void GXS_SetVisiblePlane(int plane) {
    reg_GXS_DB_DISPCNT = (u32) ((reg_GXS_DB_DISPCNT & ~GX_DISPCNT_PLANE_MASK) | (plane << GX_DISPCNT_PLANE_SHIFT));
}

static inline void GX_SetVisibleWnd(int window) {
    reg_GX_DISPCNT = (u32) ((reg_GX_DISPCNT & ~GX_DISPCNT_WND_MASK) | (window << GX_DISPCNT_WND_SHIFT));
}
static inline void GXS_SetVisibleWnd(int window) {
    reg_GXS_DB_DISPCNT = (u32) ((reg_GXS_DB_DISPCNT & ~GX_DISPCNT_WND_MASK) | (window << GX_DISPCNT_WND_SHIFT));
}

#define G2_BG_CONTROL_VALUE(reg, size, color, screenBase, charBase, extPltt)                                          \
    (u16) (((reg) & G2_BGCNT_KEEP_MASK) | ((size) << 14) | ((color) << 7) | ((screenBase) << 8) | ((charBase) << 2) | \
           ((extPltt) << 13))

static inline void G2_SetBG0Control(int size, int color, int screenBase, int charBase, int extPltt) {
    reg_G2_BG0CNT = G2_BG_CONTROL_VALUE(reg_G2_BG0CNT, size, color, screenBase, charBase, extPltt);
}
static inline void G2S_SetBG0Control(int size, int color, int screenBase, int charBase, int extPltt) {
    reg_G2S_DB_BG0CNT = G2_BG_CONTROL_VALUE(reg_G2S_DB_BG0CNT, size, color, screenBase, charBase, extPltt);
}
static inline void G2S_SetBG1Control(int size, int color, int screenBase, int charBase, int extPltt) {
    reg_G2S_DB_BG1CNT = G2_BG_CONTROL_VALUE(reg_G2S_DB_BG1CNT, size, color, screenBase, charBase, extPltt);
}
// Like G2S_SetBG1Control, but without the extended-palette bit.
static inline void G2S_SetBG2ControlText(int size, int color, int screenBase, int charBase) {
    reg_G2S_DB_BG2CNT =
        (u16) ((reg_G2S_DB_BG2CNT & G2_BGCNT_KEEP_MASK) | (size << 14) | (color << 7) | (screenBase << 8) | (charBase << 2));
}

#define G2_BG_OFFSET_VALUE(h, v) (u32) ((((h) << 0) & G2_BGOFS_H_MASK) | (((v) << 16) & G2_BGOFS_V_MASK))

static inline void G2_SetBG0Offset(int h, int v) {
    reg_G2_BG0OFS = G2_BG_OFFSET_VALUE(h, v);
}
static inline void G2S_SetBG0Offset(int h, int v) {
    reg_G2S_DB_BG0OFS = G2_BG_OFFSET_VALUE(h, v);
}
static inline void G2S_SetBG1Offset(int h, int v) {
    reg_G2S_DB_BG1OFS = G2_BG_OFFSET_VALUE(h, v);
}
static inline void G2S_SetBG2Offset(int h, int v) {
    reg_G2S_DB_BG2OFS = G2_BG_OFFSET_VALUE(h, v);
}
static inline void G2S_SetBG3Offset(int h, int v) {
    reg_G2S_DB_BG3OFS = G2_BG_OFFSET_VALUE(h, v);
}

static inline void G2_SetBG0Priority(int priority) {
    reg_G2_BG0CNT = (u16) ((reg_G2_BG0CNT & ~G2_BGCNT_PRIORITY_MASK) | (priority << 0));
}
static inline void G2S_SetBG0Priority(int priority) {
    reg_G2S_DB_BG0CNT = (u16) ((reg_G2S_DB_BG0CNT & ~G2_BGCNT_PRIORITY_MASK) | (priority << 0));
}
static inline void G2S_SetBG1Priority(int priority) {
    reg_G2S_DB_BG1CNT = (u16) ((reg_G2S_DB_BG1CNT & ~G2_BGCNT_PRIORITY_MASK) | (priority << 0));
}
static inline void G2S_SetBG2Priority(int priority) {
    reg_G2S_DB_BG2CNT = (u16) ((reg_G2S_DB_BG2CNT & ~G2_BGCNT_PRIORITY_MASK) | (priority << 0));
}
static inline void G2S_SetBG3Priority(int priority) {
    reg_G2S_DB_BG3CNT = (u16) ((reg_G2S_DB_BG3CNT & ~G2_BGCNT_PRIORITY_MASK) | (priority << 0));
}

#ifdef __cplusplus
extern "C" {
#endif
void G2x_SetBlendBrightness_(vu16 *reg, int plane, int brightness);
#ifdef __cplusplus
}
#endif

static inline void G2_SetBlendBrightness(int plane, int brightness) {
    G2x_SetBlendBrightness_(&reg_G2_BLDCNT, plane, brightness);
}
static inline void G2S_SetBlendBrightness(int plane, int brightness) {
    G2x_SetBlendBrightness_(&reg_G2S_DB_BLDCNT, plane, brightness);
}

static inline void G2_SetWndOutsidePlane(int wnd, BOOL effect) {
    u32 value = (reg_G2_WINOUT & ~G2_WINOUT_PLANE_MASK) | ((u32) wnd << 0);
    if (effect) {
        value |= G2_WINOUT_EFFECT;
    }
    reg_G2_WINOUT = (u16) value;
}
static inline void G2S_SetWndOutsidePlane(int wnd, BOOL effect) {
    u32 value = (reg_G2S_DB_WINOUT & ~G2_WINOUT_PLANE_MASK) | ((u32) wnd << 0);
    if (effect) {
        value |= G2_WINOUT_EFFECT;
    }
    reg_G2S_DB_WINOUT = (u16) value;
}

static inline void G2_BlendNone(void) {
    reg_G2_BLDCNT = 0;
}
static inline void G2S_BlendNone(void) {
    reg_G2S_DB_BLDCNT = 0;
}
