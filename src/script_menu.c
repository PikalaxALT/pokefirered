#include "global.h"
#include "gflib.h"
#include "menu.h"
#include "task.h"
#include "script_menu.h"
#include "quest_log.h"
#include "new_menu_helpers.h"
#include "event_data.h"
#include "script.h"
#include "strings.h"
#include "field_effect.h"
#include "event_scripts.h"
#include "constants/songs.h"
#include "constants/seagallop.h"
#include "constants/menu.h"

struct MultichoiceListStruct
{
    const struct MenuAction * list;
    u8 count;
};

static EWRAM_DATA u8 sDelay = 0;

static void DrawVerticalMultichoiceMenu(u8 left, u8 top, u8 mcId, u8 ignoreBpress, u8 initPos);
static u8 GetMCWindowHeight(u8 count);
static void CreateMCMenuInputHandlerTask(u8 ignoreBpress, u8 count, u8 windowId, u8 mcId);
static void Task_MultichoiceMenu_HandleInput(u8 taskId);
static void MultiChoicePrintHelpDescription(u8 mcId);
static void Task_YesNoMenu_HandleInput(u8 taskId);
static void Hask_MultichoiceGridMenu_HandleInput(u8 taskId);
static void CreatePCMenuWindow(void);
static bool8 PicboxWait(void);
static void DestroyScriptMenuWindow(u8 windowId);
static u8 CreateWindowFromRect(u8 left, u8 top, u8 width, u8 height);


static const struct MenuAction sScriptMultiChoiceMenu_YesNo[] = {
    { gText_Yes },
    { gText_No }
};

static const struct MenuAction sScriptMultiChoiceMenu_TrainerCardIconTint[] = {
    { gText_Normal },
    { gText_Black },
    { gText_Pink },
    { gText_Sepia }
};

