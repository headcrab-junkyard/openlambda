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

class CWeaponGauss : public CBaseWeapon
{
public:
	void Spawn() override;
	
	void PrimaryAttack() override;
	void SecondaryAttack() override;
private:
	void Fire();
};

LINK_ENTITY_TO_CLASS(weapon_gauss, CWeaponGauss);

void CWeaponGauss::Spawn()
{
	SetClassName("weapon_gauss"); // TODO: gpEngine->pfnMakeString(classname)
	
	//
	// TODO: Precache();
	//
	gpEngine->pfnPrecacheModel("models/v_gauss.mdl");
	gpEngine->pfnPrecacheModel("models/p_gauss.mdl");
	gpEngine->pfnPrecacheModel("models/w_gauss.mdl");
	//
	
	mnID = WEAPON_GAUSS;
	
	SetModel("models/w_gauss.mdl");
};

void CWeaponGauss::PrimaryAttack()
{
	Fire();
};

void CWeaponGauss::SecondaryAttack()
{
	Fire();
};

void CWeaponGauss::Fire()
{
	if(mnClip <= 0)
	{
		return;
	};
	
	--mnClip;
	
	mpOwner->SetAnim(PLAYER_ATTACK1);
};