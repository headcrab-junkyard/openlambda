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

class CWeaponSMG : public CBaseWeapon
{
public:
	void Spawn() override;
	
	void PrimaryAttack() override;
	void SecondaryAttack() override;
	
	void Reload() override;
};

LINK_ENTITY_TO_CLASS(weapon_9mmAR, CWeaponSMG)
LINK_ENTITY_TO_CLASS(weapon_mp5, CWeaponSMG)

void CWeaponSMG::Spawn()
{
	SetClassName("weapon_9mmAR"); // TODO: gpEngine->pfnMakeString(classname)
	
	//
	// TODO: Precache();
	//
	gpEngine->pfnPrecacheModel("models/v_9mmAR.mdl");
	gpEngine->pfnPrecacheModel("models/p_9mmAR.mdl");
	gpEngine->pfnPrecacheModel("models/w_9mmAR.mdl");
	
	gpEngine->pfnPrecacheModel("models/grenade.mdl");
	//
	
	mnID = WEAPON_SMG; // TODO: WEAPON_MP5
	
	SetModel("models/w_9mmAR.mdl");
};

void CWeaponSMG::PrimaryAttack()
{
	if(mnClip <= 0)
	{
		PlayEmptySound();
		mfNextPrimaryAttack = 0.15;
		return;
	};
	
	--mnClip;
	
	mpOwner->SetAnim(PLAYER_ATTACK1);
};

void CWeaponSMG::SecondaryAttack()
{
	mpOwner->SetAnim(PLAYER_ATTACK1);
};

void CWeaponSMG::Reload()
{
	if(mpOwner->GetAmmoCount("9mm") <= 0)
		return;
};