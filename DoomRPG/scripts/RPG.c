#include "Defs.h"

#include "Arena.h"
#include "Augs.h"
#include "GUI.h"
#include "GUIMenu.h"
#include "HealthBars.h"
#include "HUD.h"
#include "ItemData.h"
#include "Map.h"
#include "Menu.h"
#include "Mission.h"
#include "Monsters.h"
#include "Outpost.h"
#include "Popoffs.h"
#include "RPG.h"
#include "Shield.h"
#include "Shop.h"
#include "Skills.h"
#include "Stats.h"
#include "Stims.h"
#include "Turret.h"
#include "Utils.h"

// Version
const str Version = "v0.10.1 Beta - Powered by GDCC!";

// Flags
bool Transported;
bool GlobalsInitialized;
int CompatMode;
bool WadSmoosh;
bool ColourfulHell;

// Arrays
str PlayerWeapon[MAX_PLAYERS];

// Structs
PlayerData _PlayerData[MAX_PLAYERS];

// Items
bool RPGMap ItemTIDsInitialized;
int RPGMap ItemTIDs[MAX_ITEMS];

// --------------------------------------------------
// Multiplayer HUD
//

Address(8) AddressSpace MapArraySpace    RPGMap;
Address(8) AddressSpace WorldArraySpace  RPGWorld;
Address(8) AddressSpace GlobalArraySpace RPGGlobal;

Address(2)  AddressSpace GlobalArraySpace EPArray;

Address(50) AddressSpace GlobalArraySpace ShieldArray;
Address(51) AddressSpace GlobalArraySpace ShieldCapacityArray;
Address(52) AddressSpace GlobalArraySpace ShieldHealthArray;

// Energy Points (EP)
int EPArray Address(0) EP[MAX_PLAYERS];

// Shield
int ShieldArray Address(0) Shield[MAX_PLAYERS];
int ShieldCapacityArray Address(0) ShieldCapacity[MAX_PLAYERS];
int ShieldHealthArray Address(0) ShieldHealth[MAX_PLAYERS];

str const StatColors[STAT_MAX] =
{
    "Red",         // Strength
    "Green",       // Defense
    "Brick",       // Vitality
    "LightBlue",   // Energy
    "Purple",      // Regeneration
    "Orange",      // Agility
    "Blue",        // Capacity
    "Gold"         // Luck
};

NamedScript Type_OPEN void GlobalInit()
{
    if (!GlobalsInitialized)
    {
        // Version Info
        Log("\CnDoom RPG %S (Compiled on %S at %S) loaded!", Version, __DATE__, __TIME__);

        // Compatibility checking
        CheckCompatibility();

        // Get the XP Curve from the CVAR
        XPCurve = GetCVar("drpg_xp_curve");

        // Initial build of ItemData
        BuildItemData();

        // Initialize XP, Stat and Rank Tables
        InitXPTable();
        Delay(1);
        InitStatXPTable();
        Delay(1);
        InitNegativeStatXPTable();
        Delay(1);
        InitRankTable();

        // Arena Wave
        ArenaMaxWave = 1;

        GlobalsInitialized = true;

        Delay(1); // Allow CVARs to update
    }

    // Create Translations
    CreateTranslations();

    // Initial build of SkillData
    BuildSkillData();

    // Initial build of TurretUpgradeData
    BuildTurretData();

    // Initialize the Item handling script
    ItemHandler();
}

// Init Script
NamedScript Type_ENTER void Init()
{
    // Wait until globals are initialized
    while (!GlobalsInitialized) Delay(1);

    // Delay to allow for item data to initialize
    Delay(3);

    // [KS] This needs to be done on its' own, so death exits don't rob you of your TID and break EVERYTHING EVER FFS
    if (ActivatorTID() == 0)
    {
        AssignTIDs();
        if (!InTitle) // Don't give equipment on the title screen
        {
            if (!Player.FirstRun)
                Player.Capacity = GetActivatorCVar("drpg_start_capacity");

            SortStartingItems();
            DefaultLoadout();
        }
    }

    if (!Player.FirstRun)
    {
        // [KS] Portable always-use items, because having too many mandatory use-keys is bad, yo.
        if (GetActivatorCVar("drpg_give_quickinventory"))
        {
            GiveInventory("DRPGPortableStimInjectorItem", 1); // Use Stim
            GiveInventory("DRPGPortableStimDisposalItem", 1); // Toss Stim
            GiveInventory("DRPGPortableShieldItem", 1); // Toggle Shield
            GiveInventory("DRPGPortableMedkitItem", 1); // Quick Heal
            GiveInventory("DRPGPortableFocusItem", 1); // Focus
            GiveInventory("DRPGPortableAugItem", 1); // Reactivate Augs
        }

        // Funds
        GiveInventory("DRPGCredits", GetActivatorCVar("drpg_start_credits"));
        Player.PrevCredits = CheckInventory("DRPGCredits");
        GiveInventory("DRPGModule", GetActivatorCVar("drpg_start_modules"));
        GiveInventory("DRPGTurretPart", GetActivatorCVar("drpg_start_turretparts"));

        // Level/Rank/PP
        if (GetCVar("drpg_start_level") > 0)
        {
            Player.Level = GetActivatorCVar("drpg_start_level");
            Player.XP = XPTable[Player.Level - 1];
        }
        if (GetCVar("drpg_start_rank") > 0)
        {
            Player.RankLevel = GetActivatorCVar("drpg_start_rank");
            Player.Rank = RankTable[Player.RankLevel - 1];
        }
        if (GetCVar("drpg_start_pp") > 0)
            Player.PP = GetCVar("drpg_start_pp");

        // Stats
        Player.Strength = GetActivatorCVar("drpg_start_strength");
        Player.Defense = GetActivatorCVar("drpg_start_defense");
        Player.Vitality = GetActivatorCVar("drpg_start_vitality");
        Player.PrevVitality = Player.Vitality;
        Player.Energy = GetActivatorCVar("drpg_start_energy");
        Player.PrevEnergy = Player.Energy;
        Player.Regeneration = GetActivatorCVar("drpg_start_regeneration");
        Player.Agility = GetActivatorCVar("drpg_start_agility");
        Player.Capacity = GetActivatorCVar("drpg_start_capacity");
        Player.Luck = GetActivatorCVar("drpg_start_luck");

        // Stat XP
        Player.StrengthXP = StatTable[Player.Strength - 1];
        Player.DefenseXP = StatTable[Player.Defense - 1];
        Player.VitalityXP = StatTable[Player.Vitality - 1];
        Player.EnergyXP = StatTable[Player.Energy - 1];
        Player.RegenerationXP = StatTable[Player.Regeneration - 1];
        Player.AgilityXP = StatTable[Player.Agility - 1];
        Player.CapacityXP = StatTable[Player.Capacity - 1];
        Player.LuckXP = StatTable[Player.Luck - 1];

        // Default Health/EP
        Player.EP = Player.Energy * 10;
        Player.ActualHealth = Player.Vitality * 10;
        Player.PrevHealth = Player.ActualHealth;
        SetActorProperty(0, APROP_Health, Player.ActualHealth);

        // Setup HUD stuff
        Player.StatusTypeHUD = SE_NONE;

        for (int i = 0; i < MAPEVENT_MAX; i++)
            Player.SeenEventTip[i] = false;

        // Setup the Skill Wheel from the persistence CVARs
        for (int i = 0; i < MAX_SKILLKEYS; i++)
        {
            Player.SkillCategory[i] = GetActivatorCVar(StrParam("drpg_skillwheel_category_%d", i + 1));
            Player.SkillIndex[i] = GetActivatorCVar(StrParam("drpg_skillwheel_index_%d", i + 1));
        }

        // Transport should always be known
        Player.SkillLevel[MAX_CATEGORIES - 1][SkillCategoryMax[MAX_CATEGORIES - 1] - 1].Level = 1;
        Player.SkillLevel[MAX_CATEGORIES - 1][SkillCategoryMax[MAX_CATEGORIES - 1] - 1].CurrentLevel = 1;

        // Set default selected skill to nothing
        Player.SkillSelected = -1;

        // Fill Augmentation Battery
        Player.Augs.Battery = Player.Capacity * 10;

        // Setup the New! shield parts arrays
        for (int i = 0; i < SHIELDPAGE_MAX; i++)
            for (int j = 0; j < MAX_PARTS; j++)
                Player.NewShieldParts[i][j] = true;
        for (int i = 0; i < MAX_ACCESSORIES; i++)
            Player.NewShieldAccessoryParts[i] = true;

        // Setup first payout
        Player.PayTimer = 35 * 60 * GetCVar("drpg_pay_interval");

        // Build this player's GUI menu
        CreateGUIMenu();

        // Done first run
        Player.FirstRun = true;
    }

    // Death exit handling
    if (Player.ActualHealth <= 0)
        Player.ActualHealth = Player.HealthMax;

    // Apply camera textures and vars
    SetCameraToTexture(Player.TID, StrParam("P%iVIEW", PlayerNumber() + 1), 110);
    SetCameraToTexture(Player.TID, StrParam("P%iSVIEW", PlayerNumber() + 1), 90);
    Player.PlayerView = PlayerNumber();

    // Remove Aura if the keep Aura CVAR is off
    if (!GetCVar("drpg_skill_keepauras"))
        RemoveAura();

    // Transport FX
    if (Transported)
    {
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            // Player is not in-game
            if (!PlayerInGame(i)) continue;

            TransportInFX(Players(i).TID);
        }

        SetPlayerProperty(0, 0, PROP_TOTALLYFROZEN); // In case we were in the menu
        Transported = false;
    }

    // Clear the Player's summons
    for (int i = 0; i < MAX_SUMMONS; i++)
        Player.SummonTID[i] = 0;
    Player.Summons = 0;

    // Initialize DropTID Array
    ArrayCreate(&Player.DropTID, "Drops", 64, sizeof(int));

    // Mission Initialization
    InitMission();

    // Small delay before initializing the scripts for mod compatibility
    Delay(4);

    // Reassignment is necessary for some mods
    if (CompatMode == COMPAT_DRLA)
        AssignTIDs();

    // Execute Game Loops
    Loop();
    PlayerDamage();
    MoneyChecker();
    ShieldTimer();
    WeaponSpeed();
    StatRandomizer();
    DamageNumbers();
    InfoPopoffs();
    HealthBars();
    AutosaveHandler();
    ShopItemAutoHandler();
}

