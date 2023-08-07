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

#include "HLGameRulesMultiplayer.hpp"

CHLGameRulesMultiplayer::CHLGameRulesMultiplayer()
{
	gVoiceGameMgr.Init(); // TODO: why here?
	
	RefreshSkillData();
	
	if(gpEngine->pfnIsDedicatedServer())
	{
	}
	else
	{
		// Load listen server config
	};
};

void CHLGameRulesMultiplayer::Think()
{
};

bool CHLGameRulesMultiplayer::IsAllowedToSpawn(CBaseEntity *apEntity) const
{
};

bool CHLGameRulesMultiplayer::IsFlashlightAllowed() const
{
	return flashlight.GetBool();
};

bool CHLGameRulesMultiplayer::ShouldSwitchWeapon(CBasePlayer *apPlayer, CBaseItem *apWeapon) const
{
};

bool CHLGameRulesMultiplayer::GetNextBestWeapon(CBasePlayer *apPlayer, CBaseItem *apCurWeapon) const
{
};

bool CHLGameRulesMultiplayer::IsCoOp() const
{
	return gpGlobals->coop;
};

bool CHLGameRulesMultiplayer::OnClientConnected(int anClientID, const char *asName, const char *asAddr, char asRejectReason[128])
{
	gVoiceGameMgr.OnClientConnected(anClientID); // TODO: why here?
	return true;
};

void CHLGameRulesMultiplayer::OnClientDisconnected(int anClientID)
{
};

void CHLGameRulesMultiplayer::InitPlayerHUD(CBasePlayer *apPlayer)
{
};

float CHLGameRulesMultiplayer::GetPlayerFallDamage(CBasePlayer *apPlayer) const
{
	int nDmg{falldamage.GetInt()};
	
	switch(nDmg)
	{
	case 1: // Progressive amount of damage
		apPlayer->mfFallVelocity -= PLAYER_MAX_SAFE_FALL_SPEED;
		return apPlayer->mfFallVelocity * DAMAGE_FOR_FALL_SPEED;
		break;
	case 0:
	default: // Fixed 10 hp by default
		return 10;
		break;
	};
};

void CHLGameRulesMultiplayer::OnPlayerSpawn(CBasePlayer *apPlayer)
{
	bool bAddDefault{true};
	CBaseEntity *pWeaponEntity{nullptr};
	
	apPlayer->self->weapons |= (1 << WEAPON_SUIT); // TODO: ->GiveSuit()/EquipSuit()?
	
	while(pWeaponEntity = mpGame->GetWorld()->FindEntityByClassName(pWeaponEntity, "game_player_equip"))
	{
		pWeaponEntity->Touch(apPlayer);
		bAddDefault = false;
	};
	
	if(bAddDefault)
	{
		apPlayer->GiveNamedItem("weapon_crowbar");
		apPlayer->GiveNamedItem("weapon_9mmhandgun");
		apPlayer->GiveAmmo(68, "9mm", _9MM_MAX_CARRY); // 4 full clips
	};
};

void CHLGameRulesMultiplayer::OnPlayerThink(CBasePlayer *apPlayer)
{
};

float CHLGameRulesMultiplayer::GetPlayerSpawnTime(CBasePlayer *apPlayer) const
{
	return gpGlobals->time; // Right now!
};

bool CHLGameRulesMultiplayer::IsAutoTargetCrosshairAllowed() const
{
	return aimcrosshair.GetBool();
};

void CHLGameRulesMultiplayer::OnPlayerKilled(CBasePlayer *apVictim, CBaseEntity *apKiller, CBaseEntity *apInflictor)
{
	DeathNotice(apVictim, apKiller, apInflictor);
};

void CHLGameRulesMultiplayer::DeathNotice(CBasePlayer *apVictim, CBaseEntity *apKiller, CBaseEntity *apInflictor)
{
};

bool CHLGameRulesMultiplayer::ShouldRespawnWeapon(CBaseItem *apWeapon) const
{
};

float CHLGameRulesMultiplayer::GetWeaponRespawnTime(CBaseItem *apWeapon) const
{
};

float CHLGameRulesMultiplayer::TryRespawnWeapon(CBaseItem *apWeapon)
{
};

idVec3 CHLGameRulesMultiplayer::GetWeaponRespawnSpot(CBaseItem *apWeapon) const
{
	return apWeapon->GetOrigin();
};

bool CHLGameRulesMultiplayer::ShouldRespawnItem(CItemPickup *apItem) const
{
};

float CHLGameRulesMultiplayer::GetItemRespawnTime(CItemPickup *apItem) const
{
	return gpGlobals->time + ITEM_RESPAWN_TIME;
};

idVec3 CHLGameRulesMultiplayer::GetItemRespawnSpot(CItemPickup *apItem) const
{
	return apItem->GetOrigin();
};

int CHLGameRulesMultiplayer::ShouldRespawnAmmo(CBaseAmmo *apAmmo) const
{
};

float CHLGameRulesMultiplayer::GetAmmoRespawnTime(CBaseAmmo *apAmmo) const
{
	return gpGlobals->time + AMMO_RESPAWN_TIME;
};

idVec3 CHLGameRulesMultiplayer::GetAmmoRespawnSpot(CBaseAmmo *apAmmo) const
{
	return apAmmo->GetOrigin();
};

float CHLGameRulesMultiplayer::GetHealthChargerRechargeTime() const
{
	return 60;
};

float CHLGameRulesMultiplayer::GetHEVChargerRechargeTime() const
{
	return 30;
};

ItemDropType CHLGameRulesMultiplayer::HandleDeadPlayerWeapons(CBasePlayer *apPlayer)
{
	return ItemDropType::Active;
};

ItemDropType CHLGameRulesMultiplayer::HandleDeadPlayerAmmo(CBasePlayer *apPlayer)
{
	return ItemDropType::Active;
};

bool CHLGameRulesMultiplayer::AreMonstersAllowed() const
{
	return allowmonsters.GetBool();
};

int CHLGameRulesMultiplayer::GetPlayerRelationship(CBasePlayer *apPlayer, CBaseEntity *apTarget) const
{
	// Half-Life deathmatch has only enemies
	return GR_NOTTEAMMATE;
};

bool CHLGameRulesMultiplayer::PlayFootstepSounds(CBasePlayer *apPlayer, float afVolume)
{
	if()
		return false;
	
	if()
		return true;
	
	return false;
};

void CHLGameRulesMultiplayer::ChangeLevel()
{
};

void CHLGameRulesMultiplayer::GoToIntermission()
{
	// Intermission has already been triggered, so ignore
	if(gbGameOver)
		return;
	
	gpEngine->pfnMessageBegin(MSG_ALL, SVC_INTERMISSION);
	gpEngine->pfnMessageEnd();
	
	// Bounds check
	int nTime{gpEngine->pfnGetCvarFloat("mp_chattime")};
	if()
		gpEngine->pfnSetCvarString();
	else if()
		gpEngine->pfnSetCvarString();
	
	mfIntermissionEndTime = gpGlobals->time + mp_chattime.GetInt();
	gfIntermissionStartTime = gpGlobals->time;
	
	gbGameOver = true;
	mbEndIntermissionButtonHit = false;
};

void CHLGameRulesMultiplayer::SendMOTDToClient(int anClientID)
{
};