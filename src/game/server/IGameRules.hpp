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

// TODO
/*
	Difficulty levels
	Client kills and scoring
	TraceLines
	Chat team/location prefixes
	Achievements
*/
struct IGameRules
{
	virtual ~IGameRules() = default;
	//virtual void Release() = 0; // BP: this will be useful in case the implementation will be defined inside of another dll
	
///////////////////////////////////////////////////////////////////////////////
//
// SHARED METHODS
//
///////////////////////////////////////////////////////////////////////////////
	
	/// Possible game types
	enum class GameType : int
	{
		SinglePlayer = 0, ///< Just a singleplayer game
		DeathMatch, ///< Multiplayer (PvP) and everyone kills everyone
		CoOp, ///< Multiplayer and players work together (PvE)
		TeamPlay ///< Multiplayer (PvP) and players of one team kills players of another
	};
	
	/// @return The single/multiplayer status of a game
	virtual GameType GetGameType() const = 0; // Instead of IsSinglePlayer/IsDeathmatch/IsCoOp and such
	
	///
	//virtual const char *GetGameTypeName() const = 0;
	
	/// Is this a multiplayer game? (either coop or deathmatch)
	virtual bool IsMultiplayer() const = 0; // TODO: should this even be here? CBaseGame::IsMultiplayer()?
	
	///
	//virtual bool ShouldCollide(const CBaseEntity &lhs, const CBaseEntity &rhs) const = 0;
	
	///
	//virtual float GetAmmoDamage(const char *asName) const = 0;
	//virtual float GetAmmoDamage(CBaseAmmo *apAmmo) const = 0;
	
	/// Switch to the next best weapon
	virtual bool SwitchToNextBestWeapon(CBaseCharacter *apCharacter, CBaseItem *apCurWeapon) = 0;
	
	/// I can't use this weapon anymore, get me the next best one
	virtual CBaseItem *GetNextBestWeapon(CBaseCharacter *apCharacter, CBaseItem *apCurWeapon) const = 0; // TODO: remove apCurWeapon?
	
///////////////////////////////////////////////////////////////////////////////
//
// CLIENT-ONLY METHODS
//
///////////////////////////////////////////////////////////////////////////////
	
	// Nothing for now
	
///////////////////////////////////////////////////////////////////////////////
//
// SERVER-ONLY METHODS
//
///////////////////////////////////////////////////////////////////////////////
	
	/// Called at the end of game frame (i.e. after all game logic has run this frame)
	//virtual void EndGameFrame() = 0;
	
	/// Called when game rules are destroyed
	//virtual void OnLevelShutdown() = 0;
	
	/// Runs every frame, should handle any timer tasks, periodic events, etc
	virtual void Think() = 0;
	
	/// Fill skill data struct with proper values
	virtual void RefreshSkillData(/*bool abForceUpdate*/) = 0;
	
	// Methods to verify the single/multiplayer status of the game
	
	/// Is this a deathmatch game?
	virtual bool IsDeathmatch() const = 0;
	
	/// Is this a deathmatch game being played with team rules?
	virtual bool IsTeamplay() const = 0;
	
	/// Is this a cooparative game?
	virtual bool IsCoOp() const = 0;
	
	/// Returns the name of the game (that gets seen in the server browser)
	virtual const char *GetGameDescription() const = 0;
	
	/// Can this entity spawn (e.g. monsters don't spawn in deathmatch)
	virtual bool IsAllowedToSpawn(CBaseEntity *apEntity) const = 0;
	//virtual bool CanSpawn(CBaseEntity *apEntity) const = 0;
	
	/// Are players allowed to use their flashlight?
	virtual bool IsFlashlightAllowed() const = 0;
	
	/// Should the player switch to this weapon?
	virtual bool ShouldSwitchWeapon(CBasePlayer *apPlayer, CBaseItem *apWeapon) const = 0;
	
///////////////////////////////////////////////////////////////////////////////
// Client connection/disconnection
///////////////////////////////////////////////////////////////////////////////
	
