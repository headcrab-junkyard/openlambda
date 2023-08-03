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
/// @brief rules for the basic Half-Life multiplayer competition

#pragma once

#include <IGameRules.hpp>

// CHalfLifeMultiplay
// TODO: CMultiplayerGameRules?
class CHLGameRulesMultiplayer : public IGameRules
{
public:
	CHLGameRulesMultiplayer();
	virtual ~CHLGameRulesMultiplayer() = default;
	
	virtual void Think() override;
	
	virtual void RefreshSkillData() override;
	
	virtual bool IsAllowedToSpawn(CBaseEntity *apEntity) const override;
	
	virtual bool IsFlashlightAllowed() const override;
	
	virtual bool ShouldSwitchWeapon(CBasePlayer *apPlayer, CBaseItem *apWeapon) const override;
	
	virtual bool GetNextBestWeapon(CBasePlayer *apPlayer, CBaseItem *apCurWeapon) const override; // TODO: remove apCurWeapon?
	
	// Methods to verify the single/multiplayer status of the game
	virtual bool IsMultiplayer() const override;
	virtual bool IsDeathmatch() const override;
	virtual bool IsCoOp() const override;
	
	// Client connection/disconnection
	// If this returns false, the connection is rejected and the user is provided with the reason specified in asRejectReason
	// Only the client's name and remove address are provided here for verification
	virtual bool ClientConnected(int anClientID, const char *asName, const char *asAddr, char asRejectReason[128]) override;
	
	virtual void ClientDisconnected(int anClientID) override;
	
	// The client game module is ready for updating
	virtual void InitPlayerHUD(CBasePlayer *apPlayer) override;
	
	// The player needs to be informed about the current game mode
	virtual void UpdateGameMode(CBasePlayer *apPlayer) override; // TODO: UpdateGameModeForPlayer?
	
	// Player damage rules
	virtual float GetPlayerFallDamage(CBasePlayer *apPlayer) const override;
	virtual bool IsPlayerAllowedToDamage(CBasePlayer *apPlayer, CBaseEntity *apOther) const override; // FPlayerCanTakeDamage(CBasePlayer *apPlayer, CBaseEntity *apAttacker); // was From?
	
	// Player (re)spawn control
	virtual void OnPlayerSpawn(CBasePlayer *apPlayer) override;
	virtual void OnPlayerThink(CBasePlayer *apPlayer) override;
	
	virtual bool PlayerCanRespawn(CBasePlayer *apPlayer) const override;
	
	virtual float GetPlayerSpawnTime(CBasePlayer *apPlayer) const override;
	virtual CBaseEntity *GetPlayerSpawnSpot(CBasePlayer *apPlayer) const override; // TODO: return vector3?
	
	virtual bool IsAutoTargetCrosshairAllowed() const override;
	
	virtual bool HandleClientCommand(CBasePlayer *apPlayer, const ICmdArgs &aCmd) override;
	
	// Player kills/scoring
	virtual int GetPointsForKill(CBasePlayer *apAttacker, CBasePlayer *apKilled) const override; // TODO: victim, killer?
	
	virtual void OnPlayerKilled(CBasePlayer *apVictim, CBaseEntity *apKiller, CBaseEntity *apInflictor) override;
	
	virtual void DeathNotice(CBasePlayer *apVictim, CBaseEntity *apKiller, CBaseEntity *apInflictor) override;
	
	// Weapon retrieval
	virtual void OnPlayerGotWeapon(CBasePlayer *apPlayer, CBaseItem *apWeapon) override;
	
	// The player is touching an item, do I give it to him?
	virtual bool CanPlayerHaveWeapon(CBasePlayer *apPlayer, CBaseItem *apWeapon) const override; // TODO: was CanHavePlayerItem
	
	// Weapon (re)spawn control
	virtual bool ShouldRespawnWeapon(CBaseItem *apWeapon) const override; // TODO: was returning enum but seems useless
	virtual float GetWeaponRespawnTime(CBaseItem *apWeapon) const override;
	
	virtual float TryRespawnWeapon(CBaseItem *apWeapon) override;
	
	virtual idVec3 GetWeaponRespawnSpot(CBaseItem *apWeapon) const override;
	
	// Item retrieval
	virtual bool CanPlayerHaveItem(CBasePlayer *apPlayer, CItemPickup *apItem) const override; // TODO: CanPlayerPickupItem?
	virtual void OnPlayerGotItem(CBasePlayer *apPlayer, CItemPickup *apItem) override;
	
	// Item (re)spawn control
	virtual bool ShouldRespawnItem(CItemPickup *apItem) const override; // TODO: was returning enum but seems useless
	virtual float GetItemRespawnTime(CItemPickup *apItem) const override;
	virtual idVec3 GetItemRespawnSpot(CItemPickup *apItem) const override;
	
	// Ammo retrieval
	virtual void OnPlayerGotAmmo(CBasePlayer *apPlayer, const char *asName, int anCount) override;
	
	// Ammo (re)spawn control
	virtual bool ShouldRespawnAmmo(CBaseAmmo *apAmmo) const override; // TODO: was returning enum but seems useless
	virtual float GetAmmoRespawnTime(CBaseAmmo *apAmmo) const override;
	virtual idVec3 GetAmmoRespawnSpot(CBaseAmmo *apAmmo) const override;
	
	// Health charger respawn control
	virtual float GetHealthChargerRechargeTime() const override;
	virtual float GetHEVChargerRechargeTime() const override;
	
	// What happens to a dead player's weapons and ammo
	enum class ItemDropType : int
	{
		None = 0,
		Active,
		All
	};
	
	virtual ItemDropType HandleDeadPlayerWeapons(CBasePlayer *apPlayer) override;
	virtual ItemDropType HandleDeadPlayerAmmo(CBasePlayer *apPlayer) override;
	
	// Monsters
	virtual bool AreMonstersAllowed() const override;
	
	// Teamplay stuff
	virtual const char *GetTeamID(CBaseEntity *apEntity) const override {return "";} // TODO: make CBaseEntity::GetTeamID?
	virtual int GetPlayerRelationship(CBasePlayer *apPlayer, CBaseEntity *apTarget) const override;
	
	virtual bool PlayTextureSounds() override {return false;}
	virtual bool PlayFootstepSounds(CBasePlayer *apPlayer, float afVolume); // TODO: why here?
	
	// Immediately end a multiplayer game
	virtual void EndMultiplayerGame() override {GoToIntermission();}
protected:
	virtual void ChangeLevel();
	virtual void GoToIntermission();
	
	void SendMOTDToClient(int anClientID);
protected:
	float mfIntermissionEndTime{0.0f};
	
	bool mbEndIntermissionButtonHit{false};
};