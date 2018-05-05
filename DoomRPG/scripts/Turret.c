#include "Defs.h"

#include <stdlib.h>

#include "Crate.h"
#include "ItemData.h"
#include "Menu.h"
#include "RPG.h"
#include "Stims.h"
#include "Turret.h"
#include "Utils.h"

TurretUpgrade RPGMap TurretUpgradeData[MAX_UPGRADES] =
{
    // Build
    {
        "Build Turret", 1, 5,
        "Build the portable turret",
        "",
        "Issuing this command will enable or disable the turret"
    },
    
    // --------------------------------------------------
    // Weapons
    // 
    
    // Bullet
    {
        "Weapon Module - Bullet", 1, 2,
        "The turret is able to fire rapid-fire low-damage bullets",
        "",
        "Issuing this command will load the bullet weapon module"
    },
    {
        "Weapon Module - Bullet - Damage", 10, 3,
        "Increases the damage of the turret's bullets",
        "",
        ""
    },
    {
        "Weapon Module - Bullet - Capacity", 10, 3,
        "Increases the storage capacity of bullets the turret can hold",
        "",
        "issuing this command will insert bullets"
    },
    {
        "Weapon Module - Bullet - Rate of Fire", 5, 5,
        "Increases the fire rate of the turret's bullets",
        "",
        ""
    },
    
    // Pellet
    {
        "Weapon Module - Pellet", 1, 3,
        "The turret is able to fire shotgun-like pellet spread shots",
        "",
        "Issuing this command will load the pellet weapon module"
    },
    {
        "Weapon Module - Pellet - Damage", 10, 3,
        "Increases the damage of the turret's pellets",
        "",
        ""
    },
    {
        "Weapon Module - Pellet - Capacity", 10, 3,
        "Increases the storage capacity of shells the turret can hold",
        "",
        "issuing this command will insert shells"
    },
    {
        "Weapon Module - Pellet - Rate of Fire", 5, 5,
        "Increases the fire rate of the turret's pellets",
        "",
        ""
    },
    {
        "Weapon Module - Pellet - Spread", 5, 5,
        "Decreases the spread of the pellets fired",
        "",
        ""
    },
    {
        "Weapon Module - Pellet - Amount", 10, 3,
        "Increases the number of pellets fired per shot",
        "",
        ""
    },
    
    // Rocket
    {
        "Weapon Module - Rocket", 1, 5,
        "The turret is able to fire explosive rockets",
        "",
        "Issuing this command will load the rocket weapon module"
    },
    {
        "Weapon Module - Rocket - Damage", 10, 3,
        "Increases the damage of the turret's rockets",
        "",
        ""
    },
    {
        "Weapon Module - Rocket - Capacity", 10, 3,
        "Increases the storage capacity of rockets the turret can hold",
        "",
        "issuing this command will insert rockets"
    },
    {
        "Weapon Module - Rocket - Rate of Fire", 5, 5,
        "Increases the fire rate of the turret's rockets",
        "",
        ""
    },
    {
        "Weapon Module - Rocket - Seeking", 5, 10,
        "Allows fired rockets to seek targets",
        "",
        ""
    },
    
    // Plasma
    {
        "Weapon Module - Plasma", 1, 10,
        "The turret is able to fire superheated balls of plasma",
        "",
        "Issuing this command will load the plasma weapon module"
    },
    {
        "Weapon Module - Plasma - Damage", 10, 3,
        "Increases the damage of the turret's plasma shots",
        "",
        ""
    },
    {
        "Weapon Module - Plasma - Capacity", 10, 3,
        "Increases the storage capacity of cells used for plasma the turret can hold",
        "",
        "issuing this command will insert cells"
    },
    {
        "Weapon Module - Plasma - Rate of Fire", 5, 5,
        "Increases the fire rate of the turret's plasma shots",
        "",
        ""
    },
    
    // Railgun
    {
        "Weapon Module - Railgun", 1, 20,
        "The turret is able to fire piercing railgun shots",
        "",
        "Issuing this command will load the railgun weapon module"
    },
    {
        "Weapon Module - Railgun - Damage", 10, 3,
        "Increases the damage of the turret's railgun shots",
        "",
        ""
    },
    {
        "Weapon Module - Railgun - Capacity", 10, 3,
        "Increases the storage capacity of cells used for the railgun the turret can hold",
        "",
        "issuing this command will insert cells"
    },
    {
        "Weapon Module - Railgun - Rate of Fire", 5, 5,
        "Increases the fire rate of the turret's railgun shots",
        "",
        ""
    },
    {
        "Weapon Module - Railgun - Piercing", 1, 10,
        "Allows the railgun to pierce through targets",
        "",
        ""
    },
    
    // Ammo
    {
        "Ammo Module - Autoloader", 1, 5,
        "Autoloads ammo if it runs out from your own ammo pool",
        "",
        "issuing this command will toggle autoloading on and off"
    },
    {
        "Ammo Module - Nano-Generators", 10, 5,
        "Allows the turret to slowly generate it's own ammo supplies",
        "Upgrades increase generation speed",
        ""
    },
    {
        "Ammo Module - Nano-Generators - Bullets", 1, 10,
        "Allows the turret to slowly generate bullets",
        "",
        ""
    },
    {
        "Ammo Module - Nano-Generators - Shells", 1, 20,
        "Allows the turret to slowly generate shells",
        "",
        ""
    },
    {
        "Ammo Module - Nano-Generators - Rockets", 1, 50,
        "Allows the turret to slowly generate rockets",
        "",
        ""
    },
    {
        "Ammo Module - Nano-Generators - Cells", 1, 50,
        "Allows the turret to slowly generate cells",
        "",
        ""
    },
    
    // Special/Unique Weapons
    {
        "Special Weapon Module - Self-Destruct", 10, 5,
        "Destroys the turret, but creates a devastating explosion",
        "Upgrades increase explosion radius and damage",
        "Issuing this command will trigger the self-destruct sequence"
    },
    
    // --------------------------------------------------
    // Armor
    // 
    
    // Plating
    {
        "Armor Plating - Reinforcement", 10, 5,
        "Reinforce the plating of the turret, allowing it to take more damage",
        "Upgrades Increase the maximum health of the turret",
        ""
    },
    {
        "Armor Plating - Melee", 10, 5,
        "Plating which protects against melee damage",
        "Upgrades increase protection amount",
        ""
    },
    {
        "Armor Plating - Bullet", 10, 5,
        "Plating which protects against bullet damage",
        "Upgrades increase protection amount",
        ""
    },
    {
        "Armor Plating - Fire", 10, 5,
        "Plating which protects against fire damage",
        "Upgrades increase protection amount",
        ""
    },
    {
        "Armor Plating - Plasma", 10, 5,
        "Plating which protects against plasma damage",
        "Upgrades increase protection amount",
        ""
    },
    {
        "Armor Plating - Blast", 1, 20,
        "Plating which protects against blast damage",
        "",
        ""
    },
    
    // Modules
    {
        "Armor Module - Projectile Reflection", 3, 10,
        "Reflects projectiles fired at the turret",
        "Upgrades improve the abilities of the reflection",
        ""
    },
    {
        "Armor Module - Repair Nanites", 10, 5,
        "Slowly Repairs the health of the turret",
        "Upgrades increase repair speed",
        ""
    },
    {
        "Armor Module - Phase Inverter", 10, 5,
        "Randomly shifts the turret in and out of visibility",
        "Upgrades increase the time that the turret can stay invisible",
        ""
    },
    
    // --------------------------------------------------
    // Assist
    // 
    
    {
        "Assist Module - Health Injector", 10, 3,
        "Injects you with some health if you are near death",
        "Upgrades increase injection cutoff percentage",
        ""
    },
    {
        "Assist Module - Armor Repair Nanites", 10, 5,
        "Slowly repairs your armor",
        "Upgrades increase repair speed",
        ""
    },
    {
        "Assist Module - Augmentation Charge Transfer Bus", 10, 5,
        "Slowly recharges your augmentation battery",
        "Upgrades increase charge speed",
        ""
    },
    {
        "Assist Module - Shield Charge Transfer Bus", 10, 5,
        "Slowly recharges your shield",
        "Upgrades increase charge speed",
        ""
    },
    {
        "Assist Module - Stim Injector", 10, 10,
        "Turret can inject you with a random cocktail of combat-enhancing drugs",
        "Upgrades increase compound generation speed and decrease wait time between injections",
        "" // Generated in BuildTurretData()
    },
    {
        "Assist Module - Stim Injector - Type", 3, 50,
        "Determines the type of drugs you will be injected with",
        "Upgrade allows boosters and powerups to be injected",
        "Issuing this command will swap between broad injection and Focused injection",
    },
    {
        "Assist Module - Stim Injector - Amount", 10, 10,
        "Determines the amount of drugs you will be injected with",
        "",
        ""
    },
    {
        "Assist Module - Stim Injector - Potency", 10, 10,
        "Determines the potency of the drugs you will be injected with",
        "",
        ""
    },
    {
        "Assist Module - Emergency Teleportation System", 1, 10,
        "The turret will automatically teleport you out of danger when under 10% health",
        "",
        "Issuing this command will toggle Emergency Teleportation on/off"
    },
    {
        "Assist Module - Team Unit", 10, 3,
        "Allows other assist modules to be used on teammates around you",
        "Upgrades increase assistance range",
        ""
    },
    
    // --------------------------------------------------
    // Sensors
    // 
    
    {
        "Sensors - Calibration Speed", 10, 5,
        "Increases the speed and reliability of the turret's sensors",
        "Upgrades increase the speed of all sensor-based activities",
        ""
    },
    
    // Module Finder
    {
        "Sensors Module - Module Finder", 1, 20,
        "Calibrates the sensors to allow the turret to find upgrade modules",
        "",
        "Issuing this command will start the item finder, searching for upgrade modules"
    },
    
    // Item Finder
    {
        "Sensors Module - Item Finder", 1, 20,
        "Calibrates the sensors to allow the turret to find different types of items",
        "",
        "Issuing this command will have the turret scout the map and allow you to locate items"
    },
    
    // Crate Finder
    {
        "Sensors Module - Supply Drop Crate Finder", 1, 20,
        "Calibrates the sensors to allow the turret to find UAC Supply Crates",
        "",
        "Issuing this command will have the turret scout the map and allow you to locate supply crates"
    },
    
    // --------------------------------------------------
    // Battery
    // 
    
    {
        "Battery - Capacity", 10, 5,
        "Increases the capacity of the turret's battery",
        "Upgrades increase maximum battery capacity",
        ""
    },
    
    // Generators
    {
        "Generator - Kinetic", 10, 5,
        "Charges the turret's battery based on movement",
        "Upgrades decrease interval between charges",
        ""
    },
    {
        "Generator - Illumination", 10, 5,
        "Charges the turret's battery based on light level",
        "Upgrades decrease interval between charges",
        ""
    },
    {
        "Generator - Force", 10, 5,
        "Charges the turret's battery from bullet and melee based damage",
        "Upgrades increase charge amount when recieving this kind of damage",
        ""
    },
    {
        "Generator - Thermal", 10, 5,
        "Charges the turret's battery from fire based damage",
        "Upgrades increase charge amount when recieving this kind of damage",
        ""
    },
    {
        "Generator - Plasma", 10, 5,
        "Charges the turret's battery from plasma based damage",
        "Upgrades increase charge amount when recieving this kind of damage",
        ""
    },
    {
        "Generator - Nuclear", 10, 5,
        "Charges the turret's battery from toxicity and radiation based damage",
        "Upgrades increase charge amount when recieving this kind of damage",
        ""
    },
    
    // --------------------------------------------------
    // Hardware
    // 
    
    {
        "Hardware - Battery Charge Bus", 10, 5,
        "Decreases the time it takes for the turret's battery to be recharged when sent back to the Outpost",
        "Upgrades decrease charging time when in maintenance",
        ""
    },
    {
        "Hardware - Build Quality", 10, 5,
        "Decreases the parts it takes to repair the turret",
        "Upgrades decrease the turret parts necessary for repair when in maintenance",
        ""
    },
    {
        "Hardware - Parts Optimization", 10, 5,
        "Decreases the time it takes to repair the turret",
        "Upgrades decrease repair time when in maintenance",
        ""
    },
    {
        "Hardware - Specification Optimization", 10, 5,
        "Decreases the time it takes to upgrade and refit the turret",
        "Upgrades decrease refit time when in maintenance",
        ""
    },
    {
        "Hardware - Expert Fabrication", 10, 5,
        "Decreases the cost of maintenance",
        "Upgrades decrease cash flow required when in maintenance",
        ""
    },
    
    // --------------------------------------------------
    // Commands
    // 
    
    {
        "Commands - Recall", 1, 10,
        "A module that recalls the turret to you, cancelling other commands",
        "",
        "Issuing this command will call the turret to you and cancel commands"
    },
    {
        "Commands - Draw Fire", 1, 10,
        "The turret will draw enemies' attention to itself",
        "",
        "" // Generated in BuildTurretData()
    },
    {
        "Commands - Hold Position", 1, 10,
        "The turret will stay and defend its' current position",
        "",
        "Issuing this command will set the turret to hold its' position"
    },
};

