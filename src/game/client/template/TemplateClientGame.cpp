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

#include "TemplateClientGame.hpp"

CTemplateClientGame gClientGame;
IClientGame *gpClientGame{&gClientGame};

#if OPENLAMBDA_TARGET_ENGINE == OPENLAMBDA_TARGET_ENGINE_NEXT
//EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CTemplateClientGame, IClientGame, OGS_CLIENTGAME_INTERFACE_VERSION, gClientGame);
#else
#	error "Hi there!"
#endif

bool CTemplateClientGame::Init(CreateInterfaceFn afnEngineFactory)
{
	if(!CBaseClientGame::Init(afnEngineFactory))
		return false;
	
	// NOTE: Initialize your game/mod-specific stuff here
	return true;
};

void CTemplateClientGame::Shutdown()
{
	// NOTE: Shutdown your stuff here
	
	CBaseClientGame::Shutdown();
};

void CTemplateClientGame::Frame(float afTimeStep)
{
	CBaseClientGame::Frame(afTimeStep);
	
	// NOTE: You can use this to update your game/mod-specific components every frame
};