/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2019-2020 BlackPhrase
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

#include "BaseWeapon.hpp"

class CWeaponSatchel : public CBaseWeapon
{
public:
	void Spawn() override;
	
	void PrimaryAttack() override;
	void SecondaryAttack() override;
private:
	void Fire();
};

LINK_ENTITY_TO_CLASS(weapon_satchel, CWeaponSatchel);

void CWeaponSatchel::Spawn()
{
	SetClassName("weapon_satchel"); // TODO: gpEngine->pfnMakeString(classname)
	
	//
	// TODO: Precache();
	//
	gpEngine->pfnPrecacheModel("models/v_satchel.mdl");
	gpEngine->pfnPrecacheModel("models/p_satchel.mdl");
	gpEngine->pfnPrecacheModel("models/w_satchel.mdl");
	//
	
	mnID = WEAPON_SATCHEL;
	
	SetModel("models/w_satchel.mdl");
};

void CWeaponSatchel::PrimaryAttack()
{
	Fire();
};

void CWeaponSatchel::SecondaryAttack()
{
	Fire();
};

void CWeaponSatchel::Fire()
{
	if(mnClip <= 0)
	{
		return;
	};
	
	--mnClip;
	
	mpOwner->SetAnim(PLAYER_ATTACK1);
};