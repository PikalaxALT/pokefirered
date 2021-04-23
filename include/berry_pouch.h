#ifndef GUARD_BERRY_POUCH_H
#define GUARD_BERRY_POUCH_H

#include "task.h"

enum BerryPouchType
{
    BERRYPOUCH_FROMFIELD,
    BERRYPOUCH_FROMPARTYGIVE,
    BERRYPOUCH_FROMMARTSELL,
    BERRYPOUCH_FROMPOKEMONSTORAGEPC,
    BERRYPOUCH_FROMBATTLE,
    BERRYPOUCH_FROMBERRYCRUSH,
    BERRYPOUCH_NA
};

enum
{
    BP_WIN_STATIC_LISTMENU = 0,
    BP_WIN_STATIC_ITEMDESC,
    BP_WIN_STATIC_HEADER,
    BP_WIN_STATIC_CT,

    BP_WIN_VAR_TOSSQUANTITYBOX = 0,
    BP_WIN_VAR_SELLQUANTITYBOX,
    BP_WIN_VAR_SELLPRICEBOX,
    BP_WIN_VAR_YESNOBOTTOM,
    BP_WIN_VAR_YESNOTOP,
    BP_WIN_VAR_MESSAGEBOX,
    BP_WIN_VAR_BERRYISSELECTEDBOX,
    BP_WIN_VAR_CONFIRMTOSSQUANTITY,
    BP_WIN_VAR_ASKHOWMANYTOTOSS,
    BP_WIN_VAR_FINALIZETOSS,
    BP_WIN_VAR_CONTEXT_1ITEM,
    BP_WIN_VAR_CONTEXT_2ITEM,
    BP_WIN_VAR_CONTEXT_3ITEM,
    BP_WIN_VAR_CONTEXT_4ITEM,
    BP_WIN_VAR_CT
};

void BerryPouch_StartFadeToExitCallback(u8 taskId);
void BerryPouch_SetExitCallback(void (*exitCallback)(void));
void InitBerryPouch(u8 type, void (*savedCallback)(void), u8 allowSelect);
void DisplayItemMessageInBerryPouch(u8 taskId, u8 fontId, const u8 * str, TaskFunc followUpFunc);
void Task_BerryPouch_DestroyDialogueWindowAndRefreshListMenu(u8 taskId);
void BerryPouch_CursorResetToTop(void);

#endif //GUARD_BERRY_POUCH_H
