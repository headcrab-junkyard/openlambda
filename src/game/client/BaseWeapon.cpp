/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2020-2021 BlackPhrase
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

int CBaseWeapon::Save(CSave &aSave){return 1;}
int CBaseWeapon::Restore(CRestore &aRestore){return 1;}

bool CBaseWeapon::IsUseable() const {return true;}

void CBaseWeapon::RetireWeapon(){}

bool CBaseWeapon::AddPrimaryAmmo(int anCount, const char *asName, int anMaxClip, int anMaxCarry){return true;}
bool CBaseWeapon::AddSecondaryAmmo(int anCount, const char *asName, int anMax){return true;}

int CBaseWeapon::ExtractAmmo(CBaseWeapon *apWeapon){return 0;}
int CBaseWeapon::ExtractClipAmmo(CBaseWeapon *apWeapon){return 0;}

int CBaseWeapon::GetPrimaryAmmoIndex() const {return -1;}
int CBaseWeapon::GetSecondaryAmmoIndex() const {return -1;}

float CBaseWeapon::GetNextAttackDelay(float afTime) const {return afTime;}

bool CBaseWeapon::AddToPlayer(CBasePlayer *apPlayer){return false;}

int CBaseWeapon::AddDuplicate(CBaseItem *apOriginal){return 0;}

int CBaseWeapon::UpdateClientData(CBasePlayer *apPlayer){return 0;}