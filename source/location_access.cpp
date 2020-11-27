#include "location_access.hpp"
#include "settings.hpp"
#include "logic.hpp"
#include "item_location.hpp"
#include "item_list.hpp"
#include <unistd.h>
#include <vector>

using namespace Logic;
using namespace Settings;

bool DAY_NIGHT_CYCLE    = true;
bool NO_DAY_NIGHT_CYCLE = false;

//generic grotto event list
std::vector<EventPairing> grottoEvents = {
  EventPairing(&GossipStoneFairy, []{return GossipStoneFairy || CanSummonGossipFairy;}),
  EventPairing(&ButterflyFairy,   []{return ButterflyFairy   || (CanUse("Sticks"));}),
  EventPairing(&BugShrub,         []{return CanCutShrubs;}),
  EventPairing(&LoneFish,         []{return true;}),
};

//TODO: Update all advancement logic needs for every area
namespace Exits { //name, scene, hint, events, locations, exits, advancement items

  Exit Root = Exit("Root", "", "Link's Pocket", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  ItemLocationPairing(&LinksPocket, []{return true;})
                }, {
                  //Exits
                  ExitPairing::Both(&RootExits, []{return true;}) //is_starting_age || Time_Travel
  });

  Exit RootExits = Exit("Root Exits", "", "", NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  ExitPairing::Both(&KF_LinksHouse,    []{return IsChild;}),
                  ExitPairing::Both(&ToT_Main,         []{return (CanPlay(PreludeOfLight)   && CanLeaveForest) || IsAdult;}),
                  ExitPairing::Both(&SFM_Main,         []{return  CanPlay(MinuetOfForest);}),
                  ExitPairing::Both(&DMC_CentralLocal, []{return  CanPlay(BoleroOfFire)     && CanLeaveForest;}),
                  ExitPairing::Both(&LH_Main,          []{return  CanPlay(SerenadeOfWater)  && CanLeaveForest;}),
                  ExitPairing::Both(&GY_WarpPadRegion, []{return  CanPlay(NocturneOfShadow) && CanLeaveForest;}),
                  ExitPairing::Both(&Colossus_Main,    []{return  CanPlay(RequiemOfSpirit)  && CanLeaveForest;})
               }, {
                  //Advancement Needs
                  AdvancementPairing(A_ProgressiveOcarina, []{return AddedProgressiveOcarinas == 0;}),
                  AdvancementPairing(A_MinuetOfForest,     []{return CanLeaveForest && ProgressiveOcarina >= 1 && !SFM_Main.BothAges();}),
                  AdvancementPairing(A_BoleroOfFire,       []{return CanLeaveForest && ProgressiveOcarina >= 1;}),
                  AdvancementPairing(A_SerenadeOfWater,    []{return CanLeaveForest && ProgressiveOcarina >= 1 && !LH_Main.BothAges();}),
                  AdvancementPairing(A_RequiemOfSpirit,    []{return CanLeaveForest && ProgressiveOcarina >= 1;}),
                  AdvancementPairing(A_NocturneOfShadow,   []{return CanLeaveForest && ProgressiveOcarina >= 1;}),
                  AdvancementPairing(A_PreludeOfLight,     []{return CanLeaveForest && ProgressiveOcarina >= 1 && !ToT_Main.BothAges();}),
  });

  Exit KF_Main = Exit("Kokiri Forest", "Kokiri Forest", "Kokiri Forest", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&BeanPlantFairy,           []{return BeanPlantFairy   || (KF_Main.CanPlantBean() && CanPlay(SongOfStorms));}),
                  EventPairing(&GossipStoneFairy,         []{return GossipStoneFairy || CanSummonGossipFairyWithoutSuns;}),
                  EventPairing(&ShowedMidoSwordAndShield, []{return OpenForest == OPENFOREST_OPEN || (IsChild && KokiriSword && DekuShield);}),
                }, {
                  //Locations
                  ItemLocationPairing(&KF_KokiriSwordChest,  []{return IsChild;}),
                  ItemLocationPairing(&KF_GS_KnowItAllHouse, []{return IsChild && CanChildAttack && AtNight && (HasNightStart || CanLeaveForest || CanPlay(SunsSong));}),
                  ItemLocationPairing(&KF_GS_BeanPatch,      []{return CanPlantBugs && CanChildAttack;}),
                  ItemLocationPairing(&KF_GS_HouseOfTwins,   []{return IsAdult && AtNight && CanUse("Hookshot");}),
                }, {
                  //Exits
                  ExitPairing::Both(&KF_LinksHouse,       []{return true;}),
                  ExitPairing::Both(&KF_MidosHouse,       []{return true;}),
                  ExitPairing::Both(&KF_SariasHouse,      []{return true;}),
                  ExitPairing::Both(&KF_HouseOfTwins,     []{return true;}),
                  ExitPairing::Both(&KF_KnowItAllHouse,   []{return true;}),
                  ExitPairing::Both(&KF_KokiriShop,       []{return true;}),
                  ExitPairing::Both(&KF_OutsideDekuTree,  []{return IsAdult || ShowedMidoSwordAndShield;}),
                  ExitPairing::Both(&LW_Main,             []{return true;}),
                  ExitPairing::Both(&LW_BridgeFromForest, []{return CanLeaveForest;}),
                  ExitPairing::Both(&KF_StormsGrotto,     []{return CanOpenStormGrotto;})
               }, {
                  //Advancement Needs
                  AdvancementPairing(A_KokiriSword,        []{return IsChild;}),
                  AdvancementPairing(A_DekuShield,         []{return IsChild && Shopsanity && OpenForest != OPENFOREST_OPEN;}),
                  AdvancementPairing(A_SongOfStorms,       []{return ProgressiveOcarina >= 1;}),
});

  Exit KF_OutsideDekuTree = Exit("KF Outside Deku Tree", "Kokiri Forest", "Kokiri Forest", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&DekuBabaSticks, []{return DekuBabaSticks || ((IsAdult && !ShuffleDungeonEntrances) || KokiriSword || Boomerang);}),
                  EventPairing(&DekuBabaNuts,   []{return DekuBabaNuts   || ((IsAdult && !ShuffleDungeonEntrances) || KokiriSword || Slingshot || Sticks || HasExplosives || CanUse("Dins Fire"));}),
                }, {
                  //Locations
                  // ItemLocationPairing(&KF_DekuTreeGossipStoneLeft,  []{return true;}),
                  // ItemLocationPairing(&KF_DekuTreeGossipStoneRight, []{return true;})
                }, {
                  //Exits
                  ExitPairing::Both(&DekuTree_Lobby, []{return IsChild || (ShuffleDungeonEntrances && ShowedMidoSwordAndShield);}),
                  ExitPairing::Both(&KF_Main,        []{return true;}),
  });

  Exit KF_LinksHouse = Exit("KF Link's House", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  //ItemLocationPairing(&KF_LinksHouseCow, []{return IsAdult && CanPlay(EponasSong) && LinksCow;})
                }, {
                  //Exits
                  ExitPairing::Both(&KF_Main, []{return true;})
  });

  Exit KF_MidosHouse = Exit("KF Mido's House", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  ItemLocationPairing(&KF_MidoTopLeftChest,     []{return true;}),
                  ItemLocationPairing(&KF_MidoTopRightChest,    []{return true;}),
                  ItemLocationPairing(&KF_MidoBottomLeftChest,  []{return true;}),
                  ItemLocationPairing(&KF_MidoBottomRightChest, []{return true;}),
                }, {
                  //Exits
                  ExitPairing::Both(&KF_Main, []{return true;}),
  });

  Exit KF_SariasHouse = Exit("KF Saria's House", "", "", NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  ExitPairing::Both(&KF_Main, []{return true;}),
  });

  Exit KF_HouseOfTwins = Exit("KF House of Twins", "", "", NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  ExitPairing::Both(&KF_Main, []{return true;}),
  });

  Exit KF_KnowItAllHouse = Exit("KF Know It All House", "", "", NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  ExitPairing::Both(&KF_Main, []{return true;}),
  });

  Exit KF_KokiriShop = Exit("KF Kokiri Shop", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  // ItemLocationPairing(&KF_KokiriShopItem1, []{return true;}),
                  // ItemLocationPairing(&KF_KokiriShopItem2, []{return true;}),
                  // ItemLocationPairing(&KF_KokiriShopItem3, []{return true;}),
                  // ItemLocationPairing(&KF_KokiriShopItem4, []{return true;}),
                  // ItemLocationPairing(&KF_KokiriShopItem5, []{return true;}),
                  // ItemLocationPairing(&KF_KokiriShopItem6, []{return true;}),
                  // ItemLocationPairing(&KF_KokiriShopItem7, []{return true;}),
                  // ItemLocationPairing(&KF_KokiriShopItem8, []{return true;}),
                }, {
                  //Exits
                  ExitPairing::Both(&KF_Main, []{return true;}),
  });

  Exit KF_StormsGrotto = Exit("KF Storms Grotto", "", "", NO_DAY_NIGHT_CYCLE, grottoEvents, {
                  //Locations
                  ItemLocationPairing(&KF_StormsGrottoChest,       []{return true;})
                  // ItemLocationPairing(&KF_StormsGrottoGossipStone, []{return true;}),
                }, {
                  //Exits
                  ExitPairing::Both(&KF_Main, []{return true;})
  });

  Exit LW_ForestExit = Exit("LW Forest Exit", "Lost Woods", "", NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  ExitPairing::Both(&KF_Main, []{return true;})
  });

  Exit LW_Main = Exit("Lost Woods", "Lost Woods", "the Lost Woods", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  //OddMushroomAccess
                  //PoachersSawAccess
                  EventPairing(&GossipStoneFairy, []{return GossipStoneFairy || CanSummonGossipFairyWithoutSuns;}),
                  EventPairing(&BeanPlantFairy,   []{return BeanPlantFairy   || CanPlay(SongOfStorms);}),
                  EventPairing(&BugShrub,         []{return IsChild && CanCutShrubs;}),
                }, {
                  //Locations
                  ItemLocationPairing(&LW_SkullKid,               []{return IsChild && CanPlay(SariasSong);}),
                //ItemLocationPairing(&LW_OcarinaMemoryGame,      []{return IsChild && Ocarina;}),
                  ItemLocationPairing(&LW_TargetInWoods,          []{return CanUse("Slingshot");}),
                //ItemLocationPairing(&LW_DekuScrubNearBridge,    []{return IsChild && CanStunDeku;}),
                  ItemLocationPairing(&LW_GS_BeanPatchNearBridge, []{return CanPlantBugs && CanChildAttack;}),
                  //LW Gossip Stone
                }, {
                  //Exits
                  ExitPairing::Both(&LW_ForestExit, []{return true;}),
                  ExitPairing::Both(&GC_WoodsWarp,  []{return true;}),
                  ExitPairing::Both(&LW_Bridge,     []{return IsAdult && (CanUse("Hover Boots") || CanUse("Longshot") || LW_Main.CanPlantBean() || LogicLostWoodsBridge);}),
                  ExitPairing::Both(&ZR_Main,       []{return CanLeaveForest && (CanDive || CanUse("Iron Boots"));}),
                  ExitPairing::Both(&LW_BeyondMido, []{return IsChild || CanPlay(SariasSong);}),
                  ExitPairing::Both(&LW_NearShortcutsGrotto, []{return CanBlastOrSmash;})
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_ProgressiveBulletBag, []{return AddedProgressiveBulletBags == 0 && IsChild;}),
                  AdvancementPairing(A_ProgressiveBombBag,   []{return AddedProgressiveBombBags   == 0 && (IsChild || (IsAdult && !Hammer));}),
                  AdvancementPairing(A_ProgressiveScale,     []{return AddedProgressiveScales     == 0 && CanLeaveForest;}),
                  AdvancementPairing(A_SariasSong,           []{return ProgressiveOcarina         >= 1 && IsAdult;}),
                  AdvancementPairing(A_ProgressiveHookshot,  []{return AddedProgressiveHookshots  == 1 && IsAdult && !LogicLostWoodsBridge && !LW_Main.CanPlantBean() && !HoverBoots;}),
                  AdvancementPairing(A_HoverBoots,           []{return AddedProgressiveHookshots  <= 2 && IsAdult && !LogicLostWoodsBridge && !LW_Main.CanPlantBean();}),
                  AdvancementPairing(A_IronBoots,            []{return AddedProgressiveScales     == 0 && IsAdult;}),
                  AdvancementPairing(A_MegatonHammer,        []{return AddedProgressiveBombBags   == 0 && IsAdult;}),
  });

  Exit LW_BeyondMido = Exit("LW Beyond Mido", "Lost Woods", "Lost Woods", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&ButterflyFairy, []{return ButterflyFairy || CanUse("Sticks");}),
                }, {
                  //Locations
                  //ItemLocationPairing(&LW_DekuScrubNearDekuTheaterRight, []{return IsChild && CanStunDeku;}),
                  //ItemLocationPairing(&LW_DekuScrubNearDekuTheaterLeft,  []{return IsChild && CanStunDeku;})
                  ItemLocationPairing(&LW_GS_AboveTheater,         []{return IsAdult && AtNight && (LW_BeyondMido.CanPlantBean() || (LogicLostWoodsGSBean && CanUse("Hookshot") && (CanUse("Longshot") || CanUse("Bow") || HasBombchus || CanUse("Dins Fire"))));}),
                  ItemLocationPairing(&LW_GS_BeanPatchNearTheater, []{return CanPlantBugs && (CanChildAttack || (Scrubsanity == SCRUBSANITY_OFF && DekuShield));}),
                }, {
                  //Exits
                  ExitPairing::Both(&LW_ForestExit,   []{return true;}),
                  ExitPairing::Both(&LW_Main,         []{return IsChild || CanPlay(SariasSong);}),
                  ExitPairing::Both(&SFM_Entryway,    []{return true;}),
                  ExitPairing::Both(&LW_DekuTheater,  []{return true;}),
                  ExitPairing::Both(&LW_ScrubsGrotto, []{return CanBlastOrSmash;})
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_SariasSong,           []{return ProgressiveOcarina       >= 1 && IsAdult;}),
                  AdvancementPairing(A_ProgressiveBombBag,   []{return AddedProgressiveBombBags == 0 && (IsChild || (IsAdult && !Hammer));}),
                  AdvancementPairing(A_MegatonHammer,        []{return AddedProgressiveBombBags == 0 && IsAdult;}),

  });

  Exit LW_NearShortcutsGrotto = Exit("LW Near Shortcuts Grotto", "", "", NO_DAY_NIGHT_CYCLE, grottoEvents, {
                  //Locations
                  ItemLocationPairing(&LW_NearShortcutsGrottoChest, []{return true;})
                  //LW Near Shortcuts Grotto Gossip Stone
                }, {
                  //Exits
                  ExitPairing::Both(&LW_Main, []{return true;})
  });

  Exit LW_DekuTheater = Exit("Deku Theater", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  ItemLocationPairing(&DekuTheater_SkullMask,   []{return IsChild && SkullMask;}),
                  ItemLocationPairing(&DekuTheater_MaskOfTruth, []{return IsChild && MaskOfTruth;}),
                }, {
                  //Exits
                  ExitPairing::Both(&LW_BeyondMido, []{return true;}),
  });

  Exit LW_ScrubsGrotto = Exit("LW Scrubs Grotto", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  //ItemLocationPairing(&LW_DekuScrubGrottoRear,  []{return CanStunDeku;}),
                  //ItemLocationPairing(&LW_DekuScrubGrottoFront, []{return CanStunDeku;}),
                }, {
                  //Exits
                  ExitPairing::Both(&LW_BeyondMido, []{return true;}),
  });

  Exit SFM_Entryway = Exit("SFM Entryway", "Sacred Forest Meadow", "Sacred Forest Meadow", NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  ExitPairing::Both(&LW_BeyondMido,    []{return true;}),
                  ExitPairing::Both(&SFM_Main,         []{return IsAdult || Slingshot || Sticks || KokiriSword || CanUse("Dins Fire");}),
                  ExitPairing::Both(&SFM_WolfosGrotto, []{return CanOpenBombGrotto;}),
                }, {
                  AdvancementPairing(A_ProgressiveBombBag,   []{return AddedProgressiveBombBags == 0;}),
                  AdvancementPairing(A_ProgressiveMagic,     []{return AddedProgressiveMagics   == 0 && !KokiriSword && !Sticks && !Slingshot && IsChild;}),
                  AdvancementPairing(A_DinsFire,             []{return ProgressiveMagic         >= 1 && !KokiriSword && !Sticks && !Slingshot && IsChild;}),
  });

  Exit SFM_Main = Exit("Sacred Forest Meadow", "Sacred Forest Meadow", "Sacred Forest Meadow", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&GossipStoneFairy, []{return GossipStoneFairy || CanSummonGossipFairyWithoutSuns;})
                }, {
                  //Locations
                  //Song from Saria
                  //Sheik in Forest
                  ItemLocationPairing(&SFM_GS, []{return CanUse("Hookshot") && AtNight;}),
                  //SFM Maze Gossip Stone (Lower)
                  //SFM Maze Gossip Stone (Upper)
                  //SFM Saria Gossip Stone
                }, {
                  //Exits
                  ExitPairing::Both(&SFM_Entryway,       []{return true;}),
                  ExitPairing::Both(&ForestTemple_Lobby, []{return CanUse("Hookshot");}),
                  ExitPairing::Both(&SFM_FairyGrotto,    []{return true;}),
                  ExitPairing::Both(&SFM_StormsGrotto,   []{return CanOpenStormGrotto;}),
                }, {
                  //advancement needs
                  AdvancementPairing(A_ProgressiveHookshot,  []{return AddedProgressiveHookshots == 0 && IsAdult;}),
                  AdvancementPairing(A_SongOfStorms,         []{return ProgressiveOcarina        >= 1;}),

  });

  Exit SFM_FairyGrotto = Exit("SFM Fairy Grotto", "", "", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&FreeFairies, []{return true;})
                }, {}, {
                  //Exits
                  ExitPairing::Both(&SFM_Main, []{return true;}),
  });

  Exit SFM_WolfosGrotto = Exit("SFM Wolfos Grotto", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  ItemLocationPairing(&SFM_WolfosGrottoChest, []{return IsAdult || Slingshot || Sticks || KokiriSword || CanUse("Dins Fire");})
                }, {
                  //Exits
                  ExitPairing::Both(&SFM_Main, []{return true;}),
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_KokiriSword,          []{return !Sticks && !Slingshot && !DinsFire && IsChild;}),
                  AdvancementPairing(A_ProgressiveBulletBag, []{return AddedProgressiveBulletBags == 0 && !KokiriSword && !Sticks && !MagicMeter && IsChild;}),
                  AdvancementPairing(A_ProgressiveMagic,     []{return AddedProgressiveMagics     == 0 && !KokiriSword && !Sticks && !Slingshot  && IsChild;}),
                  AdvancementPairing(A_DinsFire,             []{return ProgressiveMagic           >= 1 && !KokiriSword && !Sticks && !Slingshot  && IsChild;}),

  });

  Exit SFM_StormsGrotto = Exit("SFM Storms Grotto", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  //ItemLocationPairing(&SFM_DekuScrubGrottoRear,  []{return CanStunDeku;}),
                  //ItemLocationPairing(&SFM_DekuScrubGrottoFront, []{return CanStunDeku;}),
                }, {
                  //Exits
                  ExitPairing::Both(&SFM_Main, []{return true;})
  });

  Exit LW_BridgeFromForest = Exit("LW Bridge From Forest", "Lost Woods", "the Lost Woods", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  // ItemLocationPairing(&LW_GiftFromSaria, []{return true;})
                }, {
                  //Exits
                  ExitPairing::Both(&LW_Bridge, []{return true;})
  });

  Exit LW_Bridge = Exit("LW Bridge", "Lost Woods", "the Lost Woods", NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  ExitPairing::Both(&KF_Main, []{return true;}),
                  ExitPairing::Both(&HF_Main, []{return true;}),
                  ExitPairing::Both(&LW_Main, []{return CanUse("Longshot");})
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_ProgressiveHookshot,  []{return AddedProgressiveHookshots == 1 && IsAdult;}),
  });

  Exit HF_Main = Exit("Hyrule Field", "Hyrule Field", "Hyrule Field", DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  ItemLocationPairing(&HF_OcarinaOfTimeItem, []{return IsChild && HasAllStones;}),
                  //Song from Ocarina of Time
                  //Big Poe Kill
                }, {
                  //Exits
                  ExitPairing::Both(&LW_Bridge,            []{return true;}),
                  ExitPairing::Both(&LH_Main,              []{return true;}),
                  ExitPairing::Both(&GV_Main,              []{return true;}),
                  ExitPairing::Both(&MK_Entrance,          []{return true;}),
                  ExitPairing::Both(&Kak_Main,             []{return true;}),
                  ExitPairing::Both(&ZR_Front,             []{return true;}),
                  ExitPairing::Both(&LLR_Main,             []{return true;}),
                  ExitPairing::Both(&HF_SoutheastGrotto,   []{return CanBlastOrSmash;}),
                  ExitPairing::Both(&HF_OpenGrotto,        []{return true;}),
                  ExitPairing::Both(&HF_InsideFenceGrotto, []{return CanOpenBombGrotto;}),
                  ExitPairing::Both(&HF_CowGrotto,         []{return (CanUse("Hammer") || IsChild) && CanOpenBombGrotto;}),
                  ExitPairing::Both(&HF_NearMarketGrotto,  []{return CanBlastOrSmash;}),
                  ExitPairing::Both(&HF_FairyGrotto,       []{return CanBlastOrSmash;}),
                  ExitPairing::Both(&HF_NearKakGrotto,     []{return CanOpenBombGrotto;}),
                  ExitPairing::Both(&HF_TektiteGrotto,     []{return CanOpenBombGrotto;})
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_ProgressiveBombBag, []{return AddedProgressiveBombBags == 0;}),
                  AdvancementPairing(A_MegatonHammer,      []{return AddedProgressiveBombBags == 0 && IsAdult && !Hammer;}),
  });

  Exit HF_SoutheastGrotto = Exit("HF Southeast Grotto", "", "", NO_DAY_NIGHT_CYCLE, grottoEvents, {
                  //Locations
                  ItemLocationPairing(&HF_SoutheastGrottoChest, []{return true;})
                  //HF Open Grotto Gossip Stone
                }, {
                  //Exits
                  ExitPairing::Both(&HF_Main, []{return true;})
  });

  Exit HF_OpenGrotto = Exit("HF Open Grotto", "", "", NO_DAY_NIGHT_CYCLE, grottoEvents, {
                  //Locations
                  ItemLocationPairing(&HF_OpenGrottoChest, []{return true;})
                  //HF Open Grotto Gossip Stone
                }, {
                  //Exits
                  ExitPairing::Both(&HF_Main, []{return true;})
  });

  Exit HF_InsideFenceGrotto = Exit("HF Inside Fence Grotto", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  ItemLocationPairing(&HF_DekuScrubGrotto, []{return CanStunDeku;}),
                }, {
                  //Exits
                  ExitPairing::Both(&HF_Main, []{return true;})
  });

  Exit HF_CowGrotto = Exit("HF Cow Grotto", "", "", NO_DAY_NIGHT_CYCLE, grottoEvents, {
                  //Locations
                  ItemLocationPairing(&HF_GS_CowGrotto, []{return HasFireSource && HookshotOrBoomerang;}),
                  //HF Cow Grotto Cow
                  //HF Open Grotto Gossip Stone
                }, {
                  //Exits
                  ExitPairing::Both(&HF_Main, []{return true;})
  });

  Exit HF_NearMarketGrotto = Exit("HF Near Market Grotto", "", "", NO_DAY_NIGHT_CYCLE, grottoEvents, {
                  //Locations
                  ItemLocationPairing(&HF_NearMarketGrottoChest, []{return true;})
                  //HF Open Grotto Gossip Stone
                }, {
                  //Exits
                  ExitPairing::Both(&HF_Main, []{return true;})
  });

  Exit HF_FairyGrotto = Exit("HF Fairy Grotto", "", "", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&FreeFairies, []{return true;})
                }, {}, {
                  //Exits
                  ExitPairing::Both(&HF_Main, []{return true;})
  });

  Exit HF_NearKakGrotto = Exit("HF Near Kak Grotto", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  ItemLocationPairing(&HF_GS_NearKakGrotto, []{return HookshotOrBoomerang;}),
                }, {
                  //Exits
                  ExitPairing::Both(&HF_Main, []{return true;})
  });

  Exit HF_TektiteGrotto = Exit("HF Tektite Grotto", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  ItemLocationPairing(&HF_TektiteGrottoFreestandingPoH, []{return ProgressiveScale >= 2 || CanUse("Iron Boots");}),
                }, {
                  //Exits
                  ExitPairing::Both(&HF_Main, []{return true;}),
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_ProgressiveScale, []{return AddedProgressiveScales == 0;}),
                  AdvancementPairing(A_ProgressiveScale, []{return AddedProgressiveScales == 1;}),
                  AdvancementPairing(A_IronBoots,        []{return IsAdult && AddedProgressiveScales < 2;}),
  });

  Exit LH_Main = Exit("Lake Hylia", "Lake Hylia", "Lake Hylia", DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&GossipStoneFairy, []{return GossipStoneFairy || CanSummonGossipFairy;}),
                  EventPairing(&BeanPlantFairy,   []{return BeanPlantFairy   || (LH_Main.CanPlantBean() && CanPlay(SongOfStorms));}),
                  EventPairing(&ButterflyFairy,   []{return ButterflyFairy   || CanUse("Sticks");}),
                  EventPairing(&BugShrub,         []{return BugShrub         || (IsChild && CanCutShrubs);}),
                  EventPairing(&ScarecrowSong,    []{return ScarecrowSong    || (Ocarina && LH_Main.BothAges());}),
                }, {
                  //Locations
                  ItemLocationPairing(&LH_UnderwaterItem,  []{return IsChild && CanDive;}),
                  ItemLocationPairing(&LH_Sun,             []{return IsAdult && (CanUse("Distant Scarecrow") || WaterTempleClear) && CanUse("Bow");}),
                  ItemLocationPairing(&LH_FreestandingPoH, []{return IsAdult && (CanUse("Scarecrow") || LH_Main.CanPlantBean());}),
                  ItemLocationPairing(&LH_GS_BeanPatch,    []{return CanPlantBugs && CanChildAttack;}),
                  ItemLocationPairing(&LH_GS_LabWall,      []{return IsChild && (Boomerang || (LogicLabWallGS && (Sticks || KokiriSword))) && AtNight;}),
                  ItemLocationPairing(&LH_GS_SmallIsland,  []{return IsChild && CanChildAttack && AtNight;}),
                  ItemLocationPairing(&LH_GS_Tree,         []{return CanUse("Longshot") && AtNight;}),
                  //LH Gossip Stone (Southeast)
                  //LH Gossip Stone (Southwest)
                }, {
                  //Exits
                  ExitPairing::Both(&HF_Main,           []{return true;}),
                  ExitPairing::Both(&ZD_Main,           []{return IsChild && CanDive;}),
                  ExitPairing::Both(&LH_OwlFlight,      []{return IsChild;}),
                  ExitPairing::Both(&LH_Lab,            []{return true;}),
                  ExitPairing::Both(&LH_FishingHole,    []{return IsChild || CanUse("Scarecrow") || LH_Main.CanPlantBean() || WaterTempleClear;}),
                  ExitPairing::Both(&WaterTemple_Lobby, []{return CanUse("Hookshot") && (CanUse("Iron Boots") || ((CanUse("Longshot") || LogicWaterHookshotEntry) && ProgressiveScale >= 2));}),
                  ExitPairing::Both(&LH_Grotto,         []{return true;})
                }, {
                  AdvancementPairing(A_ProgressiveScale,     []{return AddedProgressiveScales     == 0 && IsChild;}),
                  AdvancementPairing(A_ProgressiveScale,     []{return AddedProgressiveScales     == 1 && IsAdult;}),
                  AdvancementPairing(A_ProgressiveHookshot,  []{return AddedProgressiveHookshots  == 0 && IsAdult;}),
                  AdvancementPairing(A_ProgressiveHookshot,  []{return AddedProgressiveHookshots  == 1 && IsAdult;}),
                  AdvancementPairing(A_ZoraTunic,            []{return IsAdult && !LogicFewerTunicRequirements;}),
                  AdvancementPairing(A_IronBoots,            []{return IsAdult;}),
  });

  Exit LH_OwlFlight = Exit("LH Owl Flight", "Lake Hylia", "", NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  ExitPairing::Both(&HF_Main, []{return true;})
  });

  Exit LH_Lab = Exit("LH Lab", "", "", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  //EyedropsAccess
                }, {
                  //Locations
                  ItemLocationPairing(&LH_LabDive,     []{return ProgressiveScale >= 2 || (LogicLabDiving && IronBoots && CanUse("Hookshot"));}),
                  ItemLocationPairing(&LH_GS_LabCrate, []{return IronBoots && CanUse("Hookshot");}),
                }, {
                  //Exits
                  ExitPairing::Both(&LH_Main, []{return true;})
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_ProgressiveHookshot,  []{return AddedProgressiveHookshots == 0 && IsAdult;}),
                  AdvancementPairing(A_ProgressiveScale,     []{return AddedProgressiveScales    == 0;}),
                  AdvancementPairing(A_ProgressiveScale,     []{return AddedProgressiveScales    == 1;}),
                  AdvancementPairing(A_IronBoots,            []{return IsAdult;}),
  });

  Exit LH_FishingHole = Exit("LH Fishing Hole", "", "", DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  ItemLocationPairing(&LH_ChildFishing, []{return IsChild;}),
                  ItemLocationPairing(&LH_AdultFishing, []{return IsAdult;})
                }, {
                  //Exits
                  ExitPairing::Both(&LH_Main, []{return true;})
  });

  Exit LH_Grotto = Exit("LH Grotto", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  //ItemLocationPairing(&LH_DekuScrubGrottoLeft,   []{return CanStunDeku;}),
                  //ItemLocationPairing(&LH_DekuScrubGrottoRight,  []{return CanStunDeku;}),
                  //ItemLocationPairing(&LH_DekuScrubGrottoCenter, []{return CanStunDeku;}),
                }, {
                  //Exits
                  ExitPairing::Both(&LH_Main, []{return true;})
  });

  Exit GV_Main = Exit("Gerudo Valley", "Gerudo Valley", "Gerudo Valley", DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  //GV GS Small Bridge
                  ItemLocationPairing(&GV_GS_SmallBridge, []{return CanUse("Boomerang") && AtNight;}),
                  //Bug Rock
                }, {
                  //Exits
                  ExitPairing::Both(&HF_Main,          []{return true;}),
                  ExitPairing::Both(&GV_Stream,        []{return true;}),
                  ExitPairing::Both(&GV_CrateLedge,    []{return IsChild || CanUse("Longshot");}),
                  ExitPairing::Both(&GV_OctorokGrotto, []{return CanUse("Silver Gauntlets");}),
                  ExitPairing::Both(&GV_FortressSide,  []{return IsAdult && (CanRideEpona || CanUse("Longshot") || GerudoFortress == GERUDOFORTRESS_OPEN || CarpenterRescue);})
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_ProgressiveHookshot,  []{return AddedProgressiveHookshots  == 0 && IsAdult;}),
                  AdvancementPairing(A_ProgressiveHookshot,  []{return AddedProgressiveHookshots  == 1 && IsAdult;}),

  });

  Exit GV_Stream = Exit("GV Stream", "Gerudo Valley", "Gerudo Valley", DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&GossipStoneFairy, []{return GossipStoneFairy || CanSummonGossipFairy;}),
                  EventPairing(&BeanPlantFairy,   []{return BeanPlantFairy   || (GV_Stream.CanPlantBean() && CanPlay(SongOfStorms));}),
                }, {
                  //Locations
                  ItemLocationPairing(&GV_WaterfallFreestandingPoH, []{return true;}),
                  ItemLocationPairing(&GV_GS_BeanPatch,             []{return CanPlantBugs && CanChildAttack;}),
                  //GV Cow
                  //GV Gossip Stone
                }, {
                  //Exits
                  ExitPairing::Both(&LH_Main, []{return true;})
  });

  Exit GV_CrateLedge = Exit("GV Crate Ledge", "Gerudo Valley", "Gerudo Valley", DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  ItemLocationPairing(&GV_CrateFreestandingPoH, []{return true;}),
                }, {
                  //Exits
  });

  Exit GV_FortressSide = Exit("GV Fortress Side", "Gerudo Valley", "Gerudo Valley", DAY_NIGHT_CYCLE, {
                  //Events
                  //BrokenSwordAccess
                }, {
                  //Locations
                  ItemLocationPairing(&GV_Chest,         []{return CanUse("Hammer");}),
                  ItemLocationPairing(&GV_GS_BehindTent, []{return CanUse("Hookshot") && AtNight;}),
                  ItemLocationPairing(&GV_GS_Pillar,     []{return CanUse("Hookshot") && AtNight;}),
                }, {
                  //Exits
                  ExitPairing::Both(&GF_Main,          []{return true;}),
                  ExitPairing::Both(&GV_Stream,        []{return true;}),
                  ExitPairing::Both(&GV_Main,          []{return IsChild || CanRideEpona || CanUse("Longshot") || GerudoFortress == GERUDOFORTRESS_OPEN || CarpenterRescue;}),
                  ExitPairing::Both(&GV_CarpenterTent, []{return IsAdult;}),
                  ExitPairing::Both(&GV_StormsGrotto,  []{return IsAdult && CanOpenStormGrotto;})
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_ProgressiveHookshot,  []{return AddedProgressiveHookshots == 0 && IsAdult;}),
                  AdvancementPairing(A_ProgressiveHookshot,  []{return AddedProgressiveHookshots == 1 && IsAdult;}),
                  AdvancementPairing(A_MegatonHammer,        []{return IsAdult;}),
  });

  Exit GV_CarpenterTent = Exit("GV Carpenter Tent", "", "", NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  ExitPairing::Both(&GV_Main, []{return true;})
  });

  Exit GV_OctorokGrotto = Exit("GV Octorok Grotto", "", "", NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  ExitPairing::Both(&GV_Main, []{return true;})
  });

  Exit GV_StormsGrotto = Exit("GV Storms Grotto", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  //ItemLocationPairing(&GV_DekuScrubGrottoRear,  []{return CanStunDeku;}),
                  //ItemLocationPairing(&GV_DekuScrubGrottoFront, []{return CanStunDeku;}),
                }, {
                  //Exits
                  ExitPairing::Both(&GV_FortressSide, []{return true;})
  });

  Exit GF_Main = Exit("Gerudo Fortress", "Gerudo Fortress", "Gerudo's Fortress", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&CarpenterRescue, []{return CanFinishGerudoFortress;}),
                  EventPairing(&GF_GateOpen,     []{return IsAdult && GerudoToken;}),
                }, {
                  //Locations
                  ItemLocationPairing(&GF_Chest,            []{return CanUse("Hover Boots") || CanUse("Scarecrow") || CanUse("Longshot");}),
                  ItemLocationPairing(&GF_HBA1000Points,    []{return GerudoToken && CanRideEpona && Bow && AtDay;}),
                  ItemLocationPairing(&GF_HBA1500Points,    []{return GerudoToken && CanRideEpona && Bow && AtDay;}),
                  ItemLocationPairing(&GF_NorthF1Carpenter, []{return  IsAdult || KokiriSword;}),
                  ItemLocationPairing(&GF_NorthF2Carpenter, []{return (IsAdult || KokiriSword) && (GerudoToken || CanUse("Bow") || CanUse("Hookshot") || CanUse("Hover Boots") || LogicGerudoKitchen);}),
                  ItemLocationPairing(&GF_SouthF1Carpenter, []{return  IsAdult || KokiriSword;}),
                  ItemLocationPairing(&GF_SouthF2Carpenter, []{return  IsAdult || KokiriSword;}),
                  ItemLocationPairing(&GF_GerudoToken,      []{return CanFinishGerudoFortress;}),
                  ItemLocationPairing(&GF_GS_ArcheryRange,  []{return CanUse("Hookshot") && GerudoToken && AtNight;}),
                  ItemLocationPairing(&GF_GS_TopFloor,      []{return IsAdult && AtNight && (GerudoToken || CanUse("Bow") || CanUse("Hookshot") || CanUse("Hover Boots") || LogicGerudoKitchen);})
                }, {
                  //Exits
                  ExitPairing::Both(&GV_FortressSide,             []{return true;}),
                  ExitPairing::Both(&GF_OutsideGate,              []{return GF_GateOpen;}),
                  ExitPairing::Both(&GerudoTrainingGrounds_Lobby, []{return IsAdult && GerudoToken;}),
                  ExitPairing::Both(&GF_StormsGrotto,             []{return IsAdult && CanOpenStormGrotto;})
                }, {
                  AdvancementPairing(A_KokiriSword,           []{return IsChild;}),
                  AdvancementPairing(A_HoverBoots,            []{return IsAdult && AddedProgressiveBows == 0 && AddedProgressiveHookshots == 0;}),
                  AdvancementPairing(A_ProgressiveBow,        []{return IsAdult && AddedProgressiveBows == 0 && AddedProgressiveHookshots == 0 && !HoverBoots;}),
                  AdvancementPairing(A_ProgressiveHookshot,   []{return IsAdult && AddedProgressiveBows == 0 && AddedProgressiveHookshots == 0 && !HoverBoots;}),
                  AdvancementPairing(GerudoFortress_SmallKey, []{return true;}, 4),
  });

  Exit GF_OutsideGate = Exit("GF Outside Gate", "Gerudo Fortress", "", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&GF_GateOpen, []{return IsAdult && GerudoToken && (ShuffleGerudoToken || ShuffleOverworldEntrances || ShuffleSpecialIndoorEntrances);}),
                }, {}, {
                  //Exits
                  ExitPairing::Both(&GF_Main,         []{return IsAdult || (ShuffleOverworldEntrances && GF_GateOpen);}),
                  ExitPairing::Both(&HW_NearFortress, []{return true;})
  });

  Exit GF_StormsGrotto = Exit("GF Storms Grotto", "", "", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&FreeFairies, []{return true;}),
                }, {}, {
                  //Exits
                  ExitPairing::Both(&GF_Main, []{return true;})
  });

  Exit HW_NearFortress = Exit("Wasteland Near Fortress", "Haunted Wasteland", "Haunted Wasteland", NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  ExitPairing::Both(&GF_OutsideGate, []{return true;}),
                  ExitPairing::Both(&HW_Main,        []{return CanUse("Hover Boots") || CanUse("Longshot");}),
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_HoverBoots,           []{return IsAdult && AddedProgressiveHookshots < 2;}),
                  AdvancementPairing(A_ProgressiveHookshot,  []{return AddedProgressiveHookshots  == 0 && IsAdult && !HoverBoots;}),
                  AdvancementPairing(A_ProgressiveHookshot,  []{return AddedProgressiveHookshots  == 1 && IsAdult && !HoverBoots;}),
  });

  Exit HW_Main = Exit("Haunted Wasteland", "Haunted Wasteland", "Haunted Wasteland", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&FairyPot, []{return true;}),
                  EventPairing(&NutPot,   []{return true;}),
                }, {
                  //Locations
                  ItemLocationPairing(&HW_Chest, []{return HasFireSource;}),
                  //Wasteland Bombchu Salesman
                  ItemLocationPairing(&HW_GS,    []{return HookshotOrBoomerang;}),
                }, {
                  //Exits
                  ExitPairing::Both(&HW_NearColossus, []{return LogicLensWasteland || CanUse("Lens of Truth");}),
                  ExitPairing::Both(&HW_NearFortress, []{return CanUse("Hover Boots") || CanUse("Longshot");}),
                }, {
                  AdvancementPairing(A_ProgressiveMagic,    []{return AddedProgressiveMagics    == 0 && !LogicLensWasteland;}),
                  AdvancementPairing(A_LensOfTruth,         []{return ProgressiveMagic          >= 1 && !LogicLensWasteland;}),
                  AdvancementPairing(A_HoverBoots,          []{return AddedProgressiveHookshots == 0;}),
                  AdvancementPairing(A_ProgressiveHookshot, []{return AddedProgressiveHookshots == 1 && !HoverBoots;}),
                  AdvancementPairing(A_DinsFire,            []{return ProgressiveMagic          >= 1 && !FireArrows;}),
                  AdvancementPairing(A_FireArrows,          []{return ProgressiveMagic          >= 1 && ProgressiveBow >= 1 && !DinsFire;}),
  });

  Exit HW_NearColossus = Exit("Wasteland Near Colossus", "Haunted Wasteland", "", NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  ExitPairing::Both(&Colossus_Main, []{return true;}),
                  ExitPairing::Both(&HW_Main,       []{return LogicReverseWasteland;})
  });

  Exit Colossus_Main = Exit("Desert Colossus", "Desert Colossus", "Desert Colossus", DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&FairyPond, []{return FairyPond || CanPlay(SongOfStorms);}),
                  EventPairing(&BugRock,   []{return true;}),
                }, {
                  //Locations
                  ItemLocationPairing(&Colossus_FreestandingPoH, []{return IsAdult && Colossus_Main.CanPlantBean();}),
                  //Sheik at Colossus
                  ItemLocationPairing(&Colossus_GS_BeanPatch,    []{return CanPlantBugs && CanChildAttack;}),
                  ItemLocationPairing(&Colossus_GS_Tree,         []{return CanUse("Hookshot") && AtNight;}),
                  ItemLocationPairing(&Colossus_GS_Hill,         []{return IsAdult && AtNight && (Colossus_Main.CanPlantBean() || CanUse("Longshot") || (LogicColossusGS && CanUse("Hookshot")));})
                  //Colossus Gossip Stone
                }, {
                  //Exits
                  ExitPairing::Both(&Colossus_GreatFairyFountain, []{return HasExplosives;}),
                  ExitPairing::Both(&SpiritTemple_Lobby,          []{return true;}),
                  ExitPairing::Both(&HW_NearColossus,             []{return true;}),
                  ExitPairing::Both(&Colossus_Grotto,             []{return CanUse("Silver Gauntlets");})
                }, {
                  AdvancementPairing(A_MagicBeanPack,       []{return Colossus_Main.BothAges();}),
                  AdvancementPairing(A_ProgressiveStrength, []{return AddedProgressiveStrengths == 1 && IsAdult;}),
                  AdvancementPairing(A_ProgressiveBombchus, []{return AddedProgressiveBombBags  == 0;}),
                  AdvancementPairing(A_ProgressiveBombBag,  []{return AddedProgressiveBombBags  == 0;}),
  });

  Exit Colossus_GreatFairyFountain = Exit("Colossus Great Fairy Fountain", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  //Colossus Great Fairy Reward
                }, {
                  //Exits
                  ExitPairing::Both(&Colossus_Main, []{return true;})
  });

  Exit Colossus_Grotto = Exit("Colossus Grotto", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  //ItemLocationPairing(&Colossus_DekuScrubGrottoRear,  []{return CanStunDeku;}),
                  //ItemLocationPairing(&Colossus_DekuScrubGrottoFront, []{return CanStunDeku;}),
                }, {
                  //Exits
                  ExitPairing::Both(&Colossus_Main, []{return true;})
  });

  Exit MK_Entrance = Exit("Market Entrance", "Market Entrance", "the Market", NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  ExitPairing::Day(&HF_Main, []{return IsAdult || AtDay;}),
                  ExitPairing::Both(&MK_Main, []{return true;}),
                  ExitPairing::Both(&MK_GuardHouse, []{return true;})
  });

  Exit MK_Main = Exit("Market", "Market", "Market", NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  ExitPairing::Both(&MK_Entrance,          []{return true;}),
                  ExitPairing::Both(&ToT_Entrance,         []{return true;}),
                  ExitPairing::Both(&CastleGrounds,        []{return true;}),
                  ExitPairing::Day(&MK_Bazaar,            []{return IsChild && AtDay;}),
                  ExitPairing::Day(&MK_MaskShop,          []{return IsChild && AtDay;}),
                  ExitPairing::Day(&MK_ShootingGallery,   []{return IsChild && AtDay;}),
                  ExitPairing::Both(&MK_BombchuBowling,    []{return IsChild;}),
                  ExitPairing::Night(&MK_TreasureChestGame, []{return IsChild && AtNight;}),
                  ExitPairing::Day(&MK_PotionShop,        []{return IsChild && AtDay;}),
                  ExitPairing::Night(&MK_BombchuShop,       []{return IsChild && AtNight;}),
                  ExitPairing::Both(&MK_DogLadyHouse,      []{return IsChild;}),
                  ExitPairing::Night(&MK_ManInGreenHouse,   []{return IsChild && AtNight;})
  });

  Exit ToT_Entrance = Exit("ToT Entrance", "ToT Entrance", "the Market", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&GossipStoneFairy, []{return GossipStoneFairy || CanSummonGossipFairyWithoutSuns;}),
                }, {
                  //Locations
                  //ToT Gossip Stone (Left)
                  //ToT Gossip Stone (Left-Center)
                  //ToT Gossip Stone (Right)
                  //ToT Gossip Stone (Right-Center)
                }, {
                  //Exits
                  ExitPairing::Both(&MK_Main,  []{return true;}),
                  ExitPairing::Both(&ToT_Main, []{return true;})
  });

  Exit ToT_Main = Exit("Temple of Time", "", "Temple of Time", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  //ToT Light Arrow Cutscene
                }, {
                  //Exits
                  ExitPairing::Both(&ToT_Entrance,         []{return true;}),
                  ExitPairing::Both(&ToT_BeyondDoorOfTime, []{return CanPlay(SongOfTime) || OpenDoorOfTime;}),
                }, {
                  AdvancementPairing(A_SongOfTime, []{return !OpenDoorOfTime;}),
  });

  Exit ToT_BeyondDoorOfTime = Exit("Beyond Door of Time", "", "Temple of Time", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  //Master Sword Pedestal
                  //Sheik at Temple
                }, {
                  //Exits
                  ExitPairing::Both(&ToT_Main, []{return true;})
  });

  Exit CastleGrounds = Exit("Castle Grounds", "Castle Grounds", "", NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  ExitPairing::Both(&MK_Main,     []{return true;}),
                  ExitPairing::Both(&HC_Grounds,  []{return IsChild;}),
                  ExitPairing::Both(&OGC_Grounds, []{return IsAdult;})
  });

  Exit HC_Grounds = Exit("Hyrule Castle Grounds", "Castle Grounds", "Hyrule Castle", DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&GossipStoneFairy, []{return GossipStoneFairy || CanSummonGossipFairy;}),
                  EventPairing(&ButterflyFairy,   []{return ButterflyFairy   || CanUse("Sticks");}),
                  EventPairing(&BugRock,          []{return true;}),
                }, {
                  //Locations
                  ItemLocationPairing(&HC_MalonEgg, []{return true;}),
                  ItemLocationPairing(&HC_GS_Tree,  []{return CanChildAttack;}),
                  //HC Malon Gossip Stone
                  //HC HC Rock Wall Gossip Stone
                }, {
                  //Exits
                  ExitPairing::Both(&CastleGrounds,         []{return true;}),
                  ExitPairing::Both(&HC_Garden,             []{return WeirdEgg || !ShuffleWeirdEgg;}),
                  ExitPairing::Both(&HC_GreatFairyFountain, []{return HasExplosives;}),
                  ExitPairing::Both(&HC_StormsGrotto,       []{return CanOpenStormGrotto;})
                }, {
                  AdvancementPairing(A_WeirdEgg,            []{return true;}),
                  AdvancementPairing(A_ProgressiveBombBag,  []{return IsChild && AddedProgressiveBombBags == 0;}),
                  AdvancementPairing(A_ProgressiveBombchus, []{return IsChild && AddedProgressiveBombBags == 0;}),
                  AdvancementPairing(A_SongOfStorms,        []{return IsChild && ProgressiveOcarina       >= 1;}),
  });

  Exit HC_Garden = Exit("HC Garden", "Castle Grounds", "Hyrule Castle", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&ZeldasLullaby, []{return true;}), //TODO: remove eventually
                }, {
                  //Locations
                  ItemLocationPairing(&HC_ZeldasLetter, []{return true;})
                  //Song From Impa
                }, {
                  //Exits
                  ExitPairing::Both(&HC_Grounds, []{return true;})
  });

  Exit HC_GreatFairyFountain = Exit("HC Great Fairy Fountain", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  //HC Great Fairy Reward
                }, {
                  //Exits
                  ExitPairing::Both(&HC_Grounds, []{return true;})
  });

  Exit HC_StormsGrotto = Exit("HC Storms Grotto", "", "", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&NutPot,           []{return NutPot           || CanBlastOrSmash;}),
                  EventPairing(&GossipStoneFairy, []{return GossipStoneFairy || (CanBlastOrSmash && CanSummonGossipFairy);}),
                  EventPairing(&WanderingBugs,    []{return WanderingBugs    || CanBlastOrSmash;}),
                }, {
                  //Locations
                  ItemLocationPairing(&HC_GS_StormsGrotto, []{return CanBlastOrSmash && HookshotOrBoomerang;}),
                  //HC Storms Grotto Gossip Stone
                }, {
                  //Exits
                  ExitPairing::Both(&HC_Grounds, []{return true;})
  });

  Exit OGC_Grounds = Exit("Ganon's Castle Grounds", "Castle Grounds", "outside Ganon's Castle", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  ItemLocationPairing(&OGC_GS, []{return true;}),
                }, {
                  //Exits
                  ExitPairing::Night(&CastleGrounds,          []{return AtNight;}),
                  ExitPairing::Night(&OGC_GreatFairyFountain, []{return CanUse("Golden Gauntlets") && AtNight;}),
                  ExitPairing::Both(&GanonsCastle_Lobby,     []{return CanBuildRainbowBridge;})
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_ProgressiveStrength, []{return IsAdult && AddedProgressiveStrengths == 0;}),
                  AdvancementPairing(A_ProgressiveStrength, []{return IsAdult && AddedProgressiveStrengths == 1;}),
                  AdvancementPairing(A_ProgressiveStrength, []{return IsAdult && AddedProgressiveStrengths == 2;}),
  });

  Exit OGC_GreatFairyFountain = Exit("OGC Great Fairy Fountain", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  //OGC Great Fairy Reward
                }, {
                  //Exits
                  ExitPairing::Both(&CastleGrounds, []{return true;}),
  });

  Exit MK_GuardHouse = Exit("Market Guard House", "", "", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  //Sell Big Poe
                }, {
                  //Locations
                  ItemLocationPairing(&MK_10BigPoes,     []{return IsAdult && (BigPoe);}), //Needs additional logic
                  ItemLocationPairing(&MK_GS_GuardHouse, []{return IsChild;}),
                }, {
                  //Exits
                  ExitPairing::Both(&MK_Entrance, []{return true;})
  });

  Exit MK_Bazaar = Exit("Market Bazaar", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  //Market Bazaar Item 1
                  //Market Bazaar Item 2
                  //Market Bazaar Item 3
                  //Market Bazaar Item 4
                  //Market Bazaar Item 5
                  //Market Bazaar Item 6
                  //Market Bazaar Item 7
                  //Market Bazaar Item 8
                }, {
                  //Exits
                  ExitPairing::Both(&MK_Main, []{return true;})
  });

  Exit MK_MaskShop = Exit("Market Mask Shop", "", "", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  //Skull Mask
                  //Mask Of Truth
                }, {}, {
                  //Exits
                  ExitPairing::Both(&MK_Main, []{return true;})
  });

  Exit MK_ShootingGallery = Exit("Market Shooting Gallery", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  ItemLocationPairing(&MK_ShootingGalleryReward, []{return IsChild;})
                }, {
                  //Exits
                  ExitPairing::Both(&MK_Main, []{return true;})
  });

  Exit MK_BombchuBowling = Exit("Market Bombchu Bowling", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  //ItemLocationPairing(&MK_BombchuBowlingFirstPrize,  []{return FoundBombchus;}),
                  //ItemLocationPairing(&MK_BombchuBowlingSecondPrize, []{return FoundBombchus;})
                  //Market Bombchu Bowling Bombchus
                }, {
                  //Exits
                  ExitPairing::Both(&MK_Main, []{return true;})
  });

  Exit MK_PotionShop = Exit("Market Potion Shop", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  //Market Potion Shop Item 1
                  //Market Potion Shop Item 2
                  //Market Potion Shop Item 3
                  //Market Potion Shop Item 4
                  //Market Potion Shop Item 5
                  //Market Potion Shop Item 6
                  //Market Potion Shop Item 7
                  //Market Potion Shop Item 8
                }, {
                  //Exits
                  ExitPairing::Both(&MK_Main, []{return true;})
  });

  Exit MK_TreasureChestGame = Exit("Market Treasure Chest Game", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  ItemLocationPairing(&MK_TreasureChestGameReward, []{return CanUse("Lens of Truth");})
                }, {
                  //Exits
                  ExitPairing::Both(&MK_Main, []{return true;})
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_LensOfTruth,      []{return IsChild && ProgressiveMagic       >= 1;}),
                  AdvancementPairing(A_ProgressiveMagic, []{return IsChild && AddedProgressiveMagics == 0;}),
  });

  Exit MK_BombchuShop = Exit("Market Bombchu Shop", "", "", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&BuyBombchus10, []{return GoronRuby;}),
                  EventPairing(&BuyBombchus20, []{return GoronRuby;}),
                }, {
                  //Locations
                  //Market Bombchu Shop Item 1
                  //Market Bombchu Shop Item 2
                  //Market Bombchu Shop Item 3
                  //Market Bombchu Shop Item 4
                  //Market Bombchu Shop Item 5
                  //Market Bombchu Shop Item 6
                  //Market Bombchu Shop Item 7
                  //Market Bombchu Shop Item 8
                }, {
                  //Exits
                  ExitPairing::Both(&MK_Main, []{return true;})
  });

  Exit MK_DogLadyHouse = Exit("Market Dog Lady House", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  ItemLocationPairing(&MK_LostDog, []{return AtNight;})
                }, {
                  //Exits
                  ExitPairing::Both(&MK_Main, []{return true;})
  });

  Exit MK_ManInGreenHouse = Exit("Market Man in Green House", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                }, {
                  //Exits
                  ExitPairing::Both(&MK_Main, []{return true;})
  });

  Exit Kak_Main = Exit("Kakariko Village", "Kakariko Village", "Kakariko Village", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  //Cojiro Access
                  EventPairing(&BugRock,                 []{return true;}),
                  EventPairing(&KakarikoVillageGateOpen, []{return KakarikoVillageGateOpen || (IsChild && (ZeldasLetter || OpenKakariko == OPENKAKARIKO_OPEN));}),
                }, {
                  //Locations
                  //Sheik in Kakariko
                  ItemLocationPairing(&Kak_AnjuAsChild,               []{return IsChild && AtDay;}),
                  ItemLocationPairing(&Kak_AnjuAsAdult,               []{return IsAdult && AtDay;}),
                  ItemLocationPairing(&Kak_GS_HouseUnderConstruction, []{return IsChild && AtNight;}),
                  ItemLocationPairing(&Kak_GS_SkulltulaHouse,         []{return IsChild && AtNight;}),
                  ItemLocationPairing(&Kak_GS_GuardsHouse,            []{return IsChild && AtNight;}),
                  ItemLocationPairing(&Kak_GS_Tree,                   []{return IsChild && AtNight;}),
                  ItemLocationPairing(&Kak_GS_Watchtower,             []{return IsChild && (Slingshot || HasBombchus || (LogicKakarikoTowerGS && (Sticks || KokiriSword) && (DamageMultiplier != DAMAGEMULTIPLIER_OHKO || Fairy || CanUse("Nayrus Love")))) && AtNight;}),
                  ItemLocationPairing(&Kak_GS_AboveImpasHouse,        []{return CanUse("Hookshot") && AtNight;}),
                }, {
                  //Exits
                  ExitPairing::Both(&HF_Main,                []{return true;}),
                  ExitPairing::Both(&Kak_CarpenterBossHouse, []{return true;}),
                  ExitPairing::Both(&Kak_HouseOfSkulltula,   []{return true;}),
                  ExitPairing::Both(&Kak_ImpasHouse,         []{return true;}),
                  ExitPairing::Both(&Kak_Windmill,           []{return true;}),
                  ExitPairing::Day(&Kak_Bazaar,              []{return IsAdult && AtDay;}),
                  ExitPairing::Day(&Kak_ShootingGallery,     []{return IsAdult && AtDay;}),
                  ExitPairing::Both(&BottomOfTheWell,        []{return DrainWell && (IsChild || ShuffleDungeonEntrances);}),
                  ExitPairing::Day(&Kak_PotionShopFront,     []{return AtDay;}),
                  ExitPairing::Both(&Kak_PotionShopFront,    []{return IsChild;}),
                  ExitPairing::Both(&Kak_RedeadGrotto,       []{return CanOpenBombGrotto;}),
                  ExitPairing::Both(&Kak_ImpasLedge,         []{return (IsAdult && LogicVisibleCollision);}),
                  ExitPairing::Day(&Kak_ImpasLedge,          []{return (IsChild && AtDay);}),
                  ExitPairing::Both(&Kak_Rooftop,            []{return CanUse("Hookshot") || (LogicManOnRoof && (IsAdult || Slingshot || HasBombchus || (LogicKakarikoTowerGS && (Sticks || KokiriSword) && (DamageMultiplier != DAMAGEMULTIPLIER_OHKO || Fairy || CanUse("Nayrus Love")))));}),
                  ExitPairing::Day(&Kak_Rooftop,             []{return LogicManOnRoof && AtDay;}),
                  ExitPairing::Both(&GY_Main,                []{return true;}),
                  ExitPairing::Both(&Kak_BehindGate,         []{return true;})
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_ProgressiveBombBag,   []{return AddedProgressiveBombBags == 0;}),
                  AdvancementPairing(A_ZeldasLetter,         []{return IsChild;}),
                  AdvancementPairing(A_MegatonHammer,        []{return IsAdult;}),
                  AdvancementPairing(A_ProgressiveHookshot,  []{return IsAdult && AddedProgressiveHookshots == 0;}),
                  AdvancementPairing(A_ProgressiveBulletBag, []{return IsChild && AtNight && AddedProgressiveBulletBags == 0;})
  });

  Exit Kak_ImpasLedge = Exit("Kak Impas Ledge", "Kakariko Village", "Kakariko Village", NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  ExitPairing::Both(&Kak_ImpasHouseBack, []{return true;}),
                  ExitPairing::Both(&Kak_Main,           []{return true;})
  });

  Exit Kak_Rooftop = Exit("Kak Rooftop", "Kakariko VIllage", "Kakariko Village", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  ItemLocationPairing(&Kak_ManOnRoof, []{return true;})
                }, {
                  //Exits
                  ExitPairing::Both(&Kak_Backyard, []{return true;})
  });

  Exit Kak_Backyard = Exit("Kak Backyard", "Kakariko Village", "Kakariko Village", NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  ExitPairing::Both(&Kak_Main, []{return true;}),
                  ExitPairing::Both(&Kak_OpenGrotto, []{return true;}),
                  ExitPairing::Both(&Kak_OddMedicineBuilding, []{return IsAdult;}),
                  ExitPairing::Day(&Kak_PotionShopBack, []{return IsAdult && AtDay;})
  });

  Exit Kak_CarpenterBossHouse = Exit("Kak Carpenter Boss House", "", "", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  //wake up adult talon
                }, {}, {
                  //Exits
                  ExitPairing::Both(&Kak_Main, []{return true;})
  });

  Exit Kak_HouseOfSkulltula = Exit("Kak House of Skulltula", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  ItemLocationPairing(&Kak_10GoldSkulltulaReward, []{return GoldSkulltulaTokens >= 10;}),
                  ItemLocationPairing(&Kak_20GoldSkulltulaReward, []{return GoldSkulltulaTokens >= 20;}),
                  ItemLocationPairing(&Kak_30GoldSkulltulaReward, []{return GoldSkulltulaTokens >= 30;}),
                  ItemLocationPairing(&Kak_40GoldSkulltulaReward, []{return GoldSkulltulaTokens >= 40;}),
                  ItemLocationPairing(&Kak_50GoldSkulltulaReward, []{return GoldSkulltulaTokens >= 50;}),
                }, {
                  //Exits
                  ExitPairing::Both(&Kak_Main, []{return true;})
                }, {
                  AdvancementPairing(GoldSkulltulaToken, []{return Skullsanity == TOKENSANITY_ALL_LOCATIONS && TokensInPool < AdvancementItemPool.size()/3;}, 50)
  });

  Exit Kak_ImpasHouse = Exit("Kak Impas House", "", "", NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  ExitPairing::Both(&Kak_ImpasHouseNearCow, []{return true;}),
                  ExitPairing::Both(&Kak_Main,              []{return true;})
  });

  Exit Kak_ImpasHouseBack = Exit("Kak Impas House Back", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  ItemLocationPairing(&Kak_ImpasHouseFreestandingPoH, []{return true;}),
                }, {
                  //Exits
                  ExitPairing::Both(&Kak_ImpasLedge,        []{return true;}),
                  ExitPairing::Both(&Kak_ImpasHouseNearCow, []{return true;})
  });

  Exit Kak_ImpasHouseNearCow = Exit("Kak Impas House Near Cow", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  //Kak Impas House Cow
                }, {
                  //Exits
                  ExitPairing::Both(&Kak_ImpasHouseBack, []{return false;})
  });

  Exit Kak_Windmill = Exit("Kak Windmill", "", "", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&DrainWell, []{return DrainWell || (IsChild && CanPlay(SongOfStorms));}),
                }, {
                  //Locations
                  ItemLocationPairing(&Kak_WindmillFreestandingPoH, []{return CanUse("Boomerang") || DampesWindmillAccess;}),
                  //Song from Windmill
                }, {
                  //Exits
                  ExitPairing::Both(&Kak_Main, []{return true;})
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_Boomerang,          []{return IsChild && !DampesWindmillAccess;}),
                  AdvancementPairing(A_SongOfStorms,       []{return ProgressiveOcarina >= 1 && IsAdult;}),
  });

  Exit Kak_Bazaar = Exit("Kak Bazaar", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  //Kak Bazaar Item 1
                  //Kak Bazaar Item 2
                  //Kak Bazaar Item 3
                  //Kak Bazaar Item 4
                  //Kak Bazaar Item 5
                  //Kak Bazaar Item 6
                  //Kak Bazaar Item 7
                  //Kak Bazaar Item 8
                }, {
                  //Exits
                  ExitPairing::Both(&Kak_Main, []{return true;})
  });

  Exit Kak_ShootingGallery = Exit("Kak Shooting Gallery", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  ItemLocationPairing(&Kak_ShootingGalleryReward, []{return IsAdult && Bow;}),
                }, {
                  //Exits
                  ExitPairing::Both(&Kak_Main, []{return true;})
  });

  Exit Kak_PotionShopFront = Exit("Kak Potion Shop Front", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  //Kak Potion Shop Item 1
                  //Kak Potion Shop Item 2
                  //Kak Potion Shop Item 3
                  //Kak Potion Shop Item 4
                  //Kak Potion Shop Item 5
                  //Kak Potion Shop Item 6
                  //Kak Potion Shop Item 7
                  //Kak Potion Shop Item 8
                }, {
                  //Exits
                  ExitPairing::Both(&Kak_Main,           []{return true;}),
                  ExitPairing::Both(&Kak_PotionShopBack, []{return IsAdult;})
  });

  Exit Kak_PotionShopBack = Exit("Kak Potion Shop Back", "", "", NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  ExitPairing::Both(&Kak_Backyard,        []{return IsAdult;}),
                  ExitPairing::Both(&Kak_PotionShopFront, []{return true;})
  });

  Exit Kak_OddMedicineBuilding = Exit("Kak Odd Medicine Building", "", "", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  //Odd Poultice Access
                }, {}, {
                  //Exits
                  ExitPairing::Both(&Kak_Backyard, []{return true;})
  });

  Exit Kak_RedeadGrotto = Exit("Kak Redead Grotto", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  ItemLocationPairing(&Kak_RedeadGrottoChest, []{return IsAdult || (Sticks || KokiriSword || CanUse("Dins Fire"));})
                }, {
                  //Exits
                  ExitPairing::Both(&Kak_Main, []{return true;})
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_DinsFire,         []{return ProgressiveMagic       >= 1;}),
                  AdvancementPairing(A_ProgressiveMagic, []{return AddedProgressiveMagics == 0;}),
                  AdvancementPairing(A_KokiriSword,      []{return IsChild;})
                  //Sticks
  });

  Exit Kak_OpenGrotto = Exit("Kak Open Grotto", "", "", NO_DAY_NIGHT_CYCLE, grottoEvents, {
                  //Locations
                  ItemLocationPairing(&Kak_OpenGrottoChest, []{return true;})
                  //Kak Open Grotto Gossip Stone
                }, {
                  //Exits
                  ExitPairing::Both(&Kak_Backyard, []{return true;})
  });

  Exit GY_Main = Exit("Graveyard", "Graveyard", "the Graveyard", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&ButterflyFairy, []{return ButterflyFairy || (CanUse("Sticks") && AtDay);}),
                  EventPairing(&BeanPlantFairy, []{return BeanPlantFairy || (GY_Main.CanPlantBean() && CanPlay(SongOfStorms));}),
                  EventPairing(&BugRock,        []{return true;}),
                }, {
                  //Locations
                  ItemLocationPairing(&GY_FreestandingPoH,       []{return (IsAdult && (GY_Main.CanPlantBean() || CanUse("Longshot"))) || (LogicGraveyardPoH && CanUse("Boomerang"));}),
                  ItemLocationPairing(&GY_DampeGravediggingTour, []{return IsChild && AtNight;}), //This needs to change
                  ItemLocationPairing(&GY_GS_Wall,               []{return CanUse("Boomerang") && AtNight;}),
                  ItemLocationPairing(&GY_GS_BeanPatch,          []{return CanPlantBugs && CanChildAttack;}),
                }, {
                  //Exits
                  ExitPairing::Both(&GY_ShieldGrave,      []{return IsAdult;}),
                  ExitPairing::Night(&GY_ShieldGrave,     []{return AtNight;}),
                  ExitPairing::Both(&GY_ComposersGrave,   []{return CanPlay(ZeldasLullaby);}),
                  ExitPairing::Both(&GY_HeartPieceGrave,  []{return IsAdult;}),
                  ExitPairing::Night(&GY_HeartPieceGrave, []{return AtNight;}),
                  ExitPairing::Both(&GY_DampesGrave,      []{return IsAdult;}),
                  ExitPairing::Both(&GY_DampesHouse,      []{return IsAdult || AtDampeTime;}), //TODO: This needs to be handled
                  ExitPairing::Both(&Kak_Main,            []{return true;})
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_ProgressiveHookshot,  []{return AddedProgressiveHookshots == 0 && IsAdult;}),
                  AdvancementPairing(A_ProgressiveHookshot,  []{return AddedProgressiveHookshots == 1 && IsAdult;}),
                  AdvancementPairing(A_ZeldasLullaby,        []{return ProgressiveOcarina        >= 1;}),
                  AdvancementPairing(A_Boomerang,            []{return IsChild && AtNight;}),
  });

  Exit GY_ShieldGrave = Exit("GY Shield Grave", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  ItemLocationPairing(&GY_ShieldGraveChest, []{return true;})
                  //Free Fairies
                }, {
                  //Exits
                  ExitPairing::Both(&GY_Main, []{return true;})
  });

  Exit GY_HeartPieceGrave = Exit("GY Heart Piece Grave", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  ItemLocationPairing(&GY_HeartPieceGraveChest, []{return CanPlay(SunsSong);})
                }, {
                  //Exits
                  ExitPairing::Both(&GY_Main, []{return true;})
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_SunsSong, []{return ProgressiveOcarina >= 1;})
  });

  Exit GY_ComposersGrave = Exit("GY Composers Grave", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  ItemLocationPairing(&GY_ComposersGraveChest, []{return HasFireSource;})
                  //Song From Composers Grave
                }, {
                  //Exits
                  ExitPairing::Both(&GY_Main, []{return true;})
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_DinsFire,         []{return ProgressiveMagic       >= 1 && !FireArrows;}),
                  AdvancementPairing(A_FireArrows,       []{return ProgressiveMagic       >= 1 && !DinsFire && ProgressiveBow >= 1 && IsAdult;}),
                  AdvancementPairing(A_ProgressiveMagic, []{return AddedProgressiveMagics == 0;}),
                  AdvancementPairing(A_ProgressiveBow,   []{return AddedProgressiveBows   == 0 && IsAdult;}),
  });

  Exit GY_DampesGrave = Exit("GY Dampes Grave", "", "", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&DekuNutDrop,          []{return true;}),
                  EventPairing(&DampesWindmillAccess, []{return DampesWindmillAccess || (IsAdult && CanPlay(SongOfTime));}),
                }, {
                  //Locations
                  ItemLocationPairing(&GY_HookshotChest,            []{return true;}),
                  ItemLocationPairing(&GY_DampeRaceFreestandingPoH, []{return IsAdult || LogicChildDampeRacePoH;}),
                }, {
                  //Exits
                  ExitPairing::Both(&GY_Main,      []{return true;}),
                  ExitPairing::Both(&Kak_Windmill, []{return IsAdult && CanPlay(SongOfTime);})
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_SongOfTime, []{return ProgressiveOcarina >= 1 && IsAdult;}),
  });

  Exit GY_DampesHouse = Exit("GY Dampes House", "", "", NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  ExitPairing::Both(&GY_Main, []{return true;})
  });

  Exit GY_WarpPadRegion = Exit("GY Warp Pad Region", "Graveyard", "the Graveyard", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&GossipStoneFairy, []{return GossipStoneFairy || CanSummonGossipFairyWithoutSuns;}),
                }, {
                  //Locations
                  //GY Gossip Stone
                }, {
                  //Exits
                  ExitPairing::Both(&GY_Main,               []{return true;}),
                  ExitPairing::Both(&ShadowTemple_Entryway, []{return CanUse("Dins Fire") || (LogicShadowFireArrowEntry && CanUse("Fire Arrows"));}),
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_ProgressiveMagic, []{return AddedProgressiveMagics == 0;}),
                  AdvancementPairing(A_DinsFire,         []{return ProgressiveMagic       >= 1;}),
                  AdvancementPairing(A_FireArrows,       []{return ProgressiveMagic       >= 1 && ProgressiveBow >= 1 && LogicShadowFireArrowEntry;}),
  });

  Exit Kak_BehindGate = Exit("Kak Behind Gate", "Kakariko Village", "", NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  ExitPairing::Both(&Kak_Main, []{return IsAdult || LogicVisibleCollision || KakarikoVillageGateOpen || OpenKakariko == OPENKAKARIKO_OPEN;}),
                  ExitPairing::Both(&DMT_Main, []{return true;})
  });

  Exit DMT_Main = Exit("Death Mountain", "Death Mountain", "Death Mountain Trail", DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&BeanPlantFairy, []{return BeanPlantFairy || (DMT_Main.CanPlantBean() && CanPlay(SongOfStorms) && (HasExplosives || GoronBracelet));}),
                }, {
                  //Locations
                  ItemLocationPairing(&DMT_Chest,                  []{return CanBlastOrSmash || (LogicDMTBombable && IsChild && GoronBracelet);}),
                  ItemLocationPairing(&DMT_FreestandingPoH,        []{return (DamageMultiplier != DAMAGEMULTIPLIER_OHKO) || CanUse("Nayrus Love") || Fairy || CanUse("Hover Boots") || (IsAdult && DMT_Main.CanPlantBean() && (HasExplosives || GoronBracelet));}),
                  ItemLocationPairing(&DMT_GS_BeanPatch,           []{return CanPlantBugs && (HasExplosives || GoronBracelet || (LogicDMTSoilGS && CanUse("Boomerang")));}),
                  ItemLocationPairing(&DMT_GS_NearKak,             []{return CanBlastOrSmash;}),
                  ItemLocationPairing(&DMT_GS_AboveDodongosCavern, []{return IsAdult && AtNight && CanUse("Hammer");})
                }, {
                  //Exits
                  ExitPairing::Both(&Kak_BehindGate,          []{return true;}),
                  ExitPairing::Both(&GC_Main,                 []{return true;}),
                  ExitPairing::Both(&DMT_Summit,              []{return CanBlastOrSmash || (IsAdult && (DMT_Main.CanPlantBean() && GoronBracelet));}),
                  ExitPairing::Both(&DodongosCavern_Entryway, []{return HasExplosives || GoronBracelet || IsAdult;}),
                  ExitPairing::Both(&DMT_StormsGrotto,        []{return CanOpenStormGrotto;})
                }, {
                  AdvancementPairing(A_ProgressiveBombBag,  []{return AddedProgressiveBombBags  == 0;}),
                  AdvancementPairing(A_SongOfStorms,        []{return ProgressiveOcarina        >= 1;}),
                  AdvancementPairing(A_ProgressiveStrength, []{return AddedProgressiveStrengths == 0;}),
  });

  Exit DMT_Summit = Exit("Death Mountain Summit", "Death Mountain", "Death Mountain", DAY_NIGHT_CYCLE, {
                  //Events
                  //Perscription Access
                  EventPairing(&GossipStoneFairy, []{return GossipStoneFairy || CanSummonGossipFairy;}),
                  EventPairing(&BugRock,          []{return true;}),
                }, {
                  //Locations
                  //ItemLocationPairing(&DMT_Biggoron, []{return IsAdult && (ClaimCheck || (GuaranteeTradePath && (EyedropsAccess || (Eyedrops && DisableTradeRevert))));}),
                  ItemLocationPairing(&DMT_GS_FallingRocksPath, []{return IsAdult && AtNight && CanUse("Hammer");}),
                  //DMT Gossip Stone
                }, {
                  //Exits
                  ExitPairing::Both(&DMT_Main,               []{return true;}),
                  ExitPairing::Both(&DMC_UpperLocal,         []{return true;}),
                  ExitPairing::Both(&DMT_OwlFlight,          []{return IsChild;}),
                  ExitPairing::Both(&DMT_CowGrotto,          []{return CanBlastOrSmash;}),
                  ExitPairing::Both(&DMT_GreatFairyFountain, []{return CanBlastOrSmash;})
                }, {
                  AdvancementPairing(A_ProgressiveBombBag,  []{return AddedProgressiveBombBags == 0 && (IsChild || (IsAdult && !Hammer));}),
                  AdvancementPairing(A_MegatonHammer,       []{return AddedProgressiveBombBags == 0 && IsAdult;}),
                  AdvancementPairing(A_ProgressiveBombchus, []{return AddedProgressiveBombBags == 0;}),
  });

  Exit DMT_OwlFlight = Exit("DMT Owl Flight", "Death Mountain", "", NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  ExitPairing::Both(&Kak_ImpasLedge, []{return true;})
  });

  Exit DodongosCavern_Entryway = Exit("Dodongos Cavern Entryway", "Death Mountain", "", NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  ExitPairing::Both(&DodongosCavern_Beginning, []{return true;}),
                  ExitPairing::Both(&DMT_Main,                 []{return true;})
  });

  Exit DMT_CowGrotto = Exit("DMT Cow Grotto", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  //DMT Cow Grotto Cow
                }, {
                  //Exits
                  ExitPairing::Both(&DMT_Main,     []{return true;})

  });

  Exit DMT_StormsGrotto = Exit("DMT Storms Grotto", "", "", NO_DAY_NIGHT_CYCLE, grottoEvents, {
                  //Locations
                  ItemLocationPairing(&DMT_StormsGrottoChest, []{return true;}),
                  //DMT Storms Grotto Gossip Stone
                }, {
                  //Exits
                  ExitPairing::Both(&DMT_Main, []{return true;})
  });

  Exit DMT_GreatFairyFountain = Exit("DMT Great Fairy Fountain", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  //ItemLocationPairing(&DMT_GreatFairyReward, []{return CanPlay(ZeldasLullaby);})
                }, {
                  //Exits
                  ExitPairing::Both(&DMT_Summit, []{return true;})
                }, {
  });

  Exit GC_Main = Exit("Goron City", "Goron City", "Goron City", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&GossipStoneFairy,          []{return GossipStoneFairy          || CanSummonGossipFairyWithoutSuns;}),
                  EventPairing(&StickPot,                  []{return StickPot                  || IsChild;}),
                  EventPairing(&BugRock,                   []{return true;}),
                  EventPairing(&GoronCityChildFire,        []{return GoronCityChildFire        || (IsChild && CanUse("Dins Fire"));}),
                  EventPairing(&GCWoodsWarpOpen,           []{return GCWoodsWarpOpen           || (CanBlastOrSmash || CanUse("Dins Fire") || CanUse("Bow") || GoronBracelet || GoronCityChildFire);}),
                  EventPairing(&StopGCRollingGoronAsAdult, []{return StopGCRollingGoronAsAdult || (IsAdult && (GoronBracelet || HasExplosives || Bow || (LogicLinkGoronDins && CanUse("Dins Fire"))));}),
                }, {
                  //Locations
                  ItemLocationPairing(&GC_MazeLeftChest,       []{return CanUse("Hammer") || CanUse("Silver Gauntlets") || (LogicGoronCityLeftMost && HasExplosives && CanUse("Hover Boots"));}),
                  ItemLocationPairing(&GC_MazeCenterChest,     []{return CanBlastOrSmash  || CanUse("Silver Gauntlets");}),
                  ItemLocationPairing(&GC_MazeRightChest,      []{return CanBlastOrSmash  || CanUse("Silver Gauntlets");}),
                  ItemLocationPairing(&GC_PotFreestandingPoH,  []{return IsChild && GoronCityChildFire && (Bombs || (GoronBracelet && LogicGoronCityPotWithStrength) || (HasBombchus && LogicGoronCityPot));}),
                  ItemLocationPairing(&GC_RollingGoronAsChild, []{return IsChild && (HasExplosives || (GoronBracelet && LogicChildRollingWithStrength));}),
                  ItemLocationPairing(&GC_RollingGoronAsAdult, []{return StopGCRollingGoronAsAdult;}),
                  ItemLocationPairing(&GC_GS_BoulderMaze,      []{return IsChild && HasExplosives;}),
                  ItemLocationPairing(&GC_GS_CenterPlatform,   []{return IsAdult;}),
                  //GC Maze Gossip Stone
                  //GC MediGoron Gossip Stone
                }, {
                  //Exits
                  ExitPairing::Both(&DMT_Main,           []{return true;}),
                  ExitPairing::Both(&GC_WoodsWarp,       []{return GCWoodsWarpOpen;}),
                  ExitPairing::Both(&GC_Shop,            []{return (IsAdult && StopGCRollingGoronAsAdult) || (IsChild && (HasExplosives || GoronBracelet || GoronCityChildFire));}),
                  ExitPairing::Both(&GC_DaruniasChamber, []{return (IsAdult && StopGCRollingGoronAsAdult) || (IsChild && CanPlay(ZeldasLullaby));}),
                  ExitPairing::Both(&GC_Grotto,          []{return IsAdult && ((CanPlay(SongOfTime) && ((DamageMultiplier != DAMAGEMULTIPLIER_OHKO && DamageMultiplier != DAMAGEMULTIPLIER_QUADRUPLE) || CanUse("Goron Tunic") || CanUse("Longshot") || CanUse("Nayrus Love"))) || (DamageMultiplier != DAMAGEMULTIPLIER_OHKO && CanUse("Goron Tunic") && CanUse("Hookshot")) ||(CanUse("Nayrus Love") && CanUse("Hookshot")));}),
                }, {
                  //AdvancementNeeds
                  AdvancementPairing(A_ProgressiveBombBag,  []{return AddedProgressiveBombBags  == 0;}),
                  AdvancementPairing(A_ProgressiveBombchus, []{return AddedProgressiveBombBags  == 0;}),
                  AdvancementPairing(A_MegatonHammer,       []{return IsAdult;}),
                  AdvancementPairing(A_ProgressiveStrength, []{return AddedProgressiveStrengths == 0;}),
                  AdvancementPairing(A_ProgressiveStrength, []{return AddedProgressiveStrengths == 1 && IsAdult;}),
                  AdvancementPairing(A_ProgressiveBow,      []{return AddedProgressiveBows      == 0 && IsAdult;}),
                  AdvancementPairing(A_GoronTunic,          []{return IsAdult;}),
                  AdvancementPairing(A_ProgressiveMagic,    []{return AddedProgressiveMagics    == 0;}),
                  AdvancementPairing(A_DinsFire,            []{return ProgressiveMagic          >= 1;}),
  });

  Exit GC_WoodsWarp = Exit("GC Woods Warp", "Goron City", "", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&GCWoodsWarpOpen, []{return GCWoodsWarpOpen || (CanBlastOrSmash || CanUse("Dins Fire"));}),
                }, {}, {
                  //Exits
                  ExitPairing::Both(&GC_Main, []{return CanLeaveForest && GCWoodsWarpOpen;}),
                  ExitPairing::Both(&LW_Main, []{return true;})
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_ProgressiveBombBag,  []{return AddedProgressiveBombBags == 0;}),
                  AdvancementPairing(A_ProgressiveBombchus, []{return AddedProgressiveBombBags == 0;}),
                  AdvancementPairing(A_MegatonHammer,       []{return IsAdult;}),
                  AdvancementPairing(A_ProgressiveMagic,    []{return AddedProgressiveMagics   == 0;}),
                  AdvancementPairing(A_DinsFire,            []{return ProgressiveMagic         >= 1;}),
  });

  Exit GC_DaruniasChamber = Exit("GC Darunias Chamber", "Goron City", "Goron City", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&GoronCityChildFire, []{return GoronCityChildFire || (CanUse("Sticks"));}),
                }, {
                  //Locations
                  ItemLocationPairing(&GC_DaruniasJoy, []{return IsChild && CanPlay(SariasSong);})
                }, {
                  //Exits
                  ExitPairing::Both(&GC_Main,        []{return true;}),
                  ExitPairing::Both(&DMC_LowerLocal, []{return IsAdult;})
                }, {
                  AdvancementPairing(A_SariasSong,         []{return IsChild && ProgressiveOcarina       >= 1;})
  });

  Exit GC_Shop = Exit("GC Shop", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  //GC Shop Item 1
                  //GC Shop Item 2
                  //GC Shop Item 3
                  //GC Shop Item 4
                  //GC Shop Item 5
                  //GC Shop Item 6
                  //GC Shop Item 7
                  //GC Shop Item 8
                }, {
                  //Exits
                  ExitPairing::Both(&GC_Main, []{return true;})
  });

  Exit GC_Grotto = Exit("GC Grotto", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  //ItemLocationPairing(&GC_DekuScrubGrottoLeft,   []{return CanStunDeku;}),
                  //ItemLocationPairing(&GC_DekuScrubGrottoRight,  []{return CanStunDeku;}),
                  //ItemLocationPairing(&GC_DekuScrubGrottoCenter, []{return CanStunDeku;}),
                }, {
                  //Exits
                  ExitPairing::Both(&GC_Main, []{return true;})
  });

  Exit DMC_UpperNearby = Exit("DMC Upper Nearby", "Death Mountain Crater", "Death Mountain Crater", NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  ExitPairing::Both(&DMC_UpperLocal,  []{return CanUse("Goron Tunic");}),
                  ExitPairing::Both(&DMT_Summit,      []{return true;}),
                  ExitPairing::Both(&DMC_UpperGrotto, []{return CanBlastOrSmash;})
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_ProgressiveBombBag,  []{return AddedProgressiveBombBags == 0;}),
                  AdvancementPairing(A_ProgressiveBombchus, []{return AddedProgressiveBombBags == 0;}),
                  AdvancementPairing(A_MegatonHammer,       []{return IsAdult;}),
                  AdvancementPairing(A_GoronTunic,          []{return IsAdult;}),
  });

  Exit DMC_UpperLocal = Exit("DMC Upper Local", "Death Mountain Crater", "Death Mountain Crater", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&GossipStoneFairy, []{return GossipStoneFairy || (HasExplosives && CanSummonGossipFairyWithoutSuns);}),
                }, {
                  //Locations
                  ItemLocationPairing(&DMC_WallFreestandingPoH, []{return true;}),
                  ItemLocationPairing(&DMC_GS_Crate,            []{return IsChild && CanChildAttack;}),
                  //DMC Gossip Stone
                }, {
                  //Exits
                  ExitPairing::Both(&DMC_UpperNearby,      []{return true;}),
                  ExitPairing::Both(&DMC_LadderAreaNearby, []{return true;}),
                  ExitPairing::Both(&DMC_CentralNearby,    []{return CanUse("Goron Tunic") && CanUse("Longshot") && ((DamageMultiplier != DAMAGEMULTIPLIER_OHKO && DamageMultiplier != DAMAGEMULTIPLIER_QUADRUPLE) || (Fairy && !ShuffleDungeonEntrances) || CanUse("Nayrus Love"));})
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_GoronTunic,          []{return IsAdult;}),
                  AdvancementPairing(A_ProgressiveHookshot, []{return IsAdult && AddedProgressiveHookshots == 0;}),
                  AdvancementPairing(A_ProgressiveHookshot, []{return IsAdult && AddedProgressiveHookshots == 1;}),
  });

  Exit DMC_LadderAreaNearby = Exit("DMC Ladder Area Nearby", "Death Mountain Crater", "Death Mountain Crater", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  //DMC Deku Scrub
                }, {
                  //Exits
                  ExitPairing::Both(&DMC_UpperNearby, []{return IsAdult;}),
                  ExitPairing::Both(&DMC_LowerNearby, []{return CanUse("Hover Boots") || (LogicCraterUpperToLower && CanUse("Hammer"));})
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_HoverBoots,    []{return IsAdult;}),
                  AdvancementPairing(A_MegatonHammer, []{return IsAdult;}),
  });

  Exit DMC_LowerNearby = Exit("DMC Lower Nearby", "Death Mountain Crater", "Death Mountain Crater", NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  ExitPairing::Both(&DMC_LowerLocal,         []{return CanUse("Goron Tunic");}),
                  ExitPairing::Both(&GC_DaruniasChamber,     []{return true;}),
                  ExitPairing::Both(&DMC_GreatFairyFountain, []{return CanUse("Hammer");}),
                  ExitPairing::Both(&DMC_HammerGrotto,       []{return CanUse("Hammer");})
                }, {
                  AdvancementPairing(A_MegatonHammer, []{return IsAdult;}),
                  AdvancementPairing(A_GoronTunic,    []{return IsAdult;}),
  });

  Exit DMC_LowerLocal = Exit("DMC Lower Local", "Death Mountain Crater", "Death Mountain Crater", NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  ExitPairing::Both(&DMC_LowerNearby,      []{return true;}),
                  ExitPairing::Both(&DMC_LadderAreaNearby, []{return true;}),
                  ExitPairing::Both(&DMC_CentralNearby,    []{return CanUse("Hover Boots") || CanUse("Hookshot");}),
                  ExitPairing::Both(&FireTemple_Entrance,  []{return (CanUse("Hover Boots") || CanUse("Hookshot")) && (LogicFewerTunicRequirements || CanUse("Goron Tunic"));}),
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_HoverBoots,          []{return IsAdult;}),
                  AdvancementPairing(A_ProgressiveHookshot, []{return IsAdult && AddedProgressiveHookshots == 0;}),
  });

  Exit DMC_CentralNearby = Exit("DMC Central Nearby", "Death Mountain Crater", "Death Mountain Crater", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  ItemLocationPairing(&DMC_VolcanoFreestandingPoH, []{return IsAdult && (DMC_CentralLocal.CanPlantBean() || (LogicCraterBeanPoHWithHovers && HoverBoots));}),
                  //Sheik in Crater
                }, {
                  //Exits
                  ExitPairing::Both(&DMC_CentralLocal, []{return CanUse("Goron Tunic");})
                }, {
                  AdvancementPairing(A_MagicBeanPack, []{return DMC_CentralLocal.BothAges();}),
                  AdvancementPairing(A_HoverBoots,    []{return IsAdult && LogicCraterBeanPoHWithHovers;}),

  });

  Exit DMC_CentralLocal = Exit("DMC Central Local", "Death Mountain Crater", "Death Mountain Crater", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&BeanPlantFairy, []{return BeanPlantFairy || (DMC_CentralLocal.CanPlantBean() && CanPlay(SongOfStorms));}),
                }, {
                  //Locations
                  ItemLocationPairing(&DMC_GS_BeanPatch, []{return CanPlantBugs && CanChildAttack;}),
                }, {
                  //Exits
                  ExitPairing::Both(&DMC_CentralNearby,   []{return true;}),
                  ExitPairing::Both(&DMC_LowerNearby,     []{return IsAdult && (CanUse("Hover Boots") || CanUse("Hookshot") || DMC_CentralLocal.CanPlantBean());}),
                  ExitPairing::Both(&DMC_UpperNearby,     []{return IsAdult && DMC_CentralLocal.CanPlantBean();}),
                  ExitPairing::Both(&FireTemple_Entrance, []{return (IsChild && ShuffleDungeonEntrances) || (IsAdult && (LogicFewerTunicRequirements || CanUse("Goron Tunic")));}),
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_HoverBoots,          []{return IsAdult && AddedProgressiveHookshots == 0;}),
                  AdvancementPairing(A_ProgressiveHookshot, []{return IsAdult && AddedProgressiveHookshots == 0 && !HoverBoots;}),
  });

  Exit FireTemple_Entrance = Exit("Fire Temple Entrance", "Death Mountain Crater", "Death Mountain Crater", NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                    ExitPairing::Both(&FireTemple_Lower, []{return true;}),
  });

  Exit DMC_GreatFairyFountain = Exit("DMC Great Fairy Fountain", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  //ItemLocationPairing(&DMC_GreatFairyFountainReward, []{return CanPlay(ZeldasLullaby);})
                }, {
                  //Exits
                  ExitPairing::Both(&DMC_LowerLocal, []{return true;})
  });

  Exit DMC_UpperGrotto = Exit("DMC Upper Grotto", "", "", NO_DAY_NIGHT_CYCLE, grottoEvents, {
                  //Locations
                  ItemLocationPairing(&DMC_UpperGrottoChest, []{return true;})
                  //DMC Upper Grotto Gossip Stone
                }, {
                  //Exits
                  ExitPairing::Both(&DMC_UpperLocal, []{return true;})
  });

  Exit DMC_HammerGrotto = Exit("DMC Hammer Grotto", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  //ItemLocationPairing(&DMC_DekuScrubGrottoLeft,   []{return CanStunDeku;}),
                  //ItemLocationPairing(&DMC_DekuScrubGrottoRight,  []{return CanStunDeku;}),
                  //ItemLocationPairing(&DMC_DekuScrubGrottoCenter, []{return CanStunDeku;}),
                }, {
                  //Exits
                  ExitPairing::Both(&DMC_LowerLocal, []{return true;})
  });

  Exit ZR_Front = Exit("ZR Front", "Zora River", "Zora's River", DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  ItemLocationPairing(&ZR_GS_Tree, []{return IsChild && CanChildAttack;}),
                }, {
                  //Exits
                  ExitPairing::Both(&ZR_Main, []{return IsAdult || HasExplosives;}),
                  ExitPairing::Both(&HF_Main, []{return true;})
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_ProgressiveBombBag,  []{return IsChild && AddedProgressiveBombBags == 0;}),
                  AdvancementPairing(A_ProgressiveBombchus, []{return IsChild && AddedProgressiveBombBags == 0;}),
  });

  Exit ZR_Main = Exit("Zora River", "Zora River", "Zora's River", DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&GossipStoneFairy, []{return GossipStoneFairy || CanSummonGossipFairy;}),
                  EventPairing(&BeanPlantFairy,   []{return BeanPlantFairy   || (ZR_Main.CanPlantBean() && CanPlay(SongOfStorms));}),
                  EventPairing(&ButterflyFairy,   []{return ButterflyFairy   || CanUse("Sticks");}),
                  EventPairing(&BugShrub,         []{return BugShrub         || CanCutShrubs;}),
                }, {
                  //Locations
                  ItemLocationPairing(&ZR_MagicBeanSalesman,             []{return IsChild && ProgressiveWallet >= 1;}), //progressive wallet until rupee count is fixed
                  ItemLocationPairing(&ZR_FrogsOcarinaGame,              []{return IsChild && CanPlay(ZeldasLullaby) && CanPlay(SariasSong) && CanPlay(SunsSong) && CanPlay(EponasSong) && CanPlay(SongOfTime) && CanPlay(SongOfStorms);}),
                  ItemLocationPairing(&ZR_FrogsInTheRain,                []{return IsChild && CanPlay(SongOfStorms);}),
                  ItemLocationPairing(&ZR_NearOpenGrottoFreestandingPoH, []{return IsChild || CanUse("Hover Boots") || (IsAdult && LogicZoraRiverLower);}),
                  ItemLocationPairing(&ZR_NearDomainFreestandingPoH,     []{return IsChild || CanUse("Hover Boots") || (IsAdult && LogicZoraRiverUpper);}),
                  ItemLocationPairing(&ZR_GS_Ladder,                     []{return IsChild && AtNight && CanChildAttack;}),
                  ItemLocationPairing(&ZR_GS_NearRaisedGrottos,          []{return CanUse("Hookshot") && AtNight;}),
                  ItemLocationPairing(&ZR_GS_AboveBridge,                []{return CanUse("Hookshot") && AtNight;}),
                  //ZR Near Grottos Gossip Stone
                  //ZR Near Domain Gossip Stone
                }, {
                  //Exits
                  ExitPairing::Both(&ZR_Front,           []{return true;}),
                  ExitPairing::Both(&ZR_OpenGrotto,      []{return true;}),
                  ExitPairing::Both(&ZR_FairyGrotto,     []{return CanBlastOrSmash;}),
                  ExitPairing::Both(&LW_Main,            []{return CanDive || CanUse("Iron Boots");}),
                  ExitPairing::Both(&ZR_StormsGrotto,    []{return CanOpenStormGrotto;}),
                  ExitPairing::Both(&ZR_BehindWaterfall, []{return CanPlay(ZeldasLullaby);})
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_SongOfStorms,       []{return ProgressiveOcarina     >= 1;}),
                  AdvancementPairing(A_IronBoots,          []{return AddedProgressiveScales == 0 && IsAdult;}),
                  AdvancementPairing(A_ProgressiveScale,   []{return AddedProgressiveScales == 0;})
  });

  Exit ZR_BehindWaterfall = Exit("ZR Behind Waterfall", "Zora River", "", DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  ExitPairing::Both(&ZR_Main, []{return true;}),
                  ExitPairing::Both(&ZD_Main, []{return true;})
  });

  Exit ZR_OpenGrotto = Exit("ZR Open Grotto", "", "", NO_DAY_NIGHT_CYCLE, grottoEvents, {
                  //Locations
                  ItemLocationPairing(&ZR_OpenGrottoChest, []{return true;})
                  //ZR Open Grotto Gossip Stone
                }, {
                  //Exits
                  ExitPairing::Both(&ZR_Main, []{return true;})
  });

  Exit ZR_FairyGrotto = Exit("ZR Fairy Grotto", "", "", NO_DAY_NIGHT_CYCLE, {
                  //Event
                  EventPairing(&FreeFairies, []{return true;}),
                }, {
                  //Locations
                }, {
                  //Exits
                  ExitPairing::Both(&ZR_Main, []{return true;})
  });

  Exit ZR_StormsGrotto = Exit("ZR Storms Grotto", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  //ItemLocationPairing(&ZR_DekuScrubGrottoRear,  []{return CanStunDeku;}),
                  //ItemLocationPairing(&ZR_DekuScrubGrottoFront, []{return CanStunDeku;}),
                }, {
                  //Exits
                  ExitPairing::Both(&ZR_Main, []{return true;})
  });

  Exit ZD_Main = Exit("Zoras Domain", "Zoras Domain", "Zora's Domain", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  //Eyeball Frog Access
                  EventPairing(&GossipStoneFairy, []{return GossipStoneFairy || CanSummonGossipFairyWithoutSuns;}),
                  EventPairing(&NutPot,           []{return true;}),
                  EventPairing(&StickPot,         []{return StickPot         || IsChild;}),
                  EventPairing(&FishGroup,        []{return FishGroup        || IsChild;}),
                  EventPairing(&KingZoraThawed,   []{return KingZoraThawed   || (IsAdult   && BlueFire);}),
                  EventPairing(&DeliverLetter,    []{return DeliverLetter    || (RutosLetter && IsChild && ZorasFountain != ZORASFOUNTAIN_OPEN);}),
                }, {
                  //Locations
                  ItemLocationPairing(&ZD_DivingMinigame,     []{return IsChild;}),
                  ItemLocationPairing(&ZD_Chest,              []{return CanUse("Sticks");}),
                  ItemLocationPairing(&ZD_KingZoraThawed,     []{return KingZoraThawed;}),
                  ItemLocationPairing(&ZD_GS_FrozenWaterfall, []{return IsAdult && AtNight && (Hookshot || Bow || MagicMeter);}),
                  //ZD Gossip Stone
                }, {
                  //Exits
                  ExitPairing::Both(&ZR_BehindWaterfall, []{return true;}),
                  ExitPairing::Both(&LH_Main,            []{return IsChild && CanDive;}),
                  ExitPairing::Both(&ZD_BehindKingZora,  []{return DeliverLetter || ZorasFountain == ZORASFOUNTAIN_OPEN || (ZorasFountain == ZORASFOUNTAIN_ADULT && IsAdult);}),
                  ExitPairing::Both(&ZD_Shop,            []{return IsChild || BlueFire;}),
                  ExitPairing::Both(&ZD_StormsGrotto,    []{return CanOpenStormGrotto;})
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_RutosLetter,      []{return IsChild && CanDive;}),
                  AdvancementPairing(A_ProgressiveScale, []{return IsChild && AddedProgressiveScales == 0;}),
  });

  Exit ZD_BehindKingZora = Exit("ZD Behind King Zora", "Zoras Domain", "", NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  ExitPairing::Both(&ZD_Main, []{return DeliverLetter || ZorasFountain == ZORASFOUNTAIN_OPEN || (ZorasFountain == ZORASFOUNTAIN_ADULT && IsAdult);}),
                  ExitPairing::Both(&ZF_Main, []{return true;})
  });

  Exit ZD_Shop = Exit("ZD Shop", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  //ZD Shop Item 1
                  //ZD Shop Item 2
                  //ZD Shop Item 3
                  //ZD Shop Item 4
                  //ZD Shop Item 5
                  //ZD Shop Item 6
                  //ZD Shop Item 7
                  //ZD Shop Item 8
                }, {
                  //Exits
                  ExitPairing::Both(&ZD_Main, []{return true;})
  });

  Exit ZD_StormsGrotto = Exit("ZD Storms Grotto", "", "", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&FreeFairies, []{return true;}),
                }, {}, {
                  //Exits
                  ExitPairing::Both(&ZD_Main, []{return true;}),
  });

  Exit ZF_Main = Exit("Zoras Fountain", "Zoras Fountain", "Zora's Fountain", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&GossipStoneFairy, []{return GossipStoneFairy || CanSummonGossipFairyWithoutSuns;}),
                  EventPairing(&ButterflyFairy,   []{return ButterflyFairy   || (CanUse("Sticks") && AtDay);}),
                }, {
                  //Locations
                  ItemLocationPairing(&ZF_IcebergFreestandingPoH, []{return IsAdult;}),
                  ItemLocationPairing(&ZF_BottomFreestandingPoH,  []{return IsAdult && IronBoots && (LogicFewerTunicRequirements || CanUse("Zora Tunic"));}),
                  ItemLocationPairing(&ZF_GS_Tree,                []{return IsChild;}),
                  ItemLocationPairing(&ZF_GS_AboveTheLog,         []{return CanUse("Boomerang") && AtNight;}),
                  ItemLocationPairing(&ZF_GS_HiddenCave,          []{return CanUse("Silver Gauntlets") && CanBlastOrSmash && CanUse("Hookshot") && AtNight;}),
                  //ZF Fairy Gossip Stone
                  //ZF Jabu Gossip Stone
                }, {
                  //Exits
                  ExitPairing::Both(&ZD_BehindKingZora,        []{return true;}),
                  ExitPairing::Both(&JabuJabusBelly_Beginning, []{return IsChild && FishAccess;}),
                  ExitPairing::Both(&IceCavern_Beginning,      []{return IsAdult;}),
                  ExitPairing::Both(&ZF_GreatFairyFountain,    []{return HasExplosives;})
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_EmptyBottle, []{return IsChild && !HasBottle;}),
                  AdvancementPairing(A_ZoraTunic,   []{return IsAdult && !LogicFewerTunicRequirements;}),
  });

  Exit ZF_GreatFairyFountain = Exit("ZF Great Fairy Fountain", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  //ZF Great Fairy Reward
                }, {
                  //Exits
                  ExitPairing::Both(&ZF_Main, []{return true;})
  });

  Exit LLR_Main = Exit("Lon Lon Ranch", "Lon Lon Ranch", "Lon Lon Ranch", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&Epona,    []{return Epona    || (CanPlay(EponasSong) && IsAdult && AtDay);}),
                  EventPairing(&LinksCow, []{return LinksCow || (CanPlay(EponasSong) && IsAdult && AtDay);}),
                }, {
                  //Locations
                  //Song from Malon
                  ItemLocationPairing(&LLR_GS_Tree,        []{return IsChild;}),
                  ItemLocationPairing(&LLR_GS_RainShed,    []{return IsChild && AtNight;}),
                  ItemLocationPairing(&LLR_GS_HouseWindow, []{return CanUse("Boomerang") && AtNight;}),
                  ItemLocationPairing(&LLR_GS_BackWall,    []{return CanUse("Boomerang") && AtNight;}),
                }, {
                  //Exits
                  ExitPairing::Both(&HF_Main,         []{return true;}),
                  ExitPairing::Both(&LLR_TalonsHouse, []{return true;}),
                  ExitPairing::Both(&LLR_Stables,     []{return true;}),
                  ExitPairing::Both(&LLR_Tower,       []{return true;}),
                  ExitPairing::Both(&LLR_Grotto,      []{return IsChild;})
                }, {
                  AdvancementPairing(A_EponasSong, []{return ProgressiveOcarina  >= 1  && AtDay   && IsAdult;}),
                  AdvancementPairing(A_Boomerang,  []{return GoldSkulltulaTokens <= 50 && AtNight && IsChild;}),

  });

  Exit LLR_TalonsHouse = Exit("LLR Talons House", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  ItemLocationPairing(&LLR_TalonsChickens, []{return IsChild && AtDay && ZeldasLetter;})
                }, {
                  //Exits
                  ExitPairing::Both(&LLR_Main, []{return true;})
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_ZeldasLetter, []{return IsChild;}),
  });

  Exit LLR_Stables = Exit("LLR Stables", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  //LLR Stables Left Cow
                  //LLR Stables Right Cow
                }, {
                  //Exits
                  ExitPairing::Both(&LLR_Main, []{return true;})
  });

  Exit LLR_Tower = Exit("LLR Tower", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  ItemLocationPairing(&LLR_FreestandingPoH, []{return IsChild;}),
                  //LLR Tower Left Cow
                  //LLR Tower Right Cow
                }, {
                  //Exits
                  ExitPairing::Both(&LLR_Main, []{return true;}),
  });

  Exit LLR_Grotto = Exit("LLR Grotto", "", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  //ItemLocationPairing(&LLR_DekuScrubGrottoLeft,   []{return CanStunDeku;}),
                  //ItemLocationPairing(&LLR_DekuScrubGrottoRight,  []{return CanStunDeku;}),
                  //ItemLocationPairing(&LLR_DekuScrubGrottoCenter, []{return CanStunDeku;}),
                }, {
                  //Exits
                  ExitPairing::Both(&LLR_Main, []{return true;}),
  });

  Exit DekuTree_Lobby = Exit("Deku Tree Lobby", "Deku Tree", "Deku Tree", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&DekuBabaSticks, []{return DekuBabaSticks || (IsAdult || KokiriSword || Boomerang);}),
                  EventPairing(&DekuBabaNuts,   []{return DekuBabaNuts   || (IsAdult || KokiriSword || Slingshot || Sticks || HasExplosives || CanUse("Dins Fire"));}),
                }, {
                  //Locations
                  ItemLocationPairing(&DekuTree_MapChest,             []{return true;}),
                  ItemLocationPairing(&DekuTree_CompassChest,         []{return true;}),
                  ItemLocationPairing(&DekuTree_CompassRoomSideChest, []{return true;}),
                  ItemLocationPairing(&DekuTree_BasementChest,        []{return IsAdult || CanChildAttack || Nuts;}),
                  ItemLocationPairing(&DekuTree_GS_CompassRoom,       []{return IsAdult || CanChildAttack;}),
                  ItemLocationPairing(&DekuTree_GS_BasementVines,     []{return CanUseProjectile || CanUse("Dins Fire") || (LogicDekuBasementGS && (IsAdult || Sticks || KokiriSword));}),
                  ItemLocationPairing(&DekuTree_GS_BasementGate,      []{return IsAdult || CanChildAttack;}),
                  ItemLocationPairing(&DekuTree_GS_BasementBackRoom,  []{return ((HasFireSourceWithTorch || (LogicDekuB1WebsWithBow && CanUse("Bow"))) && (CanUse("Slingshot") || CanUse("Bow")) && CanBlastOrSmash && HookshotOrBoomerang) || ((LogicDekuB1Skip || IsAdult) && IsChild && HasExplosives && CanUse("Boomerang") && (Sticks || CanUse("Dins Fire")));}),
                }, {
                  //Exits
                  ExitPairing::Both(&KF_OutsideDekuTree,       []{return true;}),
                  ExitPairing::Both(&DekuTree_SlingshotRoom,   []{return HasShield;}),
                  ExitPairing::Both(&DekuTree_BossRoom,        []{return (HasFireSourceWithTorch || (LogicDekuB1WebsWithBow && CanUse("Bow"))) && (LogicDekuB1Skip || (IsAdult || CanUse("Slingshot")));})
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_ProgressiveBulletBag, []{return IsChild && AddedProgressiveBulletBags == 0;}),
                  AdvancementPairing(A_DekuShield,           []{return IsChild;}),
                  AdvancementPairing(A_HylianShield,         []{return IsAdult;}),
  });

  Exit DekuTree_SlingshotRoom = Exit("Deku Tree Slingshot Room", "Deku Tree", "Deku Tree", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  ItemLocationPairing(&DekuTree_SlingshotChest,         []{return true;}),
                  ItemLocationPairing(&DekuTree_SlingshotRoomSideChest, []{return true;})
                }, {
                  //Exits
                  ExitPairing::Both(&DekuTree_Lobby, []{return true;})
  });

  Exit DekuTree_BossRoom = Exit("Deku Tree Boss Room", "Deku Tree", "Deku Tree", NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventPairing(&DekuTreeClear, []{return DekuTreeClear || (HasShield && (IsAdult || KokiriSword || Sticks));})
                }, {
                  //Locations
                  ItemLocationPairing(&QueenGohma,               []{return HasShield && (IsAdult || KokiriSword || Sticks);}),
                  ItemLocationPairing(&DekuTree_QueenGohmaHeart, []{return HasShield && (IsAdult || KokiriSword || Sticks);}),
                }, {
                  //Exits
                  ExitPairing::Both(&DekuTree_Lobby, []{return true;})
                }, {
                  //Advancement Needs
                  AdvancementPairing(A_KokiriSword,  []{return IsChild;}),
                  AdvancementPairing(A_DekuShield,   []{return IsChild;}),
                  AdvancementPairing(A_HylianShield, []{return IsAdult;}),
  });

  Exit DodongosCavern_Beginning = Exit("Dodongos Cavern Beginning", "Dodongos Cavern", "", NO_DAY_NIGHT_CYCLE, {}, {}, {
                    //Exits
                    ExitPairing::Both(&DodongosCavern_Entryway, []{return true;}),
                    ExitPairing::Both(&DodongosCavern_Lobby,    []{return CanBlastOrSmash || GoronBracelet;})
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_ProgressiveBombBag,  []{return AddedProgressiveBombBags  == 0;}),
                    AdvancementPairing(A_ProgressiveBombchus, []{return AddedProgressiveBombBags  == 0;}),
                    AdvancementPairing(A_MegatonHammer,       []{return IsAdult;}),
                    AdvancementPairing(A_ProgressiveStrength, []{return AddedProgressiveStrengths == 0;}),
  });

  Exit DodongosCavern_Lobby = Exit("Dodongos Cavern Lobby", "Dodongos Cavern", "", NO_DAY_NIGHT_CYCLE, {
                    //Events
                    EventPairing(&GossipStoneFairy, []{return GossipStoneFairy || CanSummonGossipFairy;}),
                  }, {
                    //Locations
                    ItemLocationPairing(&DodongosCavern_MapChest,                     []{return true;}),
                    ItemLocationPairing(&DodongosCavern_CompassChest,                 []{return IsAdult || Sticks || (CanUse("Dins Fire") && (Slingshot || HasExplosives || KokiriSword));}),
                    ItemLocationPairing(&DodongosCavern_GS_SideRoomNearLowerLizalfos, []{return HasExplosives || IsAdult || Slingshot || Boomerang || Sticks || KokiriSword;}),
                    ItemLocationPairing(&DodongosCavern_GS_Scarecrow,                 []{return CanUse("Scarecrow") || CanUse("Longshot") || (LogicDCScarecrowGS && (IsAdult || CanChildAttack));}),
                    //Dodongos Cavern Deku Scrub Side Room Near Dodongos
                    //Dodongos Cavern Deku Scrub Lobby
                    //Dodongos Cavern Gossip Stone
                  }, {
                    //Exits
                    ExitPairing::Both(&DodongosCavern_Beginning, []{return true;}),
                    ExitPairing::Both(&DodongosCavern_Climb,     []{return (IsAdult || ((Sticks || CanUse("Dins Fire")) && (Slingshot || Sticks || HasExplosives || KokiriSword))) && (HasExplosives || GoronBracelet || CanUse("Dins Fire") ||(LogicDCStaircase && CanUse("Bow")));})
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_ProgressiveBombBag,  []{return AddedProgressiveBombBags  == 0;}),
                    AdvancementPairing(A_ProgressiveBombchus, []{return AddedProgressiveBombBags  == 0;}),
                    AdvancementPairing(A_ProgressiveStrength, []{return AddedProgressiveStrengths == 0;}),
                    AdvancementPairing(A_DinsFire,            []{return ProgressiveMagic          >= 1;}),
                    AdvancementPairing(A_ProgressiveMagic,    []{return AddedProgressiveMagics    == 0;}),
  });

  Exit DodongosCavern_Climb = Exit("Dodongos Cavern Climb", "Dodongos Cavern", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&DodongosCavern_BombFlowerPlatformChest, []{return true;}),
                    ItemLocationPairing(&DodongosCavern_GS_VinesAboveStairs,     []{return true;}),
                    //Dodongos Cavern Deku Scrub Near Bomb Bag Right
                    //Dodongos cavern Deku Scrub Near Bomb Bag Left
                  }, {
                    //Exits
                    ExitPairing::Both(&DodongosCavern_Lobby,     []{return true;}),
                    ExitPairing::Both(&DodongosCavern_FarBridge, []{return (IsChild && (Slingshot || (LogicDCSlingshotSkip && (Sticks || HasExplosives || KokiriSword)))) || (IsAdult && (Bow || HoverBoots || CanUse("Longshot") || LogicDCJump));})
  });

  Exit DodongosCavern_FarBridge = Exit("Dodongos Cavern Far Bridge", "Dodongos Cavern", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&DodongosCavern_BombBagChest,         []{return true;}),
                    ItemLocationPairing(&DodongosCavern_EndOfBridgeChest,     []{return CanBlastOrSmash;}),
                    ItemLocationPairing(&DodongosCavern_GS_AlcoveAboveStairs, []{return HookshotOrBoomerang;}),
                  }, {
                    //Exits
                    ExitPairing::Both(&DodongosCavern_BossArea, []{return HasExplosives;}),
                    ExitPairing::Both(&DodongosCavern_Lobby,    []{return true;})
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_ProgressiveBombBag,  []{return AddedProgressiveBombBags == 0;}),
                    AdvancementPairing(A_ProgressiveBombchus, []{return AddedProgressiveBombBags == 0;}),
                    AdvancementPairing(A_MegatonHammer,       []{return AddedProgressiveBombBags == 0 && IsAdult;}),
  });

  Exit DodongosCavern_BossArea = Exit("Dodongos Cavern Boss Area", "Dodongos Cavern", "", NO_DAY_NIGHT_CYCLE, {
                    //Events
                    EventPairing(&FairyPot, []{return true;}),
                  }, {
                    //Locations
                    ItemLocationPairing(&DodongosCavern_BossRoomChest,    []{return true;}),
                    ItemLocationPairing(&DodongosCavern_KingDodongoHeart, []{return (Bombs || GoronBracelet) && (IsAdult || Sticks || KokiriSword);}),
                    ItemLocationPairing(&KingDodongo,                     []{return (Bombs || GoronBracelet) && (IsAdult || Sticks || KokiriSword);}),
                    ItemLocationPairing(&DodongosCavern_GS_BackRoom,      []{return true;}),
                  }, {
                    //Exits
                    ExitPairing::Both(&DodongosCavern_Lobby, []{return true;})
  });

  Exit JabuJabusBelly_Beginning = Exit("Jabu Jabus Belly Beginning", "Jabu Jabus Belly", "", NO_DAY_NIGHT_CYCLE, {}, {}, {
                    //Exits
                    ExitPairing::Both(&ZF_Main,             []{return true;}),
                    ExitPairing::Both(&JabuJabusBelly_Main, []{return CanUseProjectile;})
                  }, {
                    AdvancementPairing(A_Boomerang,            []{return IsChild;}),
                    AdvancementPairing(A_ProgressiveBulletBag, []{return IsChild && AddedProgressiveBulletBags == 0;}),
                    AdvancementPairing(A_ProgressiveBow,       []{return IsAdult && AddedProgressiveBows       == 0;}),
                    AdvancementPairing(A_ProgressiveHookshot,  []{return IsAdult && AddedProgressiveHookshots  == 0;}),
  });

  Exit JabuJabusBelly_Main = Exit("Jabu Jabus Belly Main", "Jabu Jabus Belly", "", NO_DAY_NIGHT_CYCLE, {
                    //Events
                    EventPairing(&FairyPot, []{return true;}),
                  }, {
                    //Locations
                    ItemLocationPairing(&JabuJabusBelly_BoomerangChest,        []{return true;}),
                    ItemLocationPairing(&JabuJabusBelly_GS_WaterSwitchRoom,    []{return true;}),
                    ItemLocationPairing(&JabuJabusBelly_GS_LobbyBasementLower, []{return HookshotOrBoomerang;}),
                    ItemLocationPairing(&JabuJabusBelly_GS_LobbyBasementUpper, []{return HookshotOrBoomerang;}),
                    //Jabu Jabus Belly Deku Scrub
                  }, {
                    //Exits
                    ExitPairing::Both(&JabuJabusBelly_Beginning, []{return true;}),
                    ExitPairing::Both(&JabuJabusBelly_Depths,    []{return CanUse("Boomerang");}),
                    ExitPairing::Both(&JabuJabusBelly_BossArea,  []{return LogicJabuBossGSAdult && CanUse("Hover Boots");})
                  }, {
                    AdvancementPairing(A_Boomerang, []{return IsChild;}),
  });

  Exit JabuJabusBelly_Depths = Exit("Jabu Jabus Belly Depths", "Jabu Jabus Belly", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&JabuJabusBelly_MapChest,     []{return true;}),
                    ItemLocationPairing(&JabuJabusBelly_CompassChest, []{return true;}),
                  }, {
                    //Exits
                    ExitPairing::Both(&JabuJabusBelly_Main,     []{return true;}),
                    ExitPairing::Both(&JabuJabusBelly_BossArea, []{return Sticks || KokiriSword;}),
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_KokiriSword, []{return IsChild;}),
  });

  Exit JabuJabusBelly_BossArea = Exit("Jabu Jabus Belly Boss Area", "Jabu Jabus Belly", "", NO_DAY_NIGHT_CYCLE, {
                    //Events
                    EventPairing(&NutPot, []{return true;}),
                  }, {
                    //Locations
                    ItemLocationPairing(&JabuJabusBelly_BarinadeHeart, []{return CanUse("Boomerang");}),
                    ItemLocationPairing(&Barinade,                     []{return CanUse("Boomerang");}),
                    ItemLocationPairing(&JabuJabusBelly_GS_NearBoss,   []{return true;}),
                  }, {
                    //Exits
                    ExitPairing::Both(&JabuJabusBelly_Main, []{return true;}),
  });

  Exit ForestTemple_Lobby = Exit("Forest Temple Lobby", "Forest Temple", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&ForestTemple_FirstRoomChest,    []{return true;}),
                    ItemLocationPairing(&ForestTemple_FirstStalfosChest, []{return IsAdult || KokiriSword;}),
                    ItemLocationPairing(&ForestTemple_GS_FirstRoom,      []{return CanUse("Dins Fire") || CanUseProjectile;}),
                    ItemLocationPairing(&ForestTemple_GS_Lobby,          []{return HookshotOrBoomerang;}),
                  }, {
                    //Exits
                    ExitPairing::Both(&SFM_Main,                   []{return true;}),
                    ExitPairing::Both(&ForestTemple_NWOutdoors,    []{return CanPlay(SongOfTime) || IsChild;}),
                    ExitPairing::Both(&ForestTemple_NEOutdoors,    []{return CanUse("Bow") || CanUse("Slingshot");}),
                    ExitPairing::Both(&ForestTemple_BlockPushRoom, []{return SmallKeys(ForestTempleKeys, 1);}),
                    ExitPairing::Both(&ForestTemple_BossRegion,    []{return ForestTempleJoAndBeth && ForestTempleAmyAndMeg;})
                  }, {
                    //Advancement Needs
                    AdvancementPairing(ForestTemple_SmallKey,  []{return true;}, 1),
                    AdvancementPairing(A_SongOfTime,           []{return IsAdult && ProgressiveOcarina         >= 1;}),
                    AdvancementPairing(A_ProgressiveBow,       []{return IsAdult && AddedProgressiveBows       == 0;}),
                    AdvancementPairing(A_ProgressiveBulletBag, []{return IsChild && AddedProgressiveBulletBags == 0;}),
  });

  Exit ForestTemple_NWOutdoors = Exit("Forest Temple NW Outdoors", "Forest Temple", "", NO_DAY_NIGHT_CYCLE, {
                    //Events
                    EventPairing(&DekuBabaSticks, []{return DekuBabaSticks || (IsAdult || KokiriSword || Boomerang);}),
                    EventPairing(&DekuBabaNuts,   []{return DekuBabaNuts   || (IsAdult || KokiriSword || Slingshot || Sticks || HasExplosives || CanUse("Dins Fire"));}),
                  }, {
                    //Locations
                    ItemLocationPairing(&ForestTemple_GS_LevelIslandCourtyard, []{return CanUse("Longshot") || ((ForestTemple_OutsideUpperLedge.dayAdult || ForestTemple_OutsideUpperLedge.nightAdult) && CanUse("Hookshot"));}),
                  }, {
                    //Exits
                    ExitPairing::Both(&ForestTemple_OutdoorsHighBalconies, []{return IsAdult || (HasExplosives || ((CanUse("Boomerang") || Nuts || DekuShield) && (Sticks || KokiriSword || CanUse("Slingshot"))));})
  });

  Exit ForestTemple_NEOutdoors = Exit("Forest Temple NE Outdoors", "Forest Temple", "", NO_DAY_NIGHT_CYCLE, {
                    //Events
                    EventPairing(&DekuBabaSticks, []{return DekuBabaSticks || (IsAdult || KokiriSword || Boomerang);}),
                    EventPairing(&DekuBabaNuts,   []{return DekuBabaNuts   || (IsAdult || KokiriSword || Slingshot || Sticks || HasExplosives || CanUse("Dins Fire"));}),
                  }, {
                    //Locations
                    ItemLocationPairing(&ForestTemple_RaisedIslandCourtyardChest, []{return CanUse("Hookshot");}),
                    ItemLocationPairing(&ForestTemple_GS_RaisedIslandCourtyard,   []{return CanUse("Hookshot") || (LogicForestOutdoorEastGS && CanUse("Boomerang"));}),
                  }, {
                    //Exits
                    ExitPairing::Both(&ForestTemple_OutdoorsHighBalconies, []{return CanUse("Longshot")   || (LogicForestVines && CanUse("Hookshot"));}),
                    ExitPairing::Both(&ForestTemple_NWOutdoors,            []{return CanUse("Iron Boots") || ProgressiveScale >= 2;}),
                    ExitPairing::Both(&ForestTemple_Lobby,                 []{return true;})
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_ProgressiveHookshot,  []{return AddedProgressiveHookshots  == 0 && IsAdult;}),
                    AdvancementPairing(A_ProgressiveHookshot,  []{return AddedProgressiveHookshots  == 1 && IsAdult;}),
                    AdvancementPairing(A_ProgressiveScale,     []{return AddedProgressiveScales     == 0;}),
                    AdvancementPairing(A_ProgressiveScale,     []{return AddedProgressiveScales     == 1;}),
                    AdvancementPairing(A_IronBoots,            []{return IsAdult;}),
  });

  Exit ForestTemple_OutdoorsHighBalconies = Exit("Forest Temple Outdoors High Balconies", "Forest Temple", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&ForestTemple_WellChest, []{return true;}),
                    ItemLocationPairing(&ForestTemple_MapChest,  []{return true;})
                  }, {
                    //Exits
                    ExitPairing::Both(&ForestTemple_NWOutdoors,  []{return true;}),
                    ExitPairing::Both(&ForestTemple_NEOutdoors,  []{return true;}),
                    ExitPairing::Both(&ForestTemple_FallingRoom, []{return LogicForestScarecrow && CanUse("Hover Boots") && CanUse("Scarecrow");})
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_HoverBoots,          []{return IsAdult && LogicForestScarecrow;}),
                    AdvancementPairing(A_ProgressiveHookshot, []{return IsAdult && LogicForestScarecrow && AddedProgressiveHookshots == 0;}),
  });

  Exit ForestTemple_FallingRoom = Exit("Forest Temple Falling Room", "Forest Temple", "", NO_DAY_NIGHT_CYCLE, {
                    //Events
                    EventPairing(&ForestTempleAmyAndMeg, []{return ForestTempleAmyAndMeg || CanUse("Bow");}),
                  }, {
                    //Locations
                    ItemLocationPairing(&ForestTemple_FallingCeilingRoomChest, []{return true;})
                  }, {
                    //Exits
                    ExitPairing::Both(&ForestTemple_NEOutdoors, []{return true;})
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_ProgressiveBow, []{return IsAdult && AddedProgressiveBows == 0;}),
  });

  Exit ForestTemple_BlockPushRoom = Exit("Forest Temple Block Push Room", "Forest Temple", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&ForestTemple_EyeSwitchChest, []{return GoronBracelet && (CanUse("Bow") || CanUse("Slingshot"));})
                  }, {
                    //Exits
                    ExitPairing::Both(&ForestTemple_OutsideUpperLedge, []{return CanUse("Hover Boots") || (LogicForestOutsideBackdoor && IsAdult && GoronBracelet);}),
                    ExitPairing::Both(&ForestTemple_BowRegion,         []{return GoronBracelet && SmallKeys(ForestTempleKeys, 3) && IsAdult;}),
                    ExitPairing::Both(&ForestTemple_StraightenedHall,  []{return GoronBracelet && SmallKeys(ForestTempleKeys, 2) && CanUse("Bow") && IsAdult;})
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_ProgressiveBow,      []{return IsAdult && AddedProgressiveBows == 0;}),
                    AdvancementPairing(A_HoverBoots,          []{return IsAdult;}),
                    AdvancementPairing(ForestTemple_SmallKey, []{return true;}, 3),
  });

  Exit ForestTemple_StraightenedHall = Exit("Forest Temple Straightened Hall", "Forest Temple", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&ForestTemple_BossKeyChest, []{return true;})
                  }, {
                    //Exits
                    ExitPairing::Both(&ForestTemple_OutsideUpperLedge, []{return true;})
  });

  Exit ForestTemple_OutsideUpperLedge = Exit("Forest Temple Outside Upper Ledge", "Forest Temple", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&ForestTemple_FloormasterChest, []{return true;})
                  }, {
                    //Exits
                    ExitPairing::Both(&ForestTemple_NWOutdoors, []{return true;})
  });

  Exit ForestTemple_BowRegion = Exit("Forest Temple Bow Region", "Forest Temple", "", NO_DAY_NIGHT_CYCLE, {
                    //Events
                    EventPairing(&ForestTempleJoAndBeth, []{return ForestTempleJoAndBeth || CanUse("Bow");}),
                  }, {
                    //Locations
                    ItemLocationPairing(&ForestTemple_BowChest,     []{return true;}),
                    ItemLocationPairing(&ForestTemple_RedPoeChest,  []{return CanUse("Bow");}),
                    ItemLocationPairing(&ForestTemple_BluePoeChest, []{return CanUse("Bow");})
                  }, {
                    //Exits
                    ExitPairing::Both(&ForestTemple_FallingRoom, []{return SmallKeys(ForestTempleKeys, 5) && (Bow || CanUse("Dins Fire"));})
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_ProgressiveBow,      []{return IsAdult && AddedProgressiveBows   == 0;}),
                    AdvancementPairing(A_DinsFire,            []{return IsAdult && ProgressiveMagic       >= 1;}),
                    AdvancementPairing(A_ProgressiveMagic,    []{return IsAdult && AddedProgressiveMagics == 0;}),
                    AdvancementPairing(ForestTemple_SmallKey, []{return true;}, 5),
  });

  Exit ForestTemple_BossRegion = Exit("Forest Temple Boss Region", "Forest Temple", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&ForestTemple_BasementChest,     []{return true;}),
                    ItemLocationPairing(&ForestTemple_PhantomGanonHeart, []{return BossKeyForestTemple;}),
                    ItemLocationPairing(&PhantomGanon,                   []{return BossKeyForestTemple;}),
                    ItemLocationPairing(&ForestTemple_GS_Basement,       []{return HookshotOrBoomerang;}),
                  }, {}, {
                    //Advancement Needs
                    AdvancementPairing(ForestTemple_BossKey, []{return true;}),
  });

  Exit FireTemple_Lower = Exit("Fire Temple Lower", "Fire Temple", "", NO_DAY_NIGHT_CYCLE, {
                    //Events
                    EventPairing(&FairyPot, []{return FairyPot || ((CanUse("Hover Boots") || CanUse("Hookshot")) && (LogicFewerTunicRequirements || CanUse("Goron Tunic")));}),
                  }, {
                    //Locations
                    ItemLocationPairing(&FireTemple_NearBossChest,    []{return LogicFewerTunicRequirements || CanUse("Goron Tunic");}),
                    ItemLocationPairing(&FireTemple_FlareDancerChest, []{return /*SmallKeys(FireTempleKeys, 8) &&*/ CanUse("Hammer");}),
                    ItemLocationPairing(&FireTemple_BossKeyChest,     []{return /*SmallKeys(FireTempleKeys, 8) &&*/ CanUse("Hammer");}),
                    ItemLocationPairing(&FireTemple_VolvagiaHeart,    []{return CanUse("Goron Tunic") && CanUse("Hammer") && BossKeyFireTemple && (LogicFireBossDoorJump || HoverBoots || CanPlay(SongOfTime) || HasExplosives);}),
                    ItemLocationPairing(&Volvagia,                    []{return CanUse("Goron Tunic") && CanUse("Hammer") && BossKeyFireTemple && (LogicFireBossDoorJump || HoverBoots || CanPlay(SongOfTime) || HasExplosives);}),
                    ItemLocationPairing(&FireTemple_GS_BossKeyLoop,   []{return /*SmallKeys(FireTempleKeys, 8) &&*/ CanUse("Hammer");}),
                  }, {
                    //Exits
                    ExitPairing::Both(&FireTemple_Entrance,    []{return true;}),
                    ExitPairing::Both(&FireTemple_BigLavaRoom, []{return SmallKeys(FireTempleKeys, 1) && (LogicFewerTunicRequirements || CanUse("Goron Tunic"));}),
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_MegatonHammer,     []{return IsAdult;}),
                    AdvancementPairing(A_HoverBoots,        []{return IsAdult;}),
                    AdvancementPairing(A_GoronTunic,        []{return IsAdult && !LogicFewerTunicRequirements;}),
                    AdvancementPairing(FireTemple_SmallKey, []{return true;}, 1),
                    AdvancementPairing(FireTemple_BossKey,  []{return true;}),
  });

  Exit FireTemple_BigLavaRoom = Exit("Fire Temple Big Lava Room", "Fire Temple", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&FireTemple_BigLavaRoomLowerOpenDoorChest, []{return true;}),
                    ItemLocationPairing(&FireTemple_BigLavaRoomBlockedDoorChest,   []{return IsAdult && HasExplosives;}),
                    ItemLocationPairing(&FireTemple_GS_SongOfTimeRoom,             []{return IsAdult && (CanPlay(SongOfTime) || LogicFireSongOfTime);}),
                  }, {
                    //Exits
                    ExitPairing::Both(&FireTemple_Lower,  []{return true;}),
                    ExitPairing::Both(&FireTemple_Middle, []{return CanUse("Goron Tunic") && SmallKeys(FireTempleKeys, 4) && (GoronBracelet || LogicFireStrength) && (HasExplosives || Bow || Hookshot);}),
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_ProgressiveBombBag,  []{return AddedProgressiveBombBags  == 0;}),
                    AdvancementPairing(A_ProgressiveBombchus, []{return AddedProgressiveBombBags  == 0;}),
                    AdvancementPairing(A_ProgressiveStrength, []{return AddedProgressiveStrengths == 0;}),
                    AdvancementPairing(A_ProgressiveBow,      []{return AddedProgressiveBows      == 0 && IsAdult;}),
                    AdvancementPairing(A_ProgressiveHookshot, []{return AddedProgressiveHookshots == 0 && IsAdult;}),
                    AdvancementPairing(A_SongOfTime,          []{return ProgressiveOcarina        >= 1 && IsAdult;}),
                    AdvancementPairing(FireTemple_SmallKey,   []{return true;}, 4),

  });

  Exit FireTemple_Middle = Exit("Fire Temple Middle", "Fire Temple", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&FireTemple_BoulderMazeLowerChest,    []{return true;}),
                    ItemLocationPairing(&FireTemple_BoulderMazeUpperChest,    []{return SmallKeys(FireTempleKeys, 6);}),
                    ItemLocationPairing(&FireTemple_BoulderMazeSideRoomChest, []{return true;}),
                    ItemLocationPairing(&FireTemple_BoulderMazeShortcutChest, []{return SmallKeys(FireTempleKeys, 6) && HasExplosives;}),
                    ItemLocationPairing(&FireTemple_ScarecrowChest,           []{return SmallKeys(FireTempleKeys, 6) && (CanUse("Scarecrow") || (LogicFireScarecrow && CanUse("Longshot")));}),
                    ItemLocationPairing(&FireTemple_MapChest,                 []{return SmallKeys(FireTempleKeys, 6) || (SmallKeys(FireTempleKeys, 5) && CanUse("Bow"));}),
                    ItemLocationPairing(&FireTemple_CompassChest,             []{return SmallKeys(FireTempleKeys, 6);}),
                    ItemLocationPairing(&FireTemple_GS_BoulderMaze,           []{return SmallKeys(FireTempleKeys, 4) && HasExplosives;}),
                    ItemLocationPairing(&FireTemple_GS_ScarecrowClimb,        []{return SmallKeys(FireTempleKeys, 6) && (CanUse("Scarecrow") || (LogicFireScarecrow && CanUse("Longshot")));}),
                    ItemLocationPairing(&FireTemple_GS_ScarecrowTop,          []{return SmallKeys(FireTempleKeys, 6) && (CanUse("Scarecrow") || (LogicFireScarecrow && CanUse("Longshot")));}),
                  }, {
                    //Exits
                    ExitPairing::Both(&FireTemple_Upper, []{return SmallKeys(FireTempleKeys, 8) || (SmallKeys(FireTempleKeys, 7) && ((CanUse("Hover Boots") && CanUse("Hammer")) || LogicFireFlameMaze));})
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_ProgressiveBombBag,  []{return AddedProgressiveBombBags  == 0;}),
                    AdvancementPairing(A_ProgressiveBombchus, []{return AddedProgressiveBombBags  == 0;}),
                    AdvancementPairing(A_ProgressiveBow,      []{return AddedProgressiveBows      == 0 && IsAdult && SmallKeys(FireTempleKeys, 5);}),
                    AdvancementPairing(A_ProgressiveHookshot, []{return AddedProgressiveHookshots == 0 && IsAdult && CanPlay(ScarecrowSong);}),
                    AdvancementPairing(A_ProgressiveHookshot, []{return AddedProgressiveHookshots == 1 && IsAdult && LogicFireScarecrow;}),
                    AdvancementPairing(A_MegatonHammer,       []{return IsAdult && !LogicFireFlameMaze && SmallKeys(FireTempleKeys, 7);}),
                    AdvancementPairing(A_HoverBoots,          []{return IsAdult && !LogicFireFlameMaze && SmallKeys(FireTempleKeys, 7);}),
                    AdvancementPairing(FireTemple_SmallKey,   []{return true;}, 8),
  });

  Exit FireTemple_Upper = Exit("Fire Temple Upper", "Fire Temple", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&FireTemple_HighestGoronChest,  []{return CanUse("Hammer") && CanPlay(SongOfTime);}),
                    ItemLocationPairing(&FireTemple_MegatonHammerChest, []{return HasExplosives;}),
                  }, {}, {
                    //Advancement Needs
                    AdvancementPairing(A_SongOfTime, []{return ProgressiveOcarina >= 1 && IsAdult;}),
  });

  Exit WaterTemple_Lobby = Exit("Water Temple Lobby", "Water Temple", "", NO_DAY_NIGHT_CYCLE, {
                    //Events
                    EventPairing(&ChildWaterTemple, []{return IsChild;}),
                  }, {}, {
                    //Exits
                    ExitPairing::Both(&LH_Main,                       []{return true;}),
                    ExitPairing::Both(&WaterTemple_HighestWaterLevel, []{return IsAdult;}),
                  }, {
  });

  Exit WaterTemple_HighestWaterLevel = Exit("Water Temple Highest Water Level", "Water Temple", "", NO_DAY_NIGHT_CYCLE, {
                    //Events
                    EventPairing(&FairyPot,         []{return FairyPot           || CanUse("Longshot");}),
                    EventPairing(&WaterTempleClear, []{return BossKeyWaterTemple && CanUse("Longshot");})
                  }, {
                    //Locations
                    ItemLocationPairing(&Morpha,                  []{return BossKeyWaterTemple && CanUse("Longshot");}),
                    ItemLocationPairing(&WaterTemple_MorphaHeart, []{return BossKeyWaterTemple && CanUse("Longshot");}),
                  }, {
                    //Exits
                    ExitPairing::Both(&WaterTemple_DarkLinkRegion, []{return SmallKeys(WaterTempleKeys, 4) && CanUse("Hookshot");}),
                    ExitPairing::Both(&WaterTemple_Dive,           []{return (CanUse("Zora Tunic") || LogicFewerTunicRequirements) && ((LogicWaterTempleTorchLongshot && CanUse("Longshot")) || IronBoots);}),
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_ZoraTunic,            []{return IsAdult && !LogicFewerTunicRequirements;}),
                    AdvancementPairing(A_IronBoots,            []{return IsAdult;}),
                    AdvancementPairing(WaterTemple_SmallKey,   []{return true;}, 4),
                    AdvancementPairing(WaterTemple_BossKey,    []{return true;}),
  });

  Exit WaterTemple_Dive = Exit("Water Temple Dive", "Water Temple", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&WaterTemple_MapChest,              []{return CanUse("Hover Boots") || CanUse("Hookshot") || CanUse("Bow");}),
                    ItemLocationPairing(&WaterTemple_CompassChest,          []{return (CanPlay(ZeldasLullaby) || IronBoots) && CanUse("Hookshot");}),
                    ItemLocationPairing(&WaterTemple_TorchesChest,          []{return (Bow || CanUse("Dins Fire") || (ChildWaterTemple && Sticks && KokiriSword && MagicMeter)) && CanPlay(ZeldasLullaby);}),
                    ItemLocationPairing(&WaterTemple_CentralBowTargetChest, []{return GoronBracelet && CanPlay(ZeldasLullaby) && ((Bow && (LogicWaterCentralBow || HoverBoots || CanUse("Longshot"))));}),
                    ItemLocationPairing(&WaterTemple_GS_BehindGate,         []{return (CanUse("Hookshot") || CanUse("Hover Boots")) && (HasExplosives && CanPlay(ZeldasLullaby)) && (CanUse("Iron Boots") || CanDive);}),
                  }, {
                    //Exits
                    ExitPairing::Both(&WaterTemple_CrackedWall,      []{return CanPlay(ZeldasLullaby) && (CanUse("Hookshot") || CanUse("Hover Boots")) && (LogicWaterCrackedWallNothing || (LogicWaterCrackedWallHovers && CanUse("Hover Boots")));}),
                    ExitPairing::Both(&WaterTemple_MiddleWaterLevel, []{return (Bow || CanUse("Dins Fire") || (SmallKeys(WaterTempleKeys, 5) && CanUse("Hookshot")) || (ChildWaterTemple && Sticks)) && CanPlay(ZeldasLullaby);}),
                    ExitPairing::Both(&WaterTemple_NorthBasement,    []{return SmallKeys(WaterTempleKeys, 5) && (CanUse("Longshot") || (LogicWaterBossKeyRegion && CanUse("Hover Boots"))) && (CanUse("Iron Boots") || CanPlay(ZeldasLullaby));}),
                    ExitPairing::Both(&WaterTemple_DragonStatue,     []{return CanPlay(ZeldasLullaby) && GoronBracelet && ((IronBoots && CanUse("Hookshot")) || (LogicWaterDragonBombchu && HasBombchus && CanDive));}),
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_HoverBoots,          []{return IsAdult;}),
                    AdvancementPairing(A_ProgressiveHookshot, []{return IsAdult && AddedProgressiveHookshots == 0;}),
                    AdvancementPairing(A_ProgressiveHookshot, []{return IsAdult && AddedProgressiveHookshots == 1;}),
                    AdvancementPairing(A_ProgressiveBow,      []{return IsAdult && AddedProgressiveBows      == 0;}),
                    AdvancementPairing(A_ZeldasLullaby,       []{return IsAdult && ProgressiveOcarina        >= 1;}),
                    AdvancementPairing(A_ProgressiveStrength, []{return IsAdult && AddedProgressiveStrengths == 0;}),
                    AdvancementPairing(A_DinsFire,            []{return IsAdult && ProgressiveMagic          >= 1 && AddedProgressiveBows == 0;}),
                    AdvancementPairing(A_ProgressiveMagic,    []{return IsAdult && AddedProgressiveMagics    == 0;}),
                    AdvancementPairing(A_ProgressiveBombchus, []{return LogicWaterDragonBombchu;}),
                    AdvancementPairing(WaterTemple_SmallKey,  []{return true;}, 6),
  });

  Exit WaterTemple_NorthBasement = Exit("Water Temple North Basement", "Water Temple", "", NO_DAY_NIGHT_CYCLE, {
                    //Events
                    EventPairing(&FairyPot, []{return FairyPot || (SmallKeys(WaterTempleKeys, 5) && (LogicWaterBKJumpDive || CanUse("Iron Boots")) && ((LogicWaterBKChest && IronBoots) || LogicWaterNorthBasementLedgeJump || (HasExplosives && GoronBracelet) || HoverBoots));}),
                  }, {
                    //Locations
                    ItemLocationPairing(&WaterTemple_BossKeyChest, []{return SmallKeys(WaterTempleKeys, 5) && (LogicWaterBKJumpDive || CanUse("Iron Boots")) && ((LogicWaterBKChest && IronBoots) || LogicWaterNorthBasementLedgeJump || (HasExplosives && GoronBracelet) || HoverBoots);}),
                    ItemLocationPairing(&WaterTemple_GS_NearBossKeyChest, []{return true;}),
                  }, {}, {
                    //Advancement Needs
                    AdvancementPairing(WaterTemple_SmallKey,  []{return true;}, 6),

  });

  Exit WaterTemple_CrackedWall = Exit("Water Temple Cracked Wall", "Water Temple", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&WaterTemple_CrackedWallChest, []{return HasExplosives;}),
                  }, {}, {
                    //Advancement Needs
                    AdvancementPairing(A_ProgressiveBombBag,  []{return AddedProgressiveBombBags == 0;}),
  });

  Exit WaterTemple_DragonStatue = Exit("Water Temple Dragon Statue", "Water Temple", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&WaterTemple_DragonChest, []{return true;}),
                  }, {}, {
  });

  Exit WaterTemple_MiddleWaterLevel = Exit("Water Temple Middle Water Level", "Water Temple", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&WaterTemple_CentralPillarChest, []{return CanUse("Zora Tunic") && CanUse("Hookshot") && ((SmallKeys(WaterTempleKeys, 5) || CanUse("Bow") || CanUse("Dins Fire")));}),
                    ItemLocationPairing(&WaterTemple_GS_CentralPillar,   []{return ((CanUse("Longshot") || (CanUse("Farores Wind") && CanUse("Hookshot"))) && (SmallKeys(WaterTempleKeys, 5) || CanUse("Bow") || CanUse("Dins Fire"))) || (ChildWaterTemple && Boomerang && CanUse("Farores Wind") && (CanUse("Dins Fire") || Sticks));}),
                  }, {
                    //Exits
                    ExitPairing::Both(&WaterTemple_CrackedWall, []{return true;}),
                  }, {
                    //Advancement Needs
                    AdvancementPairing(WaterTemple_SmallKey,  []{return true;}, 6),
  });

  Exit WaterTemple_DarkLinkRegion = Exit("Water Temple Dark Link Region", "Water Temple", "", NO_DAY_NIGHT_CYCLE, {
                    //Events
                    EventPairing(&FairyPot, []{return FairyPot || (SmallKeys(WaterTempleKeys, 5) && CanPlay(SongOfTime));}),
                  }, {
                    //Locations
                    ItemLocationPairing(&WaterTemple_LongshotChest,          []{return SmallKeys(WaterTempleKeys, 5);}),
                    ItemLocationPairing(&WaterTemple_RiverChest,             []{return SmallKeys(WaterTempleKeys, 5) && CanPlay(SongOfTime) && Bow;}),
                    ItemLocationPairing(&WaterTemple_GS_River,               []{return CanPlay(SongOfTime) && SmallKeys(WaterTempleKeys, 5) && (IronBoots || (LogicWaterRiverGS && CanUse("Longshot") && (Bow || HasBombchus)));}),
                    ItemLocationPairing(&WaterTemple_GS_FallingPlatformRoom, []{return CanUse("Longshot") || (LogicWaterFallingPlatformGS && CanUse("Hookshot"));}),
                  }, {
                    //Exits
                    ExitPairing::Both(&WaterTemple_DragonStatue, []{return SmallKeys(WaterTempleKeys, 5) && CanPlay(SongOfTime) && Bow && ((IronBoots && (CanUse("Zora Tunic") || LogicFewerTunicRequirements)) || LogicWaterDragonJumpDive || (LogicWaterDragonBombchu && HasBombchus && CanDive));}),
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_SongOfTime,         []{return ProgressiveOcarina >= 1;}),
                    AdvancementPairing(WaterTemple_SmallKey, []{return true;}, 6),
  });

  Exit SpiritTemple_Lobby = Exit("Spirit Temple Lobby", "Spirit Temple", "", NO_DAY_NIGHT_CYCLE, {}, {}, {
                    //Exits
                    ExitPairing::Both(&Colossus_Main,           []{return true;}),
                    ExitPairing::Both(&SpiritTemple_Child,      []{return IsChild;}),
                    ExitPairing::Both(&SpiritTemple_EarlyAdult, []{return CanUse("Silver Gauntlets");}),
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_ProgressiveStrength, []{return IsAdult && AddedProgressiveStrengths == 0;}),
                    AdvancementPairing(A_ProgressiveStrength, []{return IsAdult && AddedProgressiveStrengths == 1;}),
  });

  Exit SpiritTemple_Child = Exit("Child Spirit Temple", "Spirit Temple", "", NO_DAY_NIGHT_CYCLE, {
                    //Events
                    EventPairing(&NutCrate, []{return true;}),
                  }, {
                    //Locations
                    ItemLocationPairing(&SpiritTemple_ChildBridgeChest,       []{return (Boomerang || Slingshot || (HasExplosives && LogicSpiritChildBombchu)) && (Sticks || HasExplosives || ((Nuts || Boomerang) && (KokiriSword || Slingshot)));}),
                    ItemLocationPairing(&SpiritTemple_ChildEarlyTorchesChest, []{return (Boomerang || Slingshot || (HasExplosives && LogicSpiritChildBombchu)) && (Sticks || HasExplosives || ((Nuts || Boomerang) && (KokiriSword || Slingshot))) && (Sticks || CanUse("Dins Fire"));}),
                    ItemLocationPairing(&SpiritTemple_GS_MetalFence,          []{return (Boomerang || Slingshot || (HasExplosives && LogicSpiritChildBombchu)) && (Sticks || HasExplosives || ((Nuts || Boomerang) && (KokiriSword || Slingshot)));}),
                  }, {
                    //Exits
                    ExitPairing::Both(&SpiritTemple_ChildClimb, []{return SmallKeys(SpiritTempleKeys, 1);}),
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_Boomerang,            []{return !Slingshot || (!HasExplosives && LogicSpiritChildBombchu);}),
                    AdvancementPairing(A_ProgressiveBulletBag, []{return (!Boomerang || (!HasExplosives && LogicSpiritChildBombchu)) && AddedProgressiveBulletBags == 0;}),
                    AdvancementPairing(A_KokiriSword,          []{return !Slingshot;}),
                    AdvancementPairing(A_ProgressiveMagic,     []{return AddedProgressiveMagics   == 0 && !Sticks;}),
                    AdvancementPairing(A_DinsFire,             []{return AddedProgressiveMagics   >= 1 && !Sticks;}),
                    AdvancementPairing(A_ProgressiveBombchus,  []{return AddedProgressiveBombBags == 0;}),
                    AdvancementPairing(A_ProgressiveBombBag,   []{return AddedProgressiveBombBags == 0;}),
                    AdvancementPairing(SpiritTemple_SmallKey,  []{return true;}),
  });

  Exit SpiritTemple_ChildClimb = Exit("Child Spirit Temple Climb", "Spirit Temple", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&SpiritTemple_ChildClimbNorthChest, []{return HasProjectile("Both") || ((SmallKeys(SpiritTempleKeys, 3) || (SmallKeys(SpiritTempleKeys, 2) && BombchusInLogic && !ShuffleDungeonEntrances)) && CanUse("Silver Gauntlets") && HasProjectile("Adult")) || (SmallKeys(SpiritTempleKeys, 5) && IsChild && HasProjectile("Child"));}),
                    ItemLocationPairing(&SpiritTemple_ChildClimbEastChest,  []{return HasProjectile("Both") || ((SmallKeys(SpiritTempleKeys, 3) || (SmallKeys(SpiritTempleKeys, 2) && BombchusInLogic && !ShuffleDungeonEntrances)) && CanUse("Silver Gauntlets") && HasProjectile("Adult")) || (SmallKeys(SpiritTempleKeys, 5) && IsChild && HasProjectile("Child"));}),
                    ItemLocationPairing(&SpiritTemple_GS_SunOnFloorRoom,    []{return HasProjectile("Both") || CanUse("Dins Fire") || ((DamageMultiplier != DAMAGEMULTIPLIER_OHKO || Fairy || CanUse("Nayrus Love")) && (Sticks || KokiriSword || HasProjectile("Child"))) || (IsChild && SmallKeys(SpiritTempleKeys, 5) && HasProjectile("Child")) || ((SmallKeys(SpiritTempleKeys, 3) || (SmallKeys(SpiritTempleKeys, 2) && BombchusInLogic && !ShuffleDungeonEntrances)) && CanUse("Silver Gauntlets") && (HasProjectile("Adult") || DamageMultiplier != DAMAGEMULTIPLIER_OHKO || Fairy || CanUse("Nayrus Love")));}),
                  }, {
                    //Exits
                    ExitPairing::Both(&SpiritTemple_CentralChamber, []{return HasExplosives;}),
                  }, {
                    //Advancement Needs
                    AdvancementPairing(SpiritTemple_SmallKey, []{return true;}, 5),
  });

  Exit SpiritTemple_EarlyAdult = Exit("Early Adult Spirit Temple", "Spirit Temple", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&SpiritTemple_CompassChest,          []{return CanUse("Hookshot") && CanPlay(ZeldasLullaby);}),
                    ItemLocationPairing(&SpiritTemple_EarlyAdultRightChest,  []{return Bow || Hookshot || HasBombchus || (Bombs && LogicSpiritLowerAdultSwitch);}),
                    ItemLocationPairing(&SpiritTemple_FirstMirrorLeftChest,  []{return SmallKeys(SpiritTempleKeys, 3);}),
                    ItemLocationPairing(&SpiritTemple_FirstMirrorRightChest, []{return SmallKeys(SpiritTempleKeys, 3);}),
                    ItemLocationPairing(&SpiritTemple_GS_BoulderRoom,        []{return CanPlay(SongOfTime) && (Bow || Hookshot || HasBombchus || (Bombs && LogicSpiritLowerAdultSwitch));}),
                  }, {
                    //Exits
                    ExitPairing::Both(&SpiritTemple_CentralChamber, []{return SmallKeys(SpiritTempleKeys, 1);}),
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_ZeldasLullaby,       []{return ProgressiveOcarina        >= 1;}),
                    AdvancementPairing(A_ProgressiveHookshot, []{return IsAdult && AddedProgressiveHookshots == 0;}),
                    AdvancementPairing(A_ProgressiveBow,      []{return IsAdult && AddedProgressiveHookshots == 0 && AddedProgressiveBows == 0;}),
                    AdvancementPairing(A_ProgressiveBombchus, []{return AddedProgressiveBombBags  == 0;}),
                    AdvancementPairing(A_ProgressiveBombBag,  []{return AddedProgressiveBombBags  == 0;}),
                    AdvancementPairing(SpiritTemple_SmallKey, []{return true;}, 3),
  });

  Exit SpiritTemple_CentralChamber = Exit("Spirit Temple Central Chamber", "Spirit Temple", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&SpiritTemple_MapChest,                 []{return ((HasExplosives || SmallKeys(SpiritTempleKeys, 3) || (SmallKeys(SpiritTempleKeys, 2) && BombchusInLogic && !ShuffleDungeonEntrances)) && (CanUse("Dins Fire") || (((MagicMeter && FireArrows) || LogicSpiritMapChest) && Bow && Sticks))) || (SmallKeys(SpiritTempleKeys, 5) && HasExplosives && CanUse("Sticks")) || (SmallKeys(SpiritTempleKeys, 3) && (CanUse("Fire Arrows") || (LogicSpiritMapChest && Bow)) && CanUse("Silver Gauntlets"));}),
                    ItemLocationPairing(&SpiritTemple_SunBlockRoomChest,        []{return ((HasExplosives || SmallKeys(SpiritTempleKeys, 3) || (SmallKeys(SpiritTempleKeys, 2) && BombchusInLogic && !ShuffleDungeonEntrances)) && (CanUse("Dins Fire") || (((MagicMeter && FireArrows) || LogicSpiritMapChest) && Bow && Sticks))) || (SmallKeys(SpiritTempleKeys, 5) && HasExplosives && CanUse("Sticks")) || (SmallKeys(SpiritTempleKeys, 3) && (CanUse("Fire Arrows") || (LogicSpiritMapChest && Bow)) && CanUse("Silver Gauntlets"));}),
                    ItemLocationPairing(&SpiritTemple_StatueRoomHandChest,      []{return SmallKeys(SpiritTempleKeys, 3) && CanUse("Silver Gauntlets") && CanPlay(ZeldasLullaby);}),
                    ItemLocationPairing(&SpiritTemple_StatueRoomNortheastChest, []{return SmallKeys(SpiritTempleKeys, 3) && CanUse("Silver Gauntlets") && CanPlay(ZeldasLullaby) && (Hookshot || HoverBoots);}),
                    ItemLocationPairing(&SpiritTemple_GS_HallAfterSunBlockRoom, []{return (HasExplosives && Boomerang && Hookshot) || (CanUse("Boomerang") && SmallKeys(SpiritTempleKeys, 5) && HasExplosives) || (Hookshot && CanUse("Silver Gauntlets") && (SmallKeys(SpiritTempleKeys, 3) || (SmallKeys(SpiritTempleKeys, 2) && Boomerang && BombchusInLogic && !ShuffleDungeonEntrances)));}),
                    ItemLocationPairing(&SpiritTemple_GS_Lobby,                 []{return ((HasExplosives || SmallKeys(SpiritTempleKeys, 3) || (SmallKeys(SpiritTempleKeys, 2) && BombchusInLogic && !ShuffleDungeonEntrances)) && LogicSpiritLobbyGS && Boomerang && (Hookshot || HoverBoots)) || (LogicSpiritLobbyGS && SmallKeys(SpiritTempleKeys, 5) && HasExplosives && CanUse("Boomerang")) || (SmallKeys(SpiritTempleKeys, 3) && CanUse("Silver Gauntlets") && (Hookshot || HoverBoots));}),
                  }, {
                    //Exits
                    ExitPairing::Both(&SpiritTemple_OutdoorHands,            []{return true;}),
                    ExitPairing::Both(&SpiritTemple_BeyondCentralLockedDoor, []{return SmallKeys(SpiritTempleKeys, 4) && CanUse("Silver Gauntlets");}),
                    ExitPairing::Both(&SpiritTemple_ChildClimb,              []{return true;}),
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_FireArrows, []{return IsAdult && AddedProgressiveBows >= 1 && LogicSpiritMapChest;}),
  });

  Exit SpiritTemple_OutdoorHands = Exit("Spirit Temple Outdoor Hands", "Spirit Temple", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&SpiritTemple_SilverGauntletsChest, []{return (SmallKeys(SpiritTempleKeys, 3) && Longshot && HasExplosives) || SmallKeys(SpiritTempleKeys, 5);}),
                    ItemLocationPairing(&SpiritTemple_MirrorShieldChest,    []{return SmallKeys(SpiritTempleKeys, 4) && CanUse("Silver Gauntlets") && HasExplosives;}),
                  }, {
                    //Exits
                    ExitPairing::Both(&Colossus_Main, []{return (IsChild && SmallKeys(SpiritTempleKeys, 5)) || (CanUse("Silver Gauntlets") && ((SmallKeys(SpiritTempleKeys, 3) && HasExplosives) || SmallKeys(SpiritTempleKeys, 5)));}),
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_ProgressiveHookshot, []{return IsAdult && AddedProgressiveHookshots == 1;}),
                    AdvancementPairing(SpiritTemple_SmallKey, []{return true;}, 5),
  });

  Exit SpiritTemple_BeyondCentralLockedDoor = Exit("Spirit Temple Beyond Central Locked Door", "Spirit Temple", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&SpiritTemple_NearFourArmosChest,         []{return MirrorShield && HasExplosives;}),
                    ItemLocationPairing(&SpiritTemple_HallwayLeftInvisibleChest,  []{return (LogicLensSpirit || CanUse("Lens of Truth")) && HasExplosives;}),
                    ItemLocationPairing(&SpiritTemple_HallwayRightInvisibleChest, []{return (LogicLensSpirit || CanUse("Lens of Truth")) && HasExplosives;}),
                  }, {
                    //Exits
                    ExitPairing::Both(&SpiritTemple_BeyondFinalLockedDoor, []{return SmallKeys(SpiritTempleKeys, 5) && (LogicSpiritWall || CanUse("Longshot") || HasBombchus || ((Bombs || Nuts || CanUse("Dins Fire")) && (Bow || CanUse("Hookshot") || Hammer)));}),
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_MirrorShield, []{return IsAdult;}),
                    AdvancementPairing(A_LensOfTruth,  []{return AddedProgressiveMagics >= 1 && !LogicLensSpirit;}),
  });

  Exit SpiritTemple_BeyondFinalLockedDoor = Exit("Spirit Temple Beyond Final Locked Door", "Spirit Temple", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&SpiritTemple_BossKeyChest,  []{return CanPlay(ZeldasLullaby) && Bow && Hookshot;}),
                    ItemLocationPairing(&SpiritTemple_TopmostChest,  []{return MirrorShield;}),
                    ItemLocationPairing(&SpiritTemple_TwinrovaHeart, []{return MirrorShield && HasExplosives && Hookshot && BossKeySpiritTemple;}),
                    ItemLocationPairing(&Twinrova,                   []{return MirrorShield && HasExplosives && Hookshot && BossKeySpiritTemple;}),
                  }, {}, {
                    //Advancement Needs
                    AdvancementPairing(SpiritTemple_BossKey, []{return true;}),
  });

  Exit ShadowTemple_Entryway = Exit("Shadow Temple Entryway", "Shadow Temple", "", NO_DAY_NIGHT_CYCLE, {}, {}, {
                    //Exits
                    ExitPairing::Both(&GY_WarpPadRegion,       []{return true;}),
                    ExitPairing::Both(&ShadowTemple_Beginning, []{return (LogicLensShadow || CanUse("Lens of Truth")) && (CanUse("Hover Boots") || CanUse("Hookshot"));}),
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_ProgressiveMagic,    []{return IsAdult && AddedProgressiveMagics    == 0 && !LogicLensShadow;}),
                    AdvancementPairing(A_LensOfTruth,         []{return IsAdult && ProgressiveMagic          >= 1 && !LogicLensShadow;}),
                    AdvancementPairing(A_ProgressiveHookshot, []{return IsAdult && AddedProgressiveHookshots == 0 && !HoverBoots;}),
                    AdvancementPairing(A_HoverBoots,          []{return IsAdult && AddedProgressiveHookshots == 0;}),
  });

  Exit ShadowTemple_Beginning = Exit("Shadow Temple Beginning", "Shadow Temple", "", NO_DAY_NIGHT_CYCLE, {
                    //Events
                    EventPairing(&NutPot, []{return true;}),
                  }, {
                    //Locations
                    ItemLocationPairing(&ShadowTemple_MapChest,        []{return true;}),
                    ItemLocationPairing(&ShadowTemple_HoverBootsChest, []{return true;}),
                  }, {
                    //Exits
                    ExitPairing::Both(&ShadowTemple_Entryway,    []{return true;}),
                    ExitPairing::Both(&ShadowTemple_FirstBeamos, []{return HoverBoots;}),
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_HoverBoots, []{return true;}),
  });

  Exit ShadowTemple_FirstBeamos = Exit("Shadow Temple First Beamos", "Shadow Temple", "", NO_DAY_NIGHT_CYCLE, {
                    //Events
                    EventPairing(&FairyPot, []{return true;}), //This fairy pot is only on 3DS
                  }, {
                    //Locations
                    ItemLocationPairing(&ShadowTemple_CompassChest,          []{return true;}),
                    ItemLocationPairing(&ShadowTemple_EarlySilverRupeeChest, []{return true;}),
                  }, {
                    //Exits
                    ExitPairing::Both(&ShadowTemple_HugePit, []{return HasExplosives && SmallKeys(ShadowTempleKeys, 1);})
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_ProgressiveBombchus, []{return AddedProgressiveBombBags == 0;}),
                    AdvancementPairing(A_ProgressiveBombBag,  []{return AddedProgressiveBombBags == 0;}),
                    AdvancementPairing(ShadowTemple_SmallKey, []{return true;}, 1),
  });

  Exit ShadowTemple_HugePit = Exit("Shadow Temple Huge Pit", "Shadow Temple", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&ShadowTemple_InvisibleBladesVisibleChest,   []{return true;}),
                    ItemLocationPairing(&ShadowTemple_InvisibleBladesInvisibleChest, []{return true;}),
                    ItemLocationPairing(&ShadowTemple_FallingSpikesLowerChest,       []{return true;}),
                    ItemLocationPairing(&ShadowTemple_FallingSpikesUpperChest,       []{return LogicShadowUmbrella || GoronBracelet;}),
                    ItemLocationPairing(&ShadowTemple_FallingSpikesSwitchChest,      []{return LogicShadowUmbrella || GoronBracelet;}),
                    ItemLocationPairing(&ShadowTemple_InvisibleSpikesChest,          []{return SmallKeys(ShadowTempleKeys, 2) && (LogicLensShadowBack || CanUse("Lens of Truth"));}),
                    ItemLocationPairing(&ShadowTemple_FreestandingKey,               []{return SmallKeys(ShadowTempleKeys, 2) && (LogicLensShadowBack || CanUse("Lens of Truth")) && Hookshot && (Bombs || GoronBracelet || (LogicShadowFreestandingKey && HasBombchus));}),
                    ItemLocationPairing(&ShadowTemple_GS_LikeLikeRoom,               []{return true;}),
                    ItemLocationPairing(&ShadowTemple_GS_FallingSpikesRoom,          []{return Hookshot;}),
                    ItemLocationPairing(&ShadowTemple_GS_SingleGiantPot,             []{return SmallKeys(ShadowTempleKeys, 2) && (LogicLensShadowBack || CanUse("Lens of Truth")) && Hookshot;}),
                  }, {
                    //Exits
                    ExitPairing::Both(&ShadowTemple_WindTunnel, []{return (LogicLensShadowBack || CanUse("Lens of Truth")) && Hookshot && SmallKeys(ShadowTempleKeys, 3);}),
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_ProgressiveMagic,    []{return IsAdult && AddedProgressiveMagics    == 0 && !LogicLensShadowBack;}),
                    AdvancementPairing(A_LensOfTruth,         []{return IsAdult && ProgressiveMagic          >= 1 && !LogicLensShadowBack;}),
                    AdvancementPairing(A_ProgressiveHookshot, []{return IsAdult && AddedProgressiveHookshots == 0;}),
                    AdvancementPairing(A_ProgressiveStrength, []{return AddedProgressiveStrengths == 0 && (!HasExplosives || !LogicShadowUmbrella);}),
                    AdvancementPairing(ShadowTemple_SmallKey, []{return true;}, 2),
  });

  Exit ShadowTemple_WindTunnel = Exit("Shadow Temple Wind Tunnel", "Shadow Temple", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&ShadowTemple_WindHintChest,        []{return true;}),
                    ItemLocationPairing(&ShadowTemple_AfterWindEnemyChest,  []{return true;}),
                    ItemLocationPairing(&ShadowTemple_AfterWindHiddenChest, []{return true;}),
                    ItemLocationPairing(&ShadowTemple_GS_NearShip,          []{return CanUse("Longshot") && SmallKeys(ShadowTempleKeys, 4);}),
                  }, {
                    //Exits
                    ExitPairing::Both(&ShadowTemple_BeyondBoat, []{return CanPlay(ZeldasLullaby) && SmallKeys(ShadowTempleKeys, 4);}),
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_ZeldasLullaby,       []{return ProgressiveOcarina       >= 1;}),
                    AdvancementPairing(ShadowTemple_SmallKey, []{return true;}, 4),
  });

  Exit ShadowTemple_BeyondBoat = Exit("Shadow Temple Beyond Boat", "Shadow Temple", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&ShadowTemple_SpikeWallsLeftChest,       []{return CanUse("Dins Fire");}),
                    ItemLocationPairing(&ShadowTemple_BossKeyChest,              []{return CanUse("Dins Fire");}),
                    ItemLocationPairing(&ShadowTemple_InvisibleFloormasterChest, []{return true;}),
                    ItemLocationPairing(&ShadowTemple_BongoBongoHeart,           []{return SmallKeys(ShadowTempleKeys, 5) && BossKeyShadowTemple && (Bow || CanUse("Distant Scarecrow") || (LogicShadowStatue && HasBombchus));}),
                    ItemLocationPairing(&BongoBongo,                             []{return SmallKeys(ShadowTempleKeys, 5) && BossKeyShadowTemple && (Bow || CanUse("Distant Scarecrow") || (LogicShadowStatue && HasBombchus));}),
                    ItemLocationPairing(&ShadowTemple_GS_TripleGiantPot,         []{return true;}),
                  }, {}, {
                    //Advancement Needs
                    AdvancementPairing(A_ProgressiveMagic,    []{return AddedProgressiveMagics == 0;}),
                    AdvancementPairing(A_DinsFire,            []{return ProgressiveMagic       >= 1;}),
                    AdvancementPairing(ShadowTemple_SmallKey, []{return true;}, 5),
  });

  Exit BottomOfTheWell = Exit("Bottom of the Well", "Bottom of the Well", "", NO_DAY_NIGHT_CYCLE, {}, {}, {
                    //Exits
                    ExitPairing::Both(&Kak_Main,                 []{return true;}),
                    ExitPairing::Both(&BottomOfTheWell_MainArea, []{return IsChild && (CanChildAttack || Nuts);}),
                  }, {
                    //Advancement Needs
  });

  Exit BottomOfTheWell_MainArea = Exit("Bottom of the Well Main Area", "Bottom of the Well", "", NO_DAY_NIGHT_CYCLE, {
                    //Events
                    EventPairing(&StickPot, []{return true;}),
                    EventPairing(&NutPot,   []{return true;}),
                  }, {
                    //Locations
                    ItemLocationPairing(&BottomOfTheWell_FrontLeftFakeWallChest,   []{return LogicLensBotw || CanUse("Lens of Truth");}),
                    ItemLocationPairing(&BottomOfTheWell_FrontCenterBombableChest, []{return HasExplosives;}),
                    ItemLocationPairing(&BottomOfTheWell_RightBottomFakeWallChest, []{return LogicLensBotw || CanUse("Lens of Truth");}),
                    ItemLocationPairing(&BottomOfTheWell_CompassChest,             []{return LogicLensBotw || CanUse("Lens of Truth");}),
                    ItemLocationPairing(&BottomOfTheWell_CenterSkulltulaChest,     []{return LogicLensBotw || CanUse("Lens of Truth");}),
                    ItemLocationPairing(&BottomOfTheWell_BackLeftBombableChest,    []{return (LogicLensBotw || CanUse("Lens of Truth")) && HasExplosives;}),
                    ItemLocationPairing(&BottomOfTheWell_FreestandingKey,          []{return Sticks || CanUse("Dins Fire");}),
                    ItemLocationPairing(&BottomOfTheWell_LensOfTruthChest,         []{return CanPlay(ZeldasLullaby) && (KokiriSword || (Sticks && LogicChildDeadhand));}),
                    ItemLocationPairing(&BottomOfTheWell_InvisibleChest,           []{return CanPlay(ZeldasLullaby) && (LogicLensBotw || CanUse("Lens of Truth"));}),
                    ItemLocationPairing(&BottomOfTheWell_UnderwaterFrontChest,     []{return CanPlay(ZeldasLullaby);}),
                    ItemLocationPairing(&BottomOfTheWell_UnderwaterLeftChest,      []{return CanPlay(ZeldasLullaby);}),
                    ItemLocationPairing(&BottomOfTheWell_MapChest,                 []{return HasExplosives || (((SmallKeys(BottomOfTheWellKeys, 3) && (LogicLensBotw || CanUse("Lens of Truth"))) || CanUse("Dins Fire")) && GoronBracelet);}),
                    ItemLocationPairing(&BottomOfTheWell_FireKeeseChest,           []{return SmallKeys(BottomOfTheWellKeys, 3) && (LogicLensBotw || CanUse("Lens of Truth"));}),
                    ItemLocationPairing(&BottomOfTheWell_LikeLikeChest,            []{return SmallKeys(BottomOfTheWellKeys, 3) && (LogicLensBotw || CanUse("Lens of Truth"));}),
                    ItemLocationPairing(&BottomOfTheWell_GS_WestInnerRoom,         []{return Boomerang && (LogicLensBotw || CanUse("Lens of Truth")) && SmallKeys(BottomOfTheWellKeys, 3);}),
                    ItemLocationPairing(&BottomOfTheWell_GS_EastInnerRoom,         []{return Boomerang && (LogicLensBotw || CanUse("Lens of Truth")) && SmallKeys(BottomOfTheWellKeys, 3);}),
                    ItemLocationPairing(&BottomOfTheWell_GS_LikeLikeCage,          []{return SmallKeys(BottomOfTheWellKeys, 3) && (LogicLensBotw || CanUse("Lens of Truth")) && (Boomerang || (LogicBotwCageGS && CanChildAttack));}),
                  }, {
                    //Exits
                    ExitPairing::Both(&BottomOfTheWell, []{return true;}),
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_LensOfTruth,            []{return ProgressiveMagic         >= 1 && !LogicLensBotw;}),
                    AdvancementPairing(A_ProgressiveBombchus,    []{return AddedProgressiveBombBags == 0;}),
                    AdvancementPairing(A_ProgressiveBombBag,     []{return AddedProgressiveBombBags == 0;}),
                    AdvancementPairing(A_ZeldasLullaby,          []{return ProgressiveOcarina       >= 1;}),
                    AdvancementPairing(A_ProgressiveMagic,       []{return AddedProgressiveMagics   == 0;}),
                    AdvancementPairing(A_DinsFire,               []{return ProgressiveMagic         >= 1;}),
                    AdvancementPairing(A_ProgressiveStrength,    []{return AddedProgressiveBombBags == 0 && AddedProgressiveStrengths == 0;}),
                    AdvancementPairing(BottomOfTheWell_SmallKey, []{return true;}, 3),
  });

  Exit IceCavern_Beginning = Exit("Ice Cavern Beginning", "Ice Cavern", "", NO_DAY_NIGHT_CYCLE, {}, {}, {
                    //Exits
                    ExitPairing::Both(&ZF_Main,        []{return true;}),
                    ExitPairing::Both(&IceCavern_Main, []{return IsAdult || HasExplosives || CanUse("Dins Fire");}),
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_ProgressiveBombchus, []{return IsChild && AddedProgressiveBombBags == 0 && AddedProgressiveMagics == 0;}),
                    AdvancementPairing(A_ProgressiveBombBag,  []{return IsChild && AddedProgressiveBombBags == 0 && AddedProgressiveMagics == 0;}),
                    AdvancementPairing(A_ProgressiveMagic,    []{return IsChild && AddedProgressiveBombBags == 0 && AddedProgressiveMagics == 0;}),
                    AdvancementPairing(A_DinsFire,            []{return IsChild && AddedProgressiveBombBags == 0 && ProgressiveMagic       >= 1;}),
  });

  Exit IceCavern_Main = Exit("Ice Cavern", "Ice Cavern", "", NO_DAY_NIGHT_CYCLE, {
                    //Events
                    EventPairing(&BlueFireAccess, []{return BlueFireAccess || (IsAdult && HasBottle);}),
                  }, {
                    //Locations
                    ItemLocationPairing(&IceCavern_MapChest,        []{return BlueFire && IsAdult;}),
                    ItemLocationPairing(&IceCavern_CompassChest,    []{return BlueFire;}),
                    ItemLocationPairing(&IceCavern_IronBootsChest,  []{return BlueFire && (IsAdult || Slingshot || Sticks || KokiriSword || CanUse("Dins Fire"));}),
                    //Sheik in Ice Cavern
                    ItemLocationPairing(&IceCavern_FreestandingPoH,       []{return BlueFire;}),
                    ItemLocationPairing(&IceCavern_GS_SpinningScytheRoom, []{return HookshotOrBoomerang;}),
                    ItemLocationPairing(&IceCavern_GS_HeartPieceRoom,     []{return BlueFire && HookshotOrBoomerang;}),
                    ItemLocationPairing(&IceCavern_GS_PushBlockRoom,      []{return BlueFire && HookshotOrBoomerang;}),
                  }, {}, {
                    //Advancement Needs
                    AdvancementPairing(A_EmptyBottle,          []{return !HasBottle;}),
                    AdvancementPairing(A_ProgressiveBulletBag, []{return AddedProgressiveBulletBags == 0 && !Sticks && !KokiriSword && AddedProgressiveMagics == 0;}),
                    AdvancementPairing(A_KokiriSword,          []{return AddedProgressiveBulletBags == 0 && !Sticks && !KokiriSword && AddedProgressiveMagics == 0;}),
                    AdvancementPairing(A_ProgressiveMagic,     []{return AddedProgressiveBulletBags == 0 && !Sticks && !KokiriSword && AddedProgressiveMagics == 0;}),
                    AdvancementPairing(A_DinsFire,             []{return AddedProgressiveBulletBags == 0 && !Sticks && !KokiriSword && ProgressiveMagic       >= 1;}),
  });

  Exit GerudoTrainingGrounds_Lobby = Exit ("Gerudo Training Grounds Lobby", "Gerudo Training Grounds", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&GerudoTrainingGrounds_LobbyLeftChest,  []{return CanUse("Bow") || CanUse("Slingshot");}),
                    ItemLocationPairing(&GerudoTrainingGrounds_LobbyRightChest, []{return CanUse("Bow") || CanUse("Slingshot");}),
                    ItemLocationPairing(&GerudoTrainingGrounds_StalfosChest,    []{return IsAdult || KokiriSword;}),
                    ItemLocationPairing(&GerudoTrainingGrounds_BeamosChest,     []{return HasExplosives && (IsAdult || KokiriSword);}),
                  }, {
                    //Exits
                    ExitPairing::Both(&GF_Main,                              []{return true;}),
                    ExitPairing::Both(&GerudoTrainingGrounds_HeavyBlockRoom, []{return (IsAdult || KokiriSword) && (CanUse("Hookshot") || LogicGtgWithoutHookshot);}),
                    ExitPairing::Both(&GerudoTrainingGrounds_LavaRoom,       []{return (IsAdult || KokiriSword) && HasExplosives;}),
                    ExitPairing::Both(&GerudoTrainingGrounds_CentralMaze,    []{return true;}),
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_ProgressiveBulletBag, []{return IsChild && AddedProgressiveBulletBags == 0;}),
                    AdvancementPairing(A_ProgressiveBow,       []{return IsAdult && AddedProgressiveBows       == 0;}),
                    AdvancementPairing(A_KokiriSword,          []{return IsChild;}),
                    AdvancementPairing(A_ProgressiveBombchus,  []{return AddedProgressiveBombBags  == 0;}),
                    AdvancementPairing(A_ProgressiveBombBag,   []{return AddedProgressiveBombBags  == 0;}),
                    AdvancementPairing(A_ProgressiveHookshot,  []{return AddedProgressiveHookshots == 0 && !LogicGtgWithoutHookshot;}),
  });

  Exit GerudoTrainingGrounds_CentralMaze = Exit ("Gerudo Training Grounds Central Maze", "Gerudo Training Grounds", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&GerudoTrainingGrounds_HiddenCeilingChest,  []{return SmallKeys(GerudoTrainingGroundsKeys, 3) && (LogicLensGtg || CanUse("Lens of Truth"));}),
                    ItemLocationPairing(&GerudoTrainingGrounds_MazePathFirstChest,  []{return SmallKeys(GerudoTrainingGroundsKeys, 4);}),
                    ItemLocationPairing(&GerudoTrainingGrounds_MazePathSecondChest, []{return SmallKeys(GerudoTrainingGroundsKeys, 6);}),
                    ItemLocationPairing(&GerudoTrainingGrounds_MazePathThirdChest,  []{return SmallKeys(GerudoTrainingGroundsKeys, 7);}),
                    ItemLocationPairing(&GerudoTrainingGrounds_MazePathFinalChest,  []{return SmallKeys(GerudoTrainingGroundsKeys, 9);}),
                  }, {
                    //Exits
                    ExitPairing::Both(&GerudoTrainingGrounds_CentralMazeRight, []{return SmallKeys(GerudoTrainingGroundsKeys, 9);}),
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_ProgressiveMagic,             []{return AddedProgressiveMagics == 0 && !LogicLensGtg;}),
                    AdvancementPairing(A_LensOfTruth,                  []{return ProgressiveMagic       >= 1 && !LogicLensGtg;}),
                    AdvancementPairing(GerudoTrainingGrounds_SmallKey, []{return true;}, 9)
  });

  Exit GerudoTrainingGrounds_CentralMazeRight = Exit("Gerudo Training Grounds Central Maze Right", "Gerudo Training Grounds", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&GerudoTrainingGrounds_MazeRightCentralChest, []{return true;}),
                    ItemLocationPairing(&GerudoTrainingGrounds_MazeRightSideChest,    []{return true;}),
                    ItemLocationPairing(&GerudoTrainingGrounds_FreestandingKey,       []{return true;}),
                  }, {
                    //Exits
                    ExitPairing::Both(&GerudoTrainingGrounds_HammerRoom, []{return CanUse("Hookshot");}),
                    ExitPairing::Both(&GerudoTrainingGrounds_LavaRoom,   []{return true;}),
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_ProgressiveHookshot,  []{return IsAdult;}),
  });

  Exit GerudoTrainingGrounds_LavaRoom = Exit("Gerudo Training Grounds Lava Room", "Gerudo Training Grounds", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&GerudoTrainingGrounds_UnderwaterSilverRupeeChest, []{return CanUse("Hookshot") && CanPlay(SongOfTime) && IronBoots && (LogicFewerTunicRequirements || CanUse("Zora Tunic"));}),
                  }, {
                    //Exits
                    ExitPairing::Both(&GerudoTrainingGrounds_CentralMazeRight, []{return CanPlay(SongOfTime) || IsChild;}),
                    ExitPairing::Both(&GerudoTrainingGrounds_HammerRoom,       []{return CanUse("Longshot")  || (CanUse("Hover Boots") && CanUse("Hookshot"));}),
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_SongOfTime,          []{return IsAdult && ProgressiveOcarina        >= 1;}),
                    AdvancementPairing(A_IronBoots,           []{return IsAdult;}),
                    AdvancementPairing(A_ZoraTunic,           []{return IsAdult &&!LogicFewerTunicRequirements;}),
                    AdvancementPairing(A_ProgressiveHookshot, []{return IsAdult && AddedProgressiveHookshots == 0;}),
                    AdvancementPairing(A_HoverBoots,          []{return IsAdult && AddedProgressiveHookshots <= 1;}),
                    AdvancementPairing(A_ProgressiveHookshot, []{return IsAdult && AddedProgressiveHookshots == 1 && !HoverBoots;}),
  });

  Exit GerudoTrainingGrounds_HammerRoom = Exit("Gerudo Training Grounds Hammer Room", "Gerudo Training Grounds", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&GerudoTrainingGrounds_HammerRoomClearChest,  []{return true;}),
                    ItemLocationPairing(&GerudoTrainingGrounds_HammerRoomSwitchChest, []{return CanUse("Hammer");})
                  }, {
                    //Exits
                    ExitPairing::Both(&GerudoTrainingGrounds_EyeStatueLower, []{return CanUse("Hammer") && Bow;}),
                    ExitPairing::Both(&GerudoTrainingGrounds_LavaRoom,       []{return true;}),
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_MegatonHammer,  []{return true;}),
                    AdvancementPairing(A_ProgressiveBow, []{return AddedProgressiveBows == 0;}),
  });

  Exit GerudoTrainingGrounds_EyeStatueLower = Exit("Gerudo Training Grounds Eye Statue Lower", "Gerudo Training Grounds", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&GerudoTrainingGrounds_EyeStatueChest, []{return CanUse("Bow");}),
                  }, {
                    //Exits
                    ExitPairing::Both(&GerudoTrainingGrounds_HammerRoom, []{return true;}),
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_ProgressiveBow, []{return AddedProgressiveBows == 0;}),
  });

  Exit GerudoTrainingGrounds_EyeStatueUpper = Exit("Gerudo Training Grounds Eye Statue Upper", "Gerudo Training Grounds", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&GerudoTrainingGrounds_NearScarecrowChest, []{return CanUse("Bow");})
                  }, {
                    //Exits
                    ExitPairing::Both(&GerudoTrainingGrounds_EyeStatueLower, []{return true;}),
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_ProgressiveBow, []{return AddedProgressiveBows == 0;}),
  });

  Exit GerudoTrainingGrounds_HeavyBlockRoom = Exit("Gerudo Training Grounds Heavy Block Room", "Gerudo Training Grounds", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&GerudoTrainingGrounds_BeforeHeavyBlockChest, []{return true;})
                  }, {
                    //Exits
                    ExitPairing::Both(&GerudoTrainingGrounds_EyeStatueUpper, []{return (LogicLensGtg || CanUse("Lens of Truth")) && (CanUse("Hookshot") || (LogicGtgFakeWall && CanUse("Hover Boots")));}),
                    ExitPairing::Both(&GerudoTrainingGrounds_LikeLikeRoom,   []{return (LogicLensGtg || CanUse("Lens of Truth")) && (CanUse("Hookshot") || (LogicGtgFakeWall && CanUse("Hover Boots"))) && CanUse("Silver Gauntlets");}),
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_ProgressiveMagic,       []{return AddedProgressiveMagics    == 0 && !LogicLensGtg;}),
                    AdvancementPairing(A_LensOfTruth,            []{return ProgressiveMagic          >= 1 && !LogicLensGtg;}),
                    AdvancementPairing(A_ProgressiveHookshot,    []{return AddedProgressiveHookshots == 0;}),
                    AdvancementPairing(A_ProgressiveStrength,    []{return AddedProgressiveStrengths == 0;}),
                    AdvancementPairing(A_ProgressiveStrength,    []{return AddedProgressiveStrengths == 1;}),
                    AdvancementPairing(A_HoverBoots,             []{return LogicGtgFakeWall;}),
  });

  Exit GerudoTrainingGrounds_LikeLikeRoom = Exit("Gerudo Training Grounds Like Like Room", "Gerudo Training Grounds", "", NO_DAY_NIGHT_CYCLE, {}, {
                    //Locations
                    ItemLocationPairing(&GerudoTrainingGrounds_HeavyBlockFirstChest,  []{return true;}),
                    ItemLocationPairing(&GerudoTrainingGrounds_HeavyBlockSecondChest, []{return true;}),
                    ItemLocationPairing(&GerudoTrainingGrounds_HeavyBlockThirdChest,  []{return true;}),
                    ItemLocationPairing(&GerudoTrainingGrounds_HeavyBlockFourthChest, []{return true;}),
                  }, {}, {
  });

  Exit GanonsCastle_Lobby = Exit("Ganon's Castle Lobby", "Ganon's Castle", "", NO_DAY_NIGHT_CYCLE, {}, {}, {
                    //Exits
                    ExitPairing::Both(&OGC_Grounds,              []{return true;}),
                    ExitPairing::Both(&GanonsCastle_ForestTrial, []{return true;}),
                    ExitPairing::Both(&GanonsCastle_FireTrial,   []{return true;}),
                    ExitPairing::Both(&GanonsCastle_WaterTrial,  []{return true;}),
                    ExitPairing::Both(&GanonsCastle_ShadowTrial, []{return true;}),
                    ExitPairing::Both(&GanonsCastle_SpiritTrial, []{return true;}),
                    ExitPairing::Both(&GanonsCastle_LightTrial,  []{return CanUse("Golden Gauntlets");}),
                    ExitPairing::Both(&GanonsCastle_Tower,       []{return SkippedTrials || (ForestTrialClear && FireTrialClear && WaterTrialClear && ShadowTrialClear && SpiritTrialClear && LightTrialClear);}),
                    ExitPairing::Both(&GanonsCastle_DekuScrubs,  []{return LogicLensCastle || CanUse("Lens of Truth");}),
                  }, {
                    //Advancement Needs
                    AdvancementPairing(A_ProgressiveStrength, []{return AddedProgressiveStrengths == 0;}),
                    AdvancementPairing(A_ProgressiveStrength, []{return AddedProgressiveStrengths == 1;}),
                    AdvancementPairing(A_ProgressiveStrength, []{return AddedProgressiveStrengths == 2;}),
                    AdvancementPairing(A_ProgressiveMagic,    []{return AddedProgressiveMagics    == 0 && !LogicLensCastle;}),
                    AdvancementPairing(A_LensOfTruth,         []{return ProgressiveMagic          >= 1 && !LogicLensCastle;}),
  });

  Exit GanonsCastle_DekuScrubs = Exit("Ganon's Castle Deku Scrubs", "Ganon's Castle", "", NO_DAY_NIGHT_CYCLE, {
                    //Events
                    EventPairing(&FreeFairies, []{return true;}),
                  }, {
                    //Locations
                    //Ganons Castle Deku Scrub Center Left
                    //Ganons Castle Deku Scrub Center Right
                    //Ganons Castle Deku Scrub Right
                    //Ganons Castle Deku Scrub Left
                  }, {}, {
                    //Advancement Needs
  });

  Exit GanonsCastle_ForestTrial = Exit("Ganon's Castle Forest Trial", "Ganon's Castle", "", NO_DAY_NIGHT_CYCLE, {
                    //Events
                    EventPairing(&ForestTrialClear, []{return CanUse("Light Arrows") && (FireArrows || DinsFire);}),
                  }, {
                    //Locations
                    ItemLocationPairing(&GanonsCastle_ForestTrialChest, []{return true;}),
                  }, {}, {
                    //Advancement Needs
                    AdvancementPairing(A_ProgressiveMagic, []{return AddedProgressiveMagics == 0;}),
                    AdvancementPairing(A_DinsFire,         []{return ProgressiveMagic       >= 1 && !FireArrows;}),
                    AdvancementPairing(A_ProgressiveBow,   []{return AddedProgressiveBows   == 0 && !DinsFire;}),
                    AdvancementPairing(A_FireArrows,       []{return ProgressiveBow         >= 1 && ProgressiveMagic >= 1 && !DinsFire;}),
                    AdvancementPairing(A_LightArrows,      []{return ProgressiveBow         >= 1 && ProgressiveMagic >= 1;}),
  });

  Exit GanonsCastle_FireTrial = Exit("Ganon's Castle Fire Trial", "Ganon's Castle", "", NO_DAY_NIGHT_CYCLE, {
                    //Events
                    EventPairing(&FireTrialClear, []{return CanUse("Goron Tunic") && CanUse("Golden Gauntlets") && CanUse("Light Arrows") && CanUse("Longshot");}),
                  }, {}, {}, {
                    //Advancement Needs
                    AdvancementPairing(A_ProgressiveBow,   []{return AddedProgressiveBows   == 0;}),
                    AdvancementPairing(A_ProgressiveMagic, []{return AddedProgressiveMagics == 0;}),
                    AdvancementPairing(A_LightArrows,      []{return ProgressiveBow         >= 1 && ProgressiveMagic >= 1;}),
                    AdvancementPairing(A_GoronTunic,       []{return IsAdult;}),
  });

  Exit GanonsCastle_WaterTrial = Exit("Ganon's Castle Water Trial", "Ganon's Castle", "", NO_DAY_NIGHT_CYCLE, {
                    //Events
                    EventPairing(&BlueFireAccess,  []{return true;}),
                    EventPairing(&FairyPot,        []{return FairyPot || BlueFire;}),
                    EventPairing(&WaterTrialClear, []{return BlueFire && Hammer && CanUse("Light Arrows");}),
                  }, {
                    //Locations
                    ItemLocationPairing(&GanonsCastle_WaterTrialLeftChest,  []{return true;}),
                    ItemLocationPairing(&GanonsCastle_WaterTrialRightChest, []{return true;}),
                  }, {}, {
                    //Advancement Needs
                    AdvancementPairing(A_EmptyBottle,      []{return true;}),
                    AdvancementPairing(A_MegatonHammer,    []{return IsAdult;}),
                    AdvancementPairing(A_ProgressiveBow,   []{return AddedProgressiveBows   == 0;}),
                    AdvancementPairing(A_ProgressiveMagic, []{return AddedProgressiveMagics == 0;}),
                    AdvancementPairing(A_LightArrows,      []{return ProgressiveBow         >= 1 && ProgressiveMagic >= 1;}),
  });

  Exit GanonsCastle_ShadowTrial = Exit("Ganon's Castle Shadow Trial", "Ganon's Castle", "", NO_DAY_NIGHT_CYCLE, {
                    //Events
                    EventPairing(&ShadowTrialClear, []{return CanUse("Light Arrows") && Hammer && ((FireArrows && (LogicLensCastle || CanUse("Lens of Truth"))) || (CanUse("Longshot") && (HoverBoots || (DinsFire && (LogicLensCastle || CanUse("Lens of Truth"))))));}),
                  }, {
                    //Locations
                    ItemLocationPairing(&GanonsCastle_ShadowTrialFrontChest,           []{return CanUse("Fire Arrows") || Hookshot || HoverBoots || CanPlay(SongOfTime);}),
                    ItemLocationPairing(&GanonsCastle_ShadowTrialGoldenGauntletsChest, []{return CanUse("Fire Arrows") || (CanUse("Longshot") && (HoverBoots || CanUse("Dins Fire")));}),
                  }, {}, {
                    //Advancement Needs
                    AdvancementPairing(A_ProgressiveBow,   []{return AddedProgressiveBows   == 0;}),
                    AdvancementPairing(A_ProgressiveMagic, []{return AddedProgressiveMagics == 0;}),
                    AdvancementPairing(A_LightArrows,      []{return ProgressiveBow         >= 1 && ProgressiveMagic >= 1;}),
  });

  Exit GanonsCastle_SpiritTrial = Exit("Ganon's Castle Spirit Trial", "Ganon's Castle", "", NO_DAY_NIGHT_CYCLE, {
                    //Events
                    EventPairing(&NutPot,           []{return NutPot || ((LogicSpiritTrialHookshot || Hookshot) && HasBombchus && Bow && MirrorShield);}),
                    EventPairing(&SpiritTrialClear, []{return CanUse("Light Arrows")  && MirrorShield && HasBombchus && (LogicSpiritTrialHookshot || Hookshot);}),
                  }, {
                    //Locations
                    ItemLocationPairing(&GanonsCastle_SpiritTrialCrystalSwitchChest, []{return LogicSpiritTrialHookshot || Hookshot;}),
                    ItemLocationPairing(&GanonsCastle_SpiritTrialInvisibleChest,     []{return (LogicSpiritTrialHookshot || Hookshot) && HasBombchus && (LogicLensCastle || CanUse("Lens of Truth"));}),
                  }, {}, {
                    //Advancement Needs
                    AdvancementPairing(A_ProgressiveBow,   []{return AddedProgressiveBows   == 0;}),
                    AdvancementPairing(A_ProgressiveMagic, []{return AddedProgressiveMagics == 0;}),
                    AdvancementPairing(A_LightArrows,      []{return ProgressiveBow         >= 1 && ProgressiveMagic >= 1;}),
  });

  Exit GanonsCastle_LightTrial = Exit("Ganon's Castle Light Trial", "Ganon's Castle", "", NO_DAY_NIGHT_CYCLE, {
                    //Events
                    EventPairing(&LightTrialClear, []{return CanUse("Light Arrows") && Hookshot && SmallKeys(GanonsCastleKeys, 2) && (LogicLensCastle || CanUse("Lens of Truth"));}),
                  }, {
                    //Locations
                    ItemLocationPairing(&GanonsCastle_LightTrialFirstLeftChest,        []{return true;}),
                    ItemLocationPairing(&GanonsCastle_LightTrialSecondLeftChest,       []{return true;}),
                    ItemLocationPairing(&GanonsCastle_LightTrialThirdLeftChest,        []{return true;}),
                    ItemLocationPairing(&GanonsCastle_LightTrialFirstRightChest,       []{return true;}),
                    ItemLocationPairing(&GanonsCastle_LightTrialSecondRightChest,      []{return true;}),
                    ItemLocationPairing(&GanonsCastle_LightTrialThirdRightChest,       []{return true;}),
                    ItemLocationPairing(&GanonsCastle_LightTrialInvisibleEnemiesChest, []{return LogicLensCastle || CanUse("Lens of Truth");}),
                    ItemLocationPairing(&GanonsCastle_LightTrialLullabyChest,          []{return CanPlay(ZeldasLullaby) && SmallKeys(GanonsCastleKeys, 1);}),
                  }, {}, {
                    //Advancement Needs
                    AdvancementPairing(A_LensOfTruth,         []{return ProgressiveMagic         >= 1 && !LogicLensCastle;}),
                    AdvancementPairing(A_ZeldasLullaby,       []{return ProgressiveOcarina       >= 1;}),
                    AdvancementPairing(A_ProgressiveBow,      []{return AddedProgressiveBows     == 0;}),
                    AdvancementPairing(A_ProgressiveMagic,    []{return AddedProgressiveMagics   == 0;}),
                    AdvancementPairing(A_LightArrows,         []{return ProgressiveBow           >= 1 && ProgressiveMagic >= 1;}),
                    AdvancementPairing(GanonsCastle_SmallKey, []{return true;}, 2),
  });

  Exit GanonsCastle_Tower = Exit("Ganon's Castle Tower", "Ganons Castle", "", NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  ItemLocationPairing(&GanonsCastle_BossKeyChest, []{return true;}),
                  //Ganondorf Hint
                  //Ganon
                }, {}, {
                  //Advancement Needs
                  AdvancementPairing(GanonsCastle_BossKey, []{return true;}),
  });

  /*
  //Events
}, {
  //Locations
}, {
  //Exits
}, {
  //Advancement Needs
*/

  std::vector<Exit *> ExitPool = {};
}
