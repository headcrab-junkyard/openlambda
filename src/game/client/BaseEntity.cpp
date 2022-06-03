/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2019-2022 BlackPhrase
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
/// @brief base entity stubs

#include "BaseEntity.hpp"

void CBaseEntity::FireBullets(unsigned long anShots, const idVec3 &avSrc, const idVec3 &avDir, const idVec3 &avSpread, float afDist, int anBulletType, int anTracerFreq, int anDmg, CBaseEntity *apAttacker){}

void CBaseEntity::TraceAttack(CBaseEntity *apAttacker, float afDamage, const idVec3 &avDir, TraceResult *apResult, int anDmgBitSum){}
void CBaseEntity::TraceBleed(float afDamage, const idVec3 &avDir, TraceResult *apResult, int anDmgBitSum){}

int CBaseEntity::TakeDamage(CBaseEntity *apInflictor, CBaseEntity *apAttacker, float afDamage, int anDmgBitSum){return 1;}
int CBaseEntity::TakeHealth(float afValue, int anDmgBitSum){return 1;}

int CBaseEntity::DamageDecal(int anDmgBitSum){return -1;}

int CBaseEntity::Save(CSave &aSave){return 1;}
int CBaseEntity::Restore(CRestore &aRestore){return 1;}

void CBaseEntity::SetOrigin(const idVec3 &avOrigin){}
void CBaseEntity::SetSize(const idVec3 &avMins, const idVec3 &avMaxs){}

void CBaseEntity::SUB_Remove(){}

// TODO: int?
bool CBaseEntity::ShouldToggle(UseType aeUseType, bool abCurState) const {return false;}

bool CBaseEntity::IsInWorld() const {return true;}

void CBaseEntity::MakeDormant(){}
// TODO: int?
bool CBaseEntity::IsDormant() const {return false;}

// TODO: int?
bool CBaseEntity::Intersects(CBaseEntity *apOther) const {return false;}

void CBaseEntity::SetObjectCollisionBox(){}

CBaseEntity *CBaseEntity::GetNextTarget() const {return nullptr;}

// TODO: IsVisible?
bool CBaseEntity::FVisible(CBaseEntity *apEntity) const {return false;}
bool CBaseEntity::FVisible(const idVec3 &avOrigin) const {return false;}

// TODO: CGameWorld::CreateEntity?
CBaseEntity *CBaseEntity::Create(char *asName, const idVec3 &avOrigin, const idVec3 &avAngles, CBaseEntity *apOwner){return nullptr;}