// Loop Script
NamedScript void Loop()
{
    Start:

    // If we're on the title map, terminate
    if (InTitle)
    {
        SetActorProperty(0, APROP_Invulnerable, true);
        TeleportOther(Player.TID, 1003, false);
        return;
    }

    // If the player's dead, terminate
    if (GetActorProperty(0, APROP_Health) <= 0) return;

    // Update Functions
    CheckCombo();
    CheckStats();
    CheckStatBonus();
    CheckHardStatCaps();
    CheckRegen();
    CheckLuck();
    CheckHealth();
    CheckAugSlots();
    CheckAugs();
    CheckLevel();
    CheckRank();
    CheckSkills();
    CheckAuras();
    CheckBurnout();
    CheckStim();
    CheckPerks();
    CheckShields();
    CheckShieldAccessory();
    CheckStatBounds();
    CheckToxicity();
    CheckStimImmunity();
    CheckStatusEffects();
    CheckCapacity();
    CheckShopCard();
    CheckMission();
    CheckHUD();
    CheckKeys();

    // Handle the pulsating menu colors
    MenuCursorColor = CursorColors[(Timer() / 3) % 6];
    PayReadyColor = PayReadyColors[(Timer() / 3) % 4];

    // Calculate Shop Discount
    Player.ShopDiscount = (int)((Player.RankLevel * 2.1) + (CurrentLevel->UACBase ? (Player.ShopCard * 5) : 0));

    // Main Menu
    if (Player.InMenu)
        MenuLoop();

    // Shop Menu
    if (Player.InShop)
        ShopLoop();

    // Menu dimming
    if (Player.InMenu || Player.InShop || Player.OutpostMenu > 0)
    {
        if (GetCVar("drpg_menudim") && !Player.StatusType[SE_BLIND])
            FadeRange(0, 0, 0, 0.5, 0, 0, 0, 0.0, 0.25);
    }

    // Menu-specific Help
    if (Player.InMenu || Player.InShop || Player.OutpostMenu > 0 || Player.CrateOpen)
        MenuHelp();

    // Menu icon in multiplayer
    if (InMultiplayer && (Player.InMenu || Player.InShop || Player.OutpostMenu > 0))
    {
        fixed X = GetActorX(Player.TID);
        fixed Y = GetActorY(Player.TID);
        fixed Z = GetActorZ(Player.TID);
        fixed Height = GetActorPropertyFixed(Player.TID, APROP_Height);

        SpawnForced("DRPGMenuIcon", X, Y, Z + Height + 8.0, 0, 0);
    }

    // Regeneration
    DoRegen();

    // Always Quick Heal if CVAR is set
    if (GetActivatorCVar("drpg_auto_heal"))
        if (Player.ActualHealth < Player.HealthMax / GetActivatorCVar("drpg_auto_heal_percent"))
            UseMedkit();

    // Set mass stupid high when Invulnerable or transporting to prevent knockback
    if (CheckInventory("PowerInvulnerable") || Transported)
        Player.Mass *= 128;

    // Apply Stats
    SetActorPropertyFixed(Player.TID, APROP_DamageMultiplier, (1.0 + ((fixed)Player.TotalDamage / 100.0)) * (Player.DamageMult > 1 ? Player.DamageMult : 1));
    SetActorPropertyFixed(Player.TID, APROP_DamageFactor, Player.DamageFactor);
    SetActorProperty(Player.TID, APROP_Mass, Player.Mass);
    SetActorProperty(Player.TID, APROP_SpawnHealth, Player.HealthMax);
    SetActorPropertyFixed(Player.TID, APROP_Speed, Player.Speed);
    SetActorPropertyFixed(Player.TID, APROP_JumpZ, Player.JumpHeight);

    // Store your current weapon in the Player Weapon array
    PlayerWeapon[PlayerNumber()] = GetWeapon();

    // DRLA Debug Mode
    if (GetCVar("drpg_debug_drla") & DDM_NOLIMITS)
    {
        SetInventory("RLWeaponLimit", 0);
        SetInventory("RLArmorInInventory", 0);
        SetInventory("RLModLimit", 0);
        SetInventory("RLScavengerModLimit", 0);
        SetInventory("RLArmorModItemInInventory", 0);
        SetInventory("RLSkullLimit", 0);
        SetInventory("RLPhaseDeviceLimit", 0);
    }

    // Loop
    Delay(1);

    goto Start;
}

// Damage Handler Entry Point
NamedScript void PlayerDamage()
{
    bool CanSurvive;
    bool Critical;
    int BeforeDamage;
    int DamageTaken;
    int ShieldDamageAmount;
    int MonsterID;
    fixed LuckChance;
    fixed EnergyLevel;

    Start:

    // If the player's dead, terminate
    if (GetActorProperty(0, APROP_Health) <= 0) return;

    CanSurvive = Player.SurvivalBonus > 0 && RandomFixed(0.0, 100.0) <= Player.SurvivalBonus && !Player.LastLegs;

    if (Player.ActualHealth <= 0) // [KS] This is to make sure the player actually dies, even if they're shielded.
    {
        SetPlayerProperty(0, 0, PROP_BUDDHA);
        SetActorProperty(0, APROP_Health, 0);
        goto Start;
    }

    if (!Player.Shield.Active || !Player.Shield.Charge)
    {
        SetActorProperty(0, APROP_Health, Player.ActualHealth);

        if (Player.ActualHealth > 2)
            Player.LastLegs = false;

        if (CanSurvive || CheckInventory("DRPGLife"))
            SetPlayerProperty(0, 1, PROP_BUDDHA);
        else
            SetPlayerProperty(0, 0, PROP_BUDDHA);
    }
    else
    {
        SetPlayerProperty(0, 0, PROP_BUDDHA);

        SetActorProperty(0, APROP_Health, SHIELD_HEALTH);

        // Hilarious hack to allow healing detection
        SetActorProperty(0, APROP_SpawnHealth, Player.ActualHealth < Player.HealthMax ? SHIELD_HEALTH * 2 : SHIELD_HEALTH);
    }

    Player.DamageType = DT_NONE;
    BeforeDamage = GetActorProperty(Player.TID, APROP_Health);

    Delay(1);

    // Get attacker's ID
    Player.DamageTID = WhoShotMe();

    DamageTaken = BeforeDamage - GetActorProperty(Player.TID, APROP_Health);
    ShieldDamageAmount = 0;

    if (DamageTaken > 0)
    {
        Player.AutosaveTimerReset = true;

        // Find Monster ID
        MonsterID = FindMonster(Player.DamageTID);

        // Calculate monster crit/status chance
        LuckChance = (fixed)Monsters[MonsterID].Luck / 20.0;
        EnergyLevel = (fixed)Monsters[MonsterID].Energy / 12.5;

        // Calculate a critical hit
        if (Player.DamageTID > 0 && MonsterID > 0)
            if (RandomFixed(0.0, 100.0) <= EnergyLevel)
            {
                DamageTaken *= 2;
                Critical = true;
            }

        AugDamage(DamageTaken);
        ToxicityDamage();
        if (MonsterID > 0)
            StatusDamage(DamageTaken, LuckChance, Critical);
        else
            StatusDamage(DamageTaken, RandomFixed(0.0, 100.0), Critical);
        ResetRegen();
        DamageHUD(DamageTaken, Critical);
        Player.Payout.DamageTaken += DamageTaken;
    }

    // Reset attacker's ID and critical damage state
    Player.DamageTID = -1;
    Critical = false;

    if (DamageTaken > 0 && Player.Shield.Active)
        ShieldTimerReset();

    if (DamageTaken > 0 && Player.Shield.Active && Player.Shield.Charge > 0)
    {
        ShieldDamageAmount = DamageTaken; // For callback
        if (ShieldDamageAmount > Player.Shield.Charge)
            ShieldDamageAmount = Player.Shield.Charge;

        Player.Shield.Charge -= DamageTaken;
        Player.Shield.Full = false;

        ShieldDamage(ShieldDamageAmount);

        if (Player.Shield.Charge <= 0)
        {
            if (Player.Shield.Charge < 0)
            {
                DamageTaken = -Player.Shield.Charge;
                Player.ActualHealth -= DamageTaken;
                Player.Shield.Charge = 0;
            }
            else
                DamageTaken = 0;

            ShieldBroken();
        }
        else
            DamageTaken = 0;
    }

    if (DamageTaken > 0)
    {
        Player.ActualHealth -= DamageTaken;

        if (Player.ActualHealth <= 1) // Near-Death stuff
        {
            // Extra Life check
            if (CheckInventory("DRPGLife"))
            {
                Player.ActualHealth = Player.HealthMax;
                ActivatorSound("health/resurrect", 127);
                TakeInventory("DRPGLife", 1);
                Player.Payout.LivesUsed++;

                SetHudSize(320, 200, false);
                SetFont("BIGFONT");
                HudMessage("Used an Extra Life!");
                EndHudMessage(HUDMSG_FADEOUT, 0, "Gold", 160.0, 140.0, 0.5, 1.5);
                PrintSpriteFade("P1UPA0", 0, 172.0, 210.0, 0.5, 1.5);
            }

            // Survival Bonus
            if (CanSurvive && Player.ActualHealth <= 1)
            {
                Player.ActualHealth = 2;
                Player.LastLegs = true;

                if (Player.Shield.Accessory && Player.Shield.Accessory->PassiveEffect == SHIELD_PASS_SURVIVECHARGE)
                    Player.Shield.Charge = Player.Shield.Capacity;

                ActivatorSound("health/survive", 127);
                SetHudSize(320, 200, false);
                SetFont("BIGFONT");
                HudMessage("Agility Save!");
                EndHudMessage(HUDMSG_FADEOUT, 0, "Orange", 160.0, 140.0, 0.5, 0.5);
                PrintSpriteFade("AGISAVE", 0, 160.0, 140.0, 0.5, 0.5);
            }
        }
    }
    else if (DamageTaken < 0)
    {
        DamageTaken = -DamageTaken;

        // Negative "damage" == We got health from an outside source (eg. RLA assembly unit)
        // We'll just need to assume it's a regular capped heal if we got it this way.
        // Anything that needs otherwise should be redefined anyway.

        if (Player.ActualHealth + DamageTaken > Player.HealthMax)
            DamageTaken = Player.HealthMax - Player.ActualHealth;

        Player.ActualHealth += DamageTaken;
    }

    goto Start;
}

NamedScript void MoneyChecker()
{
    int BeforeCredits;
    int AfterCredits;

    Start:

    BeforeCredits = CheckInventory("DRPGCredits");

    Delay(1);

    AfterCredits = CheckInventory("DRPGCredits");

    // Payout
    if (AfterCredits > BeforeCredits)
        Player.Payout.CreditsFound += AfterCredits - BeforeCredits;
    if (AfterCredits < BeforeCredits)
        Player.Payout.CreditsSpent -= AfterCredits - BeforeCredits;

    goto Start;
}

NamedScript DECORATE int AddHealth(int HealthPercent, int MaxPercent)
{
    int RealMax = Player.HealthMax * MaxPercent / 100;
    int HealthAmount = Player.HealthMax * HealthPercent / 100;

    if (Player.ActualHealth >= RealMax)
        return 0;

    if (Player.ActualHealth + HealthAmount > RealMax)
        HealthAmount = RealMax - Player.ActualHealth;

    // Add Vitality XP for using healing items
    if (GetCVar("drpg_levelup_natural") && !Player.Stim.Active)
    {
        fixed Scale = GetCVarFixed("drpg_vitality_scalexp");
        if (GetCVar("drpg_allow_spec"))
        {
            if (GetActivatorCVar("drpg_character_spec") == 3)
                Scale *= 2;
        }

        int Factor = CalcPercent(HealthAmount, Player.HealthMax);
        Player.VitalityXP += (int)(Factor * Scale * 10);
    }

    Player.ActualHealth += HealthAmount;
    return 1;
}

NamedScript DECORATE int AddHealthDirect(int HealthAmount, int MaxPercent)
{
    int RealMax = Player.HealthMax * MaxPercent / 100;

    if (Player.ActualHealth >= RealMax)
        return 0;

    if (Player.ActualHealth + HealthAmount > RealMax)
        HealthAmount = RealMax - Player.ActualHealth;

    // Add Vitality XP for using healing items
    if (GetCVar("drpg_levelup_natural") && !Player.Stim.Active)
    {
        fixed Scale = GetCVarFixed("drpg_vitality_scalexp");
        if (GetCVar("drpg_allow_spec"))
        {
            if (GetActivatorCVar("drpg_character_spec") == 3)
                Scale *= 2;
        }

        int Factor = CalcPercent(HealthAmount, Player.HealthMax);
        Player.VitalityXP += (int)(Factor * Scale * 10);
    }

    Player.ActualHealth += HealthAmount;
    return 1;
}

typedef struct
{
    str Title;
    str Tip;
} TipInfo;

