/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
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

#include "BaseEntity.hpp"
#include "Util.hpp"

/*
==============================================================================

SIMPLE BMODELS

==============================================================================
*/

/*QUAKED func_wall (0 .5 .8) ?
This is just a solid wall if not inhibitted
*/
class CFuncWall : public CBaseEntity
{
public:
	void Spawn() override;
	
	void Use(CBaseEntity *apActivator, CBaseEntity *apCaller, UseType aeUseType, float afValue) override;
};

LINK_ENTITY_TO_CLASS(func_wall, CFuncWall);

void CFuncWall::Spawn()
{
	SetAngles(idVec3::Origin);
	SetMoveType(MOVETYPE_PUSH); // So it doesn't get pushed by anything
	SetSolidity(SOLID_BSP);
	SetUseCallback(CFuncWall::Use);
	
	SetModel(GetModel());
};

void CFuncWall::Use(CBaseEntity *apActivator, CBaseEntity *apCaller, UseType aeUseType, float afValue)
{
	// Change to alternate textures
	self->frame = 1 - self->frame;
};

//============================================================================

/*QUAKED func_illusionary (0 .5 .8) ?
A simple entity that looks solid but lets you walk through it.
*/
class CFuncIllusionary : public CBaseEntity
{
public:
	void Spawn() override;
};

LINK_ENTITY_TO_CLASS(func_illusionary, CFuncIllusionary);

void CFuncIllusionary::Spawn()
{
	SetAngles(idVec3::Origin);
	SetMoveType(MOVETYPE_NONE);
	SetSolidity(SOLID_NOT);
	
	SetModel(GetModel());
	
	MakeStatic();
};