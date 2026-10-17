#include "UI/SelectWindow.hpp"

#include "Graphics/CellAnimation.hpp"
#include "Graphics/SpriteManager.hpp"

// The three animation indices of a cell (WindowButton::mUnk_08..mUnk_0a), copied as one object.
// Placeholder for a real struct shared by WindowCellRecord::mAnimIndices and WindowButton::mUnk_08.
struct WindowCellAnims {
    u8 mAnims[3];
};

SelectWindow::SelectWindow(u32 itemCount, const WindowItemRecord *items, s32 unk3) {
    mCellCount = 0;
    mState   = 0;
    mSlideStep   = 0;
    mSavedSelection   = -1;
    mEnabled   = 1;
    mInputLocked   = 0;
    mSlideInRequested   = 0;
    mUnk_07e   = 0;
    mResult    = 0;
    mUnk_0e0   = 0;
    mCursor    = new SelectWindowCursor(itemCount, items, unk3);
    u8 i             = 0;
    WindowItem *item = mCursor->mItems;
    do {
        mCellPos[item->mId].x = item->mX;
        mCellPos[item->mId].y = item->mY;
        mSlideToTarget[item->mId]   = 0;
        i++;
        item++;
    } while (i < itemCount);
    for (i = 0; i < 22; i++) {
        mCells[i] = NULL;
    }
}

SelectWindow::~SelectWindow() {
    FreeCells();
    FreeCursor();
}

