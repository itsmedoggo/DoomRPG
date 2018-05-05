#include "RPG.h"

#include "Monsters.h"
#include "Namegen.h"
#include "Utils.h"

str SharedPool[NAME_MAX_SHARED] =
{
    "Terrible",
    "Awful",
    "Hideous",
    "Merciless",
    "Stinky",
    "Violent",
    "Vengeful",
    "Radiant",
    "Dark",
    "Almighty",
    "Murderous",
    "Godslayer",
    "Darkwalker",
    "Burning",
    "Bloodthirsty",
    "Hungry",
    "Gravedigger",
    "Hellraiser",
    "Torturer",
    "Motionless",
    "Seething",
    "Pungent",
    "Gluttonous",
    "Greedy",
    "Murderous",
    "Writhing",
    "Agonizer",
    "Pain-bringer",
    "Wicked",
    "Dreadful",
    "Mad",
    "Insidious",
    "Shredder",
    "Destructor",
    "Jagged",
    "Terrifying",
    "Undying",
    "Wretched",
    "Diseased",
    "Heartless",
    "Bonecrusher",
    "Bane",
    "Ravenous",
    "Baleful",
    "Accursed",
    "Profane",
    "Harbinger",
    "Rebellious",
    "Feculent",
    "Pestilent",
    "Trickster",
    "Conniving",
    "Craven",
    "Nasty",
    "Vicious",
    "Evil",
    "Fallen",
    "Vile",
    "Rotten",
    "Shameful",
    "Fiendish",
    "Foul",
    "Abominable",
    "Odious",
    "Abhorrent",
    "Disgusting",
    "Horrid",
    "Loathsome",
    "Blighted",
    "Repugnant",
    "Repulsive",
    "Revolting",
    "Creepy",
    "Detestable",
    "Horrifying",
    "Appalling",
    "Daunting",
    "Dauntless",
    "Sickening",
    "Brutal",
    "Fierce",
    "Savage",
    "Disturbed",
    "Enraged",
    "Disturbing",
    "Maniacal",
    "Enduring",
    "Interminable",
    "Unfading",
    "Immortal",
};

str DemonNames[NAME_MAX_DEMON_NAMES] =
{
    "Gorgoth",
    "Merdiklo",
    "Starface",
    "Mar'khi",
    "Veritt",
    "Modoch",
    "Vup",
    "Helliox",
    "Warrdeth",
    "Killzor",
    "Ikhon",
    "Manslayer",
    "Fismit",
    "Killinger",
    "Eradicus",
    "Mortarr",
    "Pox",
    "Murdercles",
    "Jerkules",
    "Haddrox",
    "Persecon",
    "Errexioth",
    "Satan Jr.",
    "Makob",
    "Vorbenix",
    "Norvenica",
    "Mek'tethel",
    "Vex'genn",
    "Horoxoloth",
    "Chaotica",
    "Hex'vorr",
    "Legetto",
    "Fauror",
    "Kormat",
    "Mogaltu",
    "Shel",
    "Vameth",
    "Dethul",
    "Ginavak",
    "Kantul",
    "Korthaet",
    "Laniaro",
    "Lortuc",
    "Morcatu",
    "Ndur",
    "Saugur",
    "Vathan",
    "Caethind",
    "Hirthe",
    "Inchindr",
    "Kaelarmaul",
    "Koritu",
    "Nakor",
    "Ngugobal",
    "Obair",
    "Rorgia",
    "Falau",
    "Hethaeg",
    "Melulind",
    "Morchaur",
    "Nilrin",
    "Shabakauth",
    "Ulkal",
    "Valdo",
    "Wendun",
    "Atund",
    "Bagurar",
    "Shuldu",
    "Be'elzebubba",
    "K'banian",
    "Faellat",
    "G'ldethi",
    "Kazgoroth",
    "Mathanat",
    "Ngot",
    "S'goth",
    "Sot-ilh",
    "Ubote",
    "Yar'udab",
    "Ulyaoth",
    "Xel'lotath",
    "Chattur'gha",
    "Mantorok",
    "Gowron",
    "Dak'shira",
    "Keghas",
    "Pha'otho",
    "N'hogura",
    "Alevot",
    "Arafonad",
    "Axurador",
    "Erelak",
    "Eroam",
    "Kazidax",
    "Nudob",
    "Tarvo",
    "Araxidak",
    "Duri",
    "Kafondam",
    "Rilor",
    "Ukam",
    "Kofarutul",
    "Edorir",
    "Sofolos",
    "Tarivror",
    "Xibavel",
    "M'Rub", // [marrub] god damn it yholl
};

