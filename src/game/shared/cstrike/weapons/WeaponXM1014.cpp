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

class CWeaponXM1014 final : public CBaseWeapon
{
public:
	CWeaponXM1014();
	~CWeaponXM1014();
	
	void OnSpawn() override;
	
	void Attack() override;
	
	void Reload() override;
private:
};

CWeaponXM1014::CWeaponXM1014() = default;
CWeaponXM1014::~CWeaponXM1014() = default;

void CWeaponXM1014::OnSpawn()
{
};

void CWeaponXM1014::Attack()
{
};

void CWeaponXM1014::Reload()
{
};