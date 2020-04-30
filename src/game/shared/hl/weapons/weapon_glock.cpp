/*
 * This file is part of OGS Engine
 * Copyright (C) 2019-2020 BlackPhrase
 *
 * OGS Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OGS Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OGS Engine. If not, see <http://www.gnu.org/licenses/>.
 */

/// @file

//#include "exports.h"
//#include "engine.h"
#include "BaseWeapon.hpp"

class CWeaponGlock : public CBaseWeapon
{
public:
	void Spawn() override;
	
	void PrimaryAttack() override;
	void SecondaryAttack() override;
private:
	void Fire();
};

LINK_ENTITY_TO_CLASS(weapon_glock, CWeaponGlock)
LINK_ENTITY_TO_CLASS(weapon_9mmhandgun, CWeaponGlock)

void CWeaponGlock::Spawn()
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

void CWeaponGlock::PrimaryAttack()
{
	Fire();
};

void CWeaponGlock::SecondaryAttack()
{
	Fire();
};

void CWeaponGlock::Fire()
{
	if(mnClip <= 0)
	{
		return;
	};
	
	--mnClip;
	
	mpOwner->SetAnim(PLAYER_ATTACK1);
};