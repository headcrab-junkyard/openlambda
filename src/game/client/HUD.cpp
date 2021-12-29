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

#include "HUD.hpp"
#include "engine.h"
#include "cvardef.h"
#include "cdll_int.h"

void UserCmd_Close()
{
	gpEngine->pfnClientCmd("escape");
};

CHUD::~CHUD()
{
};

void CHUD::Init()
{
	default_fov = gpEngine->pfnRegisterVariable("default_fov", "90", 0);
	mpCvarDraw = gpEngine->pfnRegisterVariable("hud_draw", "1", FCVAR_ARCHIVE);
	
	// CHUDAmmo::Init
	gpEngine->pfnAddCommand("cancelselect", UserCmd_Close);
};

void CHUD::VidInit()
{
};