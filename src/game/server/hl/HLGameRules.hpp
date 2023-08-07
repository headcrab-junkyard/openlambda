/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2023 BlackPhrase
 *
 * OpenLambda Project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenLambda Project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenLambda Project. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief rules for the single player Half-Life game

#pragma once

#include <IGameRules.hpp>

// CHalfLifeRules
class CHLGameRules : public IGameRules // TODO: CBaseGameRules?
{
public:
	CHLGameRules();
	virtual ~CHLGameRules() = default;
	
	virtual void Think() override {}
	
	virtual bool IsAllowedToSpawn(CBaseEntity *apEntity) const override {return true;}
	
	virtual bool IsFlashlightAllowed() const override {return true;}
	
	virtual bool ShouldSwitchWeapon(CBasePlayer *apPlayer, CBaseItem *apWeapon) const override;
	
	virtual bool GetNextBestWeapon(CBasePlayer *apPlayer, CBaseItem *apCurWeapon) const override {return false;} // TODO: remove apCurWeapon?
	
	// Methods to verify the single/multiplayer status of the game
	virtual bool IsMultiplayer() const override {return false;}
	virtual bool IsDeathmatch() const override {return false;}
	virtual bool IsCoOp() const override {return false;}
	
	// Client connection/disconnection
	virtual bool OnClientConnected(int anClientID, const char *asName, const char *asAddr, char asRejectReason[128]) override {return true;}
	virtual void OnClientDisconnected(int anClientID) override {}
	
	// The client game module is ready for updating
	virtual void InitPlayerHUD(CBasePlayer *apPlayer) override {}
	
	// Player damage rules
	virtual float GetPlayerFallDamage(CBasePlayer *apPlayer) const override;
	
	// Player (re)spawn control
	virtual void OnPlayerSpawn(CBasePlayer *apPlayer) override {}
	virtual void OnPlayerThink(CBasePlayer *apPlayer) override {}
	
	virtual bool PlayerCanRespawn(CBasePlayer *apPlayer) const override {return true;}
	
	virtual float GetPlayerSpawnTime(CBasePlayer *apPlayer) const override;
	
	virtual bool IsAutoTargetCrosshairAllowed() const override;
	
	// Player kills/scoring
	
	/// How many points awarded to anyone that kills this player?
	virtual int GetPointsForKill(CBasePlayer *apAttacker, CBasePlayer *apKilled) const override {return 1;} // TODO: victim, killer?
	
	/// Someone/something killed this player
	virtual void OnPlayerKilled(CBasePlayer *apVictim, CBaseEntity *apKiller, CBaseEntity *apInflictor) override {}
	
	virtual void DeathNotice(CBasePlayer *apVictim, CBaseEntity *apKiller, CBaseEntity *apInflictor) override {}
	
	// Weapon retrieval
	
	/// Player has grabbed a weapon that was sitting in the world
	virtual void OnPlayerGotWeapon(CBasePlayer *apPlayer, CBaseItem *apWeapon) override {}
	
	// Weapon (re)spawn control
	
	/// Any conditions inhibiting the respawning of this weapon?
	virtual bool ShouldRespawnWeapon(CBaseItem *apWeapon) const override {return false;}
	
	/// What is the time in the future at which this weapon can respawn?
	virtual float GetWeaponRespawnTime(CBaseItem *apWeapon) const override {return -1;}
	
	/// Returns 0 if the weapon can respawn now, otherwise it returns the time at which it can try to spawn again
	virtual float TryRespawnWeapon(CBaseItem *apWeapon) override {return 0;}
	
	/// Where should this weapon be respawned? (Some game variations may choose to randomize spawn locations)
	virtual idVec3 GetWeaponRespawnSpot(CBaseItem *apWeapon) const override;
	
	// Item retrieval
	virtual bool CanPlayerHaveItem(CBasePlayer *apPlayer, CItemPickup *apItem) const override {return true;} // TODO: CanPlayerPickupItem?
	virtual void OnPlayerGotItem(CBasePlayer *apPlayer, CItemPickup *apItem) override {}
	
	// Item (re)spawn control
	virtual bool ShouldRespawnItem(CItemPickup *apItem) const override {return false;}
	
	/// At what time in the future should this item respawn?
	virtual float GetItemRespawnTime(CItemPickup *apItem) const override {return -1;}
	
	/// Where should this item be respawned? (Some game variations may choose to randomize spawn locations)
	virtual idVec3 GetItemRespawnSpot(CItemPickup *apItem) const override;
	
	// Ammo retrieval
	virtual void OnPlayerGotAmmo(CBasePlayer *apPlayer, const char *asName, int anCount) override {}
	
	// Ammo (re)spawn control
	virtual bool ShouldRespawnAmmo(CBaseAmmo *apAmmo) const override {return false;}
	virtual float GetAmmoRespawnTime(CBaseAmmo *apAmmo) const override {return -1;}
	virtual idVec3 GetAmmoRespawnSpot(CBaseAmmo *apAmmo) const override;
	
	// Health charger respawn control
	virtual float GetHealthChargerRechargeTime() const override;
	
	// What happens to a dead player's weapons and ammo
	virtual ItemDropType HandleDeadPlayerWeapons(CBasePlayer *apPlayer) override;
	virtual ItemDropType HandleDeadPlayerAmmo(CBasePlayer *apPlayer) override;
	
	// Monsters
	virtual bool AreMonstersAllowed() const override {return true;}
	
	// Teamplay stuff
	virtual const char *GetTeamID(CBaseEntity *apEntity) const override {return "";} // TODO: make CBaseEntity::GetTeamID?
	virtual int GetPlayerRelationship(CBasePlayer *apPlayer, CBaseEntity *apTarget) const override;
};