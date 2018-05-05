#include "RPG.h"

#include "Arena.h"
#include "Map.h"
#include "Mission.h"
#include "Monsters.h"
#include "Namegen.h"
#include "Popoffs.h"
#include "Shield.h"
#include "Stats.h"
#include "Utils.h"

int RPGMap MonsterID = 1;

NoInit MonsterStats RPGMap Monsters[MAX_MONSTERS];

MonsterInfo const MonsterData[MAX_DEF_MONSTERS] =
{
    { "ZombieMan",                          "Former Human",                     1,      0, false, "You hear shuffling footsteps and moans!" },
    { "ShotgunGuy",                         "Former Sergeant",                  2,      0, false, "You hear the sound of shotguns pumping!" },
    { "ChaingunGuy",                        "Former Commando",                  5,      0, false, "Quiet mutters and whirring set you on edge!" },
    { "DoomImp",                            "Imp",                              4,      0, false, "The walls are scratched and flame-scorched!" },
    { "Demon",                              "Demon",                            3,      0, false, "Hungry growls echo around you!" },
    { "Spectre",                            "Spectre",                          3,      0, false, "It's dark, you are likely to be eaten by a... Spectre?" },
    { "LostSoul",                           "Lost Soul",                        3,      0, false, "The sound of many flames echo about!" },
    { "Cacodemon",                          "Cacodemon",                        10,     0, false, "Screeches drown out all other sound!" },
    { "HellKnight",                         "Hell Knight",                      20,     0, false, "A battle cry chants in the distance!" },
    { "BaronOfHell",                        "Baron of Hell",                    30,     0, false, "Seems you've found Hell's nobility!" },
    { "PainElemental",                      "Pain Elemental",                   40,     0, false, "You feel you are being watched!" },
    { "Revenant",                           "Revenant",                         50,     0, false, "Bones clatter all around you!" },
    { "Fatso",                              "Mancubus",                         60,     0, false, "You hear deep, guttural noises!" },
    { "Arachnotron",                        "Arachnotron",                      60,     0, false, "Leg servos squeak and whirr nearby!" },
    { "ArchVile",                           "Arch-Vile",                        70,     0, false, "You hear crackling flames!" },
    { "Cyberdemon",                         "Cyberdemon",                       85,     0, true,  "Suddenly you have a great urge to turn back! You scream in TERROR!" },
    { "SpiderMastermind",                   "Spider Mastermind",                95,     0, true,  "You've stumbled into a hive of the deadliest kind!" }
};

MonsterInfo const MonsterDataLD[MAX_DEF_MONSTERS] =
{
    { "LDZombieman",                          "Former Human",                     1,      0, false, "You hear shuffling footsteps and moans!" },
    { "LDShotgunguy",                         "Former Sergeant",                  2,      0, false, "You hear the sound of shotguns pumping!" },
    { "LDChaingunguy",                        "Former Commando",                  5,      0, false, "Quiet mutters and whirring set you on edge!" },
    { "LDDoomImp",                            "Imp",                              4,      0, false, "The walls are scratched and flame-scorched!" },
    { "LDDemon",                              "Demon",                            3,      0, false, "Hungry growls echo around you!" },
    { "LDSpectre",                            "Spectre",                          3,      0, false, "It's dark, you are likely to be eaten by a... Spectre?" },
    { "LDLostSoul",                           "Lost Soul",                        3,      0, false, "The sound of many flames echo about!" },
    { "LDCacodemon",                          "Cacodemon",                        10,     0, false, "Screeches drown out all other sound!" },
    { "LDHellKnight",                         "Hell Knight",                      20,     0, false, "A battle cry chants in the distance!" },
    { "LDBaronOfHell",                        "Baron of Hell",                    30,     0, false, "Seems you've found Hell's nobility!" },
    { "LDPainElemental",                      "Pain Elemental",                   40,     0, false, "You feel you are being watched!" },
    { "LDRevenant",                           "Revenant",                         50,     0, false, "Bones clatter all around you!" },
    { "LDMancubus",                           "Mancubus",                         60,     0, false, "You hear deep, guttural noises!" },
    { "LDArachnotron",                        "Arachnotron",                      60,     0, false, "Leg servos squeak and whirr nearby!" },
    { "LDArchvile",                           "Arch-Vile",                        70,     0, false, "You hear crackling flames!" },
    { "LDCyberdemon",                         "Cyberdemon",                       85,     0, true,  "Suddenly you have a great urge to turn back! You scream in TERROR!" },
    { "LDSpiderMastermind",                   "Spider Mastermind",                95,     0, true,  "You've stumbled into a hive of the deadliest kind!" }
};

MonsterInfo const MonsterDataDRLA[MAX_DEF_MONSTERS_DRLA] =
{
    // Normal
    { "RLFormerHumanPistol",                "Former Human",                     1,      0, false, "You hear shuffling footsteps and moans!" },
    { "RLFormerHumanBattleRifle",           "Former Human",                     1,      0, false, "You hear shuffling footsteps and moans!" },
    { "RLFormerHumanBattleRifle2",          "Former Human",                     1,      0, false, "You hear shuffling footsteps and moans!" },
    { "RLFormerHumanBattleRifle3",          "Former Human",                     1,      0, false, "You hear shuffling footsteps and moans!" },
    { "RLFormerSergeantShotgun",            "Former Sergeant",                  2,      0, false, "You hear the sound of shotguns pumping!" },
    { "RLFormerSergeantCombatShotgun",      "Former Sergeant",                  2,      0, false, "You hear the sound of shotguns pumping!" },
    { "RLFormerCommandoChaingun",           "Former Commando",                  5,      0, false, "Quiet mutters and whirring set you on edge!" },
    { "RLFormerCommandoBattleRifle",        "Former Commando",                  5,      0, false, "Quiet mutters and magazines being loaded set you on edge!" },
    { "RLFormerCaptainPlasmaRifle",         "Former Captain",                   6,      0, false, "The sound of a plasma rifle priming makes you flinch!" },
    { "RLImp",                              "Imp",                              4,      0, false, "The walls are scratched and flame-scorched!" },
    { "RLDemon",                            "Demon",                            3,      0, false, "Hungry growls echo around you!" },
    { "RLSpectre",                          "Spectre",                          3,      0, false, "It's dark, you are likely to be eaten by a... Spectre?" },
    { "RLLostSoul",                         "Lost Soul",                        3,      0, false, "The sound of many flames echo about!" },
    { "RLCacodemon",                        "Cacodemon",                        10,     0, false, "Screeches drown out all other sound!" },
    { "RLHellKnight",                       "Hell Knight",                      20,     0, false, "A battle cry chants in the distance!" },
    { "RLBaronOfHell",                      "Baron of Hell",                    30,     0, false, "Seems you've found Hell's nobility!" },
    { "RLArachnotron",                      "Arachnotron",                      60,     0, false, "Leg servos squeak and whirr nearby!" },
    { "RLPainElemental",                    "Pain Elemental",                   40,     0, false, "You feel you are being watched!" },
    { "RLRevenant",                         "Revenant",                         50,     0, false, "Bones clatter all around you!" },
    { "RLMancubus",                         "Mancubus",                         60,     0, false, "You hear deep, guttural noises!" },
    { "RLArchvile",                         "Arch-Vile",                        70,     0, false, "You hear crackling flames!" },
    { "RLCyberdemon",                       "Cyberdemon",                       87,     0, true,  "Suddenly you have a great urge to turn back!\nYou scream in TERROR!" },
    { "RLSpiderMastermind",                 "Spider Mastermind",                95,     0, true,  "Looks like this is where all the local invasions are planned!" },
    
    // Special Normal enemies
    { "RLBruiserBrother",                   "Bruiser Brothers",                 85,     0, true,  "The heat in here has gotten intense!" },
    
    // Nightmare
    { "RLEliteHumanCombatPistol",           "Elite Human",                      2,      1, false, "The sound of heavy gunshots alerts you!" },
    { "RLEliteHumanHandcannon",             "Elite Human",                      2,      1, false, "Loud gunshots echo throughout the area!" },
    { "RLEliteHumanBattleRifle",            "Elite Human",                      3,      1, false, "You notice a lot of empty rifle magazines on the ground..." },
    { "RLEliteHumanBattleRifle2",           "Elite Human",                      3,      1, false, "You notice a lot of empty rifle magazines on the ground..." },
    { "RLEliteHumanUzi",                    "Elite Human",                      4,      1, false, "Looks like a whole bunch of recon squads ended up here..." },
    { "RLEliteSergeantDoubleShotgun",       "Elite Sergeant",                   8,      1, false, "Veteran soldiers patrol the halls, seeking you!" },
    { "RLEliteSergeantAssaultShotgun",      "Elite Sergeant",                   8,      1, false, "Veteran soldiers patrol the halls, seeking you!" },
    { "RLEliteSergeantPlasmaShotgun",       "Elite Sergeant",                   8,      1, false, "Veteran soldiers patrol the halls, seeking you!" },
    { "RLEliteCommandoMinigun",             "Elite Commando",                   12,     1, false, "Heavy ammo belts rustle and clank in the dark!" },
//  { "RLEliteCommandoLaserRifle",          "Elite Commando",                   12,     1, false, "Spent laser rifle cells litter the floor..." },
    { "RLEliteCaptainTristarBlaster",       "Elite Captain",                    15,     1, false, "You notice some power cells on the floor,\nused only by the most powerful weapons!" },
//  { "RLEliteCaptainMissileLauncher",      "Elite Captain",                    15,     1, false, "Seems like this area is being used to test some tank buster weapons!" },
//  { "RLEliteCaptainNapalmLauncher",       "Elite Captain",                    15,     1, false, "Empty napalm drums and scorched walls indicate some serious weaponry!" },
    { "RLNightmareImp",                     "Nightmare Imp",                    8,      1, false, "You notice dark nebulous enemies are hunting you down!" },
    { "RLNightmareDemon",                   "Nightmare Demon",                  6,      1, false, "Roars and growls fill the air!" },
    { "RLNightmareSpectre",                 "Nightmare Spectre",                6,      1, false, "You feel you are being stalked by something unseen!" },
    { "RLNightmareLostSoul",                "Nightmare Soul",                   6,      1, false, "You feel like you are being haunted!" },
    { "RLNightmareCacodemon",               "Nightmare Cacodemon",              20,     1, false, "The air is crackling with electricity!" },
    { "RLNightmareHellKnight",              "Nightmare Knight",                 25,     1, false, "Heavy hoofsteps echo through the chambers!" },
    { "RLNightmareBaronOfHell",             "Baron of Nightmares",              45,     1, false, "Commanding roars warn you of danger ahead!" },
    { "RLNightmareArachnotron",             "Nightmare Arachnotron",            65,     1, false, "The charging of heavy plasma guns fills the air with humming!" },
    { "RLNightmarePainElemental",           "Nightmare Elemental",              60,     1, false, "You feel like Hell's invisible influence\nis leaking into this area faster!" },
    { "RLNightmareRevenant",                "Nightmare Revenant",               60,     1, false, "Distant screams echo through the air and your mind!" },
    { "RLNightmareMancubus",                "Nightmare Mancubus",               75,     1, false, "You hear deep growling and the priming of heavy plasma cannons!" },
    { "RLNightmareArchvile",                "Nightmare Arch-Vile",              85,     1, false, "You suddenly break out in a flash sweat!" },
    { "RLNightmareCyberdemon",              "Nightmare Cyberdemon",             100,    1, true,  "Surrounded by hoofsteps and deep laughter,\nyou know you will not leave alive!" },
    { "RLNightmareSpiderMastermind",        "Nightmare Mastermind",             100,    1, true,  "Insane screeching and the pounding of\nmetal legs drowns out all other sound!" },
    
    // Special Nightmare enemies
    { "RLAgonyElemental",                   "Agony Elemental",                  100,    1, true, "You feel the presence of great conduits of the damned!" },
    
    // Cybernetic
    { "RLFormerCyborgBattleRifle",          "Former Human Cyborg",              4,      1, false, "Sounds like a platoon of the UAC's finest got corrupted!" },
//  { "RLFormerCyborgSergeantShotgun",      "Former Human Cyborg Sergeant",     8,      1, false, "You hear whirring legs and the cracks of shotgun fire!" },
//  { "RLFormerCyborgCommandoChaingun",     "Former Human Cyborg Commando",     12,     1, false, "You realise that the UAC still hasn't\nlearned Hell likes to possess soldiers!" },
    { "RLCyberneticImp",                    "Cyber-Imp",                        3,      1, false, "Arcs of electricity and howling creatures greet you!" },
    { "RLCyberneticDemon",                  "Mech-Demon",                       3,      1, false, "Stomping metal feet ring in your ears!" },
    { "RLCyberneticSpectre",                "Mechtre",                          4,      1, false, "You hear the sound of a cloaking device shorting out!" },
    { "RLCyberneticLostSoul",               "Hellmine",                         2,      1, false, "It's quiet...\ntoo quiet..." },
//  { "RLCyberneticCacodemon",              "Cyber-Cacodemon",                  15,     1, false, "The hum of antigrav drives vibrates through your body!" },
    { "RLCyberneticHellKnight",             "Cybruiser",                        30,     1, false, "Clanking footsteps herald Hell's mechanical legions!" },
    { "RLCyberneticBaronOfHell",            "Techno-Lord",                      45,     1, false, "Uneven footsteps marks the arrival of some of Hell's cyberforces!" },
    { "RLCyberneticArachnotron",            "Arachnosentinel",                  70,     1, false, "The walls are covered in bullet holes!" },
    { "RLCyberneticPainElemental",          "Machine Elemental",                60,     1, false, "You hear humming and beeping everywhere!" },
    { "RLCyberneticRevenant",               "Heavy Revenant",                   70,     1, false, "Seems they needed bigger rocket launchers!" },
    { "RLCyberneticMancubus",               "Volacubus",                        75,     1, false, "The smell of burning fuel fills this area!" },
    { "RLCyberneticArchvile",               "Tech-Vile",                        80,     1, false, "You hear electrical zaps and pulsing energy waves!" },
//  { "RLCyberneticCyberdemon",             "Cyberdemon Mk. II",                100,    1, true,  "You hear crashing metal footfalls! You can't stop shaking!" },
    { "RLCyberneticSpiderMastermind",       "Spider Overmind",                  100,    1, true,  "You hear endless whispering in your mind!\nYou feel your resistance slipping!" },
    
    // Special Technophobia enemies
    { "RLTerminator",                       "Terminator",                       100,    1, true,  "Distorted digital roars mark the beginning of your end!" },
    
    // Armageddon
    { "RLFormerAssaultTrooper",             "Former Human Assault Trooper",     5,      3, false, "Marching feet and magazines being slapped into\nplace alerts you of the battalion here!" },
    { "RLFormerOverwatch",                  "Former Human Overwatch",           8,      3, false, "You hear the sound of someone activating a hover drone nearby..." },
    { "RLFormerShocktrooper",               "Former Human Shocktrooper",        15,     3, false, "You get the feeling someone is about to get the drop on you..." },
//  { "RLArmageddonImp",                    "The Watcher",                      12,     3, false, "You feel incredibly uneasy, as though many things are watching you!" },
    { "RLArmageddonDemon",                  "Mech-strosity",                    8,      3, false, "You feel that using a gun here might be a baaaad idea!" },
//  { "RLArmageddonSpectre",                "Rift Demon",                       16,     3, false, "There don't appear to be any monsters, but that's likely to change!" },
    { "RLArmageddonLostSoul",               "The Hungry",                       6,      3, false, "You hear endless slavering mouths, hoping to consume human flesh!" },
    { "RLArmageddonCacodemon",              "Cacobyss",                         25,     4, false, "Metallic echoing screeches herald your doom!" },
//  { "RLArmageddonHellKnight",             "Crusader of Ruination",            50,     4, false, "You hear the clanking of metal hooves and the smell of burnt flesh!" },
//  { "RLArmageddonBaronOfHell",            "Lord of the Abyss",                65,     4, false, "Deep laughter and endless flames welcome you to your death!" },
//  { "RLArmageddonArachnotron",            "Arachnomaster",                    70,     4, false, "You feel you've intruded somewhere that you should not have!" },
    { "RLArmageddonPainElemental",          "Hellstorm Elemental",              80,     4, false, "The air ripples with instability!\nYou feel like you are being slowly torn apart!" },
    { "RLArmageddonRevenant",               "Revenihilator",                    60,     4, false, "Beeping and the sound of bones scraping\nagainst metal alerts you to the danger ahead!" },
    { "RLArmageddonMancubus",               "Magmabus",                         75,     4, false, "Deep growling and blasts of searing heat greet you here!" },
//  { "RLArmageddonArchvile",               "Dark-Vile",                        85,     5, false, "This entire area seems to be corrupted with great and terrible magic!" },
//  { "RLArmageddonCyberdemon",             "Nightmare Cyberdemon Mk. II",      100,    5, true,  "Hell has gone all out to stop you!\nYou are one doomed space marine..." },
//  { "RLArmageddonSpiderMastermind",       "Nightmare Overmind",               100,    5, true,  "The greatest of foul minds,\nthey taunt you telepathically before their hunt begins!" },
    
    // Special Armageddon enemies
    // These guys should never be one monsters or anything like that.
    // [KS] Should we add them as Megabosses instead?
//  { "RLArmageddonCyberdemon2",            "Cyber-Riftlord",                   100,    10, true, "You can feel the presence of dark entities beyond your comprehension!" },
//  { "RLArmageddonSpiderMastermind2",      "Supreme Overmind",                 100,    10, true, "Seems like you found the grand leaders of the invasion here!" }
};

