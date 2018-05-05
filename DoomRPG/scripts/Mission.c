#include "Defs.h"

#include <stdlib.h>

#include "ItemData.h"
#include "Mission.h"
#include "Map.h"
#include "Monsters.h"
#include "Outpost.h"
#include "RPG.h"
#include "Stats.h"
#include "Utils.h"

MissionInfo Missions[MAX_DIFFICULTIES][MAX_MISSIONS];

str const MissionDifficulties[MAX_DIFFICULTIES] =
{
    "\CdVery Easy",
    "\CdEasy",
    "\CjNormal",
    "\CaHard",
    "\CaVery Hard",
    "\CaNightmare",
    "\CgInsane",
    "\CgImpossible",
    "\CgSuicide",
};

str const MissionTypes[MT_MAX] =
{
    "\CdCollection",
    "\CaKill",
    "\CaKill Auras",
    "\CaKill Reinforcements",
    "\CgAssassination",
    "\CkFind Secrets",
    "\CnFind Items",
    "\CtAchieve Combo",
};

bool RPGMap MissionInit;

NamedScript void PopulateMissions()
{
    // Wait for the Rewards List to finish processing
    while (!RewardsInit) Delay(1);

    for (int i = 0; i < MAX_DIFFICULTIES; i++)
    {
        auto MissionInfo MissionsSorted[MAX_MISSIONS];

        // Create new missions
        for (int j = 0; j < MAX_MISSIONS; j++)
            MissionsSorted[j] = CreateMission(i);

        // Sort the missions by type
        qsort(MissionsSorted, MAX_MISSIONS, sizeof(MissionInfo), MissionTypeSort);

        // Add the sorted missions into the missions array
        for (int j = 0; j < MAX_MISSIONS; j++)
            Missions[i][j] = MissionsSorted[j];

        Delay(1);
    }

    MissionInit = true;
}

NamedScript void InitMission()
{
    // If you aren't on a mission or you're in the Outpost, return
    if (!Player.Mission.Active || CurrentLevel->UACBase) return;

    // Kill Reinforcements Mission
    if (Player.Mission.Type == MT_REINFORCEMENTS)
        MonsterTransport(CalculateAverageDifficulty(), 10 - Player.Mission.Difficulty, 1024);

    // Assassination Mission
    if (Player.Mission.Type == MT_ASSASSINATION)
    {
        // Need to wait until all monsters have been initialized
        Delay(35);

        if (MonsterID <= 1) return; // No monsters, no assassination.

        auto DynamicArray PotentialTargets;
        ArrayCreate(&PotentialTargets, "Targets", 32, sizeof(LevelInfo));

        for (int i = 1; i < MonsterID; i++)
        {
            if (!Monsters[i].Init) // Skip removed monsters
                continue;

            str ActorToCheck;

            for (int j = 0; j < MAX_PLAYERS; j++)
            {
                if (!PlayerInGame(j)) continue;

                ActorToCheck = StrParam("DRPG%S", Players(j).Mission.Monster->Actor);

                if (CompatMode == COMPAT_DRLA)
                    ActorToCheck = Players(j).Mission.Monster->Actor;

                if (CompatMode == COMPAT_LEGENDOOM)
                {
                    ActorToCheck = StrParam("%S", Players(j).Mission.Monster->Actor);
                    if (ActorToCheck == "LDFatso") ActorToCheck = "LDMancubus";
                }
                
                LogMessage(StrParam("Checking: %S - Looking For: %S",Monsters[i].Actor, ActorToCheck), LOG_DEBUG);
                
                if (StartsWith(Monsters[i].Actor, ActorToCheck, true))
                {
                    if (PotentialTargets.Position == PotentialTargets.Size)
                        ArrayResize(&PotentialTargets);

                    ((int *)PotentialTargets.Data)[PotentialTargets.Position++] = i;
                }

                if (!(i % 1000)) Delay(1);
            }
        }

        if (PotentialTargets.Position)
        {
            int Chosen = ((int *)PotentialTargets.Data)[Random(0, PotentialTargets.Position - 1)];
            int LevelMod = Player.Mission.Difficulty * Player.Level + Random(0, 100);

            Monsters[Chosen].Target = PlayerNumber() + 1;
            Monsters[Chosen].Level += LevelMod;
            Monsters[Chosen].Level /= 4; // Shadow Auras always double their level, and double their stats on top
            Monsters[Chosen].NeedReinit = true;

            // Shadow Aura
            for (int i = 0; i < AURA_MAX; i++)
                Monsters[Chosen].Aura.Type[i].Active = true;

            // EVIL LAUGH OF WARNING
            if (Monsters[Chosen].Threat >= 10)
                LocalAmbientSound("mission/gottarget2", 127);
            else
                LocalAmbientSound("mission/gottarget1", 127);

            if (GetCVar("drpg_debug"))
                Log("\CdDEBUG: \C-Mission Target Chosen: \Ca%d", Chosen);
        }

        ArrayDestroy(&PotentialTargets);
    }
}