NamedScript void GiveTip()
{
    if ((CurrentLevel->Event == MAPEVENT_SKILL_HELL && GameSkill() != 5) || (CurrentLevel->Event == MAPEVENT_SKILL_ARMAGEDDON && GameSkill() != 6))
        return;

    TipInfo Tips[MAX_TIPS] =
    {
        // Level/Rank
        { "\CjLevel",                           "Your level determines your basic strength and progress through the game. Your level is increased by killing monsters, completing missions and achieving combos.", },
        { "\CkRank",                            "Your rank determines your standing within the UAC. Rank will determine how much money you are paid by the UAC as well as the discount you will receive when purchasing items from the shop.", },
        { "\CtCombo",                           "The combo system allows you to kill enemies in rapid succession in order to gain bonus XP and Rank. When an enemy is killed, you will gain +1 to your combo counter and your combo timer will reset. When your timer reaches its cooldown point, indicated by the split between the red and green in the bar, it will add the indicated XP and Rank to your totals and calculate a bonus (the green number). If you keep incrementing your combo during the cooldown period, you can keep stacking XP and Rank into the bonus. When the combo timer ends, your combo will be completely reset and your bonus will be added to your totals.", },
        // TODO: PP Hint

        // Currencies
        { "\CfCredits",                         "Credits are the universal currency used by the UAC and are used for purchasing goods in the shop.", },
        { "\CdModules",                         "Modules are used to upgrade your stats and skills. They are typically given by level ups, missions and completing level events as well as occasionally being dropped by monsters and found in the world.", },
        { "\CcTurret Parts",                    "Turret parts are used to build and upgrade your turret. They can be found by disassembling broken-down turrets you find in the world.", },

        // Stats
        { "\CgStrength",                        "Your Strength stat determines the extra damage you deal with all types of attacks.", },
        { "\CdDefense",                         "Your Defense stat determines your resistance to all forms of damage.", },
        { "\CaVitality",                        "Your Vitality stat determines your max health and health regeneration rate.", },
        { "\CnEnergy",                          "Your Energy stat determines your max EP and EP regeneration rate.", },
        { "\CtRegeneration",                    "Your Regeneration stat determines your regeneration rate and the time that Regeneration Spheres last.", },
        { "\CiAgility",                         "Your Agility stat governs your movement, jumping and fire speeds as well as your survival bonus.", },
        { "\ChCapacity",                        "Your Capacity stat determines your ammo carrying capacity, how many compounds of each type you can carry for Stims as well as how many inventory items you can hold", },
        { "\CfLuck",                            "Your Luck stat determines which items can drop as well as the chance which they will drop. Luck also affects several other hidden factors as well.", },

        // Stats Extended
        { "\CiSurvival Bonus",                  "Survival Bonus is a special attribute which works as a last stand mechanic. When you take a hit that would kill you and your survival bonus is triggered, you will not be killed by the attack.", },
        { "\CdPerks",                           "Perks are special enhancements that you receive when you upgrade a stat to 100 points or more.", },
        { "\CgStat Cap",                        "You can only increase a stat past 100 up to the maximum of your current level + 100.", },

        // Skills
        { "\CnEP",                              "Energy Points are needed in order to use skills. They can also be used to charge your Shield and to deposit and withdraw items from your personal locker while not in the Outpost.", },
        { "\CnOverdrive",                       StrParam("You can Overdrive a skill by holding the \Cd%jS\C- button. Doing this will always use the skill, regardless of it's EP cost. However, this will bring your current EP into the negatives. When negative, you will suffer stat penalties and not be able to use any skills until your EP is restored past 0.", "+speed"), },
        { "\CnAuras",                           "Auras are special passive abilities you can activate to give you temporary boosts to your stats. Auras will also affect teammates within it's radius, determined by your Energy stat", },

        // Augmentations
        { "\CkAugmentations",                   "Augmentations give you permanent bonuses to your stats while they are active.", },
        { "\CkAugmentation Battery",            "Augmentations can only work while your augmentation battery has a charge. Your battery can be recharged by using augmentation batteries from your inventory or by receiving certain types of damage. Battery drain is determined by how many augmentations you have active and how high their levels are. The more augmentation active and the higher their levels, the quicker the battery will drain.", },
        { "\CkAugmentation Slots",              "You can only activate as many augmentations as you have available slots for. To increase the number of slots you have, you must find an augmentation slot upgrade. Augmentation slot upgrades can be bought from the shop, found as rare drops from enemies or found in the world.", },

        // Shield
        { "\CvShields",                         "Shields will protect you from taking direct damage by absorbing the damage into their own charge pool. When a shield takes damage, it's charge is directly affected and will be reduced by the damage's amount. When a shield's charge reaches 0, you will begin taking normal damage again.", },
        { "\CvShield Components",               "Shields are built from 4 basic components which you can collect and customize during the game. The body is named after the manufacturer and is generally responsible for modifying the capacity of the Shield, but also evenly modifies the other stats as well. The battery is generally responsible for modifying the charge rate of the shield. The capacitor is generally responsible for modifying the charge rate and delay rate of the shield. Accessories give the shield unique effects and abilities which can be triggered in various ways.", },
        { "\CvShield Stats",                    "Each shield has 4 stats which govern it's behavior and ability. Capacity determines the total amount of charge the shield can hold. Charge Rate determines the amount the charge pool will regenerate when a charge cycle is completed. Delay Rate determines the wait time that occurs when a charge cycle is interrupted before charging will resume again. Charge interval determines the length between charge cycles.", },
        { "\CvShield Charge Cycle",             "A charging cycle occurs based on the Charge Interval (default of 1 second) and increases the Shield's charge pool, determined by the Charge Rate. When the shield is struck, the charging cycle is interrupted and a waiting period, determined by the Delay Rate, must complete before charging cycles will resume again.", },

        // Stims
        { "\CcStims",                           "Stims are portable injected temporary stat increases. There are three forms of Stim vials that can be encountered: Stat vials, Booster vials and Powerup vials.", },
        { "\CcStim Injectors",                  "Injectors are what are used to actually store the compounds for usage. Injectors can either be bought in the shop or found in the world. There are 4 different injector sizes, which hold varying amounts of compounds.", },
        { "\CcStim Vials",                      "Vials are the items you pickup which contain the different kinds of compounds. Vials can be bought in the shop or found by killing enemies which have auras or elsewhere in the world.", },
        { "\CcStim Stat Vials",                 "Stat vials increase their various corresponding stats when used.", },
        { "\CcStim Booster Vials",              "Booster vials are special vials which have unique stat-related effects. Purifier vials increase the duration that stat vials will last. Potency vials increase all stats at once.", },
        { "\CcStim Powerup Vials",              "Powerup vials give various special powers for a limited duration. Unlike the Stat stims, they are unaffected by Purifier vials, and their time is affected solely by the amount of vials put into the Stim.", },
        { "\CdToxicity",                        "Toxicity is a measure of the radiation and toxins in your body. Toxicity is increased by taking radiation or toxin related damage, such as standing on damage floors. The higher your toxicity levels get, the more penalties you will suffer. If your toxicity levels reach 100%, you will die. Toxicity will naturally dissipate over time, the speed of which is determined by your Regeneration stat.", },

        // Turrets
        { "\CdTurrets",                         "Turrets are portable sentry drones which can be built and equipped with multiple offensive and defensive upgrades and abilities to aid the user. Turrets must initially be built from turret parts, at which point the turret may be used and upgraded further as more parts become available.", },
        { "\CdDestroyed Turrets",               "In the world, you will find destroyed turrets. These turrets can be scavenged for spare turret parts. Some turrets will be in better condition than others, allowing for more parts to be scavenged.", },
        { "\CdTurret Maintenance",              StrParam("Your turret will need routine maintenance performed on it in order to keep it in working condition. There are three components to maintenance: charging, repairing and refitting. Charging will charge the turret's internal battery, allowing it to stay active on the field. Repairing will patch up the turret, restoring it's health. Refitting occurs when upgrades are performed on the turret, and must be allowed to finish before the turret can be used again. In order for maintenance to begin and to continue, a steady supply of Credits will be deducted from your account. To send the turret in for maintenance, in either the turret menu or with the turret command wheel open, press \Cd%jS\C-.", "+speed"), },
        { "\CdTurret Deployment",               StrParam("To quickly deploy or deactivate your turret, use \Cd%jS\C- + \Cd%jS\C-.", "+speed", "+user2"), },

        // Monsters
        { "\CgMonster Stats",                   "Like you, monsters also have a level and stats which will affect their abilities in various ways.", },
        { "\CgMonster Auras",                   "Like you, monsters can also have one or multiple auras which will give them special abilities. Each aura a monster has will also double its respective stat.", },
        { "\CgMonster Shadow Auras",            "Monsters which have all auras active at once will become a shadow enemy with extremely high stats and abilities for their level. These enemies will receive names and should be priority targets to take down, as their combined abilities can quickly overwhelm a player if they are not prepared.", },
        { "\CgMonster Regeneration Cycle",      "Like you, monsters will regenerate a portion of their health every so often. The amount of health they will regenerate is determined by their regeneration stat.", },
        { "\CgMonster Threat Level",            "Each monster has a threat level. Their threat level is determined by their level, stats and active auras and ranges from 0 to 10. Threat level can be used as a general way to gauge a monster's abilities and which ones should be prioritized when attacking. The higher a monster's threat level, the higher XP and Rank you will receive for defeating them. Threat level is represented on a monster's health bar by the number of emblems present.", },
        { "\CgMegabosses",                      "Under certain circumstances, you will encounter monsters known as megabosses. These bosses are extremely difficult, having special and unique behaviors which can quickly and effectively destroy you if not prepared. When killed, megabosses will provide a tremendous amount of loot as well as XP and Rank.", },

        // Status Effects
        { "\CaStatus Effects",                  "Status effects are debuffs which will affect you in different ways. You can get them from taking different types of damage or by exposing yourself to specific types of hazards. When inflicted with a status effect, it will last for a certain amount of time. Each status effect has 5 levels of intensity which are increasingly more detrimental to you. If you are hit with a status effect you already have, it will add to your current timer and potentially increase it's intensity.", },
        { "\CcBlindness",                       "Blindness is a status effect which will darken your view and make it harder to see.", },
        { "\CfConfusion",                       "Confusion is a status effect which will make your vision blurry and make it harder for you to aim and move.", },
        { "\CqPoison",                          "Poison is a status effect which will deal direct health damage to you every second. It will not actually kill you if your health reaches a critical level.", },
        { "\CdCorrosion",                       "Corrosion is a status effect which will deal damage to your armor every second.", },
        { "\CiFatigue",                         "Fatigue is a status effect which will decrease your movement speed and jumping height.", },
        { "\CtVirus",                           "Virus is a status effect which will stop you from regenerating.", },
        { "\CnSilence",                         "Silence is a status effect which prevents you from using skills.", },
        { "\CgCurse",                           "Curse is a status effect which increases the amount of damage you take.", },
        { "\CvEMP",                             "EMP is a status effect which will completely disable your shields and augmentations and not allow you to re-enable them. It will also drain your augmentation battery.", },
        { "\CdRadiation",                       "Radiation is a status effect which slowly increases your toxicity levels. It will not increase your toxicity past 85%.", },

        // Outpost/Arena
        { "\CqOutpost",                         "The UAC Outpost is the main hub of operations where you can perform various activities, access various facilities and stock up between levels. You can transport to the Outpost at any time using the Transport skill.", },
        { "\CgArena",                           "In the Arena, you can fight waves of enemies for XP, Rank and loot.", },
        { "\CdLevel Transporter",               "In the Outpost, you can use the level transporter to view stats on all the levels you have visited and to transport back to them at will.", },
        { "\CdSkill Computer",                  "In the Outpost, you can use the skill computer to change the current skill level without the need to restart the game.", },
        { "\CdMinigames",                       "In the Outpost, there are minigames which you can play in order to win items using your gold and platinum chips.", },
        { "\CgPractice Target",                 "In the Outpost, you can shoot the target in order to gauge the amount of damage your weapons are doing.", },

        // Missions
        { "\CdMissions",                        "You can accept different missions in the Outpost in order to gain XP, Rank, Credits, Modules and items. There are several different types of missions at varying difficulty levels you can undertake. The available missions will refresh each time you visit the Outpost.", },
        { "\CdCollection Missions",             "You must collect a specific number of a specified item to complete the mission. These items are found scattered around the world.", },
        { "\CaKill Missions",                   "You must kill a given amount of enemies of the same type to complete the mission.", },
        { "\CaKill Auras Missions",             "You must Kill a given amount of enemies which have at least one aura to complete the mission.", },
        { "\CaKill Reinforcements Missions",    "While in the field, monsters will spawn in around you. You must kill a given amount of them to complete the mission.", },
        { "\CgAssassination Missions",          "You must eliminate the specified enemy, which will be located somewhere in the level and will always have a shadow aura to complete the mission.", },
        { "\CkFind Secrets Missions",           "You must find the given amount of unique secrets to complete the mission.", },
        { "\CnFind Items Missions",             "You must find the given amount of items to complete the mission.", },
        { "\CtAchieve Combo Missions",          "You must get your combo to the given amount to complete the mission.", },

        // Shop/Locker
        { "\CfShop",                            "The shop is where you can go to purchase and sell the various equipment and items you can find during the game. The shop also contains a locker system, where you may withdraw and deposit items for later usage.", },
        { "\ChLocker",                          StrParam("You can access the locker system via the shop and deposit/withdraw your items there. Withdrawing and depositing items has a 1%% cost to your EP per use outside the Outpost. You can switch between the shop and locker using the \Cd%jS\C- key.", "+jump"), },
        { "\CfShop Cards",                      "While playing, you may find UAC cards which provide you with a discount as well as other benefits. Finding a new card will replace your old card with the upgraded version. Discount benefits from a card will only apply when shopping at the Outpost.", },

        // Level Events
        { "\CdLevel Events",                    "When entering a level, sometimes unique level-wide events will happen. Each event has it's own unique characteristics and conditions for ending the event.", },
        { "\CdRolling Level Events",            "Level events will not only occur when entering a level, but previous levels which you have visited will also sometimes contain events and require revisiting in order to participate in the event.", },
    };

    TipInfo const EventTips[MAPEVENT_MAX] =
    {
        { "", "", }, // Never used

        { "\CgMegaboss Event",                  "All of the hostiles have vacated the area, and a very powerful creature is lurking about. Return to the Outpost if needed, and make sure you have the power and munitions to take down the death machine.", },
        { "\CqEnvironmental Hazard Event",      "The area's covered in deadly radiation, but there is also a Radiation Neutralizer to fix the problem. Gather fuel and refill the Neutralizer whenever it runs low, until the radiation is brought down to safe levels.", },
        { "\CiThermonuclear Bomb Event",        "Find as many keys as you can to disable the countdown. Worse comes to worst, you can hack through the key slots by using the bomb without keys, but it's going to take time to do so for each key missing, as indicated by the HUD. And make sure the bomb doesn't get shot!", },
        { "\CuLow Power Event",                 "The UAC had a major battle in this area, and the discards have been left behind when the area was damaged. Pluck some extra items out of the wreckage before they come back to clean up. You can even get some extra credits and modules if you restore the backup power before you leave.", },
        { "\CdAll Auras Event",                 "All of the monsters have rather deadly magic auras. Understand their weaknesses, and attempt to stay away from some of them, as they are dangerous.", },
        { "\CnOne-Monster Event",               "There is one species of demons currently occupying the area. Clearing the area of the hostiles will net you a bonus.", },
        { "\CgHell Unleashed Event",            "Open the box, and partake in the rare contents within, but be warned: You will anger the demonic army, and they will pursue you endlessly until you are killed or leave the area. The longer you stay, the more likely they are to drop items, but the higher level they will become.", },
        { "\CdHarmonized Destruction Event",    "All of the enemies in the area have one aura type. Get accustomed to the aura's strength and weakness and use it to your advantage. Don't get caught off-guard, as some monsters can and will have other auras as well.", },
        { "\CdCracks in the Veil Event",        "The area is spatially unstable. Avoid stepping into the portals, as they will teleport you to other points in the area at random. Enemies will be teleported in as well.", },
        { "\Cg12 Hours 'til Doomsday Event",    "Hell has claimed the area! Run while you still can.", },
        { "\CqVicious Downpour",                "It's raining acidic substances. Best to wear a radiation suit if you plan to head outdoors.", },
        { "\CtThe Dark Zone",                   "This is no ordinary mist. The evil fog will slowly sap away the light from its' surroundings. Once that happens, enemies will begin to resurrect and become infused with Shadow Auras. Sticking around here for long is not recommended without preparation.", },

        { "\CaC       O     N   S    U    M    E         \CgY   O     U", "Tip: Run.", },
        { "\CmWhispers of Darkness",            "\Cg\"He who fights with monsters should look to it that he himself does not become a monster. \CaAnd when you gaze long into an abyss, \Cuthe abyss also gazes into you.\"", },


        { "\CgR\CiA\CkI\CdN\ChB\CtO\CaW\CjS",   "SHE BROKE THE REALITY GENERATOR AGAIN! WE'RE DOOMED!", },

        { "\CaHell Event",                      "The difficulty level has increased to \CaHell\C- for this area.", },
        { "\CmArmageddon Event",                "The difficulty level has increased to \CmArmageddon\C- for this area.", },

        { "\CgSinstorm",                        "You're getting close to the source of the invasion. The demonic legion is pulling all of its' last stops. Expect frequent battles with shadow enemies, and reinforcements to pour in relentlessly. Be careful when traversing outdoors, as the fire rain will harm you as well.", },
    };

    int TipID = Random(0, MAX_TIPS - 1);
    fixed HoldTime = StrLen(Tips[TipID].Tip) * 0.1;

    str TipTitle = Tips[TipID].Title;
    str TipText = Tips[TipID].Tip;

    if (CurrentLevel->Event != MAPEVENT_NONE && !Player.SeenEventTip[CurrentLevel->Event])
    {
        TipTitle = EventTips[CurrentLevel->Event].Title;
        TipText = EventTips[CurrentLevel->Event].Tip;
        HoldTime = StrLen(Tips[TipID].Tip) * 0.1;

        Player.SeenEventTip[CurrentLevel->Event] = true;
    }

    SetHudSize(0, 0, false);
    // Shove it in the console as one line
    HudMessage("%S\n\n\C-%S", TipTitle, TipText);
    EndHudMessage(HUDMSG_PLAIN | HUDMSG_LOG, 9998, "White", 0, 0, 1);
    HudMessage("");
    EndHudMessage(HUDMSG_PLAIN, 9998, "White", 0, 0, 1);
    SetFont("BIGFONT");
    HudMessage("%S", TipTitle);
    EndHudMessage(HUDMSG_FADEOUT, MAKE_ID('1','T','I','P'), "White", 0.5, 0.7, HoldTime, 3.0);
    SetFont("SMALLFONT");
    HudMessage("%S", TipText);
    EndHudMessage(HUDMSG_FADEOUT, MAKE_ID('2','T','I','P'), "White", 1.5, -0.725, HoldTime, 3.0);
}

