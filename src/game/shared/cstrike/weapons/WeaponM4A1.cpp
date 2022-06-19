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

class CWeaponM4A1 final : public CBaseWeapon
{
public:
	CWeaponM4A1();
	~CWeaponM4A1();
	
	void OnSpawn() override;
	
	void Attack() override;
	
	void Reload() override;
private:
};

CWeaponM4A1::CWeaponM4A1() = default;
CWeaponM4A1::~CWeaponM4A1() = default;

void CWeaponM4A1::OnSpawn()
{
};

void CWeaponM4A1::Attack()
{
};

void CWeaponM4A1::Reload()
{
};