NamedScript void MissionDeathCheck(int Killer, MonsterStatsPtr Stats)
{
    if ((GetCVar("drpg_mission_monsterkillscount") ? Killer >= -1 : Killer > -1) && !CurrentLevel->UACBase && !GetActorProperty(0, APROP_Friendly))
    {
        // Kill Mission
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            // Determine actor name
            str Actor = GetActorClass(0);
            str NeededActor = GetMissionMonsterActor(Players(i).Mission.Monster->Actor);
            int Match;

            // Player is not in-game
            if (!PlayerInGame(i)) continue;

            // Check to see if we match
            Match = StrCmp(Actor, NeededActor);

            // Perform actor -> needed actor match checking
            if (Match == 0)
                if (Players(i).Mission.Active && Players(i).Mission.Type == MT_KILL)
                    Players(i).Mission.Current++;
        }

        // Kill Auras Mission
        if (MonsterHasAura(Stats))
        {
            for (int i = 0; i < MAX_PLAYERS; i++)
            {
                // Player is not in-game
                if (!PlayerInGame(i)) continue;

                if (Players(i).Mission.Active && Players(i).Mission.Type == MT_KILLAURAS)
                    Players(i).Mission.Current++;
            }
        }

        // Kill Reinforcements Mission
        if (Stats->Reinforcement)
        {
            for (int i = 0; i < MAX_PLAYERS; i++)
            {
                // Player is not in-game
                if (!PlayerInGame(i)) continue;

                if (Players(i).Mission.Active && Players(i).Mission.Type == MT_REINFORCEMENTS)
                    Players(i).Mission.Current++;
            }
        }

        // Assassination Mission
        if (Stats->Target)
        {
            for (int i = 0; i < MAX_PLAYERS; i++)
            {
                // Player is not in-game
                if (!PlayerInGame(i)) continue;

                if (Players(i).Mission.Active && Players(i).Mission.Type == MT_ASSASSINATION)
                    Players(i).Mission.Current++;
            }
        }
    }
}

MissionInfo CreateMission(int Difficulty)
{
    int Type = Random(0, MT_MAX - 1);
    int NumPlayers = 0;
    long int RewardXP = 0;
    long int RewardRank = 0;
    int RewardCredits = 0;
    int RewardModules = 0;
    ItemInfoPtr RewardItem;
    MissionInfo Mission;

    if (CompatMode == COMPAT_DRLA && Difficulty == MAX_DIFFICULTIES - 1 && Random(0, 100) < 15)
    {
        RewardItem = GetRewardItem(Difficulty + 1);
    }
    else
    {
        RewardItem = GetRewardItem(Difficulty);
    }

    // Calculate the rewards based on all Player's average Level and Rank
    // XPTable[Player.Level + 1] / (3 + MAX_DIFFICULTIES - Difficulty) + Random(0, XPTable[Player.Level + 1] / GameSkill());
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        long int XPNext;
        long int RankNext;

        if (!PlayerInGame(i)) continue;

        // Prevent overflows due to trying to check for a null table index
        if (Players(i).Level >= MAX_LEVEL)
            XPNext = XPTable[MAX_LEVEL - 1];
        else
            XPNext = XPTable[Players(i).Level];
        if (Players(i).RankLevel >= MAX_RANK)
            RankNext = RankTable[MAX_RANK - 1];
        else
            RankNext = RankTable[Players(i).RankLevel];

        RewardXP += XPNext * 1000 / (100000 - (Difficulty * 10000));
        RewardRank += RankNext * 1000 / (100000 - (Difficulty * 10000));
        RewardCredits += Random(100 * (Difficulty + 1) * (Players(i).Level + 1), 100 * (Difficulty + 1) * (Players(i).Level + 1) * 2);
        RewardModules += Random(10 * (Difficulty + 1) * (Players(i).Level + 1), 10 * (Difficulty + 1) * (Players(i).Level + 1) * 2);

        NumPlayers++;
    }

    // Average the rewards
    RewardXP /= NumPlayers;
    RewardRank /= NumPlayers;
    RewardCredits /= NumPlayers;
    RewardModules /= NumPlayers;

    // Assign info to the struct
    Mission.Active = false;
    Mission.Type = Type;
    Mission.Difficulty = Difficulty;
    Mission.RewardXP = RewardXP;
    Mission.RewardRank = RewardRank;
    Mission.RewardCredits = RewardCredits;
    Mission.RewardModules = RewardModules;
    Mission.RewardItem = RewardItem;

    // Collection Mission
    if (Type == MT_COLLECT)
    {
        // Generate Item and Amount
        int ItemIndex = Random(0, MAX_LOOT - 1);
        int Amount = 10 + (Difficulty * 5);
        ItemInfoPtr ItemPtr = &ItemData[7][ItemIndex];

        // Pass data to the Mission struct
        Mission.Item = ItemPtr;
        Mission.Amount = Amount;
    }

    // Kill Mission
    if (Type == MT_KILL)
        GetTargetMonster(&Mission);

    // Kill Auras and Kill Reinforcements Missions
    if (Type == MT_KILLAURAS || Type == MT_REINFORCEMENTS)
        Mission.Amount = 10 + (10 * (Difficulty + 1));

    // Assassination Mission
    if (Type == MT_ASSASSINATION)
        GetTargetMonster(&Mission);

    // Find Secrets Mission
    if (Type == MT_SECRETS)
        Mission.Amount = 3 + (3 * (Difficulty + 1));

    // Find Items Mission
    if (Type == MT_ITEMS)
        Mission.Amount = 30 + (30 * (Difficulty + 1));

    // Combo Mission
    if (Type == MT_COMBO)
        Mission.Amount = (Difficulty + 2) * 10;

    return Mission;
}

