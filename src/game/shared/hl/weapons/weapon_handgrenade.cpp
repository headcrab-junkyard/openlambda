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

#include "BaseWeapon.hpp"

class CWeaponHandGrenade : public CBaseWeapon
{
public:
	void Spawn() override;
	
	void PrimaryAttack() override;
};

LINK_ENTITY_TO_CLASS(weapon_handgrenade, CWeaponHandGrenade);

void CWeaponHandGrenade::Spawn()
{
	SetClassName("weapon_handgrenade"); // TODO: gpEngine->pfnMakeString(classname)
	
	//
	// TODO: Precache();
	//
	gpEngine->pfnPrecacheModel("models/v_grenade.mdl");
	gpEngine->pfnPrecacheModel("models/p_grenade.mdl");
	gpEngine->pfnPrecacheModel("models/w_grenade.mdl");
	//
	
	mnID = WEAPON_HEGRENADE;
	
	SetModel("models/w_grenade.mdl");
};

void CWeaponHandGrenade::PrimaryAttack()
{
	if(mnClip <= 0)
	{
		return;
	};
	
	--mnClip;
	
	mpOwner->SetAnim(PLAYER_ATTACK1);
};