/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018, 2020-2023 BlackPhrase
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

#include <assert.h>
#include <math.h>

#include <pm_shared/pm_defs.h>
#include <pm_shared/pm_shared.h>
#include <pm_shared/pm_movevars.h>
//#include <pm_shared/pm_debug.h>
#include <com_model.h>
#include <in_buttons.h>

#define STOP_EPSILON 0.1

#ifndef NULL
#define NULL (void*)0
#endif

// #define	PM_GRAVITY			800
// #define	PM_STOPSPEED		100
// #define	PM_MAXSPEED			320
// #define	PM_SPECTATORMAXSPEED	500
// #define	PM_ACCELERATE		10
// #define	PM_AIRACCELERATE	0.7
// #define	PM_WATERACCELERATE	10
// #define	PM_FRICTION			6
// #define	PM_WATERFRICTION	1

static qboolean pm_shared_initialized = false;

playermove_t *pmove = NULL;

static vec3_t rgv3tStuckTable[54];
static int rgStuckLast[MAX_CLIENTS][2];

// Texture names
static int gcTextures = 0;
static char grgszTextureName[CTEXTURESMAX][CBTEXTURENAMEMAX];
static char grgchTextureType[CTEXTURESMAX];

int g_onladder = 0;

/*
==================
PM_SwapTextures
==================
*/
void PM_SwapTextures(int a, int b)
{
	char chTemp;
	char sTemp[CBTEXTURENAMEMAX];
	
	strcpy(sTemp, grgszTextureName[a]);
	chTemp = grgchTextureType[a];
	
	strcpy(grgszTextureName[a], grgszTextureName[b]);
	grgchTextureType[a] = grgchTextureType[b];
	
	strcpy(grgszTextureName[b], sTemp);
	grgchTextureType[b] = chTemp;
};

/*
==================
PM_SortTextures
==================
*/
void PM_SortTextures()
{
	// Only occurs at startup and it's only 512 elements...
	for(int i = 0; i < gcTextures; ++i)
		for(int j = i + 1; j < gcTextures; ++j)
		{
			if(stricmp(grgszTextureName[i], grgszTextureName[j]) > 0)
			{
				// Swap
				PM_SwapTextures(i, j);
			};
		};
};

/*
==================
PM_InitTextureTypes
==================
*/
void PM_InitTextureTypes()
{
	static qboolean bTextureTypeInit = false;
	
	if(bTextureTypeInit)
		return;
	
	memset(&(grgszTextureName[0][0]), 0, CTEXTURESMAX * CBTEXTURENAMEMAX);
	memset(grgchTextureType, 0, CTEXTURESMAX);
	
	gcTextures = 0;
	
	char buffer[512];
	memset(buffer, 0, 512);
	
	int nFileSize = pmove->COM_FileSize("sound/materials.txt");
	byte *pMemFile = pmove->COM_LoadFile("sound/materials.txt", 5, NULL);
	if(pMemFile)
		return;
	
	int nFilePos = 0;
	
	int i, j;
	
	// For each line in the file...
	while(pmove->memfgets(pMemFile, nFileSize, &nFilePos, buffer, 511) != NULL && (gcTextures < CTEXTURESMAX))
	{
		// Skip whitespace
		i = 0;
		while(buffer[i] && isspace(buffer[i]))
			++i;
		
		if(!buffer[i])
			continue;
		
		// Skip comment lines
		if(buffer[i] == '/' || !isalpha(buffer[i]))
			continue;
		
		// Get texture type
		grgchTextureType[gcTextures] = toupper(buffer[i++]);
		
		// Skip whitespace
		while(buffer[i] && isspace[buffer[i]))
			++i;
		
		if(!buffer[i])
			continue;
		
		// Get sentence name
		j = i;
		while(buffer[j] && !isspace[buffer[j]))
			++j;
		
		if(!buffer[j])
			continue;
		
		// Null-terminate the name and save in sentences array
		j = min(j, CBTEXTURENAMEMAX - 1 + i);
		buffer[j] = 0;
		strcpy(&(grgszTextureName[gcTextures++][0]), &(buffer[i]));
	};
	
	// Must use the engine to free since we are in a dll
	pmove->COM_FreeFile(pMemFile);
	
	PM_SortTextures();
	
	bTextureTypeInit = true;
};

/*
==================
PM_GetTextureType
==================
*/
char PM_GetTextureType(const char *name)
{
	assert(pm_shared_initialized);
	
	int left = 0;
	int right = gcTextures - 1;
	
	int pivot;
	int val;
	
	while(left <= right)
	{
		pivot = (left + right) * 0.5;
		
		val = strnicmp(name, grgszTextureName[pivot], CBTEXTURENAMEMAX - 1);
		
		if(val == 0)
			return grgchTextureType[pivot];
		else if(val > 0)
			left = pivot + 1;
		else if(val < 0)
			right = pivot - 1;
	};
	
	return CHAR_TEX_CONCRETE;
};

/*
==================
PM_PlayStepSound
==================
*/
void PM_PlayStepSound(int step, float vol)
{
	// TODO
};

// TODO: unused
/*
==================
PM_MapTextureTypeStepType
==================
*/
int PM_MapTextureTypeStepType(char anTextureType)
{
	switch(anTextureType)
	{
	default:
	case CHAR_TEX_CONCRETE:
		return STEP_CONCRETE;
	case CHAR_TEX_METAL:
		return STEP_METAL;
	case CHAR_TEX_DIRT:
		return STEP_DIRT;
	case CHAR_TEX_VENT:
		return STEP_VENT;
	case CHAR_TEX_GRATE:
		return STEP_GRATE;
	case CHAR_TEX_TILE:
		return STEP_TILE;
	case CHAR_TEX_SLOSH:
		return STEP_SLOSH;
	};
};

/*
==================
PM_CatagorizeTextureType

Determine texture info for the texture we are standing on
==================
*/
void PM_CatagorizeTextureType()
{
	vec3_t start, end;
	
	VectorCopy(pmove->origin, start);
	VectorCopy(pmove->origin, end);
	
	// Straight down
	end[2] -= 64;
	
	// Fill in default values, just in case
	pmove->sztexturename[0] = '\0';
	pmove->chtexturetype = CHAR_TEX_CONCRETE;
	
	const char *sTextureName = pmove->PM_TraceTexture(pmove->onground, start, end);
	if(!sTextureName)
		return;
	
	// Strip leading '-0' or '+0~' or '{' or '!'
	if(*sTextureName == '-' || *sTextureName == '+')
		sTextureName += 2;
	
	if(*sTextureName == '{' || *sTextureName == '!' || *sTextureName == '~' || *sTextureName == ' ')
		++sTextureName;
	
	strcpy(pmove->sztexturename, sTextureName);
	pmove->sztexturename[CBTEXTURENAMEMAX - 1] = 0;
	
	// Get texture type
	pmove->chtexturetype = PM_FindTextureType(pmove->sztexturename);
};

// TODO: unused
/*
==================
PM_UpdateStepSound
==================
*/
void PM_UpdateStepSound()
{
	// TODO
	
	if(pmove->flTimeStepSound > 0)
		return;
	
	if(pmove->flags & FL_FROZEN)
		return;
	
	PM_CatagorizeTextureType();
	
	float speed = Length(pmove->velocity);
	
	// Determine if we are on a ladder
	int fLadder = (pmove->movetype == MOVETYPE_FLY); // TODO: IsOnLadder()?
	
	float velwalk = 120;
	float velrun = 210;
	float flduck = 0;
	
	if()
	{
		velwalk = 60;
		velrun = 80;
		flduck = 100;
	};
	
	if()
	{
		fWalking = speed < velrun;
		
		VectorCopy(pmove->origin, center);
		VectorCopy(pmove->origin, knee);
		VectorCopy(pmove->origin, feet);
		
		height = ;
		
		knee[2] = ;
		feet[2] = ;
		
		// Find out what we're stepping in or on...
		if(fLadder)
		{
			step = STEP_LADDER;
			fvol = 0.35;
			pmove->flTimeStepSound = 350;
		}
		else if()
		{
			step = STEP_WADE;
			fvol = 0.65;
			pmove->flTimeStepSound = 600;
		}
		else if()
		{
			step = STEP_SLOSH;
			fvol = fWalking ? 0.2 : 0.5;
			pmove->flTimeStepSound = fWalking ? 400 : 300;
		}
		else
		{
			// Find texture under player, if different from current texture, get material type
			step = PM_MapTextureTypeStepType(pmove->chtexturetype);
			
			switch(pmove->chtexturetype)
			{
			default:
			case CHAR_TEX_CONCRETE:
				break;
			case CHAR_TEX_METAL:
				break;
			case CHAR_TEX_DIRT:
				break;
			case CHAR_TEX_VENT:
				break;
			case CHAR_TEX_GRATE:
				break;
			case CHAR_TEX_TILE:
				break;
			case CHAR_TEX_SLOSH:
				break;
			};
		};
		
		// Slower step time if ducking
		pmove->flTimeStepSound += flduck;
		
		// Play the sound
		
		// 35% volume if ducking
		if(pmove->flags & FL_DUCKING)
			fvol *= 0.35;
		
		PM_PlayStepSound(step, fvol);
	};
};