void CreateMissionAt(int Difficulty, int Index)
{
    // Create new mission
    Missions[Difficulty][Index] = CreateMission(Difficulty);

    // Resort the missions in this category
    auto MissionInfo MissionsSorted[MAX_MISSIONS];
    for (int i = 0; i < MAX_MISSIONS; i++)
        MissionsSorted[i] = Missions[Difficulty][i];
    qsort(MissionsSorted, MAX_MISSIONS, sizeof(MissionInfo), MissionTypeSort);
    for (int i = 0; i < MAX_MISSIONS; i++)
        Missions[Difficulty][i] = MissionsSorted[i];
}

void CheckMission()
{
    bool Complete = false;

    // If you currently have no mission, return
    if (!Player.Mission.Active) return;

    // Collection Mission
    if (Player.Mission.Type == MT_COLLECT)
    {
        Player.Mission.Current = CheckInventory(Player.Mission.Item->Actor);

        // Complete
        if (Player.Mission.Current >= Player.Mission.Amount)
        {
            TakeInventory(Player.Mission.Item->Actor, Player.Mission.Amount);
            Complete = true;
        }
    }

    // Generic Checking
    if (Player.Mission.Type == MT_KILL || Player.Mission.Type == MT_KILLAURAS ||
        Player.Mission.Type == MT_REINFORCEMENTS || Player.Mission.Type == MT_SECRETS ||
        Player.Mission.Type == MT_ITEMS)
        if (Player.Mission.Current >= Player.Mission.Amount)
            Complete = true;

    // Assassination Mission
    if (Player.Mission.Type == MT_ASSASSINATION)
        if (Player.Mission.Current >= 1)
            Complete = true;

    // Combo Mission
    if (Player.Mission.Type == MT_COMBO && !CurrentLevel->UACBase)
    {
        if (Player.ComboTimer == COMBO_STOP)
            if (Player.Combo > Player.Mission.Current)
                Player.Mission.Current = Player.Combo;

        // Complete
        if (Player.Mission.Current >= Player.Mission.Amount)
            Complete = true;
    }

    // Mission Complete!
    if (Complete)
    {
        // Message
        ActivatorSound("mission/complete", 127);
        SetFont("BIGFONT");
        SetHudSize(640, 480, false);
        HudMessage("Mission Complete!\n\n\Cj+%ld XP\n\Ck+%ld Rank\n\Cf+%d Credits\n\Cd+%d Modules\n\n\CiItem: \Cj%S",
                   Player.Mission.RewardXP, Player.Mission.RewardRank, Player.Mission.RewardCredits, Player.Mission.RewardModules, Player.Mission.RewardItem->Name);
        EndHudMessage(HUDMSG_FADEOUT, MISSION_ID, "Green", 320.4, 240.0, 3.0, 2.0);

        // Reward - XP/Rank
        Player.XP += Player.Mission.RewardXP;
        Player.Rank += Player.Mission.RewardRank;

        // Credits
        GiveInventory("DRPGCredits", Player.Mission.RewardCredits);

        // Modules
        GiveInventory("DRPGModule", Player.Mission.RewardModules);

        // Spawn Item and try to pick it up
        SpawnForced(Player.Mission.RewardItem->Actor, GetActorX(0), GetActorY(0), GetActorZ(0), 0, 0);
        SetActorVelocity(Player.TID, 0.01, 0.01, 0, true, false);

        // Payout
        Player.Payout.MissionsCompleted++;

        // Clear the Mission
        ClearMission();

        // Reset the completion flag
        Complete = false;
    }
}