str BossSuffixes[NAME_MAX_BOSS_SUFFIXES] =
{
    "Supreme",
    "Lord of Brimstone",
    "Eater of Souls",
    "Betrayer of All",
    "Executioner",
    "Seeker of Bloodshed",
    "Agent of Damnation",
    "Image of Darkness",
    "Bringer of Famine",
    "Engine of Ruin",
    "Ender of Days",
    "Lord of Chaos",
    "Lord of Darkness",
    "Lord of Betrayal",
    "Slayer of Worlds",
    "Shadow of the Night",
    "Dark One",
    "Chosen of Sin",
    "Cold-eyed Slayer",
    "Juggernaut",
    "Walking Apocalypse",
    "Invincible",
    "Final Judgement",
    "One Who Seeks Battle",
    "Overlord of Pandemonium",
    "Lord of Hatred",
    "Incarnation of Death",
};

str ZombieRanks[NAME_MAX_ZOMBIE_RANK] =
{
    "MSgt.",
    "1stSgt.",
    "MGySgt.",
    "SgtMaj.",
    "2ndLt.",
    "1stLt.",
    "Capt.",
    "Maj.",
    "LtCol.",
};

str MarineRanks[NAME_MAX_MARINE_RANK] =
{
    "Pvt.",
    "Pvt.",
    "Pvt.",
    "Pvt.",
    "Pvt.",
    "Pvt.",
    "Pvt.",
    "Pvt.",
    "PFC.",
    "PFC.",
    "PFC.",
    "LCpL.",
    "LCpL.",
    "LCpL.",
    "Cpl.",
    "Cpl.",
    "Cpl.",
    "Sgt.",
    "SSgt.",
    "GySgt.",
    "MSgt.",
    "1stSgt.",
    "MGySgt.",
    "SgtMaj.",
};

str MaleNames[NAME_MAX_MALE] =
{
    "Antoine",
    "Austin",
    "Brendan",
    "Clint",
    "Christopher",
    "Daron",
    "Darrell",
    "Dean",
    "Dudley",
    "Earle",
    "Ernesto",
    "Eugene",
    "Edward",
    "Fletcher",
    "Darell",
    "Gerald",
    "Dominic",
    "Isaac",
    "Jake",
    "Jefferson",
    "Kelly",
    "Leonard",
    "Leonardo",
    "Lincoln",
    "Lou",
    "Martin",
    "Marco",
    "Markus",
    "Matt",
    "Melvin",
    "Miguel",
    "Nathanial",
    "Derek",
    "Nelson",
    "Ollie",
    "Giovanni",
    "Perry",
    "Porter",
    "Ralph",
    "Rashad",
    "Zachariah",
    "Raymond",
    "Kirby",
    "Robby",
    "Royce",
    "Rudy",
    "Scot",
    "Timmy",
    "Chadwick",
    "Tom",
    "Lucien",
    "Trey",
    "Tristan",
    "Wilber",
    "Willis",
    "Xavier",
};

str FemaleNames[NAME_MAX_FEMALE] =
{
    "Abigail",
    "Cathryn",
    "Ada",
    "Alta",
    "Amber",
    "Anita",
    "Annmarie",
    "Bernice",
    "Beryl",
    "Beverley",
    "Candy",
    "Cassie",
    "Clare",
    "Holly",
    "Claudette",
    "Desiree",
    "Dolly",
    "Michele",
    "Dora",
    "Roberta",
    "Earnestine",
    "Elba",
    "Liza",
    "Esther",
    "Felecia",
    "Tammy",
    "Geraldine",
    "Greta",
    "Gwen",
    "Bethany",
    "Harriet",
    "Hope",
    "Imogene",
    "Ivy",
    "Janna",
    "Jennie",
    "Jodie",
    "Kristin",
    "Laura",
    "Lizzie",
    "Maritza",
    "Maureen",
    "Melva",
    "Mona",
    "Natalia",
    "Fay",
    "Rosalind",
    "Selena",
    "Serena",
    "Sheri",
    "Sherrie",
    "Tammi",
    "Tisha",
    "Tracie",
    "Velma",
    "Virgie",
    "Wilda",
};