NamedScript Type_ENTER void TurretLoop()
{
    int PlayerNum = PlayerNumber();
    int Health;
    int PrevHealth;
    bool TeleportTriggered = false;
    
    Start:
    
    // Horrible hack
    // Just entered the level, despawn the turret
    if (Timer() <= 4 && Player.Turret.Init && Player.Turret.Active)
        TurretDespawn();
    
    // Turret first time init
    if (Player.Turret.Upgrade[TU_BUILD] && !Player.Turret.Init)
    {
        // Initial levels
        if (!Player.Turret.Upgrade[TU_WEAPON_BULLET_CAPACITY])
            Player.Turret.Upgrade[TU_WEAPON_BULLET_CAPACITY] = 1;
        if (!Player.Turret.Upgrade[TU_WEAPON_PELLET_CAPACITY])
            Player.Turret.Upgrade[TU_WEAPON_PELLET_CAPACITY] = 1;
        if (!Player.Turret.Upgrade[TU_WEAPON_ROCKET_CAPACITY])
            Player.Turret.Upgrade[TU_WEAPON_ROCKET_CAPACITY] = 1;
        if (!Player.Turret.Upgrade[TU_WEAPON_PLASMA_CAPACITY])
            Player.Turret.Upgrade[TU_WEAPON_PLASMA_CAPACITY] = 1;
        if (!Player.Turret.Upgrade[TU_WEAPON_RAILGUN_CAPACITY])
            Player.Turret.Upgrade[TU_WEAPON_RAILGUN_CAPACITY] = 1;
        if (!Player.Turret.Upgrade[TU_ARMOR_PLATING])
            Player.Turret.Upgrade[TU_ARMOR_PLATING] = 1;
        if (!Player.Turret.Upgrade[TU_BATTERY_CAPACITY])
            Player.Turret.Upgrade[TU_BATTERY_CAPACITY] = 1;
        if (!Player.Turret.Upgrade[TU_ASSIST_INJECTOR_TYPE])
            Player.Turret.Upgrade[TU_ASSIST_INJECTOR_TYPE] = 1;
        if (!Player.Turret.Upgrade[TU_ASSIST_INJECTOR_AMOUNT])
            Player.Turret.Upgrade[TU_ASSIST_INJECTOR_AMOUNT] = 1;
        if (!Player.Turret.Upgrade[TU_ASSIST_INJECTOR_POTENCY])
            Player.Turret.Upgrade[TU_ASSIST_INJECTOR_POTENCY] = 1;
        
        // Reset the current weapon
        Player.Turret.Weapon = TW_NONE;
        
        // Small delay to let the initial upgrades kick in and for total to be calculated
        Delay(1);
        
        // Initial Health
        if (Player.Turret.TID != 0)
            SetActorProperty(Player.Turret.TID, APROP_Health, Player.Turret.HealthMax);
        
        // Initial Battery Charge
        Player.Turret.Battery = Player.Turret.BatteryMax;
        
        // Set Flags
        Player.Turret.Init = true;
        Player.Turret.PaidForRepair = true;
    }
    
    // Sanity check. Without this, using `kill monsters` (or being really unlucky) while the turret is active will semi-destroy it, making it impossible to deploy or repair.
    if (Player.Turret.Health >= Player.Turret.HealthMax)
    {
        Player.Turret.Destroyed = false;
        Player.Turret.PaidForRepair = true;
    }
    
    while (Player.Turret.Active && Player.Turret.TID != 0)
    {
        // Pre-health check
        Health = GetActorProperty(Player.Turret.TID, APROP_Health);
        
        // Prevent Health overflow
        if (Health > Player.Turret.HealthMax)
        {
            Health = Player.Turret.HealthMax;
            SetActorProperty(Player.Turret.TID, APROP_Health, Health);
        }
        
        // Despawn the turret if itself or the owning player dies
        if (GetActorProperty(Player.TID, APROP_Health) <= 0 || GetActorProperty(Player.Turret.TID, APROP_Health) <= 0)
        {
            Player.Turret.Destroyed = true;
            Player.Turret.PaidForRepair = false;
            TurretDespawn();
            break;
        }
        
        // Nametag
        SetActorPropertyString(Player.Turret.TID, APROP_NameTag, StrParam("%tS\C-'s Turret", PlayerNumber() + 1));
        
        // Battery is drained
        if (Player.Turret.Battery <= 0)
        {
            TurretDespawn();
            break;
        }
        
        // Autoloader
        if (Player.Turret.Upgrade[TU_AMMO_AUTOLOADER] && Player.Turret.Autoload)
        {
            if (Player.Turret.Upgrade[TU_WEAPON_BULLET] && Player.Turret.BulletAmmo <= 0)
                TurretLoadAmmo(TU_WEAPON_BULLET);
            if (Player.Turret.Upgrade[TU_WEAPON_PELLET] && Player.Turret.ShellAmmo <= 0)
                TurretLoadAmmo(TU_WEAPON_PELLET);
            if (Player.Turret.Upgrade[TU_WEAPON_ROCKET] && Player.Turret.RocketAmmo <= 0)
                TurretLoadAmmo(TU_WEAPON_ROCKET);
            if (Player.Turret.Upgrade[TU_WEAPON_PLASMA] && Player.Turret.PlasmaAmmo <= 0)
                TurretLoadAmmo(TU_WEAPON_PLASMA);
            if (Player.Turret.Upgrade[TU_WEAPON_RAILGUN] && Player.Turret.RailAmmo <= 0)
                TurretLoadAmmo(TU_WEAPON_RAILGUN);
        }
        
        // Nano Ammo Generators
        if (Player.Turret.Upgrade[TU_AMMO_NANOGEN] > 0 && (Timer() % (35 * (15 - Player.Turret.Upgrade[TU_AMMO_NANOGEN]))) == 0)
        {
            if (Player.Turret.Upgrade[TU_AMMO_NANOGEN_BULLET] && Player.Turret.BulletAmmo < Player.Turret.BulletAmmoMax)
                Player.Turret.BulletAmmo += 10;
            if (Player.Turret.Upgrade[TU_AMMO_NANOGEN_SHELL] && Player.Turret.ShellAmmo < Player.Turret.ShellAmmoMax)
                Player.Turret.ShellAmmo += 4;
            if (Player.Turret.Upgrade[TU_AMMO_NANOGEN_ROCKET] && Player.Turret.RocketAmmo < Player.Turret.RocketAmmoMax)
                Player.Turret.RocketAmmo++;
            if (Player.Turret.Upgrade[TU_AMMO_NANOGEN_CELL] && Player.Turret.PlasmaAmmo < Player.Turret.PlasmaAmmoMax)
                Player.Turret.PlasmaAmmo += 20;
            if (Player.Turret.Upgrade[TU_AMMO_NANOGEN_CELL] && Player.Turret.RailAmmo < Player.Turret.RailAmmoMax)
                Player.Turret.RailAmmo++;
        }
        
        // Armor/Protection
        if (Player.Turret.Upgrade[TU_ARMOR_PLATING_MELEE] > 0)
            GiveActorInventory(Player.Turret.TID, StrParam("DRPGTurretProtectionMelee%d", Player.Turret.Upgrade[TU_ARMOR_PLATING_MELEE]), 1);
        if (Player.Turret.Upgrade[TU_ARMOR_PLATING_BULLET] > 0)
            GiveActorInventory(Player.Turret.TID, StrParam("DRPGTurretProtectionBullet%d", Player.Turret.Upgrade[TU_ARMOR_PLATING_BULLET]), 1);
        if (Player.Turret.Upgrade[TU_ARMOR_PLATING_FIRE] > 0)
            GiveActorInventory(Player.Turret.TID, StrParam("DRPGTurretProtectionFire%d", Player.Turret.Upgrade[TU_ARMOR_PLATING_FIRE]), 1);
        if (Player.Turret.Upgrade[TU_ARMOR_PLATING_PLASMA] > 0)
            GiveActorInventory(Player.Turret.TID, StrParam("DRPGTurretProtectionPlasma%d", Player.Turret.Upgrade[TU_ARMOR_PLATING_PLASMA]), 1);
        if (Player.Turret.Upgrade[TU_ARMOR_PLATING_BLAST] > 0)
            GiveActorInventory(Player.Turret.TID, "DRPGTurretProtectionBlast", 1);
        if (Player.Turret.Upgrade[TU_ARMOR_MODULE_REFLECT] > 0)
            GiveActorInventory(Player.Turret.TID, StrParam("DRPGTurretReflection%d", Player.Turret.Upgrade[TU_ARMOR_MODULE_REFLECT]), 1);
        if (Player.Turret.Upgrade[TU_ARMOR_MODULE_REPAIR] > 0)
            if (Health < Player.Turret.HealthMax && (Timer() % (35 * (30 - (Player.Turret.Upgrade[TU_ARMOR_MODULE_REPAIR] * 2.5)))) == 0)
                SetActorProperty(Player.Turret.TID, APROP_Health, Health + 1);
        if (Player.Turret.Upgrade[TU_ARMOR_MODULE_PHASE] > 0 && Health < PrevHealth)
            GiveActorInventory(Player.Turret.TID, StrParam("DRPGTurretPhase%d", Player.Turret.Upgrade[TU_ARMOR_MODULE_PHASE]), 1);
        if (GetActorPowerupTics(Player.Turret.TID, "PowerGhost") > 0)
        {
            SetActorProperty(Player.Turret.TID, APROP_RenderStyle, STYLE_Translucent);
            SetActorPropertyFixed(Player.Turret.TID, APROP_Alpha, 0.5 + (Sin(Timer() / 32.0) * 0.5));
        }
        else
        {
            SetActorProperty(Player.Turret.TID, APROP_RenderStyle, STYLE_Normal);
            SetActorPropertyFixed(Player.Turret.TID, APROP_Alpha, 1.0);
        }
        
        // Assist Modules
        if (Player.Turret.HitTimer <= 0)
            for (int i = 0; i < MAX_PLAYERS; i++)
            {
                // If this player isn't in game, continue
                if (!PlayerInGame(i)) continue;
                
                // If you don't have the Team Unit upgrade yet, skip all other players
                if (!Player.Turret.Upgrade[TU_ASSIST_TEAM] && i != PlayerNumber()) continue;
                
                // Apply to teammates around you if you have the Team Unit upgrade
                if (i != PlayerNumber() && Player.Turret.Upgrade[TU_ASSIST_TEAM] > 0 && Distance(Player.Turret.TID, Players(i).TID) >= Player.Turret.Upgrade[TU_ASSIST_TEAM] * 256) continue;
                
                // Health
                if (Player.Turret.Upgrade[TU_ASSIST_HEALTH] > 0 && CalcPercent(Players(i).ActualHealth, Players(i).HealthMax) < Player.Turret.Upgrade[TU_ASSIST_HEALTH] * 2 && ((Timer() + 8) % 35) == 0)
                {
                    for (fixed j = 0.0; j <= 1.0; j += 0.01)
                    {
                        fixed X = Lerp(GetActorX(Players(PlayerNum).Turret.TID), GetActorX(Players(i).TID), j);
                        fixed Y = Lerp(GetActorY(Players(PlayerNum).Turret.TID), GetActorY(Players(i).TID), j);
                        fixed Z = Lerp(GetActorZ(Players(PlayerNum).Turret.TID), GetActorZ(Players(i).TID) + 32.0, j);
                        
                        SpawnForced("DRPGTurretParticleAssistHealth", X, Y, Z, 0, 0);
                    }
                    
                    Players(i).ActualHealth++;
                }
                
                // Armor
                if (SetActivator(Players(i).TID)) // Yay maxhax because GetArmorInfo() has no playernum param D:
                {
                    if (Players(PlayerNum).Turret.Upgrade[TU_ASSIST_ARMOR] > 0 && CheckInventory("Armor") > 0 && CheckInventory("Armor") < GetArmorInfo(ARMORINFO_ACTUALSAVEAMOUNT) && ((Timer() + 35) % (35 * (30 - (Players(PlayerNum).Turret.Upgrade[TU_ASSIST_ARMOR] * 2.5)))) == 0)
                    {
                        for (fixed j = 0.0; j <= 1.0; j += 0.01)
                        {
                            fixed X = Lerp(GetActorX(Players(PlayerNum).Turret.TID), GetActorX(Players(i).TID), j);
                            fixed Y = Lerp(GetActorY(Players(PlayerNum).Turret.TID), GetActorY(Players(i).TID), j);
                            fixed Z = Lerp(GetActorZ(Players(PlayerNum).Turret.TID), GetActorZ(Players(i).TID) + 32.0, j);
                            
                            SpawnForced("DRPGTurretParticleAssistArmor", X, Y, Z, 0, 0);
                        }
                        
                        GiveInventory("ArmorBonus", 1);
                    }
                    
                    SetActivator(Players(PlayerNum).TID);
                }
                
                // Aug Battery
                if (Player.Turret.Upgrade[TU_ASSIST_AUG] > 0 && Players(i).Augs.Battery < Players(i).Augs.BatteryMax && ((Timer() + 26) % (35 * (30 - (Player.Turret.Upgrade[TU_ASSIST_AUG] * 2.5)))) == 0)
                {
                    for (fixed j = 0.0; j <= 1.0; j += 0.01)
                    {
                        fixed X = Lerp(GetActorX(Players(PlayerNum).Turret.TID), GetActorX(Players(i).TID), j);
                        fixed Y = Lerp(GetActorY(Players(PlayerNum).Turret.TID), GetActorY(Players(i).TID), j);
                        fixed Z = Lerp(GetActorZ(Players(PlayerNum).Turret.TID), GetActorZ(Players(i).TID) + 32.0, j);
                        
                        SpawnForced("DRPGTurretParticleAssistAug", X, Y, Z, 0, 0);
                    }
                    
                    Players(i).Augs.Battery++;
                }
                
                // Shield
                if (Player.Turret.Upgrade[TU_ASSIST_SHIELD] > 0 && Players(i).Shield.Active && Players(i).Shield.Charge < Players(i).Shield.Capacity && ((Timer() + 17) % (35 * (30 - (Player.Turret.Upgrade[TU_ASSIST_SHIELD] * 2.5)))) == 0)
                {
                    for (fixed j = 0.0; j <= 1.0; j += 0.01)
                    {
                        fixed X = Lerp(GetActorX(Players(PlayerNum).Turret.TID), GetActorX(Players(i).TID), j);
                        fixed Y = Lerp(GetActorY(Players(PlayerNum).Turret.TID), GetActorY(Players(i).TID), j);
                        fixed Z = Lerp(GetActorZ(Players(PlayerNum).Turret.TID), GetActorZ(Players(i).TID) + 32.0, j);
                        
                        SpawnForced("DRPGTurretParticleAssistShield", X, Y, Z, 0, 0);
                    }
                    
                    Players(i).Shield.Charge += Players(i).Shield.ChargeRate;
                }
            }
        
        // Emergency Teleport
        if (Player.Turret.TeleportEnabled && CalcPercent(Player.ActualHealth, Player.HealthMax) <= 10 && Player.Turret.TeleportTimer <= 0 && !TeleportTriggered)
        {
            SetInventory("ArtiTeleport", 1);
            UseInventory("ArtiTeleport");
            
            SetUserVariable(Player.Turret.TID, "user_needs_teleport", 1);
            Player.Turret.TeleportTimer = 35 * 60;
            TeleportTriggered = true;
        }
        else if (Player.Turret.TeleportTimer > 0)
            Player.Turret.TeleportTimer--;
        
        if (TeleportTriggered && CalcPercent(Player.ActualHealth, Player.HealthMax) > 10)
            TeleportTriggered = false;
        
        // Generators
        if (Player.Turret.HitTimer <= 0)
        {
            if (Player.Turret.Upgrade[TU_BATTERY_GENERATOR_KINETIC] > 0 && (AbsFixed(GetActorVelX(0)) + AbsFixed(GetActorVelY(0))) > 0 && (Timer() % (35 * (11 - Player.Turret.Upgrade[TU_BATTERY_GENERATOR_KINETIC]))) == 0)
                Player.Turret.Battery++;
            if (Player.Turret.Upgrade[TU_BATTERY_GENERATOR_ILLUMINATION] > 0 && GetActorLightLevel(Player.Turret.TID) >= 192 && (Timer() % (35 * (11 - Player.Turret.Upgrade[TU_BATTERY_GENERATOR_ILLUMINATION]))) == 0)
                Player.Turret.Battery++;
            
            // Damage-based Generators
            if (Health < PrevHealth)
            {
                if (Player.Turret.Upgrade[TU_BATTERY_GENERATOR_FORCE] > 0 && GetUserVariable(Player.Turret.TID, "user_damage_type") == DT_NORMAL)
                    Player.Turret.Battery += Player.Turret.Upgrade[TU_BATTERY_GENERATOR_FORCE];
                if (Player.Turret.Upgrade[TU_BATTERY_GENERATOR_FORCE] > 0 && GetUserVariable(Player.Turret.TID, "user_damage_type") == DT_MELEE)
                    Player.Turret.Battery += Player.Turret.Upgrade[TU_BATTERY_GENERATOR_FORCE];
                if (Player.Turret.Upgrade[TU_BATTERY_GENERATOR_THERMAL] > 0 && GetUserVariable(Player.Turret.TID, "user_damage_type") == DT_FIRE)
                    Player.Turret.Battery += Player.Turret.Upgrade[TU_BATTERY_GENERATOR_THERMAL];
                if (Player.Turret.Upgrade[TU_BATTERY_GENERATOR_PLASMA] > 0 && GetUserVariable(Player.Turret.TID, "user_damage_type") == DT_PLASMA)
                    Player.Turret.Battery += Player.Turret.Upgrade[TU_BATTERY_GENERATOR_PLASMA];
                if (Player.Turret.Upgrade[TU_BATTERY_GENERATOR_PLASMA] > 0 && GetUserVariable(Player.Turret.TID, "user_damage_type") == DT_LIGHTNING)
                    Player.Turret.Battery += Player.Turret.Upgrade[TU_BATTERY_GENERATOR_PLASMA];
                if (Player.Turret.Upgrade[TU_BATTERY_GENERATOR_NUCLEAR] > 0 && GetUserVariable(Player.Turret.TID, "user_damage_type") == DT_TOXIC)
                    Player.Turret.Battery += Player.Turret.Upgrade[TU_BATTERY_GENERATOR_NUCLEAR];
                if (Player.Turret.Upgrade[TU_BATTERY_GENERATOR_NUCLEAR] > 0 && GetUserVariable(Player.Turret.TID, "user_damage_type") == DT_RADIATION)  
                    Player.Turret.Battery += Player.Turret.Upgrade[TU_BATTERY_GENERATOR_NUCLEAR];
            }
        }
        
        // Timers
        if (Health < PrevHealth && GetUserVariable(Player.Turret.TID, "user_damage_type") != DT_TOXIC && GetUserVariable(Player.Turret.TID, "user_damage_type") != DT_RADIATION)
            Player.Turret.HitTimer = 35 * 5;
        if (Player.Turret.HitTimer > 0)
            Player.Turret.HitTimer--;
        if (Player.Turret.TeleportTimer > 0)
            Player.Turret.TeleportTimer--;
        
        // Drain Battery
        if (Player.Turret.Battery > 0 && (Timer() % 35) == 0)
            Player.Turret.Battery--;
        
        // Prevent battery overflow
        if (Player.Turret.Battery > Player.Turret.BatteryMax)
            Player.Turret.Battery = Player.Turret.BatteryMax;
        
        // Pass info to uservars
        TurretPassVars();
        
        // Draw Sensors progress bar
        if (Player.Turret.SensorsActive)
        {
            if (Player.Turret.SensorMode == TSM_MODULES)
                DrawProgressBar("\CdFinding Modules", Player.Turret.SensorPercent);
            if (Player.Turret.SensorMode == TSM_ITEMS)
                DrawProgressBar("\CjFinding Items", Player.Turret.SensorPercent);
            if (Player.Turret.SensorMode == TSM_CRATE)
                DrawProgressBar("\CcFinding Supply Drops", Player.Turret.SensorPercent);
        }
        
        // Sensor item tracing beam
        TurretItemSensorThink();
        
        // Sensor crate tracing beam
        TurretCrateSensorThink();
        
        // Reset damage type
        SetUserVariable(Player.Turret.TID, "user_damage_type", DT_NONE);
        
        Delay(1);
        
        // Payout
        if (PrevHealth < Health)
            Player.Payout.TurretDamageTaken += Abs(PrevHealth - Health);
        
        // Post-health check
        PrevHealth = Health;
        Player.Turret.Health = Health;
        
    }
    
    Delay(1);
    goto Start;
}