/*
==================
PM_AddToTouched

Adds the trace result to touch list, if contact is not already in list
==================
*/
qboolean PM_AddToTouched(pmtrace_t trace, vec3_t impactvelocity)
{
	int i;
	
	for(i = 0; i < pmove->numtouch; ++i)
		if(pmove->touchindex[i].ent == tr.ent)
			break;
	
	// Already in list
	if(i != pmove->numtouch)
		return false;
	
	VectorCopy(impactvelocity, tr.deltavelocity);
	
	if(pmove->numtouch >= MAX_PHYSENTS)
		pmove->Con_DPrintf("Too many entities were touched!\n");
	
	pmove->touchindex[pmove->numtouch] = tr;
	++pmove->numtouch;
	return true;
};

/*
==================
PM_CheckVelocity

See if the player has a bogus velocity value
==================
*/
void PM_CheckVelocity()
{
	// Bound velocity
	for(int i = 0; i < 3; ++i)
	{
		// See if it's bogus
		if(IS_NAN(pmove->velocity[i]))
		{
			pmove->Con_Printf("PM  Got a NaN velocity %d\n", i);
			pmove->velocity[i] = 0;
		};
		
		if(IS_NAN(pmove->origin[i]))
		{
			pmove->Con_Printf("PM  Got a NaN origin on %d\n", i);
			pmove->origin[i] = 0;
		};
		
		// Bound it
		if(pmove->velocity[i] > pmove->movevars->maxvelocity)
		{
			pmove->Con_DPrintf("PM  Got a velocity too high on %d\n", i);
			pmove->velocity[i] = pmove->movevars->maxvelocity;
		}
		else if(pmove->velocity[i] < -pmove->movevars->maxvelocity)
		{
			pmove->Con_DPrintf("PM  Got a velocity too low on %d\n", i);
			pmove->velocity[i] = -pmove->movevars->maxvelocity;
		};
	};
};

/*
==================
PM_ClipVelocity

Slide off of the impacting object
returns the blocked flags (0x01 = floor, 0x02 = step / wall)
==================
*/
int PM_ClipVelocity(vec3_t in, vec3_t normal, vec3_t out, float overbounce)
{
	// Assume unblocked
	int blocked = 0x00;
	
	float angle = normal[2];
	
	if(angle > 0) // If the plane that is blocking us has a positive Z-axis component, assume it's a floor
		blocked |= 0x01; // floor
	if(!angle) // If the plane has no Z-axis, it is vertical
		blocked |= 0x02; // wall/step
	
	// Determine how far along plane to slide based on incoming direction
	// Scale by overbounce factor
	float backoff = DotProduct(in, normal) * overbounce;
	
	float change;
	
	for(int i = 0; i < 3; i++)
	{
		change = normal[i] * backoff;
		out[i] = in[i] - change;
		
		// If out velocity is too small, zero it out
		if(out[i] > -STOP_EPSILON && out[i] < STOP_EPSILON)
			out[i] = 0;
	};
	
	// Return blocking flags
	return blocked;
};

/*
==================
PM_AddCorrectGravity
==================
*/
void PM_AddCorrectGravity()
{
	if(pmove->waterjumptime)
		return;
	
	float ent_gravity{1.0f};
	
	if(pmove->gravity)
		ent_gravity = pmove->gravity;
	
	// Add gravity so they'll be in the correct position during movement
	// Yes, this 0.5 looks wrong, but it's not
	pmove->velocity[2] -= (ent_gravity * pmove->movevars->gravity * 0.5 * pmove->frametime);
	pmove->velocity[2] += pmove->basevelocity[2] * pmove->frametime;
	
	pmove->basevelocity[2] = 0;
	
	PM_CheckVelocity();
};

// TODO: unused
/*
==================
PM_FixupGravityVelocity
==================
*/
void PM_FixupGravityVelocity()
{
	if(pmove->waterjumptime)
		return;
	
	float ent_gravity{1.0f};
	
	if(pmove->gravity)
		ent_gravity = pmove->gravity;
	
	// Get the correct velocity for the end of the dt
	pmove->velocity[2] -= (ent_gravity * pmove->movevars->gravity * pmove->frametime * 0.5);
	
	PM_CheckVelocity();
};

/*
============
PM_FlyMove

The basic solid body movement clip that slides along multiple planes
============
*/
int PM_FlyMove()
{
	int bumpcount, numbumps;
	vec3_t dir;
	float d;
	int numplanes;
	vec3_t planes[MAX_CLIP_PLANES];
	vec3_t primal_velocity, original_velocity;
	vec3_t new_velocity;
	int i, j;
	pmtrace_t trace;
	vec3_t end;
	float time_left, allFraction;
	int blocked;

	numbumps = 4;

	blocked = 0;
	VectorCopy(pmove->velocity, original_velocity);
	VectorCopy(pmove->velocity, primal_velocity);
	numplanes = 0;

	time_left = pmove->frametime;

	for(bumpcount = 0; bumpcount < numbumps; bumpcount++)
	{
		for(i = 0; i < 3; i++)
			end[i] = pmove->origin[i] + time_left * pmove->velocity[i];

		trace = pmove->PM_PlayerTrace(pmove->origin, end, PM_NORMAL, -1);

		if(trace.startsolid || trace.allsolid)
		{
			// entity is trapped in another solid
			VectorCopy(vec3_origin, pmove->velocity);
			return 3;
		};

		if(trace.fraction > 0)
		{
			// actually covered some distance
			VectorCopy(trace.endpos, pmove->origin);
			numplanes = 0;
		};

		if(trace.fraction == 1)
			break; // moved the entire distance

		// save entity for contact
		pmove->touchindex[pmove->numtouch].ent = trace.ent;
		pmove->numtouch++;

		if(trace.plane.normal[2] > 0.7)
			blocked |= 1; // floor
		
		if(!trace.plane.normal[2])
			blocked |= 2; // step

		time_left -= time_left * trace.fraction;

		// cliped to another plane
		if(numplanes >= MAX_CLIP_PLANES)
		{
			// this shouldn't really happen
			VectorCopy(vec3_origin, pmove->velocity);
			break;
		};

		VectorCopy(trace.plane.normal, planes[numplanes]);
		numplanes++;

		//
		// modify original_velocity so it parallels all of the clip planes
		//
		for(i = 0; i < numplanes; i++)
		{
			PM_ClipVelocity(original_velocity, planes[i], pmove->velocity, 1);
			for(j = 0; j < numplanes; j++)
				if(j != i)
				{
					if(DotProduct(pmove->velocity, planes[j]) < 0)
						break; // not ok
				};
			if(j == numplanes)
				break;
		};

		if(i != numplanes)
		{
			// go along this plane
		}
		else
		{
			// go along the crease
			if(numplanes != 2)
			{
				//				Con_Printf ("clip velocity, numplanes == %i\n",numplanes);
				VectorCopy(vec3_origin, pmove->velocity);
				break;
			};
			CrossProduct(planes[0], planes[1], dir);
			d = DotProduct(dir, pmove->velocity);
			VectorScale(dir, d, pmove->velocity);
		};

		//
		// if original velocity is against the original velocity, stop dead
		// to avoid tiny occilations in sloping corners
		//
		if(DotProduct(pmove->velocity, primal_velocity) <= 0)
		{
			VectorCopy(vec3_origin, pmove->velocity);
			break;
		};
	};

	if(pmove->waterjumptime)
		VectorCopy(primal_velocity, pmove->velocity);
	
	return blocked;
};

/*
==============
PM_Accelerate
==============
*/
void PM_Accelerate(vec3_t wishdir, float wishspeed, float accel)
{
	int i;
	float addspeed, accelspeed, currentspeed;
	
	// Dead players dont accelerate
	if(pmove->dead)
		return;
	
	// If waterjumping, don't accelerate
	if(pmove->waterjumptime)
		return;
	
	// See if we are changing direction a bit
	currentspeed = DotProduct(pmove->velocity, wishdir);
	
	// Reduce wishspeed by the amount of veer
	addspeed = wishspeed - currentspeed;
	
	// If not going to add any speed, done
	if(addspeed <= 0)
		return;
	
	// Determine amount of acceleration
	accelspeed = accel * pmove->frametime * wishspeed * pmove->friction;
	
	// Cap at addspeed
	if(accelspeed > addspeed)
		accelspeed = addspeed;
	
	// Adjust velocity
	for(i = 0; i < 3; i++)
		pmove->velocity[i] += accelspeed * wishdir[i];
};

