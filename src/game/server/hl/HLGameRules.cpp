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

#include "HLGameRules.hpp"

CHLGameRules::CHLGameRules()
{
	RefreshSkillData();
};

bool CHLGameRules::ShouldSwitchWeapon(CBasePlayer *apPlayer, CBaseItem *apWeapon) const
{
	if(!apPlayer->mpActiveItem)
	{
		// Player doesn't have an active item!
		return true;
	};
	
	if(!apPlayer->mpActiveItem->CanHolster())
		return false;
	
	return true;
};

float CHLGameRules::GetPlayerFallDamage(CBasePlayer *apPlayer) const
{
	// Subtract off the speed at which a player is allowed to fall without being hurt,
	// so damage will be based on speed beyond that, not the entire fall
	apPlayer->mfFallVelocity -= PLAYER_MAX_SAFE_FALL_SPEED;
	return apPlayer->mfFallVelocity * DAMAGE_FOR_FALL_SPEED;
};

float CHLGameRules::GetPlayerSpawnTime(CBasePlayer *apPlayer) const
{
	return gpGlobals->time; // Right now!
};

bool CHLGameRules::IsAutoTargetCrosshairAllowed() const
{
	return gnSkillLevel == SKILL_EASY;
};

idVec3 CHLGameRules::GetWeaponRespawnSpot(CBaseItem *apWeapon) const
{
	return apWeapon->GetOrigin();
};

idVec3 CHLGameRules::GetItemRespawnSpot(CItemPickup *apItem) const
{
	return apItem->GetOrigin();
};

idVec3 CHLGameRules::GetAmmoRespawnSpot(CBaseAmmo *apAmmo) const
{
	return apAmmo->GetOrigin();
};

float CHLGameRules::GetHealthChargerRechargeTime() const
{
	return 0; // Don't recharge
};

ItemDropType CHLGameRules::HandleDeadPlayerWeapons(CBasePlayer *apPlayer)
{
	return ItemDropType::None;
};

ItemDropType CHLGameRules::HandleDeadPlayerAmmo(CBasePlayer *apPlayer)
{
	return ItemDropType::None;
};

int CHLGameRules::GetPlayerRelationship(CBasePlayer *apPlayer, CBaseEntity *apTarget) const
{
	// Why would a single player in Half-Life need this?
	return GR_NOTTEAMMATE;
};