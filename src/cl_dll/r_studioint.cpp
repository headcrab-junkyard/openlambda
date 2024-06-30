/*
 * This file is part of OpenLambda Project
 * Copyright (C) 2018-2021, 2024 BlackPhrase
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

#include <cstring>

#include <common/const.h>
#include <common/r_studioint.h>

#include "StudioModelRenderer.hpp"

engine_studio_api_t gStudio; // IEngineStudio? gEngineStudio?

CGameStudioModelRenderer gStudioModelRenderer;

void R_StudioInit()
{
	gStudioModelRenderer.Init();
};

int R_StudioDrawModel(int anFlags)
{
	return gStudioModelRenderer.DrawModel(anFlags);
};

int R_StudioDrawPlayer(int anFlags, struct entity_state_s *apPlayer)
{
	return gStudioModelRenderer.DrawPlayer(anFlags, apPlayer);
};

r_studio_interface_t gStudioAPI =
{
	STUDIO_INTERFACE_VERSION,
	
	R_StudioDrawModel,
	R_StudioDrawPlayer
};

//r_studio_interface_t *pStudioAPI = &gStudioAPI;

int HUD_GetStudioModelInterface(int version, struct r_studio_interface_s **ppInterface, struct engine_studio_api_s *pStudio)
{
	if(version != STUDIO_INTERFACE_VERSION)
		return 0;
	
	*ppInterface = &gStudioAPI;
	
	memcpy(&gStudio, pStudio, sizeof(engine_studio_api_t));
	
	R_StudioInit();
	
	return 1;
};