// Handles Weapon Firing Speed
NamedScript void WeaponSpeed()
{
    int Time;

    Start:

    if (Player.Agility <= 100)
        Time = Abs(RoundInt(Player.WeaponSpeed * 0.35 - 35.0));
    else
        Time = 0;

    if (GetActivatorCVar("drpg_stat_weaponspeed") || Player.Stim.PowerupTimer[STIM_RAGE] > 0)
        SetInventory("DRPGSpeed", 1);

    Delay((Player.Stim.PowerupTimer[STIM_RAGE] > 0 ? 1 : Time + 1));
    goto Start;
}

// Stat Randomizer Script
NamedScript void StatRandomizer()
{
    Start:

    if (GetActivatorCVar("drpg_auto_spend") && !Player.Stim.Active)
    {
        while (CheckInventory("DRPGModule") > 0 && !StatsCapped())
        {
            // Select Preferred Stat
            if (Random(1, 2) == 1 && GetActivatorCVar("drpg_auto_spend_pref") >= 0)
                IncreaseStat(GetActivatorCVar("drpg_auto_spend_pref"));
            else // Select Random Stat
                IncreaseStat(Random(0, STAT_MAX - 1));

            Delay(1);
        }
    }

    Delay(1);
    goto Start;
}

// Handles autosaving
NamedScript void AutosaveHandler()
{
    // Terminate if the autosave CVar is disabled
    if (GetCVar("drpg_autosave") == 0 || (PlayerCount() > 1 && !Arbitrator)) return;

    int SaveTimer = GetCVar("drpg_autosave") * (35 * 60);
    bool Safe;
    int DamageTimer;

    while (true)
    {
        if (Player.AutosaveTimerReset)
            DamageTimer = 0;
        else
            ++DamageTimer;
        Player.AutosaveTimerReset = false;

        if (Player.InMenu || Player.InShop || Player.OutpostMenu > 0)
            DamageTimer = ASAVE_SAFETIME;

        if (!SaveTimer)
        {
            Safe = true;

            if (DamageTimer < ASAVE_SAFETIME ||
                GetActorProperty(Player.TID, APROP_Health) <= GetActorProperty(Player.TID, APROP_SpawnHealth) / 10)
                Safe = false;

            if (Safe)
            {
                Autosave();
                SaveTimer = GetCVar("drpg_autosave") * (35 * 60);
            }
            else
                SaveTimer = ASAVE_RETRYTIME;
        }
        else
            --SaveTimer;

        Delay(1);
    }
}

NamedScript Type_OPEN void ShopSpecialHandler()
{
    bool ValidItem;
    int Tries, MinValue, MaxValue, Category, Index;
    Start:

    // Reset the item
    if (ShopSpecialTimer <= 0)
    {
        ValidItem = false;
        Tries = 0;
        MinValue = 0;
        MaxValue = 0;
        Category = 0;
        Index = 0;

        // Calculate min and max based on settings
        switch (GetCVar("drpg_shopspecial_type"))
        {
        case SHOPSPECIAL_MINMAX:
            MinValue = GetCVar("drpg_shopspecial_min");
            MaxValue = GetCVar("drpg_shopspecial_max");
            break;
        case SHOPSPECIAL_LEVEL:
            MinValue = AveragePlayerLevel() * MAX_LEVEL;
            MaxValue = AveragePlayerLevel() * (100 * MAX_LEVEL);
            break;
        case SHOPSPECIAL_RANK:
            MinValue = AveragePlayerRank() * (25 * MAX_RANK);
            MaxValue = AveragePlayerRank() * (2500 * MAX_RANK);
            break;
        case SHOPSPECIAL_CREDITS:
            MinValue = AveragePlayerCredits() / 10;
            MaxValue = AveragePlayerCredits() * 10;
            break;
        case SHOPSPECIAL_LUCK:
            MinValue = AveragePlayerLuck() * 500;
            MaxValue = AveragePlayerLuck() * 5000;
            break;
        }
        if (GetCVar("drpg_debug"))
            Log("\CdDEBUG: Shop Special \C-Min/Max Calculated: \Ca%d \C-/ \Cd%d", MinValue, MaxValue);

        // Blank out the item until a new one is found
        ShopSpecialItem = GetBlankItem();

        while (!ValidItem)
        {
            ValidItem = true;

            Delay(1);

            // Pick an item
            Category = (GetCVar("drpg_shopspecial_category") >= 0 ? GetCVar("drpg_shopspecial_category") : Random(0, (ItemCategories - 1)));
            Index = Random(0, ItemMax[Category] - 1);

            // We're not having any luck meeting the criteria, or the min/max value are impossible values to meet (0 / 0)
            // Break and give up on generating a shop special this time
            if (Tries > 100 || (MinValue == 0 && MaxValue == 0))
            {
                ValidItem = false;
                break;
            }

            // Retry if you land on an item with no cost
            if (ItemData[Category][Index].Price == 0)
            {
                ValidItem = false;
                Tries++;
                continue;
            }

            // Skip Loot category entirely
            if (Category == 7)
            {
                ValidItem = false;
                Tries++;
                continue;
            }

            // Make sure the item is within the valid price range
            if (ItemData[Category][Index].Price < MinValue || ItemData[Category][Index].Price > MaxValue)
            {
                ValidItem = false;
                Tries++;
                continue;
            }

            // Item is valid, continue
            if (ValidItem)
                break;
        }

        if (ValidItem)
        {
            // Assign the item to the Shop Special item
            ShopSpecialItem = &ItemData[Category][Index];

            // Spawn the item if you're in the Outpost
            SpawnShopSpecialItem();
        }

        // Reset the timer and bought status
        ShopSpecialTimer = 35 * 60 * GetCVar("drpg_shopspecial_time");
        ShopSpecialBought = false;

        if (GetCVar("drpg_debug"))
        {
            if (ShopSpecialItem == GetBlankItem())
                Log("\CdDEBUG: Shop Special expired! \CaNo new item generated");
            else
                Log("\CdDEBUG: Shop Special expired! Now \Cj%S", ShopSpecialItem->Name);
        }
    }

    // Remove the item if it was bought
    if (CurrentLevel->UACBase && ShopSpecialBought)
        Thing_Remove(ShopSpecialTID + 1);

    // Decrease the timer
    ShopSpecialTimer--;

    Delay(1);
    goto Start;
}