	/// A client just connected to the server (and hasn't spawned yet)
	virtual bool OnClientConnected(int anClientID, const char *asName, const char *asAddr, char asRejectReason[128]) = 0;
	//virtual bool HandleClientConnect(CBaseEntity *apEntity, const char *asName, const char *asAdr, char sRejectReason[128]) = 0;
	
	/// A client just disconnected from the server
	virtual void OnClientDisconnected(int anClientID) = 0;
	//virtual void HandleClientDisconnect(CBaseEntity *apEntity) = 0;
	
///////////////////////////////////////////////////////////////////////////////
	
	// The client game module is ready for updating
	virtual void InitPlayerHUD(CBasePlayer *apPlayer) = 0;
	
	/// The client needs to be informed about the current game mode
	virtual void UpdateGameMode(CBasePlayer *apPlayer) = 0; // TODO: {}
	
///////////////////////////////////////////////////////////////////////////////
// Player damage rules
///////////////////////////////////////////////////////////////////////////////
	
	/// This player just hit the ground after a fall. How much damage should he take?
	virtual float GetPlayerFallDamage(CBasePlayer *apPlayer) const = 0;
	
	/// Can this player take damage from the specified attacker?
	virtual bool CanPlayerTakeDamage(CBasePlayer *apPlayer, CBaseEntity *apAttacker) const = 0; // TODO: {return true;}
	//virtual bool PlayerCanTakeDamage(const CBasePlayer &aPlayer, const CBaseEntity &aAttacker) = 0;
	
///////////////////////////////////////////////////////////////////////////////
// Player (re)spawn control
///////////////////////////////////////////////////////////////////////////////
	
	/// Called by CBasePlayer::Spawn just before releasing player into the game
	virtual void OnPlayerSpawn(CBasePlayer *apPlayer) = 0;
	
	/// Called by CBasePlayer::PreThink every frame, before physics are run and after keys are accepted
	virtual void OnPlayerThink(CBasePlayer *apPlayer) = 0;
	//virtual void OnPlayerThink(CBasePlayer *apPlayer){}
	
	/// Is this player allowed to respawn now?
	virtual bool PlayerCanRespawn(CBasePlayer *apPlayer) const = 0; // TODO: CanPlayerRespawn?
	
	/// Decide when in the future will this player be able to spawn
	virtual float GetPlayerSpawnTime(CBasePlayer *apPlayer) const = 0;
	
	/// Place this player on his spawn spot and face him the proper direction
	virtual CBaseEntity *GetPlayerSpawnSpot(CBasePlayer *apPlayer) const = 0;
	
	///
	//virtual bool IsSpawnPointValid() const = 0;
	
///////////////////////////////////////////////////////////////////////////////
// Client damage rules
///////////////////////////////////////////////////////////////////////////////
	
	///
	virtual bool ShouldAutoAim(CBasePlayer *apPlayer, CBaseEntity *apTarget) const = 0;
	
	///
	virtual bool IsAutoTargetCrosshairAllowed() const = 0;
	
///////////////////////////////////////////////////////////////////////////////
	
	/// Handles the user commands
	/// @return true if the command handled properly, false otherwise
	virtual bool HandleClientCommand(CBasePlayer *apPlayer, const ICmdArgs &aCmd) = 0;
	
	/// The player has changed his user info, handle that here
	virtual void OnClientUserInfoChanged(CBasePlayer *apPlayer, char *asInfoBuffer) = 0; // TODO: OnClientSettingsChanged/OnClientInfoChanged?
	
	///
	//virtual bool CanRespawn(CBaseEntity *apEntity) = 0; // TODO: CBasePlayer?
	
	///
	//virtual int OnPlayerTakeDamage(const CBasePlayer &aPlayer, const CBaseEntity &aInflictor, const CBaseEntity &aAttacker, float afDamage, int anDmgBitSum) = 0;
	
///////////////////////////////////////////////////////////////////////////////
// Player kills/scoring
///////////////////////////////////////////////////////////////////////////////
	