NamedScript Type_ENTER void TurretLoopMaintenance()
{
    int MaintCost = 0;
    
    Start:
    
    // Stats which need to be kept updated
    Player.Turret.HealthMax = 100 * Player.Turret.Upgrade[TU_ARMOR_PLATING];
    Player.Turret.BatteryMax = TURRET_BATTERY_CHARGE * Player.Turret.Upgrade[TU_BATTERY_CAPACITY];
    Player.Turret.BulletAmmoMax = 200 * Player.Turret.Upgrade[TU_WEAPON_BULLET_CAPACITY];
    Player.Turret.ShellAmmoMax = 50 * Player.Turret.Upgrade[TU_WEAPON_PELLET_CAPACITY];
    Player.Turret.RocketAmmoMax = 50 * Player.Turret.Upgrade[TU_WEAPON_ROCKET_CAPACITY];
    Player.Turret.PlasmaAmmoMax = 300 * Player.Turret.Upgrade[TU_WEAPON_PLASMA_CAPACITY];
    Player.Turret.RailAmmoMax = 10 * Player.Turret.Upgrade[TU_WEAPON_RAILGUN_CAPACITY];
    
    // Prevent ammo overflow
    if (Player.Turret.BulletAmmo > Player.Turret.BulletAmmoMax)
        Player.Turret.BulletAmmo = Player.Turret.BulletAmmoMax;
    if (Player.Turret.ShellAmmo > Player.Turret.ShellAmmoMax)
        Player.Turret.ShellAmmo = Player.Turret.ShellAmmoMax;
    if (Player.Turret.RocketAmmo > Player.Turret.RocketAmmoMax)
        Player.Turret.RocketAmmo = Player.Turret.RocketAmmoMax;
    if (Player.Turret.PlasmaAmmo > Player.Turret.PlasmaAmmoMax)
        Player.Turret.PlasmaAmmo = Player.Turret.PlasmaAmmoMax;
    if (Player.Turret.RailAmmo > Player.Turret.RailAmmoMax)
        Player.Turret.RailAmmo = Player.Turret.RailAmmoMax;
    
    // Calculate Maintenance Timers
    Player.Turret.ChargeTimer = Player.Turret.BatteryMax - Player.Turret.Battery;
    Player.Turret.RepairTimer = Player.Turret.HealthMax - Player.Turret.Health;
    
    // Reset maintenance cost this tic
    MaintCost = 0;
    
    // No Credits for payment
    if (Player.Turret.Maintenance && !CheckInventory("DRPGCredits"))
    {
        Player.Turret.Maintenance = false;
        PrintError("Maintenance has been postponed because you have run out of money");
        ActivatorSound("menu/error", 127);
    }
    
    // Maintenance
    if (Player.Turret.Maintenance)
    {
        // Charging
        if ((Timer() % (35 - (Player.Turret.Upgrade[TU_HARDWARE_BATTERY] * 3))) == 0)
            if (Player.Turret.ChargeTimer > 0)
            {
                Player.Turret.Battery++;
                
                // Payout
                Player.Payout.TurretChargeTime++;
                
                // Done
                if (Player.Turret.Battery >= Player.Turret.BatteryMax)
                    ActivatorSound("turret/chargedone", 127);
            }
        
        // Repairing
        if ((Timer() % (35 - (Player.Turret.Upgrade[TU_HARDWARE_PART] * 3))) == 0)
        {
            if (Player.Turret.RepairTimer > 0)
            {
                // Turret was destroyed, we need to pay parts up-front to fix it
                if (Player.Turret.Destroyed && !Player.Turret.PaidForRepair)
                {
                    int Parts = Player.Turret.Upgrade[TU_ARMOR_PLATING] - Player.Turret.Upgrade[TU_HARDWARE_BUILD];
                    
                    // If the parts upgrade outweighs the plating upgrade, there is no cost
                    if (Parts <= 0)
                        Player.Turret.PaidForRepair = true;
                    else if (CheckInventory("DRPGTurretPart") >= Parts)
                    {
                        Player.Turret.PaidForRepair = true;
                        TakeInventory("DRPGTurretPart", Parts);
                    }
                }
                
                if (Player.Turret.PaidForRepair)
                    Player.Turret.Health++;
                
                // Payout
                Player.Payout.TurretRepairTime++;
                
                // Done
                if (Player.Turret.Health >= Player.Turret.HealthMax)
                {
                    ActivatorSound("turret/repairdone", 127);
                    Player.Turret.Destroyed = false;
                }
            }
        }
        
        // Refitting
        if ((Timer() % (35 - (Player.Turret.Upgrade[TU_HARDWARE_SPECS] * 3))) == 0)
            if (Player.Turret.RefitTimer > 0)
            {
                Player.Turret.RefitTimer--;
                
                // Payout
                Player.Payout.TurretRefitTime++;
                
                // Done
                if (Player.Turret.RefitTimer <= 0)
                    ActivatorSound("turret/refitdone", 127);
            }
        
        // Calculate maintenance cost for this tic
        if (Player.Turret.ChargeTimer > 0)
            MaintCost++;
        if (Player.Turret.PaidForRepair && Player.Turret.RepairTimer > 0)
            MaintCost++;
        if (Player.Turret.RefitTimer > 0)
            MaintCost++;
        
        // Steady credit loss while maintenance is happening
        if ((Player.Turret.ChargeTimer > 0 || (Player.Turret.PaidForRepair && Player.Turret.RepairTimer > 0) || Player.Turret.RefitTimer > 0) && (Timer() % (5 + (Player.Turret.Upgrade[TU_HARDWARE_FABRICATION] * 3))) == 0)
            TakeInventory("DRPGCredits", MaintCost);
    }
    
    // Maintenance is done
    if (Player.Turret.Maintenance && Player.Turret.ChargeTimer <= 0 && Player.Turret.RepairTimer <= 0 && Player.Turret.RefitTimer <= 0)
    {
        Player.Turret.Maintenance = false;
        ActivatorSound("turret/maintenancedone", 127);
    }
    
    // Stim cooldown timer handling
    if (Player.Turret.StimTimer > 0 && (Timer() % 35) == 0)
        Player.Turret.StimTimer--;
    
    Delay(1);
    goto Start;
}