/*
=============
PM_WalkMove

Only used by players
Moves along the ground when player has MOVETYPE_WALK
Player is on ground, with no upwards velocity
=============
*/
void PM_WalkMove()
{
	vec3_t start, dest;
	pmtrace_t trace;
	vec3_t original, originalvel;
	vec3_t down, downvel;
	float downdist, updist;
	
	// Copy movement amounts
	float fmove = pmove->cmd.forwardmove;
	float smove = pmove->cmd.sidemove;
	
	// Zero out Z components of movement vectors
	pmove->forward[2] = 0;
	pmove->right[2] = 0;
	
	// Normalize remainder of vectors
	VectorNormalize(pmove->forward);
	VectorNormalize(pmove->right);
	
	vec3_t wishvel;
	
	// Determine X and Y parts of velocity
	for(int i = 0; i < 2; ++i)
		wishvel[i] = pmove->forward[i] * fmove + pmove->right[i] * smove;
	
	// Zero out Z part of velocity
	wishvel[2] = 0;
	
	vec3_t wishdir;
	
	// Determine magnitude of speed of move
	VectorCopy(wishvel, wishdir);
	
	float wishspeed = VectorNormalize(wishdir);
	
	// Clamp to server-defined max speed
	if(wishspeed > pmove->maxspeed)
	{
		VectorScale(wishvel, pmove->maxspeed / wishspeed, wishvel);
		wishspeed = pmove->maxspeed;
	};
	
	// Set pmove velocity
	pmove->velocity[2] = 0;
	PM_Accelerate(wishdir, wishspeed, pmove->movevars->accelerate);
	pmove->velocity[2] = 0;
	
	// Add in any base velocity to the current velocity
	VectorAdd(pmove->velocity, pmove->basevelocity, pmove->velocity);
	
	float spd = Length(pmove->velocity);
	
	if(spd < 1.0f)
	{
		VectorClear(pmove->velocity);
		return;
	};
	
	// If we are not moving, do nothing
	//if(!pmove->velocity[0] && !pmove->velocity[1] && !pmove->velocity[2])
		//return;
	
	int oldonground = pmove->onground;

	// First try just moving to the destination
	dest[0] = pmove->origin[0] + pmove->velocity[0] * pmove->frametime;
	dest[1] = pmove->origin[1] + pmove->velocity[1] * pmove->frametime;
	dest[2] = pmove->origin[2];

	// First try moving directly to the next spot
	VectorCopy(dest, start);
	trace = pmove->PM_PlayerTrace(pmove->origin, dest, PM_NORMAL, -1);
	
	// If we made it all the way, then copy trace end as new player position
	if(trace.fraction == 1)
	{
		VectorCopy(trace.endpos, pmove->origin);
		return;
	};
	
	// Don't walk up stairs if not on ground
	if(oldonground == -1 && pmove->waterlevel == 0)
		return;
	
	// If we are jumping out of water, don't do anything more
	if(pmove->waterjumptime)
		return;

	// Try sliding forward both on ground and up 16 pixels
	// take the move that goes farthest
	// Save out original pos & velocity
	VectorCopy(pmove->origin, original);
	VectorCopy(pmove->velocity, originalvel);

	// Slide move
	int clip = PM_FlyMove();
	
	// Copy the results out
	VectorCopy(pmove->origin, down);
	VectorCopy(pmove->velocity, downvel);
	
	// Reset original values
	VectorCopy(original, pmove->origin);
	VectorCopy(originalvel, pmove->velocity);

	// Move up a stair height
	VectorCopy(pmove->origin, dest);
	dest[2] += pmove->movevars->stepsize;
	
	trace = pmove->PM_PlayerTrace(pmove->origin, dest, PM_NORMAL, -1);
	
	// If we started ok and made it part of the way at least,
	// copy the results to the movement start position and then run another move try
	if(!trace.startsolid && !trace.allsolid)
		VectorCopy(trace.endpos, pmove->origin);

	// Slide move the rest of the way
	clip = PM_FlyMove();
	
	// Now try going back down from the end point
	
	// Press down the stepheight
	VectorCopy(pmove->origin, dest);
	dest[2] -= pmove->movevars->stepsize;
	
	trace = pmove->PM_PlayerTrace(pmove->origin, dest, PM_NORMAL, -1);
	
	// If we are not on the ground anymore then use the original movement attempt
	if(trace.plane.normal[2] < 0.7)
		goto usedown;
	
	// If the trace ended up in empty space, copy the end over to the origin
	if(!trace.startsolid && !trace.allsolid)
		VectorCopy(trace.endpos, pmove->origin);
	
	// Copy this origin to up
	VectorCopy(pmove->origin, pmove->up);

	// Decide which one went farther
	downdist = (down[0] - original[0]) * (down[0] - original[0]) + (down[1] - original[1]) * (down[1] - original[1]);
	updist = (pmove->up[0] - original[0]) * (pmove->up[0] - original[0]) + (pmove->up[1] - original[1]) * (pmove->up[1] - original[1]);

	if(downdist > updist)
	{
	usedown:
		VectorCopy(down, pmove->origin);
		VectorCopy(downvel, pmove->velocity);
	}
	else // Copy z value from slide move
		pmove->velocity[2] = downvel[2];

	// If at a dead stop, retry the move with nudges to get around lips
};

/*
==================
PM_Friction

Handles both ground friction and water friction
==================
*/
void PM_Friction()
{
	float *vel;
	float speed, newspeed, control;
	float friction;
	float drop;
	vec3_t newvel;
	
	// If we are in water jump cycle, don't apply friction
	if(pmove->waterjumptime)
		return;
	
	// Get velocity
	vel = pmove->velocity;
	
	// Calculate speed
	speed = sqrt(vel[0] * vel[0] + vel[1] * vel[1] + vel[2] * vel[2]);
	
	// If too slow, return
	if(speed < 0.1f)
	{
		//vel[0] = 0;
		//vel[1] = 0;
		return;
	};
	
	// if the leading edge is over a dropoff, increase friction
	/*
	if(pmove->onground != -1)
	{
		start[0] = stop[0] = pmove->origin[0] + vel[0] / speed * 16;
		start[1] = stop[1] = pmove->origin[1] + vel[1] / speed * 16;
		start[2] = pmove->origin[2] + pmove->player_mins[pmove->usehull][2];
		stop[2] = start[2] - 34;

		trace = pmove->PM_PlayerTrace(start, stop, PM_NORMAL, -1);

		if(trace.fraction == 1)
			friction *= 2;
	};
	*/
	
	drop = 0;
	
	// Apply water friction
	//if(pmove->waterlevel)
		//drop += speed * pmove->movevars->waterfriction * waterlevel * pmove->frametime;
	// Apply ground friction
	/*else*/ if(pmove->onground != -1) // On an entity that is the ground
	{
		vec3_t start, stop;
		pmtrace_t trace;
		
		start[0] = stop[0] = pmove->origin[0] + vel[0] / speed * 16;
		start[1] = stop[1] = pmove->origin[1] + vel[1] / speed * 16;
		start[2] = pmove->origin[2] + pmove->player_mins[pmove->usehull][2];
		
		stop[2] = start[2] - 34;
		
		trace = pmove->PM_PlayerTrace(stat, stop, PM_NORMAL, -1);
		
		friction = pmove->movevars->friction;
		
		if(trace.fraction == 1.0)
			friction = pmove->movevars->friction * pmove->movevars->edgefriction;
		
		// Grab friction value
		//friction = pmove->movevars->friction;
		
		// Player friction?
		friction *= pmove->friction;
		
		// Bleed off some speed, but if we have less that the bleed
		// threshold, bleed the threshold amount
		control = (speed < pmove->movevars->stopspeed) ? pmove->movevars->stopspeed : speed;
		
		// Add the amount to the drop amount
		drop += control * friction * pmove->frametime;
	};

	// Scale the velocity
	newspeed = speed - drop;
	if(newspeed < 0)
		newspeed = 0;
	
	// Determine proportion of old speed we are using
	newspeed /= speed;
	
	// Adjust velocity according to proportion
	newvel[0] = vel[0] * newspeed;
	newvel[1] = vel[1] * newspeed;
	newvel[2] = vel[2] * newspeed;
	
	VectorCopy(newvel, pmove->velocity);
};

/*
==================
PM_AirAccelerate
==================
*/
void PM_AirAccelerate(vec3_t wishdir, float wishspeed, float accel)
{
	if(pmove->dead)
		return;
	
	if(pmove->waterjumptime)
		return;
	
	float wishspd = wishspeed;
	
	if(wishspd > 30)
		wishspd = 30;
	
	// Determine veer amount
	float currentspeed = DotProduct(pmove->velocity, wishdir);
	
	// See how much to add
	float addspeed = wishspd - currentspeed;
	
	// If not adding any, done
	if(addspeed <= 0)
		return;
	
	// Determine acceleration speed after acceleration
	float accelspeed = accel * wishspeed * pmove->frametime * pmove->friction;
	
	// Cap it
	if(accelspeed > addspeed)
		accelspeed = addspeed;
	
	// Adjust pmove vel
	for(int i = 0; i < 3; i++)
		pmove->velocity[i] += accelspeed * wishdir[i];
};

