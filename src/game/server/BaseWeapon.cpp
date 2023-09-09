/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2018-2023 BlackPhrase
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

//=============================================================================

void CBaseWeapon::PrimaryAttack()
{
	GetOwner()->FireBullets();
	WeaponSound(WEAPON_SINGLE);
	SendWeaponAnim(ACT_VM_PRIMARYFIRE);
	mfNextPrimaryAttack = gpGlobals->time + mfFireRate;
	AddViewKick();
};

void CBaseWeapon::SecondaryAttack()
{
};

void CBaseWeapon::DryFire()
{
	WeaponSound(WEAPON_EMPTY);
	SendWeaponAnim(ACT_VM_DRYFIRE);
	
	mfNextPrimaryAttack = gpGlobals->time + 0.0f;
};

void CBaseWeapon::ItemPreFrame()
{
};

void CBaseWeapon::ItemBusyFrame()
{
	if(mfNextPrimaryAttackTime <= gpGlobals->time)
		FinishReload();
};

void CBaseWeapon::ItemHolsterFrame()
{
};

void CBaseWeapon::ItemPostFrame()
{
	if(mbReloading)
		return;
	
	if(GetOwner()->buttons & IN_ATTACK && mfNextPrimaryAttackTime <= gpGlobals->time)
		PrimaryAttack();
	
	if(GetOwner()->buttons & IN_ATTACK2 && mfNextSecondaryAttackTime <= gpGlobals->time)
		SecondaryAttack();
	
	HandleFireOnEmpty();
	
	if(GetOwner()->buttons & IN_RELOAD)
		Reload();
	
	WeaponIdle();
};

void CBaseWeapon::Holster()
{
	mbHolstered = true;
};

void CBaseWeapon::StartReload()
{
	mbReloading = true;
	mfNextPrimaryAttackTime = gpGlobals->time + 5.0f; // TODO
};

void CBaseWeapon::FinishReload()
{
	mbReloading = false;
};

void CBaseWeapon::Touch(CBaseEntity *other)
{
	other->ammo_shells += 10;
};