	/// How many points do I award whoever kills this player?
	virtual int GetPointsForKill(CBasePlayer *apKiller, CBasePlayer *apVictim) const = 0; // TODO: victim, killer?
	
	/// Called each time a player dies
	virtual void OnPlayerKilled(CBasePlayer *apPlayer, CBaseEntity *apKiller, CBaseEntity *apInflictor) = 0;
	//virtual void OnPlayerDeath(CBasePlayer *apPlayer) = 0;
	//virtual void OnPlayerKilled(CBasePlayer *apPlayer, const CBaseEntity &aAttacker, const CBaseEntity &aLastInflictor){}
	
	/// Call this from within a game rules class to report an obituary (TODO: is this a proper place?)
	virtual void DeathNotice(CBasePlayer *apVictim, CBaseEntity *apKiller, CBaseEntity *apInflictor) = 0;
	
///////////////////////////////////////////////////////////////////////////////
// Weapon damage
///////////////////////////////////////////////////////////////////////////////
	
	// Nothing for now
	
///////////////////////////////////////////////////////////////////////////////
// Weapon retrieval
///////////////////////////////////////////////////////////////////////////////
	
	/// The player is touching a CBaseItem entity, should I give it to him?
	virtual bool CanPlayerHaveWeapon(CBasePlayer *apPlayer, CBaseItem *apWeapon) const = 0;
	
	/// Called each time a player picks up a weapon from the ground
	virtual void OnPlayerGotWeapon(CBasePlayer *apPlayer, CBaseItem *apWeapon) = 0;
	
///////////////////////////////////////////////////////////////////////////////
// Weapon (re)spawn control
///////////////////////////////////////////////////////////////////////////////
	
	/// Can this weapon respawn now, and if not, when should the next check happen?
	virtual float TryRespawnWeapon(CBaseItem *apWeapon) = 0;
	
	/// Should this weapon respawn?
	virtual bool ShouldRespawnWeapon(CBaseItem *apWeapon) const = 0;
	
	/// When may this weapon respawn?
	virtual float GetWeaponRespawnTime(CBaseItem *apWeapon) const = 0;
	
	/// Where in the world should this weapon respawn?
	virtual idVec3 GetWeaponRespawnSpot(CBaseItem *apWeapon) const = 0;
	
///////////////////////////////////////////////////////////////////////////////
// Item retrieval
///////////////////////////////////////////////////////////////////////////////
	
	/// Is this player allowed to take this item?
	virtual bool CanPlayerHaveItem(CBasePlayer *apPlayer, CItemPickup *apItem) const = 0; // TODO: CanPlayerPickupItem?
	
	/// Call each time a player picks up an item (battery, healthkit, longjump, etc)
	virtual void OnPlayerGotItem(CBasePlayer *apPlayer, CItemPickup *apItem) = 0;
	
///////////////////////////////////////////////////////////////////////////////
// Item (re)spawn control
///////////////////////////////////////////////////////////////////////////////
	
	/// Should this item respawn?
	virtual bool ShouldRespawnItem(CItemPickup *apItem) const = 0;
	
	/// When can theis item respawn?
	virtual float GetItemRespawnTime(CItemPickup *apItem) const = 0;
	
	/// Where in the world should this item respawn?
	virtual idVec3 GetItemRespawnSpot(CItemPickup *apItem) const = 0;
	
///////////////////////////////////////////////////////////////////////////////
// Ammo retrieval
///////////////////////////////////////////////////////////////////////////////
	
	/// Can this player take more of this ammo?
	virtual bool CanPlayerHaveAmmo(CBasePlayer *apPlayer, const char *asAmmoName, int anMaxCarry) const = 0;
	