NamedScript void TurretSensors()
{
    bool Complete = false;
    int Current = 0;
    int Total = 0;
    
    Player.Turret.SensorsActive = true;
    
    Start:
    
    SetActorState(Player.Turret.TID, "Searching");
    SetUserVariable(Player.Turret.TID, "user_doing_sensor_stuff", 1);
    
    if (Player.Turret.SensorMode == TSM_MODULES)
    {
        // Determine the total amount of existing module pickup
        for (int i = 0; ItemTIDs[i] != -1; i++)
            if (!StrCmp(GetActorClass(ItemTIDs[i]), "DRPGModulePickup"))
                Total++;
        
        while (Current < Total)
        {
            for (int i = 0; ItemTIDs[i] != -1; i++)
            {
                // If the turret is destroyed while scanning, abort
                if (!Player.Turret.Active || Player.Turret.Destroyed)
                {
                    Current = Total;
                    break;
                }
            
                if (!StrCmp(GetActorClass(ItemTIDs[i]), "DRPGModulePickup"))
                {
                    SpawnForced("DRPGTurretTeleport", GetActorX(ItemTIDs[i]), GetActorY(ItemTIDs[i]), GetActorFloorZ(ItemTIDs[i]), 0, 0);
                    SetActorPosition(Player.Turret.TID, GetActorX(ItemTIDs[i]), GetActorY(ItemTIDs[i]), GetActorZ(ItemTIDs[i]), false);
                    Player.Turret.ModulesCollected += GetUserVariable(ItemTIDs[i], "user_amount");
                    Thing_Remove(ItemTIDs[i]);
                    Current++;
                    break;
                }
            }
            
            Player.Turret.SensorPercent = (int)(((fixed)Current / (fixed)Total) * 100.0);
            Delay(33 - (Player.Turret.Upgrade[TU_SENSORS] * 3));
        }
        
        if (Total > 0)
        {
            ActivatorSound("items/module", 127);
            GiveInventory("DRPGModule", Player.Turret.ModulesCollected);
            Player.Turret.ModulesCollected = 0;
        }
        
        Complete = true;
    }
    
    if (Player.Turret.SensorMode == TSM_ITEMS)
    {
        // Create the sensor items array
        if (Player.Turret.SensorItemData.Array.Position > 0)
            ArrayDestroy(&Player.Turret.SensorItemData.Array);
        ArrayCreate(&Player.Turret.SensorItemData.Array, "Turret Sensor Items", 64, sizeof(TurretSensorItem));
        
        // Determine the total amount of items
        for (int i = 0; ItemTIDs[i] != -1; i++)
            if (ClassifyActor(ItemTIDs[i]) != ACTOR_NONE)
                Total++;
        
        for (int i = 0; ItemTIDs[i] != -1; i++)
        {
            str Actor = GetActorClass(ItemTIDs[i]);
            ItemInfoPtr Item = NULL;
            
            // If the turret is destroyed while scanning, abort
            if (!Player.Turret.Active || Player.Turret.Destroyed)
                break;
            
            // This item doesn't exist, skip it
            if (ClassifyActor(ItemTIDs[i]) == ACTOR_NONE) continue;
            
            // This item is credits, skip it
            if (Contains(GetActorClass(ItemTIDs[i]), "Credits")) continue;
            
            // Array has grown too big, resize it
            if (Player.Turret.SensorItemData.Array.Position == Player.Turret.SensorItemData.Array.Size)
                ArrayResize(&Player.Turret.SensorItemData.Array);
            
            // Populate item data into the sensor items array
            if (CompatMode == COMPAT_DRLA)
            {
                // GROSS HACK
                // If this comes back as an unknown item, it's likely that it was NOT a weapon and we should go about normally
                // Otherwise, we'll do another search, stripping away the Pickup under the assumption it is a weapon and it will be found in the database
                if (FindItem(StrLeft(Actor, StrLen(Actor) - 6)) == GetBlankItem())
                    Item = FindItem(Actor);
                else
                    Item = FindItem(StrLeft(Actor, StrLen(Actor) - 6));
            }
            else
                Item = FindItem(Actor);
            ((TurretSensorItem *)Player.Turret.SensorItemData.Array.Data)[Player.Turret.SensorItemData.Array.Position].TID = ItemTIDs[i];
            ((TurretSensorItem *)Player.Turret.SensorItemData.Array.Data)[Player.Turret.SensorItemData.Array.Position].Category = Item->Category;
            Player.Turret.SensorItemData.Array.Position++;
            
            SpawnForced("DRPGTurretTeleport", GetActorX(ItemTIDs[i]), GetActorY(ItemTIDs[i]), GetActorFloorZ(ItemTIDs[i]), 0, 0);
            SetActorPosition(Player.Turret.TID, GetActorX(ItemTIDs[i]), GetActorY(ItemTIDs[i]), GetActorZ(ItemTIDs[i]));
            Current++;
            
            Player.Turret.SensorPercent = (int)(((fixed)Current / (fixed)Total) * 100.0);
            Delay(33 - (Player.Turret.Upgrade[TU_SENSORS] * 3));
        }
        
        Player.Turret.SensorItemData.Enabled = true;
        Complete = true;
    }
    
    if (Player.Turret.SensorMode == TSM_CRATE)
    {
        // Create the sensor crates array
        if (Player.Turret.SensorCrateData.Array.Position > 0)
            ArrayDestroy(&Player.Turret.SensorCrateData.Array);
        ArrayCreate(&Player.Turret.SensorCrateData.Array, "Turret Sensor Crates", 64, sizeof(int));
        
        Total = CrateID;
        
        for (int i = 0; i < CrateID; i++)
        {
            // If the turret is deactivated or destroyed while scanning, abort
            if (!Player.Turret.Active || Player.Turret.Destroyed)
                break;
            
            // Crate is empty, skip it
            if (Crates[i].Empty) continue;
            
            // Array has grown too big, resize it
            if (Player.Turret.SensorCrateData.Array.Position == Player.Turret.SensorCrateData.Array.Size)
                ArrayResize(&Player.Turret.SensorCrateData.Array);
            
            ((int *)Player.Turret.SensorCrateData.Array.Data)[Player.Turret.SensorCrateData.Array.Position++] = Crates[i].TID;
            
            SpawnForced("DRPGTurretTeleport", GetActorX(Crates[i].TID), GetActorY(Crates[i].TID), GetActorZ(Crates[i].TID), 0, 0);
            SetActorPosition(Player.Turret.TID, GetActorX(Crates[i].TID), GetActorY(Crates[i].TID), GetActorZ(Crates[i].TID), false);
            Current++;
            
            Player.Turret.SensorPercent = (int)(((fixed)Current / (fixed)Total) * 100.0);
            Delay(33 - (Player.Turret.Upgrade[TU_SENSORS] * 3));
        }
        
        Player.Turret.SensorCrateData.Enabled = true;
        Complete = true;
    }
    
    // Sensor operation complete
    if (Complete)
    {
        SetActorState(Player.Turret.TID, "Spawn");
        SetUserVariable(Player.Turret.TID, "user_doing_sensor_stuff", 0);
        SetUserVariable(Player.Turret.TID, "user_needs_teleport", 1);
        Player.Turret.SensorMode = TSM_NONE;
        Player.Turret.SensorPercent = 0;
        Player.Turret.SensorsActive = false;
        return;
    }
    
    Delay(1);
    goto Start;
}

NamedScript void TurretItemSensorThink()
{
    str ItemTrailActors[ITEM_CATEGORIES];
    
    if (CompatMode == COMPAT_DRLA)
    {
        ItemTrailActors[0] = "DRPGTurretParticleItemTrailWeapon";
        ItemTrailActors[1] = "DRPGTurretParticleItemTrailAmmo";
        ItemTrailActors[2] = "DRPGTurretParticleItemTrailHealth";
        ItemTrailActors[3] = "DRPGTurretParticleItemTrailArmor";
        ItemTrailActors[4] = "DRPGTurretParticleItemTrailPowerups";
        ItemTrailActors[5] = "DRPGTurretParticleItemTrailShield";
        ItemTrailActors[6] = "DRPGTurretParticleItemTrailStimAugs";
        ItemTrailActors[7] = "DRPGTurretParticleItemTrailLoot";
        ItemTrailActors[8] = "DRPGTurretParticleItemTrailModPacks";
        ItemTrailActors[9] = "DRPGTurretParticleItemTrailBoots";
        ItemTrailActors[10] = "DRPGTurretParticleItemTrailMisc";
    }
    else
    {
        ItemTrailActors[0] = "DRPGTurretParticleItemTrailWeapon";
        ItemTrailActors[1] = "DRPGTurretParticleItemTrailAmmo";
        ItemTrailActors[2] = "DRPGTurretParticleItemTrailHealth";
        ItemTrailActors[3] = "DRPGTurretParticleItemTrailArmor";
        ItemTrailActors[4] = "DRPGTurretParticleItemTrailPowerups";
        ItemTrailActors[5] = "DRPGTurretParticleItemTrailShield";
        ItemTrailActors[6] = "DRPGTurretParticleItemTrailStimAugs";
        ItemTrailActors[7] = "DRPGTurretParticleItemTrailLoot";
        ItemTrailActors[8] = "DRPGTurretParticleItemTrailMisc";
    }
    
    if (!Player.Turret.SensorItemData.Enabled)
        return;
    
    if (Player.Turret.SensorItemData.Array.Position < 1)
        return;
    
    TurretSensorItem Item;
    
    if (Player.Turret.SensorItemData.PulseDelay == 0)
    {
        if (Player.Turret.SensorItemData.CurrentPosition >= Player.Turret.SensorItemData.Array.Position)
        {
            Player.Turret.SensorItemData.CurrentPosition = 0;
            Player.Turret.SensorItemData.PulseDelay = 35;
        }
        else
        {
            if (Player.Turret.SensorItemData.CheckDelay)
                Player.Turret.SensorItemData.CheckDelay--;
            else
            {
                if (Player.Turret.SensorItemData.CurrentPosition == 0)
                    qsort(Player.Turret.SensorItemData.Array.Data, Player.Turret.SensorItemData.Array.Position, sizeof(TurretSensorItem), TurretSensorDistanceComparison);
                
                Item = *(&((TurretSensorItem *)Player.Turret.SensorItemData.Array.Data)[Player.Turret.SensorItemData.CurrentPosition]);
                
                while (ClassifyActor(Item.TID) == ACTOR_NONE && Player.Turret.SensorItemData.CurrentPosition < Player.Turret.SensorItemData.Array.Position)
                {
                    Player.Turret.SensorItemData.CurrentPosition++;
                    
                    if (Player.Turret.SensorItemData.CurrentPosition < Player.Turret.SensorItemData.Array.Position)
                        Item = *(&((TurretSensorItem *)Player.Turret.SensorItemData.Array.Data)[Player.Turret.SensorItemData.CurrentPosition]);
                }
                
                if (Distance(Player.TID, Item.TID) > GetActivatorCVarFixed("drpg_turret_sensordist"))
                    Player.Turret.SensorItemData.CurrentPosition++;
                else if (Player.Turret.SensorItemData.CurrentPosition < Player.Turret.SensorItemData.Array.Position)
                {
                    for (fixed j = 0.0; j <= 1.0; j += 0.01)
                    {
                        fixed X = Lerp(GetActorX(Player.Turret.TID), GetActorX(Item.TID), j);
                        fixed Y = Lerp(GetActorY(Player.Turret.TID), GetActorY(Item.TID), j);
                        fixed Z = Lerp(GetActorZ(Player.Turret.TID), GetActorZ(Item.TID), j);
                        
                        SpawnForced(ItemTrailActors[Item.Category], X, Y, Z, 0, 0);
                    }
                    
                    Player.Turret.SensorItemData.CurrentPosition++;
                    Player.Turret.SensorItemData.CheckDelay = 3;
                }
            }
        }
    }
    else
        Player.Turret.SensorItemData.PulseDelay--;
}

NamedScript void TurretCrateSensorThink()
{
    if (!Player.Turret.SensorCrateData.Enabled)
        return;
    
    if (Player.Turret.SensorCrateData.Array.Position < 1)
        return;
    
    int TID;
    
    if (Player.Turret.SensorCrateData.PulseDelay == 0)
    {
        if (Player.Turret.SensorCrateData.CurrentPosition >= Player.Turret.SensorCrateData.Array.Position)
        {
            Player.Turret.SensorCrateData.CurrentPosition = 0;
            Player.Turret.SensorCrateData.PulseDelay = 35;
        }
        else
        {
            if (Player.Turret.SensorCrateData.CheckDelay)
                Player.Turret.SensorCrateData.CheckDelay--;
            else
            {
                if (Player.Turret.SensorCrateData.CurrentPosition == 0)
                    qsort(Player.Turret.SensorCrateData.Array.Data, Player.Turret.SensorCrateData.Array.Position, sizeof(int), TurretSensorDistanceComparison);
                
                TID = *(&((int *)Player.Turret.SensorCrateData.Array.Data)[Player.Turret.SensorCrateData.CurrentPosition]);
                
                while (ClassifyActor(TID) == ACTOR_NONE && Player.Turret.SensorCrateData.CurrentPosition < Player.Turret.SensorCrateData.Array.Position)
                {
                    Player.Turret.SensorCrateData.CurrentPosition++;
                    
                    if (Player.Turret.SensorCrateData.CurrentPosition < Player.Turret.SensorCrateData.Array.Position)
                        TID = *(&((int *)Player.Turret.SensorCrateData.Array.Data)[Player.Turret.SensorCrateData.CurrentPosition]);
                }
                
                if (Player.Turret.SensorCrateData.CurrentPosition < Player.Turret.SensorCrateData.Array.Position)
                {
                    for (fixed j = 0.0; j <= 1.0; j += 0.01)
                    {
                        fixed X = Lerp(GetActorX(Player.Turret.TID), GetActorX(TID), j);
                        fixed Y = Lerp(GetActorY(Player.Turret.TID), GetActorY(TID), j);
                        fixed Z = Lerp(GetActorZ(Player.Turret.TID), GetActorZ(TID), j);
                        
                        SpawnForced("DRPGTurretParticleCrateTrail", X, Y, Z, 0, 0);
                    }
                    
                    Player.Turret.SensorCrateData.CurrentPosition++;
                    Player.Turret.SensorCrateData.CheckDelay = 3;
                }
            }
        }
    }
    else
        Player.Turret.SensorCrateData.PulseDelay--;
}

NamedScript void TurretPickStimMenu()
{
    str const VialSprites[STIM_MAX] =
    {
        "STVLA0",
        "STVLB0",
        "STVLC0",
        "STVLD0",
        "STVLE0",
        "STVLF0",
        "STVLG0",
        "STVLH0",
        "STVLI0",
        "STVLJ0",
        "STVLK0",
        "STVLL0",
        "STVLM0",
        "STVLN0",
        "STVLO0",
        "STVLP0",
        "STVLQ0",
        "STVLR0",
        "STVLS0"
    };
    
    int Radius = 128;
    int Selection = 0;
    int TypeMax = 0;
    fixed LerpPos = 0;
    
    // Fail if the upgrade isn't present
    if (!Player.Turret.Upgrade[TU_ASSIST_INJECTOR])
    {
        Player.Turret.StimVialType = -1;
        Player.Turret.StimPicked = true;
        PrintError("Your turret does not have the appropriate upgrade");
        ActivatorSound("menu/error", 127);
        return;
    }
    
    // Determine which types can be picked
    if (Player.Turret.Upgrade[TU_ASSIST_INJECTOR_TYPE] == 1) // Stats
        TypeMax = StimStatsEnd;
    else if (Player.Turret.Upgrade[TU_ASSIST_INJECTOR_TYPE] == 2) // Bosters
        TypeMax = StimPowerupStart;
    else // Everything
        TypeMax = STIM_MAX;
    
    // Set Resolution
    SetHudSize(640, 480, false);
    
    // Silly hack to make sure you let go of the key before the actual wheel comes up
    while (GetPlayerInput(PlayerNumber(), INPUT_BUTTONS) & BT_SPEED)
        Delay(1);
    
    ActivatorSound("menu/click", 127);
    
    while (true)
    {
        int Buttons = GetPlayerInput(PlayerNumber(), INPUT_BUTTONS);
        int OldButtons = GetPlayerInput(PlayerNumber(), INPUT_OLDBUTTONS);
        fixed Increment = 1.0 / TypeMax;
        fixed Location = (Increment * (fixed)Selection);
        fixed OldLocation;
        fixed Angle;
        fixed X;
        fixed Y;
        
        if (Location < 0.5 && OldLocation > 0.5 && OldLocation - Location > 0.5)
            OldLocation -= 1.0;
        else if (Location > 0.5 && OldLocation < 0.5 && Location - OldLocation > 0.5)
            OldLocation += 1.0;
        
        // Keep menus closed - This is technically a hack and shouldn't be here
        Player.InMenu = false;
        Player.InShop = false;
        
        // Freeze
        SetPlayerProperty(0, 1, PROP_FROZEN);
        
        // Lerp position
        if (LerpPos <= 1.0)
            LerpPos += 0.1;
        if (LerpPos > 1.0)
            LerpPos = 1.0;
        
        // Vials
        for (int i = 0; i < TypeMax; i++)
        {
            Angle = 0.25 + Lerp(OldLocation, Location, LerpPos) - (Increment * i);
            X = 320.0 + (Radius * Cos(Angle)) + 4.0;
            Y = 240.0 + (Radius * Sin(Angle)) + 16.0;
            
            PrintSprite(VialSprites[i], 0, (int)X, (int)Y, 0.05);
        }
        
        // Box
        PrintSprite("ItemBoxH", 0, 320, 240 + Radius, 0.05);
        
        // Draw Name
        SetFont("BIGFONT");
        HudMessage("%S", CompoundNames[Selection]);
        EndHudMessage(HUDMSG_PLAIN, 0, CompoundColors[Selection], 320, 240 + Radius + 32, 0.05);
        
        // Input
        if (Buttons & BT_USE)
        {
            Player.Turret.StimVialType = Selection;
            Player.Turret.StimPicked = true;
            break;
        }
        if (Buttons & BT_SPEED)
        {
            ActivatorSound("menu/click", 127);
            Player.Turret.StimVialType = -1;
            Player.Turret.StimPicked = true;
            break;
        }
        if (Buttons & BT_MOVELEFT && !(OldButtons & BT_MOVELEFT))
        {
            ActivatorSound("menu/click", 127);
            Selection--;
            OldLocation = Location;
            LerpPos = 0;
            if (Selection < 0) Selection = TypeMax - 1;
        }
        if (Buttons & BT_MOVERIGHT && !(OldButtons & BT_MOVERIGHT))
        {
            ActivatorSound("menu/click", 127);
            Selection++;
            OldLocation = Location;
            LerpPos = 0;
            if (Selection > TypeMax - 1) Selection = 0;
        }
        
        Delay(1);
    }
    
    SetPlayerProperty(0, 0, PROP_FROZEN);
}

