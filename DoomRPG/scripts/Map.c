#include "Defs.h"

#include <stdlib.h>
#include <limits.h>

#include "ItemData.h"
#include "Map.h"
#include "Mission.h"
#include "Monsters.h"
#include "RPG.h"
#include "Stats.h"
#include "Outpost.h"
#include "Utils.h"

// Level Info
DynamicArray WSMapPacks[MAX_WSMAPPACKS];
DynamicArray *KnownLevels = &WSMapPacks[0];
LevelInfo *CurrentLevel;
LevelInfo *PreviousLevel;
LevelInfo *TransporterLevel;
LevelInfo *DefaultOutpost;
int PreviousLevelNum;
int PreviousPrimaryLevelNum;
bool UsedSecretExit;
bool WaitingForReplacements;
int AllBonusMaps; // For the OCD Shield
int CurrentSkill; // Keeps track of skill changes based on events

//WadSmoosh
bool MapPackActive[MAX_WSMAPPACKS];

// Local
static int PassingEventTimer;
static bool DisableEvent;

//WadSmoosh Local
static bool WadSmooshInitialized;

// Map Init Script
NamedScript Type_OPEN void MapInit()
{
    // Running the game for the first time
    if (KnownLevels->Data == NULL)
    {
        if (GetCVar("drpg_monster_mapweight") < 1)
            SetCVar("drpg_monster_mapweight", 1);
        if (GetCVar("drpg_monster_mapweight") > 1000)
            SetCVar("drpg_monster_mapweight", 1000);
        int StartMapNum = GetCVar("drpg_monster_mapweight");
        CurrentSkill = GameSkill() - 1;
        UsedSecretExit = false;
        PreviousLevelSecret = false;
        PreviousLevelNum = StartMapNum - 1;
        PreviousPrimaryLevelNum = StartMapNum - 1;
        ArrayCreate(KnownLevels, "Levels", 32, sizeof(LevelInfo));
        CurrentLevel = NULL;
        PreviousLevel = NULL;
        PassingEventTimer = GetCVar("drpg_mapevent_eventtime") * 35 * 60;
        DisableEvent = true;
        
        if (KnownLevels->Data == NULL)
        {
            Log("\CgWARNING: \CaCould not allocate level info!");
            return;
        }
        
        // Quick check to add the standard Outpost if we didn't start on it
        if (ThingCountName("DRPGOutpostMarker", 0) == 0 && ThingCountName("DRPGArenaMarker", 0) == 0)
        {
            LevelInfo *OutpostMap = &((LevelInfo *)KnownLevels->Data)[KnownLevels->Position++];
            OutpostMap->LevelNum = 0;
            OutpostMap->LumpName = "OUTPOST";
            OutpostMap->NiceName = "UAC Outpost";
            OutpostMap->Completed = true;
            OutpostMap->UACBase = true;
            OutpostMap->UACArena = false;
            OutpostMap->NeedsRealInfo = false;
            
            DefaultOutpost = OutpostMap;
            
            // [KS] Not right now.
            /*
            LevelInfo *ArenaMap = &((LevelInfo *)KnownLevels.Data)[KnownLevels.Position++];
            ArenaMap->LevelNum = 0;
            ArenaMap->LumpName = "ARENA";
            ArenaMap->NiceName = "UAC Arena";
            ArenaMap->Completed = true;
            ArenaMap->UACArena = true;
            ArenaMap->NeedsRealInfo = false;
            */
        }
    }
    
    CurrentLevel = FindLevelInfo();
    
    if (CurrentLevel == NULL) // New map - We need to create new info for it
    {
        if (KnownLevels->Position == KnownLevels->Size)
            ArrayResize(KnownLevels);
        
        CurrentLevel = &((LevelInfo *)KnownLevels->Data)[KnownLevels->Position++];
        
        if (KnownLevels->Data == NULL)
        {
            Log("\CgWARNING: \CaCould not allocate level info!");
            return;
        }
        
        CurrentLevel->LevelNum = 0;
        CurrentLevel->NeedsRealInfo = true;
    }
    
    if (CurrentLevel->NeedsRealInfo)
    {
        // [KS] Is this an outpost or an arena?
        if (ThingCountName("DRPGOutpostMarker", 0) > 0)
        {
            CurrentLevel->LevelNum = 0;
            CurrentLevel->SecretMap = false;
            
            CurrentLevel->LumpName = StrParam("%tS", PRINTNAME_LEVEL);
            CurrentLevel->NiceName = StrParam("%tS", PRINTNAME_LEVELNAME);
            
            CurrentLevel->Completed = true;
            CurrentLevel->UACBase = true;
            CurrentLevel->UACArena = false;
            
            CurrentLevel->Event = UACEVENT_NONE;
            CurrentLevel->NeedsRealInfo = false;
        }
        else if (ThingCountName("DRPGArenaMarker", 0) > 0)
        {
            CurrentLevel->LevelNum = 0;
            CurrentLevel->SecretMap = false;
            
            CurrentLevel->LumpName = StrParam("%tS", PRINTNAME_LEVEL);
            CurrentLevel->NiceName = StrParam("%tS", PRINTNAME_LEVELNAME);

            CurrentLevel->Completed = true;
            CurrentLevel->UACBase = false;
            CurrentLevel->UACArena = true;
            
            CurrentLevel->Event = UACEVENT_NONE;
            CurrentLevel->NeedsRealInfo = false;
        }
        else
        {
            // [KS] Generate map numbers on the fly based on visitation order and
            // exit usage. The map order for a non-hub is typically a tree - one
            // linear path with optional branching (secret-exit-based) paths that
            // stop in dead ends. Using that knowledge, we can deduce that the maps
            // will travel in a straight line to the end for the most part, and
            // predict the next map number from the last non-secret map that we've
            // discovered.
            
            // This will allow us to not have to rely on LEVELNUM since that can,
            // and most often will, wind up being absent for user maps that provide
            // their own MAPINFO definitions.
            
            if (CurrentLevel->LevelNum == 0)
            {
                PreviousLevelNum++;
                if (!UsedSecretExit)
                    PreviousLevelNum = ++PreviousPrimaryLevelNum;
                
                if (PreviousLevelNum > 1000)
                    PreviousLevelNum = 1000;
                if (PreviousPrimaryLevelNum > 1000)
                    PreviousPrimaryLevelNum = 1000;
                
                CurrentLevel->LevelNum = PreviousLevelNum;
                CurrentLevel->SecretMap = UsedSecretExit;
            }
            
            if (GetCVar("drpg_debug"))
                Log ("\CdDEBUG: \CjLevel number: \Cd%d", CurrentLevel->LevelNum);
            
            CurrentLevel->LumpName = StrParam("%tS", PRINTNAME_LEVEL);
            CurrentLevel->NiceName = StrParam("%tS", PRINTNAME_LEVELNAME);
            
            CurrentLevel->UACBase = false;
            CurrentLevel->UACArena = false;
            
            CurrentLevel->Completed = false;
            
            CurrentLevel->MaxMonstersKilled = GetLevelInfo(LEVELINFO_KILLED_MONSTERS);
            CurrentLevel->MaxTotalMonsters = GetLevelInfo(LEVELINFO_TOTAL_MONSTERS);
            CurrentLevel->MaxMonsterPercentage = CalcPercent(GetLevelInfo(LEVELINFO_KILLED_MONSTERS), GetLevelInfo(LEVELINFO_TOTAL_MONSTERS));
            CurrentLevel->MaxItemsFound = GetLevelInfo(LEVELINFO_FOUND_ITEMS);
            CurrentLevel->MaxTotalItems = GetLevelInfo(LEVELINFO_TOTAL_ITEMS);
            CurrentLevel->MaxItemPercentage = CalcPercent(GetLevelInfo(LEVELINFO_FOUND_ITEMS), GetLevelInfo(LEVELINFO_TOTAL_ITEMS));
            CurrentLevel->MaxSecretsFound = GetLevelInfo(LEVELINFO_FOUND_SECRETS);
            CurrentLevel->MaxTotalSecrets = GetLevelInfo(LEVELINFO_TOTAL_SECRETS);
            CurrentLevel->MaxSecretPercentage = CalcPercent(GetLevelInfo(LEVELINFO_FOUND_SECRETS), GetLevelInfo(LEVELINFO_TOTAL_SECRETS));
            
            // For rank giving and mission counting
            CurrentLevel->UniqueSecrets = GetLevelInfo(LEVELINFO_FOUND_SECRETS);
            
            CurrentLevel->ShortestTime = 0x7FFFFFFF;
            
            // These never change so we don't need to update them
            CurrentLevel->Par = GetLevelInfo(LEVELINFO_PAR_TIME);
            CurrentLevel->Sucks = GetLevelInfo(LEVELINFO_SUCK_TIME);
            
            CurrentLevel->KillBonus = false;
            CurrentLevel->ItemsBonus = false;
            CurrentLevel->SecretsBonus = false;
            CurrentLevel->AllBonus = false;
            CurrentLevel->ParBonus = false;
            
            // Check to see if these are unobtainable
            if (CurrentLevel->MaxTotalMonsters == 0)
                CurrentLevel->KillBonus = true;
            if (CurrentLevel->MaxTotalItems == 0)
                CurrentLevel->ItemsBonus = true;
            if (CurrentLevel->MaxTotalSecrets == 0)
                CurrentLevel->SecretsBonus = true;
            if (CurrentLevel->MaxTotalMonsters == 0 && CurrentLevel->MaxTotalItems == 0 && CurrentLevel->MaxTotalSecrets == 0)
                CurrentLevel->AllBonus = true;
            if (CurrentLevel->Par == 0)
                CurrentLevel->ParBonus = true;
            CalculateBonusMaps();
            
            CurrentLevel->AdditionalMonsters = 0;
            CurrentLevel->Event = MAPEVENT_NONE;
            
            // Allocate the Monster Positions dynamic array
            ArrayCreate(&(CurrentLevel->MonsterPositions), "MPOS", 64, sizeof(Position));
            
            // Decide the map's event, if any
            DecideMapEvent(CurrentLevel);
            
            // [KS] If the player wants to warp back to the outpost on a new map, do so now
            bool AllowTransport = true;
            
            if (DisableEvent || Transported) AllowTransport = false;
            
            for (int i = 0; i < MAX_PLAYERS; i++)
            {
                if (!PlayerInGame(i)) continue;
                
                SetActivator(0, AAPTR_PLAYER1 << i);
                if (CheckInventory("DRPGDisallowTransport"))
                    AllowTransport = false;
                SetActivator(0, AAPTR_NULL);
            }
            
            if (GetCVar("drpg_transport_on_new_map") && AllowTransport && DefaultOutpost)
            {
                qsort(KnownLevels->Data, KnownLevels->Position, sizeof(LevelInfo), LevelSort);
                CurrentLevel = FindLevelInfo();
                CurrentLevel->NeedsRealInfo = false;
                
                Transported = true;
                TransporterLevel = CurrentLevel;
                ChangeLevel(DefaultOutpost->LumpName, 0, CHANGELEVEL_NOINTERMISSION, CurrentSkill);
                
                return;
            }
            
            // Tip
            for (int i = 0; i < MAX_PLAYERS; i++)
            {
                // Player is not in-game
                if (!PlayerInGame(i)) continue;
                
                // Player has the tips CVAR disabled
                if (!GetUserCVar(i, "drpg_tips")) continue;
                
                SetActivator(0, AAPTR_PLAYER1 << i);
                GiveTip();
                SetActivator(0, AAPTR_NULL);
            }
            
            CurrentLevel->NeedsRealInfo = false;
        }
        
        // We need to make sure the maps stay sorted whenever we add one
        qsort(KnownLevels->Data, KnownLevels->Position, sizeof(LevelInfo), LevelSort);
        Delay(2);
    }
    
    // We need to do this again because qsort invalidated all our pointers
    CurrentLevel = FindLevelInfo();
    
    // Transport will take us to the last base we were in.
    if (CurrentLevel->UACBase)
        DefaultOutpost = CurrentLevel;
    
    //WadSmoosh
    InitWadSmoosh();
    
    if (CurrentLevel->UACBase || CurrentLevel->UACArena)
        return; // [KS] These maps set themselves up, so nothing more to do.
    
    //Flag to run monster replacements
    WaitingForReplacements = true;

    // Reduce monster population based on difficulty settings
    ReduceMonsterCount();
    
    // Populate the positions array
    for (int i = 1; i < MonsterID; i++)
    {
        if (!Monsters[i].Init)
            continue;
        
        // Array has grown too big, resize it
        if (CurrentLevel->MonsterPositions.Position == CurrentLevel->MonsterPositions.Size)
        {
            ArrayResize(&CurrentLevel->MonsterPositions);
        }
        
        // Store position
        ((Position *)CurrentLevel->MonsterPositions.Data)[CurrentLevel->MonsterPositions.Position++] = Monsters[i].spawnPos;
        
        // Prevent running away on gigantic maps (see holyhell.wad MAP05)
        if ((i % 4096) == 0)
            Delay(1);
    }
    
    // Set up the currently in-effect map event
    SetupMapEvent();
    
    // Also give a tip for events if they're new to the player
    if (CurrentLevel->Event != MAPEVENT_NONE)
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            // Player is not in-game
            if (!PlayerInGame(i)) continue;
            
            // Player has the tips CVAR disabled
            if (!GetUserCVar(i, "drpg_tips")) continue;
            
            // Player has seen this event before
            if (Players(i).SeenEventTip[CurrentLevel->Event]) continue;
            
            SetActivator(Players(i).TID);
            GiveTip();
            SetActivator(0, AAPTR_NULL);
        }
    
    // Initialize the Dynamic Loot Generator
    // [KS] Call it here so that events can modify the loot it spawns
    if (GetCVar("drpg_loot_system") && !CurrentLevel->UACBase)
        DynamicLootGenerator("DRPGLuckDropper");
    
    AddAdditionalMonsters();
    
    // Setup missions which require generation
    SetupMapMissions();
    
    DisableEvent = false;
    WaitingForReplacements = false;
    
    // Hell Skill has some additional challenges
    if (GameSkill() >= 5)
    {
        AddMiniboss();
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (!PlayerInGame(i))
                continue;
            
            HellSkillTransport(i);
        }
    }
    
    if (CurrentLevel)
        MapLoop();
}

NamedScriptSync void ReduceMonsterCount()
{
    int TotalMonsters = GetLevelInfo(LEVELINFO_TOTAL_MONSTERS);
    
    if (TotalMonsters == 0)
        return;
    
    int DesiredMonsterCount = TotalMonsters;
    
    DesiredMonsterCount = DesiredMonsterCount * GetCVar("drpg_monster_population") / 100;
    if (GetCVar("drpg_monster_limit") > 0 && DesiredMonsterCount > GetCVar("drpg_monster_limit"))
        DesiredMonsterCount = GetCVar("drpg_monster_limit");
    
    if (TotalMonsters == DesiredMonsterCount)
        return;
    
    int MonstersToRemove = TotalMonsters - DesiredMonsterCount;
    
    if (MonstersToRemove < 0)
    {
        MonstersToRemove = -MonstersToRemove;
        if (GetCVar("drpg_debug"))
            Log("\CdDEBUG: \CcAdding \Cd%d\Cc more monsters!", MonstersToRemove);
        CurrentLevel->AdditionalMonsters = MonstersToRemove;
        return;
    }
    
    if (TotalMonsters <= MonstersToRemove)
        MonstersToRemove = TotalMonsters - 1;
    
    if (GetCVar("drpg_debug"))
        Log("\CdDEBUG: \CcReducing monster count to \Cd%d\Cc from \Cd%d", TotalMonsters - MonstersToRemove, TotalMonsters);
    
    int Iterations = 0;
    while (MonstersToRemove)
    {
        for (int i = Random(1, MonsterID - 1); i < MonsterID; i++)
        {
            if (!Monsters[i].Init || ClassifyActor(Monsters[i].TID) == ACTOR_NONE || !(CheckFlag(Monsters[i].TID, "COUNTKILL")))
                continue;
            
            Thing_Remove(Monsters[i].TID);
            Monsters[i].Init = false;
            Monsters[i].TID = 0;
            
            MonstersToRemove--;
            
            break;
        }
        
        Iterations++;
        
        if ((Iterations % 4096) == 0)
            Delay(1);
    }
}