MonsterInfo const MonsterDataCH[MAX_DEF_MONSTERS_CH] =
{
	// Zombieman - 8
    { "CommonZombie",                         "Former Human",                     1,      0, false, "You hear shuffling footsteps and moans!" },
	{ "GreenZombie",                          "Uncommon Former Human",            2,      0, false, "You hear shuffling footsteps and moans!" },
	{ "BlueZombie",                           "Rare Former Human",                2,      0, false, "You hear shuffling footsteps and moans!" },
	{ "PurpleZombie",                         "Epic Former Human",                3,      0, false, "You hear shuffling footsteps and moans!" },
	{ "YellowZombie",                         "Legendary Former Human",           3,      1, false, "You hear shuffling footsteps and moans!" },
	{ "RedZombie",                            "Demonic Former Human",             8,      1, false, "You hear shuffling footsteps and moans!" },
	{ "BlackZombie1",                         "Player 9",                         70,     3, true,  "You hear shuffling footsteps and moans!" },
	{ "WhiteZombie1",                         "The Undertaker",                   90,     4, true,  "You hear shuffling footsteps and moans!" },
	
	// Shotgunner - 8
    { "CommonSG",                             "Former Sergeant",                  2,      0, false, "You hear the sound of shotguns pumping!" },
	{ "GreenSG",                              "Uncommon Former Sergeant",         3,      0, false, "You hear the sound of shotguns pumping!" },
	{ "BlueSG",                               "Rare Former Sergeant",             4,      0, false, "You hear the sound of shotguns pumping!" },
	{ "PurpleSG",                             "Epic Former Sergeant",             5,      0, false, "You hear the sound of shotguns pumping!" },
	{ "YellowSG",                             "Legendary Former Sergeant",        6,      1, false, "You hear the sound of shotguns pumping!" },
	{ "RedSG",                                "Demonic Former Sergeant",          10,      1, false, "You hear the sound of shotguns pumping!" },
	{ "BlackSG3",                             "Shotgun Crew Commander",           75,     3, true,  "You hear the sound of shotguns pumping!" },
	{ "WhiteSG2",                             "Benellus, God of Shotguns",        90,     4, true,  "You hear the sound of shotguns pumping!" },
	
	// Chaingunner - 8
    { "CommonCguy",                           "Former Commando",                  5,      0, false, "Quiet mutters and whirring set you on edge!" },
	{ "GreenCguy",                            "Uncommon Former Commando",         6,      0, false, "Quiet mutters and whirring set you on edge!" },
	{ "BlueCguy",                             "Rare Former Commando",             8,      0, false, "Quiet mutters and whirring set you on edge!" },
	{ "PurpleCguy",                           "Epic Former Commando",             10,     1, false, "Quiet mutters and whirring set you on edge!" },
	{ "YellowCguy",                           "Legendary Former Commando",        14,     1, false, "Quiet mutters and whirring set you on edge!" },
	{ "RedCguy",                              "Demonic Former Commando",          16,     2, false, "Quiet mutters and whirring set you on edge!" },
	{ "BlackCguy2",                           "Mr. General",                      70,     3, true,  "Quiet mutters and whirring set you on edge!" },
	{ "WhiteCguy2",                           "Crazy Lady Scientist",             80,     4, true,  "Quiet mutters and whirring set you on edge!" },
	
	// Imp - 8
    { "CommonImp",                            "Imp",                              4,      0, false, "The walls are scratched and flame-scorched!" },
	{ "GreenImp",                             "Uncommon Imp",                     5,      0, false, "The walls are scratched and flame-scorched!" },
	{ "BlueImp",                              "Rare Imp",                         6,      0, false, "The walls are scratched and flame-scorched!" },
	{ "PurpleImp",                            "Epic Imp",                         7,      0, false, "The walls are scratched and flame-scorched!" },
	{ "YellowImp",                            "Legendary Imp",                    9,      1, false, "The walls are scratched and flame-scorched!" },
	{ "RedImp",                               "Demonic Imp",                      15,     1, false, "The walls are scratched and flame-scorched!" },
	{ "BlackImp1",                            "Smoking Black Imp",                50,     2, true,  "The walls are scratched and flame-scorched!" },
	{ "WhiteImp2",                            "Imp Master",                       60,     3, true,  "The walls are scratched and flame-scorched!" },
	
	// Demon - 8
    { "CommonDemon",                          "Demon",                            3,      0, false, "Hungry growls echo around you!" },
	{ "GreenDemon",                           "Uncommon Demon",                   4,      0, false, "Hungry growls echo around you!" },
	{ "BlueDemon",                            "Rare Demon",                       5,      0, false, "Hungry growls echo around you!" },
	{ "PurpleDemon",                          "Epic Demon",                       6,      0, false, "Hungry growls echo around you!" },
	{ "YellowDemon",                          "Legendary Demon",                  12,     1, false, "Hungry growls echo around you!" },
	{ "RedDemon",                             "Demonic Demon",                    21,     2, false, "Hungry growls echo around you!" },
	{ "BlackDemon3",                          "The Butcher",                      55,     3, true,  "Hungry growls echo around you!" },
	{ "WhiteDemon2",                          "The Juggernaut",                   70,     4, true,  "Hungry growls echo around you!" },
	
	// Spectre - 8
    { "CommonSpectre",                        "Spectre",                          3,      0, false, "It's dark, you are likely to be eaten by a... Spectre?" },
	{ "GreenSpectre",                         "Uncommon Spectre",                 4,      0, false, "It's dark, you are likely to be eaten by a... Spectre?" },
	{ "BlueSpectre",                          "Rare Spectre",                     5,      0, false, "It's dark, you are likely to be eaten by a... Spectre?" },
	{ "PurpleSpectre",                        "Epic Spectre",                     7,      0, false, "It's dark, you are likely to be eaten by a... Spectre?" },
	{ "YellowSpectre",                        "Legendary Spectre",                14,     1, false, "It's dark, you are likely to be eaten by a... Spectre?" },
	{ "RedSpectre",                           "Demonic Spectre",                  20,     2, false, "It's dark, you are likely to be eaten by a... Spectre?" },
	{ "BlackSpectre2",                        "The Rogue",                        64,     3, true,  "It's dark, you are likely to be eaten by a... Spectre?" },
	{ "WhiteSpectre2",                        "???",                              77,     4, true,  "It's dark, you are likely to be eaten by a... Spectre?" },
	
	// Cacodemon - 8
    { "CommonCaco",                           "Cacodemon",                        10,     0, false, "Screeches drown out all other sound!" },
	{ "GreenCaco",                            "Uncommon Cacodemon",               11,     0, false, "Screeches drown out all other sound!" },
	{ "BlueCaco",                             "Rare Cacodemon",                   12,     1, false, "Screeches drown out all other sound!" },
	{ "PurpleCaco",                           "Epic Cacodemon",                   14,     1, false, "Screeches drown out all other sound!" },
	{ "YellowCaco",                           "Cacolich",                         22,     2, false, "Screeches drown out all other sound!" },
	{ "RedCaco",                              "Demonic Cacodemon",                30,     3, false, "Screeches drown out all other sound!" },
	{ "BlackCaco2",                           "Shockmaster",                      56,     4, true,  "Screeches drown out all other sound!" },
	{ "WhiteCaco2",                           "White Cacodemon",                  87,     5, true,  "Screeches drown out all other sound!" },
	
	// Hell Knight - 8
    { "CommonHK",                             "Hell Knight",                      20,     0, false, "A battle cry chants in the distance!" },
	{ "GreenHK",                              "Uncommon Hell Knight",             22,     0, false, "A battle cry chants in the distance!" },
	{ "BlueHK",                               "Rare Hell Knight",                 24,     1, false, "A battle cry chants in the distance!" },
	{ "PurpleHK",                             "Epic Hell Knight",                 29,     1, false, "A battle cry chants in the distance!" },
	{ "YellowHK",                             "Bruiser",                          38,     2, false, "A battle cry chants in the distance!" },
	{ "RedHK",                                "Hell Knightmare",                  50,     3, false, "A battle cry chants in the distance!" },
	{ "BlackHK2",                             "The Terminator",                   74,     4, true,  "A battle cry chants in the distance!" },
	{ "WhiteHK3",                             "Ghost of 1993",                    84,     5, true,  "A battle cry chants in the distance!" },
	
	// Baron - 7
    { "CommonBaron",                          "Baron of Hell",                    30,     0, false, "Seems you've found Hell's nobility!" },
	{ "GreenBaron",                           "Uncommon Baron of Hell",           34,     1, false, "Seems you've found Hell's nobility!" },
	{ "BlueBaron",                            "Rare Baron of Hell",               38,     2, false, "Seems you've found Hell's nobility!" },
	{ "PurpleBaron",                          "Epic Baron of Hell",               42,     3, false, "Seems you've found Hell's nobility!" },
	{ "YellowBaron",                          "Grand Baron of Hell",              55,     3, false, "Seems you've found Hell's nobility!" },
	{ "RedBaron3",                            "Red Baron",                        69,     4, false, "Seems you've found Hell's nobility!" },
	{ "BlackBaron2",                          "Baron of the Abyss",               90,     5, true,  "Seems you've found Hell's nobility!" },
	
	// Lost Soul - 8
	{ "CommonLSoul",                          "Lost Soul",                        3,      0, false, "The sound of many flames echo about!" },
	{ "GreenLSoul",                           "Uncommon Lost Soul",               4,      0, false, "The sound of many flames echo about!" },
	{ "BlueLSoul",                            "Rare Lost Soul",                   5,      0, false, "The sound of many flames echo about!" },
	{ "PurpleLSoul",                          "Epic Lost Soul",                   6,      0, false, "The sound of many flames echo about!" },
	{ "YellowLSoul",                          "Legendary Lost Soul",              8,      0, false, "The sound of many flames echo about!" },
	{ "RedLSoul",                             "Bloody Lost Soul",                 12,     1, false, "The sound of many flames echo about!" },
	{ "BlackLSoul3",                          "Queen Bee",                        18,     2, true,  "The sound of many flames echo about!" },
	{ "WhiteLSoul2",                          "The Shifter",                      25,     3, true,  "The sound of many flames echo about!" },
	
	// Pain Elemental - 8
    { "CommonPE",                             "Pain Elemental",                   40,     0, false, "You feel you are being watched!" },
	{ "GreenPE",                              "Uncommon Pain Elemental",          42,     1, false, "You feel you are being watched!" },
	{ "BluePE",                               "Rare Pain Elemental",              44,     1, false, "You feel you are being watched!" },
	{ "PurplePE",                             "Epic Pain Elemental",              46,     2, false, "You feel you are being watched!" },
	{ "YellowPE",                             "Volcanic Pain Elemental",          55,     3, false, "You feel you are being watched!" },
	{ "RedPE",                                "Rage Elemental",                   65,     4, false, "You feel you are being watched!" },
	{ "BlackPE2",                             "Hell Soul Elemental",              80,     5, true,  "You feel you are being watched!" },
	{ "WhitePE2",                             "The Watcher",                      95,     5, true,  "You feel you are being watched!" },
	
	// Revenant - 7
    { "CommonRevenant",                       "Revenant",                         50,     0, false, "Bones clatter all around you!" },
	{ "GreenRevenant",                        "Uncommon Revenant",                52,     1, false, "Bones clatter all around you!" },
	{ "BlueRevenant",                         "Rare Revenant",                    54,     1, false, "Bones clatter all around you!" },
	{ "PurpleRevenant",                       "Epic Revenant",                    58,     2, false, "Bones clatter all around you!" },
	{ "YellowRevenant",                       "Ghostly Revenant",                 68,     3, false, "Bones clatter all around you!" },
	{ "RedRevenant",                          "Bloody Revenant",                  78,     4, false, "Bones clatter all around you!" },
	{ "BlackRevenant3",                       "The Black Knight",                 88,     5, true,  "Bones clatter all around you!" },
	
	// Mancubus - 7
    { "CommonFatso",                          "Mancubus",                         50,     0, false, "You hear deep, guttural noises!" },
	{ "GreenFatso",                           "Uncommon Mancubus",                54,     1, false, "You hear deep, guttural noises!" },
	{ "BlueFatso",                            "Rare Mancubus",                    58,     1, false, "You hear deep, guttural noises!" },
	{ "PurpleFatso",                          "Epic Mancubus",                    63,     2, false, "You hear deep, guttural noises!" },
	{ "YellowFatso",                          "Legendary Mancubus",               70,     3, false, "You hear deep, guttural noises!" },
	{ "RedFatso",                             "Horned Beast",                     80,     4, false, "You hear deep, guttural noises!" },
	{ "BlackFatso2",                          "Dark Beast",                       91,     5, true,  "You hear deep, guttural noises!" },
	
	// Arachnotron - 8
    { "CommonSP",                             "Arachnotron",                      45,     0, false, "Leg servos squeak and whirr nearby!" },
	{ "GreenSP",                              "Uncommon Arachnotron",             50,     0, false, "Leg servos squeak and whirr nearby!" },
	{ "BlueSP",                               "Rare Arachnotron",                 55,     1, false, "Leg servos squeak and whirr nearby!" },
	{ "PurpleSP",                             "Epic Arachnotron",                 60,     2, false, "Leg servos squeak and whirr nearby!" },
	{ "YellowSP",                             "Brainchotron",                     67,     3, false, "Leg servos squeak and whirr nearby!" },
	{ "RedSP",                                "Raging Arachnotron",               78,     4, false, "Leg servos squeak and whirr nearby!" },
	{ "BlackSP2",                             "Explosive Arachnotron",            87,     5, true,  "Leg servos squeak and whirr nearby!" },
	{ "WhiteSP11",                            "White Spider",                     90,     5, true,  "Leg servos squeak and whirr nearby!" },
	
	// Archvile - 8
    { "CommonArch",                           "Archvile",                         70,     0, false, "You hear crackling flames!" },
	{ "GreenArch",                            "Uncommon Archvile",                74,     1, false, "You hear crackling flames!" },
	{ "BlueArch",                             "Rare Archvile",                    78,     1, false, "You hear crackling flames!" },
	{ "PurpleArch",                           "Epic Archvile",                    82,     2, false, "You hear crackling flames!" },
	{ "YellowArch",                           "Golden Archvile",                  86,     3, false, "You hear crackling flames!" },
	{ "RedArch2",                             "Infernovile",                      90,     4, false, "You hear crackling flames!" },
	{ "BlackVile",                            "The Void",                         94,     5, true,  "You hear crackling flames!" },
	{ "Whitevile",                            "Godly Archvile",                   98,     5, true,  "You hear crackling flames!" },
	
	// Cyberdemon - 6
    { "CommonCybie",                          "Cyberdemon",                       85,     1, true,  "Suddenly you have a great urge to turn back! You scream in TERROR!" },
	{ "GreenCybie",                           "Uncommon Cyberdemon",              88,     2, true,  "Suddenly you have a great urge to turn back! You scream in TERROR!" },
	{ "BlueCybie",                            "Rare Cyberdemon",                  92,     3, true,  "Suddenly you have a great urge to turn back! You scream in TERROR!" },
	{ "PurpleCybie",                          "Epic Cyberdemon",                  95,     4, true,  "Suddenly you have a great urge to turn back! You scream in TERROR!" },
	{ "YellowCybie",                          "Legendary Cyberdemon",             99,     4, true,  "Suddenly you have a great urge to turn back! You scream in TERROR!" },
	{ "RedCybie",                             "Moloch",                           100,    5, true,  "Suddenly you have a great urge to turn back! You scream in TERROR!" },
	
	// Mastermind - 6
    { "CommonMind",                           "Spider Mastermind",                95,     1, true,  "You've stumbled into a hive of the deadliest kind!" },
	{ "GreenMind",                            "Uncommon Spider Mastermind",       96,     2, true,  "You've stumbled into a hive of the deadliest kind!" },
	{ "BlueMind",                             "Rare Spider Mastermind",           97,     3, true,  "You've stumbled into a hive of the deadliest kind!" },
	{ "PurpleMind",                           "Epic Spider Mastermind",           98,     4, true,  "You've stumbled into a hive of the deadliest kind!" },
	{ "YellowMind",                           "Legendary Spider Mastermind",      99,     4, true,  "You've stumbled into a hive of the deadliest kind!" },
	{ "RedMind",                              "Arachnophyte",                     100,    5, true,  "You've stumbled into a hive of the deadliest kind!" }
};