BOOL SelectWindow::Update(BOOL input) {
    int result = 0;
    u32 event  = 0;
    if (mEnabled == 0) {
        return 0;
    }
    switch (mState) {
    case 0:
        if (UpdateSlideIn()) {
            mState = 1;
        }
        break;
    case 1: {
        s8 oldSel        = mCursor->mSelection;
        WindowItem *items = mCursor->mItems;
        int oldId         = oldSel == -1 ? -1 : items[oldSel].mId;
        int oldPrev       = mCursor->mUnk_05;
        SelectWindowCursor *cur = mCursor;
        int oldPrevId     = oldPrev == -1 ? -1 : cur->mItems[oldPrev].mId;
        if (mInputLocked == 0) {
            event = mCursor->Update(input);
        }
        int newSel    = mCursor->mSelection;
        int newId     = newSel == -1 ? -1 : mCursor->mItems[newSel].mId;
        s8 newPrev   = mCursor->mUnk_05;
        int newPrevId = newPrev == -1 ? -1 : mCursor->mItems[newPrev].mId;
        if (mUnk_07e == 0 && newId != -1 && mButtons[newId].mEnabled == 0) {
            mCursor->mSelection = oldSel;
            return 0;
        }
        switch (event) {
        case 1:
            if (newId == -1) {
                return 0;
            }
            mCells[newId]->SetAnimation(mButtons[newId].mResultAnimIndex);
            if (oldId != -1 && newId != oldId) {
                mCells[oldId]->SetAnimation(mButtons[oldId].mUnk_08);
            }
            mResult  = 1;
            mInputLocked = 1;
            mState = 2;
            break;
        case 2:
            if (newId == -1) {
                return 0;
            }
            mCells[newId]->SetAnimation(mButtons[newId].mResultAnimIndex);
            if (oldId != -1 && newId != oldId) {
                CellAnimation *cell = mCells[oldId];
                u8 anim             = mButtons[oldId].mUnk_08;
                if (anim != cell->mAnimIndex) {
                    cell->SetAnimation(anim);
                }
            }
            mResult  = 2;
            mInputLocked = 1;
            mState = 2;
            break;
        case 3:
            if (newId == -1) {
                return 0;
            }
            mCells[newId]->SetAnimation(mButtons[newId].mResultAnimIndex);
            if (oldId != -1 && newId != oldId && mCursor->HasNeighbor(newSel)) {
                mCells[oldId]->SetAnimation(mButtons[oldId].mUnk_08);
            }
            mSavedSelection = oldSel;
            mResult  = 3;
            mState = 2;
            break;
        case 5:
            mUnk_0e0 = 1;
            if (newPrev != oldPrev) {
                if (newPrevId != -1 && items[newPrev].mUnk_18 == 3) {
                    CellAnimation *cell = mCells[newPrevId];
                    u8 anim             = mButtons[newPrevId].mUnk_0a;
                    if (anim != cell->mAnimIndex) {
                        cell->SetAnimation(anim);
                    }
                }
                if (oldPrevId != -1 && newPrevId != oldPrevId && oldPrev != newSel) {
                    mCells[oldPrevId]->SetAnimation(mButtons[oldPrevId].mUnk_08);
                }
            }
            break;
        case 4:
            mUnk_0e0 = 1;
            if (newSel != oldSel) {
                if (newId != -1) {
                    CellAnimation *cell = mCells[newId];
                    u8 anim             = mButtons[newId].mUnk_0a;
                    if (anim != cell->mAnimIndex) {
                        cell->SetAnimation(anim);
                    }
                }
                if (oldId != -1 && newId != oldId) {
                    mCells[oldId]->SetAnimation(mButtons[oldId].mUnk_08);
                }
            }
            if (newPrev != oldPrev) {
                if (newPrevId != -1) {
                    CellAnimation *cell = mCells[newPrevId];
                    u8 anim             = mButtons[newPrevId].mUnk_0a;
                    if (anim != cell->mAnimIndex) {
                        cell->SetAnimation(anim);
                    }
                }
                if (oldPrevId != -1 && newPrevId != oldPrevId && oldPrev != newSel) {
                    mCells[oldPrevId]->SetAnimation(mButtons[oldPrevId].mUnk_08);
                }
            }
            break;
        case 0:
            mUnk_0e0 = 0;
            if (newSel != oldSel) {
                if (newId != -1) {
                    CellAnimation *cell = mCells[newId];
                    u8 anim             = mButtons[newId].mUnk_0a;
                    if (anim != cell->mAnimIndex) {
                        cell->SetAnimation(anim);
                    }
                }
                if (oldId != -1 && newId != oldId) {
                    mCells[oldId]->SetAnimation(mButtons[oldId].mUnk_08);
                }
            }
            if (newPrev != oldPrev) {
                if (newPrevId != -1) {
                    CellAnimation *cell = mCells[newPrevId];
                    u8 anim             = mButtons[newPrevId].mUnk_0a;
                    if (anim != cell->mAnimIndex) {
                        cell->SetAnimation(anim);
                    }
                }
                if (oldPrevId != -1 && newPrevId != oldPrevId && oldPrev != newSel) {
                    mCells[oldPrevId]->SetAnimation(mButtons[oldPrevId].mUnk_08);
                }
            }
            break;
        case 7:
            mUnk_0e0 = 0;
            if (newPrev != oldPrev && oldPrevId != -1 && newPrevId != oldPrevId && oldPrev != newSel) {
                mCells[oldPrevId]->SetAnimation(mButtons[oldPrevId].mUnk_08);
            }
            break;
        }
        break;
    }
    case 2: {
        BOOL flag = FALSE;
        SelectWindowCursor *c = mCursor;
        s8 sel = c->mSelection;
        if (mCells[sel == -1 ? -1 : c->mItems[sel].mId]->mFinished != 0) {
            flag = TRUE;
        }
        if (mCells[sel == -1 ? -1 : c->mItems[sel].mId]->mMode == 1) {
            flag = TRUE;
        }
        if (flag) {
            mUnk_0e0 = 0;
            if (mCursor->mItems[mCursor->mSelection].mFlags & 8) {
                mState = 5;
                result   = mResult;
            } else {
                result   = mResult;
                mState = 3;
            }
        }
        break;
    }
    case 3:
        UpdateSlideOut();
        SelectWindowCursor *c = mCursor;
        if (mSlideStep == 2) {
            if (c->mSelection != -1) {
                mCells[GetSelectedId()]->SetAnimation(mButtons[GetSelectedId()].mUnk_08);
            }
        } else if (mSlideStep == 0) {
            mSlideInRequested = 0;
            mState = 4;
        }
        break;
    case 4:
        if (mSlideInRequested != 0) {
            mInputLocked = 0;
            mState = 0;
        }
        break;
    case 5: {
        if (mSavedSelection != mCursor->mSelection) {
            int id = GetSelectedId();
            if (id != -1) {
                mCells[GetSelectedId()]->SetAnimation(mButtons[GetSelectedId()].mUnk_08);
            }
        }
        mCursor->mSelection = mSavedSelection;
        mSavedSelection            = -1;
        SelectWindowCursor *cursor = mCursor;
        cursor->mUnk_05    = -1;
        cursor->mPressedItem    = -1;
        cursor->mPressedOnSelection    = 0;
        mState            = 1;
        break;
    }
    }
    return result;
}

