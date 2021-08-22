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

class CWeaponCrowbar : public CBaseWeapon
{
public:
	void Spawn() override;
	
	void PrimaryAttack() override;
private:
	void Swing();
};

LINK_ENTITY_TO_CLASS(weapon_crowbar, CWeaponCrowbar);

void CWeaponCrowbar::Spawn()
{
	mnID = WEAPON_CROWBAR;
	
	SetModel("models/w_crowbar.mdl");
	
	mnClip = -1;
};

void CWeaponCrowbar::PrimaryAttack()
{
	Swing();
};

void CWeaponCrowbar::Swing()
{
	mfNextPrimaryAttack = 0.5;
	
	mpOwner->SetAnimation(PLAYER_ATTACK1);
};