MegabossInfo const MegaBosses[MAX_MEGABOSSES] =
{
    { "DRPGMegabossPride",   "Pride" },
    { "DRPGMegabossSloth",   "Sloth" },
};

MegabossInfo const MegaBossesCH[MAX_MEGABOSSES_CH] =
{
	{ "DRPGMegabossPride",   "Pride" },
    { "DRPGMegabossSloth",   "Sloth" },
	{ "BlackCybie2RPG",		"Cybersmith" },
	{ "BlackMind2RPG",		"Queen Arachne" },
};

int NewMonsterID()
{
    int CurrentID = MonsterID++;
    if (CurrentID >= MAX_MONSTERS)
    {
        MonsterID = MAX_MONSTERS;
        return 0;
    }
    
    // When re-using an existing ID, all of these need to be reset so Init knows what to do with them
    Monsters[CurrentID].Actor = NULL;
    Monsters[CurrentID].ReplaceActor = "";
    Monsters[CurrentID].Flags = 0;
    Monsters[CurrentID].RenderStyle = 0;
    Monsters[CurrentID].SpawnHealth = 0;
    Monsters[CurrentID].HealthMax = 0;
    Monsters[CurrentID].Speed = 0;
    Monsters[CurrentID].Height = 0;
    Monsters[CurrentID].Radius = 0;
    Monsters[CurrentID].NeedReinit = false;
    Monsters[CurrentID].HealthBar = false;
    Monsters[CurrentID].Named = false;
    Monsters[CurrentID].Reinforcement = false;
    Monsters[CurrentID].Target = 0;
    Monsters[CurrentID].Level = 0;
    Monsters[CurrentID].Aura.Time = 0;
    for (int i = 0; i < AURA_MAX; i++)
    {
        Monsters[CurrentID].Aura.Type[i].Active = false;
        Monsters[CurrentID].Aura.Type[i].Level = 0;
    }
    Monsters[CurrentID].Threat = 0;
    Monsters[CurrentID].Strength = 0;
    Monsters[CurrentID].Defense = 0;
    Monsters[CurrentID].Vitality = 0;
    Monsters[CurrentID].Energy = 0;
    Monsters[CurrentID].Regeneration = 0;
    Monsters[CurrentID].Agility = 0;
    Monsters[CurrentID].Capacity = 0;
    Monsters[CurrentID].Luck = 0;
    
    return CurrentID;
}

// Monster Init Script
NamedScript DECORATE void MonsterInit(int Flags)
{
    // Get a new ID for the monster
    if (GetMonsterID(0) == 0)
        SetMonsterID(0, NewMonsterID());
    
    if (GetMonsterID(0) == 0)
    {
        // The monster couldn't be given an ID, so there's nothing we can possibly do here.
        Thing_Remove(0);
        return;
    }
    
    // Monster Stats Pointer
    MonsterStatsPtr Stats = &Monsters[GetMonsterID(0)];
    
    // If initialization has already occurred on this monster, terminate
    if (Stats->Init) return;
    
    // Store TID
    if (ActivatorTID() == 0)
    {
        int TID = UniqueTID();
        Thing_ChangeTID(0, TID);
        Stats->TID = TID;
    }
    else
        Stats->TID = ActivatorTID();
    
    // Store Position
    Stats->spawnPos.X = GetActorX(0);
    Stats->spawnPos.Y = GetActorY(0);
    Stats->spawnPos.Z = GetActorZ(0);
    Stats->spawnPos.Angle = GetActorAngle(0);
    Stats->spawnPos.Pitch = GetActorPitch(0);
    
    // Set as initialized
    Stats->Init = true;
    SetInventory("DRPGMonsterInit", 1);
    
    // Store Actor Name
    Stats->Actor = GetActorClass(0);
    
    Delay(1); // Allow map events to modify our stats/replacements (see Map.ds:18)
    while (WaitingForReplacements)
        Delay(1);
    
    if (!Stats->Init || Stats->TID == 0 || ClassifyActor(Stats->TID) == ACTOR_NONE)
    {
        // Some other event trigger removed us, or we removed ourselves
        Stats->Init = false;
        Stats->TID = 0;
        return;
    }
    
    if (StrLen(Stats->ReplaceActor) > 0)
    {
        int OldTID = ActivatorTID();
        bool Ambush = GetActorProperty(0, APROP_Ambush);
        Thing_Remove(0);
        Stats->Init = false;
        Stats->TID = 0;
        if (StrICmp(Stats->ReplaceActor, "None") != 0)
        {
            int NewTID = UniqueTID();
            if (Spawn(Stats->ReplaceActor, Stats->spawnPos.X, Stats->spawnPos.Y, Stats->spawnPos.Z, NewTID, Stats->spawnPos.Angle * 256))
            {
                SetActorProperty(NewTID, APROP_Ambush, Ambush);
                Thing_ChangeTID(NewTID, OldTID);
            }
        }
        return;        
    }
    
    // Store Monster Flags
    Stats->Flags = Flags;
    
    // Set the Height and Radius
    Stats->Height = GetActorPropertyFixed(0, APROP_Height);
    Stats->Radius = GetActorPropertyFixed(0, APROP_Radius);
    
    // Apply base Spawn Health to HealthMax property
    Stats->SpawnHealth = GetActorProperty(0, APROP_SpawnHealth);
    Stats->HealthMax = Stats->SpawnHealth;
    
    // Start Damage Numbers Script
    DamageNumbers();
    
    // Give it a Health Bar
    if (!(Flags & MF_NOHEALTHBAR))
        Stats->HealthBar = true;
    
    // Apply Stats
    if (!(Flags & MF_NOSTATS))
        MonsterInitStats();
    
    // Save the Render Type
    Stats->RenderStyle = GetActorProperty(0, APROP_RenderStyle);
    
    // Stat-Change Handling
    MonsterStatsHandler();
    
    // Aura Spawner
    MonsterAuraDisplayHandler();
    
    // Regeneration Stat Handling
    if (!(Flags & MF_NOSTATS))
        MonsterRegenerationHandler();
    
    // Energy Stat Handling
    MonsterAggressionHandler();
    
    // Death Handler
    MonsterDeathCheck();
    
    // Environmental Hazard Map Event
    if (CurrentLevel->Event == MAPEVENT_TOXICHAZARD && (Random(1, 24) == 1 || Stats->Flags & MF_BOSS || Stats->Flags & MF_MEGABOSS))
    {
        //GiveInventory("DRPGMonsterRadiationProtection", 1);
        GiveInventory("DRPGMonsterRadSuit", 1);
    }
}

// Modify the targeted monster (mainly for debugging)
NamedScript Console void MonsterSet(int Level, int Aura, int Flags)
{
    // Move script to the targeted monster
    SetActivatorToTarget(Player.TID);
    
    // Pointer
    MonsterStatsPtr Stats = &Monsters[GetMonsterID(0)];
    
    // Set Stats/Flags
    if (Level > 0)
        Stats->Level = Level;
    if (Aura > 0)
        Stats->Aura.Type[Aura - 1].Active = true;
    if (Flags > 0)
        Stats->Flags = Flags;
    
    // Needs reinitialization to apply new info
    Stats->NeedReinit = true;
}

// Give the targeted monster a Shadow Aura (mainly for debugging)
NamedScript Console void MonsterSetShadow()
{
    // Move script to the targeted monster
    SetActivatorToTarget(Player.TID);
    
    // Pointer
    MonsterStatsPtr Stats = &Monsters[GetMonsterID(0)];
    
    // Apply all Auras
    for (int i = 0; i < AURA_MAX; i++)
        Stats->Aura.Type[i].Active = true;
    
    // Needs reinitialization to apply new info
    Stats->NeedReinit = true;
}

// Modify the targeted monsters stats (mainly for debugging)
NamedScript Console void MonsterModStat(int Stat, int Value)
{
    // Move script to the targeted monster
    SetActivatorToTarget(Player.TID);
    
    // Pointer
    MonsterStatsPtr Stats = &Monsters[GetMonsterID(0)];
    
    switch (Stat - 1)
    {
    case STAT_STRENGTH:     Stats->Strength = Value;        break;
    case STAT_DEFENSE:      Stats->Defense = Value;         break;
    case STAT_VITALITY:     Stats->Vitality = Value;        break;
    case STAT_ENERGY:       Stats->Energy = Value;          break;
    case STAT_REGENERATION: Stats->Regeneration = Value;    break;
    case STAT_AGILITY:      Stats->Agility = Value;         break;
    case STAT_CAPACITY:     Stats->Capacity = Value;        break;
    case STAT_LUCK:         Stats->Luck = Value;            break;
    }
}

// Dump the targeted monster's stats to the console
NamedScript Console void MonsterDump()
{
    int PlayerNum = PlayerNumber();
    
    // Move script to the targeted monster
    SetActivatorToTarget(Player.TID);
    
    // Pointer
    MonsterStatsPtr Stats = &Monsters[GetMonsterID(0)];
    
    // If you have no target, terminate
    if (ActivatorTID() == Players(PlayerNum).TID)
    {
        PrintError("You have no target");
        ActivatorSound("menu/error", 127);
        return;
    }
    
    Log("\Ca===== MONSTER INFORMATION =====");
    Log(" Actor: %S", GetActorClass(0));
    Log(" Replacement Actor: %S", Stats->ReplaceActor);
    Log(" Position: %.2k, %.2k, %.2k", Stats->spawnPos.X, Stats->spawnPos.Y, Stats->spawnPos.Z);
    Log(" Tag: %S", GetActorPropertyString(0, APROP_NameTag));
    Log(" Height: %.2k", GetActorPropertyFixed(0, APROP_Height));
    Log(" Radius: %.2k", GetActorPropertyFixed(0, APROP_Radius));
    Log(" Speed: %.2k", GetActorPropertyFixed(0, APROP_Speed));
    Log(" TID: %d", ActivatorTID());
    Log(" ID: %d", GetMonsterID(0));
    
    if (GetMonsterID(0) != 0)
    {
        Log("\Cf===== MONSTER FLAGS =====");
        Log(" Init: %d", Stats->Init);
        Log(" Named: %d", Stats->Named);
        Log(" HealthBar: %d", Stats->HealthBar);
        Log(" RenderStyle: %d", Stats->RenderStyle);
        Log(" DRPG Flags: %d", Stats->Flags);
        Log(" Reinforcement: %d", Stats->Reinforcement);
        Log(" Assassination Target: %d", Stats->Target);
        Log(" MegaBoss: %d", Stats->MegaBoss);
        
        Log("\Cd===== MONSTER STATS =====");
        Log(" Level: %d", Stats->Level);
        Log(" Threat: %d", Stats->Threat);
        Log(" Spawn Health: %d", Stats->SpawnHealth);
        Log(" Health Max: %d", Stats->HealthMax);
        Log(" Health Regenerated: %d", Stats->RegenHealth);
        Log(" Base Speed: %d", Stats->Speed);
        Log(" \CgStrength: %d", Stats->Strength);
        Log(" \CdDefense: %d", Stats->Defense);
        Log(" \CaVitality: %d", Stats->Vitality);
        Log(" \CnEnergy: %d", Stats->Energy);
        Log(" \CtRegeneration: %d", Stats->Regeneration);
        Log(" \CiAgility: %d", Stats->Agility);
        Log(" \ChCapacity: %d", Stats->Capacity);
        Log(" \CkLuck: %d", Stats->Luck);
        
        Log("\Cd===== MONSTER AURAS =====");
        Log(" Time: %S", FormatTime(Stats->Aura.Time));
        for (int i = 0; i < AURA_MAX; i++) // TODO: Make this prettier
            if (Stats->Aura.Type[i].Active)
                Log(" Active: %d", i);
    }
}

