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

#include "BaseWeapon.hpp"

class CWeaponPython : public CBaseWeapon
{
public:
	void Spawn() override;
	
	void PrimaryAttack() override;
	void SecondaryAttack() override;
};

LINK_ENTITY_TO_CLASS(weapon_python, CWeaponPython)
LINK_ENTITY_TO_CLASS(weapon_357, CWeaponPython)

void CWeaponPython::Spawn()
{
	SetClassName("weapon_357"); // TODO: gpEngine->pfnMakeString(classname)
	
	//
	// TODO: Precache();
	//
	gpEngine->pfnPrecacheModel("models/v_357.mdl");
	gpEngine->pfnPrecacheModel("models/p_357.mdl");
	gpEngine->pfnPrecacheModel("models/w_357.mdl");
	//
	
	mnID = WEAPON_PYTHON;
	
	SetModel("models/w_357.mdl");
};

void CWeaponPython::PrimaryAttack()
{
	if(mnClip <= 0)
		return;
	
	--mnClip;
	
	mpOwner->SendAnim(PLAYER_ATTACK1);
	
	mpOwner->FireBullets(1, mpOwner->GetGunPosition());
	
	mfNextPrimaryAttack = 0.75;
};

void CWeaponPython::SecondaryAttack()
{
	mfNextSecondaryAttack = 0.5;
};