/*
===================
PM_WaterMove

===================
*/
void PM_WaterMove()
{
	int i;
	vec3_t wishvel;
	float wishspeed;
	vec3_t wishdir;
	vec3_t start, dest;
	pmtrace_t trace;

	//
	// user intentions
	//
	for(i = 0; i < 3; i++)
		wishvel[i] = pmove->forward[i] * pmove->cmd.forwardmove + pmove->right[i] * pmove->cmd.sidemove;

	if(!pmove->cmd.forwardmove && !pmove->cmd.sidemove && !pmove->cmd.upmove)
		wishvel[2] -= 60; // drift towards bottom
	else
		wishvel[2] += pmove->cmd.upmove;

	VectorCopy(wishvel, wishdir);
	wishspeed = VectorNormalize(wishdir);

	if(wishspeed > pmove->movevars->maxspeed)
	{
		VectorScale(wishvel, pmove->movevars->maxspeed / wishspeed, wishvel);
		wishspeed = pmove->movevars->maxspeed;
	};
	wishspeed *= 0.7;

	//
	// water acceleration
	//
	//if(pmove->waterjumptime)
		//Con_Printf ("wm->%f, %f, %f\n", pmove->velocity[0], pmove->velocity[1], pmove->velocity[2]);
	PM_Accelerate(wishdir, wishspeed, pmove->movevars->wateraccelerate);

	// assume it is a stair or a slope, so press down from stepheight above
	VectorMA(pmove->origin, pmove->frametime, pmove->velocity, dest);
	VectorCopy(dest, start);
	start[2] += pmove->movevars->stepsize + 1;
	trace = pmove->PM_PlayerTrace(start, dest, PM_NORMAL, -1);
	if(!trace.startsolid && !trace.allsolid) // FIXME: check steep slope?
	{                                        // walked up the step
		VectorCopy(trace.endpos, pmove->origin);
		return;
	};

	PM_FlyMove();
	//	if (pmove->waterjumptime)
	//		Con_Printf ("<-wm%f, %f, %f\n", pmove->velocity[0], pmove->velocity[1], pmove->velocity[2]);
};

/*
===================
PM_AirMove

===================
*/
void PM_AirMove()
{
	int i;
	vec3_t wishvel;
	float fmove, smove;
	vec3_t wishdir;
	float wishspeed;
	
	// Copy movement amounts
	fmove = pmove->cmd.forwardmove;
	smove = pmove->cmd.sidemove;
	
	// Zero out the z components of movement vectors
	pmove->forward[2] = 0;
	pmove->right[2] = 0;
	
	// Renormalize
	VectorNormalize(pmove->forward);
	VectorNormalize(pmove->right);
	
	// Determine x and y parts of velocity
	for(i = 0; i < 2; i++)
		wishvel[i] = pmove->forward[i] * fmove + pmove->right[i] * smove;
	
	// Zero out the z part of velocity
	wishvel[2] = 0;
	
	// Determine magnitude of speed of move
	VectorCopy(wishvel, wishdir);
	wishspeed = VectorNormalize(wishdir);

	// Clamp to server defined max speed
	if(wishspeed > pmove->movevars->maxspeed) // TODO: was pmove->maxspeed
	{
		VectorScale(wishvel, pmove->movevars->maxspeed / wishspeed, wishvel); // TODO: was pmove->maxspeed
		wishspeed = pmove->movevars->maxspeed; // TODO: was pmove->maxspeed
	};

	//if(pmove->waterjumptime)
	//	Con_Printf("am->%f, %f, %f\n", pmove->velocity[0], pmove->velocity[1], pmove->velocity[2]);

	// Not on ground, so little effect on velocity
	PM_AirAccelerate(wishdir, wishspeed, pmove->movevars->airaccelerate);

	// Add gravity
	//pmove->velocity[2] -= pmove->movevars->entgravity * pmove->movevars->gravity * pmove->frametime;
	
	// Add in any base velocity to the current velocity value
	VectorAdd(pmove->velocity, pmove->basevelocity, pmove->velocity);
	
	PM_FlyMove();

	//Con_Printf("airmove:vec: %4.2f %4.2f %4.2f\n",
	//			pmove->velocity[0],
	//			pmove->velocity[1],
	//			pmove->velocity[2]);
	//

	//	if (pmove->waterjumptime)
	//		Con_Printf ("<-am%f, %f, %f\n", pmove->velocity[0], pmove->velocity[1], pmove->velocity[2]);
};

/*
==================
PM_InWater
==================
*/
qboolean PM_InWater()
{
	return pmove->waterlevel > 1;
};

/*
==================
PM_CheckWater

Sets pmove->waterlevel and pmove->watertype values
==================
*/
qboolean PM_CheckWater()
{
	// Assume that we are not in water at all
	pmove->waterlevel = 0;
	pmove->watertype = CONTENTS_EMPTY;
	
	vec3_t point;
	
	// Pick a spot just above the players feet
	point[0] = pmove->origin[0] + (pmove->player_mins[pmove->usehull][0] + pmove->player_maxs[pmove->usehull][0]) * 0.5;
	point[1] = pmove->origin[1] + (pmove->player_mins[pmove->usehull][1] + pmove->player_maxs[pmove->usehull][1]) * 0.5;
	point[2] = pmove->origin[2] + pmove->player_mins[pmove->usehull][2] + 1;
	
	// Grab point contents
	int truecont;
	int cont = pmove->PM_PointContents(point, &truecont);
	
	// Are we under water? (not solid and not empty?)
	if(cont <= CONTENTS_WATER && cont > CONTENTS_TRANSLUCENT)
	{
		// Set water type
		pmove->watertype = cont;
		
		// We are at least at level one
		pmove->waterlevel = 1;
		
		float height = pmove->player_mins[pmove->usehull][2] + pmove->player_maxs[pmove->usehull][2];
		float heightover2 = height * 0.5;
		
		// Now check a point that is at the player hull midpoint
		point[2] = pmove->origin[2] + heightover2;
		cont = pmove->PM_PointContents(point, NULL);
		
		// If that point is also under water...
		if(cont <= CONTENTS_WATER && cont > CONTENTS_TRANSLUCENT)
		{
			// Set a higher water level
			pmove->waterlevel = 2;
			
			// Now check the eye position (view_ofs is relative to the origin)
			point[2] = pmove->origin[2] + pmove->view_ofs[2];
			
			cont = pmove->PM_PointContents(point, NULL);
			if(cont <= CONTENTS_WATER && cont > CONTENTS_TRANSLUCENT)
				pmove->waterlevel = 3; // In over our eyes
		};
		
		// Adjust velocity based on water current, if any
		if((truecont <= CONTENTS_CURRENT_0) && (truecont > CONTENTS_CURRENT_DOWN))
		{
			// The deeper we are, the stronger the current
			static vec3_t current_table[] =
			{
				{1, 0, 0}, {0, 1, 0}, {-1, 0, 0},
				{0, -1, 0}, {0, 0, 1}, {0, 0, -1}
			};
			
			VectorMA(pmove->basevelocity, 50.0 * pmove->waterlevel, current_table[CONTENTS_CURRENT_0 - truecont], pmove->basevelocity);
		};
	};
	
	return pmove->waterlevel > 1;
};

/*
=============
PM_CatagorizePosition
=============
*/
void PM_CatagorizePosition()
{
	vec3_t point;
	pmtrace_t tr;

	// if the player hull point one unit down is solid, the player
	// is on ground
	
	// Doing this before we move may introduce a potential latency in water detection
	PM_CheckWater();
	
	// see if standing on something solid
	point[0] = pmove->origin[0];
	point[1] = pmove->origin[1];
	point[2] = pmove->origin[2] - 2;
	
	if(pmove->velocity[2] > 180)
		pmove->onground = -1;
	else
	{
		tr = pmove->PM_PlayerTrace(pmove->origin, point, PM_NORMAL, -1);
		if(tr.plane.normal[2] < 0.7)
			pmove->onground = -1; // too steep
		else
			pmove->onground = tr.ent;
		if(pmove->onground != -1)
		{
			pmove->waterjumptime = 0;
			if(!tr.startsolid && !tr.allsolid)
				VectorCopy(tr.endpos, pmove->origin);
		};

		// standing on an entity other than the world
		if(tr.ent > 0)
		{
			pmove->touchindex[pmove->numtouch].ent = tr.ent;
			pmove->numtouch++;
		};
	};

	//
	// get waterlevel
	//
	pmove->waterlevel = 0;
	pmove->watertype = CONTENTS_EMPTY;

	point[2] = pmove->origin[2] + pmove->player_mins[pmove->usehull][2] + 1;
	int cont = pmove->PM_PointContents(point, NULL);

	if(cont <= CONTENTS_WATER)
	{
		pmove->watertype = cont;
		pmove->waterlevel = 1;
		point[2] = pmove->origin[2] + (pmove->player_mins[pmove->usehull][2] + pmove->player_maxs[pmove->usehull][2]) * 0.5;
		cont = pmove->PM_PointContents(point, NULL);
		if(cont <= CONTENTS_WATER)
		{
			pmove->waterlevel = 2;
			point[2] = pmove->origin[2] + 22;
			cont = pmove->PM_PointContents(point, NULL);
			if(cont <= CONTENTS_WATER)
				pmove->waterlevel = 3;
		};
	};
};