// Damage the target
NamedScript Console void MonsterDamage(int Amount, int Type)
{
    SetActivatorToTarget(Player.TID);
    
    if (ActivatorTID() == Player.TID)
        return;
    
    DamageThing(Amount, Type);
}

// Apply stats to monsters
OptionalArgs(1) NamedScript void MonsterInitStats(int StatFlags)
{
    // Move Activator to whoever the player is targeting
    if (StatFlags & SF_PLAYERTARGET)
        SetActivatorToTarget(Player.TID);
    
    if (ClassifyActor(0) & ACTOR_WORLD)
        return; // The Monster Reducer can remove a monster before it attains stats, so if that happens, don't bother.
    
    if (GetMonsterID(0) < 1 || GetMonsterID(0) >= MAX_MONSTERS)
    {
        Thing_Remove(0);
        return;
    }
    
    // Pointer
    MonsterStatsPtr Stats = &Monsters[GetMonsterID(0)];
    
    int MonsterLevel = Stats->Level;
    int MonsterThreat = Stats->Threat;
    fixed MonsterSpeed = Stats->Speed;
    int MonsterStrength = Stats->Strength;
    int MonsterDefense = Stats->Defense;
    int MonsterVitality = Stats->Vitality;
    int MonsterEnergy = Stats->Energy;
    int MonsterRegeneration = Stats->Regeneration;
    int MonsterAgility = Stats->Agility;
    int MonsterCapacity = Stats->Capacity;
    int MonsterLuck = Stats->Luck;
    int LevelType = GetCVar("drpg_monster_levels");
    fixed LevelWeight = GetCVarFixed("drpg_monster_level_weight");
    fixed MapWeight = GetCVarFixed("drpg_monster_map_weight");
    fixed RandomMinWeight = GetCVarFixed("drpg_monster_random_min_mult");
    fixed RandomMaxWeight = GetCVarFixed("drpg_monster_random_max_mult");
    int LevelNum = CurrentLevel->LevelNum;
    int NumPlayers;
    int StatEffect[8];
    int MonsterStatPool;
    
    // Cap Level Number to 100
    if (LevelNum > 100)
        LevelNum = 100;
    
    // If the Arena is active, base the Monster Levels Map Number portion on the current wave
    if (CurrentLevel->UACBase && ArenaActive)
        LevelNum = ArenaWave / 3;
    
    // Calculate Monster Level
    if (LevelType == 1 || LevelType == 3) // Player Level
        MonsterLevel += (int)((fixed)AveragePlayerLevel() * LevelWeight);
    if (LevelType == 2 || LevelType == 3) // Map Number
        MonsterLevel += (int)((fixed)LevelNum * MapWeight);
    if (AveragePlayerLuck() < 0)
        MonsterLevel += -AveragePlayerLuck();
    
    // Randomization Weight
    if (RandomMinWeight > RandomMaxWeight)
        Log("\CgERROR: \C-Monster Random Min Multiplier cannot be above Monster Random Max Multiplier!");
    else
        MonsterLevel = (int)(MonsterLevel * RandomFixed(RandomMinWeight, RandomMaxWeight));
    
    // If the monster is friendly, it has the average level of all players in the game
    if (GetActorProperty(0, APROP_Friendly))
        MonsterLevel = AveragePlayerLevel();
    
    // Special case for Bosses
    if (Stats->Flags & MF_BOSS)
        MonsterLevel += (GameSkill() * 10);
    
    // Special case for Megabosses
    if (Stats->Flags & MF_MEGABOSS)
        MonsterLevel += ((1000 / MAX_PLAYERS) * PlayerCount());
    
    // Special case for Powersuit Mk. II
    if (GetActorClass(0) == "DRPGSuperPowerSuit")
        MonsterLevel = 1000;
    
    MonsterStatPool = 40 + GameSkill() * MonsterLevel;
    
    // Calculate the monster's cut and special stats
    if (GetCVar("drpg_monster_specialize"))
    {
        // [KS] Here's a fun and fast little way of doing it:
        
        // 2 specialized stats
        StatEffect[0] = 2;
        StatEffect[1] = 2;
        
        // 2 normal stats
        StatEffect[2] = 0;
        StatEffect[3] = 0;
        
        // 4 cut stats
        StatEffect[4] = 1;
        StatEffect[5] = 1;
        StatEffect[6] = 1;
        StatEffect[7] = 1;
        
        // Shuffle 'em up!
        for (int i = 0; i < STAT_MAX; i++)
        {
            int SwapWith = Random(0, STAT_MAX - 1);
            int Temp = StatEffect[i];
            
            StatEffect[i] = StatEffect[SwapWith];
            StatEffect[SwapWith] = Temp;
        }
        
        // Next, create a weighted list of stats.
        // Specialized stats are preferred over normal stats, and cut stats are avoided.
        int StatSelector[14];
        int j = 0;
        
        for (int i = 0; i < STAT_MAX; i++)
        {
            if (StatEffect[i] == 2)
            {
                StatSelector[j++] = i;
                StatSelector[j++] = i;
                StatSelector[j++] = i;
            }
            else if (StatEffect[i] == 0)
            {
                StatSelector[j++] = i;
                StatSelector[j++] = i;
            }
            else
                StatSelector[j++] = i;
        }
        
        // Finally, distribute the points with our weighted list.
        while (MonsterStatPool > 0)
        {
            switch (StatSelector[Random (0, 13)])
            {
                case 0:
                    MonsterStrength++;
                    break;
                case 1:
                    MonsterDefense++;
                    break;
                case 2:
                    MonsterVitality++;
                    break;
                case 3:
                    MonsterEnergy++;
                    break;
                case 4:
                    MonsterRegeneration++;
                    break;
                case 5:
                    MonsterAgility++;
                    break;
                case 6:
                    MonsterCapacity++;
                    break;
                case 7:
                    MonsterLuck++;
                    break;
            }
            MonsterStatPool--;
        }
    }
    else
    {
        // Distribute entirely at random
        while (MonsterStatPool > 0)
        {
            switch (Random (0, STAT_MAX - 1))
            {
                case 0:
                    MonsterStrength++;
                    break;
                case 1:
                    MonsterDefense++;
                    break;
                case 2:
                    MonsterVitality++;
                    break;
                case 3:
                    MonsterEnergy++;
                    break;
                case 4:
                    MonsterRegeneration++;
                    break;
                case 5:
                    MonsterAgility++;
                    break;
                case 6:
                    MonsterCapacity++;
                    break;
                case 7:
                    MonsterLuck++;
                    break;
            }
            MonsterStatPool--;
        }
    }
    
    // Pity Points
    if (MonsterStrength < 1)     MonsterStrength = 1;
    if (MonsterDefense < 1)      MonsterDefense = 1;
    if (MonsterVitality < 1)     MonsterVitality = 1;
    if (MonsterEnergy < 1)       MonsterEnergy = 1;
    if (MonsterRegeneration < 1) MonsterRegeneration = 1;
    if (MonsterAgility < 1)      MonsterAgility = 1;
    if (MonsterCapacity < 1)     MonsterCapacity = 1;
    if (MonsterLuck < 1)         MonsterLuck = 1;
    
    // Map Event - RAINBOWS!
    if (CurrentLevel->Event == MAPEVENT_BONUS_RAINBOWS)
        MonsterCapacity *= Random(2, 4);
    
    // Apply Aura
    if (!GetActorProperty(0, APROP_Friendly) && !(Stats->Flags & MF_NOAURA) && !(Stats->Flags & MF_NOAURAGEN) && !StatFlags)
    {
        // Check to see if any players have a Kill Auras mission
        int AuraMissionAdd = 0;
        if (CurrentLevel && !CurrentLevel->UACBase && !CurrentLevel->UACArena)
            for (int i = 0; i < MAX_PLAYERS; i++)
                if (Players(i).Mission.Type == MT_KILLAURAS)
                    AuraMissionAdd += Players(i).Mission.Amount;
        
        // Hell skill and above always has an additional +5% chance
        if (GameSkill() >= 5)
            AuraMissionAdd += 5;
        
        // 1st roll: Chance of having an aura at all
        bool HasAura = true;
        if (CurrentLevel->Event != MAPEVENT_ALLAURAS)
        {
            int AuraChance = 31 + ((GetCVar("drpg_aura_curve") * 100 / 97) + (MonsterLuck * 100 / 97));
            if (AuraChance < 0)    AuraChance = 0;
            if (AuraChance > 1000) AuraChance = 1000;
            AuraChance *= AuraChance;
            AuraChance /= 1000;
            if (AuraMissionAdd > 0)
                AuraChance += (AuraMissionAdd * 10);
            HasAura = (Random(0, 1000) < AuraChance);
        }
        
        // 2nd roll: Number of auras to have
        int AuraRand = Random(MonsterEnergy / 10, 200);
        int AuraNumber = (AuraRand * AuraRand) / 4000;
        if (AuraNumber < 1) AuraNumber = 1;
        if (AuraNumber > AURA_MAX) AuraNumber = AURA_MAX;
        if (!HasAura) AuraNumber = 0;

        // Run through and decide on which Auras it should have
        for (int i = 0; i < AURA_MAX; i++)
        {
            if (AuraNumber == 0)
                break;
            
            Stats->Aura.Type[i].Active = true;
            AuraNumber--;
            
            int SwapWith = Random(0, AURA_MAX - 1);
            bool Temp = Stats->Aura.Type[i].Active;
            
            Stats->Aura.Type[i].Active = Stats->Aura.Type[SwapWith].Active;
            Stats->Aura.Type[SwapWith].Active = Temp;
        }
        
        // Are You A Bad Enough Dude?
        if (GetCVar("drpg_monster_shadows"))
            for (int i = 0; i < AURA_MAX; i++)
                Stats->Aura.Type[i].Active = true;
        
        if (CurrentLevel->Event == MAPEVENT_HARMONIZEDAURAS)
        {
            if (CurrentLevel->AuraType == AURA_MAX)
                for (int j = 0; j < AURA_MAX; j++)
                    Stats->Aura.Type[j].Active = true;
            else
                Stats->Aura.Type[CurrentLevel->AuraType].Active = true;
        }
    }
    
    // Apply the aura effects
    if (Stats->Aura.Type[AURA_WHITE].Active) // White Aura - XP
    {
        int OldMonsterLevel = MonsterLevel;
        MonsterLevel *= 2;
        
        MonsterStatPool = GameSkill() * (MonsterLevel - OldMonsterLevel);
        
        // Distribute those new stats now.
        while (MonsterStatPool > 0)
        {
            switch (Random (0, STAT_MAX - 1))
            {
                case 0:
                    MonsterStrength++;
                    break;
                case 1:
                    MonsterDefense++;
                    break;
                case 2:
                    MonsterVitality++;
                    break;
                case 3:
                    MonsterEnergy++;
                    break;
                case 4:
                    MonsterRegeneration++;
                    break;
                case 5:
                    MonsterAgility++;
                    break;
                case 6:
                    MonsterCapacity++;
                    break;
                case 7:
                    MonsterLuck++;
                    break;
            }
            MonsterStatPool--;
        }
        
        GiveInventory("DRPGWhiteAuraGiver", 1);
        MonsterLevelupHandler();
    }
    if (Stats->Aura.Type[AURA_RED].Active) // Red Aura - Strength
    {
        MonsterStrength *= 2;
        GiveInventory("DRPGRedAuraGiver", 1);
        MonsterDamageRetaliationHandler();
    }
    if (Stats->Aura.Type[AURA_GREEN].Active) // Green Aura - Defense
    {
        MonsterDefense *= 2;
        GiveInventory("DRPGGreenAuraGiver", 1);
    }
    if (Stats->Aura.Type[AURA_PINK].Active) // Pink Aura - Vitality
    {
        MonsterVitality *= 2;
        GiveInventory("DRPGPinkAuraGiver", 1);
        MonsterFellowResurrectionHandler();
    }
    if (Stats->Aura.Type[AURA_BLUE].Active) // Blue Aura - Energy
    {
        MonsterEnergy *= 2;
        GiveInventory("DRPGBlueAuraGiver", 1);
        MonsterEPDrainHandler();
    }
    if (Stats->Aura.Type[AURA_PURPLE].Active) // Purple Aura - Regeneration
        MonsterRegeneration *= 2;
    if (Stats->Aura.Type[AURA_ORANGE].Active) // Orange Aura - Agility
    {
        MonsterAgility *= 2;
        GiveInventory("DRPGOrangeAuraGiver", 1);
        MonsterEpicVisitTimeHandler();
    }
    if (Stats->Aura.Type[AURA_DARKBLUE].Active) // Dark Blue Aura - Capacity
    {
        MonsterCapacity *= 2;
        MonsterAmmoDrainHandler();
    }
    if (Stats->Aura.Type[AURA_YELLOW].Active) // Yellow Aura - Luck
    {
        MonsterLuck *= 2;
        MonsterMoneyDrainHandler();
    }
    
    // Calculate Aura time
    Stats->Aura.Time = (30 * 35) + (int)((fixed)MonsterEnergy * 0.57) * 35;
    
    // Cap Level and Stats
    CapMonsterStats(Stats);
    
    // We need to make sure we only pass the speed to the monster if it's being initially created
    MonsterSpeed = GetActorPropertyFixed(0, APROP_Speed);
    
    // Apply the stats to the monster
    Stats->Level = MonsterLevel;
    Stats->Speed = MonsterSpeed;
    Stats->Strength = MonsterStrength;
    Stats->Defense = MonsterDefense;
    Stats->Vitality = MonsterVitality;
    Stats->Energy = MonsterEnergy;
    Stats->Regeneration = MonsterRegeneration;
    Stats->Agility = MonsterAgility;
    Stats->Capacity = MonsterCapacity;
    Stats->Luck = MonsterLuck;
    
    // [KS] We only need to do a health check here since the other checks are handled below.
    // Calculate Health
    Stats->HealthMax = CalculateMonsterMaxHealth(Stats);
    SetActorProperty(0, APROP_Health, Stats->HealthMax);
    
    // Generate Name
    if (ColourfulHell || (!Stats->Named && (Stats->Flags & MF_MEGABOSS || Stats->Flags & MF_NAMEGEN || MonsterHasShadowAura(Stats))))
    {
        SetActorPropertyString(0, APROP_NameTag, GenerateName(GetActorClass(0), GetActorPropertyString(0, APROP_NameTag)));
        Stats->Named = true;
    }
    
    // Reinitialize the stats handler if recreation has occurred
    if (StatFlags & SF_RECREATE)
    {
        Stats->NeedReinit = false;
        Delay(1);
        MonsterStatsHandler();
    }
}