str LastNames[NAME_MAX_LAST] =
{
    "Gibbs",
    "Allen",
    "Dalton",
    "Lane",
    "Pena",
    "Castro",
    "Riddle",
    "Hester",
    "Dillon",
    "Davidson",
    "Clark",
    "Flynn",
    "Gamble",
    "Esperanza",
    "Haney",
    "McCormick",
    "Leach",
    "Leonard",
    "Knox",
    "Howard",
    "Hopper",
    "Hobbs",
    "Vance",
    "Hammond",
    "Knox",
    "Wallace",
    "Good",
    "Rivers",
    "Summers",
    "Lynn",
    "Hebert",
    "Dawson",
    "Workman",
    "Sweeney",
    "Buckner",
    "Noble",
    "Page",
    "Murphy",
    "Duran",
    "Alvarez",
    "Evans",
    "Holland",
    "Olsen",
    "David",
    "Fernandez",
    "Fitzgerald",
    "Anthony",
    "Solis",
    "Carrillo",
    "Calderon",
    "Kidd",
    "MacDonald",
    "Boone",
    "Cross",
    "Cabrera",
    "Sweeney",
    "Walker",
    "McDonald",
    "Valdez",
    "Cross",
    "Keith",
    "Waller",
    "Hooper",
    "Johnson",
    "Garrison",
    "Barber",
    "Dorsey",
    "Porter",
    "Burch",
    "Landry",
    "Browning",
    "Clements",
    "Conley",
    "Bates",
    "Underwood",
    "Schmidt",
    "Ferguson",
    "Albert",
    "Swanson",
    "McKay",
    "McKenzie",
    "Crawford",
    "Duran",
    "Guzman",
    "Prince",
    "Lott",
    "Burnett",
    "Bishop",
    "Hogan",
    "Edwards",
    "Crosby",
    "Richardson",
    "Johnston",
    "Parrish",
    "Levine",
    "Patel",
    "Cole",
};

// TODO: Should be MAX_MEGABOSSES when they are finished
str MegabossSuffixes[7] =
{
    "The Prideful",
    "The Wrathladen",
    "The Lustful",
    "The Greedy",
    "The Gluttonous",
    "The Envious",
    "The Slothful",
};

str PowersuitNames[NAME_MAX_SUIT] =
{
    "Col. Roysten",
    "Maj. Solo",
    "Gen. Belanger",
    "Sgt. Shivers",
    "Gen. Rainbow Bacon",
};

str NameColors[NAME_MAX_COLORS] =
{
    "\Ca", "\Cb", "\Cc", "\Cd", "\Ce", "\Cf", "\Cg",
    "\Ch", "\Ci", "\Cj", "\Ck", "\Cm", "\Cn", "\Cp",
    "\Cq", "\Cr", "\Cs", "\Ct", "\Cu", "\Cv",
};

str GenerateName(str Actor, str Tag)
{
	if (ColourfulHell)
		return GenerateColourfulHellName(Actor, Tag);

    // Humans
    if (CompatMode == COMPAT_DRLA)
    {
        if (Contains(Tag, "Human"))
        {
            bool Female = false;
            
            // Check female actors
            if (!StrICmp(Actor, "RLFormerHumanBattleRifle3RPG") || !StrICmp(Actor, "RLEliteHumanBattleRifle2RPG"))
                Female = true;
            
            return GenerateZombieName(Female);
        }
    }
    else // Vanilla/Extras
    {
        if (Contains(Actor, "ZombieMan", true) || 
            Contains(Actor, "ShotgunGuy", true) ||
            Contains(Actor, "ChaingunGuy", true))
            return GenerateZombieName(false);
    }
    
    // Marines
    if (Contains(Tag, "Marine"))
    {
        bool Female = false;
        
        // Check female actors
        if (!StrICmp(Actor, "DRPGMarineFemGuardBattleRifle") || !StrICmp(Actor, "DRPGMarineFemBattleRifle"))
            Female = true;
        
        return GenerateMarineName(Female);
    }
    
    // Megabosses
    if (Contains(Actor, "Megaboss"))
    {
        if (!StrICmp(Tag, "Pride"))
            return GenerateMegabossName(0);
        if (!StrICmp(Tag, "Sloth"))
            return GenerateMegabossName(6);
    }
    
    // Heavy and Super Powersuits
    if (Contains(Actor, "PowerSuit"))
        return GeneratePowersuitName();
    
    // Monster
    return GenerateMonsterName();
}

str GenerateColourfulHellName(str Actor, str Tag)
{
	//common
	if (Contains(Actor, "Common"))
		return StrParam("\Cc%S", Tag);
	
	//green
	if (Contains(Actor, "Green"))
		return StrParam("\Cq%S", Tag);
	
	//blue
	if (Contains(Actor, "Blue"))
		return StrParam("\Ch%S", Tag);
	
	//purple
	if (Contains(Actor, "Purple"))
		return StrParam("\Ct%S", Tag);
	
	//yellow
	if (Contains(Actor, "Yellow"))
		return StrParam("\Ck%S", Tag);
	
	//red
	if (Contains(Actor, "Red"))
		return StrParam("\Cg%S", Tag);
	
	//black
	if (Contains(Actor, "Black"))
		return StrParam("\Cm%S", Tag);
	
	//white
	if (Contains(Actor, "White"))
		return StrParam("\Cj%S", Tag);
	
	//grey
	if (Contains(Actor, "Gray") || Contains(Actor, "Grey"))
		return StrParam("\Cu%S", Tag);
	
	//fireblu
	if (Contains(Actor, "Fireblu", true))
		return StrParam("\Ca%S", Tag);
	
	str Color = NameColors[Random(0, NAME_MAX_COLORS - 1)];
	return StrParam("%S%S", Color, Tag);
}