void SelectWindow::SetCells(u32 count, const WindowCellRecord *cells) {
    u8 i                 = 0;
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.SetObjVramMode(3);
    do {
        const WindowCellRecord *cell = &cells[i];
        if (cell->mPath != NULL) {
            mCells[mCellCount]            = new CellAnimation(cell->mPath, cell->mAnimIndex, 1, 1);
            mCells[mCellCount]->mCharBase = screen.GetCharBase(cell->mCharOffset);
            mCells[mCellCount]->mPaletteBase   = cell->mPaletteBase;
        }
        WindowButton *button = &mButtons[mCellCount];
        i++;
        button->mUnk_00  = cell->mUnk_10;
        button->mEnabled = cell->mEnabled;
        *(WindowCellAnims *) &button->mUnk_08 = *(const WindowCellAnims *) cell->mAnimIndices;
        mCellCount++;
    } while (i < count);
}

void SelectWindow::FreeCursor() {
    delete mCursor;
    mCursor = NULL;
}

void SelectWindow::FreeCells() {
    u8 i = 0;
    do {
        CellAnimation *cell = mCells[i];
        if (cell != NULL) {
            delete cell;
        }
        mCells[i] = NULL;
        i++;
    } while (i < 22);
}

void SelectWindow::Draw() {
    u8 i = 0;
    do {
        if (mButtons[i].mEnabled != 0) {
            if (mCells[i] != NULL) {
                mCells[i]->Update();
                mCells[i]->Draw(1, mCellPos[i].x, mCellPos[i].y, mButtons[i].mUnk_00);
            }
        } else if (mUnk_07e != 0 && mCells[i] != NULL) {
            mCells[i]->Update();
        }
        i++;
    } while (i < mCellCount);
}