NamedScript bool MonsterHasTarget()
{
    return SetActivator(0, AAPTR_TARGET);
}

NamedScript void MonsterAggressionHandler()
{
    
    if (CheckInventory("DRPGMonsterAggressionHandler"))
        return;
    
    GiveInventory("DRPGMonsterAggressionHandler", 1);
    
    // Delay Stagger
    Delay(35 + (GetMonsterID(0) % 4));
    
    // Pointer
    MonsterStatsPtr Stats = &Monsters[GetMonsterID(0)];
    
    int Capacity = Stats->Capacity;
    
    Start:
    
    if (!CheckInventory("DRPGMonsterAggressionHandler"))
        return;
    
    if (GetActorProperty(0, APROP_Health) <= 0)
    {
        Delay(35);
        goto Start;
    }

    if (ClassifyActor(0) & ACTOR_WORLD)
        return;
    
    if (!MonsterHasTarget())
    {
        Delay(10);
        goto Start;
    }

    Capacity = Stats->Capacity;
    
    fixed Aggression = 0.25 + (Capacity * 0.00475);
    
    if (Aggression < 1.0)
    {
        if (RandomFixed(0.0, 1.0) < Aggression)
            GiveInventory("DRPGMonsterDontAttack", 1);
    }
    else
    {
        if (RandomFixed(0.0, 1.0) < ((Aggression - 1.0) / 4.0))
            GiveInventory("DRPGMonsterAttackMore", 1);
    }
    
    Delay(8);
    goto Start;
}

NamedScript void MonsterStatsHandler()
{
    if (CheckInventory("DRPGMonsterStatsHandler"))
        return;
    
    GiveInventory("DRPGMonsterStatsHandler", 1);
    
    // Delay Stagger
    Delay(35 + (GetMonsterID(0) % 4));
    
    // Pointer
    MonsterStatsPtr Stats = &Monsters[GetMonsterID(0)];
    
    int OldStrength;
    int OldDefense;
    int OldCapacity;
    int OldVitality;
    int OldAgility;
    int OldRegeneration;
    int OldEnergy;
    int OldLuck;
    bool StatsChanged;
    bool MonsterWasDisrupted = false;
    
    Start:
    
    if (!CheckInventory("DRPGMonsterStatsHandler"))
        return;
    
    if (GetActorProperty(0, APROP_Health) <= 0)
    {
        if (OldStrength)
        {
            OldStrength = 0;
            OldDefense = 0;
            OldCapacity = 0;
            OldVitality = 0;
            OldAgility = 0;
            OldRegeneration = 0;
            OldEnergy = 0;
            OldLuck = 0;
        }
        Delay(35);
        goto Start;
    }

    if (ClassifyActor(0) & ACTOR_WORLD)
        return;
    
    if (Stats->NeedReinit)
    {
        MonsterInitStats(SF_RECREATE);
        TakeInventory("DRPGMonsterStatsHandler", 1);
        return;
    }
    
    /* Monster is no longer friendly, remove their summon bonuses and species
    // This causes issues with infighting and I don't remember why I did this in the first place
    // Just comment it out and leave it alone for now
    if (!GetActorProperty(0, APROP_Friendly))
    {
        GiveInventory("DRPGFriendlyRemover", 1);
        SetActorPropertyString(0, APROP_Species, "");
    } */
    
    // Levelup handling via White Aura radius
    if (CheckInventory("DRPGMonsterLevelup"))
        MonsterLevelup(Stats);
    
    // Cap Level and Stats
    CapMonsterStats(Stats);
    
    // Strength
    if (Stats->Strength != OldStrength)
    {
        StatsChanged = true;
        
        SetActorPropertyFixed(0, APROP_DamageMultiplier, 1.0 + ((fixed)(Stats->Strength * (fixed)GameSkill()) / 100.0));
        OldStrength = Stats->Strength;
    }
    
    // Defense
    if (Stats->Defense != OldDefense)
    {
        StatsChanged = true;
        
        SetActorPropertyFixed(0, APROP_DamageFactor, 1.0 - (0.09 * ((fixed)Stats->Defense / 100.0)));
        OldDefense = Stats->Defense;
    }
    
    // Capacity
    if (Stats->Capacity != OldCapacity)
    {
        StatsChanged = true;
        
        int StolenCredits = 0;
        if (CheckInventory("DRPGCredits") > OldCapacity)
             StolenCredits = CheckInventory("DRPGCredits") - OldCapacity;
        
        SetInventory("DRPGCredits", Stats->Capacity + StolenCredits);
        OldCapacity = Stats->Capacity;
    }
    
    // Agility
    if (Stats->Agility != OldAgility)
    {
        StatsChanged = true;
        
        SetActorPropertyFixed(0, APROP_Speed, CalculateMonsterSpeed(Stats));
        OldAgility = Stats->Agility;
    }
    
    // Vitality
    if (Stats->Vitality != OldVitality)
    {
        StatsChanged = true;
        
        int HealthPercentage = 100;
        if (Stats->HealthMax > 0)
            HealthPercentage = (long long)GetActorProperty(0, APROP_Health) * 100 / Stats->HealthMax;
        Stats->HealthMax = CalculateMonsterMaxHealth(Stats);
        
        if (OldVitality == 0)
        {
            // This will trigger if the monster was previously dead and revived, so heal up fully
            SetActorProperty(0, APROP_Health, Stats->HealthMax);
        }
        else
        {
            SetActorProperty(0, APROP_Health, (long long)Stats->HealthMax * HealthPercentage / 100);
        }
        
        OldVitality = Stats->Vitality;
    }
    
    // Nothing much to do for these
    
    // Regeneration
    if (Stats->Regeneration != OldRegeneration)
    {
        StatsChanged = true;
        OldRegeneration = Stats->Regeneration;
    }
    
    // Energy
    if (Stats->Energy != OldEnergy)
    {
        StatsChanged = true;
        OldEnergy = Stats->Energy;
    }
    
    // Luck
    if (Stats->Luck != OldLuck)
    {
        StatsChanged = true;
        OldLuck = Stats->Luck;
    }
    
    if (StatsChanged)
    {
        // Re-calculate the monster's threat level
        Stats->Threat = CalculateMonsterThreatLevel(Stats);
        
        StatsChanged = false;
    }
    
    // Disruption timer
    if (CheckInventory("DRPGMonsterDisrupted"))
    {
        if (!MonsterWasDisrupted)
        {
            if (Stats->Aura.Type[AURA_WHITE].Active)
                GiveInventory("DRPGWhiteAuraRemover", 1);
            if (Stats->Aura.Type[AURA_RED].Active)
                GiveInventory("DRPGRedAuraRemover", 1);
            if (Stats->Aura.Type[AURA_GREEN].Active)
                GiveInventory("DRPGGreenAuraRemover", 1);
            if (Stats->Aura.Type[AURA_PINK].Active)
                GiveInventory("DRPGPinkAuraRemover", 1);
            if (Stats->Aura.Type[AURA_BLUE].Active)
                GiveInventory("DRPGBlueAuraRemover", 1);
            if (Stats->Aura.Type[AURA_ORANGE].Active) 
                GiveInventory("DRPGOrangeAuraRemover", 1);
            
            ActivatorSound("skills/disruption", 127);
            MonsterWasDisrupted = true;
        }
        
        TakeInventory("DRPGMonsterDisrupted", MonsterHasShadowAura(Stats) ? 12 : 4);
    }
    else
    {
        if (MonsterWasDisrupted)
        {
            if (Stats->Aura.Type[AURA_WHITE].Active)
                GiveInventory("DRPGWhiteAuraGiver", 1);
            if (Stats->Aura.Type[AURA_RED].Active)
                GiveInventory("DRPGRedAuraGiver", 1);
            if (Stats->Aura.Type[AURA_GREEN].Active)
                GiveInventory("DRPGGreenAuraGiver", 1);
            if (Stats->Aura.Type[AURA_PINK].Active)
                GiveInventory("DRPGPinkAuraGiver", 1);
            if (Stats->Aura.Type[AURA_BLUE].Active)
                GiveInventory("DRPGBlueAuraGiver", 1);
            if (Stats->Aura.Type[AURA_ORANGE].Active) 
                GiveInventory("DRPGOrangeAuraGiver", 1);
            
            MonsterWasDisrupted = false;
        }
    }
    
    Delay(4);
    goto Start;
}

NamedScript void MonsterAuraDisplayHandler()
{
    // Delay Stagger
    Delay(35 + (GetMonsterID(0) % 4));
    
    // Pointer
    MonsterStatsPtr Stats = &Monsters[GetMonsterID(0)];
    
    bool VisibleToPlayers;
    
    Start:
    
    if (GetActorProperty(0, APROP_Health) <= 0)
    {
        Delay(35);
        goto Start;
    }

    if (ClassifyActor(0) & ACTOR_WORLD)
        return;
    
    if (GetCVar("drpg_simple_auras"))
        VisibleToPlayers = true;
    else
    {
        VisibleToPlayers = false;
        
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (!PlayerInGame(i))
                continue;
            
            if (CheckSight(Players(i).TID, 0, CSF_NOBLOCKALL))
            {
                VisibleToPlayers = true;
                break;
            }
        }
    }
    
    if (!VisibleToPlayers && !InTitle)
    {
        Delay(4);
        goto Start;
    }
    
    // Spawn Auras
    if (MonsterHasAura(Stats) || (GetActorProperty(0, APROP_Friendly) && !CurrentLevel->UACBase))
        SpawnAuras(0, false);
    
    // Shadow Aura Effects
    if (MonsterHasShadowAura(Stats))
        SetActorProperty(0, APROP_RenderStyle, STYLE_Subtract);
    
    // Spawn Assassination Target
    if (Stats->Target > 0)
        SpawnForced("DRPGAssassinationIcon", GetActorX(0), GetActorY(0), GetActorZ(0) + GetActorPropertyFixed(0, APROP_Height) + 16.0 + (Sin(Timer() / 64.0) * 8.0), 0, 0);
    
    // Spawn Disruption Icon
    if (MonsterHasAura(Stats) && CheckInventory("DRPGMonsterDisrupted"))
        SpawnForced("DRPGDisruptionIcon", GetActorX(0), GetActorY(0), GetActorZ(0) + GetActorPropertyFixed(0, APROP_Height) + 16.0 + (Sin(Timer() / 64.0) * 8.0), 0, 0);
    
    if (ClassifyActor(0) & ACTOR_DEAD)
        return;
    
    Delay(1);
    goto Start;
}

NamedScript void MonsterLevelupHandler()
{
    if (CheckInventory("DRPGMonsterLevelupHandler"))
        return;
    
    GiveInventory("DRPGMonsterLevelupHandler", 1);
    
    // Delay Stagger
    Delay(35 + (GetMonsterID(0) % 4));
    
    int Energy;
    int DelayTime;
    
    // Pointer
    MonsterStatsPtr Stats = &Monsters[GetMonsterID(0)];
    
    Start:
    
    // Calculate the Delay Time
    Energy = Stats->Energy;
    DelayTime = 35 * (int)((60.0 - ((fixed)Clamp(0, Energy, 1000) / 16.75)));
    if (DelayTime < 35)
        DelayTime = 35;
    Delay(DelayTime);
    
    if (ClassifyActor(0) & ACTOR_WORLD)
        return;
    
    if (ClassifyActor(0) & ACTOR_DEAD)
    {
        Delay(35);
        goto Start;
    }
    
    if (!CheckInventory("DRPGMonsterLevelupHandler"))
        return;
    
    // Loop back to the top if the monster is max level
    if (Stats->Level >= 1000)
        goto Start;
    
    if (!Stats->Aura.Type[AURA_WHITE].Active || CheckInventory("DRPGMonsterDisrupted"))
        goto Start;
    
    // White Aura monsters levelup monsters near them
    if (Stats->Aura.Type[AURA_WHITE].Active && !CheckInventory("DRPGMonsterDisrupted"))
        for (int i = 0; i < MAX_PLAYERS; i++) // LOS Check on players
            if (CheckSight(0, Players(i).TID, 0))
            {
                if (!PlayerInGame(i)) continue;
                
                GiveInventory("DRPGMonsterLevelupGiver", 1);
                break;
            }
    
    goto Start;
}

NamedScript void MonsterEPDrainHandler()
{
    if (CheckInventory("DRPGMonsterEPDrainHandler"))
        return;
    
    GiveInventory("DRPGMonsterEPDrainHandler", 1);
    
    // Delay Stagger
    Delay(35 + (GetMonsterID(0) % 4));
    
    // Pointer
    MonsterStatsPtr Stats = &Monsters[GetMonsterID(0)];
    
    Start:
    
    if (ClassifyActor(0) & ACTOR_WORLD)
        return;
    
    if (ClassifyActor(0) & ACTOR_DEAD)
    {
        Delay(35);
        goto Start;
    }
    
    if (!CheckInventory("DRPGMonsterEPDrainHandler"))
        return;
    
    if (!Stats->Aura.Type[AURA_BLUE].Active || CheckInventory("DRPGMonsterDisrupted"))
    {
        Delay(35);
        goto Start;
    }
    
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        // Stupid hack to prevent stealing from triggering at the beginning of a level
        if (Timer() <= 4) break;
        
        if (!PlayerInGame(i)) continue;
        
        if (Distance(0, Players(i).TID) < Stats->Radius * GameSkill() * 4 + Stats->Radius && CheckSight(0, Players(i).TID, 0) && Players(i).EP > 0)
        {
            // Continue if this player is immune to stealing
            if ((Players(i).Aura.Type[AURA_PINK].Active && Players(i).Aura.Type[AURA_PINK].Level >= 3) || Players(i).SoulActive[SOUL_PINK]) continue;
            
            int Previous = Players(i).EP;
            
            // [KS] Okay, so at natural max, a player has 2000 EP. We want that to drain in 20 seconds rather than just instantly.
            // This should also make specialized Energy auras more tolerable in general.
            int DrainAmount = Stats->Energy / 10 * GetCVarFixed("drpg_aurasteal_amount");
            if (DrainAmount < 1)
                DrainAmount = 1;
            
            Players(i).EP -= DrainAmount;
            
            if (Players(i).EP < 0)
                Players(i).EP = 0;
            
            if (Players(i).EP < Previous)
            {
                Popoff(0, Previous - Players(i).EP, DNUM_EPGAIN, "DRPGDigit", true);
                
                int OrigTID = ActivatorTID();
                int NewTID = UniqueTID(0, 0);
                Thing_ChangeTID(0, NewTID);
                SetActivator(Players(i).TID);
                FadeRange(64, 128, 128, 0.5, 64, 128, 128, 0.0, 0.5);
                ActivatorSound("drain/ep", 32);
                SetActivator(NewTID);
                Thing_ChangeTID(0, OrigTID);
            }
        }
    }
    
    Delay(GetCVar("drpg_aurasteal_rate"));
    
    goto Start;
}