static const struct MenuAction sScriptMultiChoiceMenu_HOF_Quit[] = {
    { gText_HallOfFame },
    { gText_Quit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Eggs_Quit[] = {
    { gText_Eggs },
    { gText_Quit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Victories_Quit[] = {
    { gText_Victories },
    { gText_Quit }
};

static const struct MenuAction sScriptMultiChoiceMenu_HOF_Eggs_Quit[] = {
    { gText_HallOfFame },
    { gText_Eggs },
    { gText_Quit }
};

static const struct MenuAction sScriptMultiChoiceMenu_HOF_Victories_Quit[] = {
    { gText_HallOfFame },
    { gText_Victories },
    { gText_Quit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Eggs_Victories_Quit[] = {
    { gText_Eggs },
    { gText_Victories },
    { gText_Quit }
};

static const struct MenuAction sScriptMultiChoiceMenu_HOF_Eggs_Victories_Quit[] = {
    { gText_HallOfFame },
    { gText_Eggs },
    { gText_Victories },
    { gText_Quit }
};

static const struct MenuAction sScriptMultiChoiceMenu_TrainerSchoolWhiteboard[] = {
    { gText_Slp },
    { gText_Psn },
    { gText_Par },
    { gText_Brn },
    { gText_Frz },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_YesNoInfo[] = {
    { gText_Yes },
    { gText_No },
    { gText_Info }
};

static const struct MenuAction sScriptMultiChoiceMenu_SingleDoubleMultiInfoExit[] = {
    { gText_SingleBattle },
    { gText_DoubleBattle },
    { gText_MultiBattle },
    { gText_Info },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_YesNoInfo2[] = {
    { gText_Yes },
    { gText_No },
    { gText_Info }
};

static const struct MenuAction sScriptMultiChoiceMenu_ChallengeInfoExit[] = {
    { gText_MakeAChallenge },
    { gText_Info_2 },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_RooftopB1F[] = {
    { gText_Rooftop },
    { gText_B1F },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Helix[] = {
    { gText_HelixFossil },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Dome[] = {
    { gText_DomeFossil },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Amber[] = {
    { gText_OldAmber },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_HelixAmber[] = {
    { gText_HelixFossil },
    { gText_OldAmber },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_DomeAmber[] = {
    { gText_DomeFossil },
    { gText_OldAmber },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_CeladonVendingMachine[] = {
    { gText_FreshWater_200 },
    { gText_SodaPop_300 },
    { gText_Lemonade_350 },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_GameCornerTMPrizes[] = {
    { gText_Tm13_4000Coins },
    { gText_Tm23_3500Coins },
    { gText_Tm24_4000Coins },
    { gText_Tm30_4500Coins },
    { gText_Tm35_4000Coins },
    { gText_NoThanks_2 }
};

static const struct MenuAction sScriptMultiChoiceMenu_GameCornerBattleItemPrizes[] = {
    { gText_SmokeBall_800Coins },
    { gText_MiracleSeed_1000Coins },
    { gText_Charcoal_1000Coins },
    { gText_MysticWater_1000Coins },
    { gText_YellowFlute_1600Coins },
    { gText_NoThanks_2 }
};

static const struct MenuAction sScriptMultiChoiceMenu_GameCornerCoinPurchaseCounter[] = {
    { gText_50Coins_1000 },
    { gText_500Coins_10000 },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Excellent_NotSoBad[] = {
    { gText_Excellent },
    { gText_NotSoBad }
};

static const struct MenuAction sScriptMultiChoiceMenu_RightLeft[] = {
    { gText_Right },
    { gText_Left }
};

static const struct MenuAction sScriptMultiChoiceMenu_DeptStoreElevator[] = {
    { gText_5F_2 },
    { gText_4F_2 },
    { gText_3F_2 },
    { gText_2F_2 },
    { gText_1F_2 },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_ThirstyGirlFreshWater[] = {
    { gText_FreshWater },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_ThirstyGirlSodaPop[] = {
    { gText_SodaPop },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_ThirstyGirlFreshWaterSodaPop[] = {
    { gText_FreshWater },
    { gText_SodaPop },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_ThirstyGirlLemonade[] = {
    { gText_Lemonade },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_ThirstyGirlFreshWaterLemonade[] = {
    { gText_FreshWater },
    { gText_Lemonade },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_ThirstyGirlSodaPopLemonade[] = {
    { gText_SodaPop },
    { gText_Lemonade },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_ThirstyGirlFreshWaterSodaPopLemonade[] = {
    { gText_FreshWater },
    { gText_SodaPop },
    { gText_Lemonade },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Unref_Shards_0[] = {
    { gText_GreenShard },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Unref_Shards_1[] = {
    { gText_RedShard },
    { gText_GreenShard },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Unref_Shards_2[] = {
    { gText_YellowShard },
    { gText_GreenShard },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Unref_Shards_3[] = {
    { gText_RedShard },
    { gText_YellowShard },
    { gText_GreenShard },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Unref_Shards_4[] = {
    { gText_BlueShard },
    { gText_GreenShard },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Unref_Shards_5[] = {
    { gText_RedShard },
    { gText_BlueShard },
    { gText_GreenShard },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Unref_Shards_6[] = {
    { gText_YellowShard },
    { gText_BlueShard },
    { gText_GreenShard },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Unref_Shards_7[] = {
    { gText_RedShard },
    { gText_YellowShard },
    { gText_BlueShard },
    { gText_GreenShard },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Eeveelutions[] = {
    { gText_Eevee },
    { gText_Flareon },
    { gText_Jolteon },
    { gText_Vaporeon },
    { gText_QuitLooking }
};

static const struct MenuAction sScriptMultiChoiceMenu_BikeShop[] = {
    { gText_Bicycle_1000000 },
    { gText_NoThanks }
};

static const struct MenuAction sScriptMultiChoiceMenu_GameCornerPokemonPrizes[] = {
#if defined(FIRERED)
    { gText_Abra_180Coins },
    { gText_Clefairy_500Coins },
    { gText_Dratini_2800Coins },
    { gText_Scyther_5500Coins },
    { gText_Porygon_9999Coins },
#elif defined(LEAFGREEN)
    { gText_Abra_120Coins },
    { gText_Clefairy_750Coins },
    { gText_Pinsir_2500Coins },
    { gText_Dratini_4600Coins },
    { gText_Porygon_6500Coins },
#endif
    { gText_NoThanks_2 }
};

static const struct MenuAction sScriptMultiChoiceMenu_TradeCenter_Colosseum[] = {
    { gText_TradeCenter },
    { gText_Colosseum },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Link_Wireless[] = {
    { gText_GameLinkCable },
    { gText_Wireless },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_RocketHideoutElevator[] = {
    { gText_B1F },
    { gText_B2F },
    { gText_B4F },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_LinkedDirectUnion[] = {
    { gText_LinkedGamePlay },
    { gText_DirectCorner },
    { gText_UnionRoom },
    { gOtherText_Quit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Island23[] = {
    { gText_TwoIsland },
    { gText_ThreeIsland },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Island13[] = {
    { gText_OneIsland },
    { gText_ThreeIsland },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Island12[] = {
    { gText_OneIsland },
    { gText_TwoIsland },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_TradeColosseumCrush[] = {
    { gText_TradeCenter },
    { gText_Colosseum_2 },
    { gText_BerryCrush },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_48[] = {
    { gText_Dummy_8417E66 },
    { gText_Dummy_8417E67 },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_TradeColosseum_2[] = {
    { gText_TradeCenter },
    { gText_Colosseum_2 },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_PokejumpDodrio[] = {
    { gText_PokemonJump },
    { gText_DodrioBerryPicking },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Mushrooms[] = {
    { gText_2Tinymushrooms },
    { gText_1BigMushroom }
};

static const struct MenuAction sScriptMultiChoiceMenu_TradeColosseumBlankCrush[] = {
    { gText_TradeCenter },
    { gText_Colosseum_2 },
    { gText_Dummy_8417E71 },
    { gText_BerryCrush },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_TradeColosseumBlank[] = {
    { gText_TradeCenter },
    { gText_Colosseum_2 },
    { gText_Dummy_8417E71 },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_SeviiNavel[] = {
    { gText_SeviiIslands },
    { gText_NavelRock },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_SeviiBirth[] = {
    { gText_SeviiIslands },
    { gText_BirthIsland },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_SeviiNavelBirth[] = {
    { gText_SeviiIslands },
    { gText_NavelRock },
    { gText_BirthIsland },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Seagallop123[] = {
    { gText_OneIsland },
    { gText_TwoIsland },
    { gText_ThreeIsland },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_SeagallopV23[] = {
    { gText_Vermilion },
    { gText_TwoIsland },
    { gText_ThreeIsland },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_SeagallopV13[] = {
    { gText_Vermilion },
    { gText_OneIsland },
    { gText_ThreeIsland },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_SeagallopV12[] = {
    { gText_Vermilion },
    { gText_OneIsland },
    { gText_TwoIsland },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_SeagallopVermilion[] = {
    { gText_Vermilion },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_62[] = {
    { gText_Dummy_8417F68 },
    { gText_Dummy_8417F67 },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_JoinOrLead[] = {
    { gText_JoinGroup },
    { gText_BecomeLeader },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_TrainerTowerMode[] = {
    { gOtherText_Single },
    { gOtherText_Double },
    { gOtherText_Knockout },
    { gOtherText_Mixed },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Exit[] = {
    { gOtherText_Exit }
};

const struct MultichoiceListStruct gScriptMultiChoiceMenus[] = {
    [MULTICHOICE_YES_NO]                                     = { sScriptMultiChoiceMenu_YesNo,  NELEMS(sScriptMultiChoiceMenu_YesNo)  },
    [MULTICHOICE_EEVEELUTIONS]                               = { sScriptMultiChoiceMenu_Eeveelutions,  NELEMS(sScriptMultiChoiceMenu_Eeveelutions)  },
    [MULTICHOICE_TRAINER_CARD_ICON_TINT]                     = { sScriptMultiChoiceMenu_TrainerCardIconTint,  NELEMS(sScriptMultiChoiceMenu_TrainerCardIconTint)  },
    [MULTICHOICE_HOF_QUIT]                                   = { sScriptMultiChoiceMenu_HOF_Quit,  NELEMS(sScriptMultiChoiceMenu_HOF_Quit)  },
    [MULTICHOICE_EGGS_QUIT]                                  = { sScriptMultiChoiceMenu_Eggs_Quit,  NELEMS(sScriptMultiChoiceMenu_Eggs_Quit)  },
    [MULTICHOICE_VICTORIES_QUIT]                             = { sScriptMultiChoiceMenu_Victories_Quit,  NELEMS(sScriptMultiChoiceMenu_Victories_Quit)  },
    [MULTICHOICE_HOF_EGGS_QUIT]                              = { sScriptMultiChoiceMenu_HOF_Eggs_Quit,  NELEMS(sScriptMultiChoiceMenu_HOF_Eggs_Quit)  },
    [MULTICHOICE_HOF_VICTORIES_QUIT]                         = { sScriptMultiChoiceMenu_HOF_Victories_Quit,  NELEMS(sScriptMultiChoiceMenu_HOF_Victories_Quit)  },
    [MULTICHOICE_EGGS_VICTORIES_QUIT]                        = { sScriptMultiChoiceMenu_Eggs_Victories_Quit,  NELEMS(sScriptMultiChoiceMenu_Eggs_Victories_Quit)  },
    [MULTICHOICE_HOF_EGGS_VICTORIES_QUIT]                    = { sScriptMultiChoiceMenu_HOF_Eggs_Victories_Quit,  NELEMS(sScriptMultiChoiceMenu_HOF_Eggs_Victories_Quit)  },
    [MULTICHOICE_EXIT]                                       = { sScriptMultiChoiceMenu_Exit, NELEMS(sScriptMultiChoiceMenu_Exit) },
    [MULTICHOICE_EXIT_2]                                     = { sScriptMultiChoiceMenu_Exit, NELEMS(sScriptMultiChoiceMenu_Exit) },
    [MULTICHOICE_EXIT_3]                                     = { sScriptMultiChoiceMenu_Exit, NELEMS(sScriptMultiChoiceMenu_Exit) },
    [MULTICHOICE_BIKE_SHOP]                                  = { sScriptMultiChoiceMenu_BikeShop, NELEMS(sScriptMultiChoiceMenu_BikeShop) },
    [MULTICHOICE_GAME_CORNER_POKEMON_PRIZES]                 = { sScriptMultiChoiceMenu_GameCornerPokemonPrizes, NELEMS(sScriptMultiChoiceMenu_GameCornerPokemonPrizes) },
    [MULTICHOICE_TRAINER_SCHOOL_WHITEBOARD]                  = { sScriptMultiChoiceMenu_TrainerSchoolWhiteboard, NELEMS(sScriptMultiChoiceMenu_TrainerSchoolWhiteboard) },
    [MULTICHOICE_YES_NO_INFO]                                = { sScriptMultiChoiceMenu_YesNoInfo, NELEMS(sScriptMultiChoiceMenu_YesNoInfo) },
    [MULTICHOICE_SINGLE_DOUBLE_MULTI_INFO_EXIT]              = { sScriptMultiChoiceMenu_SingleDoubleMultiInfoExit, NELEMS(sScriptMultiChoiceMenu_SingleDoubleMultiInfoExit) },
    [MULTICHOICE_YES_NO_INFO_2]                              = { sScriptMultiChoiceMenu_YesNoInfo2, NELEMS(sScriptMultiChoiceMenu_YesNoInfo2) },
    [MULTICHOICE_CHALLENGE_INFO_EXIT]                        = { sScriptMultiChoiceMenu_ChallengeInfoExit, NELEMS(sScriptMultiChoiceMenu_ChallengeInfoExit) },
    [MULTICHOICE_ROOFTOP_B1F]                                = { sScriptMultiChoiceMenu_RooftopB1F, NELEMS(sScriptMultiChoiceMenu_RooftopB1F) },
    [MULTICHOICE_HELIX]                                      = { sScriptMultiChoiceMenu_Helix, NELEMS(sScriptMultiChoiceMenu_Helix) },
    [MULTICHOICE_DOME]                                       = { sScriptMultiChoiceMenu_Dome, NELEMS(sScriptMultiChoiceMenu_Dome) },
    [MULTICHOICE_AMBER]                                      = { sScriptMultiChoiceMenu_Amber, NELEMS(sScriptMultiChoiceMenu_Amber) },
    [MULTICHOICE_HELIX_AMBER]                                = { sScriptMultiChoiceMenu_HelixAmber, NELEMS(sScriptMultiChoiceMenu_HelixAmber) },
    [MULTICHOICE_DOME_AMBER]                                 = { sScriptMultiChoiceMenu_DomeAmber, NELEMS(sScriptMultiChoiceMenu_DomeAmber) },
    [MULTICHOICE_CELADON_VENDING_MACHINE]                    = { sScriptMultiChoiceMenu_CeladonVendingMachine, NELEMS(sScriptMultiChoiceMenu_CeladonVendingMachine) },
    [MULTICHOICE_GAME_CORNER_COIN_PURCHASE_COUNTER]          = { sScriptMultiChoiceMenu_GameCornerCoinPurchaseCounter, NELEMS(sScriptMultiChoiceMenu_GameCornerCoinPurchaseCounter) },
    [MULTICHOICE_EXCELLENT_NOT_SO_BAD]                       = { sScriptMultiChoiceMenu_Excellent_NotSoBad, NELEMS(sScriptMultiChoiceMenu_Excellent_NotSoBad) },
    [MULTICHOICE_RIGHT_LEFT]                                 = { sScriptMultiChoiceMenu_RightLeft, NELEMS(sScriptMultiChoiceMenu_RightLeft) },
    [MULTICHOICE_GAME_CORNER_TMPRIZES]                       = { sScriptMultiChoiceMenu_GameCornerTMPrizes, NELEMS(sScriptMultiChoiceMenu_GameCornerTMPrizes) },
    [MULTICHOICE_DEPT_STORE_ELEVATOR]                        = { sScriptMultiChoiceMenu_DeptStoreElevator, NELEMS(sScriptMultiChoiceMenu_DeptStoreElevator) },
    [MULTICHOICE_THIRSTY_GIRL_FRESH_WATER]                   = { sScriptMultiChoiceMenu_ThirstyGirlFreshWater, NELEMS(sScriptMultiChoiceMenu_ThirstyGirlFreshWater) },
    [MULTICHOICE_THIRSTY_GIRL_SODA_POP]                      = { sScriptMultiChoiceMenu_ThirstyGirlSodaPop, NELEMS(sScriptMultiChoiceMenu_ThirstyGirlSodaPop) },
    [MULTICHOICE_THIRSTY_GIRL_FRESH_WATER_SODA_POP]          = { sScriptMultiChoiceMenu_ThirstyGirlFreshWaterSodaPop, NELEMS(sScriptMultiChoiceMenu_ThirstyGirlFreshWaterSodaPop) },
    [MULTICHOICE_THIRSTY_GIRL_LEMONADE]                      = { sScriptMultiChoiceMenu_ThirstyGirlLemonade, NELEMS(sScriptMultiChoiceMenu_ThirstyGirlLemonade) },
    [MULTICHOICE_THIRSTY_GIRL_FRESH_WATER_LEMONADE]          = { sScriptMultiChoiceMenu_ThirstyGirlFreshWaterLemonade, NELEMS(sScriptMultiChoiceMenu_ThirstyGirlFreshWaterLemonade) },
    [MULTICHOICE_THIRSTY_GIRL_SODA_POP_LEMONADE]             = { sScriptMultiChoiceMenu_ThirstyGirlSodaPopLemonade, NELEMS(sScriptMultiChoiceMenu_ThirstyGirlSodaPopLemonade) },
    [MULTICHOICE_THIRSTY_GIRL_FRESH_WATER_SODA_POP_LEMONADE] = { sScriptMultiChoiceMenu_ThirstyGirlFreshWaterSodaPopLemonade, NELEMS(sScriptMultiChoiceMenu_ThirstyGirlFreshWaterSodaPopLemonade) },
    [MULTICHOICE_TRADE_CENTER_COLOSSEUM]                     = { sScriptMultiChoiceMenu_TradeCenter_Colosseum, NELEMS(sScriptMultiChoiceMenu_TradeCenter_Colosseum) },
    [MULTICHOICE_LINK_WIRELESS]                              = { sScriptMultiChoiceMenu_Link_Wireless, NELEMS(sScriptMultiChoiceMenu_Link_Wireless) },
    [MULTICHOICE_GAME_CORNER_BATTLE_ITEM_PRIZES]             = { sScriptMultiChoiceMenu_GameCornerBattleItemPrizes, NELEMS(sScriptMultiChoiceMenu_GameCornerBattleItemPrizes) },
    [MULTICHOICE_ROCKET_HIDEOUT_ELEVATOR]                    = { sScriptMultiChoiceMenu_RocketHideoutElevator, NELEMS(sScriptMultiChoiceMenu_RocketHideoutElevator) },
    [MULTICHOICE_LINKED_DIRECT_UNION]                        = { sScriptMultiChoiceMenu_LinkedDirectUnion, NELEMS(sScriptMultiChoiceMenu_LinkedDirectUnion) },
    [MULTICHOICE_ISLAND_23]                                  = { sScriptMultiChoiceMenu_Island23, NELEMS(sScriptMultiChoiceMenu_Island23) },
    [MULTICHOICE_ISLAND_13]                                  = { sScriptMultiChoiceMenu_Island13, NELEMS(sScriptMultiChoiceMenu_Island13) },
    [MULTICHOICE_ISLAND_12]                                  = { sScriptMultiChoiceMenu_Island12, NELEMS(sScriptMultiChoiceMenu_Island12) },
    [MULTICHOICE_TRADE_COLOSSEUM_CRUSH]                      = { sScriptMultiChoiceMenu_TradeColosseumCrush, NELEMS(sScriptMultiChoiceMenu_TradeColosseumCrush) },
    [MULTICHOICE_48]                                         = { sScriptMultiChoiceMenu_48, NELEMS(sScriptMultiChoiceMenu_48) },
    [MULTICHOICE_POKEJUMP_DODRIO]                            = { sScriptMultiChoiceMenu_PokejumpDodrio, NELEMS(sScriptMultiChoiceMenu_PokejumpDodrio) },
    [MULTICHOICE_TRADE_COLOSSEUM_2]                          = { sScriptMultiChoiceMenu_TradeColosseum_2, NELEMS(sScriptMultiChoiceMenu_TradeColosseum_2) },
    [MULTICHOICE_MUSHROOMS]                                  = { sScriptMultiChoiceMenu_Mushrooms, NELEMS(sScriptMultiChoiceMenu_Mushrooms) },
    [MULTICHOICE_TRADE_COLOSSEUM_BLANK_CRUSH]                = { sScriptMultiChoiceMenu_TradeColosseumBlankCrush, NELEMS(sScriptMultiChoiceMenu_TradeColosseumBlankCrush) },
    [MULTICHOICE_TRADE_COLOSSEUM_BLANK]                      = { sScriptMultiChoiceMenu_TradeColosseumBlank, NELEMS(sScriptMultiChoiceMenu_TradeColosseumBlank) },
    [MULTICHOICE_SEVII_NAVEL]                                = { sScriptMultiChoiceMenu_SeviiNavel, NELEMS(sScriptMultiChoiceMenu_SeviiNavel) },
    [MULTICHOICE_SEVII_BIRTH]                                = { sScriptMultiChoiceMenu_SeviiBirth, NELEMS(sScriptMultiChoiceMenu_SeviiBirth) },
    [MULTICHOICE_SEVII_NAVEL_BIRTH]                          = { sScriptMultiChoiceMenu_SeviiNavelBirth, NELEMS(sScriptMultiChoiceMenu_SeviiNavelBirth) },
    [MULTICHOICE_SEAGALLOP_123]                              = { sScriptMultiChoiceMenu_Seagallop123, NELEMS(sScriptMultiChoiceMenu_Seagallop123) },
    [MULTICHOICE_SEAGALLOP_V23]                              = { sScriptMultiChoiceMenu_SeagallopV23, NELEMS(sScriptMultiChoiceMenu_SeagallopV23) },
    [MULTICHOICE_SEAGALLOP_V13]                              = { sScriptMultiChoiceMenu_SeagallopV13, NELEMS(sScriptMultiChoiceMenu_SeagallopV13) },
    [MULTICHOICE_SEAGALLOP_V12]                              = { sScriptMultiChoiceMenu_SeagallopV12, NELEMS(sScriptMultiChoiceMenu_SeagallopV12) },
    [MULTICHOICE_SEAGALLOP_VERMILION]                        = { sScriptMultiChoiceMenu_SeagallopVermilion, NELEMS(sScriptMultiChoiceMenu_SeagallopVermilion) },
    [MULTICHOICE_62]                                         = { sScriptMultiChoiceMenu_62, NELEMS(sScriptMultiChoiceMenu_62) },
    [MULTICHOICE_JOIN_OR_LEAD]                               = { sScriptMultiChoiceMenu_JoinOrLead, NELEMS(sScriptMultiChoiceMenu_JoinOrLead) },
    [MULTICHOICE_TRAINER_TOWER_MODE]                         = { sScriptMultiChoiceMenu_TrainerTowerMode, NELEMS(sScriptMultiChoiceMenu_TrainerTowerMode) }
};

// From Cool to Berries goes unused
const u8 *const gStdStringPtrs[] = {
    [STDSTRING_COOL]             = gText_Cool,
    [STDSTRING_BEAUTY]           = gText_Beauty,
    [STDSTRING_CUTE]             = gText_Cute,
    [STDSTRING_SMART]            = gText_Smart,
    [STDSTRING_TOUGH]            = gText_Tough,
    [STDSTRING_COOL2]            = gText_Cool_2,
    [STDSTRING_BEAUTY2]          = gText_Beauty_2,
    [STDSTRING_CUTE2]            = gText_Cute_2,
    [STDSTRING_SMART2]           = gText_Smart_2,
    [STDSTRING_TOUGH2]           = gText_Tough_2,
    [STDSTRING_ITEMS]            = gText_Items,
    [STDSTRING_KEY_ITEMS]        = gText_KeyItems,
    [STDSTRING_POKEBALLS]        = gText_PokeBalls,
    [STDSTRING_TMHMS]            = gText_TMsAndHMs,
    [STDSTRING_BERRIES]          = gText_Berries,
    [STDSTRING_BOULDER_BADGE]    = gText_Boulderbadge,
    [STDSTRING_CASCADE_BADGE]    = gText_Cascadebadge,
    [STDSTRING_THUNDER_BADGE]    = gText_Thunderbadge,
    [STDSTRING_RAINBOW_BADGE]    = gText_Rainbowbadge,
    [STDSTRING_SOUL_BADGE]       = gText_Soulbadge,
    [STDSTRING_MARSH_BADGE]      = gText_Marshbadge,
    [STDSTRING_VOLCANO_BADGE]    = gText_Volcanobadge,
    [STDSTRING_EARTH_BADGE]      = gText_Earthbadge,
    [STDSTRING_COINS]            = gText_Coins_2,
    [STDSTRING_ITEMS_POCKET]     = gText_ItemsPocket,
    [STDSTRING_KEY_ITEMS_POCKET] = gText_KeyItemsPocket,
    [STDSTRING_POKEBALLS_POCKET] = gText_PokeBallsPocket,
    [STDSTRING_TM_CASE]          = gText_TmCase,
    [STDSTRING_BERRY_POUCH]      = gText_BerryPouch_2
};

static const u8 *const sDescriptionPtrs_CableClub_TradeBattleCancel[] = {
    CableClub_Text_TradeMonsUsingLinkCable,
    CableClub_Text_BattleUsingLinkCable,
    CableClub_Text_CancelSelectedItem
};

static const u8 *const sDescriptionPtrs_WirelessCenter_TradeBattleCrushCancel[] = {
    CableClub_Text_YouMayTradeHere,
    CableClub_Text_YouMayBattleHere,
    CableClub_Text_CanMakeBerryPowder,
    CableClub_Text_CancelSelectedItem
};

static const u8 *const sDescriptionPtrs_WirelessCenter_TradeBattleCancel[] = {
    CableClub_Text_YouMayTradeHere,
    CableClub_Text_YouMayBattleHere,
    CableClub_Text_CancelSelectedItem
};

static const union AnimCmd sMuseumFossilAnim0[] = {
    ANIMCMD_FRAME(0, 10),
    ANIMCMD_END
};

static const union AnimCmd *const sMuseumFossilAnimCmdTable[] = {
    sMuseumFossilAnim0
};

static const struct OamData sMuseumFossilOamData = {
    .shape = SPRITE_SHAPE(64x64),
    .size = SPRITE_SIZE(64x64)
};

static const struct SpriteTemplate sMuseumFossilSprTemplate = {
    .tileTag = 7000,
    .paletteTag = 0xFFFF,
    .oam = &sMuseumFossilOamData,
    .anims = sMuseumFossilAnimCmdTable,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

static const u16 sMuseumAerodactylSprTiles[] = INCBIN_U16("graphics/script_menu/aerodactyl_fossil.4bpp");
static const u16 sMuseumAerodactylSprPalette[] = INCBIN_U16("graphics/script_menu/aerodactyl_fossil.gbapal");
static const u16 sMuseumKabutopsSprTiles[] = INCBIN_U16("graphics/script_menu/kabutops_fossil.4bpp");
static const u16 sMuseumKabutopsSprPalette[] = INCBIN_U16("graphics/script_menu/kabutops_fossil.gbapal");

static const struct SpriteSheet sMuseumKabutopsSprSheets[] = {
    {sMuseumKabutopsSprTiles, 0x800, 7000},
    {}
};

static const struct SpriteSheet sMuseumAerodactylSprSheets[] = {
    {sMuseumAerodactylSprTiles, 0x800, 7000},
    {}
};


static const u8 *const sSeagallopDestStrings[] = {
    gText_Vermilion,
    gText_OneIsland,
    gText_TwoIsland,
    gText_ThreeIsland,
    gText_FourIsland,
    gText_FiveIsland,
    gText_SixIsland,
    gText_SevenIsland
};

static u16 GetStringTilesWide(const u8 *str)
{
    return (GetStringWidth(1, str, 0) + 7) / 8;
}

static u8 GetMenuWidthFromList(const struct MenuAction * items, u8 count)
{
    u16 i;
    u8 width = GetStringTilesWide(items[0].text);
    u8 tmp;

    for (i = 1; i < count; i++)
    {
        tmp = GetStringTilesWide(items[i].text);
        if (width < tmp)
            width = tmp;
    }
    return width;
}

bool8 ScriptMenu_Multichoice(u8 left, u8 top, u8 mcId, u8 ignoreBpress)
{
    if (FuncIsActiveTask(Task_MultichoiceMenu_HandleInput) == TRUE)
        return FALSE;
    gSpecialVar_Result = SCR_MENU_UNSET;
    DrawVerticalMultichoiceMenu(left, top, mcId, ignoreBpress, 0);
    return TRUE;
}

bool8 ScriptMenu_MultichoiceWithDefault(u8 left, u8 top, u8 mcId, u8 ignoreBpress, u8 cursorPos)
{
    if (FuncIsActiveTask(Task_MultichoiceMenu_HandleInput) == TRUE)
        return FALSE;
    gSpecialVar_Result = SCR_MENU_UNSET;
    DrawVerticalMultichoiceMenu(left, top, mcId, ignoreBpress, cursorPos);
    return TRUE;
}

static void DrawVerticalMultichoiceMenu(u8 left, u8 top, u8 mcId, u8 ignoreBpress, u8 initPos)
{
    s32 i;
    s32 strWidth;
    s32 tmp;
    u8 width;
    u8 height;
    u8 count;
    u8 windowId;
    const struct MenuAction * list;

    if ((ignoreBpress & 2) || QuestLog_SchedulePlaybackCB(QLPlaybackCB_DestroyScriptMenuMonPicSprites) != TRUE)
    {
        ignoreBpress &= 1;
        count = gScriptMultiChoiceMenus[mcId].count;
        list = gScriptMultiChoiceMenus[mcId].list;
        strWidth = 0;
        for (i = 0; i < count; i++)
        {
            tmp = GetStringWidth(2, list[i].text, 0);
            if (tmp > strWidth)
                strWidth = tmp;
        }
        width = (strWidth + 9) / 8 + 1;
        if (left + width > 28)
            left = 28 - width;
        height = GetMCWindowHeight(count);
        windowId = CreateWindowFromRect(left, top, width, height);
        SetStdWindowBorderStyle(windowId, FALSE);
        if (mcId == MULTICHOICE_GAME_CORNER_TMPRIZES || mcId == MULTICHOICE_BIKE_SHOP || mcId == MULTICHOICE_GAME_CORNER_BATTLE_ITEM_PRIZES)
            MultichoiceList_PrintItems(windowId, 2, 8, 2, 14, count, list, 0, 2);
        else
            MultichoiceList_PrintItems(windowId, 2, 8, 2, 14, count, list, 0, 2);
        InitMenuInUpperLeftCornerPlaySoundWhenAPressed(windowId, 2, 0, 2, 14, count, initPos);
        CreateMCMenuInputHandlerTask(ignoreBpress, count, windowId, mcId);
        ScheduleBgCopyTilemapToVram(0);
    }
}

static u8 GetMCWindowHeight(u8 count)
{
    switch (count)
    {
    case 0:
        return 1;
    case 1:
        return 2;
    case 2:
        return 4;
    case 3:
        return 6;
    case 4:
        return 7;
    case 5:
        return 9;
    case 6:
        return 11;
    case 7:
        return 13;
    case 8:
        return 14;
    default:
        return 1;
    }
}

#define tIgnoreBPress      data[4]
#define tWrapsAround       data[5]
#define tWindowId          data[6]
#define tMultichoiceId     data[7]

static void CreateMCMenuInputHandlerTask(u8 ignoreBpress, u8 count, u8 windowId, u8 mcId)
{
    u8 taskId;
    if (mcId == MULTICHOICE_TRADE_CENTER_COLOSSEUM || mcId == MULTICHOICE_TRADE_COLOSSEUM_CRUSH || mcId == MULTICHOICE_TRADE_COLOSSEUM_2)
        sDelay = 12;
    else
        sDelay = 0;
    taskId = CreateTask(Task_MultichoiceMenu_HandleInput, 80);
    gTasks[taskId].tIgnoreBPress = ignoreBpress;
    if (count > 3)
        gTasks[taskId].tWrapsAround = TRUE;
    else
        gTasks[taskId].tWrapsAround = FALSE;
    gTasks[taskId].tWindowId = windowId;
    gTasks[taskId].tMultichoiceId = mcId;
    MultiChoicePrintHelpDescription(mcId);
}

static void Task_MultichoiceMenu_HandleInput(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    s8 input;
    if (!gPaletteFade.active)
    {
        if (sDelay != 0)
            sDelay--;
        else
        {
            if (tWrapsAround == FALSE)
                input = Menu_ProcessInputNoWrapAround();
            else
                input = Menu_ProcessInput();
            if (JOY_NEW(DPAD_UP | DPAD_DOWN))
                MultiChoicePrintHelpDescription(tMultichoiceId);
            switch (input)
            {
            case MENU_NOTHING_CHOSEN:
                return;
            case MENU_B_PRESSED:
                if (tIgnoreBPress)
                    return;
                PlaySE(SE_SELECT);
                gSpecialVar_Result = SCR_MENU_CANCEL;
                break;
            default:
                gSpecialVar_Result = input;
                break;
            }
            DestroyScriptMenuWindow(tWindowId);
            DestroyTask(taskId);
            EnableBothScriptContexts();
        }
    }
}

#undef tIgnoreBPress
#undef tWrapsAround
#undef tWindowId
#undef tMultichoiceId

static void MultiChoicePrintHelpDescription(u8 mcId)
{
    switch (mcId)
    {
    case MULTICHOICE_TRADE_CENTER_COLOSSEUM:
        FillWindowPixelBuffer(0, PIXEL_FILL(1));
        AddTextPrinterParameterized2(0, 2, sDescriptionPtrs_CableClub_TradeBattleCancel[Menu_GetCursorPos()], 0, NULL, 2, 1, 3);
        break;
    case MULTICHOICE_TRADE_COLOSSEUM_CRUSH:
        FillWindowPixelBuffer(0, PIXEL_FILL(1));
        AddTextPrinterParameterized2(0, 2, sDescriptionPtrs_WirelessCenter_TradeBattleCrushCancel[Menu_GetCursorPos()], 0, NULL, 2, 1, 3);
        break;
    case MULTICHOICE_TRADE_COLOSSEUM_2:
        FillWindowPixelBuffer(0, PIXEL_FILL(1));
        AddTextPrinterParameterized2(0, 2, sDescriptionPtrs_WirelessCenter_TradeBattleCancel[Menu_GetCursorPos()], 0, NULL, 2, 1, 3);
        break;
    }
}

bool8 ScriptMenu_YesNo(u8 unused, u8 stuff)
{
    if (FuncIsActiveTask(Task_YesNoMenu_HandleInput) == TRUE)
        return FALSE;
    gSpecialVar_Result = SCR_MENU_UNSET;
    if (!QuestLog_SchedulePlaybackCB(QLPlaybackCB_DestroyScriptMenuMonPicSprites))
    {
        DisplayYesNoMenuDefaultYes();
        CreateTask(Task_YesNoMenu_HandleInput, 80);
    }
    return TRUE;
}

bool8 sub_809CE38(void)
{
    if (gSpecialVar_Result == SCR_MENU_UNSET)
        return FALSE;
    else
        return TRUE;
}

static void Task_YesNoMenu_HandleInput(u8 taskId)
{
    s8 input;
    if (gTasks[taskId].data[2] < 5)
    {
        gTasks[taskId].data[2]++;
    }
    else
    {
        input = Menu_ProcessInputNoWrapClearOnChoose();
        switch (input)
        {
        case -2:
            return;
        case -1:
        case 1:
            PlaySE(SE_SELECT);
            gSpecialVar_Result = FALSE;
            break;
        case 0:
            gSpecialVar_Result = TRUE;
            break;
        }
        DestroyTask(taskId);
        EnableBothScriptContexts();
    }
}

bool8 ScriptMenu_MultichoiceGrid(u8 left, u8 top, u8 multichoiceId, u8 a4, u8 columnCount)
{
    const struct MenuAction * list;
    u8 count;
    u8 width;
    u8 rowCount;
    u8 taskId;
    if (FuncIsActiveTask(Hask_MultichoiceGridMenu_HandleInput) == TRUE)
        return FALSE;
    gSpecialVar_Result = SCR_MENU_UNSET;
    if (QuestLog_SchedulePlaybackCB(QLPlaybackCB_DestroyScriptMenuMonPicSprites) != TRUE)
    {
        list = gScriptMultiChoiceMenus[multichoiceId].list;
        count = gScriptMultiChoiceMenus[multichoiceId].count;
        width = GetMenuWidthFromList(list, count) + 1;
        rowCount = count / columnCount;
        taskId = CreateTask(Hask_MultichoiceGridMenu_HandleInput, 80);
        gTasks[taskId].data[4] = a4;
        gTasks[taskId].data[6] = CreateWindowFromRect(left, top, width * columnCount, rowCount * 2);
        SetStdWindowBorderStyle(gTasks[taskId].data[6], FALSE);
        MultichoiceGrid_PrintItems(gTasks[taskId].data[6], 1, width * 8, 16, columnCount, rowCount, list);
        MultichoiceGrid_InitCursor(gTasks[taskId].data[6], 1, 0, 1, width * 8, columnCount, rowCount, 0);
        ScheduleBgCopyTilemapToVram(0);
    }
    return TRUE;
}

static void Hask_MultichoiceGridMenu_HandleInput(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    s8 input = Menu_ProcessInputGridLayout();
    switch (input)
    {
    case -2:
        return;
    case -1:
        if (data[4])
            return;
        PlaySE(SE_SELECT);
        gSpecialVar_Result = SCR_MENU_CANCEL;
        break;
    default:
        gSpecialVar_Result = input;
        break;
    }
    DestroyScriptMenuWindow(data[6]);
    DestroyTask(taskId);
    EnableBothScriptContexts();
}

bool8 CreatePCMenu(void)
{
    if (FuncIsActiveTask(Task_MultichoiceMenu_HandleInput) == TRUE)
        return FALSE;
    gSpecialVar_Result = SCR_MENU_UNSET;
    CreatePCMenuWindow();
    return TRUE;
}

static void CreatePCMenuWindow(void)
{
    u8 cursorWidth = GetMenuCursorDimensionByFont(2, 0);
    u8 height = GetFontAttribute(2, FONTATTR_MAX_LETTER_HEIGHT);
    u8 windowWidth;
    u8 nitems;
    u8 windowId;
    switch (GetStringTilesWide(gText_SPc))
    {
    default:
        if (FlagGet(FLAG_SYS_POKEDEX_GET))
            windowWidth = 14;
        else
            windowWidth = 13;
        break;
    case 9:
    case 10:
        windowWidth = 14;
        break;
    }
    if (FlagGet(FLAG_SYS_GAME_CLEAR))
    {
        nitems = 5;
        windowId = CreateWindowFromRect(0, 0, windowWidth, 10);
        SetStdWindowBorderStyle(windowId, FALSE);
        AddTextPrinterParameterized(windowId, 2, gText_ProfOakSPc, cursorWidth, 34, 0xFF, NULL);
        AddTextPrinterParameterized(windowId, 2, gText_HallOfFame_2, cursorWidth, 50, 0xFF, NULL);
        AddTextPrinterParameterized(windowId, 2, gText_LogOff, cursorWidth, 66, 0xFF, NULL);
    }
    else
    {
        if (FlagGet(FLAG_SYS_POKEDEX_GET))
            nitems = 4;
        else
            nitems = 3;
        windowId = CreateWindowFromRect(0, 0, windowWidth, nitems * 2);
        SetStdWindowBorderStyle(windowId, FALSE);
        if (FlagGet(FLAG_SYS_POKEDEX_GET))
            AddTextPrinterParameterized(windowId, 2, gText_ProfOakSPc, cursorWidth, 34, 0xFF, NULL);
        AddTextPrinterParameterized(windowId, 2, gText_LogOff, cursorWidth, 2 + 16 * (nitems - 1), 0xFF, NULL);
    }
    if (FlagGet(FLAG_SYS_NOT_SOMEONES_PC))
        AddTextPrinterParameterized(windowId, 2, gText_BillSPc, cursorWidth, 2 , 0xFF, NULL);
    else
        AddTextPrinterParameterized(windowId, 2, gText_SomeoneSPc, cursorWidth, 2 , 0xFF, NULL);
    StringExpandPlaceholders(gStringVar4, gText_SPc);
    Menu_PrintFormatIntlPlayerName(windowId, gStringVar4, cursorWidth, 18);
    InitMenuInUpperLeftCornerPlaySoundWhenAPressed(windowId, 2, 0, 2, 16, nitems, 0);
    CreateMCMenuInputHandlerTask(FALSE, nitems, windowId, 0xFF);
    ScheduleBgCopyTilemapToVram(0);
}

void ScriptMenu_DisplayPCStartupPrompt(void)
{
    SetDlgWindowBorderStyle(DLG_WINDOW_ID, TRUE);
    AddTextPrinterParameterized2(DLG_WINDOW_ID, 2, Text_AccessWhichPC, 0, NULL, 2, 1, 3);
}

static void Task_ScriptShowMonPic(u8 taskId)
{
    struct Task * task = &gTasks[taskId];
    switch (task->data[0])
    {
    case 0:
        task->data[0]++;
        break;
    case 1:
        break;
    case 2:
        FreeResourcesAndDestroySprite(&gSprites[task->data[2]], task->data[2]);
        task->data[0]++;
        break;
    case 3:
        DestroyScriptMenuWindow(task->data[5]);
        DestroyTask(taskId);
        break;
    }
}

bool8 ScriptMenu_ShowPokemonPic(u16 species, u8 x, u8 y)
{
    u8 spriteId;
    u8 taskId;
    if (QuestLog_SchedulePlaybackCB(QLPlaybackCB_DestroyScriptMenuMonPicSprites) == TRUE)
        return TRUE;
    if (FindTaskIdByFunc(Task_ScriptShowMonPic) != 0xFF)
        return FALSE;
    spriteId = CreateMonSprite_PicBox(species, 8 * x + 40, 8 * y + 40, FALSE);
    taskId = CreateTask(Task_ScriptShowMonPic, 80);
    gTasks[taskId].data[5] = CreateWindowFromRect(x, y, 8, 8);
    gTasks[taskId].data[0] = 0;
    gTasks[taskId].data[1] = species;
    gTasks[taskId].data[2] = spriteId;
    gSprites[spriteId].callback = SpriteCallbackDummy;
    gSprites[spriteId].oam.priority = 0;
    SetStdWindowBorderStyle(gTasks[taskId].data[5], TRUE);
    ScheduleBgCopyTilemapToVram(0);
    return TRUE;
}

bool8 (*ScriptMenu_GetPicboxWaitFunc(void))(void)
{
    u8 taskId = FindTaskIdByFunc(Task_ScriptShowMonPic);
    if (taskId == 0xFF)
        return NULL;
    gTasks[taskId].data[0]++;
    return PicboxWait;
}

static bool8 PicboxWait(void)
{
    if (FindTaskIdByFunc(Task_ScriptShowMonPic) == 0xFF)
        return TRUE;
    else
        return FALSE;
}

void PicboxCancel(void)
{
    u8 taskId = FindTaskIdByFunc(Task_ScriptShowMonPic);
    struct Task * task;
    if (taskId != 0xFF)
    {
        task = &gTasks[taskId];
        switch (task->data[0])
        {
        case 0:
        case 1:
        case 2:
            FreeResourcesAndDestroySprite(&gSprites[task->data[2]], task->data[2]);
            DestroyScriptMenuWindow(task->data[5]);
            DestroyTask(taskId);
            break;
        case 3:
            DestroyScriptMenuWindow(task->data[5]);
            DestroyTask(taskId);
            break;
        }
    }
}

void Task_WaitMuseumFossilPic(u8 taskId)
{
    struct Task * task = &gTasks[taskId];
    switch (task->data[0])
    {
    case 0:
        task->data[0]++;
        break;
    case 1:
        break;
    case 2:
        DestroySprite(&gSprites[task->data[2]]);
        FreeSpriteTilesByTag(7000);
        task->data[0]++;
        break;
    case 3:
        DestroyScriptMenuWindow(task->data[5]);
        DestroyTask(taskId);
        break;
    }
}

bool8 OpenMuseumFossilPic(void)
{
    u8 spriteId;
    u8 taskId;
    if (QuestLog_SchedulePlaybackCB(QLPlaybackCB_DestroyScriptMenuMonPicSprites) == TRUE)
        return TRUE;
    if (FindTaskIdByFunc(Task_WaitMuseumFossilPic) != 0xFF)
        return FALSE;
    if (gSpecialVar_0x8004 == SPECIES_KABUTOPS)
    {
        LoadSpriteSheets(sMuseumKabutopsSprSheets);
        LoadPalette(sMuseumKabutopsSprPalette, 0x1D0, 0x20);
    }
    else if (gSpecialVar_0x8004 == SPECIES_AERODACTYL)
    {
        LoadSpriteSheets(sMuseumAerodactylSprSheets);
        LoadPalette(sMuseumAerodactylSprPalette, 0x1D0, 0x20);
    }
    else
    {
        return FALSE;
    }
    spriteId = CreateSprite(&sMuseumFossilSprTemplate, gSpecialVar_0x8005 * 8 + 40, gSpecialVar_0x8006 * 8 + 40, 0);
    gSprites[spriteId].oam.paletteNum = 13;
    taskId = CreateTask(Task_WaitMuseumFossilPic, 80);
    gTasks[taskId].data[5] = CreateWindowFromRect(gSpecialVar_0x8005, gSpecialVar_0x8006, 8, 8);
    gTasks[taskId].data[0] = 0;
    gTasks[taskId].data[2] = spriteId;
    SetStdWindowBorderStyle(gTasks[taskId].data[5], TRUE);
    ScheduleBgCopyTilemapToVram(0);
    return TRUE;
}

bool8 CloseMuseumFossilPic(void)
{
    u8 taskId = FindTaskIdByFunc(Task_WaitMuseumFossilPic);
    if (taskId == 0xFF)
        return FALSE;
    gTasks[taskId].data[0]++;
    return TRUE;
}

static u8 CreateWindowFromRect(u8 left, u8 top, u8 width, u8 height)
{
    struct WindowTemplate template = SetWindowTemplateFields(0, left + 1, top + 1, width, height, 15, 0x038);
    u8 windowId = AddWindow(&template);
    PutWindowTilemap(windowId);
    return windowId;
}

static void DestroyScriptMenuWindow(u8 windowId)
{
    ClearWindowTilemap(windowId);
    ClearStdWindowAndFrameToTransparent(windowId, TRUE);
    RemoveWindow(windowId);
}

void QLPlaybackCB_DestroyScriptMenuMonPicSprites(void)
{
    u8 taskId;
    s16 *data;
    ScriptContext1_SetupScript(EventScript_ReleaseEnd);
    taskId = FindTaskIdByFunc(Task_ScriptShowMonPic);
    if (taskId != 0xFF)
    {
        data = gTasks[taskId].data;
        if (data[0] < 2)
            FreeResourcesAndDestroySprite(&gSprites[data[2]], data[2]);
    }
    taskId = FindTaskIdByFunc(Task_WaitMuseumFossilPic);
    if (taskId != 0xFF)
    {
        data = gTasks[taskId].data;
        if (data[0] < 2)
        {
            DestroySprite(&gSprites[data[2]]);
            FreeSpriteTilesByTag(7000);
        }
    }
}

void DrawSeagallopDestinationMenu(void)
{
    // 8004 = Starting location
    // 8005 = Page (0: Verm, One, Two, Three, Four, Other, Exit; 1: Four, Five, Six, Seven, Other, Exit)
    u8 r4;
    u8 top;
    u8 nitems;
    u8 cursorWidth;
    u8 fontHeight;
    u8 windowId;
    u8 i;
    gSpecialVar_Result = SCR_MENU_UNSET;
    if (QuestLog_SchedulePlaybackCB(QLPlaybackCB_DestroyScriptMenuMonPicSprites) != TRUE)
    {
        if (gSpecialVar_0x8005 == 1)
        {
            if (gSpecialVar_0x8004 < SEAGALLOP_FIVE_ISLAND)
                r4 = SEAGALLOP_FIVE_ISLAND;
            else
                r4 = SEAGALLOP_FOUR_ISLAND;
            nitems = 5;
            top = 2;
        }
        else
        {
            r4 = SEAGALLOP_VERMILION_CITY;
            nitems = 6;
            top = 0;
        }
        cursorWidth = GetMenuCursorDimensionByFont(2, 0);
        fontHeight = GetFontAttribute(2, FONTATTR_MAX_LETTER_HEIGHT);
        windowId = CreateWindowFromRect(17, top, 11, nitems * 2);
        SetStdWindowBorderStyle(windowId, FALSE);
        for (i = 0; i < nitems - 2; i++)
        {
            if (r4 != gSpecialVar_0x8004)
                AddTextPrinterParameterized(windowId, 2, sSeagallopDestStrings[r4], cursorWidth, i * 16 + 2, 0xFF, NULL);
            else
                i--;
            r4++;
            if (r4 == SEAGALLOP_CINNABAR_ISLAND)
                r4 = SEAGALLOP_VERMILION_CITY;
        }
        AddTextPrinterParameterized(windowId, 2, gText_Other, cursorWidth, i * 16 + 2, 0xFF, NULL);
        i++;
        AddTextPrinterParameterized(windowId, 2, gOtherText_Exit, cursorWidth, i * 16 + 2, 0xFF, NULL);
        InitMenuInUpperLeftCornerPlaySoundWhenAPressed(windowId, 2, 0, 2, 16, nitems, 0);
        CreateMCMenuInputHandlerTask(FALSE, nitems, windowId, 0xFF);
        ScheduleBgCopyTilemapToVram(0);
    }
}

u16 GetSelectedSeagallopDestination(void)
{
    // 8004 = Starting location
    // 8005 = Page (0: Verm, One, Two, Three, Four, Other, Exit; 1: Four, Five, Six, Seven, Other, Exit)
    if (gSpecialVar_Result == SCR_MENU_CANCEL)
        return SCR_MENU_CANCEL;
    if (gSpecialVar_0x8005 == 1)
    {
        if (gSpecialVar_Result == 3)
        {
            return SEAGALLOP_MORE;
        }
        else if (gSpecialVar_Result == 4)
        {
            return SCR_MENU_CANCEL;
        }
        else if (gSpecialVar_Result == 0)
        {
            if (gSpecialVar_0x8004 > SEAGALLOP_FOUR_ISLAND)
                return SEAGALLOP_FOUR_ISLAND;
            else
                return SEAGALLOP_FIVE_ISLAND;
        }
        else if (gSpecialVar_Result == 1)
        {
            if (gSpecialVar_0x8004 > SEAGALLOP_FIVE_ISLAND)
                return SEAGALLOP_FIVE_ISLAND;
            else
                return SEAGALLOP_SIX_ISLAND;
        }
        else if (gSpecialVar_Result == 2)
        {
            if (gSpecialVar_0x8004 > SEAGALLOP_SIX_ISLAND)
                return SEAGALLOP_SIX_ISLAND;
            else
                return SEAGALLOP_SEVEN_ISLAND;
        }
    }
    else
    {
        if (gSpecialVar_Result == 4)
            return SEAGALLOP_MORE;
        else if (gSpecialVar_Result == 5)
            return SCR_MENU_CANCEL;
        else if (gSpecialVar_Result >= gSpecialVar_0x8004)
            return gSpecialVar_Result + 1;
        else
            return gSpecialVar_Result;
    }
    return SEAGALLOP_VERMILION_CITY;
}