BOOL SelectWindow::UpdateSlideIn() {
    u8 i             = 0;
    BOOL done        = TRUE;
    WindowItem *item = mCursor->mItems;
    switch (mSlideStep) {
    case 0:
        do {
            s8 id = item->mId;
            if (mSlideToTarget[id] == 0 && item->mSlideIn != 0) {
                u8 type = item->mSlideIn;
                if (type == 4) {
                    mCellPos[id].x = -item->mWidth;
                } else if (type == 3) {
                    mCellPos[id].x = 0x100;
                } else if (type == 1) {
                    mCellPos[id].y = 0xc0;
                } else if (type == 2) {
                    mCellPos[id].y = -item->mHeight;
                }
            }
            i++;
            if (item->mSlideIn != 0) {
                mButtons[item->mId].mEnabled = TRUE;
            }
            item++;
        } while (i < mCursor->mCount);
        mSlideStep = 1;
        break;
    case 1:
        do {
            u8 type = item->mSlideIn;
            if (type == 4) {
                s8 id = item->mId;
                if (mSlideToTarget[id] != 0) {
                    mCellPos[id].x += (s8)((mSlideTargetX + 0x20 - mCellPos[id].x) / 7);
                    if (mSlideTargetX <= mCellPos[item->mId].x) {
                        mCellPos[item->mId].x = mSlideTargetX;
                    } else {
                        done = FALSE;
                    }
                }
            } else if (type == 3) {
                s8 id = item->mId;
                if (mSlideToTarget[id] != 0) {
                    mCellPos[id].x += (s8)((mSlideTargetX - 0x20 - mCellPos[id].x) / 7);
                    if (mSlideTargetX >= mCellPos[item->mId].x) {
                        mCellPos[item->mId].x = mSlideTargetX;
                    } else {
                        done = FALSE;
                    }
                }
            } else if (type == 1) {
                s8 id = item->mId;
                if (mSlideToTarget[id] != 0) {
                    mCellPos[id].y += (s8)((mSlideTargetY - 0x20 - mCellPos[id].y) / 7);
                    if (mSlideTargetY >= mCellPos[item->mId].y) {
                        mCellPos[item->mId].y = mSlideTargetY;
                    } else {
                        done = FALSE;
                    }
                }
            } else if (type == 2) {
                s8 id = item->mId;
                if (mSlideToTarget[id] != 0) {
                    mCellPos[id].y += (s8)((mSlideTargetY + 0x20 - mCellPos[id].y) / 7);
                    if (mSlideTargetY <= mCellPos[item->mId].y) {
                        mCellPos[item->mId].y = mSlideTargetY;
                    } else {
                        done = FALSE;
                    }
                }
            }
            i++;
            item++;
        } while (i < mCursor->mCount);
        if (done) {
            i    = 0;
            item = mCursor->mItems;
            do {
                u8 type = item->mSlideIn;
                if (type == 4) {
                    s8 id = item->mId;
                    if (mSlideToTarget[id] == 0) {
                        mCellPos[id].x += (s8)((item->mX + 0x20 - mCellPos[id].x) / 7);
                        if (item->mX <= mCellPos[item->mId].x) {
                            mCellPos[item->mId].x = item->mX;
                        } else {
                            done = FALSE;
                        }
                    }
                } else if (type == 3) {
                    s8 id = item->mId;
                    if (mSlideToTarget[id] == 0) {
                        mCellPos[id].x += (s8)((item->mX - 0x20 - mCellPos[id].x) / 7);
                        if (item->mX >= mCellPos[item->mId].x) {
                            mCellPos[item->mId].x = item->mX;
                        } else {
                            done = FALSE;
                        }
                    }
                } else if (type == 1) {
                    s8 id = item->mId;
                    if (mSlideToTarget[id] == 0) {
                        mCellPos[id].y += (s8)((item->mY - 0x20 - mCellPos[id].y) / 7);
                        if (item->mY >= mCellPos[item->mId].y) {
                            mCellPos[item->mId].y = item->mY;
                        } else {
                            done = FALSE;
                        }
                    }
                } else if (type == 2) {
                    s8 id = item->mId;
                    if (mSlideToTarget[id] == 0) {
                        mCellPos[id].y += (s8)((item->mY + 0x20 - mCellPos[id].y) / 7);
                        if (item->mY <= mCellPos[item->mId].y) {
                            mCellPos[item->mId].y = item->mY;
                        } else {
                            done = FALSE;
                        }
                    }
                }
                i++;
                item++;
            } while (i < mCursor->mCount);
        }
        if (done) {
            mSlideStep = 2;
        }
        break;
    case 2:
        mSlideStep = i;
        return done;
    }
    return FALSE;
}

