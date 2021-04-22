#include "global.h"
#include "task.h"
#include "text.h"
#include "window.h"
#include "text_window.h"
#include "money.h"
#include "menu_helpers.h"
#include "new_menu_helpers.h"
#include "menu.h"
#include "shop.h"

static const struct WindowTemplate sShopBuyMenuWindowTemplatesNormal[] =
{
    [BMH_WIN_MONEYBOX] = {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 1,
        .width = 8,
        .height = 3,
        .paletteNum = 15,
        .baseBlock = 0x27,
    },
    [BMH_WIN_BAGQUANTITYBOX] = {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 11,
        .width = 13,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 0x3F,
    },
    [BMH_WIN_MESSAGEBOX] = {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 15,
        .width = 26,
        .height = 4,
        .paletteNum = 14,
        .baseBlock = 0x59,
    },
    [BMH_WIN_BUYQUANTITYBOX] = {
        .bg = 0,
        .tilemapLeft = 17,
        .tilemapTop = 9,
        .width = 12,
        .height = 4,
        .paletteNum = 14,
        .baseBlock = 0xC1,
    },
    [BMH_WIN_ITEMLIST] = {
        .bg = 0,
        .tilemapLeft = 11,
        .tilemapTop = 1,
        .width = 17,
        .height = 12,
        .paletteNum = 14,
        .baseBlock = 0xF1,
    },
    [BMH_WIN_ITEMDESC] = {
        .bg = 0,
        .tilemapLeft = 5,
        .tilemapTop = 14,
        .width = 25,
        .height = 6,
        .paletteNum = 15,
        .baseBlock = 0x1BD,
    },
    DUMMY_WIN_TEMPLATE,
};

// firered uses different layout when selling TMs
static const struct WindowTemplate sShopBuyMenuWindowTemplatesTM[] =
{
    [BMH_WIN_MONEYBOX] = {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 1,
        .width = 8,
        .height = 3,
        .paletteNum = 15,
        .baseBlock = 0x27,
    },
    [BMH_WIN_BAGQUANTITYBOX] = {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 11,
        .width = 13,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 0x3F,
    },
    [BMH_WIN_MESSAGEBOX] = {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 15,
        .width = 26,
        .height = 4,
        .paletteNum = 14,
        .baseBlock = 0x59,
    },
    [BMH_WIN_BUYQUANTITYBOX] = {
        .bg = 0,
        .tilemapLeft = 17,
        .tilemapTop = 9,
        .width = 12,
        .height = 4,
        .paletteNum = 14,
        .baseBlock = 0xC1,
    },
    [BMH_WIN_ITEMLIST] = {
        .bg = 0,
        .tilemapLeft = 11,
        .tilemapTop = 1,
        .width = 17,
        .height = 10,
        .paletteNum = 14,
        .baseBlock = 0xF1,
    },
    [BMH_WIN_ITEMDESC] = {
        .bg = 0,
        .tilemapLeft = 12,
        .tilemapTop = 12,
        .width = 18,
        .height = 8,
        .paletteNum = 14,
        .baseBlock = 0x19B,
    },
    [BMH_WIN_TMHMMOVENAME] = {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 14,
        .width = 10,
        .height = 4,
        .paletteNum = 14,
        .baseBlock = 0x22B,
    },
    DUMMY_WIN_TEMPLATE,
};

static const struct WindowTemplate sShopBuyMenuYesNoWindowTemplate =
{
    .bg = 0,
    .tilemapLeft = 21,
    .tilemapTop = 9,
    .width = 6,
    .height = 4,
    .paletteNum = 14,
    .baseBlock = 0xC1,
};

static const u8 sShopBuyMenuTextColors[][3] =
{
    {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY},
    {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY},
    {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_LIGHT_GRAY, TEXT_COLOR_DARK_GRAY}
};

void BuyMenuInitWindows(bool32 isSellingTM)
{
    if (isSellingTM != TRUE)
        InitWindows(sShopBuyMenuWindowTemplatesNormal);
    else
        InitWindows(sShopBuyMenuWindowTemplatesTM);
    DeactivateAllTextPrinters();
    TextWindow_SetUserSelectedFrame(BMH_WIN_MONEYBOX, 0x1, 0xD0);
    TextWindow_LoadResourcesStdFrame0(BMH_WIN_MONEYBOX, 0x13, 0xE0);
    TextWindow_SetStdFrame0_WithPal(BMH_WIN_MONEYBOX, 0xA, 0xF0);
    PutWindowTilemap(BMH_WIN_MONEYBOX);
    PutWindowTilemap(BMH_WIN_ITEMLIST);
    PutWindowTilemap(BMH_WIN_ITEMDESC);
    if (isSellingTM == TRUE)
        PutWindowTilemap(BMH_WIN_TMHMMOVENAME);
}

void BuyMenuDrawMoneyBox(void)
{
    PrintMoneyAmountInMoneyBoxWithBorder(BMH_WIN_MONEYBOX, 0xA, 0xF, GetMoney(&gSaveBlock1Ptr->money));
}

void BuyMenuPrint(u8 windowId, u8 font, const u8 *text, u8 x, u8 y, u8 letterSpacing, u8 lineSpacing, u8 speed, u8 color)
{
    AddTextPrinterParameterized4(windowId, font, x, y, letterSpacing, lineSpacing, sShopBuyMenuTextColors[color], speed, text);
}

void BuyMenuDisplayMessage(u8 taskId, const u8 *text, TaskFunc callback)
{
    DisplayMessageAndContinueTask(taskId, BMH_WIN_MESSAGEBOX, 0x13, 0xE, GetMartMessageFontId(), GetTextSpeedSetting(), text, callback);
    ScheduleBgCopyTilemapToVram(0);
}

void BuyMenuQuantityBoxNormalBorder(u8 windowId, bool8 copyToVram)
{
    DrawStdFrameWithCustomTileAndPalette(windowId, copyToVram, 0x1, 0xD);
}

void BuyMenuQuantityBoxThinBorder(u8 windowId, bool8 copyToVram)
{
    DrawStdFrameWithCustomTileAndPalette(windowId, copyToVram, 0xA, 0xF);
}

void BuyMenuConfirmPurchase(u8 taskId, const struct YesNoFuncTable *yesNo)
{
    CreateYesNoMenuWithCallbacks(taskId, &sShopBuyMenuYesNoWindowTemplate, 2, 0, 2, 1, 0xD, yesNo);
}