NamedScript void MonsterRegenerationHandler()
{
    if (CheckInventory("DRPGMonsterRegenerationHandler"))
        return;
    
    GiveInventory("DRPGMonsterRegenerationHandler", 1);
    
    // Delay Stagger
    Delay(35 + (GetMonsterID(0) % 4));
    
    // Pointer
    MonsterStatsPtr Stats = &Monsters[GetMonsterID(0)];
    
    Start:
    
    if (GetActorProperty(0, APROP_Health) <= 0)
    {
        Delay(35);
        goto Start;
    }

    if (ClassifyActor(0) & ACTOR_WORLD)
        return;
    
    if (!CheckInventory("DRPGMonsterRegenerationHandler"))
        return;
    
    long long RegenAmount = (long long)Stats->HealthMax * (long long)Stats->Regeneration;
    
    if (Stats->RegenHealth >= Stats->HealthMax * 10)
        RegenAmount /= 10000;
    else
        RegenAmount /= 1000;
    
    if (RegenAmount > INT_MAX)
        RegenAmount = INT_MAX;
    
    SetActorProperty(0, APROP_Health, GetActorProperty(0, APROP_Health) + RegenAmount);
    Stats->RegenHealth += RegenAmount;
    
    if (Stats->Aura.Type[AURA_PURPLE].Active && !CheckInventory("DRPGMonsterDisrupted"))
        GiveInventory("DRPGMonsterRadiusHealer", 1);
    
    // Prevent going over 100%
    if (GetActorProperty(0, APROP_Health) > Stats->HealthMax)
        SetActorProperty(0, APROP_Health, Stats->HealthMax);
    
    Delay(35 * (Stats->Aura.Type[AURA_PURPLE].Active ? 10 : 30));
    goto Start;
}

NamedScript DECORATE void MonsterRegenHealer()
{
    // Pointer
    MonsterStatsPtr Stats = &Monsters[GetMonsterID(0)];
    
    int Health = GetActorProperty(0, APROP_Health);
    
    int HealAmount = Stats->HealthMax / 10;
    if (HealAmount < 1)
        HealAmount = 1;
    
    if (Health + HealAmount > Stats->HealthMax)
        HealAmount = Stats->HealthMax - Health;
    
    if (HealAmount <= 0)
        return;
    
    GiveInventory("DRPGMonsterHealEffect", 1);
    SetActorProperty(0, APROP_Health, Health + HealAmount);
}

NamedScript void MonsterDamageRetaliationHandler()
{
    if (CheckInventory("DRPGMonsterDamageRetaliationHandler"))
        return;
    
    GiveInventory("DRPGMonsterDamageRetaliationHandler", 1);
    
    // Delay Stagger
    Delay(35 + (GetMonsterID(0) % 4));
    
    // Pointer
    MonsterStatsPtr Stats = &Monsters[GetMonsterID(0)];
    int Health = GetActorProperty(0, APROP_Health);
    int OldHealth = GetActorProperty(0, APROP_Health);
    
    Start:
    if (ClassifyActor(0) & ACTOR_WORLD)
        return;
    
    if (ClassifyActor(0) & ACTOR_DEAD)
    {
        Delay(35);
        goto Start;
    }
    
    if (!CheckInventory("DRPGMonsterDamageRetaliationHandler"))
        return;
    
    if (!Stats->Aura.Type[AURA_RED].Active || CheckInventory("DRPGMonsterDisrupted"))
    {
        Delay(35);
        goto Start;
    }
    
    Health = GetActorProperty(0, APROP_Health);
    
    if (Health < OldHealth)
    {
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (!PlayerInGame(i)) continue;
            if (!CheckSight(0, Players(i).TID, 0))  continue;
            
            fixed CurseChance = ((fixed)Stats->Strength / 10.0) - (Distance(0, Players(i).TID) / 5.12);
            
            if (CurseChance <= 0)
                continue;
            
            if (CurseChance >= 100.0 || RandomFixed(0.0, 100.0) <= CurseChance)
            {
                int Intensity = ((fixed)Stats->Strength / 200.0) - (Distance(0, Players(i).TID) / 102.4);
                if (Intensity < 1) Intensity = 1;
                int OrigTID = ActivatorTID();
                int NewTID = UniqueTID(0, 0);
                Thing_ChangeTID(0, NewTID);
                SetActivator(Players(i).TID);
                FadeRange(255, 0, 0, 0.5, 255, 0, 0, 0.0, 0.5);
                TryStatusEffect(SE_CURSE, Random(30,90), Intensity);
                SetActivator(NewTID);
                Thing_ChangeTID(0, OrigTID);
            }
        }
    }
    
    if (CheckInventory("DRPGMonsterEPAttacked"))
    {
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (!PlayerInGame(i)) continue;
            if (!CheckSight(0, Players(i).TID, 0)) continue;
            
            fixed SilenceChance = (Stats->Strength / 5) - (Distance(0, Players(i).TID) / 10.24);
            
            if (SilenceChance <= 0)
                continue;
            
            if (SilenceChance >= 100.0 || RandomFixed(0.0, 100.0) <= SilenceChance)
            {
                int Intensity = (Stats->Strength / 200) - (Distance(0, Players(i).TID) / 102.4);
                if (Intensity < 1) Intensity = 1;
                int OrigTID = ActivatorTID();
                int NewTID = UniqueTID(0, 0);
                Thing_ChangeTID(0, NewTID);
                SetActivator(Players(i).TID);
                FadeRange(255, 0, 0, 0.5, 255, 0, 0, 0.0, 0.5);
                TryStatusEffect(SE_SILENCE, Random(30,90), Intensity);
                SetActivator(NewTID);
                Thing_ChangeTID(0, OrigTID);
            }
        }
        
        SetInventory("DRPGMonsterEPAttacked", 0);
    }
    
    Delay(4);
    
    OldHealth = Health;
    
    goto Start;
}

NamedScript void MonsterMoneyDrainHandler()
{
    if (CheckInventory("DRPGMonsterMoneyDrainHandler"))
        return;
    
    GiveInventory("DRPGMonsterMoneyDrainHandler", 1);
    
    // Delay Stagger
    Delay(35 + (GetMonsterID(0) % 4));
    
    // Pointer
    MonsterStatsPtr Stats = &Monsters[GetMonsterID(0)];
    
    Start:
    if (ClassifyActor(0) & ACTOR_WORLD)
        return;
    
    if (ClassifyActor(0) & ACTOR_DEAD)
    {
        Delay(35);
        goto Start;
    }
    
    if (!CheckInventory("DRPGMonsterMoneyDrainHandler"))
        return;
    
    if (!Stats->Aura.Type[AURA_YELLOW].Active || CheckInventory("DRPGMonsterDisrupted"))
    {
        Delay(35);
        goto Start;
    }
    
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        // Stupid hack to prevent stealing from triggering at the beginning of a level
        if (Timer() <= 4) break;
        
        if (!PlayerInGame(i)) continue;
        
        if (Distance(0, Players(i).TID) < Stats->Radius * GameSkill() * 4 + Stats->Radius && CheckSight(0, Players(i).TID, 0))
        {
            // Continue if this player is immune to stealing
            if ((Players(i).Aura.Type[AURA_PINK].Active && Players(i).Aura.Type[AURA_PINK].Level >= 3) || Players(i).SoulActive[SOUL_PINK]) continue;
            
            int Previous = CheckActorInventory(Players(i).TID, "DRPGCredits");
            int OrigTID = ActivatorTID();
            int NewTID = UniqueTID(0, 0);
            Thing_ChangeTID(0, NewTID);
            TakeActorInventory(Players(i).TID, "DRPGCredits", (int)(Stats->Luck * GetCVarFixed("drpg_aurasteal_amount")));
            GiveInventory("DRPGCredits", (int)(Stats->Luck * GetCVarFixed("drpg_aurasteal_amount")));
            SetActivator(Players(i).TID);
            FadeRange(255, 255, 0, 0.5, 255, 255, 0, 0.0, 0.5);
            ActivatorSound("drain/money", 32);
            SetActivator(NewTID);
            Thing_ChangeTID(0, OrigTID);
            
            if (CheckActorInventory(Players(i).TID, "DRPGCredits") < Previous)
                Popoff(0, Previous - CheckActorInventory(Players(i).TID, "DRPGCredits"), DNUM_CREDGAIN, "DRPGDIGIT", true);
        }
    }
    
    Delay(GetCVar("drpg_aurasteal_rate"));
    
    goto Start;
}

NamedScript void MonsterAmmoDrainHandler()
{
    if (CheckInventory("DRPGMonsterAmmoDrainHandler"))
        return;
    
    GiveInventory("DRPGMonsterAmmoDrainHandler", 1);
    
    // Delay Stagger
    Delay(35 + (GetMonsterID(0) % 4));
    
    // Pointer
    MonsterStatsPtr Stats = &Monsters[GetMonsterID(0)];
    
    Start:
    if (ClassifyActor(0) & ACTOR_WORLD)
        return;
    
    if (ClassifyActor(0) & ACTOR_DEAD)
    {
        Delay(35);
        goto Start;
    }
    
    if (!CheckInventory("DRPGMonsterAmmoDrainHandler"))
        return;
    
    if (!Stats->Aura.Type[AURA_DARKBLUE].Active || CheckInventory("DRPGMonsterDisrupted"))
    {
        Delay(35);
        goto Start;
    }
    
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        // Stupid hack to prevent stealing from triggering at the beginning of a level
        if (Timer() <= 4) break;
        
        if (!PlayerInGame(i)) continue;
        
        if (Distance(0, Players(i).TID) < Stats->Radius * GameSkill() * 4 + Stats->Radius && CheckSight(0, Players(i).TID, 0))
        {
            // Continue if this player is immune to stealing
            if ((Players(i).Aura.Type[AURA_PINK].Active && Players(i).Aura.Type[AURA_PINK].Level >= 3) || Players(i).SoulActive[SOUL_PINK]) continue;
            
            // [KS] We'll give the player 30 seconds of ammo if they were full at stat max.
            int ClipSteal = (int)((fixed)Stats->Capacity / 7.5 * GetCVarFixed("drpg_aurasteal_amount"));
            int ShellSteal = (int)((fixed)Stats->Capacity / 30 * GetCVarFixed("drpg_aurasteal_amount"));
            int RocketSteal = (int)((fixed)Stats->Capacity / 60 * GetCVarFixed("drpg_aurasteal_amount"));
            int CellSteal = (int)((fixed)Stats->Capacity / 20 * GetCVarFixed("drpg_aurasteal_amount"));
            
            if (ClipSteal < 2) ClipSteal = 2;
            if (ShellSteal < 1) ShellSteal = 1;
            if (RocketSteal < 1) RocketSteal = 1;
            if (CellSteal < 3) CellSteal = 3;
            
            if (CheckActorInventory(Players(i).TID, "Clip") >= ClipSteal)
            {
                TakeActorInventory(Players(i).TID, "Clip",       ClipSteal);
                GiveInventory("Clip",       ClipSteal);
            }
            if (CheckActorInventory(Players(i).TID, "Shell") >= ShellSteal)
            {
                TakeActorInventory(Players(i).TID, "Shell",      ShellSteal);
                GiveInventory("Shell",      ShellSteal);
            }
            if (CheckActorInventory(Players(i).TID, "RocketAmmo") >= RocketSteal)
            {
                TakeActorInventory(Players(i).TID, "RocketAmmo", RocketSteal);
                GiveInventory("RocketAmmo", RocketSteal);
            }
            if (CheckActorInventory(Players(i).TID, "Cell") >= CellSteal)
            {
                TakeActorInventory(Players(i).TID, "Cell",       CellSteal);
                GiveInventory("Cell",       CellSteal);
            }
            
            int OrigTID = ActivatorTID();
            int NewTID = UniqueTID(0, 0);
            Thing_ChangeTID(0, NewTID);
            SetActivator(Players(i).TID);
            FadeRange(0, 0, 255, 0.5, 0, 0, 255, 0.0, 0.5);
            ActivatorSound("drain/ammo", 32);
            SetActivator(NewTID);
            Thing_ChangeTID(0, OrigTID);
        }
    }
    
    Delay(GetCVar("drpg_aurasteal_rate"));
    
    goto Start;
}

NamedScript void MonsterFellowResurrectionHandler()
{
    if (CheckInventory("DRPGMonsterFellowResurrectionHandler"))
        return;
    
    GiveInventory("DRPGMonsterFellowResurrectionHandler", 1);
    
    // Delay Stagger
    Delay(35 + (GetMonsterID(0) % 4));
    
    // Pointer
    MonsterStatsPtr Stats = &Monsters[GetMonsterID(0)];
    
    Start:
    if (ClassifyActor(0) & ACTOR_WORLD)
        return;
    
    if (ClassifyActor(0) & ACTOR_DEAD)
    {
        Delay(35);
        goto Start;
    }
    
    if (!CheckInventory("DRPGMonsterFellowResurrectionHandler"))
        return;
    
    if (!Stats->Aura.Type[AURA_PINK].Active || CheckInventory("DRPGMonsterDisrupted"))
    {
        Delay(35);
        goto Start;
    }
    
    GiveInventory("DRPGMonsterRadiusResurrection", 1);
    
    Delay(35 * (30 - (20 * Clamp(0, Stats->Vitality, 1000) / 1000)));
    
    goto Start;
}