NamedScript void SetupMapMissions()
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        // Player is currently not in-game
        if (!PlayerInGame(i)) continue;
        
        // Kill
        if (Players(i).Mission.Active && Players(i).Mission.Type == MT_KILL)
        {
            int Amount = Players(i).Mission.Amount - Players(i).Mission.Current;
            DynamicLootGenerator(GetMissionMonsterActor(Players(i).Mission.Monster->Actor), Amount);
        }
        
        // Assassination
        if (Players(i).Mission.Active && Players(i).Mission.Type == MT_ASSASSINATION)
            DynamicLootGenerator(GetMissionMonsterActor(Players(i).Mission.Monster->Actor), 1);
    }
}

NamedScript void MapLoop()
{
    long int XPBonus;
    long int RankBonus;
    
    int ItemsFound;
    int SecretsFound;
    
    int ItemsLastFound = 0;
    
    Delay(5); // Allow Map Events to change the monster/item counts
    
    Start:
    
    // Mission Trackers
    ItemsFound = GetLevelInfo(LEVELINFO_FOUND_ITEMS);
    SecretsFound = GetLevelInfo(LEVELINFO_FOUND_SECRETS);
    
    if (ItemsFound > ItemsLastFound)
    {
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            Players(i).Payout.ItemsFound++;

            if (!PlayerInGame(i) || Players(i).Mission.Type != MT_ITEMS)
                continue;
            
            Players(i).Mission.Current += ItemsFound - ItemsLastFound;
        }
    }
    
    // These should be unique-only, because they're not secrets anymore once
    // you know about them.
    if (SecretsFound > CurrentLevel->UniqueSecrets)
    {
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (!PlayerInGame(i))
                continue;
            
            FadeRange(255, 255, 0, 0.25, 255, 255, 0, 0, 1.0);
            
            RankBonus = (long int)(RankTable[Players(i).RankLevel]) / 100l;
            Players(i).Rank += RankBonus;

            Players(i).Payout.SecretsFound++;
            
            if (Players(i).Mission.Type != MT_SECRETS)
                continue;

            Players(i).Mission.Current += SecretsFound - CurrentLevel->UniqueSecrets;
        }
        
        CurrentLevel->UniqueSecrets = SecretsFound;
    }
    
    // Kills/Item/Secrets special bonuses
    if (CalcPercent(GetLevelInfo(LEVELINFO_KILLED_MONSTERS), GetLevelInfo(LEVELINFO_TOTAL_MONSTERS)) > CurrentLevel->MaxMonsterPercentage && CurrentLevel->Event != MAPEVENT_MEGABOSS && CurrentLevel->Event != MAPEVENT_DRLA_OVERMIND && CurrentLevel->Event != MAPEVENT_ONEMONSTER && CurrentLevel->Event != MAPEVENT_DRLA_FEEDINGFRENZY)
    {
        CurrentLevel->MaxMonstersKilled = GetLevelInfo(LEVELINFO_KILLED_MONSTERS);
        CurrentLevel->MaxTotalMonsters = GetLevelInfo(LEVELINFO_TOTAL_MONSTERS);
        CurrentLevel->MaxMonsterPercentage = CalcPercent(GetLevelInfo(LEVELINFO_KILLED_MONSTERS), GetLevelInfo(LEVELINFO_TOTAL_MONSTERS));
    }
    if (CalcPercent(GetLevelInfo(LEVELINFO_FOUND_ITEMS), GetLevelInfo(LEVELINFO_TOTAL_ITEMS)) > CurrentLevel->MaxItemPercentage)
    {
        CurrentLevel->MaxItemsFound = GetLevelInfo(LEVELINFO_FOUND_ITEMS);
        CurrentLevel->MaxTotalItems = GetLevelInfo(LEVELINFO_TOTAL_ITEMS);
        CurrentLevel->MaxItemPercentage = CalcPercent(GetLevelInfo(LEVELINFO_FOUND_ITEMS), GetLevelInfo(LEVELINFO_TOTAL_ITEMS));
    }
    if (CalcPercent(GetLevelInfo(LEVELINFO_FOUND_SECRETS), GetLevelInfo(LEVELINFO_TOTAL_SECRETS)) > CurrentLevel->MaxSecretPercentage)
    {
        CurrentLevel->MaxSecretsFound = GetLevelInfo(LEVELINFO_FOUND_SECRETS);
        CurrentLevel->MaxTotalSecrets = GetLevelInfo(LEVELINFO_TOTAL_SECRETS);
        CurrentLevel->MaxSecretPercentage = CalcPercent(GetLevelInfo(LEVELINFO_FOUND_SECRETS), GetLevelInfo(LEVELINFO_TOTAL_SECRETS));
    }
    
    if (CurrentLevel->MaxMonsterPercentage >= 100 && !CurrentLevel->KillBonus && CurrentLevel->Event != MAPEVENT_MEGABOSS && CurrentLevel->Event != MAPEVENT_DRLA_OVERMIND && CurrentLevel->Event != MAPEVENT_ONEMONSTER && CurrentLevel->Event != MAPEVENT_DRLA_FEEDINGFRENZY)
    {
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (!PlayerInGame(i))
                continue;
            
            SetActivator(Players(i).TID);
            
            FadeRange(255, 0, 0, 0.25, 255, 0, 0, 0, 1.0);
            
            XPBonus = XPTable[Players(i).Level] / 100l;
            Player.XP += XPBonus;
            
            HudMessage("Monsters Killed Bonus!\n%ld XP Bonus", XPBonus);
            EndHudMessage(HUDMSG_FADEOUT, 0, "Brick", 1.5, 0.4, 3.0, 3.0);
        }
        
        SetActivator(0, AAPTR_NULL);
        
        CurrentLevel->KillBonus = true;
    }

    if (CurrentLevel->MaxItemPercentage >= 100 && !CurrentLevel->ItemsBonus)
    {
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (!PlayerInGame(i))
                continue;
            
            SetActivator(Players(i).TID);
            
            FadeRange(0, 255, 255, 0.25, 0, 255, 255, 0, 1.0);
            
            HealThing(SHIELD_HEALTH);
            Players(i).EP = Players(i).EPMax;
            
            HudMessage("Items Found Bonus!\nFull HP/EP Restore");
            EndHudMessage(HUDMSG_FADEOUT, 0, "LightBlue", 1.5, 0.6, 3.0, 3.0);
        }
        
        SetActivator(0, AAPTR_NULL);
        
        CurrentLevel->ItemsBonus = true;
    }

    if (CurrentLevel->MaxSecretPercentage >= 100 && !CurrentLevel->SecretsBonus)
    {
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (!PlayerInGame(i))
                continue;
            
            SetActivator(Players(i).TID);
            
            FadeRange(255, 255, 0, 0.25, 255, 255, 0, 0, 1.0);
            
            RankBonus = RankTable[Players(i).RankLevel] / 20l;
            Players(i).Rank += RankBonus;

            HudMessage("Secrets Found Bonus!\n%ld Rank Bonus", RankBonus);
            EndHudMessage(HUDMSG_FADEOUT, 0, "Yellow", 1.5, 0.8, 3.0, 3.0);
        }
        
        SetActivator(0, AAPTR_NULL);
        
        CurrentLevel->SecretsBonus = true;
    }
    
    if (CurrentLevel->KillBonus && CurrentLevel->ItemsBonus && CurrentLevel->SecretsBonus && !CurrentLevel->AllBonus)
    {
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (!PlayerInGame(i))
                continue;
            
            SetActivator(Players(i).TID);
            
            FadeRange(255, 255, 255, 0.25, 255, 255, 255, 0, 1.0);
            
            XPBonus = XPTable[Players(i).Level] / 100l;
            RankBonus = RankTable[Players(i).RankLevel] / 20l;
            
            Players(i).XP += XPBonus;
            Players(i).Rank += RankBonus;
            
            HealThing(SHIELD_HEALTH);
            Players(i).EP = Players(i).EPMax;
            
            HudMessage("\CaMonsters Killed Bonus!\n\CnItems Found Bonus!\n\CkSecrets Found Bonus!\n\n\Cj%ld XP Bonus\n\Ck%ld Rank Bonus\n\CnFull HP/EP Restore",
                       XPBonus, RankBonus);
            EndHudMessage(HUDMSG_FADEOUT, 0, "White", 0.5, 0.2, 5.0, 5.0);
        }
        
        SetActivator(0, AAPTR_NULL);
        
        CurrentLevel->AllBonus = true;
        CalculateBonusMaps();
    }
    
    if (CalcPercent(GetLevelInfo(LEVELINFO_KILLED_MONSTERS), GetLevelInfo(LEVELINFO_TOTAL_MONSTERS)) >= 100 && (CurrentLevel->Event == MAPEVENT_ALLAURAS || CurrentLevel->Event == MAPEVENT_HARMONIZEDAURAS) && !CurrentLevel->EventCompleted)
    {
        // All Auras and Harmonized Destruction events are ended by killing everything on the map
        if (Timer() > 4)
        {
            AmbientSound("misc/secret", 127);
            SetFont("BIGFONT");
            HudMessage("Everything falls silent.");
            EndHudMessageBold(HUDMSG_FADEOUT, 0, "LightBlue", 0.5, 0.7, 5.0, 5.0);
            SetFont("SMALLFONT");
            MapEventReward();
        }
        
        CurrentLevel->EventCompleted = true;
    }
    
    Delay(1);
    
    ItemsLastFound = ItemsFound;
    
    goto Start;
}

NamedScript void CalculateBonusMaps()
{
    int Count = 0;
    
    for (int i = 0; i < KnownLevels->Position; i++)
        if (((LevelInfo *)KnownLevels->Data)[i].AllBonus)
            Count++;
    
    AllBonusMaps = Count;
}

int LevelSort(void const *Left, void const *Right)
{
    LevelInfo LeftLevel = *((LevelInfo *)Left);
    LevelInfo RightLevel = *((LevelInfo *)Right);
    
    // Sort order: Outpost, Arena, Normal Map, Secret Map
    
    int LeftScore = LeftLevel.LevelNum + (LeftLevel.SecretMap ? 1000 : 0) + (LeftLevel.UACBase ? -2000 : 0) + (LeftLevel.UACArena ? -1000 : 0);
    int RightScore = RightLevel.LevelNum + (RightLevel.SecretMap ? 1000 : 0) + (RightLevel.UACBase ? -2000 : 0) + (RightLevel.UACArena ? -1000 : 0);
    
    if (LeftScore < RightScore) return -1;
    if (LeftScore > RightScore) return 1;
    
    return 0;
}

void AddAdditionalMonsters()
{
    if (CurrentLevel->AdditionalMonsters < 1 || CurrentLevel->Event == MAPEVENT_MEGABOSS)
        return;
    
    if (CurrentLevel->Event == MAPEVENT_ONEMONSTER)
        DynamicLootGenerator(GetMissionMonsterActor(CurrentLevel->SelectedMonster->Actor), CurrentLevel->AdditionalMonsters);
    else
        DynamicLootGenerator("DRPGGenericMonsterDropper", CurrentLevel->AdditionalMonsters);
}

LevelInfo *FindLevelInfo(str MapName)
{
    if (MapName == NULL)
        MapName = StrParam("%tS", PRINTNAME_LEVEL);
    
    for (int i = 0; i < KnownLevels->Position; i++)
        if (!StrICmp(((LevelInfo *)KnownLevels->Data)[i].LumpName, MapName))
            return &((LevelInfo *)KnownLevels->Data)[i];
    
    return NULL;
}

int FindLevelInfoIndex(str MapName)
{
    if (MapName == NULL)
        MapName = StrParam("%tS", PRINTNAME_LEVEL);
    
    for (int i = 0; i < KnownLevels->Position; i++)
        if (!StrICmp(((LevelInfo *)KnownLevels->Data)[i].LumpName, MapName))
            return i;
    
    return 0; // Default to the Outpost because we don't actually know where we are
}

NamedScript MapSpecial void AddUnknownMap(str Name, str DisplayName, int LevelNumber, int Secret)
{
    while (KnownLevels->Data == NULL)
        Delay(1);
    
    if (WadSmoosh)
        return; // this messes up WadSmoosh too much, running every time we go to the outpost
    
    if (FindLevelInfo(Name) != NULL)
        return; // This map was already unlocked, so ignore it
    
    LevelInfo *NewMap = &((LevelInfo *)KnownLevels->Data)[KnownLevels->Position++];
    NewMap->LumpName = Name;
    NewMap->NiceName = DisplayName;
    NewMap->LevelNum = LevelNumber;
    NewMap->SecretMap = Secret;
    NewMap->UACBase = false;
    NewMap->UACArena = false;
    NewMap->SecretMap = false;
    NewMap->Completed = false;
    NewMap->NeedsRealInfo = true;
}