// Handles the Items dynamic array and associated behavior
NamedScript void ItemHandler()
{
    fixed Dist, Height, Divisor, Angle, Pitch, Amount, X, Y, Z;
    int ItemTID;
    bool NoClip;

    // Create the Items Array
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        ItemTIDs[i] = -1;

        if ((i % 15000) == 0)
            Delay(1);
    }

    // Items array initialization complete
    ItemTIDsInitialized = true;

    while (ItemTIDsInitialized && !CurrentLevel->UACBase)
    {
        for (int i = 0; i < MAX_ITEMS; i++)
        {
            // We've hit the end of the array
            if (ItemTIDs[i] == -1) break;

            // Invalid item, continue
            if (ItemTIDs[i] == 0) continue;

            // This item was picked up or otherwise removed
            if (ClassifyActor(ItemTIDs[i]) == ACTOR_NONE || ClassifyActor(ItemTIDs[i]) == ACTOR_WORLD) continue;
            
            NoClip = false;
            // Iterate players and check distances
            for (int j = 0; j < MAX_PLAYERS; j++)
            {
                // Skip this player if they aren't in the game
                if (!PlayerInGame(j)) continue;

                // Skip this player if they aren't Magnetic
                if (Players(j).Stim.PowerupTimer[STIM_MAGNETIC] <= 0) continue;

                // Skip this player if they are dead
                if (GetActorProperty(Players(j).TID, APROP_Health) <= 0) continue;

                Dist = Distance(ItemTIDs[i], Players(j).TID);
                Height = GetActorPropertyFixed(Players(j).TID, APROP_Height);
                Divisor = (Dist - 16.0) + Dist;
                Angle = VectorAngle (GetActorX(Players(j).TID) - GetActorX(ItemTIDs[i]), GetActorY(Players(j).TID) - GetActorY(ItemTIDs[i]));
                Pitch = VectorAngle (Dist, GetActorZ(Players(j).TID) - GetActorZ(ItemTIDs[i]));

                // Calculate the amount based on close versus far distance
                if (Dist < 16.0)
                    Amount = 1.0;
                else
                    Amount = 16.0 / Divisor;

                // Calculate the lerped positions
                X = (Amount * 16.0) * Cos(Angle) * Cos(Pitch);
                Y = (Amount * 16.0) * Sin(Angle) * Cos(Pitch);
                Z = (Amount * 16.0) * Sin(Pitch);

                SetActorVelocity(ItemTIDs[i], X, Y, Z, true, false);
                NoClip = true;
            }

            // Enable/Disable clipping on this item
            if (NoClip)
                SetActorInventory(ItemTIDs[i], "DRPGItemNoClip", 1);
            else
                SetActorInventory(ItemTIDs[i], "DRPGItemNoClipOff", 1);
        }
        
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            // Skip this player if they aren't in the game
            if (!PlayerInGame(i)) continue;

            // Skip this player if they aren't Magnetic
            if (Players(i).Stim.PowerupTimer[STIM_MAGNETIC] <= 0) continue;

            // Skip this player if they are dead
            if (GetActorProperty(Players(i).TID, APROP_Health) <= 0) continue;
            
            
            int *ItemTID = (int *)Player.DropTID.Data;
            
            for (int j = 0; j < Players(i).DropTID.Position; j++)
            {
                NoClip = false;
                if (ClassifyActor(ItemTID[j]) == ACTOR_NONE || ClassifyActor(ItemTID[j]) == ACTOR_WORLD) continue;

                Dist = Distance(ItemTID[j], Players(i).TID);
                Height = GetActorPropertyFixed(Players(i).TID, APROP_Height);
                Divisor = (Dist - 16.0) + Dist;
                Angle = VectorAngle (GetActorX(Players(i).TID) - GetActorX(ItemTID[j]), GetActorY(Players(i).TID) - GetActorY(ItemTID[j]));
                Pitch = VectorAngle (Dist, GetActorZ(Players(i).TID) - GetActorZ(ItemTID[j]));

                // Calculate the amount based on close versus far distance
                if (Dist < 16.0)
                    Amount = 1.0;
                else
                    Amount = 16.0 / Divisor;

                // Calculate the lerped positions
                X = (Amount * 16.0) * Cos(Angle) * Cos(Pitch);
                Y = (Amount * 16.0) * Sin(Angle) * Cos(Pitch);
                Z = (Amount * 16.0) * Sin(Pitch);

                SetActorVelocity(ItemTID[j], X, Y, Z, true, false);
                NoClip = true;
                
                if (NoClip)
                    SetActorInventory(ItemTID[j], "DRPGItemNoClip", 1);
                else
                    SetActorInventory(ItemTID[j], "DRPGItemNoClipOff", 1);
            }
        }

        Delay(4);
    }
}

// Initializes an item and adds it to the Items array
NamedScript DECORATE void ItemInit()
{
    int TID;
    
    // Delay while the Items array is being initialized
    while (!ItemTIDsInitialized) Delay(1);

    // [KS] Some items cease existing once they're added to inventory
    // If that's the case, return early so we don't add empty item info
    if (ClassifyActor(0) == ACTOR_WORLD)
        return;

    // Add to Items array
    for (int i = 0; i < MAX_ITEMS; i++)
        if (ItemTIDs[i] == -1)
        {
            //if (GetCVar("drpg_debug"))
            //    Log("\CdDEBUG: \C-Item \Cd%S\C- added (Index \Cd%d\C-)", GetActorClass(0), i);

            // Doesn't have a TID, so assign it one
            if (ActivatorTID() == 0)
            {
                TID = UniqueTID();
                ItemTIDs[i] = TID;
                Thing_ChangeTID(0, TID);
            }
            else
                ItemTIDs[i] = ActivatorTID();

            break;
        }
}

NamedScript Console void ItemDump()
{
    Log("\Cd  ===== ITEM ARRAY DATA =====");

    for (int i = 0; i < MAX_ITEMS; i++)
    {
        if (ItemTIDs[i] == -1) break;
        Log("\Cd%d\C-: \Cd%S", i, GetActorClass(ItemTIDs[i]));
    }

    Log("\Cd  ===== ITEM ARRAY DATA END =====");
}

// Dynamic Loot Generation System
NamedScript bool DynamicLootGeneratorCheckRemoval(int TID, fixed Z)
{
    bool Remove = true;

    for (int i = 0; ItemTIDs[i] != -1; i++)
    {
        // If this spot is blank, skip it
        if (ClassifyActor(ItemTIDs[i]) == ACTOR_WORLD) continue;

        // Randomly continue for variance
        if (Random(1, 4) == 1) continue;

        bool CanSee = CheckSight(ItemTIDs[i], TID, 0);

        // [KS] Don't spawn stuff on special-effects flats like skyfloor or blackness, or at extreme height differences, because more often than not those are used as void space filler or instakill floors.
        // Examples: Epic2 MAP14, SF2012 MAP02, some CC4 maps.
        bool BadFloor = (CheckActorFloorTexture(TID, "F_SKY1") || CheckActorFloorTexture(TID, "F_SKY2") || CheckActorFloorTexture(TID, "BLACK") || CheckActorFloorTexture(TID, "FBLACK") || CheckActorFloorTexture(TID, "ALLBLAKF"));

        if (CanSee && !BadFloor && Abs(Z - GetActorZ(ItemTIDs[i])) <= 24)
        {
            Remove = false;
            break;
        }
    }

    return Remove;
}

NamedScript OptionalArgs(1) void DynamicLootGenerator(str Actor, int MaxItems)
{
    LogMessage(StrParam("Running DynamicLootGenerator to create %d items of %S", MaxItems, Actor), LOG_DEBUG);
    fixed LowerX = GetActorX(0);
    fixed UpperX = GetActorX(0);
    fixed LowerY = GetActorY(0);
    fixed UpperY = GetActorY(0);
    int LevelNum = CurrentLevel->LevelNum;
    int Items = 0;
    int Iterations = 0;
    int NumItems = 0;

    // Initial delay to make sure all items have been added/initialized in the level
    while (!ItemTIDsInitialized) Delay(1);
    Delay(4);

    // Determine the max amount of items to create if it's not specifically specified
    if (MaxItems == 0)
    {
        MaxItems = Random(LevelNum / GameSkill(), LevelNum) + ((AveragePlayerLuck() + AveragePlayerLevel()) / (GameSkill() * 2));
        MaxItems *= GetCVarFixed("drpg_lootgen_factor");
    }

    if (MaxItems == 0)
    {
        if (GetCVar("drpg_debug"))
            Log("\CdDebug: \C-Skipped item generation.");
        return;
    }
    else if (MaxItems < 0)
    {
        // [KS] Negative luck? Na na nana na! *raspberry*
        if (CurrentLevel->Event == MAPEVENT_MEGABOSS)
            return;

        MaxItems = -MaxItems;
        if (CurrentLevel != NULL && CurrentLevel->Event == MAPEVENT_ONEMONSTER)
            Actor = GetMissionMonsterActor(CurrentLevel->SelectedMonster->Actor);
        else
            Actor = "DRPGGenericMonsterDropper";
    }

    fixed ItemX, ItemY;

    // Find the furthest item from the player to determine some approximate boundaries to work with
    for (int i = 0; ItemTIDs[i] != -1; i++)
    {
        if (ClassifyActor(ItemTIDs[i]) == ACTOR_NONE) continue;

        NumItems = i + 1;

        ItemX = GetActorX(ItemTIDs[i]);
        ItemY = GetActorY(ItemTIDs[i]);

        if (LowerX > ItemX) LowerX = ItemX;
        if (UpperX < ItemX) UpperX = ItemX;
        if (LowerY > ItemY) LowerY = ItemY;
        if (UpperY < ItemY) UpperY = ItemY;

        if (i % 1000 == 0) Delay(1);
    }

    if (NumItems < 1) // [KS] If we can't find any possible positions to spawn anything, fuck it.
        return;

    int TID, A;
    fixed X, Y, Z;

    while (Items < MaxItems)
    {
        TID = UniqueTID();
        X = RandomFixed(LowerX, UpperX);
        Y = RandomFixed(LowerY, UpperY);
        Z = 0;
        A = 32 * Random(0, 7);

        // Try to keep Z near the floor
        SpawnForced("MapSpot", X, Y, 0, TID, 0);
        Z = GetActorFloorZ(TID);
        Thing_Remove(TID);

        bool Spawned = Spawn("MapSpotGravity", X, Y, Z, TID, A);
        if (Spawned)
        {
            bool Remove = DynamicLootGeneratorCheckRemoval(TID, Z);

            Thing_Remove(TID);
            if (!Remove)
            {
                if (Spawn(Actor, X, Y, Z, TID, A))
                    Items++;
            }
        }

        if (GetCVar("drpg_debug"))
        {
            HudMessage("\CfGenerating Loot\n\Cd%d \Cj/ \Cd%d\n\n\CdActor: \C-%S\n\CdIteration: %d\n\CiBoundaries: %.2k-%.2k, %.2k-%.2k\n\nX: %.2k\nY: %.2k\nZ: %.2k", Items, MaxItems, Actor, Iterations, LowerX, UpperX, LowerY, UpperY, X, Y, Z);
            EndHudMessage(HUDMSG_FADEOUT, MAKE_ID('L', 'O', 'O', 'T'), "White", 1.5, 0.8, 1.5, 0.5);
        }

        Iterations++;

        if (Iterations % 50 == 0) Delay(1);

        /*if (Iterations == 2000) // Trick to restart ourselves if we ran out of iterations
        {
            if (GetCVar("drpg_debug"))
                Log("\CdDebug: \C-Dynamic Loot Generation created \Cd%d\C- items of type \Cd%S\C- and is restarting to place more", Items, Actor);
            DynamicLootGenerator(Actor, MaxItems - Items);
            return;
        }*/
    }

    if (GetCVar("drpg_debug"))
        Log("\CdDebug: \C-Dynamic Loot Generation created \Cd%d\C- items of type \Cd%S", Items, Actor);
}