str GenerateMonsterName()
{
    int PrefixPick;
    int Prefix2Pick;
    int NamePick;
    int SuffixPick;
    str Prefix;
    str Prefix2;
    str Name;
    str Suffix;
    str FinalName;
    
    // Pick Prefix
    PrefixPick = Random(0, NAME_MAX_SHARED - 1);
    while (Prefix2Pick == PrefixPick)
        Prefix2Pick = Random(0, NAME_MAX_SHARED - 1);
    
    // Pick Name
    NamePick = Random(0, NAME_MAX_DEMON_NAMES - 1);
    
    // Pick Suffix
    if (CheckFlag(0, "BOSS"))
        SuffixPick = Random(0, NAME_MAX_BOSS_SUFFIXES - 1);
    else
        SuffixPick = Random(0, NAME_MAX_SHARED - 1);
    
    // Apply Prefix
    Prefix = SharedPool[PrefixPick];
    Prefix2 = SharedPool[Prefix2Pick];
    
    // Apply Name
    Name = DemonNames[NamePick];
    
    // Apply Suffix
    if (CheckFlag(0, "BOSS"))
        Suffix = BossSuffixes[SuffixPick];
    else
        Suffix = SharedPool[SuffixPick];
    
    // Pick Color
    str Color = NameColors[Random(0, NAME_MAX_COLORS - 1)];
    
    // Determine the format of the name
    int NameType = 1;
    if (CheckFlag(0, "BOSS"))
    {
        if (Random(1, 2) == 1)
            NameType = 1;
        else
            NameType = 3;
    }
    else
        NameType = Random(1, 5);
    
    switch (NameType)
    {
    case 1: FinalName = StrParam("%S%S %S, the %S", Color, Prefix, Name, Suffix); break; // (Prefix) (Name) the (Suffix)
    case 2: FinalName = StrParam("%S%S %S", Color, Prefix, Name);                 break; // (Prefix) (Name)
    case 3: FinalName = StrParam("%S%S the %S", Color, Name, Suffix);             break; // (Name) the (Suffix)
    case 4: FinalName = StrParam("%S%S", Color, Name);                            break; // (Name)
    case 5: FinalName = StrParam("%S%S, %S %S", Color, Prefix, Prefix2, Name);    break; // (Prefix) (Prefix) (Name)
    }
    
    return FinalName;
}

str GenerateZombieName(bool Female)
{
    int RankPick = Random(0, NAME_MAX_ZOMBIE_RANK - 1);
    int FirstNamePick = Random(0, (Female ? NAME_MAX_FEMALE - 1 : NAME_MAX_MALE - 1));
    int LastNamePick = Random(0, NAME_MAX_LAST - 1);
    str Rank = ZombieRanks[RankPick];
    str FirstName = (Female ? FemaleNames[FirstNamePick] : MaleNames[FirstNamePick]);
    str LastName = LastNames[LastNamePick];
    str Color = NameColors[Random(0, NAME_MAX_COLORS - 1)];
    
    return StrParam("%S%S %S %S", Color, Rank, FirstName, LastName);
}

str GenerateMarineName(bool Female)
{
    int RankPick = Random(0, NAME_MAX_MARINE_RANK - 1);
    int FirstNamePick = Random(0, (Female ? NAME_MAX_FEMALE - 1 : NAME_MAX_MALE - 1));
    int LastNamePick = Random(0, NAME_MAX_LAST - 1);
    str Rank = MarineRanks[RankPick];
    str FirstName = (Female ? FemaleNames[FirstNamePick] : MaleNames[FirstNamePick]);
    str LastName = LastNames[LastNamePick];
    
    return StrParam("%S %S %S", Rank, FirstName, LastName);
}

str GenerateMegabossName(int SuffixIndex)
{
    int Pick = Random(0, NAME_MAX_DEMON_NAMES - 1);
    
    return StrParam("%S %S", DemonNames[Pick], MegabossSuffixes[SuffixIndex]);
}

str GeneratePowersuitName()
{
    int Pick = Random(0, NAME_MAX_SUIT - 1);
    
    return StrParam("%S", PowersuitNames[Pick]);
}
