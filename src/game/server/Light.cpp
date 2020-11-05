/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2020 BlackPhrase
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

const float START_OFF = 1;

class CLight : public CPointEntity
{
public:
	bool PreSpawn() const override;
	void Spawn() override;
	
	void Use(CBaseEntity *other) override;
};

bool CLight::PreSpawn()
{
	if(!self->targetname)
	{
		// inert light
		return false;
	};
	
	return true;
};

void CLight::Spawn()
{
	if(self->style >= 32)
	{
		SetUseCallback(CLight::Use);
		if(self->spawnflags & START_OFF)
			gpEngine->SetLightStyle(self->style, "a");
		else
			gpEngine->SetLightStyle(self->style, "m");
	};
};

void CLight::Use(CBaseEntity *other)
{
	if(self->spawnflags & START_OFF)
	{
		gpEngine->SetLightStyle(self->style, "m");
		self->spawnflags = self->spawnflags - START_OFF;
	}
	else
	{
		gpEngine->SetLightStyle(self->style, "a");
		self->spawnflags = self->spawnflags + START_OFF;
	};
};

/*QUAKED light (0 1 0) (-8 -8 -8) (8 8 8) START_OFF
Non-displayed light.
Default light value is 300
Default style is 0
If targeted, it will toggle between on or off.
*/
LINK_ENTITY_TO_CLASS(light, CLight);