void ClearMission()
{
    Player.Mission.Active = false;
    Player.Mission.Difficulty = 0;

    Player.Mission.RewardXP = 0;
    Player.Mission.RewardRank = 0;
    Player.Mission.RewardCredits = 0;
    Player.Mission.RewardItem = NULL;

    Player.Mission.Item = NULL;
    Player.Mission.Monster = NULL;
    Player.Mission.Current = 0;
    Player.Mission.Amount = 0;
}

void GetTargetMonster(MissionInfo *Mission)
{
    bool DRLA = (CompatMode == COMPAT_DRLA);
    int Amount;
    MonsterInfoPtr PotentialMonsters[MAX_TEMP_MONSTERS];
    int NumPotentialMonsters;
    int MonsterDataAmount;

	if (ColourfulHell)
        MonsterDataAmount = MAX_DEF_MONSTERS_CH;
    else if (DRLA)
        MonsterDataAmount = MAX_DEF_MONSTERS_DRLA;
	else
		MonsterDataAmount = MAX_DEF_MONSTERS;

    // Generate a list based on monsters' threat levels.
    for (int i = 0; i < MonsterDataAmount; i++)
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

        int TestDifficulty = TempMonster->Difficulty + (10 * TempMonster->ThreatLevel);
        int TestAmount = (30 + (320 * Mission->Difficulty)) / TestDifficulty;

        if (Mission->Type != MT_KILL)
        {
            if (TempMonster->Difficulty > ((Mission->Difficulty + 1) * 11) - 11 &&
                TempMonster->Difficulty < ((Mission->Difficulty + 1) * 11) + 11)
                PotentialMonsters[NumPotentialMonsters++] = TempMonster;
        }
        else
        {
            if (!TempMonster->Boss && TestAmount >= 5 &&
                TestAmount <= 40)
                PotentialMonsters[NumPotentialMonsters++] = TempMonster;
        }
    }

    if (NumPotentialMonsters < 1)
    {
        Log("\CgERROR: \C-GetTargetMonster for %S mission on difficulty %d has no monsters!", Mission->Type == MT_KILL? "kill" : "assassination", Mission->Difficulty + 1);
        Mission->Monster = NULL;
        Mission->Amount = 0;
        return;
    }

    MonsterInfoPtr MonsterPtr = PotentialMonsters[Random(0, NumPotentialMonsters - 1)];

    // Now that we know the monster type, we can calculate an amount
    int EffectiveDifficulty = MonsterPtr->Difficulty + (10 * MonsterPtr->ThreatLevel);
    int BaseAmount = (30 + (320 * Mission->Difficulty)) / EffectiveDifficulty;
    Amount = Random((fixed)BaseAmount * 0.8, (fixed)BaseAmount * 1.2);

    // Insert info into the passed struct
    Mission->Monster = MonsterPtr;
    Mission->Amount = Amount;
}

int CalculateAverageDifficulty()
{
    int NumPlayers;
    int AvgDifficulty;

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!PlayerInGame(i)) continue;

        if (Players(i).Mission.Active)
            AvgDifficulty += Players(i).Mission.Difficulty;

        NumPlayers++;
    }

    AvgDifficulty /= NumPlayers;

    return AvgDifficulty;
}

str GetMissionMonsterActor(str Actor)
{
    if (CompatMode == COMPAT_DRLA || CompatMode == COMPAT_LEGENDOOM)
        return StrParam("%SRPG", Actor);
    else if (CompatMode == COMPAT_EXTRAS)
        return StrParam("DRPG%SExtras", Actor);

    return StrParam("DRPG%S", Actor);
}

int MissionTypeSort(void const *Left, void const *Right)
{
    MissionInfo LeftMission = *((MissionInfo *)Left);
    MissionInfo RightMission = *((MissionInfo *)Right);

    if (LeftMission.Type < RightMission.Type) return -1;
    if (LeftMission.Type > RightMission.Type) return 1;

    return 0;
}