// Activate Focus Mode
NamedScript KeyBind void ToggleFocusMode()
{
    Player.Focusing = !Player.Focusing;

    if (Player.Focusing)
        FocusMode();
}

// Focus Mode Script
NamedScript void FocusMode()
{
    int PrevEP = Player.EP;
    int RegenWindupSpeed = ((35 * 40) - ((35 * 35) * Player.Regeneration / 200));
    if (RegenWindupSpeed < 35) // Enforce a 1-second wind-up at least
        RegenWindupSpeed = 35;
    int StartWindupSpeed = RegenWindupSpeed;
    int RegenDelay = (RegenWindupSpeed * (Player.EPTime / 4)) / StartWindupSpeed;
    int Buttons, Percent;

    // [KS] Someone did this!
    if (GetActorProperty(0, APROP_Health) <= 0) return;

    // Return if you're already at max EP
    if (Player.EP >= Player.EPMax) return;

    SetPlayerProperty(0, 1, PROP_FROZEN);

    while (Player.Focusing)
    {
        Buttons = GetPlayerInput(PlayerNumber(), INPUT_BUTTONS);
        Percent = 100 - (RegenWindupSpeed * 100 / StartWindupSpeed);
        fixed X = GetActorX(0) + Cos((fixed)Timer() / 64.0) * 32.0;
        fixed Y = GetActorY(0) + Sin((fixed)Timer() / 64.0) * 32.0;
        fixed Z = GetActorZ(0);

        if (GetActorProperty(0, APROP_Health) <= 0)
        {
            Player.Focusing = false;
            return;
        }

        Player.EPRate = 0;

        PlaySound(0, "misc/epfocus", CHAN_BODY, 0.5, true, ATTN_NORM);
        SpawnForced("DRPGEPFocusAura", X, Y, Z, 0, 0);
        DrawProgressBar("\CnFocusing", Percent);

        if (RegenWindupSpeed > 1)
            RegenWindupSpeed--;

        if (RegenDelay > 0)
            RegenDelay--;
        else
        {
            Player.EP += Player.EPAmount;
            if (Player.EP > Player.EPMax)
                Player.EP = Player.EPMax;
            RegenDelay = (RegenWindupSpeed * (Player.EPTime / 4)) / StartWindupSpeed;
        }

        if (Buttons > 0 || Player.EP >= Player.EPMax)
            Player.Focusing = false;

        Delay(1);
    }

    Player.Focusing = false; // So we can't gain Regen XP out of thin air
    PlaySound(0, "misc/epfocusdone", CHAN_BODY, 0.5, false, ATTN_NORM);
    SetPlayerProperty(0, 0, PROP_FROZEN);
}

// Death Script
NamedScript Type_DEATH void Dead()
{
    // Reset menu vars
    Player.InMenu = false;
    Player.InShop = false;
    Player.OutpostMenu = 0;

    // Remove Aura
    RemoveAura();

    // Remove Shield
    SetInventory("DRPGShield", 0);

    // Remove Status Effect
    for (int i = 0; i < SE_MAX; i++)
        Player.StatusTimer[i] = 0;

    // Drop Credits
    if (GetCVar("drpg_multi_dropcredits") && CheckInventory("DRPGCredits") > 0)
    {
        int DropAmount = CheckInventory("DRPGCredits") / 100 * GetCVar("drpg_multi_dropcredits_percent");

        // Cap out at a million so if you have stupid amounts of Credits you don't freeze/nuke the game
        if (DropAmount > 1000000) DropAmount = 1000000;

        TakeInventory("DRPGCredits", DropAmount);
        DropMoney(PlayerNumber(), 0, DropAmount);
    }

    // Drop Inventory
    if (GetCVar("drpg_multi_dropinv"))
        DropEntireInventory();

    // VENG-R Accessory
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        // Skip yourself
        if (Player.TID == Players(i).TID) continue;

        if (Players(i).Shield.Accessory && Players(i).Shield.Accessory->PassiveEffect == SHIELD_PASS_AVENGER)
        {
            GiveActorInventory(Players(i).TID, "DRPGShieldAvengerDamage", 1);
            GiveActorInventory(Players(i).TID, "DRPGShieldAvengerDefense", 1);
        }
    }

    Player.Payout.Deaths++;

    // Remove TID
    Thing_ChangeTID(Player.TID, 0);
}

// Respawn
NamedScript Type_RESPAWN void Respawn()
{
    // Reassign TID
    if (CompatMode == COMPAT_DRLA)
        Delay(4);
    AssignTIDs();

    // Heal to max health
    Player.ActualHealth = Player.HealthMax;
    SetActorProperty(0, APROP_Health, Player.ActualHealth);

    // XP/Rank Penalty
    if (GetCVar("drpg_multi_takexp"))
    {
        long int XPPenalty = (long int)(XPTable[Player.Level] * GetCVar("drpg_multi_takexp_percent") / 100);
        long int RankPenalty = (long int)(RankTable[Player.RankLevel] * GetCVar("drpg_multi_takexp_percent") / 100);

        if (XPPenalty > 0 || RankPenalty > 0)
        {
            Player.XP -= XPPenalty;
            Player.Rank -= RankPenalty;
            SetFont("BIGFONT");
            HudMessage("\CjXP -%d\n\CkRank -%d", XPPenalty, RankPenalty);
            EndHudMessage(HUDMSG_FADEOUT | HUDMSG_LOG, 0, "White", 1.5, 0.75, 2.0, 2.0);
        }
    }

    // Restore EP if CVAR is set
    if (GetCVar("drpg_multi_restoreep"))
        Player.EP = Player.EPMax;

    // Give a box of ammo if a specific ammo type is empty if the CVAR is set
    if (GetCVar("drpg_multi_restoreammo"))
    {
        if (CheckInventory("Clip") < GetAmmoAmount("Clip") * (Player.Capacity / 10))
            SetInventory("Clip", GetAmmoAmount("Clip") * (Player.Capacity / 10));
        if (CheckInventory("Shell") < GetAmmoAmount("Shell") * (Player.Capacity / 10))
            SetInventory("Shell", GetAmmoAmount("Shell") * (Player.Capacity / 10));
        if (CheckInventory("RocketAmmo") < GetAmmoAmount("RocketAmmo") * (Player.Capacity / 10))
            SetInventory("RocketAmmo", GetAmmoAmount("RocketAmmo") * (Player.Capacity / 10));
        if (CheckInventory("Cell") < GetAmmoAmount("Cell") * (Player.Capacity / 10))
            SetInventory("Cell", GetAmmoAmount("Cell") * (Player.Capacity / 10));
    }

    // Apply camera textures and vars
    SetCameraToTexture(Player.TID, StrParam("P%iVIEW", PlayerNumber() + 1), 110);
    SetCameraToTexture(Player.TID, StrParam("P%iSVIEW", PlayerNumber() + 1), 90);
    Player.PlayerView = PlayerNumber();

    // Run Scripts
    Loop();
    PlayerDamage();
    MoneyChecker();
    DamageNumbers();
    InfoPopoffs();
    HealthBars();
    ShieldTimer();

    // NU-YU Accessory
    if (Player.Shield.Accessory && Player.Shield.Accessory->PassiveEffect == SHIELD_PASS_HYPERION)
    {
        ActivatorSound("shield/newu", 127);
        Player.Shield.Active = true;
        Player.Shield.Charge = Player.Shield.Capacity;
        GiveInventory("DRPGShieldNewUProtection", 1);
    }

    // Quick Teleport
    if (GetPlayerInput(PlayerNumber(), INPUT_BUTTONS) & BT_SPEED)
    {
        int PlayerNum = -1;
        while (PlayerNum == -1 || PlayerNum == PlayerNumber() || !PlayerInGame(PlayerNum))
            PlayerNum = Random(0, PlayerCount());
        PlayerTeleport(PlayerNum);
        Player.EP -= ScaleEPCost(50);
    }
}

str const Loadout_WeaponInfo[LOADOUT_WEAPONS][3] =
{
    // CVAR, Vanilla Actor, Extras Actor

    { "drpg_start_weapon_chainsaw",         "DRPGChainsaw",         "DRPGChainsawExtras",       },
    { "drpg_start_weapon_pistol",           "DRPGPistol",           "DRPGPistolExtras",         },
    { "drpg_start_weapon_shotgun",          "DRPGShotgun",          "DRPGShotgunExtras",        },
    { "drpg_start_weapon_ssg",              "DRPGSuperShotgun",     "DRPGSuperShotgunExtras",   },
    { "drpg_start_weapon_chaingun",         "DRPGChaingun",         "DRPGChaingunExtras",       },
    { "drpg_start_weapon_rocketlauncher",   "DRPGRocketLauncher",   "DRPGRocketLauncherExtras", },
    { "drpg_start_weapon_plasmarifle",      "DRPGPlasmaRifle",      "DRPGPlasmaRifleExtras",    },
    { "drpg_start_weapon_bfg",              "DRPGBFG9000",          "DRPGBFG9000Extras",        },
};
str const Loadout_ArmorInfo[LOADOUT_ARMORS] =
{
    "DRPGGreenArmorEffect",
    "DRPGBlueArmorEffect",
    "DRPGYellowArmorEffect",
    "DRPGRedArmorEffect",
    "DRPGWhiteArmorEffect",
    "DRPGReinforcedGreenArmorEffect",
    "DRPGReinforcedBlueArmorEffect",
    "DRPGReinforcedYellowArmorEffect",
    "DRPGReinforcedRedArmorEffect",
    "DRPGReinforcedWhiteArmorEffect",
};
str const Loadout_ShieldPartColorCodes[LOADOUT_SHIELDPARTS] =
{
    "\Cv",  // Capacity +
    "\Ch",  // Capacity -
    "\Cd",  // Charge Rate +
    "\Cq",  // Charge Rate -
    "\Cr",  // Delay Rate +
    "\Cg",  // Delay Rate -
};
str const Loadout_ShieldAccessoryColorCodes[LOADOUT_SHIELDACCS] =
{
    "\Cv",  // Shield Capacity
    "\Cg",  // Strength / Damage
    "\Ca",  // Vitality / Health
    "\Ci",  // Agility / Speed
    "\Cd",  // Defense / Armor / Shield Charge
    "\Cf",  // Luck / Money
    "\Cn",  // Energy
    "\Ch",  // Capacity / Ammo
    "\Ck",  // Rank
    "\Cc",  // Augmentation
    "\Cq",  // Powerup
    "\Cj",  // Misc / Multi-Category
};
str const Loadout_DRLAWeaponColorCodes[LOADOUT_DRLAWEAPONS] =
{
    "",     // Common
    "\Cv",  // Assembled
    "\Ct",  // Exotic
    "\Ci",  // Superior
    "\Cd",  // Unique
    "\Cg",  // Demonic
    "\Cf",  // Legendary
};
str const Loadout_DRLAArmorColorCodes[LOADOUT_DRLAARMORS] =
{
    "",     // Common
    "\Cv",  // Assembled
    "\Ct",  // Exotic
    "\Cc",  // Onyx
    "\Cd",  // Unique
    "\Cf",  // Legendary
};
str const Loadout_DRLAModPacks[LOADOUT_DRLAMODPACKS] =
{
    "RLPowerModItem",
    "RLBulkModItem",
    "RLAgilityModItem",
    "RLTechnicalModItem",
    "RLSniperModItem",
    "RLFirestormModItem",
    "RLNanoModItem",
    "RLOnyxModItem",
};