// Level exit script
// Can't use NamedExecute in xlat, lame
NumberedScript(MAP_EXIT_SCRIPTNUM) MapSpecial void MapExit(bool Secret)
{
    int ParTime = GetLevelInfo(LEVELINFO_PAR_TIME);
    bool Waiting = true;
    
    // Megabosses prevent you from leaving until they are killed
    // Hell Unleashed prevents you from leaving until you have opened the box
    if ((CurrentLevel->Event == MAPEVENT_MEGABOSS && !CurrentLevel->EventCompleted) || (CurrentLevel->Event == MAPEVENT_DRLA_OVERMIND && !CurrentLevel->EventCompleted) || (CurrentLevel->Event == MAPEVENT_HELLUNLEASHED && !CurrentLevel->HellUnleashedActive))
    {
        AmbientSound("mission/gottarget2", 127);
        
        SetHudSize(640, 480, false);
        SetFont("BIGFONT");
        HudMessage("\CgA mysterious force prevents you from leaving!");
        EndHudMessageBold(HUDMSG_TYPEON | HUDMSG_LOG, 0, "Red", 320.4, 160.0, 3.0, 0.03, 0.5);
        SetHudSize(0, 0, false);
        SetFont("SMALLFONT");
        
        // In Hell Unleashed, spawn waves of enemies to spite the player for trying to leave without opening the box
        if (CurrentLevel->Event == MAPEVENT_HELLUNLEASHED && !CurrentLevel->HellUnleashedActive)
            while (!CurrentLevel->HellUnleashedActive)
            {
                HellUnleashedSpawnMonsters();
                Delay(35 * 30);
            }
        
        // Wait until the event is over
        while ((CurrentLevel->Event == MAPEVENT_MEGABOSS && !CurrentLevel->EventCompleted) || (CurrentLevel->Event == MAPEVENT_DRLA_OVERMIND && !CurrentLevel->EventCompleted) || (CurrentLevel->Event == MAPEVENT_HELLUNLEASHED && !CurrentLevel->HellUnleashedActive))
            Delay(35 * 5);
    }
    
    // Environmental Hazard triggers a security lockdown until the hazard level is cleared
    if (CurrentLevel->Event == MAPEVENT_TOXICHAZARD && !CurrentLevel->EventCompleted)
    {
        AmbientSound("transfer/fail", 127);
        
        SetHudSize(640, 480, false);
        SetFont("BIGFONT");
        HudMessage("Level \Cg%d\C- Security Lockdown is currently in effect.\nCannot transfer until \Cjenvironmental hazard\C- is \Cjclear\C-.", CurrentLevel->HazardLevel);
        EndHudMessageBold(HUDMSG_TYPEON | HUDMSG_LOG, 0, "Green", 320.4, 160.0, 3.0, 0.03, 0.5);
        SetHudSize(0, 0, false);
        SetFont("SMALLFONT");
        
        while (CurrentLevel->Event == MAPEVENT_TOXICHAZARD && !CurrentLevel->EventCompleted)
            Delay(35 * 5);
    }
    
    // How long it took to reach the exit
    int ExitTime = Timer() / 35;
    if (CurrentLevel->ShortestTime > ExitTime)
        CurrentLevel->ShortestTime = ExitTime;
    
    // Prevent level exit until everyone is outside the menu
    if (InMultiplayer && PlayerCount() > 1)
        while (Waiting)
        {
            Waiting = false;
            
            for (int i = 0; i < MAX_PLAYERS; i++)
                if (Players(i).InMenu || Players(i).InShop || Players(i).OutpostMenu > 0)
                    Waiting = true;
            
            Delay(35 * 3);
        }
    
    // Check par time and give bonus if you beat it
    if (ParTime > 0 && ExitTime < ParTime && (CurrentLevel && !CurrentLevel->ParBonus))
    {
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (!PlayerInGame(i)) continue;
            
            SetActivator(Players(i).TID);
            long int RankBonus = RankTable[Players(i).RankLevel] / 20;
            
            SetFont("SMALLFONT");
            FadeRange(255, 255, 0, 0.25, 255, 255, 0, 0.0, 1.0);
            HudMessage("Par Time Beaten!\n%ld Rank Bonus", RankBonus);
            EndHudMessage(HUDMSG_FADEOUT, 0, "Gold", 1.5, 0.5, 3.0, 2.0);
            
            Players(i).Rank += RankBonus;
            Players(i).Payout.ParTimesBeaten++;
        }
        
        AmbientSound("misc/parbonus", 127);
        
        if (CurrentLevel)
            CurrentLevel->ParBonus = true;
        
        Delay(35 * 5);
    }
    
    if (CurrentLevel->Event == MAPEVENT_TELEPORTCRACKS || CurrentLevel->Event == MAPEVENT_DOOMSDAY || CurrentLevel->Event == MAPEVENT_DRLA_FEEDINGFRENZY || CurrentLevel->Event == MAPEVENT_SKILL_HELL || CurrentLevel->Event == MAPEVENT_SKILL_ARMAGEDDON)
        CurrentLevel->EventCompleted = true; // These don't actually end until you leave the map normally
    
    CurrentLevel->Completed = true; // We finished the map
    
    // Payout
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!PlayerInGame(i)) continue;
        
        Players(i).Payout.MapsCompleted++;
    }
    
    UsedSecretExit = Secret;
    PreviousLevel = CurrentLevel;
    PreviousLevelNum = CurrentLevel->LevelNum;
    if (!CurrentLevel->SecretMap)
        PreviousPrimaryLevelNum = CurrentLevel->LevelNum;

    // Now exit the level
    if (Secret)
        Exit_Secret(0);
    else
        Exit_Normal(0);
}

// Hell Skill modifiers -------------------------------------------------------

// Non-mission version of Assassination
NamedScript void AddMiniboss()
{
    if (MonsterID <= 1) return; // No monsters, no miniboss.
    
    int Chosen = Random(1, MonsterID - 1);
    int LevelMod = (4 + (GameSkill() - 5)) * Player.Level + Random(0, 100);
    
    while (!Monsters[Chosen].Init)
        Chosen = Random(1, MonsterID - 1);
    
    Monsters[Chosen].Level += LevelMod;
    Monsters[Chosen].Level /= 4;
    Monsters[Chosen].NeedReinit = true;
    
    // Shadow Aura
    for (int i = 0; i < AURA_MAX; i++)
        Monsters[Chosen].Aura.Type[i].Active = true;
    
    // EVIL LAUGH OF WARNING
    if (Monsters[Chosen].Threat >= 10)
        LocalAmbientSound("mission/gottarget2", 127);
}

// Non-mission version of Reinforcements
NamedScript void HellSkillTransport(int player)
{
    SetActivator(Players(player).TID);
    
    bool DRLA = (CompatMode == COMPAT_DRLA);
    int MonsterDataAmount;
    MonsterInfoPtr MonsterList[MAX_TEMP_MONSTERS];
    int MonsterListLength;
    int BossesSpawned = 0;
    
    Delay(35 * 60); // Grace Period
    
    // Build a list of monsters
	if (ColourfulHell)
        MonsterDataAmount = MAX_DEF_MONSTERS_CH;
    else if (DRLA)
        MonsterDataAmount = MAX_DEF_MONSTERS_DRLA;
    else
        MonsterDataAmount = MAX_DEF_MONSTERS;
    
    for (int i = 0; i < MonsterDataAmount && MonsterListLength < MAX_TEMP_MONSTERS; i++)
    {
        MonsterInfoPtr TempMonster;
		if (ColourfulHell)
            TempMonster = &MonsterDataCH[i];
        else if (DRLA)
            TempMonster = &MonsterDataDRLA[i];
        else if (CompatMode == COMPAT_LEGENDOOM)
            TempMonster = &MonsterDataLD[i];
        else
            TempMonster = &MonsterData[i];
    
        if (TempMonster->Difficulty <= 2 + AveragePlayerLevel() * 2)
            MonsterList[MonsterListLength++] = TempMonster;
    }
    
    //Log("%d monsters", MonsterListLength);

    fixed X, Y, Z;
    int MonsterIndex;
    fixed SpawnX;
    fixed SpawnY;
    int TID;
    bool Success;
    int SpawnTries;
    int CurrentRadius;
    
    while (GetLevelInfo(LEVELINFO_KILLED_MONSTERS) < GetLevelInfo(LEVELINFO_TOTAL_MONSTERS))
    {
        X = GetActorX(0);
        Y = GetActorY(0);
        Z = GetActorZ(0);
        MonsterIndex = 0;
        SpawnX = 0.0;
        SpawnY = 0.0;
        TID = 0;
        Success = false;
        SpawnTries = 0;
        CurrentRadius = 0;
        
        // Stop spawning if time is frozen
        while (IsTimeFrozen()) Delay(1);
        
        TID = UniqueTID();
        Success = false;
        SpawnTries = 0;
        CurrentRadius = 1024;
        
        while (!Success && SpawnTries < 3)
        {
            MonsterIndex = Random(0, MonsterListLength - 1);
            
            SpawnX = RandomFixed(-(fixed)CurrentRadius, (fixed)CurrentRadius);
            SpawnY = RandomFixed(-(fixed)CurrentRadius, (fixed)CurrentRadius);
            
            // Get the floor Z position at this spot
            SpawnForced("MapSpot", X + SpawnX, Y + SpawnY, Z, TID, 0);
            Z = GetActorFloorZ(TID);
            Thing_Remove(TID);
            
            Success = Spawn(GetMissionMonsterActor(MonsterList[MonsterIndex]->Actor), X + SpawnX, Y + SpawnY, Z, TID, 0);
            
            bool IsBoss = CheckFlag(TID, "BOSS");
            
            if (Success)
                Success = CheckSight(0, TID, 0);
            if (Success)
                Success = !IsBoss || (!Random (0, 3) && BossesSpawned < 3);
            
            if (!Success)
            {
                // Try again, closer to the player each time, up to 3 times, before giving up.
                Thing_Remove(TID);
                CurrentRadius /= 2;
            }
            else
            {
                if (IsBoss)
                    BossesSpawned++;
            }
            
            SpawnTries++;
        }
        
        if (Success)
        {
            //Log("Spawned %S", MonsterList[MonsterIndex]->Name);
            Thing_Hate(TID, Player.TID);
            Thing_ChangeTID(TID, 0); // Get rid of the ID
            Spawn("TeleportFog", X + SpawnX, Y + SpawnY, Z, 0, 0);
        }
        
        Delay(35 * 30);
    }
}

// Map Events -----------------------------------------------------------------

bool CheckMapEvent(int Event, LevelInfo *TargetLevel)
{
    switch (Event)
    {
        case MAPEVENT_SPECIAL_SINSTORM:
            return false;
        
        case MAPEVENT_MEGABOSS:
            if (AveragePlayerLevel() < 50)
                return false;
            return true;
        
        case MAPEVENT_ALLAURAS:
            if (AveragePlayerLevel() < 20)
                return false;
            return true;
        
        case MAPEVENT_HARMONIZEDAURAS:
            if (AveragePlayerLevel() < 10)
                return false;
            return true;
        
        case MAPEVENT_DOOMSDAY:
            if (AveragePlayerLevel() < 20 || Random(0, 3) || TargetLevel->Completed)
                return false;
            return true;
        
        case MAPEVENT_DRLA_FEEDINGFRENZY:
            if (CompatMode != COMPAT_DRLA || AveragePlayerLevel() < 20 || ColourfulHell)
                return false;
            return true;
        
        case MAPEVENT_DRLA_OVERMIND:
            if (CompatMode != COMPAT_DRLA || AveragePlayerLevel() < 40 || ColourfulHell)
                return false;
            return true;
        
        case MAPEVENT_BONUS_RAINBOWS:
            if (Random(0, 15) || !GetCVar("drpg_bonus_events"))
                return false;
            return true;
        
        case MAPEVENT_HELLUNLEASHED:
            if (AveragePlayerLevel() < 20)
                return false;
            return true;
        
        case MAPEVENT_SKILL_HELL:
            if (AveragePlayerLevel() < 10)
                return false;
            if (CurrentSkill >= 4)
                return false;
            return true;
        case MAPEVENT_SKILL_ARMAGEDDON:
            if (AveragePlayerLevel() < 10)
                return false;
            if (CurrentSkill >= 5)
                return false;
            if (CompatMode != COMPAT_DRLA || AveragePlayerLevel() < 10 || ColourfulHell)
                return false;
            return true;
        
        case MAPEVENT_TOXICHAZARD:
        case MAPEVENT_NUCLEARBOMB:
        case MAPEVENT_LOWPOWER:
        case MAPEVENT_ONEMONSTER:
        case MAPEVENT_TELEPORTCRACKS:
        case MAPEVENT_ACIDRAIN:
        case MAPEVENT_DARKZONE:
        default:
            return true;
    }
    
    return false;
}

void MapEventReward()
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        // Player is not in-game
        if (!PlayerInGame(i)) continue;
        
        SetActivator(Players(i).TID);
        
        if (Player.Level > 0)
        {
            str Message = "Event Cleared";
            
            switch (CurrentLevel->Event)
            {
            case MAPEVENT_TOXICHAZARD:
                Message = "Radiation Cleared";
                break;
            case MAPEVENT_NUCLEARBOMB:
                Message = "Bomb Disarmed";
                break;
            case MAPEVENT_LOWPOWER:
                Message = "Backup Power Restored";
                break;
            case MAPEVENT_ALLAURAS:
            case MAPEVENT_ONEMONSTER:
            case MAPEVENT_HARMONIZEDAURAS:
                Message = "Area Secured";
                break;
            }
            
            ActivatorSound("mission/complete", 127);
            PrintMessage(Message);
            Player.Payout.EventsCompleted++;
        }
    }
}

NamedScript void DecideMapEvent(LevelInfo *TargetLevel, bool FakeIt)
{
    if (GetCVar("drpg_debug"))
        Log("\CdDEBUG: \ChDeciding event for \Cd%S", TargetLevel->LumpName);

    str const EventNames[MAPEVENT_MAX] =
    {
        "None",
        
        "MegaBoss",
        "Environmental Hazard",
        "Thermonuclear Bomb",
        "Low Power",
        "All Auras",
        "One-Monster",
        "Hell Unleashed",
        "Harmonized Destruction",
        "Cracks in the Veil",
        "12 Hours 'til Doomsday",
        "The Dark Zone",
        
        "Feeding Frenzy",
        "Whispers of Darkness",
        
        "RAINBOWS!",
        
        "Skills - Hell",
        "Skills - Armageddon"
    };
    
    if (TargetLevel->UACBase)
        // [KS] One day, there will be invasions. Someday.
        return;
    
    // We need to clear all of the event data before choosing one
    
    TargetLevel->EventCompleted = false;
    
    // Megaboss
    TargetLevel->MegabossActor = NULL;
    
    // Environmental Hazard
    TargetLevel->HazardLevel = 0;
    TargetLevel->RadLeft = 0;
    TargetLevel->GeneratorFuel = 0;
    
    // Thermonuclear Bomb
    TargetLevel->BombTime = 0;
    TargetLevel->BombExplode = false;
    TargetLevel->BombAnnouncing = false;
    for (int i = 0; i < MAX_NUKE_KEYS; i++)
    {
        TargetLevel->BombKeyActive[i] = false;
        TargetLevel->BombKeyDisarming[i] = false;
        TargetLevel->BombKeyTimer[i] = 0;
    }
    
    // Low Power
    TargetLevel->PowerGeneratorActive = false;
    
    // One-Monster
    TargetLevel->SelectedMonster = NULL;
    
    // Hell Unleashed
    TargetLevel->HellUnleashedActive = false;
    TargetLevel->PandoraBoxTID = 0;
    TargetLevel->LevelAdd = 0;
    TargetLevel->RareAdd = 0;
    
    // Harmonized Destruction
    TargetLevel->AuraType = 0;
    
    // [KS] Super-special events for super-special levels (and by "special" I of course mean retarded)
    // [KS] PS: I hate you already.
    if (!StrICmp(TargetLevel->LumpName, "E1M8"))
    {
        // Phobos Anomaly
        // Tags: 666 (Floor_LowerToLowest)
        return;
    }
    else if (!StrICmp(TargetLevel->LumpName, "E2M8"))
    {
        // Tower of Babel
        // Tags: None, level ends
        return;
    }
    else if (!StrICmp(TargetLevel->LumpName, "E3M8"))
    {
        // Dis
        // Tags: None, level ends
        return;
    }
    else if (!StrICmp(TargetLevel->LumpName, "E4M6"))
    {
        // Against Thee Wickedly
        // Tags: 666 (Door_Open)
        return;
    }
    else if (!StrICmp(TargetLevel->LumpName, "E4M8"))
    {
        // Unto the Cruel
        // Tags: 666 (Floor_LowerToLowest)
        return;
    }
    else if (!StrICmp(TargetLevel->LumpName, "MAP07"))
    {
        // Dead Simple
        // Tags: 666 (Floor_LowerToLowest), 667 (Floor_RaiseByTexture)
        return;
    }
    else if (!StrICmp(TargetLevel->LumpName, "LEVEL07"))
    {
        // Baron's Banquet
        // Tags: 666 (Floor_LowerToLowest), 667 (Floor_RaiseByTexture)
        return;
    }
    else if (!StrICmp(TargetLevel->LumpName, "MAP30"))
    {
        // Icon of Sin
        // Blurb about a demon spitter and the game ending finale here.
        
        TargetLevel->Event = MAPEVENT_SPECIAL_SINSTORM;
        return;
    }
    
    if (!FakeIt)
    {
        // Don't bother creating an event if the level has no monsters
        if (GetLevelInfo(LEVELINFO_TOTAL_MONSTERS) == 0)
        {
            TargetLevel->Event = MAPEVENT_NONE;
            return;
        }
        
        if (RandomFixed(0.0, 99.9) > GetCVarFixed("drpg_mapevent_chance") || DisableEvent)
        {
            TargetLevel->Event = MAPEVENT_NONE;
            return; // No special event
        }
        
        int Rerolls = 256; // Just in case *no* events are possible for some reason
        while (TargetLevel->Event == MAPEVENT_NONE && Rerolls)
        {
            TargetLevel->Event = Random(MAPEVENT_NONE + 1, MAPEVENT_MAX - 1);
            
            if (GetCVar("drpg_debug"))
                Log("\CdDEBUG: Trying \Cj%S", EventNames[TargetLevel->Event]);
            
            if (!CheckMapEvent(TargetLevel->Event, TargetLevel))
            {
                if (GetCVar("drpg_debug"))
                    Log("\CdDEBUG: \Cj%S\Cg not possible", EventNames[TargetLevel->Event]);
                TargetLevel->Event = MAPEVENT_NONE;
            }
            
            Rerolls--;
        }
        
        if (TargetLevel->Event == MAPEVENT_NONE)
            return;
    }
    
    if (GetCVar("drpg_debug") && TargetLevel->Event != MAPEVENT_NONE)
        Log("\CdDEBUG: Special Event on \Cc%S\Cd: \Cg%S", TargetLevel->NiceName, EventNames[TargetLevel->Event]);
    
    // Initialize some basic info for the chosen event
    
    if (TargetLevel->Event == MAPEVENT_MEGABOSS)
    {
		if (ColourfulHell)
			TargetLevel->MegabossActor = &MegaBossesCH[Random(0, MAX_MEGABOSSES_CH - 1)];
		else
			TargetLevel->MegabossActor = &MegaBosses[Random(0, MAX_MEGABOSSES - 1)];
        
        if (GetCVar("drpg_debug"))
            Log("\CdDEBUG: Chosen Boss: \Cg%S", TargetLevel->MegabossActor->Actor);
    }
    else if (TargetLevel->Event == MAPEVENT_TOXICHAZARD)
    {
        TargetLevel->HazardLevel = Random(1, 5);
        TargetLevel->RadLeft = 100;
    }
    else if (TargetLevel->Event == MAPEVENT_ONEMONSTER)
    {
        bool DRLA = (CompatMode == COMPAT_DRLA);
        
        int Amount;
        MonsterInfoPtr PotentialMonsters[MAX_TEMP_MONSTERS];
        int NumPotentialMonsters;
        int MonsterDataAmount;
        fixed MonsterLevelDivisor;
        
		if (ColourfulHell)
		{
			MonsterDataAmount = MAX_DEF_MONSTERS_CH;
            MonsterLevelDivisor = 2.5;
		}
        else if (DRLA)
        {
            MonsterDataAmount = MAX_DEF_MONSTERS_DRLA;
            MonsterLevelDivisor = 2.5;
        }
        else
        {
            MonsterDataAmount = MAX_DEF_MONSTERS;
            MonsterLevelDivisor = 1.25;
        }
        
        // Generate a list based on monsters' threat levels.
        for (int i = 0; i < MonsterDataAmount; i++)
        {
            MonsterInfoPtr TempMonster;
            int RequiredLevel;
            int AverageLevel = AveragePlayerLevel();
            
			if (ColourfulHell)
				TempMonster = &MonsterDataCH[i];
            else if (DRLA)
                TempMonster = &MonsterDataDRLA[i];
            else if (CompatMode == COMPAT_LEGENDOOM)
                TempMonster = &MonsterDataLD[i];
            else
                TempMonster = &MonsterData[i];
            
            RequiredLevel = (fixed)(TempMonster->Difficulty + ((TempMonster->ThreatLevel) * 20)) / MonsterLevelDivisor;
            RequiredLevel = Clamp(0, RequiredLevel, 80);
            if (GetCVar("drpg_debug"))
                Log("\CdDEBUG: \CcPotential monster: \Cg%S \Cc/ Needed level: \Cg%d", TempMonster->Name, RequiredLevel);
            
            if (AverageLevel >= RequiredLevel && TempMonster->ThreatLevel <= 5)
                PotentialMonsters[NumPotentialMonsters++] = TempMonster;
        }
        
        for (int i = 0; i < NumPotentialMonsters; i++)
        
        TargetLevel->SelectedMonster = PotentialMonsters[Random(0, NumPotentialMonsters - 1)];
        
        if (GetCVar("drpg_debug"))
            Log("\CdDEBUG: Chosen Monster: \Cg%S", TargetLevel->SelectedMonster->Name);
    }
    else if (TargetLevel->Event == MAPEVENT_HARMONIZEDAURAS)
    {
        TargetLevel->AuraType = Random(0, AURA_MAX);
    }
}