NamedScript void MonsterEpicVisitTimeHandler()
{
    if (CheckInventory("DRPGMonsterEpicVisitTimeHandler"))
        return;
    
    GiveInventory("DRPGMonsterEpicVisitTimeHandler", 1);
    
    // Delay Stagger
    Delay(35 + (GetMonsterID(0) % 4));
    
    // Pointer
    MonsterStatsPtr Stats = &Monsters[GetMonsterID(0)];
    
    Start:
    if (ClassifyActor(0) & ACTOR_WORLD)
        return;
    
    if (ClassifyActor(0) & ACTOR_DEAD)
    {
        Delay(35);
        goto Start;
    }
    
    if (!CheckInventory("DRPGMonsterEpicVisitTimeHandler"))
        return;
    
    if (!Stats->Aura.Type[AURA_ORANGE].Active || CheckInventory("DRPGMonsterDisrupted"))
    {
        Delay(35);
        goto Start;
    }
    
    fixed TeleportDistance = 0;
    int TeleportPlayer = -1;
    
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!PlayerInGame(i)) continue;
        if (!CheckSight(0, Players(i).TID, 0)) continue;
        if (Distance(0, Players(i).TID) <= TeleportDistance) continue;
        
        TeleportDistance = Distance(0, Players(i).TID);
        TeleportPlayer = i;
    }
    
    fixed TeleportChance = ((fixed)TeleportDistance / 40.96) + (25.0 * (Stats->Agility / 1000.0)) ;
    
    if (RandomFixed(0.0, 100.0) <= TeleportChance)
    {
        int TeleportSpotTID = UniqueTID();
        int TargetPlayerTID = Players(TeleportPlayer).TID;
        
        bool Success = false;
        fixed TeleportAngle;
        fixed TeleportDistance;
        fixed X, Y, Z, Angle;
        
        for (int i = 0; i < 10; i++)
        {
            fixed TeleportAngle = RandomFixed(0, 1);
            fixed TeleportDistance = RandomFixed(128, 384);
            
            X = GetActorX(TargetPlayerTID) + (Cos(TeleportAngle) * TeleportDistance);
            Y = GetActorY(TargetPlayerTID) + (Sin(TeleportAngle) * TeleportDistance);
            Z = GetActorZ(TargetPlayerTID);
            Angle = TeleportAngle + 0.5;
            Angle %= 1.0;
            
            Success = Spawn("MapSpot", X, Y, Z, TeleportSpotTID, Angle);
            if (Success && !SetActorPosition(TeleportSpotTID, GetActorX(TeleportSpotTID), GetActorY(TeleportSpotTID), GetActorFloorZ(TeleportSpotTID), false)) Success = false;
            if (Success && !CheckSight(TargetPlayerTID, TeleportSpotTID, 0)) Success = false;
            if (Success)
                break;
        }
        
        if (Success)
        {
            // Surprise!!
            SpawnSpotFacingForced("TeleportFog", 0, 0);
            SetActorPosition(0, GetActorX(TeleportSpotTID), GetActorY(TeleportSpotTID), GetActorZ(TeleportSpotTID), false);
            SetActorAngle(0, Angle);
            SpawnSpotFacingForced("TeleportFog", 0, 0);
            Thing_Remove(TeleportSpotTID);
        }
    }
    
    Delay(35 * 10);
    
    goto Start;
}

NamedScript void MonsterDeathCheck()
{
    // Delay Stagger
    Delay(35 + (GetMonsterID(0) % 4));
    
    Delay(1);
    MonsterStatsPtr Stats = &Monsters[GetMonsterID(0)];

    Start:
    
    if (ClassifyActor(0) & ACTOR_WORLD)
        goto Disappeared;
    
    if (GetActorProperty(0, APROP_Health) > 0)
    {
        Delay(4);
        goto Start;
    }

    MonsterDeath();
    
    PostDeathWait:
    
    Delay(35);
    
    if (ClassifyActor(0) & ACTOR_WORLD)
        goto Disappeared;
    
    if (GetActorProperty(0, APROP_Health) > 0)
        goto Start;
    
    goto PostDeathWait;
    
    Disappeared:
    
    Stats->Init = false; // Mark as unused
    Stats->TID = 0;
}

NamedScript void MonsterDeath()
{
    // Pointer
    MonsterStatsPtr Stats = &Monsters[GetMonsterID(0)];
    
    int Killer = WhoKilledMe();
    long int XPAmount = Random(GetActorProperty(0, APROP_SpawnHealth) / 2.0, GetActorProperty(0, APROP_SpawnHealth)) * (1 + (Stats->Threat - (Stats->Threat > 0 ? 1 : 0)));;
    long int RankAmount = GetActorProperty(0, APROP_SpawnHealth) * (1 + (Stats->Threat - (Stats->Threat > 0 ? 1 : 0)));
    
    // Aura-Based XP/Rank Modifiers
    if (MonsterHasShadowAura(Stats))
    {
        XPAmount *= 2;
        RankAmount *= 4;
    }
    else if (Stats->Aura.Type[AURA_WHITE].Active)
        XPAmount *= 2;
    
    if (Players(Killer).Shield.Accessory)
    {
        switch (Players(Killer).Shield.Accessory->PassiveEffect)
        {
        case SHIELD_PASS_KILLSCHARGE:
            AddRemoteShield(Players(Killer).TID, Stats->HealthMax / 10);
            break;
        case SHIELD_PASS_BLOODYSHIELDSOREAL:
            Players(Killer).Shield.AccessoryBattery = 35 * 3;
            break;
        }
    }
    
    // Credit Rain
    if (Killer > -1 && (Players(Killer).Luck >= 100 || CurrentLevel->Event == MAPEVENT_BONUS_RAINBOWS) && Random(1, 256) == 1)
    {
        fixed Z = GetActorCeilingZ(Players(Killer).TID) - GetActorPropertyFixed(Players(Killer).TID, APROP_Height);
        fixed Radius = GetActorPropertyFixed(Players(Killer).TID, APROP_Radius);
        
        ThingSound(Players(Killer).TID, "menu/buy", 127);
        Log("\CdLUDICROUS CREDITS!");
        
        for (int i = 0; i < 100; i++)
        {
            int CreditType = Random(1, 6);
            str CreditName;
            
            switch (CreditType)
            {
            case 1: CreditName = "DRPGCredits1";    break;
            case 2: CreditName = "DRPGCredits5";    break;
            case 3: CreditName = "DRPGCredits10";   break;
            case 4: CreditName = "DRPGCredits20";   break;
            case 5: CreditName = "DRPGCredits50";   break;
            case 6: CreditName = "DRPGCredits100";  break;
            }
            
            DropMonsterItem(Killer, Players(Killer).TID, CreditName, 256, RandomFixed(-Radius, Radius), RandomFixed(-Radius, Radius), Z);
            Delay(1);
        }
    }
    
    // Add XP/Rank
    if (!(Stats->Flags & MF_NOXP) && !GetActorProperty(0, APROP_Friendly))
    {
        // Take Rank if the enemy has MF_RANKLOSS
        if (Stats->Flags & MF_RANKLOSS)
            RankAmount = -RankAmount;
        
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (!PlayerInGame(i)) continue;
            
            if (Killer == i)
            {
                AddXP(i, XPAmount, RankAmount);
                if (GetCVar("drpg_levelup_natural") && !Players(i).Stim.Active)
                {
                    fixed Scale = GetCVarFixed("drpg_strength_scalexp");
                    if (GetCVar("drpg_allow_spec"))
                    {
                        if (GetActivatorCVar("drpg_character_spec") == 1)
                            Scale *= 2;
                    }
                    Players(i).StrengthXP += (int)(XPAmount * Scale);
                }
            }
            else
                AddXP(i, XPAmount / 3, 0);
        }
    }
    
    // Add a kill to the payout kills
    if (Killer > -1 && !(Stats->Flags & MF_NOPAYKILL))
    {
        int PPKills = 1;
        if (Stats->Flags & MF_BOSS)
            PPKills = 4;
        else if (Stats->Flags & MF_MEGABOSS)
            PPKills = 8;
        
        if (Players(Killer).Shield.Accessory && Players(Killer).Shield.Accessory->PassiveEffect == SHIELD_PASS_EPICMEGACASH && Players(Killer).Shield.Active)
            PPKills *= 3;
        
        Players(Killer).Payout.Kills += PPKills;
    }
    
    // Mission Handling
    MissionDeathCheck(Killer, Stats);
    
    // SHOTGUNS!
    if (GetCVar("drpg_shotguns"))
        DropMonsterItem(Killer, 0, "DRPGShotSpawn", 1, 256);
    
    // Drops
    if (!(Stats->Flags & MF_NODROPS) && !GetActorProperty(0, APROP_Friendly) && !GetCVar("drpg_monster_shadows"))
    {
        // Auras have a chance of rare vial drops
        for (int i = 0; i < AURA_MAX; i++)
            if (Stats->Aura.Type[i].Active)
                DropMonsterItem(Killer, 0, "DRPGVialDropperRare", 51);
        
        // Aura Drops
        if (Stats->Aura.Type[AURA_RED].Active) // Red Aura - Strength
            DropMonsterItem(Killer, 0, "DRPGVialStrength", 256);
        if (Stats->Aura.Type[AURA_GREEN].Active) // Green Aura - Defense
            DropMonsterItem(Killer, 0, "DRPGVialDefense", 256);
        if (Stats->Aura.Type[AURA_WHITE].Active) // White Aura - XP
            DropMonsterItem(Killer, 0, "DRPGStimDropper", 256);
        if (Stats->Aura.Type[AURA_PINK].Active) // Pink Aura - Vitality
            DropMonsterItem(Killer, 0, "DRPGVialVitality", 256);
        if (Stats->Aura.Type[AURA_BLUE].Active) // Blue Aura - Energy
            DropMonsterItem(Killer, 0, "DRPGVialEnergy", 256);
        if (Stats->Aura.Type[AURA_PURPLE].Active) // Purple Aura - Regeneration
            DropMonsterItem(Killer, 0, "DRPGVialRegeneration", 256);
        if (Stats->Aura.Type[AURA_ORANGE].Active) // Orange Aura - Agility
            DropMonsterItem(Killer, 0, "DRPGVialAgility", 256);
        if (Stats->Aura.Type[AURA_DARKBLUE].Active) // Dark Blue Aura - Capacity
            DropMonsterItem(Killer, 0, "DRPGVialCapacity", 256);
        if (Stats->Aura.Type[AURA_YELLOW].Active) // Yellow Aura - Luck
            DropMonsterItem(Killer, 0, "DRPGVialLuck", 256);
        
        // Luck-based Drops
        if (Killer > -1 && !(Stats->Flags & MF_MEGABOSS))
        {
            if (Players(Killer).HealthDrop && RandomFixed(0.0, 100.0) < Players(Killer).HealthChance)    DropMonsterItem(Killer, 0, "DRPGHealthDropper", 256);
            if (Players(Killer).EPDrop && RandomFixed(0.0, 100.0) < Players(Killer).EPChance)            DropMonsterItem(Killer, 0, "DRPGEPCapsule", 256);
            if (Players(Killer).ArmorDrop && RandomFixed(0.0, 100.0) < Players(Killer).ArmorChance)      DropMonsterItem(Killer, 0, "DRPGArmorDropper", 256);
            if (Players(Killer).WeaponDrop && RandomFixed(0.0, 100.0) < Players(Killer).WeaponChance)    DropMonsterItem(Killer, 0, "DRPGWeaponDropper", 256);
            if (Players(Killer).PowerupDrop && RandomFixed(0.0, 100.0) < Players(Killer).PowerupChance)  DropMonsterItem(Killer, 0, "DRPGPowerupDropper", 256);
            if (Players(Killer).StimDrop && RandomFixed(0.0, 100.0) < Players(Killer).StimChance)        DropMonsterItem(Killer, 0, "DRPGVialDropperRare", 256);
            if (Players(Killer).ModuleDrop && RandomFixed(0.0, 100.0) < Players(Killer).ModuleChance)    DropMonsterItem(Killer, 0, "DRPGModuleDropper", 256);
            if (Players(Killer).ShieldDrop && RandomFixed(0.0, 100.0) < Players(Killer).ShieldChance)    DropMonsterItem(Killer, 0, "DRPGShieldDropper", 256);
            if (Players(Killer).AugDrop && RandomFixed(0.0, 100.0) < Players(Killer).AugChance)          DropMonsterItem(Killer, 0, "DRPGAugDropper", 256);
        }
        
        Delay(1);
        
        // Boss Drops
        if (Stats->Flags & MF_BOSS)
        {
            DropMonsterItem(Killer, 0, "DRPGCredits1000", 256);
            DropMonsterItem(Killer, 0, "DRPGSoulsphereRandomizer", 256);
            DropMonsterItem(Killer, 0, "DRPGLifeDropper", 128);
            DropMonsterItem(Killer, 0, "DRPGModuleDropper", 256);
            DropMonsterItem(Killer, 0, "DRPGAugDropper", 128);
            DropMonsterItem(Killer, 0, "DRPGShieldDropper", 128);
            DropMonsterItem(Killer, 0, "DRPGUACCard", 128 / (Players(Killer).ShopCard + 1));
            DropMonsterItem(Killer, 0, "DRPGImmunityCrystalDropper", 8);
        }
        
        // Megaboss Drops
        if (Stats->Flags & MF_MEGABOSS)
        {
            AmbientSound("megaboss/lootsplosion", 127);
            Delay(35 * 2.25); // Just long enough to sync up with the sound nicely
            
            // LOOTSPLOOOOOOOOOOOOOSIIIOOOOOOOOOOOOOONNNNNN
            int Rolls = 200 - (GameSkill() * 25);
            while (Rolls--)
            {
                DropMonsterItem(Killer, 0, "DRPGCredits1000", 256, 0, 0, 0, 16, 16, Random(8, 16));
                for (int i = 0; i < PlayerCount(); i++)
                    DropMonsterItem(Killer, 0, "DRPGGenericBossDropper", 256, 0, 0, 0, 16, 16, Random(8, 16));
                if ((Rolls % 5) == 0)
                    Delay(1);
            }
            
            DropMonsterItem(Killer, 0, "DRPGDiamondUACCard", 256);
        }
    }
    
    // Drop Radiation Suits during the Environmental Hazard event
    if (!(Stats->Flags & MF_NODROPS) && !GetActorProperty(0, APROP_Friendly) && CheckInventory("DRPGMonsterRadSuit"))
        DropMonsterItem(Killer, 0, "RadSuit", 256);
    
    // Drop Nuke Keys during the Thermonuclear Bomb Map Event
    if (!(Stats->Flags & MF_NODROPS) && !GetActorProperty(0, APROP_Friendly) && CurrentLevel->Event == MAPEVENT_NUCLEARBOMB && (Random(1, 2) == 1 || Stats->Flags & MF_BOSS || Stats->Flags & MF_MEGABOSS))
        DropMonsterItem(Killer, 0, StrParam("DRPGNukeKey%d", Random(1, MAX_NUKE_KEYS)), 256);
    
    // Drop Generator Power Cells during the Low Power Event
    if (!(Stats->Flags & MF_NODROPS) && !GetActorProperty(0, APROP_Friendly) && CurrentLevel->Event == MAPEVENT_LOWPOWER && !CurrentLevel->PowerGeneratorActive)
        if (Random(1, CurrentLevel->MaxTotalMonsters / 4) == 1)
            DropMonsterItem(Killer, 0, "DRPGGeneratorCell", 256);
        else
            DropMonsterItem(Killer, 0, "DRPGGeneratorCellDead", 128);
    
    // Drop Credits
    if (!(Stats->Flags & MF_NODROPS) && CheckInventory("DRPGCredits") > 0)
    {
        int LuckMult = 100 + Players(Killer).Luck;
        int CreditsMin = (CheckInventory("DRPGCredits") * LuckMult) / 1000;
        int CreditsMax = (CheckInventory("DRPGCredits") * LuckMult) / 100;
        int CreditsAmount = Random(CreditsMin, CreditsMax);
        
        // RAINBOWS Event
        if (CurrentLevel->Event == MAPEVENT_BONUS_RAINBOWS)
            CreditsAmount *= 4;
        
        // Log("\CfInitial Amount: %d\n\CfLuck Mult: %d\n\CfMin: %d\n\CfMax: %d\n\CfAmount: %d", CheckInventory("DRPGCredits"), LuckMult, CreditsMin, CreditsMax, CreditsAmount);
        DropMoney(Killer, 0, CreditsAmount);
    }
    
    // Drop stolen ammo
    int Count = 0;
    while (CheckInventory("Clip") >= 10)
    {
        DropMonsterItem(Killer, 0, "DRPGStolenClip", 256);
        TakeInventory("Clip", 10);
        if ((Count++ % 1000) == 0) Delay(1);
    }
    while (CheckInventory("Shell") >= 4)
    {
        DropMonsterItem(Killer, 0, "DRPGStolenShell", 256);
        TakeInventory("Shell", 4);
        if ((Count++ % 1000) == 0) Delay(1);
    }
    while (CheckInventory("RocketAmmo") >= 1)
    {
        DropMonsterItem(Killer, 0, "DRPGStolenRocketAmmo", 256);
        TakeInventory("RocketAmmo", 1);
        if ((Count++ % 1000) == 0) Delay(1);
    }
    while (CheckInventory("Cell") >= 20)
    {
        DropMonsterItem(Killer, 0, "DRPGStolenCell", 256);
        TakeInventory("Cell", 20);
        if ((Count++ % 1000) == 0) Delay(1);
    }
    
    // [KS] We killed this monster now, so if an Arch-vile resurrects it, it doesn't count anymore to re-kill
    // TODO: Need to maybe make sure that for Kill Missions, we can't just gimp Arch-vile resurrection.
    Stats->Flags |= MF_NOXP;
    Stats->Flags |= MF_NODROPS;
    SetActorProperty(0, APROP_RenderStyle, Stats->RenderStyle); // In case of Shadow Aura
    Stats->Target = false;
    Stats->Reinforcement = false;
    
    // [KS] Nuke auras so we don't cause bizarre problems
    // [KS] 5/11/2015 - Actually shouldn't shouldn't anymore, uncomment it if it still does
    //RemoveMonsterAura(Stats);
}

