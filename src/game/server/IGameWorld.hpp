/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2021 BlackPhrase
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

#pragma once

#include <string>

class CBaseEntity;

struct IGameWorld
{
	///
	virtual CBaseEntity *SpawnEntity(const std::string &asName = "") = 0;
	
	///
	virtual void RemoveEntity(CBaseEntity *apEntity) = 0;
	
	///
	virtual CBaseEntity *GetEntityByIndex(int anIndex) const = 0;
	
	///
	virtual CBaseEntity *FindEntityByString(CBaseEntity *apEntSearchAfter, const std::string &asString, const std::string &asValue) const = 0;
	
	///
	virtual CBaseEntity *FindEntityByTargetName(CBaseEntity *apEntSearchAfter, const std::string &asTargetName) const = 0;
};