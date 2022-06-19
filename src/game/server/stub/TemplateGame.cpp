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

#include "TemplateGame.hpp"

EXPOSE_SINGLE_INTERFACE(CTemplateGame, IGame, OGS_GAME_INTERFACE_VERSION);

bool CTemplateGame::Init(CreateInterfaceFn afnEngineFactory)
{
	if(!CBaseGame::Init())
		return false;
	
	RegisterCvars();
	
	// TODO: Initialize your game/mod here
	
	return true;
};

void CTemplateGame::InstallRules()
{
	// TODO: Install your game rules here
};

void CTemplateGame::RegisterCvars()
{
	// TODO: Register your game console variables here
};