//[[alloc_Aut(16384)]]
NamedScript DECORATE void MonsterTransport(int Difficulty, int Time, int Radius)
{
    bool DRLA = (CompatMode == COMPAT_DRLA);
    int MonsterDataAmount;
    MonsterInfoPtr MonsterList[MAX_TEMP_MONSTERS];
    int MonsterListLength;
    int BossesSpawned = 0;
    
    MonsterInfoPtr TempMonster;
    
    // Build a list of monsters
	if (ColourfulHell)
        MonsterDataAmount = MAX_DEF_MONSTERS_CH;
    else if (DRLA)
        MonsterDataAmount = MAX_DEF_MONSTERS_DRLA;
    else
        MonsterDataAmount = MAX_DEF_MONSTERS;
    
    for (int i = 0; i < MonsterDataAmount && MonsterListLength < MAX_TEMP_MONSTERS; i++)
    {
		if (ColourfulHell)
			TempMonster = &MonsterDataCH[i];
        else if (DRLA)
            TempMonster = &MonsterDataDRLA[i];
        else if (CompatMode == COMPAT_LEGENDOOM)
            TempMonster = &MonsterDataLD[i];
        else
            TempMonster = &MonsterData[i];
    
        if (TempMonster->Difficulty >= ((Difficulty + 1) * 10) - 40 &&
            TempMonster->Difficulty <= ((Difficulty + 1) * 10) + 10)
            MonsterList[MonsterListLength++] = TempMonster;
    }
    
    if (!MonsterListLength)
    {
        Log("\CiWARNING: \CaReinforcement Mission on %S\Ca has no monsters!", MissionDifficulties[Difficulty]);
        return;
    }
    
    bool Complete, Success, IsBoss;
    fixed X, Y, Z, SpawnX, SpawnY;
    int MonsterIndex, TID, SpawnTries, CurrentRadius;
    MonsterStatsPtr Stats;
    
    while (true)
    {
        Delay((Time > 0 ? (35 * Time) : -Time));
        
        Complete = true;
        X = GetActorX(0);
        Y = GetActorY(0);
        Z = GetActorZ(0);
        
        // Stop spawning if time is frozen
        while (IsTimeFrozen()) Delay(1);
        
        // Check to see if the mission is complete
        for (int i = 0; i < MAX_PLAYERS; i++)
            if (PlayerInGame(i) && Players(i).Mission.Active && Players(i).Mission.Type == MT_REINFORCEMENTS)
                Complete = false;
        
        // If the mission is complete, terminate
        if (Complete) return;
        
        TID = UniqueTID();
        Success = false;
        SpawnTries = 0;
        CurrentRadius = Radius;
        Stats = NULL;
        IsBoss = false;
        
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
            
            IsBoss = CheckFlag(TID, "BOSS");
            
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
            // Delay here to make sure the init script has time to run
            Delay(4);
            
            // Pointer
            Stats = &Monsters[GetMonsterID(TID)];
            
            Thing_Hate(TID, Player.TID);
            Stats->Reinforcement = true;
            Thing_ChangeTID(TID, 0); // Get rid of the ID

            Spawn("TeleportFog", X + SpawnX, Y + SpawnY, Z, 0, 0);
            
            if (GetCVar("drpg_debug"))
                Log("\CdDEBUG: \C-Reinforcements Spawning Index %d (\Ca%S\C-)", MonsterIndex, MonsterList[MonsterIndex]->Name);
        }
    }
}

NamedScript int WhoKilledMe()
{
    SetActivatorToTarget(0);
    
    // Killed by a player
    if (PlayerNumber() > -1)
        return PlayerNumber();
    
    // Killed by a player's turret
    for (int i = 0; i < MAX_PLAYERS; i++)
        if (ActivatorTID() == Players(i).Turret.TID)
            return i;
    
    // Killed by a player's summons
    if (ClassifyActor(0) & ACTOR_MONSTER && GetMonsterID(0) && GetActorProperty(0, APROP_MasterTID))
    {
        SetActivator(GetActorProperty(0, APROP_MasterTID));
        return PlayerNumber();
    }
    
    // Killed by another monster or the world
    return -1;
}

NamedScript int WhoShotMe()
{
    SetActivator(0, AAPTR_TARGET);
    
    if (GetActorProperty(0, APROP_Health) <= 0)
        return -1;
    
    return ActivatorTID();
}

void MonsterLevelup(MonsterStatsPtr Stats)
{
    // If the monster is max level, return
    if (Stats->Level >= 1000) return;
    
    // Apply the stats to the monster
    Stats->Level++;
    
    int Pool = GameSkill();
    
    while (Pool > 0)
    {
        switch (Random (0, STAT_MAX - 1))
        {
            case 0:
                Stats->Strength++;
                break;
            case 1:
                Stats->Defense++;
                break;
            case 2:
                Stats->Vitality++;
                break;
            case 3:
                Stats->Energy++;
                break;
            case 4:
                Stats->Regeneration++;
                break;
            case 5:
                Stats->Agility++;
                break;
            case 6:
                Stats->Capacity++;
                break;
            case 7:
                Stats->Luck++;
                break;
        }
        Pool--;
    }
    
    // Cap the stats
    CapMonsterStats(Stats);
    
    // Spawn Level-up Arrow
    SpawnForced("DRPGLevelUpArrow", GetActorX(0), GetActorY(0), GetActorZ(0) + GetActorPropertyFixed(0, APROP_Height), 0, 0);
    
    if (CheckInventory("DRPGMonsterLevelup"))
        TakeInventory("DRPGMonsterLevelup", 1);
}

void CapMonsterStats(MonsterStatsPtr Stats)
{
    if (Stats->Level <= 0)
        Stats->Level = 1;
    if (Stats->Level > 1000)
        Stats->Level = 1000;
    if (Stats->Strength > 1000)
        Stats->Strength = 1000;
    if (Stats->Defense > 1000)
        Stats->Defense = 1000;
    if (Stats->Vitality > 1000)
        Stats->Vitality = 1000;
    if (Stats->Energy > 1000)
        Stats->Energy = 1000;
    if (Stats->Regeneration > 1000)
        Stats->Regeneration = 1000;
    if (Stats->Agility > 1000)
        Stats->Agility = 1000;
    if (Stats->Capacity > 1000)
        Stats->Capacity = 1000;
    if (Stats->Luck > 1000)
        Stats->Luck = 1000;
}

int CalculateMonsterThreatLevel(MonsterStatsPtr Stats)
{
    int Threat;
    
    // Stats
    Threat += Stats->Strength;
    Threat += Stats->Defense;
    Threat += Stats->Vitality;
    Threat += Stats->Energy;
    Threat += Stats->Regeneration;
    Threat += Stats->Agility;
    Threat += Stats->Capacity;
    Threat += Stats->Luck;
    Threat /= 800;
    
    // Auras
    for (int i = 0; i < AURA_MAX; i++)
        if (Stats->Aura.Type[i].Active)
            Threat++;
    
    // Bosses
    if (CheckFlag(0, "BOSS") || Stats->Flags & MF_BOSS)
        Threat++;
    // Megaboss
    if (Stats->Flags & MF_MEGABOSS)
        Threat += 2;
    
	//CH
	if (ColourfulHell)
	{
        str Actor = GetActorClass(0);
        
        for (int i = 0; i < MAX_DEF_MONSTERS_CH; i++)
        {
            MonsterInfoPtr MonsterIterPtr = &MonsterDataCH[i];
            
            if (StartsWith(Actor, MonsterIterPtr->Actor))
            {
                Threat += MonsterIterPtr->ThreatLevel;
                break;
            }
        }
    }
    // DRLA
    else if (CompatMode == COMPAT_DRLA)
    {
        str Actor = GetActorClass(0);
        
        for (int i = 0; i < MAX_DEF_MONSTERS_DRLA; i++)
        {
            MonsterInfoPtr MonsterIterPtr = &MonsterDataDRLA[i];
            
            if (StartsWith(Actor, MonsterIterPtr->Actor))
            {
                Threat += MonsterIterPtr->ThreatLevel;
                break;
            }
        }
    }
    
    // Cap
    if (Threat > 10)
        Threat = 10;
    
    return Threat;
}

int CalculateMonsterMaxHealth(MonsterStatsPtr Stats)
{
    long int Health = Stats->SpawnHealth;
    long int HealthAddition = Stats->SpawnHealth * 9;
    
    HealthAddition *= Stats->Vitality;
    HealthAddition /= 1000;
    
    Health += HealthAddition;
    
    return Health;
}

fixed CalculateMonsterSpeed(MonsterStatsPtr Stats)
{
    fixed Speed = (fixed)Stats->Speed;
    fixed SpeedMax = Stats->Radius - 1.0;
    
    Speed *= 1.0 + ((fixed)Stats->Agility / 1000.0 * 4.0);
    if (Speed > SpeedMax)
        Speed = SpeedMax;
    
    return Speed;
}

void RemoveMonsterAura(MonsterStatsPtr Stats)
{
    Stats->Aura.Time = 0;
    Stats->Aura.Team = false;
    
    for (int i = 0; i < AURA_MAX; i++)
    {
        Stats->Aura.Type[i].Active = false;
        Stats->Aura.Type[i].Level = 0;
    };
    
    // Flag Removers
    GiveInventory("DRPGRedAuraRemover", 1);
    GiveInventory("DRPGGreenAuraRemover", 1);
    GiveInventory("DRPGWhiteAuraRemover", 1);
    GiveInventory("DRPGPinkAuraRemover", 1);
    GiveInventory("DRPGBlueAuraRemover", 1);
    GiveInventory("DRPGOrangeAuraRemover", 1);
    
    // Reset Renderstyle
    SetActorProperty(0, APROP_RenderStyle, Stats->RenderStyle);
}

str DetermineBestStatColor(MonsterStatsPtr Stats)
{
    str const StatColorsM[STAT_MAX] =
    {
        "\Cg",
        "\Cd",
        "\Ca",
        "\Cn",
        "\Ct",
        "\Ci",
        "\Ch",
        "\Cf"
    };
    
    int BaseStat[STAT_MAX];
    int HighestStat = 0;
    int HighestStatType = 0;
    
    if (Stats->Flags & MF_NOSTATS)
        return "\Cc";
    
    BaseStat[0] = Stats->Strength;
    BaseStat[1] = Stats->Defense;
    BaseStat[2] = Stats->Vitality;
    BaseStat[3] = Stats->Energy;
    BaseStat[4] = Stats->Regeneration;
    BaseStat[5] = Stats->Agility;
    BaseStat[6] = Stats->Capacity;
    BaseStat[7] = Stats->Luck;
    
    for (int i = 0; i < STAT_MAX; i++)
        if (BaseStat[i] > HighestStat)
        {
            HighestStat = BaseStat[i];
            HighestStatType = i;
        }
    
    return StatColorsM[HighestStatType];
}

int BestStat(MonsterStatsPtr Stats)
{
    int BaseStat[STAT_MAX];
    int HighestStat = 0;
    int HighestStatType = 0;
    
    if (Stats->Flags & MF_NOSTATS)
        return Random(0, STAT_MAX);
    
    BaseStat[0] = Stats->Strength;
    BaseStat[1] = Stats->Defense;
    BaseStat[2] = Stats->Vitality;
    BaseStat[3] = Stats->Energy;
    BaseStat[4] = Stats->Regeneration;
    BaseStat[5] = Stats->Agility;
    BaseStat[6] = Stats->Capacity;
    BaseStat[7] = Stats->Luck;
    
    for (int i = 0; i < STAT_MAX; i++)
        if (BaseStat[i] > HighestStat)
        {
            HighestStat = BaseStat[i];
            HighestStatType = i;
        }
    
    return HighestStatType;
}

int FindMonster(int TID)
{
    for (int i = 1; i < MonsterID; i++)
    {
        if (!Monsters[i].Init)
            continue;
        
        if (Monsters[i].TID == TID)
            return i;
    }
    
    return -1;
}

// For Debugging
NamedScript Console void OutputMonsters()
{
    for (int i = 1; i < MonsterID; i++)
    {
        //Log("\CdMonster %d:", i);
        //Output Monsters[i];
    }
}