// TODO: unused
/*
==================
PM_GetRandomStuckOffsets

When a player is stuck, it's costly to try and unstick them
Grab a test offset for the player based on a passed in index
==================
*/
void PM_GetRandomStuckOffsets(int nIndex, int server, vec3_t offset)
{
	// Last time we did a full
	int idx = rgStuckLast[nIndex][server];
	++rgStuckLast[nIndex][server];
	
	VectorCopy(rgv3tStuckTable[idx & 54], offset);
	
	return idx & 54;
};

// TODO: unused
/*
==================
PM_ResetStuckOffsets
==================
*/
void PM_ResetStuckOffsets(int nIndex, int server)
{
	rgStuckLast[nIndex][server] = 0;
};

/*
=================
NudgePosition

If pmove->origin is in a solid position,
try nudging slightly on all axis to
allow for the cut precision of the net coordinates
=================
*/
void PM_CheckStuck()
{
	vec3_t base;
	//int x, y, z;
	//int i;
	//static int sign[3] = { 0, -1, 1 };
	pmtrace_t traceresult;

	VectorCopy(pmove->origin, base);

	for(i = 0; i < 3; i++)
		pmove->origin[i] = ((int)(pmove->origin[i] * 8)) * 0.125;
	//	pmove->origin[2] += 0.124;

	//	if (pmove->dead)
	//		return;		// might be a squished point, so don'y bother
	//	if (pmove->PM_TestPlayerPosition (pmove->origin) )
	//		return;

	for(z = 0; z <= 2; z++)
	{
		for(x = 0; x <= 2; x++)
		{
			for(y = 0; y <= 2; y++)
			{
				pmove->origin[0] = base[0] + (sign[x] * 1.0 / 8);
				pmove->origin[1] = base[1] + (sign[y] * 1.0 / 8);
				pmove->origin[2] = base[2] + (sign[z] * 1.0 / 8);
				if(pmove->PM_TestPlayerPosition(pmove->origin, &traceresult))
					return;
			};
		};
	};
	VectorCopy(base, pmove->origin);
	//	Con_DPrintf ("NudgePosition: stuck\n");
};

/*
===============
PM_SpectatorMove
===============
*/
void PM_SpectatorMove()
{
	// This routine keeps track of the spectators position
	// There are two different main move types - track player or move freely (OBS_ROAMING)
	// Doesn't need exact track position, only to generate PVS, so just copy target's position and real view position is calculated on client (saves server CPU)
	
	if(pmove->iuser1 == OBS_ROAMING)
	{
		int i;
		
#ifdef CLIENT_DLL
		// Jump only in roaming mode
		if(iJumpSpectator)
		{
			VectorCopy(vJumpOrigin, pmove->origin);
			VectorCopy(vJumpAngles, pmove->angles);
			VectorCopy(vec3_origin, pmove->velocity);
			
			iJumpSpectator = 0;
			return;
		};
#endif

		// Move around in normal spectator method

		float speed = Length(pmove->velocity);
		if(speed < 1)
			VectorCopy(vec3_origin, pmove->velocity)
		else
		{
			float drop = 0;

			float friction = pmove->movevars->friction * 1.5; // extra friction
			float control = speed < pmove->movevars->stopspeed ? pmove->movevars->stopspeed : speed;
			drop += control * friction * pmove->frametime;

			// scale the velocity
			float newspeed = speed - drop;
			if(newspeed < 0)
				newspeed = 0;
			newspeed /= speed;

			VectorScale(pmove->velocity, newspeed, pmove->velocity);
		};

		// accelerate
		float fmove = pmove->cmd.forwardmove;
		float smove = pmove->cmd.sidemove;

		VectorNormalize(pmove->forward);
		VectorNormalize(pmove->right);
		
		vec3_t wishvel;
		
		for(i = 0; i < 3; i++)
			wishvel[i] = pmove->forward[i] * fmove + pmove->right[i] * smove;
		
		wishvel[2] += pmove->cmd.upmove;
		
		vec3_t wishdir;
		
		VectorCopy(wishvel, wishdir);
		float wishspeed = VectorNormalize(wishdir);

		//
		// clamp to server defined max speed
		//
		if(wishspeed > pmove->movevars->spectatormaxspeed)
		{
			VectorScale(wishvel, pmove->movevars->spectatormaxspeed / wishspeed, wishvel);
			wishspeed = pmove->movevars->spectatormaxspeed;
		};

		float currentspeed = DotProduct(pmove->velocity, wishdir);
		float addspeed = wishspeed - currentspeed;
		if(addspeed <= 0)
			return;
		
		float accelspeed = pmove->movevars->accelerate * pmove->frametime * wishspeed;
		if(accelspeed > addspeed)
			accelspeed = addspeed;

		for(i = 0; i < 3; i++)
			pmove->velocity[i] += accelspeed * wishdir[i];

		// move
		VectorMA(pmove->origin, pmove->frametime, pmove->velocity, pmove->origin);
	}
	else
	{
		// All other modes just tract some kind of target, so spectator PVS = target PVS
		
		int target;
		
		// No valid target?
		if(pmove->iuser2 <= 0)
			return;
		
		// Find the client this player is targeting
		for(target = 0; target < pmove->numphysent; ++target)
			if(pmove->physents[target].info == pmove->iuser2)
				break;
		
		if(target == pmove->numphysent)
			return;
		
		// Use target's position as own origin for PVS
		VectorCopy(pmove->physents[target].angles, pmove->angles);
		VectorCopy(pmove->physents[target].origin, pmove->origin);
		
		// No velocity
		VectorCopy(vec3_origin, pmove->velocity);
	};
};

// TODO: unused
/*
==================
PM_SplineFraction

Use for ease-int, ease-out style interpolation (accel/decel)
Used by ducking code
==================
*/
float PM_SplineFraction(float value, float scale)
{
	value = value * scale;
	
	float fValueSquared{value * value};
	
	// Nice little ease-in, ease-out spline-like curve
	return 3 * fValueSquared - 2 * fValueSquared * value;
};

// TODO: unused
/*
==================
PM_FixPlayerCrouchStuck
==================
*/
void PM_FixPlayerCrouchStuck(int direction)
{
	int hitent = pmove->PM_TestPlayerPosition(pmove->origin, NULL);
	if(hitent == -1)
		return;
	
	vec3_t test;
	VectorCopy(pmove->origin, test);
	
	for(int i = 0; i < 36; ++i)
	{
		pmove->origin[2] += direction;
		hitent = pmove->PM_TestPlayerPosition(pmove->origin, NULL);
		if(hitent == -1)
			return;
	};
	
	// Failed
	VectorCopy(test, pmove->origin);
};

// TODO: unused
/*
==================
PM_UnDuck
==================
*/
void PM_UnDuck()
{
	int i;
	
	vec3_t vNewOrigin;
	VectorCopy(pmove->origin, vNewOrigin);
	
	if(pmove->onground != -1)
		for(i = 0; i < 3; ++i)
			vNewOrigin[i] += (pmove->player_mins[1][i] - pmove->player_mins[0][i]);
	
	pmtrace_t trace = pmove->PM_PlayerTrace(vNewOrigin, vNewOrigin, PM_NORMAL, -1);
	
	if(!trace.startsolid)
	{
		pmove->usehull = 0;
		
		// Oh, no, changing hulls stuck us into something! Try unsticking downward first
		trace = pmove->PM_PlayerTrace(vNewOrigin, vNewOrigin, PM_NORMAL, -1);
		
		if(trace.startsolid)
		{
			// See if we are stuck
			// If so, stay ducked with the duck hull until we have a clear spot
			//Con_Printf("unstick got stuck\n");
			pmove->usehull = 1;
			return;
		};
		
		pmove->flags &= ~FL_DUCKING;
		pmove->bInDuck = false;
		pmove->view_ofs[2] = VEC_VIEW;
		pmove->flDuckTime = 0;
		
		VectorCopy(vNewOrigin, pmove->origin);
		
		// Recatagorize position since ducking can change origin
		PM_CatagorizePosition();
	};
};

/*
==================
PM_Duck
==================
*/
void PM_Duck()
{
	// TODO
};

/*
================
PM_CheckLadderMove

  Checks to see if we are on a ladder
================
*/
// TODO
/*/////////////////////////////
qboolean ladderforward;
vec3_t laddervec;

void PM_CheckLadderMove()
{
	vec3_t spot;
	vec3_t flatforward;
	trace_t trace;
	float tracedist;
	#define TRACE_LADDER_DIST   48.0
	qboolean wasOnLadder;

	if ( pm->ps->pm_time )
		return;

	//if (pmove->dead)
	//	return;

	if ( pml.walking )
		tracedist = 1.0;
	else
		tracedist = TRACE_LADDER_DIST;

	wasOnLadder = ( ( pm->ps->pm_flags & PMF_LADDER ) != 0 );

	pml.ladder = false;
	pm->ps->pm_flags &= ~PMF_LADDER;    // clear ladder bit
	ladderforward = false;
*////////////////////////////////
	/*
	if (pm->ps->eFlags & EF_DEAD) {	// dead bodies should fall down ladders
		return;
	}

	if (pmove->dead && pmove->stats[STAT_HEALTH] <= 0)
	{
		return;
	}
	*/
