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
/// @brief base game rules implementation

#pragma once

#include "IGameRules.hpp"

// TODO: CGameRulesScripted/CScriptedGameRules?
// TODO: CGameRulesSkeleton/CSkeletonGameRules?
class CBaseGameRules : public IGameRules
{
public:
	CBaseGameRules();
	virtual ~CBaseGameRules() = default;
	
	virtual void Think() override;
	
	virtual bool IsAllowedToSpawn(CBaseEntity *apEntity) const override;
	
	virtual bool IsFlashlightAllowed() const override {return true;}
	
	virtual bool ShouldSwitchWeapon(CBasePlayer *apPlayer, CBaseItem *apWeapon) const override;
	
	virtual bool GetNextBestWeapon(CBasePlayer *apPlayer, CBaseItem *apCurWeapon) const override; // TODO: remove apCurWeapon?
	
	// Methods to verify the single/multiplayer status of the game
	virtual bool IsMultiplayer() const override;
	virtual bool IsDeathmatch() const override;
	virtual bool IsCoOp() const override;
	
	// Client connection/disconnection
	virtual bool ClientConnected(int anClientID, const char *asName, const char *asAddr, char asRejectReason[128]) override;
	virtual void ClientDisconnected(int anClientID) override;
	
	// The client game module is ready for updating
	virtual void InitPlayerHUD(CBasePlayer *apPlayer) override;
	
	// Player damage rules
	virtual float GetPlayerFallDamage(CBasePlayer *apPlayer) const override;
	
	// Player (re)spawn control
	virtual void OnPlayerSpawn(CBasePlayer *apPlayer) override;
	virtual void OnPlayerThink(CBasePlayer *apPlayer) override;
	
	virtual bool PlayerCanRespawn(CBasePlayer *apPlayer) const override;
	
	virtual float GetPlayerSpawnTime(CBasePlayer *apPlayer) const override;
	
	virtual bool IsAutoTargetCrosshairAllowed() const override;
	
	// Player kills/scoring
	virtual int GetPointsForKill(CBasePlayer *apAttacker, CBasePlayer *apKilled) const override; // TODO: victim, killer?
	
	virtual void OnPlayerKilled(CBasePlayer *apVictim, CBaseEntity *apKiller, CBaseEntity *apInflictor) override;
	
	virtual void DeathNotice(CBasePlayer *apVictim, CBaseEntity *apKiller, CBaseEntity *apInflictor) override;
	
	// Weapon retrieval
	virtual void OnPlayerGotWeapon(CBasePlayer *apPlayer, CBaseItem *apWeapon) override;
	
	// Weapon (re)spawn control
	virtual bool ShouldRespawnWeapon(CBaseItem *apWeapon) const override;
	virtual float GetWeaponRespawnTime(CBaseItem *apWeapon) const override;
	
	virtual float TryRespawnWeapon(CBaseItem *apWeapon) override;
	
	virtual idVec3 GetWeaponRespawnSpot(CBaseItem *apWeapon) const override;
	
	// Item retrieval
	virtual bool CanPlayerHaveItem(CBasePlayer *apPlayer, CItemPickup *apItem) const override; // TODO: CanPlayerPickupItem?
	virtual void OnPlayerGotItem(CBasePlayer *apPlayer, CItemPickup *apItem) override;
	
	// Item (re)spawn control
	virtual bool ShouldRespawnItem(CItemPickup *apItem) const override;
	virtual float GetItemRespawnTime(CItemPickup *apItem) const override;
	virtual idVec3 GetItemRespawnSpot(CItemPickup *apItem) const override;
	
	// Ammo retrieval
	virtual void OnPlayerGotAmmo(CBasePlayer *apPlayer, const char *asName, int anCount) override;
	
	// Ammo (re)spawn control
	virtual int ShouldRespawnAmmo(CBaseAmmo *apAmmo) const override;
	virtual float GetAmmoRespawnTime(CBaseAmmo *apAmmo) const override;
	virtual idVec3 GetAmmoRespawnSpot(CBaseAmmo *apAmmo) const override;
	
	// Health charger respawn control
	virtual float GetHealthChargerRechargeTime() const override;
	
	// What happens to a dead player's weapons and ammo
	virtual int HandleDeadPlayerWeapons(CBasePlayer *apPlayer) override;
	virtual int HandleDeadPlayerAmmo(CBasePlayer *apPlayer) override;
	
	// Monsters
	virtual bool AreMonstersAllowed() const override;
	
	// Teamplay stuff
	virtual const char *GetTeamID(CBaseEntity *apEntity) const override {return "";} // TODO: make CBaseEntity::GetTeamID?
	virtual int GetPlayerRelationship(CBasePlayer *apPlayer, CBaseEntity *apTarget) const override;
};