/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2018(-2019?), 2020-2022 BlackPhrase
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

#include <pm_shared/pm_defs.h>
#include <pm_shared/pm_debug.h>
#include <pm_shared/pm_shared.h>
//#include <pm_shared/pm_movevars.h>

extern playermove_t *pmove;

// Expand debugging bounding box particle hulls by this many units
const float BOX_GAP = 0.0f;

static int PM_boxpnt[6][4] =
{
	{0, 4, 6, 2}, // +X
	{0, 1, 5, 4}, // +Y
	{0, 2, 3, 1}, // +Z
	{7, 5, 1, 3}, // -X
	{7, 3, 2, 6}, // -Y
	{7, 6, 4, 5}, // -Z
};

/*
================
PM_DrawPhysEntBBox
================
*/
void PM_DrawPhysEntBBox(int num, int pcolor, float life)
{
	physent_t *pe;
	vec3_t org;
	int j;
	vec3_t tmp;
	vec3_t p[8];
	float gap = BOX_GAP;
	vec3_t modelmins, modelmaxs;
	
	if(num >= pmove->numphysent || num <= 0)
		return;
	
	pe = &pmove->physents[num];
	
	if(pe->model)
	{
		VectorCopy(pe->origin, org);
		
		pmove->PM_GetModelBounds(pe->model, modelmins, modelmaxs);
		
		for(j = 0; j < 8; ++j)
		{
			tmp[0] = (j & 1) ? modelmins[0] - gap : modelmaxs[0] + gap;
			tmp[1] = (j & 2) ? modelmins[1] - gap : modelmaxs[1] + gap;
			tmp[2] = (j & 4) ? modelmins[2] - gap : modelmaxs[2] + gap;
			
			VectorCopy(tmp, p[j]);
		};
		
		// If the bounding box should be rotated, do that
		if(pe->angles[0] || pe->angles[1] || pe->angles[2])
		{
			vec3_t forward, right, up;
			AngleVectorsTranspose(pe->angles, forward, right, up);
			
			for(j = 0; j < 8; ++j)
			{
				VectorCopy(p[j], tmp);
				
				p[j][0] = DotProduct(tmp, forward);
				p[j][1] = DotProduct(tmp, right);
				p[j][2] = DotProduct(tmp, up);
			};
		};
		
		// Offset by entity origin, if any
		for(j = 0; j < 8; ++j)
			VectorAdd(p[j], org, p[j]);
		
		for(j = 0; j < 6; ++j)
			PM_DrawRectangle(p[PM_boxpnt[j][1]], p[PM_boxpnt[j][0]], p[PM_boxpnt[j][2]], p[PM_boxpnt[j][3]], pcolor, life);
	}
	else
	{
		for(j = 0; j < 8; ++j)
		{
			tmp[0] = (j & 1) ? pe->mins[0] : pe->maxs[0];
			tmp[1] = (j & 2) ? pe->mins[1] : pe->maxs[1];
			tmp[2] = (j & 4) ? pe->mins[2] : pe->maxs[2];
			
			VectorAdd(tmp, pe->origin, tmp);
			VectorCopy(tmp, p[j]);
		};
		
		for(j = 0; j < 6; ++j)
			PM_DrawRectangle(p[PM_boxpnt[j][1]], p[PM_boxpnt[j][0]], p[PM_boxpnt[j][2]], p[PM_boxpnt[j][3]], pcolor, life);
	};
};

#ifndef DEDICATED // TODO: SWDS?
/*
================
PM_ViewEntity

Shows a particle trail from player to entity in crosshair
Shows particles at that entities bounding box

Tries to shoot a ray out by about 128 units
================
*/
void PM_ViewEntity()
{
#if 0
	if(!pm_showclip.value)
		return;
#endif
	
	// Determine movement angles
	vec3_t forward, right, up;
	AngleVectors(pmove->angles, forward, right, up);
	
	vec3_t origin;
	VectorCopy(pmove->origin, origin);
	
	float fup = 0.5 * (pmove->player_mins[pmove->usehull][2] + pmove->player_maxs[pmove->usehull][2]);
	fup += pmove->view_ofs[2];
	fup -= 4;
	
	vec3_t end;
	float raydist = 256.0f;
	
	for(int i = 0; i < 3; ++i)
		end[i] = origin[i] + raydist * forward[i];
	
	pmtrace_t trace = pmove->PM_PlayerTrace(origin, end, PM_STUDIO_BOX, -1);
	
	int pcolor = 77;
	
	if(trace.ent > 0)
	{
		// Not the world
		pcolor = 111;
		
		// Draw the hull or bounding box
		PM_DrawPhysEntBBox(trace.ent, pcolor, 0.3f);
	};
};
#endif // DEDICATED

