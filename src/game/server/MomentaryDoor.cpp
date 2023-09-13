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

//=============================================================================

class CMomentaryDoor : public CBaseDoor
{
public:
	void Spawn() override;
	//void Precache();
};

LINK_ENTITY_TO_CLASS(momentary_door, CMomentaryDoor);

void CMomentaryDoor::Spawn()
{
	SetMovedir(self);
};

bool CMomentaryDoor::HandleKeyValue(ogs::tStringView asKey, ogs::tStringView asValue)
{
	if(asKey == "movesnd")
	{
		mbMoveSound = std::stof(asValue.data());
		return true;
	}
	else if(asKey == "stopsnd")
	{
		return true;
	}
	else if(asKey == "healthvalue")
	{
		return true;
	};
	
	return CBaseToggle::HandleKeyValue(asKey, asValue);
};

void CMomentaryDoor::Use(CBaseEntity *apActivator, CBaseEntity *apCaller, UseType aeUseType, float afValue)
{
};

void CMomentaryDoor::DoorMoveDone()
{
};