NamedScript Type_OPEN void PassingEvents()
{
    while (true)
    {
        if (GetCVar("drpg_mapevent_eventtime") == 0)
            return;
        
        Delay(1);
        PassingEventTimer--;
        
        if (PassingEventTimer <= 0)
        {
            if (GetCVar("drpg_debug"))
                Log("\CdDEBUG: \CfRe-rolling events for all inactive levels");
            
            for (int i = 0; i < KnownLevels->Position; i++)
            {
                LevelInfo *ThisLevel = &((LevelInfo *)KnownLevels->Data)[i];
                
                if (!ThisLevel->Completed)
                {
                    if (GetCVar("drpg_debug"))
                        Log("\CdDEBUG: \CgIncomplete: %S", ThisLevel->LumpName);
                    continue;
                }
                
                if (CurrentLevel && ThisLevel == CurrentLevel)
                {
                    if (GetCVar("drpg_debug"))
                        Log("\CdDEBUG: \CfCurrent: %S", ThisLevel->LumpName);
                    continue;
                }
                
                if (GetCVar("drpg_debug"))
                    Log("\CdDEBUG: \CqInactive: %S", ThisLevel->LumpName);
                
                DecideMapEvent(ThisLevel);
            }
            
            PassingEventTimer = GetCVar ("drpg_mapevent_eventtime") * 35 * 60;
        }
    }
}

NamedScript void SetupMapEvent()
{
    switch (CurrentLevel->Event)
    {
        // Normal Events
        // --------------------------------------------------
        
        case MAPEVENT_MEGABOSS:
            // Megaboss: One incredibly powerful super-monster spawns, nothing else in level.
            MegaBossEvent();
            break;
        
        case MAPEVENT_TOXICHAZARD:
            // Environmental Hazard: Entire map is filled with damaging radiation. Monsters can drop extra Radiation Suits when killed.
            EnvironmentalHazard();
            break;
        
        case MAPEVENT_NUCLEARBOMB:
            // Thermonuclear Bomb: A Thermonuclear Bomb spawns next to you on map start. You have PAR * 2 to escape before it explodes.
            ThermonuclearBombEvent();
            break;
        
        case MAPEVENT_LOWPOWER:
            // Low Power: Light levels are extremely diminished. IR goggles recommended.
            LowPowerEvent();
            break;
        
        case MAPEVENT_ALLAURAS:
            // All Auras: Everything spawns with evil, evil auras.
            SetMusic("AllAuras");
            SetHudSize(640, 480, false);
            SetFont("BIGFONT");
            HudMessage("The air crackles with the darkest of magics!");
            EndHudMessageBold(HUDMSG_FADEOUT, 0, "DarkRed", 320.0, 150.0, 1.0, 19.0);
            SetHudSize(0, 0, false);
            break;
        
        case MAPEVENT_ONEMONSTER:
            // One-Monster: All monsters are of a single type.
            OneMonsterEvent();
            break;
        
        case MAPEVENT_HELLUNLEASHED:
            // Hell Unleashed: Monsters will continue to spawn in over time
            // Monster levels and rare drop rates will steadily increase while you stay in the level
            HellUnleashedEvent();
            break;
        
        case MAPEVENT_HARMONIZEDAURAS:
            // Harmonized Destruction: Everything spawns with the same aura.
            HarmonizedDestructionEvent();
            break;
        
        case MAPEVENT_TELEPORTCRACKS:
            // Cracks in the Veil: Portals litter the level, slowly ripping it apart spatially.
            TeleportCracksEvent();
            break;
        
        case MAPEVENT_DOOMSDAY:
            // 12 Hours 'til Doomsday: You have PAR * 5 until doomsday.
            DoomsdayEvent();
            break;
        
        case MAPEVENT_ACIDRAIN:
            // Vicious Downpour: Outdoor areas rain poison.
            ViciousDownpourEvent();
            break;
        
        case MAPEVENT_DARKZONE:
            DarkZoneEvent();
            break;
        
        // DRLA Events
        // --------------------------------------------------
        
        case MAPEVENT_DRLA_FEEDINGFRENZY:
            // T     H     E    Y      4   R    E      C  0   M  I    N   G
            //     %   !  &        F    3   E  D       F   1   E   5   H
            FeedingFrenzyEvent();
            break;
        
        case MAPEVENT_DRLA_OVERMIND:
            // Whispers of Darkness: The Overmind is watching. The Overmind is close. One of us. One of us. One of us.
            WhispersofDarknessEvent();
            break;
        
        // Bonus Events
        // --------------------------------------------------
        
        case MAPEVENT_BONUS_RAINBOWS:
            // RAINBOWS: ALL OF THE COLORS!
            RainbowEvent();
            break;
        
        // Skill Events
        // --------------------------------------------------
        
        case MAPEVENT_SKILL_HELL:
            if (GameSkill() != 5)
                ChangeLevel(CurrentLevel->LumpName, 0, CHANGELEVEL_NOINTERMISSION, 4);
            SetMusic("Skill5");
            SetHudSize(640, 480, false);
            SetFont("BIGFONT");
            HudMessage("It smells of burnt flesh and rotting corpses. It is likely you could be joining them soon.");
            EndHudMessageBold(HUDMSG_FADEOUT, 0, "Brick", 320.4, 150.0, 1.0, 19.0);
            SetHudSize(0, 0, false);
            break;
        
        case MAPEVENT_SKILL_ARMAGEDDON:
            if (GameSkill() != 6)
                ChangeLevel(CurrentLevel->LumpName, 0, CHANGELEVEL_NOINTERMISSION, 5);
            SetMusic("Skill6");
            SetHudSize(640, 480, false);
            SetFont("BIGFONT");
            HudMessage("A foul misfortune sweeps the land, turning up the darkest creatures. There is no God now.");
            EndHudMessageBold(HUDMSG_FADEOUT, 0, "Gray", 320.4, 150.0, 1.0, 19.0);
            SetHudSize(0, 0, false);
            break;
        
        // Map-Specific Special Events
        // --------------------------------------------------
        
        case MAPEVENT_SPECIAL_SINSTORM:
            // Sinstorm: MAP30 event - More Shadows than usual, plus occasional teleport rifts that open up and spill out monsters
            SinstormEvent();
            break;
        
        // Standard Level
        // --------------------------------------------------
        
        default:
            break;
    }
}

NamedScript void SetupOutpostEvent()
{
    // TODO: Outpost Invasions, Power Fluctuations, etc
    return;
}

NamedScript Type_UNLOADING void ResetMapEvent()
{
    // Remove the active event if it's been completed
    if (CurrentLevel && CurrentLevel->Event && CurrentLevel->EventCompleted)
    {
        // And reset the skill for these
        if (CurrentLevel->Event == MAPEVENT_SKILL_HELL || CurrentLevel->Event == MAPEVENT_SKILL_ARMAGEDDON)
            ChangeSkill(CurrentSkill);
        
        CurrentLevel->Event = MAPEVENT_NONE;
        CurrentLevel->EventCompleted = false;
    }
}

bool SpawnEventActor(str Actor, int TID)
{
    fixed Angle = GetActorAngle(Players(0).TID) + 0.5;
    fixed X = GetActorX(Players(0).TID) + Cos(Angle) * 64.0;
    fixed Y = GetActorY(Players(0).TID) + Sin(Angle) * 64.0;
    fixed Z = GetActorZ(Players(0).TID);
    bool Spawned = Spawn(Actor, X, Y, Z, TID, 0);
    
    if (!CheckSight(TID, Players(0).TID, 0) || !Spawned)
    {
        Thing_Remove(TID);
        return false;
    }

    return true;
}

NamedScript Console void SetMapEvent(int Level, int ID)
{
    LevelInfo *MapToChange = NULL;
    
    if (Level == 0)
        MapToChange = CurrentLevel;
    else if (Level < 0 || Level >= KnownLevels->Position)
        MapToChange = NULL;
    else
        MapToChange = &((LevelInfo *)KnownLevels->Data)[Level];
    
    if (MapToChange == NULL)
    {
        Log("\CgYou attempt black voodoo magic and fail.");
        
        if (Random(1, 6) == 1) // Roll for critical failure
            DamageThing(1000000);
        
        return;
    }
    
    MapToChange->Event = ID;
    
    DecideMapEvent(MapToChange, true);
    
    if (CurrentLevel == MapToChange)
        Log("\CjYou will need to warp back to this level to see changes take effect.");
}

// Event - Megaboss -----------------------------------------------------------

NamedScript void MegaBossEvent()
{
    bool Spawned;
    bool Spotted;
    int TID;
    int BossType;
    int Index;
    Position *ChosenPosition;
    
    // Ambient Music
    SetMusic(StrParam("MBossA%d", Random(1, 2)));
    
    // Pick Boss
	if (ColourfulHell)
		CurrentLevel->MegabossActor = &MegaBossesCH[Random(0, MAX_MEGABOSSES_CH - 1)];
	else
		CurrentLevel->MegabossActor = &MegaBosses[Random(0, MAX_MEGABOSSES - 1)];
    
    // Replace them with nothing
    for (int i = 1; i < MonsterID; i++)
    {
        if (!Monsters[i].Init)
            continue;
        
        Monsters[i].ReplaceActor = "None";
    }
    
    WaitingForReplacements = false;
    Delay(1); // Monsters disappear!
    
    // Shuffle positions
    for (int i = 0; i < CurrentLevel->MonsterPositions.Position; i++)
    {
        int X = Random(0, CurrentLevel->MonsterPositions.Position - 1);
        Position TempPosition;
        
        TempPosition = ((Position *)CurrentLevel->MonsterPositions.Data)[i];
        ((Position *)CurrentLevel->MonsterPositions.Data)[i] = ((Position *)CurrentLevel->MonsterPositions.Data)[X];
        ((Position *)CurrentLevel->MonsterPositions.Data)[X] = TempPosition;
    }
    
    // Spawning
    while (!Spawned)
    {
        TID = UniqueTID();
        ChosenPosition = &((Position *)CurrentLevel->MonsterPositions.Data)[Index];
        Spawned = Spawn(CurrentLevel->MegabossActor->Actor, ChosenPosition->X, ChosenPosition->Y, ChosenPosition->Z, TID, ChosenPosition->Angle * 256);
        
        if (GetCVar("drpg_debug"))
            Log("\CdDEBUG: Iterating for Spawn Point... (Class %S, Index %d, Position %.2k/%.2k/%.2k", CurrentLevel->MegabossActor->Actor, Index, ChosenPosition->X, ChosenPosition->Y, ChosenPosition->Z);
        
        // Successful spawn
        if (Spawned)
        {
            SpawnForced("TeleportFog", ChosenPosition->X, ChosenPosition->Y, ChosenPosition->Z, 0, 0);
            GiveActorInventory(TID, "DRPGWhiteAuraGiver", 1);
            if (GetCVar("drpg_debug"))
                Log("\CdDEBUG: \Cg%S MegaBoss successfully spawned", CurrentLevel->MegabossActor->Actor);
        }
        
        Index++;
        if (Index >= CurrentLevel->MonsterPositions.Position)
            Index = 0;
        
        Delay(1);
    }
    
    // EVIL WARNING LAUGHTER
    AmbientSound("mission/gottarget2", 127);
    
    // Loop
    while (true)
    {
        // Checks to perform while the player has not been spotted
        if (!Spotted)
        {
            // Cycle locations every minute until the player has been spotted
            if ((Timer() % (35 * 60)) == 0)
            {
                SetActivator(TID);
                TeleportMonster();
                SetActivator(0);
            }
            
            // Check LOS
            for (int i = 0; i < MAX_PLAYERS; i++)
                if (CheckSight(TID, Players(i).TID, 0))
                {
                    SetMusic(CurrentLevel->MegabossActor->Music);
                    Spotted = true;
                    break;
                }
        }
        
        // Defeated
        if (GetActorProperty(TID, APROP_Health) <= 0)
        {
            SetMusic("*"); // TODO: Some sort of victory fanfare thingy here?
            Delay(35 * 60); // Let the players pick up the loot
            CurrentLevel->EventCompleted = true;
            return;
        }
        
        Delay(1);
    }
}