NamedScript Type_ENTER void TurretCommandWheel()
{
    int const Commands[MAX_COMMANDS] =
    {
        TU_WEAPON_BULLET,
        TU_WEAPON_BULLET_CAPACITY,
        TU_WEAPON_PELLET,
        TU_WEAPON_PELLET_CAPACITY,
        TU_WEAPON_ROCKET,
        TU_WEAPON_ROCKET_CAPACITY,
        TU_WEAPON_PLASMA,
        TU_WEAPON_PLASMA_CAPACITY,
        TU_WEAPON_RAILGUN,
        TU_WEAPON_RAILGUN_CAPACITY,
        TU_AMMO_AUTOLOADER,
        TU_WEAPON_SELFDESTRUCT,
        TU_ASSIST_INJECTOR_TYPE,
        TU_ASSIST_TELEPORT,
        TU_SENSORS_MODULEFINDER,
        TU_SENSORS_ITEMFINDER,
        TU_SENSORS_SUPPLYFINDER,
        TU_COMMAND_RECALL,
        TU_COMMAND_DRAW_FIRE,
        TU_COMMAND_HOLD_POSITION
    };
    
    Start: NOP;
    
    bool Close = false;
    int CurrentCommands[MAX_COMMANDS];
    int CurrentCommandsCount = 0;
    int Buttons = GetPlayerInput(PlayerNumber(), INPUT_BUTTONS);
    int OldButtons = GetPlayerInput(PlayerNumber(), INPUT_OLDBUTTONS);
    int Radius = 160;
    int CurrentRadius = 0;
    fixed LerpPos = 0;
    
    // Rebuild currently known commands
    for (int i = 0; i < MAX_COMMANDS; i++)
        if (Player.Turret.Upgrade[Commands[i]] > 0)
            CurrentCommands[CurrentCommandsCount++] = Commands[i];
    
    // Open the wheel or quickly activate/deactivate the turret
    if (Buttons & BT_SPEED && Buttons & BT_USER2 && !(OldButtons & BT_USER2))
        TurretCommand(TU_BUILD);
    else if (!(Buttons & BT_SPEED) && Buttons & BT_USER2 && !((Player.InMenu && Player.Menu != 3) || Player.InShop || Player.OutpostMenu > 0 || Player.SkillWheelOpen) && Player.Turret.Init)
    {
        ActivatorSound("menu/click", 127);
        Player.Turret.WheelOpen = true;
        LerpPos = 1.0;
    }
    
    while (Player.Turret.WheelOpen || CurrentRadius > 0)
    {
        fixed Increment = 1.0 / CurrentCommandsCount;
        fixed Location = (Increment * (fixed)Player.Turret.WheelCommand);
        fixed OldLocation;
        fixed Angle;
        fixed X;
        fixed Y;
        
        if (Location < 0.5 && OldLocation > 0.5 && OldLocation - Location > 0.5)
            OldLocation -= 1.0;
        else if (Location > 0.5 && OldLocation < 0.5 && Location - OldLocation > 0.5)
            OldLocation += 1.0;
        
        // Freeze player
        SetPlayerProperty(0, 1, PROP_FROZEN);
        
        // Check Input
        Buttons = GetPlayerInput(PlayerNumber(), INPUT_BUTTONS);
        OldButtons = GetPlayerInput(PlayerNumber(), INPUT_OLDBUTTONS);
        
        // Check for release
        if (!(Buttons & BT_USER2) && !Close)
            Close = true;
        
        // Lerp position
        if (LerpPos <= 1.0)
            LerpPos += 0.1;
        if (LerpPos > 1.0)
            LerpPos = 1.0;
        
        // Animate wheel closed/open
        if (Close)
        {
            CurrentRadius -= 32;
            
            // Closed
            if (CurrentRadius <= 0)
            {
                SetPlayerProperty(0, 0, PROP_FROZEN);
                Player.Turret.WheelOpen = false;
            }
        }
        else if (CurrentRadius < Radius)
            CurrentRadius += 32;
        
        // Set Resolution
        SetHudSize(640, 480, false);
        
        // Turret Timers
        DrawTurretTimers(206.0, 32.0);
        
        // Command Icons/Info
        for (int i = 0; i < CurrentCommandsCount; i++)
        {
            Angle = 0.25 + Lerp(OldLocation, Location, LerpPos) - (Increment * i);
            X = 320.0 + (CurrentRadius * Cos(Angle));
            Y = 240.0 + (CurrentRadius * Sin(Angle));
            
            if (Player.Turret.Upgrade[CurrentCommands[i]] > 0)
                DrawTurretInfo((int)X, (int)Y, CurrentCommands[i]);
            
            PrintSprite(StrParam("T_UPG%d", CurrentCommands[i] + 1), 0, (int)X, (int)Y, 0.05);
        }
        
        // Box
        PrintSprite("ItemBoxH", 0, 320, 240 + CurrentRadius, 0.05);
        
        // Name
        SetFont("BIGFONT");
        HudMessage("%S", TurretUpgradeData[CurrentCommands[Player.Turret.WheelCommand]].Name);
        EndHudMessage(HUDMSG_PLAIN, 0, "White", 320, 240 + CurrentRadius + 32, 0.05);
        
        // Description
        SetFont("SMALLFONT");
        HudMessage("%S", TurretUpgradeData[CurrentCommands[Player.Turret.WheelCommand]].CommandInfo);
        EndHudMessage(HUDMSG_PLAIN, 0, "Yellow", 320.4, 240.1 + CurrentRadius + 48, 0.05);
        
        // Input
        if (!Close)
        {
            if (Buttons & BT_USE && !(OldButtons & BT_USE))
                TurretCommand(CurrentCommands[Player.Turret.WheelCommand]);
            if (Buttons & BT_SPEED && !(OldButtons & BT_SPEED))
                TurretMaintenance();
            if (Buttons & BT_MOVELEFT && !(OldButtons & BT_MOVELEFT))
            {
                ActivatorSound("menu/click", 127);
                Player.Turret.WheelCommand--;
                OldLocation = Location;
                LerpPos = 0;
                if (Player.Turret.WheelCommand < 0) Player.Turret.WheelCommand = CurrentCommandsCount - 1;
            }
            if (Buttons & BT_MOVERIGHT && !(OldButtons & BT_MOVERIGHT))
            {
                ActivatorSound("menu/click", 127);
                Player.Turret.WheelCommand++;
                OldLocation = Location;
                LerpPos = 0;
                if (Player.Turret.WheelCommand > CurrentCommandsCount - 1) Player.Turret.WheelCommand = 0;
            }
        }
        
        Delay(1);
    }
    
    Delay(1);
    goto Start;
}

NamedScript DECORATE void TurretSetOffset(int AngleOffset, int DistanceOffset, int HeightOffset)
{
    Player.Turret.AngleOffset += (fixed)AngleOffset / 100.0;
    Player.Turret.DistanceOffset += (fixed)DistanceOffset;
    Player.Turret.HeightOffset += (fixed)HeightOffset;
}

NamedScript DECORATE void TurretUseAmmo(int Type)
{
    SetActivator(GetActorProperty(0, APROP_MasterTID)); // Transfer from Turret to Player
    
    if (Type == TW_BULLET)
        Player.Turret.BulletAmmo--;
    else if (Type == TW_PELLET)
        Player.Turret.ShellAmmo--;
    else if (Type == TW_ROCKET)
        Player.Turret.RocketAmmo--;
    else if (Type == TW_PLASMA)
        Player.Turret.PlasmaAmmo--;
    else if (Type == TW_RAILGUN)
        Player.Turret.RailAmmo--;
}

NamedScript DECORATE int TurretGetProjectileDamage(int Type)
{
    SetActivator(GetActorProperty(0, APROP_TargetTID)); // Transfer from missile to Turret
    SetActivator(GetActorProperty(0, APROP_MasterTID)); // Transfer from Turret to Player
    
    if (Type == TP_ROCKET)
        return (100 * (Player.Turret.Upgrade[TU_WEAPON_ROCKET_DAMAGE] + 1));
    else if (Type == TP_PLASMA)
        return (10 * (Player.Turret.Upgrade[TU_WEAPON_PLASMA_DAMAGE] + 1));
    
    return 0;
}

NamedScript DECORATE int TurretGetProjectileProperty(int Type)
{
    SetActivator(GetActorProperty(0, APROP_TargetTID)); // Transfer from missile to Turret
    SetActivator(GetActorProperty(0, APROP_MasterTID)); // Transfer from Turret to Player
    
    if (Type == TP_ROCKET)
        return Player.Turret.Upgrade[TU_WEAPON_ROCKET_SEEKING];
    
    return 0;
}

NamedScript DECORATE void TurretPostSelfDestruct()
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        // Player is not in-game
        if (!PlayerInGame(i)) continue;
        
        SetActivator(Players(i).TID);
        FadeRange(255, 255, 255, 0.75, 255, 255, 255, 0.0, 5.0);
    }
    
    SetActivator(GetActorProperty(0, APROP_MasterTID)); // Transfer from Turret to Player
    
    Player.Turret.Active = false;
    Player.Turret.Destroyed = true;
    Player.Turret.PaidForRepair = false;
    Player.Turret.SelfDestruct = false;
    Player.Turret.Health = 0;
}

// Turret AI

