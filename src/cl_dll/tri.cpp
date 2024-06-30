/*
 * This file is part of OpenLambda Project
 * Copyright (C) 2018-2021, 2023-2024 BlackPhrase
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
/// @brief triangle rendering (if any)

#include "tri.h"
#include "hud.h"

//#include "cl_util.h"

#include <common/const.h>
#include <common/entity_state.h>
#include <common/cl_entity.h>
#include <common/triangleapi.h>

//#include "Exports.h"

// NOTE: Triangle rendering APIs are in gpEngine->pTriAPI

/*
===========
HUD_DrawNormalTriangles

NOTE: Add non-transparent triangles here
===========
*/
void /*CL_DLLEXPORT*/ HUD_DrawNormalTriangles()
{
	//RecClDrawNormalTriangles();
	
	gHUD.mSpectator.DrawOverview();
};

#ifdef _TFC
void RunEventList();
#endif

/*
===========
HUD_DrawTransparentTriangles

NOTE: Render any triangles with transparent rendering mode here
===========
*/
void /*CL_DLLEXPORT*/ HUD_DrawTransparentTriangles()
{
	//RecClDrawTransparentTriangles();
	
#ifdef _TFC
	RunEventList();
#endif
	
	if(gpParticleMan)
		gpParticleMan->Update();
};