// Event - Environmental Hazard -----------------------------------------------

NamedScript void EnvironmentalHazard()
{
    // TODO: [03:23:14] <@Yholl/ID> Maybe fill the radiation event with Stalker style singularities that explode into cool stuff when the science machine sciences at them
    bool NeutralizerSpawned = false;
    int NeutralizerTID;
    SetMusic("EvHazard");
    
    Delay (1);
    
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!PlayerInGame(i))
            continue;
        
        TakeActorInventory(Players(i).TID, "DRPGNeutralizerFuel", 5);
        UseActorInventory(Players(i).TID, "DRPGRadSuit");
        UseActorInventory(Players(i).TID, "RadSuit2"); // DRLA
        
        // [KS] Haaaaaaaax
        GiveActorInventory(Players(i).TID, "DRPGAntiRadiationForMyVoodooDolls", 1);
    }
    
    // Setup
    EnvironmentalHazardSetColors();
    EnvironmentalHazardDamage();
    
    int CheckPar = GetLevelInfo(LEVELINFO_PAR_TIME);
    if (CheckPar == 0)
        CheckPar = GetCVar("drpg_default_par_seconds"); // Default Par
    
    CheckPar *= 35;
    
    // Spawn the Neutralizer Fuel Tanks
    int FuelAmount = 35 * 30 * (6 - GameSkill());
    if (FuelAmount < 35 * 30)
        FuelAmount = 35 * 30;
    
    int RadTime = CheckPar * CurrentLevel->HazardLevel;
    int TanksNeeded = RadTime / FuelAmount;
    
    DynamicLootGenerator("DRPGNeutralizerFuel", TanksNeeded + 1 + (12 - GameSkill() * 2));
    
    while (!NeutralizerSpawned)
    {
        NeutralizerTID = UniqueTID();
        NeutralizerSpawned = SpawnEventActor("DRPGRadiationNeutralizer", NeutralizerTID);
        Delay(1);
    }
    
    int RadTimeRequired = CheckPar / 100;
    while (CurrentLevel->Event == MAPEVENT_TOXICHAZARD && !CurrentLevel->EventCompleted)
    {
        if (CurrentLevel->GeneratorFuel)
        {
            if (!GetUserVariable(NeutralizerTID, "user_running"))
            {
                SetUserVariable(NeutralizerTID, "user_running", 1);
                Thing_Activate(NeutralizerTID);
            }
            
            CurrentLevel->GeneratorFuel--;
            
            if (Timer() % RadTimeRequired == 0)
            {
                CurrentLevel->RadLeft--;
                
                if (CurrentLevel->RadLeft == 0)
                {
                    CurrentLevel->HazardLevel--;
                    
                    if (CurrentLevel->HazardLevel == 0)
                    {
                        if (GetUserVariable(NeutralizerTID, "user_running"))
                            Thing_Deactivate(NeutralizerTID);
                        EnvironmentalHazardDisarm();
                        return;
                    }
                    
                    CurrentLevel->RadLeft = 100;
                    EnvironmentalHazardSetColors();
                    AmbientSound("radiation/lowered", 127);
                    SetFont("BIGFONT");
                    HudMessage("Radiation level reduced to %d", CurrentLevel->HazardLevel);
                    EndHudMessageBold(HUDMSG_FADEOUT | HUDMSG_LOG, 0, "Yellow", 0.5, 0.25, 2.0, 1.0);
                }
            }
        }
        else
        {
            if (GetUserVariable(NeutralizerTID, "user_running"))
                Thing_Deactivate(NeutralizerTID);
        }
        
        Delay(1);
    }
}

NamedScript void EnvironmentalHazardSetColors()
{
    if (CurrentLevel->EventCompleted)
    {
        for (int i = 0; i < 65536; i++)
        {
            Sector_SetColor(i, 255, 255, 255);
            Sector_SetFade(i, 0, 0, 0);
        }
        
        return;
    }
    
    int FadeR = (63  * CurrentLevel->HazardLevel) / 5;
    int FadeG = (131 * CurrentLevel->HazardLevel) / 5;
    int FadeB = (47  * CurrentLevel->HazardLevel) / 5;
    
    int ColorR = 255 - ((192 * CurrentLevel->HazardLevel) / 5);
    int ColorG = 255 - ((124 * CurrentLevel->HazardLevel) / 5);
    int ColorB = 255 - ((208 * CurrentLevel->HazardLevel) / 5);
    
    for (int i = 0; i < 65536; i++)
    {
        Sector_SetColor(i, ColorR, ColorG, ColorB);
        Sector_SetFade(i, FadeR, FadeG, FadeB);
    }
}

NamedScript void EnvironmentalHazardDamage()
{
    Delay(32); // Initial buffer to allow players to Transport away if they don't have suits
    
    int Damage;
    
    while (CurrentLevel->HazardLevel)
    {
        Damage = 20;

        if (CurrentLevel->HazardLevel < 3)
            Damage = 5;
        else if (CurrentLevel->HazardLevel < 5)
            Damage = 10;
        
        for (int i = 0; i < 65536; i++)
            SectorDamage(i, Damage, "Radiation", "PowerIronFeet", DAMAGE_PLAYERS | DAMAGE_IN_AIR | DAMAGE_SUBCLASSES_PROTECT);
        
        // Damage Turrets
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            // Player is not in-game
            if (!PlayerInGame(i)) continue;
            
            // Don't have a turret or it isn't out
            if (!Players(i).Turret.Upgrade[TU_BUILD] || !Players(i).Turret.Active) continue;
            
            Thing_Damage2(Players(i).Turret.TID, Damage, "Radiation");
            SetUserVariable(Players(i).Turret.TID, "user_damage_type", DT_RADIATION);
        }
        
        Delay(32);
    }
}

NamedScript DECORATE void EnvironmentalHazardRefillGenerator()
{
    int FuelAmount = 35 * 30 * (6 - GameSkill());
    if (FuelAmount < 35 * 30)
        FuelAmount = 35 * 30;
    
    SetActivator(0, AAPTR_TARGET);
    
    if (CurrentLevel->Event != MAPEVENT_TOXICHAZARD || CurrentLevel->EventCompleted)
        return;
    
    if (!CheckInventory("DRPGNeutralizerFuel"))
    {
        PrintError("You don't have any fuel tanks");
        return;
    }
    
    if (CurrentLevel->GeneratorFuel > 0)
    {
        DropInventory(0, "DRPGNeutralizerFuel");
        ActivatorSound("radiation/tankup", 127);
        PrintMessage("You set down the spare tank");
        return;
    }
    
    TakeInventory("DRPGNeutralizerFuel", 1);
    CurrentLevel->GeneratorFuel = FuelAmount;
    
    AmbientSound("radiation/refuel", 127);
    SetFont("BIGFONT");
    HudMessage("Generator refueled");
    EndHudMessageBold(HUDMSG_FADEOUT, 0, "White", 0.5, 0.25, 2.0, 1.0);
}

NamedScript void EnvironmentalHazardDisarm()
{
    if (CurrentLevel->Event != MAPEVENT_TOXICHAZARD || CurrentLevel->EventCompleted)
        return;
    
    EnvironmentalHazardSetColors();
    CurrentLevel->EventCompleted = true;
    
    SetMusic("*");
    
    SetFont("BIGFONT");
    HudMessage("Radiation cleared");
    EndHudMessageBold(HUDMSG_FADEOUT | HUDMSG_LOG, 0, "Green", 0.5, 0.25, 2.0, 1.0);
    AmbientSound("radiation/cleared", 127);
    
    MapEventReward();
}

// Event - Thermonuclear Bomb -------------------------------------------------

NamedScript void ThermonuclearBombEvent()
{
    int BombTID = UniqueTID();
    int MaxKeys = GameSkill() + 3;
    bool BombSpawned = false;
    bool BombDisarmed = false;
    bool JC = (!Random(0, 9));
    
    // Calculate bomb time
    CurrentLevel->BombTime = (GetLevelInfo(LEVELINFO_PAR_TIME) ? GetLevelInfo(LEVELINFO_PAR_TIME) * 2 : GetCVar("drpg_default_par_seconds") * 2) * 35;
    
    // Reset key states
    for (int i = 0; i < MAX_NUKE_KEYS; i++)
    {
        CurrentLevel->BombKeyActive[i] = false;
        CurrentLevel->BombKeyDisarming[i] = false;
    }
    
    // Setup Keys
    if (GetCVar("drpg_debug"))
        Log("\CdDEBUG: \C-Generating %d keys for bomb", MaxKeys);
    while (MaxKeys > 0)
    {
        int RandomKey = Random(0, MAX_NUKE_KEYS - 1);
        
        if (!CurrentLevel->BombKeyActive[RandomKey])
        {
            CurrentLevel->BombKeyActive[RandomKey] = true;
            CurrentLevel->BombKeyTimer[RandomKey] = CurrentLevel->BombTime / MAX_NUKE_KEYS * 2;
            MaxKeys--;
        };
        
        Delay(1);
    }
    
    // Spawn the Bomb
    while (!BombSpawned)
    {
        BombSpawned = SpawnEventActor("DRPGThermonuclearBomb", BombTID);
        Delay(1);
    }
    
    if (JC)
    {
        SetMusic("Bomb2");
        Delay(35 * 3);
    }
    else
    {
        SetMusic("");
        Delay(35);
    }
    
    // Warning message
    SetHudSize(640, 480, false);
    SetFont("BIGFONT");
    HudMessage("\CgWARNING! WARNING!\n\C[White]THERMONUCLEAR BOMB ACTIVATED!\n\CiTIME UNTIL DETONATION: %S",
               FormatTime(CurrentLevel->BombTime));
    EndHudMessageBold(HUDMSG_TYPEON, 0, "Red", 320.4, 160.0, 3.0, 0.03, 0.5);
    AmbientSound("nuke/alert", 127);
    SetHudSize(0, 0, false);
    
    Delay(35 * 4);
    
    if (!JC)
        SetMusic("Bomb");
    
    // Set bomb's active state
    SetActorState(BombTID, "SpawnActive");
    
    int DisarmCount;
    
    // Bomb Loop
    while (CurrentLevel->BombTime > 0 && !BombDisarmed && !CurrentLevel->BombExplode)
    {
        // Announcements
        ThermonuclearBombAnnounce(CurrentLevel->BombTime);
        
        // Acceleration handling
        fixed Velocity = AbsFixed(GetActorVelX(BombTID)) + AbsFixed(GetActorVelX(BombTID));
        if (Velocity > 0)
            PlaySound(BombTID, "nuke/beep", CHAN_BODY, 1.0, true, ATTN_NORM);
        else
            StopSound(BombTID, CHAN_BODY);
        if (Velocity > 7.5) // WHOOPS, you made it angry, now die
            CurrentLevel->BombExplode = true;
        
        // Decrease key timers if the key is being disarmed, also disarm the specified lock if disarming is finished
        for (int i = 0; i < MAX_NUKE_KEYS; i++)
        {
            if (CurrentLevel->BombKeyDisarming[i])
                CurrentLevel->BombKeyTimer[i]--;
            
            if (CurrentLevel->BombKeyDisarming[i] && CurrentLevel->BombKeyTimer[i] <= 0)
            {
                CurrentLevel->BombKeyActive[i] = false;
                CurrentLevel->BombKeyDisarming[i] = false;
                PlaySound(BombTID, "nuke/unlock", CHAN_ITEM, 1.0, false, ATTN_NORM);
            }
        }
        
        // Disarm the bomb if all keys are disarmed
        DisarmCount = 0;
        for (int i = 0; i < MAX_NUKE_KEYS; i++)
            if (!CurrentLevel->BombKeyActive[i])
                DisarmCount++;
        if (DisarmCount >= MAX_NUKE_KEYS)
        {
            CurrentLevel->BombTime = 0;
            BombDisarmed = true;
        }
        
        CurrentLevel->BombTime--;
        Delay(1);
    }
    
    // Successful disarm
    if (BombDisarmed)
    {
        CurrentLevel->EventCompleted = true;
        
        SetActorState(BombTID, "Disarmed");
        SetUserVariable(BombTID, "user_disarmed", 1);
        AmbientSound("nuke/disarmed", 127);
        SetMusic("*");
        
        MapEventReward();
    }
    else if (!BombDisarmed || CurrentLevel->BombExplode) // Epic failure, explode everyone to die
    {
        SetActorState(BombTID, "Explode");
        
        CurrentLevel->EventCompleted = true;
        
        SetMusic("");
        
        Delay(35 * 5);
        SetMusic("*");
    }
    
    // Remove all keys from the players
    while (true)
    {
        for (int i = 0; i < MAX_PLAYERS; i++)
            for (int j = 0; j < MAX_NUKE_KEYS; j++)
                SetActorInventory(Players(i).TID, StrParam("DRPGNukeKey%d", j + 1), 0);
        
        Delay(1);
    }
}

NamedScript void ThermonuclearBombAnnounce(int Time)
{
    int Minutes = Time / 60 / 35;
    int Seconds = Time / 35 % 60;
    bool ValidMinutes = (Minutes == 1 || Minutes == 2 || Minutes == 3 || Minutes == 4 || Minutes == 5 || Minutes == 10 || Minutes == 20);
    bool ValidSeconds = (Seconds == 0 || Seconds == 1 || Seconds == 2 || Seconds == 3 || Seconds == 4 || Seconds == 5 || Seconds == 10 || Seconds == 20 || Seconds == 30);
    
    // Return if there is currently an announcement playing
    if (CurrentLevel->BombAnnouncing) return;
    
    if (ValidMinutes || ValidSeconds)
    {
        CurrentLevel->BombAnnouncing = true;
        
        // Minutes
        if (Minutes > 0 && ValidMinutes && Seconds == 0)
        {
            AmbientSound("nuke/announce1", 127);
            Delay(35 * 1.9);
            AmbientSound("nuke/announce2", 127);
            Delay(35 * 1.4);
            AmbientSound(StrParam("nuke/minutes/%d", Minutes), 127);
        }
        
        // Seconds
        if (Minutes == 0 && ValidSeconds)
        {
            AmbientSound(StrParam("nuke/seconds/%d", Seconds), 127);
            Delay(35);
        }
    }
    
    CurrentLevel->BombAnnouncing = false;
}

NamedScript DECORATE void ThermonuclearBombActivate()
{
    SetActivator(GetActorProperty(0, APROP_TargetTID));
    
    for (int i = 0; i < MAX_NUKE_KEYS; i++)
        if (CheckInventory(StrParam("DRPGNukeKey%d", i + 1)))
        {
            CurrentLevel->BombKeyActive[i] = false;
            TakeInventory(StrParam("DRPGNukeKey%d", i + 1), 1);
        }
    
    for (int i = 0; i < MAX_NUKE_KEYS; i++)
    {
        if (!CurrentLevel->BombKeyActive[i]) continue;
        
        if (!CurrentLevel->BombKeyDisarming[i])
        {
            CurrentLevel->BombKeyDisarming[i] = true;
            break;
        }
        else
            break;
    }
}

NamedScript DECORATE void ThermonuclearBombExplode()
{
    CurrentLevel->BombExplode = true;
}

// Event - Low Power ----------------------------------------------------------

NamedScriptSync void LowPowerAdjustTheLights1a()
{
    for (int i = 0; i < 65536; i++)
    {
        Light_Stop(i);
    }
}

NamedScriptSync void LowPowerAdjustTheLights1b()
{
    for (int i = 0; i < 65536; i++)
    {
        Light_Flicker(i, 48, 96);
    }
}

