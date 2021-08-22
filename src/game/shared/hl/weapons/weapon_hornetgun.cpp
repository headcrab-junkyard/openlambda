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

class CWeaponHornetGun : public CBaseWeapon
{
public:
	void Spawn() override;
	
	void PrimaryAttack() override;
	void SecondaryAttack() override;
private:
	void Fire();
};

LINK_ENTITY_TO_CLASS(weapon_hornetgun, CWeaponHornetGun);

void CWeaponHornetGun::Spawn()
{
	SetClassName("weapon_9mmhandgun"); // TODO: gpEngine->pfnMakeString(classname)
	
	//
	// TODO: Precache();
	//
	gpEngine->pfnPrecacheModel("models/v_9mmhandgun.mdl");
	gpEngine->pfnPrecacheModel("models/p_9mmhandgun.mdl");
	gpEngine->pfnPrecacheModel("models/w_9mmhandgun.mdl");
	//
	
	mnID = WEAPON_GLOCK;
	
	SetModel("models/w_9mmhandgun.mdl");
};

void CWeaponHornetGun::PrimaryAttack()
{
	Fire();
};

void CWeaponHornetGun::SecondaryAttack()
{
	Fire();
};

void CWeaponHornetGun::Fire()
{
	if(mnClip <= 0)
	{
		return;
	};
	
	--mnClip;
	
	mpOwner->SetAnim(PLAYER_ATTACK1);
};