// Shield Databases
static ItemInfoPtr Loadout_ShieldBodyTypes[LOADOUT_SHIELDPARTS][MAX_BODIES];
static ItemInfoPtr Loadout_ShieldBatteryTypes[LOADOUT_SHIELDPARTS][MAX_BATTERIES];
static ItemInfoPtr Loadout_ShieldCapacitorTypes[LOADOUT_SHIELDPARTS][MAX_CAPACITORS];
static ItemInfoPtr Loadout_ShieldAccessoryTypes[LOADOUT_SHIELDACCS][MAX_ACCESSORIES];
static int Loadout_BodyCount[LOADOUT_SHIELDPARTS];
static int Loadout_BatteryCount[LOADOUT_SHIELDPARTS];
static int Loadout_CapacitorCount[LOADOUT_SHIELDPARTS];
static int Loadout_AccessoryCount[LOADOUT_SHIELDACCS];

// DRLA Databases
static ItemInfoPtr Loadout_DRLAWeaponTypes[LOADOUT_DRLAWEAPONS][ITEM_MAX];
static ItemInfoPtr Loadout_DRLAArmorTypes[LOADOUT_DRLAARMORS][ITEM_MAX];
static ItemInfoPtr Loadout_DRLABootsTypes[LOADOUT_DRLAARMORS][ITEM_MAX];
static int Loadout_DRLAWeaponCount[LOADOUT_DRLAWEAPONS];
static int Loadout_DRLAArmorCount[LOADOUT_DRLAARMORS];
static int Loadout_DRLABootsCount[LOADOUT_DRLAARMORS];

NamedScript void SortStartingItems()
{
    // Build Shield and DRLA item databases based on their respective items' color codes
    for (int i = 0; i < MAX_BODIES; i++)
        for (int j = 0; j < LOADOUT_SHIELDPARTS; j++)
            if (Contains(ShieldParts[0][i].Name, Loadout_ShieldPartColorCodes[j]))
            {
                Loadout_ShieldBodyTypes[j][Loadout_BodyCount[j]++] = FindItemInCategory(ShieldParts[0][i].Actor, 5);
                break;
            }
    for (int i = 0; i < MAX_BATTERIES; i++)
        for (int j = 0; j < LOADOUT_SHIELDPARTS; j++)
            if (Contains(ShieldParts[1][i].Name, Loadout_ShieldPartColorCodes[j]))
            {
                Loadout_ShieldBatteryTypes[j][Loadout_BatteryCount[j]++] = FindItemInCategory(ShieldParts[1][i].Actor, 5);
                break;
            }
    for (int i = 0; i < MAX_CAPACITORS; i++)
        for (int j = 0; j < LOADOUT_SHIELDPARTS; j++)
            if (Contains(ShieldParts[2][i].Name, Loadout_ShieldPartColorCodes[j]))
            {
                Loadout_ShieldCapacitorTypes[j][Loadout_CapacitorCount[j]++] = FindItemInCategory(ShieldParts[2][i].Actor, 5);
                break;
            }
    for (int i = 0; i < MAX_ACCESSORIES; i++)
        for (int j = 0; j < LOADOUT_SHIELDACCS; j++)
            if (Contains(ShieldAccessories[i].Name, Loadout_ShieldAccessoryColorCodes[j]))
            {
                Loadout_ShieldAccessoryTypes[j][Loadout_AccessoryCount[j]++] = FindItemInCategory(ShieldAccessories[i].Actor, 5);
                break;
            }
    for (int i = 0; i < ItemMax[0]; i++)
        for (int j = 1; j < LOADOUT_DRLAWEAPONS; j++)
        {
            if (i < 10) // Special catch for Common
            {
                Loadout_DRLAWeaponTypes[0][Loadout_DRLAWeaponCount[0]++] = &ItemData[0][i];
                break;
            }
            else if (Contains(ItemData[0][i].Name, Loadout_DRLAWeaponColorCodes[j]))
            {
                Loadout_DRLAWeaponTypes[j][Loadout_DRLAWeaponCount[j]++] = &ItemData[0][i];
                break;
            }
        };
    for (int i = 1; i < ItemMax[3]; i++)
        for (int j = 1; j < LOADOUT_DRLAARMORS; j++)
        {
            // Onyx is handled below
            if (j == 3) continue;

            if (i < 4) // Special catch for Common
            {
                Loadout_DRLAArmorTypes[0][Loadout_DRLAArmorCount[0]++] = &ItemData[3][i];
                break;
            }
            else if (Contains(ItemData[3][i].Name, "Onyx")) // Special catch for Onyx
            {
                Loadout_DRLAArmorTypes[3][Loadout_DRLAArmorCount[3]++] = &ItemData[3][i];
                break;
            }
            else if (Contains(ItemData[3][i].Name, Loadout_DRLAArmorColorCodes[j]))
            {
                Loadout_DRLAArmorTypes[j][Loadout_DRLAArmorCount[j]++] = &ItemData[3][i];
                break;
            }
        }
    for (int i = 0; i < ItemMax[9]; i++)
        for (int j = 1; j < LOADOUT_DRLAARMORS; j++)
        {
            if (i < 3) // Special catch for Common
            {
                Loadout_DRLABootsTypes[0][Loadout_DRLABootsCount[0]++] = &ItemData[9][i];
                break;
            }
            else if (Contains(ItemData[9][i].Name, Loadout_DRLAArmorColorCodes[j]))
            {
                Loadout_DRLABootsTypes[j][Loadout_DRLABootsCount[j]++] = &ItemData[9][i];
                break;
            }
        }
}

// Gives the player their default loadout based on their starting options
NamedScript void Loadout_GiveAugs()
{
    // Active Augs
    int ActiveAugs = 0;
    int Type;
    
    while (ActiveAugs < GetActivatorCVar("drpg_start_aug_amount"))
    {
        Type = Random(0, AUG_MAX - 1);

        // Skip this aug if it's already active
        if (Player.Augs.Level[Type] > 0) continue;

        Player.Augs.Level[Type]++;
        ActiveAugs++;
    }

    // Aug Canisters/Upgrades
    GiveInventory("DRPGAugCanister", GetActivatorCVar("drpg_start_aug_canisters"));
    GiveInventory("DRPGAugUpgradeCanister", GetActivatorCVar("drpg_start_aug_upgrades"));

    // Aug Slots
    Player.Augs.BaseSlots = GetActivatorCVar("drpg_start_aug_slots");
}

NamedScript void Loadout_GiveShieldParts()
{
    // Shield Parts
    for (int i = 0; i < 3; i++)
    {
        int *Count[3] =
        {
            &Loadout_BodyCount[0],
            &Loadout_BatteryCount[0],
            &Loadout_CapacitorCount[0]
        };
        int Total[3] =
        {
            GetActivatorCVar("drpg_start_shield_amount_body"),
            GetActivatorCVar("drpg_start_shield_amount_battery"),
            GetActivatorCVar("drpg_start_shield_amount_capacitor")
        };
        int Current[3];

        // Calculate total if we specify a type so that overflows or explosions don't happen
        if (GetActivatorCVar("drpg_start_shield_type") >= 0)
            if (Total[i] > Count[i][GetActivatorCVar("drpg_start_shield_type")])
                Total[i] = Count[i][GetActivatorCVar("drpg_start_shield_type")];

        while (Current[i] < Total[i])
        {
            int Category = Random(0, LOADOUT_SHIELDPARTS - 1);

            // Force the category type if the CVAR is set
            if (GetActivatorCVar("drpg_start_shield_type") >= 0)
                Category = GetActivatorCVar("drpg_start_shield_type");

            for (int j = 0; j < Count[i][Category]; j++)
            {
                ItemInfoPtr *Type[3];

                Type[0] = &Loadout_ShieldBodyTypes[Category][j];
                Type[1] = &Loadout_ShieldBatteryTypes[Category][j];
                Type[2] = &Loadout_ShieldCapacitorTypes[Category][j];

                if (!CheckInventory((*(Type[i]))->Actor))
                {
                    GiveInventory((*(Type[i]))->Actor, 1);
                    Current[i]++;
                    break;
                }
            }
        }
    }
}

NamedScript void Loadout_GiveShieldAccessories()
{
    // Shield Accessories
    int TotalAccessories = GetActivatorCVar("drpg_start_shield_amount_acc");
    int Accessories = 0;
    int Category;
    if (GetActivatorCVar("drpg_start_shield_type_acc") >= 0)
        if (TotalAccessories > Loadout_AccessoryCount[GetActivatorCVar("drpg_start_shield_type_acc")])
            TotalAccessories = Loadout_AccessoryCount[GetActivatorCVar("drpg_start_shield_type_acc")];

    while (Accessories < TotalAccessories)
    {
        Category = Random(0, LOADOUT_SHIELDACCS - 1);

        // Force the category type if the CVAR is set
        if (GetActivatorCVar("drpg_start_shield_type_acc") >= 0)
            Category = GetActivatorCVar("drpg_start_shield_type_acc");

        for (int i = 0; i < Loadout_AccessoryCount[Category]; i++)
        {
            ItemInfoPtr Type = Loadout_ShieldAccessoryTypes[Category][i];

            if (!CheckInventory(Type->Actor))
            {
                GiveInventory(Type->Actor, 1);
                Accessories++;
                break;
            }
        }
    }
}

NamedScript void Loadout_GiveStims()
{
    // Stims
    int Injectors = 0;

    while (Injectors < GetActivatorCVar("drpg_start_stim_injectors"))
    {
        switch (Random(1, 4))
        {
        case 1: // Small
            if (CheckInventory("DRPGStimSmall") >= 1000) continue;
            GiveInventory("DRPGStimSmall", 1);
            break;
        case 2: // Medium
            if (CheckInventory("DRPGStimMedium") >= 1000) continue;
            GiveInventory("DRPGStimMedium", 1);
            break;
        case 3: // Large
            if (CheckInventory("DRPGStimLarge") >= 1000) continue;
            GiveInventory("DRPGStimLarge", 1);
            break;
        case 4: // XL
            if (CheckInventory("DRPGStimXL") >= 1000) continue;
            GiveInventory("DRPGStimXL", 1);
            break;
        }

        Injectors++;
    }
}

NamedScript void Loadout_GiveVials()
{
    int Vials = 0;
    int MaxVials = GetActivatorCVar("drpg_start_stim_vials");
    if (MaxVials > 3000) MaxVials = 3000;
    int Type;

    while (Vials < MaxVials)
    {
        Type = Random(0, STIM_MAX - 1);
        bool Maxed = true;

        // Check to make sure all the vials aren't maxed
        for (int i = 0; i < STIM_MAX; i++)
            if (Player.Stim.Vials[i] < Player.Capacity * 10)
                Maxed = false;
        if (Maxed) break;

        // Don't include Boosters
        if (!GetActivatorCVar("drpg_start_stim_boosters") && Type >= StimStatsEnd && Type <= StimPowerupStart) continue;

        // Don't include Powerups
        if (!GetActivatorCVar("drpg_start_stim_powerups") && Type >= StimPowerupStart && Type <= StimPowerupEnd) continue;

        // Skip this one if this vial type is full
        if (Player.Stim.Vials[Type] >= Player.Capacity * 10) continue;

        Player.Stim.Vials[Type]++;
        Vials++;
    }
}