NamedScriptSync void LowPowerAdjustTheLights1c()
{
    for (int i = 0; i < 65536; i++)
    {
        Sector_SetColor(i, 64, 96, 255, 0);
    }
}

NamedScriptSync void LowPowerAdjustTheLights2a()
{
    for (int i = 0; i < 65536; i++)
    {
        Light_Stop(i);
    }
}

NamedScriptSync void LowPowerAdjustTheLights2b()
{
    for (int i = 0; i < 65536; i++)
    {
        Light_Glow(i, 160, 96, 35 * 3);
    }
}

NamedScriptSync void LowPowerAdjustTheLights2c()
{
    for (int i = 0; i < 65536; i++)
    {
        Sector_SetColor(i, 255, 128, 32, 0);
    }
}

NamedScript void LowPowerEvent()
{
    int GeneratorTID = UniqueTID();
    bool GeneratorSpawned = false;
    
    AmbientSound("misc/poweroff", 127);
    SetMusic("LowPower");
    CurrentLevel->EventCompleted = true; // This event disappears when you leave
    DynamicLootGenerator("DRPGLowPowerJunkSpawner", Random(50, 100));
    
    LowPowerAdjustTheLights1a();
    LowPowerAdjustTheLights1b();
    LowPowerAdjustTheLights1c();
    
    // Spawn the Generator
    while (!GeneratorSpawned)
    {
        GeneratorSpawned = SpawnEventActor("DRPGGenerator", GeneratorTID);
        Delay(1);
    }
    
    // Wait until the generator is powered back up
    while (!CurrentLevel->PowerGeneratorActive)
        Delay(1);
    
    // Generator is now active - Activate emergency lighting
    AmbientSound("misc/poweron", 127);
    SetActorState(GeneratorTID, "PoweredUp");
    SetUserVariable(GeneratorTID, "user_powered", 1);
    LowPowerAdjustTheLights2a();
    LowPowerAdjustTheLights2b();
    LowPowerAdjustTheLights2c();
    MapEventReward();
    
    // Remove generator power cells from the players
    while (true)
    {
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            SetActorInventory(Players(i).TID, "DRPGGeneratorCell", 0);
            SetActorInventory(Players(i).TID, "DRPGGeneratorCellDead", 0);
        }
        
        Delay(1);
    }
}

NamedScript DECORATE void PowerGeneratorActivate()
{
    SetActivator(GetActorProperty(0, APROP_TargetTID));
    
    if (CheckInventory("DRPGGeneratorCell"))
    {
        CurrentLevel->PowerGeneratorActive = true;
        TakeInventory("DRPGGeneratorCell", 1);
    }
}

// Event - One-Monster --------------------------------------------------------

NamedScript void OneMonsterEvent()
{
    bool Extras = (CompatMode == COMPAT_EXTRAS);
    bool DRLA = (CompatMode == COMPAT_DRLA);

    for (int i = 0; i < MonsterID; i++)
    {
        if (!Monsters[i].Init)
            continue;
        
        Monsters[i].ReplaceActor = GetMissionMonsterActor(CurrentLevel->SelectedMonster->Actor);
    }
    
    // Level feeling
    SetHudSize(640, 480, false);
    SetFont("BIGFONT");
    HudMessage("%S", CurrentLevel->SelectedMonster->Feeling);
    EndHudMessageBold(HUDMSG_FADEOUT, 0, "Brick", 320.4, 150.0, 1.0, 19.0);
    SetHudSize(0, 0, false);
    
    Delay(35); // Allow the replacements to actually spawn.
    
    while (ThingCountName(GetMissionMonsterActor(CurrentLevel->SelectedMonster->Actor), 0) > 0)
        Delay(1);
    
    AmbientSound("misc/secret", 127);
    HudMessage("Everything falls silent.");
    EndHudMessageBold(HUDMSG_FADEOUT, 0, "LightBlue", 0.5, 0.7, 5.0, 5.0);
    MapEventReward();
    
    CurrentLevel->EventCompleted = true;
}

// Event - Hell Unleashed -----------------------------------------------------

NamedScript void HellUnleashedEvent()
{
    bool BoxSpawned = false;
    
    // Generate a TID for the box
    CurrentLevel->PandoraBoxTID = UniqueTID();
    
    // Spawn Pandora's Box
    while (!BoxSpawned && !CurrentLevel->HellUnleashedActive)
    {
        BoxSpawned = SpawnEventActor("DRPGPandoraBox", CurrentLevel->PandoraBoxTID);
        Delay(35);
    }
    
    Start:
    
    while (CurrentLevel->HellUnleashedActive >= 2)
    {
        fixed ParTime = (fixed)GetLevelInfo(LEVELINFO_PAR_TIME);
        if (ParTime <= 0) // Assign a default value to prevent divide-by-zero
            ParTime = 35 * GetCVar("drpg_default_par_seconds"); // Default Par
        fixed AddMult = 0.1 / ParTime;
        
        // Cap this to make sure it isn't too low
        if (AddMult < 0.001)
            AddMult = 0.001;
        
        // Spawn in next wave of enemies
        if ((Timer() % (35 * (int)(ParTime / GameSkill()))) == 0)
            HellUnleashedSpawnMonsters();
        
        // Slowly increment the values
        CurrentLevel->LevelAdd += RandomFixed(AddMult, AddMult * 1.0);
        CurrentLevel->RareAdd += RandomFixed(AddMult * 0.01, AddMult * 0.1);
        
        // Level Imposed Caps
        if (CurrentLevel->LevelAdd > (fixed)AveragePlayerLevel() * (fixed)GameSkill())
            CurrentLevel->LevelAdd = (fixed)AveragePlayerLevel() * (fixed)GameSkill();
        if (CurrentLevel->RareAdd > AveragePlayerLevel() / 10.0)
            CurrentLevel->RareAdd = AveragePlayerLevel() / 10.0;
        
        // Hard Caps
        if (CurrentLevel->LevelAdd > 1000)
            CurrentLevel->LevelAdd = 1000;
        if (CurrentLevel->RareAdd > 100)
            CurrentLevel->RareAdd = 100;
        
        Delay(1);
    }
    
    Delay(1);
    goto Start;
}

NamedScript DECORATE void HellUnleashedStart()
{
    // Spit out a rare item for each player
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!PlayerInGame(i)) continue;
        
        int Rarity = (CompatMode == COMPAT_DRLA ? MAX_DIFFICULTIES + 1 : MAX_DIFFICULTIES - 1);
        ItemInfoPtr Item = GetRewardItem(Rarity);
        int ItemTID = UniqueTID();
        fixed X = GetActorX(CurrentLevel->PandoraBoxTID);
        fixed Y = GetActorY(CurrentLevel->PandoraBoxTID);
        fixed Z = GetActorZ(CurrentLevel->PandoraBoxTID) + 16.0;
        
        SpawnForced(Item->Actor, X, Y, Z, ItemTID, 0);
        SetActorVelocity(ItemTID, RandomFixed(-4.0, 4.0), RandomFixed(-4.0, 4.0), RandomFixed(2.0, 8.0), true, false);
    };
    
    CurrentLevel->HellUnleashedActive = 1;
    
    Delay(35 * 8);
    
    CurrentLevel->HellUnleashedActive = 2;
    
    // Spawn an initial wave of enemies
    HellUnleashedSpawnMonsters();
    
    SetMusic("HellUnle");
}

NamedScript void HellUnleashedSpawnMonsters()
{
    if (GetCVar("drpg_debug"))
        Log("\CdDEBUG: \CaHell Unleashed \C-- Spawning next wave of monsters...");
    
    Delay(1); // Maximize our instructions
    
    int TID;
    MonsterStatsPtr Stats;
    bool Success;
    
    for (int i = 0; i < CurrentLevel->MonsterPositions.Position; i++)
    {
        TID = UniqueTID();
        Success = false;
        Position *CurrentPosition = &((Position *)CurrentLevel->MonsterPositions.Data)[i];; // Totally leaving this typo here because IT'S CRYING OKAY, I AM TOO
        
        // Determine a monster
        MonsterInfoPtr Monster;
		if (ColourfulHell)
			Monster = &MonsterDataCH[Random(0, MAX_DEF_MONSTERS_CH - 1)];
        else if (CompatMode == COMPAT_DRLA)
            Monster = &MonsterDataDRLA[Random(0, MAX_DEF_MONSTERS_DRLA - 1)];
        else if (CompatMode == COMPAT_LEGENDOOM)
            Monster = &MonsterDataLD[Random(0, MAX_DEF_MONSTERS - 1)];
        else
            Monster = &MonsterData[Random(0, MAX_DEF_MONSTERS - 1)];
        
        Success = Spawn(Monster->Actor, CurrentPosition->X, CurrentPosition->Y, CurrentPosition->Z, TID, CurrentPosition->Angle);
        if (Success)
        {
            Spawn("TeleportFog", CurrentPosition->X, CurrentPosition->Y, CurrentPosition->Z, 0, CurrentPosition->Angle);
            // Setup Stats
            Stats = &Monsters[GetMonsterID(TID)];
            Stats->Level += (int)CurrentLevel->LevelAdd;
            Stats->NeedReinit = true;        
        }
        // Stagger the loop here so that we can make monsters appear to spawn in semi-randomly
        Delay(Random(1, 10));
    }
}

// Event - Harmonized Destruction ---------------------------------------------

NamedScript void HarmonizedDestructionEvent()
{
    if (AveragePlayerLevel() < 20 && CurrentLevel->AuraType == AURA_MAX)
        CurrentLevel->AuraType = Random(0, AURA_MAX - 1);
    
    if (GetCVar("drpg_debug"))
        Log("\CdDEBUG: Aura type %d", CurrentLevel->AuraType);
    
    for (int i = 0; i < MonsterID; i++)
    {
        if (!Monsters[i].Init)
            continue;
        
        if (CurrentLevel->AuraType == AURA_MAX)
        {
            for (int j = 0; j < AURA_MAX; j++)
                Monsters[i].Aura.Type[j].Active = true;
        }
        else
            Monsters[i].Aura.Type[CurrentLevel->AuraType].Active = true;
        
        Monsters[i].Flags &= MF_NOAURAGEN; // Don't let our aura get overwritten
    }
    
    // Level feeling
    SetMusic("OneAura");
    SetHudSize(640, 480, false);
    SetFont("BIGFONT");
    if (CurrentLevel->AuraType != AURA_MAX)
    {
        HudMessage("A strange, malicious harmony fills the air.");
        EndHudMessageBold(HUDMSG_FADEOUT, 0, "DarkRed", 320.0, 150.0, 1.0, 19.0);
    }
    else
    {
        HudMessage("A strange, malicious harmony fills the air.\nAnd frankly, it's terrifying.");
        EndHudMessageBold(HUDMSG_FADEOUT, 0, "DarkRed", 320.0, 150.0, 1.0, 19.0);
    }
    SetHudSize(0, 0, false);
}

// Event - Cracks in the Veil -------------------------------------------------

NamedScript void TeleportCracksEvent()
{
    SetMusic("Cracks");
    
    int X, InPortalTID, OutPortalTID;
    // Shuffle positions
    for (int i = 0; i < CurrentLevel->MonsterPositions.Position; i++)
    {
        X = Random(0, CurrentLevel->MonsterPositions.Position - 1);
        Position TempPosition;
        
        TempPosition = ((Position *)CurrentLevel->MonsterPositions.Data)[i];
        ((Position *)CurrentLevel->MonsterPositions.Data)[i] = ((Position *)CurrentLevel->MonsterPositions.Data)[X];
        ((Position *)CurrentLevel->MonsterPositions.Data)[X] = TempPosition;
    }
    
    // Spawn the cracks
    bool SpawnedCracks = false;
    
    while (!SpawnedCracks)
    {
        for (int i = 0; i < CurrentLevel->MonsterPositions.Position - 1; i += 2)
        {
            if (Random(0, 3))
                continue;
            
            Position Source = ((Position *)CurrentLevel->MonsterPositions.Data)[i];
            Position Destination = ((Position *)CurrentLevel->MonsterPositions.Data)[i + 1];
            InPortalTID = UniqueTID();
            if (!Spawn("DRPGTeleportCrackIn", Source.X, Source.Y, Source.Z, InPortalTID, Source.Angle * 256))
                continue;
            
            OutPortalTID = UniqueTID();
            if (!Spawn("DRPGTeleportCrackOut", Destination.X, Destination.Y, Destination.Z, OutPortalTID, Destination.Angle * 256))
            {
                Thing_Remove(InPortalTID);
                continue;
            }
            
            SpawnedCracks = true;
            TeleporterCrack(InPortalTID, OutPortalTID);
        }
    }
    
    // Crack effects
    while (true)
    {
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (!PlayerInGame(i)) continue;
            
            // Quake
            Radius_Quake2(Players(i).TID, 4, 20, 0, 16, "world/quake");
            
            // View Fuckery
            if (!Random(0, 3))
                TeleporterCrackView(i);
        }
        
        AmbientSound("misc/teleport", 127);
        
        Delay(Random(35 * 2, 35 * 20));
    }
}

NamedScript void TeleporterCrackView(int PlayerID)
{
    SetActivator(Players(PlayerID).TID);
    fixed ViewTimeMax = 35.0 * RandomFixed(1.0, 3.0);
    fixed ViewTime = 0;
    fixed Intensity = RandomFixed(1.0, 5.0);
    
    fixed ViewCycle, ViewDist, Angle1, Angle2;
    
    Start:
    
    if (ViewTime >= ViewTimeMax) return;
    
    SetHudSize(640, 480, true);
    SetFont(StrParam("P%iVIEW", PlayerID + 1));
    
    ViewCycle = Timer() / (120.0 - (10.0 * Intensity));
    ViewDist = 25.0 * Intensity * (ViewTime++ / ViewTimeMax);
    Angle1 = RandomFixed(0.0, 1.0);
    Angle2 = RandomFixed(0.0, 1.0);
    
    // View Intensification
    HudMessage("A");
    EndHudMessage(HUDMSG_PLAIN | HUDMSG_LAYER_UNDERHUD | HUDMSG_NOTWITHFULLMAP, CONFUSION_ID + 2, "Untranslated", 320, 240, 0.029);
    HudMessage("A");
    EndHudMessage(HUDMSG_PLAIN | HUDMSG_ALPHA | HUDMSG_LAYER_UNDERHUD | HUDMSG_NOTWITHFULLMAP, CONFUSION_ID, "Untranslated",
               320 + (int)(ViewDist * Cos(Angle1)),
               240 + (int)(ViewDist * Sin(Angle1)),
               0.029, 0.6 - (ViewTime / ViewTimeMax) / 2.0);
    HudMessage("A");
    EndHudMessage(HUDMSG_PLAIN | HUDMSG_ALPHA | HUDMSG_LAYER_UNDERHUD | HUDMSG_NOTWITHFULLMAP, CONFUSION_ID + 1, "Untranslated",
               320 + (int)(ViewDist * Cos(Angle2)),
               240 + (int)(ViewDist * Sin(Angle2)),
               0.029, 0.6 - (ViewTime / ViewTimeMax) / 2.0);
    
    Delay(1);
    goto Start;
}

NamedScript void TeleporterCrack(int Source, int Destination)
{
    bool DRLA = (CompatMode == COMPAT_DRLA);
    int SpawnEnemyTimer = 0;
    bool CloseToPlayer;
    str TempMonster;
    
    Start:
    CloseToPlayer = false;
    
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (Distance(Source, Players(i).TID) <= 32.0)
            TeleportOther(Players(i).TID, Destination, true);
        
        if (Distance(Destination, Players(i).TID) <= 2048.0)
            CloseToPlayer = true;
    }
    
    if (++SpawnEnemyTimer >= 263 && Random(1, 4096) == 1 && CloseToPlayer)
    {
        TempMonster = "";
		if (ColourfulHell)
			TempMonster = "BlackPE2";
        if (DRLA)
            TempMonster = "RLArmageddonPainElemental";
        else
            TempMonster = "ArchVile";
        
        if (SpawnSpotFacing(GetMissionMonsterActor(TempMonster), Destination))
            SpawnSpotFacing("TeleportFog", Destination);
        
        SpawnEnemyTimer = 0;
    }
    
    Delay(4);
    goto Start;
}