/*///////////////////////////////
	if ( pmove->stats[STAT_HEALTH] <= 0 )
	{
		pm->ps->groundEntityNum = ENTITYNUM_NONE;
		pml.groundPlane = false;
		pml.walking = false;
		return;
	}

	// check for ladder
	flatforward[0] = pml.forward[0];
	flatforward[1] = pml.forward[1];
	flatforward[2] = 0;
	VectorNormalize( flatforward );

	VectorMA( pm->ps->origin, tracedist, flatforward, spot );
	pm->trace( &trace, pm->ps->origin, pm->mins, pm->maxs, spot, pm->ps->clientNum, pm->tracemask );
	if ( ( trace.fraction < 1 ) && ( trace.surfaceFlags & SURF_LADDER ) )
		pml.ladder = true;
*////////////////////////////////
/*
	if (!pml.ladder && DotProduct(pm->ps->velocity, pml.forward) < 0) {
		// trace along the negative velocity, so we grab onto a ladder if we are trying to reverse onto it from above the ladder
		flatforward[0] = -pm->ps->velocity[0];
		flatforward[1] = -pm->ps->velocity[1];
		flatforward[2] = 0;
		VectorNormalize (flatforward);

		VectorMA (pm->ps->origin, tracedist, flatforward, spot);
		pm->trace (&trace, pm->ps->origin, pm->mins, pm->maxs, spot, pm->ps->clientNum, pm->tracemask);
		if ((trace.fraction < 1) && (trace.surfaceFlags & SURF_LADDER))
		{
			pml.ladder = true;
		}
	}
*/
/*///////////////////////////////////
	if ( pml.ladder )
		VectorCopy( trace.plane.normal, laddervec );

	if ( pml.ladder && !pml.walking && ( trace.fraction * tracedist > 1.0 ) )
	{
		vec3_t mins;
		// if we are only just on the ladder, don't do this yet, or it may throw us back off the ladder
		pml.ladder = false;
		VectorCopy( pm->mins, mins );
		mins[2] = -1;
		VectorMA( pm->ps->origin, -tracedist, laddervec, spot );
		pm->trace( &trace, pm->ps->origin, mins, pm->maxs, spot, pm->ps->clientNum, pm->tracemask );
		if ( ( trace.fraction < 1 ) && ( trace.surfaceFlags & SURF_LADDER ) )
		{
			// if AI, then be more stringent on their viewangles
			if ( pm->ps->aiChar && ( DotProduct( trace.plane.normal, pml.forward ) > -0.9 ) )
				pml.ladder = false;
			else
			{
				ladderforward = true;
				pml.ladder = true;
				pm->ps->pm_flags |= PMF_LADDER; // set ladder bit
			}
		}
		else
			pml.ladder = qfalse;
	}
	else if ( pml.ladder )
		pm->ps->pm_flags |= PMF_LADDER; // set ladder bit

	// create some up/down velocity if touching ladder
	if ( pml.ladder )
	{
		if ( pml.walking )
		{
			// we are currently on the ground, only go up and prevent X/Y if we are pushing forwards
			if ( pm->cmd.forwardmove <= 0 )
				pml.ladder = false;
		}
	}

	// if we have just dismounted the ladder at the top, play dismount
	if ( !pml.ladder && wasOnLadder && pm->ps->velocity[2] > 0 )
		BG_AnimScriptEvent( pm->ps, ANIM_ET_CLIMB_DISMOUNT, false, false );

	// if we have just mounted the ladder
	if ( pml.ladder && !wasOnLadder && pm->ps->velocity[2] < 0 )    // only play anim if going down ladder
		BG_AnimScriptEvent( pm->ps, ANIM_ET_CLIMB_MOUNT, false, false );
}
*////////////////////////////////

/*
============
PM_LadderMove
============
*/
void PM_LadderMove(physent_t *pLadder)
{
	// TODO
/*
	float wishspeed, scale;
	vec3_t wishdir, wishvel;
	float upscale;

	if ( ladderforward )
	{
		// move towards the ladder
		VectorScale( laddervec, -200.0, wishvel );
		pmove->velocity[0] = wishvel[0];
		pmove->velocity[1] = wishvel[1];
	}

	upscale = ( pmove->forward[2] + 0.5 ) * 2.5;
	if ( upscale > 1.0 )
		upscale = 1.0;
	else if ( upscale < -1.0 )
		upscale = -1.0;

	// forward/right should be horizontal only
	pmove->forward[2] = 0;
	pmove->right[2] = 0;
	VectorNormalize( pmove->forward );
	VectorNormalize( pmove->right );

	// move depending on the view, if view is straight forward, then go up
	// if view is down more then X degrees, start going down
	// if they are back pedalling, then go in reverse of above
	scale = PM_CmdScale( &pmove->cmd );
	VectorClear( wishvel );

	if ( pmove->cmd.forwardmove )
	{
		if ( pmove->aiChar )
			wishvel[2] = 0.5 * upscale * scale * (float)pmove->cmd.forwardmove;
		else // player speed
			wishvel[2] = 0.9 * upscale * scale * (float)pmove->cmd.forwardmove;
	}
//Con_Printf("wishvel[2] = %i, fwdmove = %i\n", (int)wishvel[2], (int)pmove->cmd.forwardmove );

	if ( pmove->cmd.sidemove )
	{
		// strafe, so we can jump off ladder
		vec3_t ladder_right, ang;
		vectoangles( laddervec, ang );
		AngleVectors( ang, NULL, ladder_right, NULL );

		// if we are looking away from the ladder, reverse the right vector
		if ( DotProduct( laddervec, pmove->forward ) > 0 )
			VectorInverse( ladder_right );

		VectorMA( wishvel, 0.5 * scale * (float)pmove->cmd.sidemove, pmove->right, wishvel );
	}

	// do strafe friction
	PM_Friction();

	wishspeed = VectorNormalize2( wishvel, wishdir );

	PM_Accelerate( wishdir, wishspeed, pm_accelerate );
	if ( !wishvel[2] ) {
		if ( pmove->velocity[2] > 0 )
		{
			pmove->velocity[2] -= pmove->gravity * pml.frametime;
			if ( pmove->velocity[2] < 0 )
				pmove->velocity[2]  = 0;
		}
		else
		{
			pmove->velocity[2] += pmove->gravity * pml.frametime;
			if ( pmove->velocity[2] > 0 )
				pmove->velocity[2]  = 0;
		}
	}

	//Con_Printf("vel[2] = %i\n", (int)pm->ps->velocity[2] );

	PM_StepSlideMove( false );  // no gravity while going up ladder

	// always point legs forward
	pmove->movementDir = 0;
*/
};

/*
==================
PM_Ladder
==================
*/
physent_t *PM_Ladder()
{
	physent_t *pe;
	hull_t *hull;
	int num;
	vec3_t vTest;
	
	for(int i = 0; i < pmove->nummoveent; ++i)
	{
		pe = &pmove->moveents[i];
		
		if(pe->model && (modtype_t)pmove->PM_GetModelType(pe->model) == mod_brush && pe->skin == CONTENTS_LADDER)
		{
			hull = (hull_t*)pmove->PM_HullForBsp(pe, vTest);
			num = hull->firstclipnode;
			
			// Offset the test appropriately for this hull
			VectorSubtract(pmove->origin, vTest, vTest);
			
			// Test the player's hull for intersection with this model
			if(pmove->PM_HullPointContents(hull, num, vTest) == CONTENTS_EMPTY)
				continue;
			
			return pe;
		};
	};
	
	return NULL;
};

/*
==================
PM_WaterJump
==================
*/
void PM_WaterJump()
{
	if(pmove->waterjumptime > 10000)
		pmove->waterjumptime = 10000;
	
	if(!pmove->waterjumptime)
		return;
	
	pmove->waterjumptime -= pmove->cmd.msec;
	
	if(pmove->waterjumptime < 0 || !pmove->waterlevel) // TODO: was sv.time instead of 0; probably should be pmove->time
	{
		pmove->flags &= ~FL_WATERJUMP;
		pmove->waterjumptime = 0;
	};
	
	pmove->velocity[0] = pmove->movedir[0];
	pmove->velocity[1] = pmove->movedir[1];
};

// TODO: unused
/*
==================
PM_AddGravity
==================
*/
void PM_AddGravity()
{
	float ent_gravity{1.0f};
	
	if(pmove->gravity)
		ent_gravity = pmove->gravity;
	
	// Add gravity incorrectly
	pmove->velocity[2] -= (ent_gravity * pmove->movevars->gravity * pmove->frametime);
	pmove->velocity[2] += pmove->basevelocity[2] * pmove->frametime;
	pmove->basevelocity[2] = 0;
	
	PM_CheckVelocity();
};

// TODO: unused
/*
==================
PM_PushEntity

Doesn't change the entities' velocity at all
==================
*/
pmtrace_t PM_PushEntity(vec3_t push)
{
	vec3_t end;
	VectorAdd(pmove->origin, push, end);
	
	pmtrace_t trace = pmove->PM_PlayerTrace(pmove->origin, end, PM_NORMAL, -1);
	
	VectorCopy(trace.endpos, pmove->origin);
	
	// So we can run impact function afterwards
	if(trace.fraction < 1.0 && !trace.allsolid)
		PM_AddToTouched(trace, pmove->velocity);
	
	return trace;
};