// DRLA
NamedScript void Loadout_GiveDRLAEquipment()
{
    bool FoundWeapon = false;
    bool FoundArmor = false;
    bool FoundBoots = false;
    int WeaponCount = 0;
    int MaxWeaponCount = GetActivatorCVar("drpg_start_drla_weapon_amount");
    int ModPackCount = 0;
    int ModPackMax = GetActivatorCVar("drpg_start_drla_modpacks");

    // Weapons
    if (GetActivatorCVar("drpg_start_drla_weapon_type") > 0)
        if (MaxWeaponCount > Loadout_DRLAWeaponCount[GetActivatorCVar("drpg_start_drla_weapon_type")])
            MaxWeaponCount = Loadout_DRLAWeaponCount[GetActivatorCVar("drpg_start_drla_weapon_type")];
    while (WeaponCount < MaxWeaponCount)
    {
        for (int i = 0; i < LOADOUT_DRLAWEAPONS; i++)
        {
            FoundWeapon = false;

            // Force the weapon type if the CVAR is set
            if (GetActivatorCVar("drpg_start_drla_weapon_type") >= 0)
                i = GetActivatorCVar("drpg_start_drla_weapon_type");

            for (int j = 0; j < Loadout_DRLAWeaponCount[i]; j++)
                if (Random(1, 10) == 1 && !CheckInventory(Loadout_DRLAWeaponTypes[i][j]->Actor))
                {
                    GiveInventory(Loadout_DRLAWeaponTypes[i][j]->Actor, 1);
                    WeaponCount++;
                    FoundWeapon = true;
                    break;
                }

            if (FoundWeapon)
                break;
        }
    }
    GiveInventory("RLWeaponLimit", WeaponCount);

    // Armor
    if (GetActivatorCVar("drpg_start_drla_armor") >= -1)
        while (!FoundArmor)
        {
            for (int i = 0; i < LOADOUT_DRLAARMORS; i++)
            {
                // If there is nothing in this category, skip it
                if (GetActivatorCVar("drpg_start_drla_armor") >= 0 && Loadout_DRLAArmorCount[GetActivatorCVar("drpg_start_drla_armor")] == 0)
                {
                    FoundArmor = true;
                    continue;
                }
                else if (GetActivatorCVar("drpg_start_drla_armor") >= 0) // Force the armor type if the CVAR is setup
                    i = GetActivatorCVar("drpg_start_drla_armor");

                for (int j = 0; j < Loadout_DRLAArmorCount[i]; j++)
                    if (Random(1, 10) == 1)
                    {
                        GiveInventory(Loadout_DRLAArmorTypes[i][j]->Actor, 1);
                        FoundArmor = true;
                        break;
                    }

                if (FoundArmor)
                    break;
            }
        }

    // Boots
    if (GetActivatorCVar("drpg_start_drla_boots") >= -1)
        while (!FoundBoots)
        {
            for (int i = 0; i < LOADOUT_DRLAARMORS; i++)
            {
                // If there is nothing in this category, skip it
                if (GetActivatorCVar("drpg_start_drla_boots") >= 0 && Loadout_DRLABootsCount[GetActivatorCVar("drpg_start_drla_boots")] == 0)
                {
                    FoundBoots = true;
                    continue;
                }
                else if (GetActivatorCVar("drpg_start_drla_boots") >= 0) // Force the boots type if the CVAR is setup
                    i = GetActivatorCVar("drpg_start_drla_boots");

                for (int j = 0; j < Loadout_DRLABootsCount[i]; j++)
                    if (Random(1, 10) == 1)
                    {
                        GiveInventory(Loadout_DRLABootsTypes[i][j]->Actor, 1);
                        FoundBoots = true;
                        break;
                    }

                if (FoundBoots)
                    break;
            }
        }

    // Mod Packs
    int Type;
    if (PlayerClass(0) == 2)
        ModPackMax--;
    while (ModPackCount < ModPackMax)
    {
        Type = Random(0, LOADOUT_DRLAMODPACKS - 1);

        // Stop if we're not the technician and we try and give over 4 mods
        if (PlayerClass(0) != 2 && ModPackCount >= 4) break;

        // Skip exotic modpacks if the selection is disabled
        if (Type > 3 && !GetActivatorCVar("drpg_start_drla_modpacks_amount")) continue;

        GiveInventory(Loadout_DRLAModPacks[Type], 1);
        ModPackCount++;
    }
}

NamedScript void DefaultLoadout()
{
    // Weapons
    if (CompatMode != COMPAT_DRLA && CompatMode != COMPAT_LEGENDOOM)
    {
        for (int i = 0; i < 8; i++)
        {
            if (GetActivatorCVar(Loadout_WeaponInfo[i][0]))
            {
                if (CompatMode == COMPAT_EXTRAS)
                {
                    GiveInventory(Loadout_WeaponInfo[i][2], 1);
                    SetWeapon(Loadout_WeaponInfo[i][2]);
                }
                else
                {
                    GiveInventory(Loadout_WeaponInfo[i][1], 1);
                    SetWeapon(Loadout_WeaponInfo[i][1]);
                }
            }
            else
            {
                if (CompatMode == COMPAT_EXTRAS)
                    TakeInventory(Loadout_WeaponInfo[i][2], 1);
                else
                    TakeInventory(Loadout_WeaponInfo[i][1], 1);
            }
        }
    }

    // Ammo
    SetInventory("Clip", GetActivatorCVar("drpg_start_ammo_bullet"));
    SetInventory("Shell", GetActivatorCVar("drpg_start_ammo_shell"));
    SetInventory("RocketAmmo", GetActivatorCVar("drpg_start_ammo_rocket"));
    SetInventory("Cell", GetActivatorCVar("drpg_start_ammo_cell"));

    // Armor
    if (CompatMode != COMPAT_DRLA && GetActivatorCVar("drpg_start_armor") >= 0)
        GiveInventory(Loadout_ArmorInfo[GetActivatorCVar("drpg_start_armor")], 1);

    // Medkit
    Player.Medkit = GetActivatorCVar("drpg_start_medkit");

    Loadout_GiveAugs();
    Loadout_GiveShieldParts();
    Loadout_GiveShieldAccessories();
    Loadout_GiveStims();
    Loadout_GiveVials();

    // Minigame Chips
    GiveInventory("DRPGChipGold", GetActivatorCVar("drpg_start_bonus_goldchips"));
    GiveInventory("DRPGChipPlatinum", GetActivatorCVar("drpg_start_bonus_platchips"));

    // UAC Shop Card
    if (GetActivatorCVar("drpg_start_bonus_shopcard") > 0)
    {
        // Diamond Card
        if (GetActivatorCVar("drpg_start_bonus_shopcard") >= 5)
            GiveInventory("DRPGDiamondUACCard", 1);
        else
            GiveInventory("DRPGUACCard", GetActivatorCVar("drpg_start_bonus_shopcard"));
    }

    if (CompatMode == COMPAT_DRLA)
        Loadout_GiveDRLAEquipment();
}

// Apply values to global vars visible on the HUD
void CheckHUD()
{
    int PlayerNum = PlayerNumber();

    // EP
    EP[PlayerNum] = Player.EP;

    // EP Bar on HUD
    if (Player.EPMax <= 0)
        SetInventory("DRPGEP", 0);
    else
        SetInventory("DRPGEP", Player.EP * 100 / Player.EPMax);

    // Shield
    if (Player.Shield.Capacity > 0)
    {
        Shield[PlayerNum] = Player.Shield.Charge;
        ShieldCapacity[PlayerNum] = Player.Shield.Capacity;
        ShieldHealth[PlayerNum] = Player.ActualHealth;
        SetInventory("DRPGShieldCapacity", (int)(((fixed)Player.Shield.Charge / (fixed)Player.Shield.Capacity) * 100.0));
        SetInventory("DRPGShield", Player.Shield.Active);
    }
}

void CheckKeys()
{
    str const Keycards[6] =
    {
        "DRPGRedCard",
        "DRPGYellowCard",
        "DRPGBlueCard",
        "DRPGRedSkull",
        "DRPGYellowSkull",
        "DRPGBlueSkull"
    };

    // Don't bother if we're not in multiplayer
    if (!InMultiplayer) return;

    for (int i = 0; i < 6; i++)
    {
        if (CheckInventory(Keycards[i]))
        {
            for (int j = 0; j < MAX_PLAYERS; j++)
            {
                if (!PlayerInGame(j)) continue;
                if (CheckActorInventory(Players(j).TID, Keycards[i])) continue;

                SetActorInventory(Players(j).TID, Keycards[i], 1);
            }
        }
    }
}

void CheckCompatibility()
{
    bool Success = false;
    int TID = UniqueTID();

    if (GetCVar("drpg_debug"))
        Log("\CdDEBUG: \C-Checking Compatibility...");

    CompatMode = COMPAT_NONE;
    WadSmoosh = false;
	ColourfulHell = false;
    
    //WadSmoosh
    Success = SpawnForced("DRPGWadSmooshActive", 0, 0, 0, TID, 0);
    if (Success)
    {
        if (GetCVar("drpg_debug"))
            Log("\CdDEBUG: \CaWadSmoosh\C- detected");
        WadSmoosh = true;
        Thing_Remove(TID);
    }

    // Extras
    Success = SpawnForced("DRPGExtrasIsLoaded", 0, 0, 0, TID, 0);
    if (Success)
    {
        if (GetCVar("drpg_debug"))
            Log("\CdDEBUG: \CaExtras\C- detected");
        CompatMode = COMPAT_EXTRAS;
        Thing_Remove(TID);
        return;
    }

	// Colourful Hell
	Success = SpawnForced("GreenZombie", 0, 0, 0, TID, 0);
    if (Success)
    {
        if (GetCVar("drpg_debug"))
            Log("\CdDEBUG: \CdColourful Hell\C- detected");
        ColourfulHell = true;
        Thing_Remove(TID);
    }
	
    // DoomRL
    Success = SpawnForced("RLKateMatterstormHarnessArmorToken", 0, 0, 0, TID, 0);
    if (Success)
    {
        if (GetCVar("drpg_debug"))
            Log("\CdDEBUG: \CdDoomRL \C-detected");
        CompatMode = COMPAT_DRLA;
        SetInventory("DRPGDRLAActive", 1);
        Thing_Remove(TID);

        Success = SpawnForced("RLBaronOfHell", 0, 0, 0, TID, 0);
        if (!Success)
        {
            SetHudSize(640, 480);
            FadeRange(0, 0, 0, 1.0, 0, 0, 0, 1.0, 0);

            HudMessage("\CgLISTEN UP\n\n\C-You currently have \CfDoomRL Arsenal\C- loaded. In order for this mod to work with DoomRPG, you also need the \CfDoomRL Monster Pack\C-. Go here to download it: http://tinyurl.com/DoomRLArsenal\n\nThanks.");
            EndHudMessageBold(HUDMSG_PLAIN | HUDMSG_LOG, 772, "White", 0.1, 0.1, 0.01);

            SetFont("BigFont");
            HudMessage("LISTEN UP");
            EndHudMessageBold(HUDMSG_PLAIN, 772, "Red", 320.4, 200.0, 0.01);

            SetFont("SmallFont");
            HudMessage("You currently have \CfDoomRL Arsenal\C- loaded. In order for this mod to work with DoomRPG, you also need the \CfDoomRL Monster Pack\C-. Go here to download it: http://tinyurl.com/DoomRLArsenal\n\nThanks.");
            EndHudMessageBold(HUDMSG_PLAIN, 773, "White", 320.4, 232.0, 0.01);
        }
        else
        {
            Thing_Remove(TID);
            return;
        }
    }

    // LegenDoom
    Success = SpawnForced("LDLegendaryZombie", 0, 0, 0, TID, 0);
    if (Success)
    {
        if (GetCVar("drpg_debug"))
            Log("\CdDEBUG: \CdLegenDoom\C- detected");
        CompatMode = COMPAT_LEGENDOOM;
        Thing_Remove(TID);
        return;
    }
}

void AssignTIDs()
{
    Player.TID = PLAYER_TID + PlayerNumber();
    Thing_ChangeTID(0, Player.TID);

    if (GetCVar("drpg_debug"))
        Log("\CdDEBUG: Player TID: %d", Player.TID);
}
