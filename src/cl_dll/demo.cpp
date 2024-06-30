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

#include "demo.h"

//#include "hud.h"
//#include "cl_util.h"

//#include <common/demo_api.h>

//#include <memory.h>

//#include "exports.hpp"

// TODO: There should be buffer helper functions to avoid all of the "*(int*)&"s crap

int gnDemoSniper{0};
int gnDemoSniperDamage{0};

vec3_t gvDemoSniperOrg{};
vec3_t gvDemoSniperAngles{};

float gfDemoZoom;

/*
===========
Demo_WriteBuffer

Write some data to the demo stream
===========
*/
void Demo_WriteBuffer(int anType, int anSize, byte *apBuffer)
{
	int nPos{0};
	byte buf[32 * 1024]{};
	
	*(int*)&buf[nPos] = anType;
	nPos += sizeof(int);
	
	memcpy(&buf[pos], apBuffer, anSize);
	
	// Write full buffer out
	gpEngine->pDemoAPI->WriteBuffer(anSize + sizeof(int), apBuffer);
};

/*
===========
CL_ReadDemoBuffer

Engine wants us to parse some data from the demo stream
===========
*/
void /*CL_DLLEXPORT*/ CL_ReadDemoBuffer(int size, byte *buffer)
{
	//RecClReadDemoBuffer(size, buffer);
	
	int nType{*(int*)buffer};
	int i{0};
	
	i += sizeof(int);
	
	switch(nType)
	{
	case TYPE_SNIPERDOT:
		gnDemoSniper = *(int*)&buffer[i];
		i += sizeof(int);
		
		if(gnDemoSniper)
		{
			gnDemoSniperDamage = *(int*)&buffer[i];
			i += sizeof(int);
			
			gvDemoSniperAngles[0] = *(float*)&buffer[i];
			i += sizeof(float);
			
			gvDemoSniperAngles[1] = *(float*)&buffer[i];
			i += sizeof(float);
			
			gvDemoSniperAngles[2] = *(float*)&buffer[i];
			i += sizeof(float);
			
			gvDemoSniperOrg[0] = *(float*)&buffer[i];
			i += sizeof(float);
			
			gvDemoSniperOrg[1] = *(float*)&buffer[i];
			i += sizeof(float);
			
			gvDemoSniperOrg[2] = *(float*)&buffer[i];
			i += sizeof(float);
		};
		
		break;
	case TYPE_ZOOM:
		gfDemoZoom = *(float*)&buffer[i];
		i += sizeof(float);
		break;
	default:
		gpEngine->Con_DPrintf("Unknown demo buffer type, skipping.\n");
		break;
	};
};