	/// Called each time a player picks up some ammo in the world
	virtual void OnPlayerGotAmmo(CBasePlayer *apPlayer, const char *asName, int anCount) = 0;
	
///////////////////////////////////////////////////////////////////////////////
// Ammo (re)spawn control
///////////////////////////////////////////////////////////////////////////////
	
	// TODO: removed from src?
	
	/// Should this ammo item respawn?
	virtual bool ShouldRespawnAmmo(CBaseAmmo *apAmmo) const = 0;
	
	/// When should this ammo item respawn?
	virtual float GetAmmoRespawnTime(CBaseAmmo *apAmmo) const = 0;
	
	/// Where in the world should this ammo item respawn?
	virtual idVec3 GetAmmoRespawnSpot(CBaseAmmo *apAmmo) const = 0;
	
///////////////////////////////////////////////////////////////////////////////
// AI definitions
///////////////////////////////////////////////////////////////////////////////
	
	// Nothing for now
	
///////////////////////////////////////////////////////////////////////////////	
// Health charger respawn control
///////////////////////////////////////////////////////////////////////////////
	
	/// How long until a depleted health charger recharges itself?
	virtual float GetHealthChargerRechargeTime() const = 0;
	
	/// How long until a depleted armor charger recharges itself?
	virtual float GetHEVChargerRechargeTime() const = 0; // TODO: GetArmorChargerRechargeTime? Attach a specific script to entity to make a check there? + What if I want different values?
	
///////////////////////////////////////////////////////////////////////////////
// What happens to a dead player's weapons and ammo
///////////////////////////////////////////////////////////////////////////////
	
	// What happens to a dead player's weapons and ammo
	enum class ItemDropType : int
	{
		None = 0,
		Active,
		All
	};
	
	/// What should I do with a player's weapons when he's killed?
	virtual ItemDropType HandleDeadPlayerWeapons(CBasePlayer *apPlayer) = 0;
	
	/// Do I drop ammo (and how much) when the player dies?
	virtual ItemDropType HandleDeadPlayerAmmo(CBasePlayer *apPlayer) = 0;
	
///////////////////////////////////////////////////////////////////////////////
// Monsters/NPCs
///////////////////////////////////////////////////////////////////////////////
	
	/// Are monsters allowed?
	virtual bool AreMonstersAllowed() const = 0;
	
	// Are NPCs allowed?
	//virtual bool AreNPCsAllowed() const = 0;
	
///////////////////////////////////////////////////////////////////////////////
// Teamplay stuff
///////////////////////////////////////////////////////////////////////////////
	
	/// What team is this entity on?
	virtual const char *GetTeamID(CBaseEntity *apEntity) const = 0; // TODO: make CBaseEntity::GetTeamID?
	
	/// Player relationship return codes
	enum class RelationshipType : int
	{
		NotTeamMate = 0,
		TeamMate,
		Enemy,
		Ally,
		Neutral
	};
	
	/// What is the player's relationship with this entity?
	virtual RelationshipType GetPlayerRelationship(CBasePlayer *apPlayer, CBaseEntity *apTarget) const = 0;
	
	virtual int GetTeamIndex(const char *asTeamName) const = 0;
	virtual const char *GetIndexedTeamName(int anTeamID) const = 0;
	virtual bool IsValidTeam(const char *asTeamName) const = 0;
	
	virtual void ChangePlayerTeam(CBasePlayer *apPlayer, const char *asTeamName, bool abKill, bool abGib) = 0;
	virtual const char *SetDefaultPlayerTeam(CBasePlayer *apPlayer) = 0;
	
///////////////////////////////////////////////////////////////////////////////
// Sounds
///////////////////////////////////////////////////////////////////////////////
	
	virtual bool PlayTextureSounds() = 0;
	virtual bool PlayFootstepSounds(CBasePlayer *apPlayer, float afVolume) = 0;
	
///////////////////////////////////////////////////////////////////////////////
	
	// Immediately end a multiplayer game
	virtual void EndMultiplayerGame() = 0;
};

extern /*DLL_GLOBAL*/ IGameRules *gpGameRules;