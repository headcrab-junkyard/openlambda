/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2022 BlackPhrase
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

#include "GameWorld.hpp"

void CGameWorld::RemoveEntity(CBaseEntity *apEntity){}

CBaseEntity *CGameWorld::FindEntityInSphere(CBaseEntity *apStartEntity, const idVec3 &avCenter, float afRadius) const {return nullptr;}

void CGameWorld::SpawnBlood(const idVec3 &avOrigin, int anBloodColor, float afDamage){}

void CGameWorld::BloodDrips(const idVec3 &avOrigin, const idVec3 &avDirection, int anBloodColor, int anAmount){}

void CGameWorld::DecalGunShot(TraceResult *apTraceResult, int anBulletType){}

void CGameWorld::EjectBrass(const idVec3 &avOrigin, const idVec3 &avVelocity, float afRotation, int anModel, int anSoundType){}

void CGameWorld::RadiusDamage(const idVec3 &avOrigin, CBaseEntity *apInflictor, CBaseEntity *apAttacker, float afDamage, float afRadius, int anClassIgnore, int anDmgBitSum){}