void SelectWindow::UpdateSlideOut() {
    SelectWindowCursor *cursor;
    BOOL done;
    WindowItem *item;
    u8 i;
    i    = 0;
    done = TRUE;
    item = mCursor->mItems;
    switch (mSlideStep) {
    case 0:
        mSlideStep = 1;
        break;
    case 1:
        do {
            u8 type = item->mSlideOut;
            if (type == 4) {
                s8 id = item->mId;
                if (mSlideToTarget[id] == 0) {
                    mCellPos[id].x += (s8)((0x120 - mCellPos[id].x) / 7);
                    if (mCellPos[item->mId].x >= 0x100) {
                        mCellPos[item->mId].x = 0x100;
                    } else {
                        done = FALSE;
                    }
                }
            } else if (type == 3) {
                s8 id = item->mId;
                if (mSlideToTarget[id] == 0) {
                    mCellPos[id].x += (s8)(-(item->mWidth + 0x20 + mCellPos[id].x) / 7);
                    if (-item->mWidth >= mCellPos[item->mId].x) {
                        mCellPos[item->mId].x = -item->mWidth;
                    } else {
                        done = FALSE;
                    }
                }
            } else if (type == 1) {
                s8 id = item->mId;
                if (mSlideToTarget[id] == 0) {
                    mCellPos[id].y += (s8)(-(item->mHeight + 0x20 + mCellPos[id].y) / 7);
                    if (-item->mHeight >= mCellPos[item->mId].y) {
                        mCellPos[item->mId].y = -item->mHeight;
                    } else {
                        done = FALSE;
                    }
                }
            } else if (type == 2) {
                s8 id = item->mId;
                if (mSlideToTarget[id] == 0) {
                    mCellPos[id].y += (s8)((0xe0 - mCellPos[id].y) / 7);
                    if (mCellPos[item->mId].y >= 0xc0) {
                        mCellPos[item->mId].y = 0xc0;
                    } else {
                        done = FALSE;
                    }
                }
            }
            cursor = mCursor;
            i++;
            item++;
        } while (i < cursor->mCount);
        if (done) {
            i    = 0;
            item = cursor->mItems;
            do {
                u8 type = item->mSlideOut;
                if (type == 4) {
                    s8 id = item->mId;
                    if (mSlideToTarget[id] != 0) {
                        mCellPos[id].x += (s8)((mSlideTargetX + 0x20 - mCellPos[id].x) / 7);
                        if (mSlideTargetX <= mCellPos[item->mId].x) {
                            mCellPos[item->mId].x = mSlideTargetX;
                        } else {
                            done = FALSE;
                        }
                    }
                } else if (type == 3) {
                    s8 id = item->mId;
                    if (mSlideToTarget[id] != 0) {
                        mCellPos[id].x += (s8)((mSlideTargetX - 0x20 - mCellPos[id].x) / 7);
                        if (mSlideTargetX >= mCellPos[item->mId].x) {
                            mCellPos[item->mId].x = mSlideTargetX;
                        } else {
                            done = FALSE;
                        }
                    }
                } else if (type == 1) {
                    s8 id = item->mId;
                    if (mSlideToTarget[id] != 0) {
                        mCellPos[id].y += (s8)((mSlideTargetY - 0x20 - mCellPos[id].y) / 7);
                        if (mSlideTargetY >= mCellPos[item->mId].y) {
                            mCellPos[item->mId].y = mSlideTargetY;
                        } else {
                            done = FALSE;
                        }
                    }
                } else if (type == 2) {
                    s8 id = item->mId;
                    if (mSlideToTarget[id] != 0) {
                        mCellPos[id].y += (s8)((mSlideTargetY + 0x20 - mCellPos[id].y) / 7);
                        if (mSlideTargetY <= mCellPos[item->mId].y) {
                            mCellPos[item->mId].y = mSlideTargetY;
                        } else {
                            done = FALSE;
                        }
                    }
                }
                i++;
                item++;
            } while (i < mCursor->mCount);
        }
        if (done) {
            mSlideStep = 2;
        }
        break;
    case 2:
        do {
            if (item->mSlideIn != 0) {
                s8 id = item->mId;
                if (mSlideToTarget[id] == 0) {
                    mButtons[id].mEnabled = FALSE;
                }
            }
            i++;
            item++;
        } while (i < mCursor->mCount);
        mSlideStep = 0;
        break;
    }
}

void SelectWindow::SetItems(u32 count, WindowItem *items, s32 selection) {
    mCursor->mSelection = -1;
    mCursor->mSelection = selection;
    SelectWindowCursor *cursor = mCursor;
    cursor->mItems             = items;
    cursor->mCount             = count;
    WindowItem *item           = mCursor->mItems;
    u8 i                = 0;
    do {
        mCellPos[item->mId].x = item->mX;
        mCellPos[item->mId].y = item->mY;
        mSlideToTarget[item->mId]   = 0;
        i++;
        item++;
    } while (i < count);
    mSlideInRequested = 1;
}

void SelectWindow::Reset(int selection) {
    mCursor->mSelection = -1;
    mCursor->mSelection = selection;
    WindowItem *item    = mCursor->mItems;
    u8 i                = 0;
    do {
        mCellPos[item->mId].x = item->mX;
        mCellPos[item->mId].y = item->mY;
        mSlideToTarget[item->mId]   = 0;
        i++;
        item++;
    } while (i < mCursor->mCount);
    mSlideInRequested = 1;
    mUnk_0e0 = 0;
}

void SelectWindow::SetResultItem(u32 unk1, u32 unk2) {
    s8 oldId = GetSelectedId();
    mCursor->mSelection = unk1;
    s8 sel = mCursor->mSelection;
    mCells[unk1]->SetAnimation(mButtons[unk1].mResultAnimIndex);
    if (oldId != -1 && unk1 != oldId && mCursor->HasNeighbor(sel)) {
        mCells[oldId]->SetAnimation(mButtons[oldId].mUnk_08);
    }
    mResult   = unk2;
    mState  = 2;
}