NamedScript void TurretAI(int TID)
{
    SetActivator(TID);
    
    int Weapon = GetUserVariable(0, "user_weapon");
    int Ammo = 0;
    int PlayerTID = GetActorProperty(0, APROP_MasterTID);
    int PlayerID = TurretGetOwningPlayer();
    fixed TurretSpeed = GetUserCVarFixed(PlayerID, "drpg_turret_movespeed");
    bool NeedsTeleport = false;
    bool HasTarget = false;
    bool TargetAssignedTID = false;
    bool TargetForced = false;
    bool AnnouncedTarget = false;
    int LosingInterestTime = 0;
    int MaxInterestTime = GetUserCVar(PlayerID, "drpg_turret_pursuittime");
    int TargetTID = 0;
    int TargetOriginalTID = 0;
    int TravelTime = 0;
    int TeleportWaitTime = GetUserCVar(PlayerID, "drpg_turret_telewaittime");
    int AggressionType = GetUserCVar(PlayerID, "drpg_turret_aggression");
    int AggressionMode = 0;
    int HeatLevel = 0;
    int CurrentCommand = GetUserVariable(0, "user_command");
    bool Cooldown = false;
    fixed TurretPlayerDistance = GetUserCVarFixed(PlayerID, "drpg_turret_playerdist");
    fixed TurretEnemyDistance = GetUserCVarFixed(PlayerID, "drpg_turret_enemydist");
    fixed TurretAngle;
    fixed TurretPitch;
    fixed EnemyAngle;
    fixed EnemyPitch;
    fixed DodgeDirection;
    fixed TurretIdleRotation = 0.0;
    
    Start:
    
    if (ClassifyActor(0) == ACTOR_WORLD)
        return;
    
    if (GetUserVariable(0, "user_selfdestruction") || GetUserVariable(0, "user_doing_sensor_stuff"))
    {
        SetActorVelocity(0, GetActorVelX(0) * 0.90625, GetActorVelY(0) * 0.90625, GetActorVelZ(0) * 0.90625, false, false);
        SetActorVelocity(0, 0, 0, 0.125 * Sin((fixed)Timer() / 70.0), true, false);
        
        if (AbsFixed(GetActorVelX(0)) < 0.01 && AbsFixed(GetActorVelY(0)) < 0.01 && AbsFixed(GetActorVelZ(0)) < 0.01)
            SetActorVelocity(0, 0, 0, 0, false, false); // Dead stop if we're going too slow
        
        Delay(1);
        goto Start;
    }
    
    // Just in case it was changed
    TurretSpeed = GetUserCVarFixed(PlayerID, "drpg_turret_movespeed");
    TurretPlayerDistance = GetUserCVarFixed(PlayerID, "drpg_turret_playerdist");
    TurretEnemyDistance = GetUserCVarFixed(PlayerID, "drpg_turret_enemydist");
    AggressionType = GetUserCVar(PlayerID, "drpg_turret_aggression");
    MaxInterestTime = GetUserCVar(PlayerID, "drpg_turret_pursuittime");
    TeleportWaitTime = GetUserCVar(PlayerID, "drpg_turret_telewaittime");
    
    CurrentCommand = GetUserVariable(0, "user_command");
    
    Weapon = GetUserVariable(0, "user_weapon");
    switch (Weapon)
    {
    case TW_BULLET:
        Ammo = GetUserVariable(0, "user_ammo_bullet");
        break;
    case TW_PELLET:
        Ammo = GetUserVariable(0, "user_ammo_shell");
        break;
    case TW_ROCKET:
        Ammo = GetUserVariable(0, "user_ammo_rocket");
        break;
    case TW_PLASMA:
        Ammo = GetUserVariable(0, "user_ammo_plasma");
        break;
    case TW_RAILGUN:
        Ammo = GetUserVariable(0, "user_ammo_rail");
        break;
    }
    
    // Target handling
    HasTarget = TurretCheckTarget() && !TurretCheckTargetIsSelf();
    if (HasTarget)
    {
        TargetTID = UniqueTID();
        TargetOriginalTID = TurretGetTargetTID();
        TargetAssignedTID = TurretForceTargetTID(TargetTID);
    }
    else
    {
        TargetTID = 0;
        TargetOriginalTID = 0;
    }
    
    if (HasTarget)
    {
        if ((!TurretCheckTargetSuitable() && !TargetForced) || TurretCheckTargetDead())
        {
            SetPointer(AAPTR_TARGET, 0, AAPTR_NULL);
            HasTarget = false;
            TargetForced = false;
        }
        else
        {
            if (!CheckSight(0, TargetTID, CSF_NOFAKEFLOORS) && !TargetForced)
            {
                LosingInterestTime++;
                
                if (LosingInterestTime >= 35 * MaxInterestTime)
                {
                    SetPointer(AAPTR_TARGET, 0, AAPTR_NULL);
                    HasTarget = false;
                    LosingInterestTime = 0;
                }
            }
            else
            {
                LosingInterestTime = 0;
                
                if (CurrentCommand == TC_DRAW_FIRE)
                    TurretGetAttention(TargetTID, ActivatorTID());
            }
        }
    }
    
    // Player is ordering us to switch targets
    if (CurrentCommand == TC_DRAW_FIRE && TurretCheckPlayerTarget() && TurretWantsToSwitchToPlayerTarget())
    {
        TurretSyncWithPlayerTarget();
        HasTarget = true;
        TargetForced = true;
        SetActorState(0, "Targeting");
        AnnouncedTarget = true;
    }
    
    // Air friction, don't slide away helplessly like cacodemons do.
    SetActorVelocity(0, GetActorVelX(0) * 0.90625, GetActorVelY(0) * 0.90625, GetActorVelZ(0) * 0.90625, false, false);
    
    // Float bobbing
    SetActorVelocity(0, 0, 0, 0.125 * Sin((fixed)Timer() / 70.0), true, false);
    
    if (AbsFixed(GetActorVelX(0)) < 0.01 && AbsFixed(GetActorVelY(0)) < 0.01 && AbsFixed(GetActorVelZ(0)) < 0.01)
        SetActorVelocity(0, 0, 0, 0, false, false); // Dead stop if we're going too slow
    
    // Movement
    
    // Stay off the floor
    if (GetActorZ(0) - GetActorFloorZ(0) < 32)
        SetActorVelocity(0, 0, 0, TurretSpeed, true, false);
    
    // Teleport detection - If the player suddenly poofs into another part of the map, we need to go with them.
    if (GetActorProperty(PlayerTID, APROP_ReactionTime) > 0 && Distance(0, PlayerTID) > 96.0)
        NeedsTeleport = true;
    
    if (GetUserVariable(0, "user_needs_teleport"))
    {
        NeedsTeleport = true;
        SetUserVariable(0, "user_needs_teleport", 0);
    }
    
    if (NeedsTeleport)
        NeedsTeleport = !TurretTeleport(PlayerTID);
    
    // Spawn a goal flag if we need to hold our position
    if (CurrentCommand == TC_HOLD_POSITION)
    {
        if (!GetUserVariable(0, "user_goal_tid"))
        {
            SetUserVariable(0, "user_goal_tid", UniqueTID());
            SpawnForced("DRPGTurretGoalFlag", GetActorX(0), GetActorY(0), GetActorFloorZ(0), GetUserVariable(0, "user_goal_tid"), GetActorAngle(0) * 256);
        }
    }
    else
    {
        if (GetUserVariable(0, "user_goal_tid"))
        {
            Thing_Remove(GetUserVariable(0, "user_goal_tid"));
            SetUserVariable(0, "user_goal_tid", 0);
        }
    }
    
    /* Turret Status Debug
    SetHudSize(640, 480, 1);
    HudMessage("Turret Status\n\nHas Target: %S\nTarget Forced: %S\nTarget Original TID: %d\nTarget TID: %d\nInterest Time: %d", HasTarget? "Yes" : "No", TargetForced? "Yes" : "No", TargetOriginalTID, TargetTID, LosingInterestTime);
    EndhudMessage(HUDMSG_FADEOUT, 864, "Green", 32.1, 32.1, 1.0, 1.0);
    */
    
    if (!HasTarget)
    {
        // Idle routine
        
        if (AnnouncedTarget)
            AnnouncedTarget = false;
        
        if (CurrentCommand == TC_HOLD_POSITION)
        {
            if (Distance(0, GetUserVariable(0, "user_goal_tid")) > 80.0)
            {
                TurretAngle = VectorAngle(GetActorX(GetUserVariable(0, "user_goal_tid")) - GetActorX(0), GetActorY(GetUserVariable(0, "user_goal_tid")) - GetActorY(0));
                TurretPitch = VectorAngle(Distance(0, GetUserVariable(0, "user_goal_tid")), GetActorZ(GetUserVariable(0, "user_goal_tid")) - GetActorZ(0));
                
                TurretTurn(TurretAngle, TurretPitch);
                TurretFly(TurretAngle, TurretPitch, TurretSpeed);
                
                TurretIdleRotation = 0.0;
                TravelTime++;
            }
            else
            {
                TurretAngle = (GetActorAngle(GetUserVariable(0, "user_goal_tid")) + TurretIdleRotation) % 1.0;
                TurretTurn(TurretAngle, 0);
                
                TurretIdleRotation += 0.007;
                TurretIdleRotation %= 1.0;
            }
            
            if (Weapon != TW_NONE && !(Timer() % 35))
            {
                SetActorState(0, "ScanForEnemies");
            }
        }
        else
        {
            if (Distance(0, PlayerTID) > TurretPlayerDistance)
            {
                if (TravelTime > 35 * TeleportWaitTime && (AbsFixed(GetActorVelX(0)) < 0.1 || AbsFixed(GetActorVelY(0)) < 0.1))
                    // Been trying to reach the player for the specified seconds and going nowhere, so give up and teleport to them if possible
                    NeedsTeleport = true;
                
                TurretAngle = VectorAngle(GetActorX(PlayerTID) - GetActorX(0), GetActorY(PlayerTID) - GetActorY(0));
                TurretPitch = VectorAngle(Distance(0, PlayerTID), GetActorZ(PlayerTID) - GetActorZ(0));
                
                TurretTurn(TurretAngle, TurretPitch);
                TurretFly(TurretAngle, TurretPitch, TurretSpeed);
                
                TravelTime++;
            }
            else
            {
                if (TravelTime)
                    TravelTime = 0;
                
                TurretTurn(GetActorAngle(PlayerTID), -GetActorPitch(PlayerTID));
            }
            
            if (Weapon != TW_NONE && !(Timer() % 35))
            {
                if (TurretCheckPlayerTarget())
                {
                    TurretSyncWithPlayerTarget();
                    HasTarget = true;
                }
                else
                    SetActorState(0, "ScanForEnemies");
            }
        }
    }
    else if (TargetAssignedTID)
    {
        // Battle routine - Circle around the target and fire at it, keeping a distance away.
        
        if (!AnnouncedTarget)
        {
            SetActorState(0, "Targeting");
            AnnouncedTarget = true;
        }
        
        if (CurrentCommand == TC_DRAW_FIRE)
            AggressionMode = 1;
        else if (CurrentCommand == TC_HOLD_POSITION)
            AggressionMode = 0;
        else if (AggressionType == 2) // Determine per weapon
        {
            if (Weapon == TW_PELLET || Weapon == TW_PLASMA)
                AggressionMode = 1;
            else
                AggressionMode = 0;
        }
        else
            AggressionMode = AggressionType;
        
        // We can't be aggressive if we can't fight back
        if (Weapon == TW_NONE || Ammo < 1)
            AggressionMode = 0;
        
        EnemyAngle = VectorAngle(GetActorX(TargetTID) - GetActorX(0), GetActorY(TargetTID) - GetActorY(0));
        EnemyPitch = VectorAngle(Distance(0, TargetTID), GetActorZ(TargetTID) - GetActorZ(0));
        
        TurretTurn(EnemyAngle, EnemyPitch);
        GiveInventory("DRPGTurretTargetingLaser", 1);
        
        if (Distance(0, TargetTID) <= GetActorPropertyFixed(TargetTID, APROP_Radius) + TurretEnemyDistance)
        {
            // Invading our personal space, back away
            TurretFly(EnemyAngle, 0, -TurretSpeed);
        }
        
        if (AggressionMode == 0)
        {
            // Passive Fire - Stay in slow orbit by the player and only fire at the enemy
            
            if (CurrentCommand == TC_HOLD_POSITION)
            {
                 if (GetActorZ(TargetTID) - GetActorZ(0) < -32)
                    SetActorVelocity(0, 0, 0, -TurretSpeed, true, false);
                else if (GetActorZ(TargetTID) - GetActorZ(0) > 32)
                    SetActorVelocity(0, 0, 0, TurretSpeed, true, false);
                
                if (Distance(0, TargetTID) > GetActorPropertyFixed(TargetTID, APROP_Radius) + TurretEnemyDistance || LosingInterestTime)
                    TurretFly(EnemyAngle, -EnemyPitch, TurretSpeed);
                
                TurretFly(EnemyAngle - 0.25, 0, TurretSpeed);
                
                TurretAngle = VectorAngle(GetActorX(GetUserVariable(0, "user_goal_tid")) - GetActorX(0), GetActorY(GetUserVariable(0, "user_goal_tid")) - GetActorY(0));
                TurretPitch = VectorAngle(Distance(0, GetUserVariable(0, "user_goal_tid")), GetActorZ(GetUserVariable(0, "user_goal_tid")) - GetActorZ(0));
                
                if (Distance(0, GetUserVariable(0, "user_goal_tid")) > 80.0)
                {
                    TurretFly(TurretAngle, TurretPitch, TurretSpeed);
                    TurretTurn(TurretAngle, TurretPitch);
                }
            }
            else
            {
                TurretAngle = VectorAngle(GetActorX(PlayerTID) - GetActorX(0), GetActorY(PlayerTID) - GetActorY(0));
                TurretPitch = VectorAngle(Distance(0, PlayerTID), GetActorZ(PlayerTID) - GetActorZ(0));
                
                if (Distance(0, PlayerTID) > TurretPlayerDistance)
                    TurretFly(TurretAngle, TurretPitch, TurretSpeed);
                else
                    TurretFly(TurretAngle, 0, -TurretSpeed);
                
                TurretFly(TurretAngle - 0.25, 0, TurretSpeed / 3.0);
            }
        }
        else
        {
            // Aggressive Fire - Orbit the enemy and draw them away from the player aggressively
            
            // Stay level with the target so we have a better time shooting it
            if (GetActorZ(TargetTID) - GetActorZ(0) < -32)
                SetActorVelocity(0, 0, 0, -TurretSpeed, true, false);
            else if (GetActorZ(TargetTID) - GetActorZ(0) > 32)
                SetActorVelocity(0, 0, 0, TurretSpeed, true, false);
            
            if (Distance(0, TargetTID) > GetActorPropertyFixed(TargetTID, APROP_Radius) + TurretEnemyDistance || LosingInterestTime)
                TurretFly(EnemyAngle, -EnemyPitch, TurretSpeed);
            
            TurretFly(EnemyAngle - 0.25, 0, TurretSpeed);
        }
        
        // Unload all you've got!
        if (Ammo > 0 && !Cooldown && HeatLevel < 100 && !GetUserVariable(0, "user_firing") && CheckSight(0, TargetTID, CSF_NOFAKEFLOORS) && (Weapon != TW_ROCKET || ((Distance(0, TargetTID) > 176.0 || GetUserArray(0, "user_upgrades", TU_ARMOR_PLATING_BLAST)) && Distance(PlayerID, TargetTID) > 176.0)))
        {
            SetActorState(0, "Missile");
            switch (Weapon)
            {
            case TW_BULLET:
                HeatLevel += 5;
                break;
            case TW_PELLET:
                HeatLevel += 15;
                break;
            case TW_ROCKET:
                HeatLevel += 30;
                break;
            case TW_PLASMA:
                HeatLevel += 8;
                break;
            case TW_RAILGUN:
                HeatLevel += 50;
                break;
            }
        }
    }
    
    if (HeatLevel > 0)
    {
        if (HeatLevel >= 100 && !GetUserVariable(0, "user_firing") && !Cooldown)
        {
            SetActorState(0, "Cooldown");
            Cooldown = true;
        }
        
        if (Cooldown)
            SpawnForced("DRPGTurretCooldownIcon", GetActorX(0), GetActorY(0), GetActorZ(0) + 40.0, 0, 0);
        
        if (!(Timer() % 16))
            HeatLevel -= 8;
    }
    else if (Cooldown)
        Cooldown = false;
    
    if (TargetAssignedTID)
    {
        Thing_ChangeTID(TargetTID, TargetOriginalTID);
        TargetAssignedTID = false;
    }
    
    /* Aim debugging assist
    if (GetCVar("drpg_debug"))
      SpawnForced("DRPGTurretParticleAssistHealth",
                  GetActorX(0) + (64.0 * Cos(GetActorAngle(0)) * Cos(-GetActorPitch(0))),
                  GetActorY(0) + (64.0 * Sin(GetActorAngle(0)) * Cos(-GetActorPitch(0))),
                  GetActorZ(0) + (64.0 * Sin(-GetActorPitch(0))), 0, 0); */
    
    Delay(1);
    
    goto Start;
}

void TurretFly(fixed Angle, fixed Pitch, fixed Speed)
{
    SetActorVelocity(0, Speed * Cos(Angle) * Cos(Pitch), Speed * Sin(Angle) * Cos(Pitch), Speed * Sin(Pitch), true, false);
}

void TurretTurn(fixed NewAngle, fixed NewPitch)
{
    fixed OldAngle = GetActorAngle(0);
    fixed OldPitch = GetActorPitch(0);
    
    NewPitch = -(((0.5 + NewPitch) % 1.0) - 0.5);
    
    if (OldAngle == NewAngle && OldPitch == NewPitch)
        return;
    
    if (OldAngle != NewAngle)
    {
        if (OldAngle < NewAngle && NewAngle - OldAngle < 0.5)
            SetActorAngle(0, OldAngle + Clamp(0, NewAngle - OldAngle, 0.03));
        else if (OldAngle < NewAngle && NewAngle - OldAngle > 0.5)
            SetActorAngle(0, OldAngle + Clamp(-0.03, NewAngle - (OldAngle + 1.0), 0));
        else if (OldAngle > NewAngle && OldAngle - NewAngle < 0.5)
            SetActorAngle(0, OldAngle + Clamp(-0.03, NewAngle - OldAngle, 0));
        else
            SetActorAngle(0, OldAngle + Clamp(0, (NewAngle + 1.0) - OldAngle, 0.03));
    }
    
    if (OldPitch != NewPitch)
        SetActorPitch(0, OldPitch + Clamp(-0.03, NewPitch - OldPitch, 0.03));
}