/*
==================
PM_Physics_Toss

Dead player flying through air, e.g.
==================
*/
void PM_Physics_Toss()
{
	PM_CheckWater();
	
	if(pmove->velocity[2] > 0)
		pmove->onground = -1;
	
	if()
	{
		if()
			return;
	};
	
	PM_CheckVelocity();
	
	if()
		PM_AddGravity();
	
	VectorAdd();
	
	PM_CheckVelocity();
	
	VectorScale();
	VectorSubtract();
	
	trace = PM_PushEntity(move);
	
	PM_CheckVelocity();
	
	if(trace.allsolid)
	{
		pmove->onground = trace.ent;
		VectorCopy();
		return;
	};
	
	if(trace.fraction == 1)
	{
		PM_CheckWater();
		return;
	};
	
	float backoff = 1.0f;
	
	if(pmove->movetype = MOVETYPE_BOUNCE)
		backoff = 2.0f - pmove->friction;
	else if(pmove->movetype == MOVETYPE_BOUNCEMISSILE)
		backoff = 2.0f;
	
	PM_ClipVelocity(pmove->velocity, trace.plane.normal, pmove->velocity, backoff);
	
	// Stop if on ground
	if(trace.plane.normal[2] > 0.7)
	{
	};
	
	// Check for in water
	PM_CheckWater();
};

/*
==================
PM_NoClip
==================
*/
void PM_NoClip()
{
	// Copy movement amounts
	float fmove = pmove->cmd.forwardmove;
	float smove = pmove->cmd.sidemove;
	
	VectorNormalize(pmove->forward);
	VectorNormalize(pmove->right);
	
	vec3_t wishvel;
	
	// Determine x and y parts of velocity
	for(int i = 0; i < 3; ++i)
		wishvel[i] = pmove->forward[i] * fmove + pmove->right[i] * smove;
	
	wishvel[2] += pmove->cmd.upmove;
	
	VectorMA(pmove->origin, pmove->frametime, wishvel, pmove->origin);
	
	// Zero out the velocity so that we don't accumulate a huge downward velocity from gravity and such
	VectorClear(pmove->velocity);
};

/*
==================
PM_PreventMegaBunnyJumping
==================
*/
void PM_PreventMegaBunnyJumping()
{
	// TODO
};

/*
=============
JumpButton
=============
*/
void PM_Jump()
{
	if(pmove->dead)
	{
		pmove->oldbuttons |= IN_JUMP; // don't jump again until released
		return;
	};

	if(pmove->waterjumptime)
	{
		pmove->waterjumptime -= pmove->frametime;
		if(pmove->waterjumptime < 0)
			pmove->waterjumptime = 0;
		return;
	};

	if(pmove->waterlevel >= 2)
	{ // swimming, not jumping
		pmove->onground = -1;

		if(pmove->watertype == CONTENTS_WATER)
			pmove->velocity[2] = 100;
		else if(pmove->watertype == CONTENTS_SLIME)
			pmove->velocity[2] = 80;
		else
			pmove->velocity[2] = 50;
		return;
	};

	if(pmove->onground == -1)
		return; // in air, so no effect

	if(pmove->oldbuttons & IN_JUMP)
		return; // don't pogo stick

	pmove->onground = -1;
	
	PM_PlayStepSound(0, 1.0);
	
	pmove->velocity[2] += 270;

	pmove->oldbuttons |= IN_JUMP; // don't jump again until released
};

/*
=============
PM_CheckWaterJump
=============
*/
void PM_CheckWaterJump()
{
	vec3_t spot;
	int cont;
	vec3_t flatforward;

	if(pmove->waterjumptime)
		return;

	// ZOID, don't hop out if we just jumped in
	if(pmove->velocity[2] < -180)
		return; // only hop out if we are moving up

	// see if near an edge
	flatforward[0] = pmove->forward[0];
	flatforward[1] = pmove->forward[1];
	flatforward[2] = 0;
	VectorNormalize(flatforward);

	VectorMA(pmove->origin, 24, flatforward, spot);
	spot[2] += 8;
	cont = pmove->PM_PointContents(spot, NULL);
	if(cont != CONTENTS_SOLID)
		return;
	spot[2] += 24;
	cont = pmove->PM_PointContents(spot, NULL);
	if(cont != CONTENTS_EMPTY)
		return;
	// jump out of water
	VectorScale(flatforward, 50, pmove->velocity);
	pmove->velocity[2] = 310;
	pmove->waterjumptime = 2;         // safety net
	pmove->oldbuttons |= IN_JUMP; // don't jump again until released
};

// TODO: unused
/*
==================
PM_CheckFalling
==================
*/
void PM_CheckFalling()
{
	if(pmove->onground != -1 && !pmove->dead && pmove->flFallVelocity >= PLAYER_FALL_PUNCH_THRESHOLD)
	{
		float fvol{0.5f};
		
		if(pmove->waterlevel > 0)
		{
		}
		else if()
		{
		}
		else if()
		{
			fvol = 0.85f;
		}
		else if(pmove->flFallVelocity < PLAYER_MIN_BOUNCE_SPEED)
			fvol = 0.0f;
	};
	
	if(pmove->onground != -1)
		pmove->flFallVelocity = 0;
};

// TODO: unused
/*
==================
PM_PlayWaterSounds
==================
*/
void PM_PlayWaterSounds()
{
	// Did we enter or leave water?
	if((pmove->oldwaterlevel == 0 && pmove->waterlevel != 0) || 
	   (pmove->oldwaterlevel != 0 && pmove->waterlevel == 0))
	{
		const char *sSampleToUse{""};
		
		// TODO: va()
		
		switch(pmove->RandomLong(0, 3))
		{
		case 0:
			sSampleToUse = "player/pl_wade1.wav";
			break;
		case 1:
			sSampleToUse = "player/pl_wade2.wav";
			break;
		case 2:
			sSampleToUse = "player/pl_wade3.wav";
			break;
		case 3:
			sSampleToUse = "player/pl_wade4.wav";
			break;
		};
		
		pmove->PM_PlaySound(CHAN_BODY, sSampleToUse, 1, ATTN_NORM, 0, PITCH_NORM);
	};
};

/*
==================
PM_CalcRoll
==================
*/
float PM_CalcRoll(vec3_t angles, vec3_t velocity, float rollangle, float rollspeed)
{
	vec3_t forward, right, up;
	AngleVectors(angles, forward, right, up);
	
	float side = DotProduct(velocity, right);
	float sign = (side < 0) ? -1 : 1;
	
	side = fabs(side);
	
	side = rollangle;
	
	if(side < rollspeed)
		side = side * rollangle / rollspeed;
	
	return side * sign;
};

/*
==================
PM_DropPunchAngle
==================
*/
void PM_DropPunchAngle(vec3_t punchangle)
{
	float len{VectorNormalize(punchangle)};
	len -= (10.0 + len * 0.5) * pmove->frametime;
	len = max(len, 0.0f);
	VectorScale(punchangle, len, punchangle);
};

/*
==================
PM_CheckParameters
==================
*/
void PM_CheckParameters()
{
	float spd;
	float maxspeed;
	vec3_t v_angle;
	
	spd = (pmove->cmd.forwardmove * pmove->cmd.forwardmove) + 
		  (pmove->cmd.sidemove * pmove->cmd.sidemove) + 
		  (pmove->cmd.upmove * pmove->cmd.upmove);
	spd = sqrt(spd);
	
	maxspeed = pmove->clientmaxspeed;
	
	if(maxspeed != 0.0)
		pmove->maxspeed = min(maxspeed, pmove->maxspeed);
	
	if(spd != 0.0 && spd > pmove->maxspeed)
	{
		float fRatio = pmove->maxspeed / spd;
		pmove->cmd.forwardmove *= fRatio;
		pmove->cmd.sidemove *= fRatio;
		pmove->cmd.upmove *= fRatio;
	};
	
	if(pmove->flags & FL_FROZEN || pmove->flags & FL_ONTRAIN || pmove->dead)
	{
		pmove->cmd.forwardmove = 0;
		pmove->cmd.sidemove = 0;
		pmove->cmd.upmove = 0;
	};
	
	PM_DropPunchAngle(pmove->punchangle);
	
	// Take angles from command
	if(!pmove->dead)
	{
		VectorCopy(pmove->cmd.viewangles, v_angle);
		VectorAdd(v_angle, pmove->punchangle, v_angle);
		
		// Set up view angles
		pmove->angles[PITCH] = v_angle[PITCH];
		pmove->angles[YAW] = v_angle[YAW];
		pmove->angles[ROLL] = PM_CalcRoll(v_angle, pmove->velocity, pmove->movevars->rollangle, pmove->movevars->rollspeed) * 4;
	}
	else
		VectorCopy(pmove->oldangles, pmove->angles);
	
	// Set dead player view offset
	if(pmove->dead)
		pmove->view_ofs[2] = PM_DEAD_VIEWHEIGHT;
	
	// Adjust client view angles to match values used on server
	if(pmove->angles[YAW] > 180.0f)
		pmove->angles[YAW] -= 360.0f;
};

