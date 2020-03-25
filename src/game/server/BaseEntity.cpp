/*
 * This file is part of OGS Engine
 * Copyright (C) 2020 BlackPhrase
 *
 * OGS Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OGS Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OGS Engine. If not, see <http://www.gnu.org/licenses/>.
 */

/// @file

#include "BaseEntity.hpp"
#include "engine.h"

edict_t *CBaseEntity::ToEdict() const
{
	return gpEngine->pfnFindEntityByVars(self);
};
	
int CBaseEntity::GetIndex() const
{
	return gpEngine->pfnIndexOfEdict(ToEdict());
};

void CBaseEntity::SetModel(const std::string &asName)
{
	gpEngine->pfnSetModel(ToEdict(), asName.c_str());
};

void CBaseEntity::SetOrigin(const idVec3 &avOrigin)
{
	mvOrigin = avOrigin;
	gpEngine->pfnSetOrigin(ToEdict(), avOrigin);
};

void CBaseEntity::SetSize(const idVec3 &avMins, const idVec3 &avMaxs)
{
	gpEngine->pfnSetSize(ToEdict(), avMins, avMaxs);
};

void CBaseEntity::EmitSound(int anChannel, const std::string &asSample, float afVolume, float afAttenuation, int anFlags, int anPitch)
{
	gpEngine->pfnEmitSound(ToEdict(), anChannel, asSample.c_str(), afVolume, afAttenuation, anFlags, anPitch);
};