NamedScript bool TurretCheckTarget()
{
    return SetActivator(0, AAPTR_TARGET);
}

NamedScript bool TurretCheckTargetIsSelf()
{
    return IsPointerEqual(AAPTR_TARGET, AAPTR_DEFAULT);
}

NamedScript bool TurretCheckPlayerTarget()
{
    if (!SetActivator(0, AAPTR_FRIENDPLAYER))
        return false;
    
    return SetActivator(0, AAPTR_PLAYER_GETTARGET);
}

NamedScript bool TurretCheckTargetSuitable()
{
    bool Suitable = true;
    int OriginalTargetTID;
    int NewTargetTID = UniqueTID();
    
    if (!SetActivator(0, AAPTR_TARGET))
        return false;
    
    int ActorInfo = ClassifyActor(0);
    if (ActorInfo == ACTOR_WORLD || ActorInfo & ACTOR_DEAD || CheckFlag(0, "NONSHOOTABLE") || !CheckFlag(0, "SHOOTABLE") || CheckFlag(0, "FRIENDLY") || CheckFlag(0, "NEVERTARGET") || !CheckFlag(0, "ISMONSTER"))
        Suitable = false;
    
    return Suitable;
}

NamedScript bool TurretCheckTargetDead()
{
    bool TargetDead = false;
    int OriginalTargetTID;
    int NewTargetTID = UniqueTID();
    
    if (!SetActivator(0, AAPTR_TARGET))
        return true;

    int ActorInfo = ClassifyActor(0);
    if (ActorInfo == ACTOR_WORLD || ActorInfo & ACTOR_DEAD)
        TargetDead = true;
    
    return TargetDead;
}

NamedScript void TurretGetAttention(int MonsterTID, int TurretTID)
{
    if (!SetActivator(MonsterTID))
        return;
    
    SetPointer(AAPTR_TARGET, TurretTID);
}

NamedScript bool TurretForceTargetTID(int NewTID)
{
    SetActivator(0, AAPTR_TARGET | AAPTR_NULL);
    
    if (ClassifyActor(0) != ACTOR_WORLD)
    {
        Thing_ChangeTID(0, NewTID);
        return true;
    }
    
    return false;
}

NamedScript void TurretSyncWithPlayerTarget()
{
    int EnemyOriginalTID = 0;
    int EnemyTID = 0;
    int TurretOriginalTID = ActivatorTID();
    int TurretTID = UniqueTID();
    Thing_ChangeTID(0, TurretTID);
    
    if (!SetActivator(0, AAPTR_FRIENDPLAYER))
    {
        Thing_ChangeTID(TurretTID, TurretOriginalTID);
        return;
    }
    
    if (SetActivator(0, AAPTR_PLAYER_GETTARGET | AAPTR_NULL) && ClassifyActor(0) != ACTOR_WORLD)
    {
        EnemyOriginalTID = ActivatorTID();
        EnemyTID = UniqueTID();
        Thing_ChangeTID(0, EnemyTID);
        
        SetActivator(TurretTID);
        SetPointer(AAPTR_TARGET, EnemyTID);
    }
    
    if (EnemyTID != 0)
        Thing_ChangeTID(EnemyTID, EnemyOriginalTID);
    
    Thing_ChangeTID(TurretTID, TurretOriginalTID);
}

NamedScript int TurretGetOwningPlayer()
{
    if (!SetActivator(0, AAPTR_FRIENDPLAYER))
        return -1;
    
    return PlayerNumber();
}

NamedScript int TurretGetTargetTID()
{
    if (SetActivator(0, AAPTR_TARGET | AAPTR_NULL) && ClassifyActor(0) != ACTOR_WORLD)
        return ActivatorTID();
    
    return 0;
}

NamedScript bool TurretWantsToSwitchToPlayerTarget()
{
    if (!SetActivator(0, AAPTR_FRIENDPLAYER))
        return false;
    
    return GetPlayerInput(-1, MODINPUT_BUTTONS) & BT_ATTACK && GetPlayerInput(-1, MODINPUT_BUTTONS) & BT_SPEED;
}

void BuildTurretData()
{
    // Command description for Stim Injector
    TurretUpgradeData[TU_ASSIST_INJECTOR].CommandInfo = StrParam("Use \Cd%jS\C- + \Cd%jS\Ck to issue this command", "+speed", "drpg_usestim");
    TurretUpgradeData[TU_COMMAND_DRAW_FIRE].CommandInfo = StrParam("When issued, use \Cd%jS\C- + \Cd%jS\Ck to force a target switch", "+speed", "+attack");
}

bool TurretTeleport(int DestTID)
{
    fixed OldX = GetActorX(0);
    fixed OldY = GetActorY(0);
    fixed OldZ = GetActorZ(0);
    fixed TeleX[3] =
    {
        GetActorX(DestTID) + (64.0 * Cos(GetActorAngle(DestTID) + 0.25)),
        GetActorX(DestTID) + (64.0 * Cos(GetActorAngle(DestTID) - 0.25)),
        GetActorX(DestTID) + (64.0 * Cos(GetActorAngle(DestTID)))
    };
    fixed TeleY[3] =
    {
        GetActorY(DestTID) + (64.0 * Sin(GetActorAngle(DestTID) + 0.25)),
        GetActorY(DestTID) + (64.0 * Sin(GetActorAngle(DestTID) - 0.25)),
        GetActorY(DestTID) + (64.0 * Sin(GetActorAngle(DestTID)))
    };
    fixed TeleZ = GetActorZ(DestTID) + 32.0;
    
    bool Teleported = false;
    for (int i = 0; i < 3; i++)
    {
        // Try left, right, then in front
        Teleported = SetActorPosition(0, TeleX[i], TeleY[i], TeleZ, false);
        
        if (Teleported && CheckSight(DestTID, 0, CSF_NOFAKEFLOORS))
        {
            PlaySound(0, "turret/spawn", CHAN_BODY, 1.0, false, ATTN_NORM);
            SpawnForced("DRPGTurretTeleport", TeleX[i], TeleY[i], TeleZ - 32.0, 0, 0);
            
            SetPointer(AAPTR_TARGET, 0, AAPTR_NULL);
            break;
        }
        else
        {
            Teleported = false;
            continue;
        }
    }
    
    if (!Teleported)
        SetActorPosition(0, OldX, OldY, OldZ, false);
    
    return Teleported;
}

void TurretMaintenance()
{
    // Don't do anything if the turret isn't built yet
    if (!Player.Turret.Upgrade[TU_BUILD]) return;
    
    // Don't send for maintenance if there is no maintenance to be done
    if (!Player.Turret.Maintenance && Player.Turret.ChargeTimer <= 0 && Player.Turret.RepairTimer <= 0 && Player.Turret.RefitTimer <= 0) return;
    
    // You don't have any money to begin maintenance
    if (!CheckInventory("DRPGCredits"))
    {
        ActivatorSound("menu/error", 127);
        PrintError("Cannot perform maintenance because you have no money");
        return;
    }
    
    if (!Player.Turret.Maintenance)
    {
        ActivatorSound("menu/move", 127);
        Player.Turret.Maintenance = true;
        TurretDespawn();
    }
    else
    {
        ActivatorSound("menu/move", 127);
        Player.Turret.Maintenance = false;
    }
}

bool TurretLoadAmmo(int Type)
{
    str AmmoActor[5] =
    {
        "Clip",
        "Shell",
        "RocketAmmo",
        "Cell",
        "Cell"
    };
    str ReloadSound[5] =
    {
        "turret/reloadbullet",
        "turret/reloadpellet",
        "turret/reloadrocket",
        "turret/reloadplasma",
        "turret/reloadrail"
    };
    int MinAmount[5] = { 50, 20, 5, 100, 50 };
    int *Ammo[5] =
    {
        &Player.Turret.BulletAmmo,
        &Player.Turret.ShellAmmo,
        &Player.Turret.RocketAmmo,
        &Player.Turret.PlasmaAmmo,
        &Player.Turret.RailAmmo
    };
    int *AmmoMax[5] =
    {
        &Player.Turret.BulletAmmoMax,
        &Player.Turret.ShellAmmoMax,
        &Player.Turret.RocketAmmoMax,
        &Player.Turret.PlasmaAmmoMax,
        &Player.Turret.RailAmmoMax
    };
    int AmmoType;
    
    // Determine proper ammo type from param
    switch (Type)
    {
    case TU_WEAPON_BULLET:  AmmoType = 0; break;
    case TU_WEAPON_PELLET:  AmmoType = 1; break;
    case TU_WEAPON_ROCKET:  AmmoType = 2; break;
    case TU_WEAPON_PLASMA:  AmmoType = 3; break;
    case TU_WEAPON_RAILGUN: AmmoType = 4; break;
    }
    
    if (CheckInventory(AmmoActor[AmmoType]) >= MinAmount[AmmoType] && AmmoType == 4) // Railgun special case
    {
        (*Ammo[AmmoType])++;
        TakeInventory(AmmoActor[AmmoType], MinAmount[AmmoType]);
        ThingSound(Player.Turret.TID, ReloadSound[AmmoType], 127);
        return true;
    }
    else if (CheckInventory(AmmoActor[AmmoType]) >= MinAmount[AmmoType] && *Ammo[AmmoType] + MinAmount[AmmoType] > *AmmoMax[AmmoType])
    {
        int Amount = *AmmoMax[AmmoType] - *Ammo[AmmoType];
        *Ammo[AmmoType] += Amount;
        TakeInventory(AmmoActor[AmmoType], Amount);
        ThingSound(Player.Turret.TID, ReloadSound[AmmoType], 127);
        return true;
    }
    else if (CheckInventory(AmmoActor[AmmoType]) >= MinAmount[AmmoType])
    {
        *Ammo[AmmoType] += MinAmount[AmmoType];
        TakeInventory(AmmoActor[AmmoType], MinAmount[AmmoType]);
        ThingSound(Player.Turret.TID, ReloadSound[AmmoType], 127);
        return true;
    }

    return false;
}

void TurretCommand(int Index)
{
    // Don't issue the command if you don't have the base upgrade yet
    if (!Player.Turret.Upgrade[Index]) return;
    
    if (Index == TU_BUILD)
    {
        if (Player.Turret.Active)
            TurretDespawn();
        else
            TurretSpawn();
    }
    
    // Do these checks after TurretSpawn/TurretDespawn, because TurretSpawn will print its own errors for these conditions, and these conditions aren't even possible for TurretDespawn.
    if (Player.Turret.Destroyed || Player.Turret.Maintenance) return;
    
    if (Index == TU_WEAPON_BULLET && Player.Turret.Active)
    {
        if (Player.Turret.Weapon != TW_BULLET)
        {
            Player.Turret.Weapon = TW_BULLET;
            ThingSound(Player.Turret.TID, "turret/loadbullet", 127);
        }
        else
        {
            Player.Turret.Weapon = TW_NONE;
            ThingSound(Player.Turret.TID, "turret/cooldown", 127);
        }
    }
    
    if (Index == TU_WEAPON_BULLET_CAPACITY && Player.Turret.BulletAmmo < Player.Turret.BulletAmmoMax && Player.Turret.Active)
    {
        if (!TurretLoadAmmo(TU_WEAPON_BULLET))
        {
            ActivatorSound("menu/error", 127);
            PrintError("You need at least \Ca50 Bullets\C- to load into the turret");
        }
    }
    
    if (Index == TU_WEAPON_PELLET && Player.Turret.Active)
    {
        if (Player.Turret.Weapon != TW_PELLET)
        {
            Player.Turret.Weapon = TW_PELLET;
            ThingSound(Player.Turret.TID, "turret/loadpellet", 127);
        }
        else
        {
            Player.Turret.Weapon = TW_NONE;
            ThingSound(Player.Turret.TID, "turret/cooldown", 127);
        }
    }

    if (Index == TU_WEAPON_PELLET_CAPACITY && Player.Turret.ShellAmmo < Player.Turret.ShellAmmoMax && Player.Turret.Active)
    {
        if (!TurretLoadAmmo(TU_WEAPON_PELLET))
        {
            ActivatorSound("menu/error", 127);
            PrintError("You need at least \Ci20 Shells\C- to load into the turret");
        }
    }
    
    if (Index == TU_WEAPON_ROCKET && Player.Turret.Active)
    {
        if (Player.Turret.Weapon != TW_ROCKET)
        {
            Player.Turret.Weapon = TW_ROCKET;
            ThingSound(Player.Turret.TID, "turret/loadrocket", 127);
        }
        else
        {
            Player.Turret.Weapon = TW_NONE;
            ThingSound(Player.Turret.TID, "turret/cooldown", 127);
        }
    }

    if (Index == TU_WEAPON_ROCKET_CAPACITY && Player.Turret.RocketAmmo < Player.Turret.RocketAmmoMax && Player.Turret.Active)
    {
        if (!TurretLoadAmmo(TU_WEAPON_ROCKET))
        {
            ActivatorSound("menu/error", 127);
            PrintError("You need at least \Cc5 Rockets\C- to load into the turret");
        }
    }
    
    if (Index == TU_WEAPON_PLASMA && Player.Turret.Active)
    {
        if (Player.Turret.Weapon != TW_PLASMA)
        {
            Player.Turret.Weapon = TW_PLASMA;
            ThingSound(Player.Turret.TID, "turret/loadplasma", 127);
        }
        else
        {
            Player.Turret.Weapon = TW_NONE;
            ThingSound(Player.Turret.TID, "turret/cooldown", 127);
        }
    }
    
    if (Index == TU_WEAPON_PLASMA_CAPACITY && Player.Turret.PlasmaAmmo < Player.Turret.PlasmaAmmoMax && Player.Turret.Active)
    {
        if (!TurretLoadAmmo(TU_WEAPON_PLASMA))
        {
            ActivatorSound("menu/error", 127);
            PrintError("You need at least \Cd100 Cells\C- to load into the turret");
        }
    }
    
    if (Index == TU_WEAPON_RAILGUN && Player.Turret.Active)
    {
        if (Player.Turret.Weapon != TW_RAILGUN)
        {
            Player.Turret.Weapon = TW_RAILGUN;
            ThingSound(Player.Turret.TID, "turret/loadrail", 127);
        }
        else
        {
            Player.Turret.Weapon = TW_NONE;
            ThingSound(Player.Turret.TID, "turret/cooldown", 127);
        }
    }
    
    if (Index == TU_WEAPON_RAILGUN_CAPACITY && Player.Turret.RailAmmo < Player.Turret.RailAmmoMax && Player.Turret.Active)
    {
        if (!TurretLoadAmmo(TU_WEAPON_RAILGUN))
        {
            ActivatorSound("menu/error", 127);
            PrintError("You need at least \Cd50 Cells\C- to load into the turret");
        }
    }
    
    if (Index == TU_AMMO_AUTOLOADER)
    {
        ActivatorSound("menu/move", 127);
        Player.Turret.Autoload = !Player.Turret.Autoload;
    }
    
    if (Index == TU_WEAPON_SELFDESTRUCT && !Player.Turret.SelfDestruct && Player.Turret.Active)
    {
        SetActorState(Player.Turret.TID, "SelfDestruct");
        Player.Turret.SelfDestruct = true;
    }
    
    if (Index == TU_ASSIST_INJECTOR_TYPE)
    {
        ActivatorSound("menu/move", 127);
        Player.Turret.StimFocused = !Player.Turret.StimFocused;
    }
    
    if (Index == TU_ASSIST_TELEPORT)
    {
        ActivatorSound("menu/move", 127);
        Player.Turret.TeleportEnabled = !Player.Turret.TeleportEnabled;
    }
    
    if (Index == TU_SENSORS_MODULEFINDER && !Player.Turret.SensorsActive && Player.Turret.Active)
    {
        ActivatorSound("menu/move", 127);
        Player.Turret.SensorMode = TSM_MODULES;
        TurretSensors();
    }
    
    if (Index == TU_SENSORS_ITEMFINDER && !Player.Turret.SensorsActive && Player.Turret.Active)
    {
        ActivatorSound("menu/move", 127);
        Player.Turret.SensorMode = TSM_ITEMS;
        TurretSensors();
    }
    
    if (Index == TU_SENSORS_SUPPLYFINDER && !Player.Turret.SensorsActive && Player.Turret.Active)
    {
        ActivatorSound("menu/move", 127);
        Player.Turret.SensorMode = TSM_CRATE;
        TurretSensors();
    }
    
    if (Index == TU_COMMAND_RECALL && Player.Turret.Active)
    {
        ActivatorSound("menu/move", 127);
        Player.Turret.Command = TC_NONE;
        SetUserVariable(Player.Turret.TID, "user_needs_teleport", 1);
    }
    
    if (Index == TU_COMMAND_DRAW_FIRE && Player.Turret.Active)
    {
        ActivatorSound("menu/move", 127);
        if (Player.Turret.Command != TC_DRAW_FIRE)
            Player.Turret.Command = TC_DRAW_FIRE;
        else
            Player.Turret.Command = TC_NONE;
    }
    
    if (Index == TU_COMMAND_HOLD_POSITION && Player.Turret.Active)
    {
        ActivatorSound("menu/move", 127);
        if (Player.Turret.Command != TC_HOLD_POSITION)
            Player.Turret.Command = TC_HOLD_POSITION;
        else
            Player.Turret.Command = TC_NONE;
    }
}

