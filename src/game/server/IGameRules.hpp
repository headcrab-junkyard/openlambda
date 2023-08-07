/*
 * This file is part of OpenLambda Project
 * Copyright (C) 2019-2021, 2023 BlackPhrase
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
/// @brief game rules interface

#pragma once

class CBaseEntity;
class CBasePlayer;
class CBaseAmmo;
class CBaseItem;
class CItemPickup;

struct IGameRules
{
	virtual ~IGameRules() = default;
	
	//virtual void Release() = 0;
	
	virtual void Think() = 0;
	
	// Shared
	
	enum class GameType : int
	{
		SinglePlayer,
		MultiPlayer,
		CoOp,
		TeamPlay
	};
	
	///
	//virtual bool ShouldCollide(const CBaseEntity &lhs, const CBaseEntity &rhs) const = 0;
	
	///
	//virtual float GetAmmoDamage(const char *asName) const = 0;
	//virtual float GetAmmoDamage(CBaseAmmo *apAmmo) const = 0;
	
	///
	//virtual float GetAmmoRespawnTime(CBaseAmmo *apAmmo) const = 0;
	
	/// @return The single/multiplayer status of a game
	virtual GameType GetType() const = 0; // Intead of IsSinglePlayer/IsMultiPlayer/IsCoOp and such
	
	// Methods to verify the single/multiplayer status of the game
	virtual bool IsMultiplayer() const = 0;
	
	/// Is this a deathmatch game?
	virtual bool IsDeathmatch() const = 0;
	
	///
	virtual bool IsTeamplay() const = 0;
	
	/// Is this a cooparative game?
	virtual bool IsCoOp() const = 0;
	
	///
	virtual const char *GetGameDescription() const = 0; // TODO: {return "Half-Life";}
	
	// Server
	
	///
	//virtual void EndGameFrame() = 0;
	
	///
	//virtual void LevelShutdown() = 0;
	
	/// Can this entity spawn (e.g. monsters don't spawn in deathmatch)
	virtual bool IsAllowedToSpawn(CBaseEntity *apEntity) const = 0;
	//virtual bool CanSpawn(CBaseEntity *apEntity) const = 0;
	
	/// Are players allowed to use their flashlight?
	virtual bool IsFlashlightAllowed() const = 0;
	
	/// Should the player switch to this weapon?
	virtual bool ShouldSwitchWeapon(CBasePlayer *apPlayer, CBaseItem *apWeapon) const = 0;
	
	virtual bool GetNextBestWeapon(CBasePlayer *apPlayer, CBaseItem *apCurWeapon) const = 0; // TODO: remove apCurWeapon?
	
	// Client connection/disconnection
	
	/// A client just connected to the server (and hasn't spawned yet)
	virtual bool OnClientConnected(int anClientID, const char *asName, const char *asAddr, char asRejectReason[128]) = 0;
	//virtual bool HandleClientConnect(CBaseEntity *apEntity, const char *asName, const char *asAdr, char sRejectReason[128]) = 0;
	
	/// A client just disconnected from the server
	virtual void OnClientDisconnected(int anClientID) = 0;
	//virtual void HandleClientDisconnect(CBaseEntity *apEntity) = 0;
	
	// The client game module is ready for updating
	virtual void InitPlayerHUD(CBasePlayer *apPlayer) = 0;
	
	/// The client needs to be informed about the current game mode
	virtual void UpdateGameMode(CBasePlayer *apPlayer) = 0; // TODO: {}
	
	// Player damage rules
	
	/// This player just hit the ground after a fall. How much damage should he take?
	virtual float GetPlayerFallDamage(CBasePlayer *apPlayer) const = 0;
	
	/// Can this player take damage from the specified attacker?
	virtual bool CanPlayerTakeDamage(CBasePlayer *apPlayer, CBaseEntity *apAttacker) const = 0; // TODO: {return true;}
	
	// Player (re)spawn control
	
	/// Called by CBasePlayer::Spawn just before releasing player into the game
	virtual void OnPlayerSpawn(CBasePlayer *apPlayer) = 0;
	//virtual void OnPlayerSpawn(CBasePlayer *apPlayer) = 0;
	
	///
	virtual void OnPlayerThink(CBasePlayer *apPlayer) = 0;
	//virtual void OnPlayerThink(CBasePlayer *apPlayer){}
	
	///
	virtual bool PlayerCanRespawn(CBasePlayer *apPlayer) const = 0;
	
	///
	virtual float GetPlayerSpawnTime(CBasePlayer *apPlayer) const = 0;
	
	///
	virtual bool IsAutoTargetCrosshairAllowed() const = 0;
	
	///
	//virtual bool CanRespawn(CBaseEntity *apEntity) = 0; // TODO: CBasePlayer?
	
	///
	//virtual void OnPlayerDeath(CBasePlayer *apPlayer) = 0;
	
	///
	//virtual void OnPlayerKilled(CBasePlayer *apPlayer, const CBaseEntity &aAttacker, const CBaseEntity &aLastInflictor){}
	
	///
	//virtual bool PlayerCanTakeDamage(const CBasePlayer &aPlayer, const CBaseEntity &aAttacker) = 0;
	
	///
	//virtual int PlayerTakeDamage(const CBasePlayer &aPlayer, const CBaseEntity &aInflictor, const CBaseEntity &aAttacker, float afDamage, int anDmgBitSum) = 0;
	
	// TODO
	/*
	Difficulty levels
	Client damage rules
	Client kills and scoring
	Weapon Damage
	Weapon/item/ammo retrieval
	Weapon/item spawn and respawn control
	AI Definitions
	Healthcharger respawn control
	What happens to a dead player's weapons and ammo
	Teamplay rules
	Footstep sounds
	NPCs
	TraceLines
	Chat team/location prefixes
	Achievements
	*/
	
	// Player kills/scoring
	virtual int GetPointsForKill(CBasePlayer *apAttacker, CBasePlayer *apKilled) const = 0; // TODO: victim, killer?
	
	virtual void OnPlayerKilled(CBasePlayer *apVictim, CBaseEntity *apKiller, CBaseEntity *apInflictor) = 0;
	
	virtual void DeathNotice(CBasePlayer *apVictim, CBaseEntity *apKiller, CBaseEntity *apInflictor) = 0;
	
	// Weapon retrieval
	virtual void OnPlayerGotWeapon(CBasePlayer *apPlayer, CBaseItem *apWeapon) = 0;
	
	// Weapon (re)spawn control
	virtual bool ShouldRespawnWeapon(CBaseItem *apWeapon) const = 0;
	virtual float GetWeaponRespawnTime(CBaseItem *apWeapon) const = 0;
	
	virtual float TryRespawnWeapon(CBaseItem *apWeapon) = 0;
	
	virtual idVec3 GetWeaponRespawnSpot(CBaseItem *apWeapon) const = 0;
	
	// Item retrieval
	virtual bool CanPlayerHaveItem(CBasePlayer *apPlayer, CItemPickup *apItem) const = 0; // TODO: CanPlayerPickupItem?
	virtual void OnPlayerGotItem(CBasePlayer *apPlayer, CItemPickup *apItem) = 0;
	
	// Item (re)spawn control
	virtual bool ShouldRespawnItem(CItemPickup *apItem) const = 0;
	virtual float GetItemRespawnTime(CItemPickup *apItem) const = 0;
	virtual idVec3 GetItemRespawnSpot(CItemPickup *apItem) const = 0;
	
	// Ammo retrieval
	virtual void OnPlayerGotAmmo(CBasePlayer *apPlayer, const char *asName, int anCount) = 0;
	
	// Ammo (re)spawn control
	virtual int ShouldRespawnAmmo(CBaseAmmo *apAmmo) const = 0;
	virtual float GetAmmoRespawnTime(CBaseAmmo *apAmmo) const = 0;
	virtual idVec3 GetAmmoRespawnSpot(CBaseAmmo *apAmmo) const = 0;
	
	// Health charger respawn control
	virtual float GetHealthChargerRechargeTime() const = 0;
	
	// What happens to a dead player's weapons and ammo
	enum class ItemDropType : int
	{
		None = 0,
		Active,
		All
	};
	
	// What happens to a dead player's weapons and ammo
	virtual ItemDropType HandleDeadPlayerWeapons(CBasePlayer *apPlayer) = 0;
	virtual ItemDropType HandleDeadPlayerAmmo(CBasePlayer *apPlayer) = 0;
	
	// Monsters
	virtual bool AreMonstersAllowed() const = 0;
	
	// Teamplay stuff
	virtual const char *GetTeamID(CBaseEntity *apEntity) const = 0; // TODO: make CBaseEntity::GetTeamID?
	virtual int GetPlayerRelationship(CBasePlayer *apPlayer, CBaseEntity *apTarget) const = 0;
};

extern /*DLL_GLOBAL*/ IGameRules *gpGameRules;