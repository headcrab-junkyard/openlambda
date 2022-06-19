/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2018, 2021 BlackPhrase
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

class CWeaponDeagle final : public CBaseWeapon
{
public:
	CWeaponDeagle();
	~CWeaponDeagle();
	
	void OnSpawn() override;
	
	void PrimaryAttack() override;
	
	void Reload() override;
private:
};

CWeaponDeagle::CWeaponDeagle() = default;
CWeaponDeagle::~CWeaponDeagle() = default;

void CWeaponDeagle::OnSpawn()
{
	// TODO: Precache()
	gpEngine->PrecacheModel("models/v_deagle.mdl");
	gpEngine->PrecacheModel("models/p_deagle.mdl");
	gpEngine->PrecacheModel("models/w_deagle.mdl");
	//
	
	mnID = WEAPON_DEAGLE;
	
	SetModel("models/w_deagle.mdl");
};

void CWeaponDeagle::PrimaryAttack()
{
	if(mnClip <= 0)
	{
		return;
	};
	
	--mnClip;
	
	mpOwner->SetAnim(PLAYER_ATTACK1);
};

void CWeaponDeagle::Reload()
{
};