//PM_ParticleLine

/*
================
PM_DrawRectangle
================
*/
void PM_DrawRectangle(vec3_t topleft, vec3_t bottomleft, vec3_t topright, vec3_t bottomright, int pcolor, float life)
{
	PM_ParticleLine(topleft, bottomleft, pcolor, life, 0);
	PM_ParticleLine(bottomleft, bottomright, pcolor, life, 0);
	PM_ParticleLine(bottomright, topright, pcolor, life, 0);
	PM_ParticleLine(topright, topleft, pcolor, life, 0);
};

/*
================
PM_DrawBBox
================
*/
void PM_DrawBBox(vec3_t mins, vec3_t maxs, vec3_t origin, int pcolor, float life)
{
	int j;
	vec3_t tmp;
	vec3_t p[8];
	float gap = BOX_GAP;
	
	for(j = 0; j < 8; ++j)
	{
		tmp[0] = (j & 1) ? mins[0] - gap : maxs[0] + gap;
		tmp[1] = (j & 2) ? mins[1] - gap : maxs[1] + gap;
		tmp[2] = (j & 4) ? mins[2] - gap : maxs[2] + gap;
		
		VectorAdd(tmp, origin, tmp);
		VectorCopy(tmp, p[j]);
	};
	
	for(j = 0; j < 6; ++j)
		PM_DrawRectangle(p[PM_boxpnt[j][1]], p[PM_boxpnt[j][0]], p[PM_boxpnt[j][2]], p[PM_boxpnt[j][3]], pcolor, life);
};

/*
================
PM_ParticleLine
================
*/
void PM_ParticleLine(vec3_t start, vec3_t end, int pcolor, float life, float vert)
{
	float linestep = 2.0f;
	vec3_t curpos;
	vec3_t diff;
	
	// Determine distance
	VectorSubtract(end, start, diff);
	
	float len = VectorNormalize(diff);
	
	float curdist = 0;
	
	while(curdist <= len)
	{
		for(int i = 0; i < 3; ++i)
			curpos[i] = start[i] + curdist * diff[i];
		
		pmove->PM_Particle(curpos, pcolor, life, 0, vert);
		curdist += linestep;
	};
};

/*
================
PM_ShowClipBox
================
*/
void PM_ShowClipBox()
{
#ifdef _DEBUG
	vec3_t org;
	vec3_t offset = {0, 0, 0};
	
	if(!pmove->runfuncs)
		return;
	
	// More debugging, draw the particle bbox for player and for the 
	// entity we are looking directly at
	// Also prints entity info to the console overlay
	//if(!pmove->server)
		//return;
	
	// Draw entity in center of view
	// Also draws the normal to the clip plane that intersects our
	// movement ray. Leaves a particle trail at the intersection point
	PM_ViewEntity();
	
	VectorCopy(pmove->origin, org);
	
	if(pmove->server)
		VectorAdd(org, offset, org);
	else
		VectorSubtract(org, offset, org);
	
	// Show our bounding box in particles
	PM_DrawBBox(pmove->player_mins[pmove->usehull], pmove->player_maxs[pmove->usehull], org, pmove->server ? 132 : 0, 0.1);
	
	PM_ParticleLine(org, org, pmove->server ? 132 : 0, 0.1, 5.0);
	
/*
	{
		for(int i = 0; i < pmove->numphysent; ++i)
		{
			if(pmove->physents[i].info >= 1 && pmove->physents[i].info <= 4)
				PM_DrawBBox(pmove->player_mins[pmove->usehull], pmove->player_maxs[pmove->usehull], pmove->physents[i].origin, 132, 0.1);
		};
	};
*/
#endif // _DEBUG
};