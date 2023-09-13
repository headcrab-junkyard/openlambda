/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2023 BlackPhrase
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

#include "BaseDoor.hpp"

//============================================================================

class CRotatingDoor : public CBaseDoor
{
public:
	void Spawn() override;
	
	virtual void SetToggleState(int anState);
};

LINK_ENTITY_TO_CLASS(func_door_rotating, CRotatingDoor);

void CRotatingDoor::Spawn()
{
	//Precache(); // TODO
	
	// Set the axis of rotation
	CBaseToggle::AxisDir(self);
	
	// Check for clockwise rotation
	if(GetSpawnFlags() & SF_DOOR_ROTATE_BACKWARDS)
		SetMoveDir(-GetMoveDir());
	
	mvAngle1 = GetAngles();
	mvAngle2 = GetAngles() + GetMoveDir() * mfMoveDistance;
	
	ASSERTZ(mvAngle1 != mvAngle2, "rotating door start/end positions are equal");
	
	SetSolidity(CBaseEntity::Solidity::BSP);
	
	if(GetSpawnFlags() & SF_DOOR_PASSABLE)
		SetSolidity(CBaseEntity::Solidity::None);
	
	SetMoveType(CBaseEntity::MoveType::Push);
	SetOrigin(GetOrigin());
	SetModel(gpEngine->pfnString(GetModel()));
	
	if(GetSpeed() == 0)
		SetSpeed(100);
	
	// DOOR_START_OPEN is to allow an entity to be lighted in the closed position but spawn in the open position
	if(GetSpawnFlags() & SF_DOOR_START_OPEN)
	{
		// Swap pos1 and pos2, put the door at pos2, invert the movement dir
		self->angles = mvAngle2;
		idVec3 vSav{mvAngle1};
		mvAngle2 = mvAngle1;
		mvAngle1 = vSav;
		SetMoveDir(-GetMoveDir());
	};
	
	mToggleState = TS_AT_BOTTOM;
	
	// Touchable button
	SetTouchCallback(CRotatingDoor::DoorTouch);
	
	if(GetSpawnFlags() & SF_DOOR_USE_ONLY)
		SetTouchCallback(nullptr);
};

void CRotatingDoor::SetToggleState(int anState)
{
	SetAngles(mvAngle1);
	
	if(anState == TS_AT_TOP)
		SetAngles(mvAngle2);
	
	SetOrigin(GetOrigin());
};