/*
==================
PM_ReduceTimers
==================
*/
void PM_ReduceTimers()
{
	if(pmove->flTimeStepSound > 0)
	{
		pmove->flTimeStepSound -= pmove->cmd.msec;
		
		if(pmove->flTimeStepSound < 0)
			pmove->flTimeStepSound = 0;
	};
	
	if(pmove->flDuckTime > 0)
	{
		pmove->flDuckTime -= pmove->cmd.msec;
		
		if(pmove->flDuckTime < 0)
			pmove->flDuckTime = 0;
	};
	
	if(pmove->flSwimTime > 0)
	{
		pmove->flSwimTime -= pmove->cmd.msec;
		
		if(pmove->flSwimTime < 0)
			pmove->flDuckTime = 0;
	};
};

/*
=============
PlayerMove

Returns with origin, angles, and velocity modified in place.

Numtouch and touchindex[] will be set if any of the physents
were contacted during the move.
=============
*/
void PM_PlayerMove(qboolean server)
{
	// Are we running server code?
	pmove->server = server;
	
	// Adjust speeds, etc
	PM_CheckParameters();
	
	// Assume we don't touch anything
	pmove->numtouch = 0;
	
	// Number of msec to apply movement
	pmove->frametime = pmove->cmd.msec * 0.001;
	
	PM_ReduceTimers();
	
	// Convert view angles into vectors
	AngleVectors(pmove->angles, pmove->forward, pmove->right, pmove->up); // TODO: pmove->angles = sv_player->v.angles/sv_player->v.v_angle
	
	//PM_ShowClipBox();
	
	// Special handling for spectator and observers (iuser1 is set if the player's in observer mode)
	if(pmove->spectator || pmove->iuser1 > 0)
	{
		PM_SpectatorMove();
		PM_CatagorizePosition();
		return;
	};
	
	// Always try and unstick us unless we are in noclip mode
	if(pmove->movetype != MOVETYPE_NOCLIP && pmove->movetype != MOVETYPE_NONE)
		if(PM_CheckStuck())
			return; // Can't move, we're stuck

	// take angles directly from command
	//VectorCopy(pmove->cmd.viewangles, pmove->angles);

	// Now that we're "unstuck", see where we are (set onground, watertype, and waterlevel)
	PM_CatagorizePosition();

	//PM_CheckLadderMove(); // TODO
	
	// Store off the starting water level
	pmove->oldwaterlevel = pmove->waterlevel;
	
	// If we are not on ground, store off how fast we are moving down
	if(pmove->onground == -1)
		pmove->flFallVelocity = -pmove->velocity[2];
	
	g_onladder = 0;
	
	physent_t *pLadder = NULL;
	
	// Don't run ladder code if dead or on a train
	if(!pmove->dead && !(pmove->flags & FL_ONTRAIN))
	{
		pLadder = PM_Ladder();
		if(pLadder)
			g_onladder = 1;
	};
	
	PM_UpdateStepSound();
	
	PM_Duck();
	
	// Don't run ladder code if dead or on a train
	if(!pmove->dead && !(pmove->flags & FL_ONTRAIN))
	{
		if(pLadder) // TODO: (pml.ladder) in q2/3?
			PM_LadderMove(pLadder);
		else if(pmove->movetype != MOVETYPE_WALK &&
				pmove->movetype != MOVETYPE_NOCLIP)
		{
			// Clear ladder stuff unless player is noclipping
			// It will be set immediately again next frame if necessary
			pmove->movetype = MOVETYPE_WALK;
		};
	};
	
#if !defined(_TFC)
	// Slow down, I'm pulling it! (But only when I'm standing on ground)
	if((pmove->onground != -1) && (pmove->cmd.buttons & IN_USE))
		VectorScale(pmove->velocity, 0.3, pmove->velocity);
#endif
	
	// Handle movement
	switch(pmove->movetype)
	{
	default:
		pmove->Con_DPrintf("Bogus pmove player movetype %d on (%d) 0=cl 1=sv\n", pmove->movetype, pmove->server);
		break;
	case MOVETYPE_NONE:
		break;
	case MOVETYPE_NOCLIP:
		PM_NoClip();
		break;
	case MOVETYPE_TOSS:
	case MOVETYPE_BOUNCE:
		PM_Physics_Toss();
		break;
	case MOVETYPE_FLY:
		PM_CheckWater();
		
		// Was the jump button pressed?
		// If so, set velocity to 270 away from ladder (This is currently wrong)
		// Also, set the move type to walk, too
		
		if(pmove->cmd.buttons & IN_JUMP)
			if(!pLadder)
				PM_Jump();
		else
			pmove->oldbuttons &= ~IN_JUMP;
		
		// Perform the move accouting for any base velocity
		VectorAdd(pmove->velocity, pmove->basevelocity, pmove->velocity);
		PM_FlyMove();
		VectorSubtract(pmove->velocity, pmove->basevelocity, pmove->velocity);
		break;
	case MOVETYPE_WALK:
		if(!PM_InWater())
			PM_AddCorrectGravity();
		
		// If we're leaping out of the water, just update the counters
		if(pmove->waterjumptime)
		{
			PM_WaterJump();
			PM_FlyMove();
			
			// Make sure water level is set correctly
			PM_CheckWater();
			return;
		};
		
		// If we're swimming in the water, see if we are nudging against a place we can jump up out of, and, if so, start out jump. Otherwise, if we are not moving up, reset jump timer to 0
		if(pmove->waterlevel >= 2)
		{
			if(pmove->waterlevel == 2)
				PM_CheckWaterJump();
			
			// If we're falling again, we must not try to jump out of the water anymore
			if(pmove->velocity[2] < 0 && pmove->waterjumptime)
				pmove->waterjumptime = 0;
			
			// Was jump button pressed?
			if(pmove->cmd.buttons & IN_JUMP)
				PM_Jump();
			else
				pmove->oldbuttons &= ~IN_JUMP;
			
			// Perform regular water movement
			PM_WaterMove();
			
			VectorSubtract(pmove->velocity, pmove->basevelocity, pmove->velocity);
			
			// Set onground, watertype, and waterlevel for final spot
			PM_CatagorizePosition();
		}
		else // Not underwater
		{
			// Was jump button pressed?
			if(pmove->cmd.buttons & IN_JUMP)
				if(!pLadder)
					PM_Jump();
			else
				pmove->oldbuttons &= ~IN_JUMP;
			
			// Friction is handled before we add in any base velocity
			// That way, if we are on a conveyour, we don't slowdown when standing still, relative to the conveyor
			if(pmove->onground != -1)
			{
				pmove->velocity[2] = 0.0;
				PM_Friction();
			};
			
			// Make sure velocity is valid
			PM_CheckVelocity();
			
			// Are we on ground now?
			if(pmove->onground != -1)
			{
				//PM_Accelerate(wishdir, wishspeed, pmove->movevars->accelerate);
				//pmove->velocity[2] -= pmove->movevars->entgravity * pmove->movevars->gravity * pmove->frametime;
				PM_WalkMove();
			}
			else
				PM_AirMove(); // Take the air movement into account
			
			// Set onground, watertype, and waterlevel for final spot
			PM_CatagorizePosition();
			
			// Now pull the base velocity back out
			// Base velocity is set if you are on a moving object, like a conveyor (or maybe another monster)
			VectorSubtract(pmove->velocity, pmove->basevelocity, pmove->velocity);
			
			// Make sure velocity is valid
			PM_CheckVelocity();
			
			// Add any remaining gravitational component
			if(!PM_InWater())
				PM_FixupGravityVelocity();
			
			// If we are on ground, no downward velocity
			if(pmove->onground != -1)
				pmove->velocity[2] = 0;
			
			// See if we landed on the ground with enough force to play a landing sound
			PM_CheckFalling();
		};
		
		// Did we enter or leave the water?
		PM_PlayWaterSounds();
		
		break;
	};
};

/*
==================
PM_CreateStuckTable
==================
*/
void PM_CreateStuckTable()
{
	// TODO
};

/*
==================
PM_Move
==================
*/
void PM_Move(struct playermove_s *ppmove, int server)
{
	assert(pm_shared_initialized);
	
	pmove = ppmove;
	
	PM_PlayerMove(server);
	
	if(pmove->onground != -1)
		pmove->flags |= FL_ONGROUND;
	else
		pmove->flags &= ~FL_ONGROUND;
	
	// In singleplayer, reset friction after each movement to FrictionModifier Triggers work still
	if(!pmove->multiplayer && pmove->movetype == MOVETYPE_WALK)
		pmove->friction = 1.0f;
};

/*
==================
PM_Init
==================
*/
void PM_Init(struct playermove_s *ppmove)
{
	assert(!pm_shared_initialized);
	
	pmove = ppmove;
	
	PM_CreateStuckTable();
	PM_InitTextureTypes();
	
	pm_shared_initialized = true;
};