// Event - 12 Hours 'til Doomsday ---------------------------------------------

NamedScript void DoomsdayEvent()
{
    ChangeSky("FIRESK00", "-");
    
    for (int i = 0; i < 65536; i++)
        Sector_SetColor(i, 255, 128, 64, 0);
    
    for (int i = 0; i < MonsterID; i++)
    {
        if (!Monsters[i].Init || Random(0, 3))
            continue;
        
        Monsters[i].Aura.Type[AURA_RED].Active = true;
        
        Monsters[i].NeedReinit = true;
    }
    
    // Calculate Doomsday time
    CurrentLevel->DoomTime = (GetLevelInfo(LEVELINFO_PAR_TIME) ? GetLevelInfo(LEVELINFO_PAR_TIME) * 5 : GetCVar("drpg_default_par_seconds") * 5) * 35;
    
    SetMusic("");
    
    Delay(35);
    
    // Warning message
    SetHudSize(640, 480, false);
    SetFont("BIGFONT");
    HudMessage("The smell of a massacre...\n\n\CiTime until Doomsday: %S",
               FormatTime(CurrentLevel->DoomTime));
    EndHudMessageBold(HUDMSG_TYPEON, 0, "Red", 320.4, 160.0, 3.0, 0.03, 0.5);
    SetHudSize(0, 0, false);
    
    Delay(35 * 4);
    
    SetMusic("Doomsday");
    
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!PlayerInGame(i))
            continue;
        
        DoomsdayFirebomb(Players(i).TID);
        DoomsdayFlameRain(Players(i).TID);
    }
    
    // Wait Loop
    while (CurrentLevel->DoomTime > 0)
    {
        if (CurrentLevel->DoomTime == (GetLevelInfo(LEVELINFO_PAR_TIME) ? GetLevelInfo(LEVELINFO_PAR_TIME) : GetCVar("drpg_default_par_seconds")) * 35)
        {
            AmbientSound ("doomsday/doombell", 127);
            
            SetHudSize(640, 480, false);
            SetFont("BIGFONT");
            HudMessage("Hurry, there is not much time...");
            EndHudMessageBold(HUDMSG_TYPEON, 0, "Brick", 320.4, 160.0, 3.0, 0.03, 0.5);
            SetHudSize(0, 0, false);
        }
        
        CurrentLevel->DoomTime--;
        Delay(1);
    }
    
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!PlayerInGame(i))
            continue;
        
        SetActorProperty(Players(i).TID, APROP_Health, -1000000);
        FadeRange(255, 255, 255, 1.0, 255, 255, 255, 0.0, 2.0);
        ActivatorSound("nuke/detonate", 127);
        Radius_Quake2(0, 9, 70, 0, 512, "None");
    }
    
    SetMusic("");
    
    if (GameType() == GAME_NET_COOPERATIVE)
    {
        Delay(35);
        
        ChangeLevel(DefaultOutpost->LumpName, 0, CHANGELEVEL_NOINTERMISSION, CurrentSkill);
    }
}

NamedScript void DoomsdayFirebomb(int PlayerTID)
{
    SetActivator(PlayerTID);
    
    while (CurrentLevel->Event == MAPEVENT_DOOMSDAY && CurrentLevel->DoomTime > 0)
    {
        Delay(Random(35 * 30, 35 * 90));
        SpawnSpotFacing("DRPGDoomsdayMortarBlast", 0, 0);
        Radius_Quake2(0, 6, 16, 0, 512, "None");
        FadeRange(255, 128, 64, 0.33, 255, 128, 64, 0.0, 0.5);
        ActivatorSound("drpgmarines/bulletexp", 127);
        if (CurrentLevel->DoomTime < (GetLevelInfo(LEVELINFO_PAR_TIME) ? GetLevelInfo(LEVELINFO_PAR_TIME) * 2 : GetCVar("drpg_default_par_seconds") * 2))
        {
            Delay(35);
            Radius_Quake2(0, 3, 316, 0, 1024, "doomsday/moonshake");
            Delay(316);
        }
    }
}

NamedScript void DoomsdayFlameRain(int PlayerTID)
{
    SetActivator(PlayerTID);
    
    while (CurrentLevel->Event == MAPEVENT_DOOMSDAY)
    {
        Delay(1);
        SpawnSpot("DRPGDoomsdayFireBloodRainSpawner", 0, 0, 0);
    }
}

NamedScript Type_RESPAWN void DoomsdaySupplement()
{
    if (CurrentLevel->Event == MAPEVENT_DOOMSDAY && CurrentLevel->DoomTime == 0)
        SetActorProperty(0, APROP_Health, -1000000);
}

// Event - Vicious Downpour ---------------------------------------------------

NamedScript void ViciousDownpourEvent()
{
    SetMusic("AcidRain");
    SpawnForced("DRPGRainAmbiance", 0, 0, 0, 0, 0);
    
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!PlayerInGame(i))
            continue;
        
        AcidRain(Players(i).TID);
    }
}

NamedScript void AcidRain(int PlayerTID)
{
    SetActivator(PlayerTID);
    
    while (CurrentLevel->Event == MAPEVENT_ACIDRAIN)
    {
        Delay(1);
        SpawnSpot("DRPGViciousRainSpawner", 0, 0, 0);
    }
}

// Event - The Dark Zone ------------------------------------------------------

NamedScript void DarkZoneEvent()
{
    int ShadowTime = (GetLevelInfo(LEVELINFO_PAR_TIME) ? GetLevelInfo(LEVELINFO_PAR_TIME) * 2 : GetCVar("drpg_default_par_seconds") * 2) * 35;
    int ShadowTimeMax = (GetLevelInfo(LEVELINFO_PAR_TIME) ? GetLevelInfo(LEVELINFO_PAR_TIME) * 2 : GetCVar("drpg_default_par_seconds") * 2) * 35;
    int Color = 16 + (ShadowTime * 239 / ShadowTimeMax);
    int LastColor = 0;
    
    SetMusic("DarkZone");
    ChangeSky("BlackSky", "-");
    
    for (int i = 0; i < 65536; i++)
        Light_Fade(i, 16, ShadowTime);

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!PlayerInGame(i))
            continue;
        
        DarkZoneFloorMist(Players(i).TID);
    }
    
    // Wait Loop
    while (ShadowTime > 0)
    {
        ShadowTime--;
        
        Color = 16 + (ShadowTime * 239 / ShadowTimeMax);
        if (Color != LastColor)
        {
            for (int i = 0; i < 65536; i++)
                Sector_SetColor(i, 128 + (Color / 2), Color, 170 + (Color / 3), 0);
            
            LastColor = Color;
        }
        
        Delay(1);
    }
    
    SetHudSize(640, 480, false);
    SetFont("BIGFONT");
    HudMessage("The Shadows have arrived...");
    EndHudMessageBold(HUDMSG_TYPEON, 0, "Purple", 320.4, 160.0, 3.0, 0.03, 0.5);
    AmbientSound("darkzone/arrived", 127);
    SetHudSize(0, 0, false);
    
    while (true)
    {
        for (int i = 0; i < MonsterID; i++)
        {
            if (!Monsters[i].Init || MonsterHasShadowAura(&(Monsters[i])) || ClassifyActor(Monsters[i].TID) & ACTOR_DEAD && !CanRaiseActor(Monsters[i].TID))
                continue;
            
            if (ClassifyActor(Monsters[i].TID) & ACTOR_DEAD)
                Thing_Raise(Monsters[i].TID);
            
            for (int a = 0; a < AURA_MAX; ++a)
                Monsters[i].Aura.Type[a].Active = true;
            
            Monsters[i].NeedReinit = true;
            SpawnSpotFacing("ArchvileFire", Monsters[i].TID, 0);
            break;
        }
        
        Delay(35 * Random(20, 60));
    }
}

NamedScript void DarkZoneFloorMist(int PlayerTID)
{
    SetActivator(PlayerTID);
    
    while (CurrentLevel->Event == MAPEVENT_DARKZONE)
    {
        Delay(1);
        SpawnSpot("DRPGPurpleFloorMistSpawner", 0, 0, 0);
    }
}

// Special Map Events

// MAP30 Special Event - Sinstorm ---------------------------------------------

NamedScript void SinstormEvent()
{
    ChangeSky("FIRESK00", "-");
    
    for (int i = 0; i < 65536; i++)
        Sector_SetColor(i, 255, 128, 64, 0);
    
    for (int i = 0; i < MonsterID; i++)
    {
        if (!Monsters[i].Init || Random(0, 30))
            continue;
        
        for (int j = 0; j < AURA_MAX; j++)
            Monsters[i].Aura.Type[j].Active = true;
        
        Monsters[i].NeedReinit = true;
    }
    
    SetMusic("");
    
    Delay(35);
    
    // Warning message
    SetHudSize(640, 480, false);
    SetFont("BIGFONT");
    HudMessage("The Icon of Sin senses your approach.");
    EndHudMessageBold(HUDMSG_TYPEON, 0, "Red", 320.4, 160.0, 3.0, 0.03, 0.5);
    AmbientSound ("doomsday/doombell", 127);
    SetHudSize(0, 0, false);
    
    Delay(35 * 4);
    
    SetMusic("Sinstorm");
    
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!PlayerInGame(i))
            continue;
        
        DoomsdayFlameRain(Players(i).TID);
        SinstormSpawner(Players(i).TID);
    }
}

NamedScript void SinstormSpawner(int PlayerTID)
{
    SetActivator(PlayerTID);
    
    bool Success;
    int Tries;
    int TID;
    
    while (true)
    {
        Delay(35 * 45);
        
        Success = false;
        Tries = 10;
        TID = UniqueTID();
        
        while (Tries-- > 0)
        {
            Success = Spawn("DRPGSinstormRift", GetActorX(0) + RandomFixed(-1024, 1024), GetActorY(0) + RandomFixed(-1024, 1024), GetActorFloorZ(0), TID, 0);
            
            if (Success)
                Success = CheckSight(0, TID, 0);
            
            if (!Success)
                Thing_Remove(TID);
            else
                break;
        }
    }
}

// DRLA Event - Feeding Frenzy ------------------------------------------------

NamedScript void FeedingFrenzyEvent()
{
    bool Spawned;
    bool Spotted;
    int TID;
    int BossType;
    int Index;
    Position *ChosenPosition;
    
    SetMusic("");
    
    for (int i = 0; i < 65536; i++)
    {
        Light_Stop(i);
        Light_Glow(i, 64, 96, 35 * 30);
        Sector_SetColor(i, 255, 64, 64, 128);
    }
    
    // Replace them with corpses
    for (int i = 1; i < MonsterID; i++)
    {
        if (!Monsters[i].Init)
            continue;
        
        Monsters[i].ReplaceActor = "DRPGRLGibbedStuff";
    }
    
    WaitingForReplacements = false;
    
    Delay(35 * 10);
    
    // Spawn the reward item
    str const RewardItems[8] =
    {
        // Monster-wielded weapons
        "RLRevenantsLauncherPickup", // Revenant
        "RLPlasmaRedirectionCannonPickup", // Nightmare Cyberdemon
        
        // Demonic weapons
        "RLHellsReignPickup",
        "RLUnmakerPickup",
        // "RLMonsterFrisbee",
        "RLDeathsGaze",
        "RLSoulstormRifle",
        // "RLMortalyzer",
        // "RLDreadshotMortar",
        
        // Demonic armor set
        "RLDemonicCarapaceArmorPickup",
        "RLDemonicBootsPickup"
    };
    
    str RewardItem = RewardItems[Random(0, 5)];
    DynamicLootGenerator(RewardItem, 1);
    
    // Ambient Music
    SetMusic("FeedThem");
    
    int ActiveHungry = 0;
    int LastActiveHungry = 0;
    int KilledHungry = 0;
    
    for (int i = 0; i < MAX_PLAYERS; i++)
        FeedingFrenzyVisualHorror(i);
    
    int SpotTID;
    bool VisibleToPlayer;
    
    while (ThingCountName("RLArmageddonLostSoulRPG", 0) < 10)
    {
        Position SpawnPosition = ((Position *)CurrentLevel->MonsterPositions.Data)[Random(0, CurrentLevel->MonsterPositions.Position)];
        
        SpotTID = UniqueTID();
        
        Spawn("MapSpot", SpawnPosition.X, SpawnPosition.Y, SpawnPosition.Z, SpotTID, SpawnPosition.Angle * 256);
        
        VisibleToPlayer = false;
        
        for (int i = 0; i < MAX_PLAYERS; i++)
            if (CheckSight(SpotTID, Players(i).TID, CSF_NOBLOCKALL))
                VisibleToPlayer = true;
        
        Thing_Remove(SpotTID);
        
        if (VisibleToPlayer)
            continue;
        
        Spawn("RLArmageddonLostSoulRPG", SpawnPosition.X, SpawnPosition.Y, SpawnPosition.Z, 0, SpawnPosition.Angle * 256);
    }
    
    while (true)
    {
        ActiveHungry = ThingCountName("RLArmageddonLostSoulRPG", 0);
        
        if (ActiveHungry < LastActiveHungry)
            KilledHungry += (LastActiveHungry - ActiveHungry) * GameSkill();
        
        for (int i = 0; i < CurrentLevel->MonsterPositions.Position; i++)
        {
            if (ThingCountName("RLArmageddonLostSoulRPG", 0) >= 100)
                break;
            
            if (Random(0, 1200) > KilledHungry)
                continue;
            
            Position SpawnPosition = ((Position *)CurrentLevel->MonsterPositions.Data)[i];
            
            SpotTID = UniqueTID();
            
            Spawn("MapSpot", SpawnPosition.X, SpawnPosition.Y, SpawnPosition.Z, SpotTID, SpawnPosition.Angle * 256);
            
            VisibleToPlayer = false;
            
            for (int i = 0; i < MAX_PLAYERS; i++)
                if (CheckSight(SpotTID, Players(i).TID, CSF_NOBLOCKALL))
                    VisibleToPlayer = true;
            
            Thing_Remove(SpotTID);
            
            if (VisibleToPlayer)
                continue;
            
            Spawn("RLArmageddonLostSoulRPG", SpawnPosition.X, SpawnPosition.Y, SpawnPosition.Z, 0, SpawnPosition.Angle * 256);
        }
        
        Delay(35 * 5);
        
        LastActiveHungry = ActiveHungry;
    }
}

NamedScript void FeedingFrenzyVisualHorror(int PlayerID)
{
    SetActivator(0, AAPTR_PLAYER1 << PlayerID);
    
    Start:
    
    NoiseAlert(0, 0);
    
    SetHudSize(640, 480, true);
    SetFont(StrParam("P%iVIEW", PlayerID + 1));
    
    HudMessage("A");
    EndHudMessage(HUDMSG_PLAIN | HUDMSG_ALPHA | HUDMSG_LAYER_UNDERHUD | HUDMSG_NOTWITHFULLMAP, CONFUSION_ID, "Untranslated",
               320 + Random(-8, 8),
               240 + Random(-8, 8),
               0.029, 0.25);
    
    Delay(1);
    goto Start;
}

// DRLA Event - Whispers of Darkness ------------------------------------------

