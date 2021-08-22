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

class CWeaponRPG : public CBaseWeapon
{
public:
	void Spawn() override;
	
	void PrimaryAttack() override;
	void SecondaryAttack() override;
private:
	void Fire();
};

LINK_ENTITY_TO_CLASS(weapon_rpg, CWeaponRPG);

void CWeaponRPG::Spawn()
{
	SetClassName("weapon_rpg"); // TODO: gpEngine->pfnMakeString(classname)
	
	//
	// TODO: Precache();
	//
	gpEngine->pfnPrecacheModel("models/v_rpg.mdl");
	gpEngine->pfnPrecacheModel("models/p_rpg.mdl");
	gpEngine->pfnPrecacheModel("models/w_rpg.mdl");
	//
	
	mnID = WEAPON_RPG;
	
	SetModel("models/w_rpg.mdl");
};

void CWeaponRPG::PrimaryAttack()
{
	Fire();
};

void CWeaponRPG::SecondaryAttack()
{
	Fire();
};

void CWeaponRPG::Fire()
{
	if(mnClip <= 0)
	{
		return;
	};
	
	--mnClip;
	
	mpOwner->SetAnim(PLAYER_ATTACK1);
};