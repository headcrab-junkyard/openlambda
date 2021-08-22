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

class CWeaponShotgun : public CBaseWeapon
{
public:
	void Spawn() override;
	
	void PrimaryAttack() override;
	void SecondaryAttack() override;
	
	void Reload() override;
private:
	unsigned short mnSingleFireEvent{0};
	unsigned short mnDoubleFireEvent{0};
};

LINK_ENTITY_TO_CLASS(weapon_shotgun, CWeaponShotgun);

void CWeaponShotgun::Spawn()
{
	SetClassName("weapon_9mmhandgun"); // TODO: gpEngine->pfnMakeString(classname)
	
	//
	// TODO: Precache();
	//
	gpEngine->pfnPrecacheModel("models/v_shotgun.mdl");
	gpEngine->pfnPrecacheModel("models/p_shotgun.mdl");
	gpEngine->pfnPrecacheModel("models/w_shotgun.mdl");
	
	mnShell = gpEngine->pfnPrecacheModel("models/shotgunshell.mdl");
	
	gpEngine->pfnPrecacheSound("items/9mmclip1.wav");
	
	gpEngine->pfnPrecacheSound("weapons/dbarrel1.wav");
	gpEngine->pfnPrecacheSound("weapons/sbarrel1.wav");
	
	gpEngine->pfnPrecacheSound("weapons/reload1.wav");
	gpEngine->pfnPrecacheSound("weapons/reload3.wav");
	
	gpEngine->pfnPrecacheSound("weapons/357_cock1.wav");
	gpEngine->pfnPrecacheSound("weapons/scock1.wav");
	
	mnSingleFireEvent = gpEngine->pfnPrecacheEvent(1, "events/shotgun1.sc");
	mnDoubleFireEvent = gpEngine->pfnPrecacheEvent(1, "events/shotgun2.sc");
	//
	
	mnID = WEAPON_SHOTGUN;
	
	SetModel("models/w_shotgun.mdl");
};

void CWeaponShotgun::PrimaryAttack()
{
	if(mnClip <= 0)
	{
		return;
	};
	
	--mnClip;
	
	mpOwner->SetAnim(PLAYER_ATTACK1);
};

void CWeaponShotgun::SecondaryAttack()
{
	if(mnClip <= 1)
	{
		Reload();
		PlayEmptySound();
		return;
	};
	
	mnClip -= 2;
	
	int nFlags = 0;
};

void CWeaponShotgun::Reload()
{
	if(mfNextPrimaryAttack > UTIL_WeaponTimeBase())
		return;
	
	++mnClip;
	mpOwner->GetInventory()->AddAmmo(mnPrimaryAmmoType, -1);
	mbInSpecialReload = true;
};