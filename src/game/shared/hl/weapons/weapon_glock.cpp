/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2019-2021 BlackPhrase
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

//#include "exports.h"
//#include "engine.h"
#include "BaseWeapon.hpp"

class CWeaponGlock : public CBaseWeapon
{
public:
	void Spawn() override;
	//void Precache();
	
	bool Deploy() override;
	
	void PrimaryAttack() override;
	void SecondaryAttack() override;
	
	void Reload() override;
private:
	void Fire(float afSpread, float afCycleTime, bool abUseAutoAim);
private:
	int mnShell{0};
	
	unsigned short mnFireGlock1;
	unsigned short mnFireGlock2;
};

LINK_ENTITY_TO_CLASS(weapon_glock, CWeaponGlock);
LINK_ENTITY_TO_CLASS(weapon_9mmhandgun, CWeaponGlock);

void CWeaponGlock::Spawn()
{
	SetClassName("weapon_9mmhandgun"); // TODO: gpEngine->pfnMakeString(classname)
	
	//
	// TODO: Precache();
	//
	gpEngine->pfnPrecacheModel("models/v_9mmhandgun.mdl");
	gpEngine->pfnPrecacheModel("models/p_9mmhandgun.mdl");
	gpEngine->pfnPrecacheModel("models/w_9mmhandgun.mdl");
	
	mnShell = gpEngine->pfnPrecacheModel("models/shell.mdl");
	
	gpEngine->pfnPrecacheSound("items/9mmclip1.wav");
	gpEngine->pfnPrecacheSound("items/9mmclip2.wav");
	
	gpEngine->pfnPrecacheSound("weapons/pl_gun1.wav");
	gpEngine->pfnPrecacheSound("weapons/pl_gun2.wav");
	gpEngine->pfnPrecacheSound("weapons/pl_gun3.wav");
	
	mnFireGlock1 = gpEngine->pfnPrecacheEvent(1, "events/glock1.sc");
	mnFireGlock2 = gpEngine->pfnPrecacheEvent(1, "events/glock2.sc");
	//
	
	mnID = WEAPON_GLOCK;
	
	SetModel("models/w_9mmhandgun.mdl");
};

bool CWeaponGlock::Deploy()
{
	return DefaultDeploy();
};

void CWeaponGlock::PrimaryAttack()
{
	Fire(0.01, 0.3, true);
};

void CWeaponGlock::SecondaryAttack()
{
	Fire(0.1, 0.2, false);
};

void CWeaponGlock::Reload()
{
	bool bResult{false};
	
	if(mnClip == 0)
		bResult = DefaultReload(17, GLOCK_RELOAD, 1.5);
	else
		bResult = DefaultReload(18, GLOCK_RELOAD_NOT_EMPTY, 1.5);
	
	if(bResult)
		mfTimeWeaponIdle = gpGlobals->time + RANDOM_FLOAT(10, 15);
};

void CWeaponGlock::Fire(float afSpread, float afCycleTime, bool abUseAutoAim)
{
	if(mnClip <= 0)
	{
		return;
	};
	
	--mnClip;
	
	mpOwner->SetAnim(PLAYER_ATTACK1);
};