/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2019-2023 BlackPhrase
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
/// @brief multiplayer intermission spot entity

#include "PointEntity.hpp"
#include "BaseGame.hpp"
#include "IGameWorld.hpp"
#include "IStringPool.hpp"
#include "Util.hpp"

//=============================================================================

/*QUAKED info_intermission (1 0.5 0.5) (-16 -16 -16) (16 16 16)
This is the camera point for the intermission.
Use mangle instead of angle, so you can set pitch or roll as well as yaw.  'pitch roll yaw'
*/
class CInfoIntermission : public CPointEntity
{
public:
	void Spawn() override;
	
	void Think() override;
};

LINK_ENTITY_TO_CLASS(info_intermission, CInfoIntermission);

void CInfoIntermission::Spawn()
{
	//SetAngles(self->mangle); // So C can get at it
	
	SetOrigin(GetOrigin());
	SetSolidity(CBaseEntity::Solidity::None);
	SetEffects(EF_NODRAW);
	self->v_angle = idVec3::Origin;
	
	SetNextThink(gpGlobals->time + 2); // Let the targets spawn
};

void CInfoIntermission::Think()
{
	// Find my target
	auto pTarget{mpGame->GetWorld()->FindEntityByTargetName(nullptr, mpGame->GetStringPool()->GetByIndex(this->GetTarget()))};
	
	if(pTarget) // TODO: !FNullEnt(pTarget)?
	{
		self->v_angle = gpEngine->pfnVecToAngles((pTarget->GetOrigin() - GetOrigin()).Normalize());
		self->v_angle.x = -self->v_angle.x;
	};
};