bool TurretStim()
{
    if (!Player.Turret.Upgrade[TU_BUILD])
    {
        PrintError("You haven't built a turret");
        ActivatorSound("menu/error", 127);
        return false;
    }
    
    if (!Player.Turret.Active)
    {
        PrintError("Your turret is not active");
        ActivatorSound("menu/error", 127);
        return false;
    }
    
    if (!Player.Turret.Upgrade[TU_ASSIST_INJECTOR] || !Player.Turret.Upgrade[TU_ASSIST_INJECTOR_TYPE] || !Player.Turret.Upgrade[TU_ASSIST_INJECTOR_AMOUNT] || !Player.Turret.Upgrade[TU_ASSIST_INJECTOR_POTENCY])
    {
        PrintError("Your turret does not have the appropriate upgrade");
        ActivatorSound("menu/error", 127);
        return false;
    }
    
    if (Distance(Player.Turret.TID, Player.TID) >= 192)
    {
        PrintError("You are too far away from the turret for a stim injection");
        ActivatorSound("menu/error", 127);
        return false;
    }
    
    if (Player.Turret.StimTimer > 0)
    {
        PrintError(StrParam("Your turret is currently generating more compounds and will be ready in \Cc%S", FormatTime(Player.Turret.StimTimer * 35)));
        ActivatorSound("menu/error", 127);
        return false;
    }
    
    bool Powerups = (Player.Turret.Upgrade[TU_ASSIST_INJECTOR_TYPE] > 1);
    int MaxAmount = Player.Turret.Upgrade[TU_ASSIST_INJECTOR_AMOUNT];
    int Potency = Player.Turret.Upgrade[TU_ASSIST_INJECTOR_POTENCY];
    int Amount = 0;
    
	
	/*
    // Generate the stim
    if (Player.Turret.StimFocused) // Focused Injection
    {
        // Multipliers
        if (Player.Turret.StimVialType < StimStatsEnd)
            Potency *= 10;
        if (Player.Turret.StimVialType == STIM_PURIFIER || Player.Turret.StimVialType == STIM_POTENCY)
            Potency *= 2;
        
        Player.Stim.Current[Player.Turret.StimVialType] += Potency;
    }
    else // Broad injection
        while (Amount++ < MaxAmount)
            Player.Stim.Current[Random(0, (Powerups ? StimPowerupEnd : StimStatsEnd))] += Potency;
    
    // Reset stim cooldown timer
    if (Player.Turret.StimFocused)
        Player.Turret.StimTimer = 60 * 10 - (Player.Turret.Upgrade[TU_ASSIST_INJECTOR] * 30);
    else
        Player.Turret.StimTimer = 60 * 5 - (Player.Turret.Upgrade[TU_ASSIST_INJECTOR] * 24);
    */
	// Generate the stim
    if (Player.Turret.StimFocused) // Focused Injection
        Player.Stim.Current[Player.Turret.StimVialType] += Potency * 10;
    else // Broad injection
        for (int i = 0; i < StimStatsEnd; i++)
            Player.Stim.Current[Random(0, StimStatsEnd)] += Potency;
    
    // Reset stim cooldown timer
    Player.Turret.StimTimer = 60 * 10 - (Player.Turret.Upgrade[TU_ASSIST_INJECTOR] * 24);
    
    return true;
}

int TurretUpgradeCost(int Index)
{
    int Cost = TurretUpgradeData[Index].Cost * (Player.Turret.Upgrade[Index] + 1);
    
    // Technician gets a 25% cost reduction bonus
    if (CompatMode == COMPAT_DRLA && PlayerClass(PlayerNumber()) == 2)
        return (int)(Cost * 0.75);

    return Cost;
}

void TurretPassVars()
{
    // Master TID
    SetUserVariable(Player.Turret.TID, "user_master", Player.TID);
    
    // Upgrade info
    for (int i = 0; i < MAX_UPGRADES; i++)
        SetUserArray(Player.Turret.TID, "user_upgrade", i, Player.Turret.Upgrade[i]);
    
    // Weapon Type
    SetUserVariable(Player.Turret.TID, "user_weapon", Player.Turret.Weapon);
    
    // Bullet
    SetUserVariable(Player.Turret.TID, "user_bullet_damage", Player.Turret.Upgrade[TU_WEAPON_BULLET_DAMAGE]);
    SetUserVariable(Player.Turret.TID, "user_bullet_rof", Player.Turret.Upgrade[TU_WEAPON_BULLET_ROF]);
    
    // Pellet
    SetUserVariable(Player.Turret.TID, "user_pellet_damage", Player.Turret.Upgrade[TU_WEAPON_PELLET_DAMAGE]);
    SetUserVariable(Player.Turret.TID, "user_pellet_rof", Player.Turret.Upgrade[TU_WEAPON_PELLET_ROF]);
    SetUserVariable(Player.Turret.TID, "user_pellet_spread", Player.Turret.Upgrade[TU_WEAPON_PELLET_SPREAD]);
    SetUserVariable(Player.Turret.TID, "user_pellet_amount", Player.Turret.Upgrade[TU_WEAPON_PELLET_AMOUNT]);
    
    // Rocket
    SetUserVariable(Player.Turret.TID, "user_rocket_rof", Player.Turret.Upgrade[TU_WEAPON_ROCKET_ROF]);
    
    // Plasma
    SetUserVariable(Player.Turret.TID, "user_plasma_rof", Player.Turret.Upgrade[TU_WEAPON_PLASMA_ROF]);
    
    // Railgun
    SetUserVariable(Player.Turret.TID, "user_railgun_damage", Player.Turret.Upgrade[TU_WEAPON_RAILGUN_DAMAGE]);
    SetUserVariable(Player.Turret.TID, "user_railgun_rof", Player.Turret.Upgrade[TU_WEAPON_RAILGUN_ROF]);
    SetUserVariable(Player.Turret.TID, "user_railgun_ripping", Player.Turret.Upgrade[TU_WEAPON_RAILGUN_RIPPING] * 2);
    
    // Ammo
    SetUserVariable(Player.Turret.TID, "user_ammo_bullet", Player.Turret.BulletAmmo);
    SetUserVariable(Player.Turret.TID, "user_ammo_shell", Player.Turret.ShellAmmo);
    SetUserVariable(Player.Turret.TID, "user_ammo_rocket", Player.Turret.RocketAmmo);
    SetUserVariable(Player.Turret.TID, "user_ammo_plasma", Player.Turret.PlasmaAmmo);
    SetUserVariable(Player.Turret.TID, "user_ammo_rail", Player.Turret.RailAmmo);
    
    // Self Destruct
    SetUserVariable(Player.Turret.TID, "user_destruct_range", Player.Turret.Upgrade[TU_WEAPON_SELFDESTRUCT] * 1024);
    SetUserVariable(Player.Turret.TID, "user_destruct_damage", Player.Turret.Upgrade[TU_WEAPON_SELFDESTRUCT] * 1000);
    
    // Misc
    SetUserVariable(Player.Turret.TID, "user_command", Player.Turret.Command);
}

int TurretSensorDistanceComparison(void const *ItemPtr1, void const *ItemPtr2)
{
    int TID1 = (*(TurretSensorItem *)ItemPtr1).TID;
    int TID2 = (*(TurretSensorItem *)ItemPtr2).TID;
    
    if (ClassifyActor(TID1) == ACTOR_NONE && ClassifyActor(TID2) == ACTOR_NONE)
        return 0;
    if (ClassifyActor(TID1) == ACTOR_NONE)
        return -1;
    if (ClassifyActor(TID2) == ACTOR_NONE)
        return 1;
    
    fixed Distance1 = Distance(Player.Turret.TID, TID1);
    fixed Distance2 = Distance(Player.Turret.TID, TID2);
    
    if (Distance2 == Distance1)
        return 0;
    else if (Distance2 > Distance1)
        return -1;
    return 1;
}

// Spawning/Despawning

void TurretSpawn()
{
    if (GetActorProperty(0, APROP_Health) <= 0) return;
    
    if (Player.Turret.Battery <= 0)
    {
        ActivatorSound("menu/error", 127);
        PrintError("Your turret's battery is depleted");
        return;
    }
    
    if (Player.Turret.Destroyed)
    {
        ActivatorSound("menu/error", 127);
        PrintError("Your turret is destroyed and must be repaired");
        return;
    }
    
    if (Player.Turret.Maintenance)
    {
        ActivatorSound("menu/error", 127);
        PrintError("Your turret is currently in the Outpost for maintenance");
        return;
    }
    
    if (Player.Turret.RefitTimer > 0)
    {
        ActivatorSound("menu/error", 127);
        PrintError("Your turret is currently being refitted with upgrades");
        return;
    }
    
    int TID = UniqueTID();
    const fixed Distance = 80.0;
    fixed X = GetActorX(0) + (80.0 * Cos(GetActorAngle(0)));
    fixed Y = GetActorY(0) + (80.0 * Sin(GetActorAngle(0)));
    fixed Z = GetActorZ(0) + 32.0;
    
    bool Spawned = Spawn("DRPGPortableTurret", X, Y, Z, TID, GetActorAngle(0) * 256);
    bool CanSee = CheckSight(Player.TID, TID, 0);
    
    if (Spawned && CanSee)
    {
        PlaySound(TID, "turret/spawn", CHAN_BODY, 1.0, false, ATTN_NORM);
        Player.Turret.TID = TID;
        SetActorProperty(TID, APROP_MasterTID, Player.TID);
        Thing_SetTranslation(TID, -1);
        SpawnForced("DRPGTurretTeleport", X, Y, Z - 32.0, 0, 0);
        
        if (Player.Turret.Health > 0)
            SetActorProperty(TID, APROP_Health, Player.Turret.Health);
        
        TurretAI(TID);
        
        Player.Turret.Active = true;
    }
    else
    {
        Thing_Remove(TID);
        ActivatorSound("menu/error", 127);
        PrintError("The turret has no room to spawn here");
    }
}

void TurretDespawn()
{
    if (!Player.Turret.Active) return;
    
    Player.Turret.Active = false;
    Player.Turret.SensorsActive = false;
    Thing_Remove(GetUserVariable(Player.Turret.TID, "user_goal_tid"));
    
    PlaySound(Player.Turret.TID, "turret/despawn", CHAN_BODY, 1.0, false, ATTN_NORM);
    SpawnForced("DRPGTurretTeleport", GetActorX(Player.Turret.TID), GetActorY(Player.Turret.TID), GetActorZ(Player.Turret.TID) - 32.0, 0, 0);
    Thing_Remove(Player.Turret.TID);
    Player.Turret.TID = 0;
}

// Debugging

NamedScript Console void GiveTurret()
{
    Player.Turret.Upgrade[TU_BUILD] = true;
}

NamedScript Console void FullTurret()
{
    Player.Turret.Upgrade[TU_BUILD] = true;
    
    Delay(1);
    
    for (int i = 1; i < TU_MAX; i++)
        Player.Turret.Upgrade[i] = TurretUpgradeData[i].MaxLevel;
        
    Delay(1);
    
    Player.Turret.Health = Player.Turret.HealthMax;
    TurretSpawn();
}