NamedScript void WhispersofDarknessEvent()
{
    bool Spawned;
    bool Spotted;
    int TID;
    int BossType;
    int Index;
    int MonsterIndex;
    Position *ChosenPosition;
    
    // Ambient Music
    SetMusic("Overmind", 0);
    
    // Sector Lighting
    for (int i = 0; i < 65536; i++)
    {
        Sector_SetColor(i, 255, 0, 0, 255);
        Light_Glow(i, 160, 192, 30);
    }
    
    // Replace them with nothing
    for (int i = 1; i < MonsterID; i++)
    {
        if (!Monsters[i].Init)
            continue;
        
        Monsters[i].ReplaceActor = "None";
    }
    
    WaitingForReplacements = false;
    Delay(1); // Monsters disappear!
    
    // Shuffle positions
    for (int i = 0; i < CurrentLevel->MonsterPositions.Position; i++)
    {
        int X = Random(0, CurrentLevel->MonsterPositions.Position - 1);
        Position TempPosition;
        
        TempPosition = ((Position *)CurrentLevel->MonsterPositions.Data)[i];
        ((Position *)CurrentLevel->MonsterPositions.Data)[i] = ((Position *)CurrentLevel->MonsterPositions.Data)[X];
        ((Position *)CurrentLevel->MonsterPositions.Data)[X] = TempPosition;
    }
    
    // Spawning
    while (!Spawned)
    {
        TID = UniqueTID();
        ChosenPosition = &((Position *)CurrentLevel->MonsterPositions.Data)[Index];
        Spawned = Spawn("RLCyberneticSpiderMastermindRPG", ChosenPosition->X, ChosenPosition->Y, ChosenPosition->Z, TID, ChosenPosition->Angle * 256);
        
        if (GetCVar("drpg_debug"))
            Log("\CdDEBUG: Iterating for Spawn Point... (Class %S, Index %d, Position %.2k/%.2k/%.2k", CurrentLevel->MegabossActor->Actor, Index, ChosenPosition->X, ChosenPosition->Y, ChosenPosition->Z);
        
        // Successful spawn
        if (Spawned)
        {
            SpawnForced("TeleportFog", ChosenPosition->X, ChosenPosition->Y, ChosenPosition->Z, 0, 0);
            Delay(1);
            MonsterIndex = GetMonsterID(TID);
            
            Monsters[MonsterIndex].Level += ((250 / MAX_PLAYERS) * PlayerCount());
            Monsters[MonsterIndex].Level /= 4;
            Monsters[MonsterIndex].NeedReinit = true;
            
            // Shadow Aura
            for (int i = 0; i < AURA_MAX; i++)
                Monsters[MonsterIndex].Aura.Type[i].Active = true;

            if (GetCVar("drpg_debug"))
                Log("\CdDEBUG: \CgShadow Overmind successfully spawned");
        }
        
        Index++;
        if (Index >= CurrentLevel->MonsterPositions.Position)
            Index = 0;
        
        Delay(1);
    }
    
    AmbientSound("spiderovermind/whisper", 127);
    
    // Horrible, horrible things.
    WhispersofDarknessBackgroundCreepiness();
    
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!PlayerInGame(i))
            continue;
        
        WhispersofDarknessMindRaper(i);
        WhispersofDarknessVisionIntensifier(i);
    }
    
    // Loop
    while (true)
    {
        if (!Spotted)
        {
            // Check LOS
            for (int i = 0; i < MAX_PLAYERS; i++)
                if (CheckSight(TID, Players(i).TID, 0))
                {
                    SetMusic("Overmind", 1);
                    Spotted = true;
                    break;
                }
        }
        
        // Defeated
        if (GetActorProperty(TID, APROP_Health) <= 0)
        {
            SetMusic("");
            CurrentLevel->EventCompleted = true;
            return;
        }
        
        Delay(1);
    }
}

NamedScript void WhispersofDarknessBackgroundCreepiness()
{
    AmbientSound("misc/gzdoom.exe", 127);
    Delay(1);
    while (true)
    {
        if ((Timer() % 5565) == 0)
            AmbientSound("misc/gzdoom.exe", 127);
        
        Delay(1);
    }
}

NamedScript void WhispersofDarknessMindRaper(int PlayerID)
{
    Start:
    
    SetActivator(0, AAPTR_PLAYER1 << PlayerID);
    
    if (CurrentLevel->EventCompleted)
        return;
    
    if (GetActorProperty(0, APROP_Health) <= 0)
    {
        Delay(35);
        goto Start;
    }
    
    if (GetActorProperty(0, APROP_Health) == 1)
    {
        SetPlayerProperty(0, false, PROP_BUDDHA);
        SetPlayerProperty(0, false, PROP_INVULNERABILITY);
        Thing_Damage2(0, 6666666, "Mindrape");
        ActivatorSound("spiderovermind/mindpulse", 127);
    }
    else
        SetActorProperty(0, APROP_Health, GetActorProperty(0, APROP_Health) - 1);
    
    Delay(Random(35, 70));
    goto Start;
}

NamedScript void WhispersofDarknessVisionIntensifier(int PlayerID)
{
    int MindblastTime = 1;
    int MindblastTimeMax = 1;
    bool ShowOvermind = false;
    Position SpawnPosition;
    int SpotTID;
    bool VisibleToPlayer;
    
    Start:
    
    SetActivator(0, AAPTR_PLAYER1 << PlayerID);
    
    if (CurrentLevel->EventCompleted)
        return;
    
    if (GetActorProperty(0, APROP_Health) <= 0)
    {
        Delay(35);
        goto Start;
    }
    
    if (--MindblastTime == 0)
    {
        Delay(Random(35 * 20, 35 * 60));
        
        MindblastTime = Random(35 * 2, 35 * 4);
        MindblastTimeMax = MindblastTime;
    }
    
    if (MindblastTime == MindblastTimeMax)
    {
        if (!Random(0, 7))
            StatusEffect(SE_BLIND, Random(30, 60), Random(1, 5));
        
        if (!Random(0, 7))
            StatusEffect(SE_CONFUSION, Random(30, 60), Random(1, 5));
        
        if (!Random(0, 3))
        {
            for (int i = 0; i < CurrentLevel->MonsterPositions.Position; i++)
            {
                SpawnPosition = ((Position *)CurrentLevel->MonsterPositions.Data)[i];
                
                if (!Random(0, 7))
                    continue;
                
                Spawn("RLFakeShadowCredits", SpawnPosition.X, SpawnPosition.Y, SpawnPosition.Z, 0, SpawnPosition.Angle * 256);
            }
        }
        
        if (!Random(0, 3))
        {
            ShowOvermind = true;
            for (int i = 0; i < CurrentLevel->MonsterPositions.Position; i++)
            {
                SpawnPosition = ((Position *)CurrentLevel->MonsterPositions.Data)[i];
                
                SpotTID = UniqueTID();
                
                Spawn("MapSpot", SpawnPosition.X, SpawnPosition.Y, SpawnPosition.Z, SpotTID, SpawnPosition.Angle * 256);
                
                VisibleToPlayer = false;
                
                for (int i = 0; i < MAX_PLAYERS; i++)
                    if (CheckSight(SpotTID, Players(i).TID, CSF_NOBLOCKALL))
                        VisibleToPlayer = true;
                
                Thing_Remove(SpotTID);
                
                if (!VisibleToPlayer)
                    continue;
                
                if (!Random(0, 7))
                {
                    if (!Random(0, 3))
                        Spawn("RLFakeShadowOvermind", SpawnPosition.X, SpawnPosition.Y, SpawnPosition.Z, 0, SpawnPosition.Angle * 256);
                    
                    continue;
                }
                
                if (!Spawn("RLCyberneticArachnotronRPG", SpawnPosition.X, SpawnPosition.Y, SpawnPosition.Z, 0, SpawnPosition.Angle * 256))
                    Spawn("RLCyberneticImpRPG", SpawnPosition.X, SpawnPosition.Y, SpawnPosition.Z, 0, SpawnPosition.Angle * 256);
            }
        }
        
        if (ShowOvermind)
            LocalAmbientSound("spiderovermind/mindpulse", 127);
        else
            LocalAmbientSound("spiderovermind/whisper", 32);
    }
    
    SetHudSize(640, 480, true);
    
    SetFont(StrParam("P%iVIEW", PlayerID + 1));
    HudMessage("A");
    EndHudMessage(HUDMSG_PLAIN | HUDMSG_ALPHA | HUDMSG_LAYER_UNDERHUD | HUDMSG_NOTWITHFULLMAP, CONFUSION_ID + 1, "Untranslated",
               320 + Random(-8, 8),
               240 + Random(-8, 8),
               0.029, (((fixed)MindblastTime / (fixed)MindblastTimeMax) * 0.66));
    
    if (ShowOvermind)
    {
        SetFont("OVERMIND");
        HudMessage("A");
        EndHudMessage(HUDMSG_PLAIN | HUDMSG_ALPHA | HUDMSG_LAYER_UNDERHUD | HUDMSG_NOTWITHFULLMAP, CONFUSION_ID, "Untranslated",
                   320 + Random(-4, 4),
                   240 + Random(-4, 4),
                   0.029, (((fixed)MindblastTime / (fixed)MindblastTimeMax) * 0.66));
    }
    
    Delay(1);
    
    goto Start;
}

// Bonus Events ---------------------------------------------------------------

NamedScript void RainbowEvent()
{
    bool AzureCliffs = !Random(0, 3);
    CurrentLevel->EventCompleted = true; // Disappears when we leave
    
    fixed Angle = 0;
    int Red, Green, Blue;
    
    for (int i = 0; i < 65536; i++)
        Light_ChangeToValue(i, 176);
    
    Delay(1);
    
    Start:
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!PlayerInGame(i))
            continue;
        SetActivator(Players(i).TID);
        
        if (AzureCliffs)
        {
            if (GetActorPowerupTics(0, "PowerInvulnerable") > 0)
                LocalSetMusic("RAINBO2I");
            else
                LocalSetMusic("RAINBOW2");
        }
        else
            LocalSetMusic("RAINBOWS");
    }
    
    Angle -= 1.0 / 350;
    if (Angle < 0.0) Angle = 1.0 + Angle;
    Red = 128 + (Sin(Angle) * 128.0);
    Green = 128 + (Sin(Angle + 0.33) * 128.0);
    Blue = 128 + (Sin(Angle + 0.67) * 128.0);
    
    for (int i = 0; i < 65536; Sector_SetColor(i++, Red, Green, Blue, 128));
    
    Delay(1);
    goto Start;
}

// WadSmoosh --------------------------------------------------
NamedScript void InitWadSmoosh()
{
    str CvarNames[MAX_WSMAPPACKS] =
    {
        "drpg_ws_doom1",
        "drpg_ws_doom2",
        "drpg_ws_master",
        "drpg_ws_nerve",
        "drpg_ws_plut",
        "drpg_ws_tnt"
    };
    
    str LumpNames[MAX_WSMAPPACKS] = 
    {
        "E1M1",
        "MAP01",
        "ML_MAP01",
        "NERVE_MAP01",
        "PLUT_MAP01",
        "TNT_MAP01"
    };
    int i;
    bool BlankStart;

    Delay(10); //Give a chance for data to load
    
    if (CurrentLevel->LumpName == "TITLEMAP") //don't need to run on title screen
        return;
    
    if (!WadSmoosh || WadSmooshInitialized) return; //let's be safe
    LogMessage("\CdStarting Wad Smoosh Initialization", LOG_DEBUG);
    
    for (i = 0; i < MAX_WSMAPPACKS; i++)
    {
        MapPackActive[i] = GetCVar(CvarNames[i]); //find which iwads user says they have
    };

    bool StartedOnMap = KnownLevels->Position > 1;
    
    if (!StartedOnMap) //we started in the outpost and map arrays are empty - lets add one
    {
        for (i = 0; i < MAX_WSMAPPACKS; i++)
        {
            if (MapPackActive[i])
            {
                LevelInfo *NewMap = &((LevelInfo *)KnownLevels->Data)[KnownLevels->Position++];
                NewMap->LumpName = LumpNames[i];
                NewMap->NiceName = "Unknown Area";
                NewMap->LevelNum = 0;
                NewMap->SecretMap = 0;
                NewMap->UACBase = false;
                NewMap->UACArena = false;
                NewMap->SecretMap = false;
                NewMap->Completed = false;
                NewMap->NeedsRealInfo = true;
                LogMessage(StrParam("Loaded on Outpost! - Added Lump %S", ((LevelInfo *)KnownLevels->Data)[1].LumpName, KnownLevels), LOG_DEBUG);
                break;
            }
        }
        if (i == MAX_WSMAPPACKS)
        {
            LogMessage(StrParam("\CdERROR: \C-WadSmoosh loaded with no IWADS enabled!"), LOG_ERROR);
            return;
        }
    }
    str Lump = ((LevelInfo *)WSMapPacks[WS_DOOM1].Data)[1].LumpName;

    
    if (Lump != "E1M1") //we didn't start with doom 1 or started on outpost with add unknown map not set to doom 1
    {
        LogMessage(StrParam("\Cgnot E1M1!!!\C- - Lump is: %S", Lump), LOG_DEBUG);
        
        DynamicArray Temp;

        for (i = 1; i < MAX_WSMAPPACKS; i++)
        {
            if (Lump == LumpNames[i])    //get which iwad was loaded
                break;
        }

        LogMessage("Swapping Array positions", LOG_DEBUG);
        Temp = WSMapPacks[i];   //store current data of correct location
        LogMessage(StrParam("Swapping %p with %p",&WSMapPacks[i], &WSMapPacks[WS_DOOM1]), LOG_DEBUG);
        WSMapPacks[i] = WSMapPacks[WS_DOOM1]; //move map data to correct position
        WSMapPacks[WS_DOOM1] = Temp; //place replaced data in the outdated doom 1 slot
       
        KnownLevels = &WSMapPacks[i];   //update pointer to the new location
    }

    //we need to do this again to update i, just in case
    //it's possible to get here if data is already initialised and this would
    //mess up the current map pack pointer if we didn't recheck
    Lump = ((LevelInfo *)KnownLevels->Data)[1].LumpName;
    for (i = 0; i < MAX_WSMAPPACKS; i++)
    {
        if (Lump == LumpNames[i])    //get which iwad was loaded
            break;
    }
    
    for (int j = 0; j < MAX_WSMAPPACKS; j++) //loop through all map packs
    {
        if (j != i && MapPackActive[j]) //the "current" pack already has data so ignore it, also make sure the iwad is marked active
        {
            KnownLevels = &WSMapPacks[j]; //move pointer for this operation
            if (KnownLevels->Data == NULL)
                ArrayCreate(KnownLevels, "Levels", 32, sizeof(LevelInfo)); //allocate memory                
            
            //we need to add the outpost to each array
            LevelInfo *OutpostMap = &((LevelInfo *)KnownLevels->Data)[KnownLevels->Position++];
            OutpostMap->LevelNum = 0;
            OutpostMap->LumpName = "OUTPOST";
            OutpostMap->NiceName = "UAC Outpost";
            OutpostMap->Completed = true;
            OutpostMap->UACBase = true;
            OutpostMap->UACArena = false;
            OutpostMap->NeedsRealInfo = false;

            //we add our maps manually because AddUnknownMap is expected to run on outpost load
            LevelInfo *NewMap = &((LevelInfo *)KnownLevels->Data)[KnownLevels->Position++];
            NewMap->LumpName = LumpNames[j];
            NewMap->NiceName = "Unknown Area";
            NewMap->LevelNum = 0;
            NewMap->SecretMap = 0;
            NewMap->UACBase = false;
            NewMap->UACArena = false;
            NewMap->SecretMap = false;
            NewMap->Completed = false;
            NewMap->NeedsRealInfo = true;
            LogMessage(StrParam("Added Lump %S to Array %p", ((LevelInfo *)KnownLevels->Data)[1].LumpName, KnownLevels), LOG_DEBUG);
        }
    }
    
    KnownLevels = &WSMapPacks[i]; //move pointer back to current mappack
    CurrentLevel = FindLevelInfo(); //the CurrentLevel pointer is invalidated and needs to be reset
    Player.SelectedMapPack = i;
    
    WadSmooshInitialized = true;
}
