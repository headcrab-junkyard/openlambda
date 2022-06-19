/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2020-2022 BlackPhrase
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

#include "PointEntity.hpp"
#include "Util.hpp"

/*QUAKED light (0 1 0) (-8 -8 -8) (8 8 8) START_OFF
Non-displayed light.
Default light value is 300
Default style is 0
If targeted, it will toggle between on or off.
*/

constexpr auto START_OFF{1};

class CLight : public CPointEntity
{
public:
	bool PreSpawn() const override;
	void Spawn() override;
	
	bool HandleKeyValue(const std::string &asKey, const std::string &asValue) override;
	
	void Use(CBaseEntity *apActivator, CBaseEntity *apCaller, UseType aeUseType, float afValue) override;
private:
	int mnStyle{0};
};

LINK_ENTITY_TO_CLASS(light, CLight);
LINK_ENTITY_TO_CLASS(light_spot, CLight);

bool CLight::PreSpawn() const
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
	if(mnStyle >= 32)
	{
		//SetUseCallback(CLight::Use);
		if(self->spawnflags & START_OFF)
			gpEngine->pfnLightStyle(mnStyle, "a");
		else
			gpEngine->pfnLightStyle(mnStyle, "m");
	};
};

bool CLight::HandleKeyValue(const std::string &asKey, const std::string &asValue)
{
	if(asKey == "style")
	{
		mnStyle = std::stoi(asValue);
		return true;
	}
	else if(asKey == "pitch")
	{
		self->angles[0] = std::stof(asValue);
		return true;
	}
	else if(asKey == "pattern")
	{
		//mnPattern = gpEngine->pfnAllocString(asValue.c_str());
		return true;
	};
	
	return CPointEntity::HandleKeyValue(asKey, asValue);
};

void CLight::Use(CBaseEntity *apActivator, CBaseEntity *apCaller, UseType aeUseType, float afValue)
{
	if(self->spawnflags & START_OFF)
	{
		gpEngine->pfnLightStyle(mnStyle, "m");
		self->spawnflags &= ~START_OFF;
	}
	else
	{
		gpEngine->pfnLightStyle(mnStyle, "a");
		self->spawnflags |= START_OFF;
	};
};