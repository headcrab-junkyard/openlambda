/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018, 2020-2022 BlackPhrase
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

// TODO: unused
/*
==================
PM_InitTextureTypes
==================
*/
void PM_InitTextureTypes()
{
	// TODO
	
	nFilePos = 0;
	
	while()
	{
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
	const char *sTextureName;
	
	// TODO
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
	
	PM_CatagorizeTextureType();
};

// TODO: unused
/*
==================
PM_AddToTouched
==================
*/
qboolean PM_AddToTouched(pmtrace_t trace, vec3_t impactvelocity)
{
	// TODO
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
	// TODO
};

/*
==================
PM_ClipVelocity

Slide off of the impacting object
returns the blocked flags (1 = floor, 2 = step / wall)
==================
*/
int PM_ClipVelocity(vec3_t in, vec3_t normal, vec3_t out, float overbounce)
{
	float backoff;
	float change;
	int i, blocked;
	
	float angle = normal[2];
	
	blocked = 0;
	if(normal[2] > 0)
		blocked |= 1; // floor
	if(!normal[2])
		blocked |= 2; // step

	backoff = DotProduct(in, normal) * overbounce;

	for(i = 0; i < 3; i++)
	{
		change = normal[i] * backoff;
		out[i] = in[i] - change;
		if(out[i] > -STOP_EPSILON && out[i] < STOP_EPSILON)
			out[i] = 0;
	};
	
	// Return blocking flags
	return blocked;
};

// TODO: unused
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
	int i, j;
	pmtrace_t trace;
	vec3_t end;
	float time_left;
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
PM_GroundMove

Player is on ground, with no upwards velocity
=============
*/
void PM_WalkMove()
{
	vec3_t start, dest;
	pmtrace_t trace;
	vec3_t original, originalvel, down, up, downvel;
	float downdist, updist;

	pmove->velocity[2] = 0;
	if(!pmove->velocity[0] && !pmove->velocity[1] && !pmove->velocity[2])
		return;

	// first try just moving to the destination
	dest[0] = pmove->origin[0] + pmove->velocity[0] * pmove->frametime;
	dest[1] = pmove->origin[1] + pmove->velocity[1] * pmove->frametime;
	dest[2] = pmove->origin[2];

	// first try moving directly to the next spot
	VectorCopy(dest, start);
	trace = pmove->PM_PlayerTrace(pmove->origin, dest, PM_NORMAL, -1);
	if(trace.fraction == 1)
	{
		VectorCopy(trace.endpos, pmove->origin);
		return;
	};

	// try sliding forward both on ground and up 16 pixels
	// take the move that goes farthest
	VectorCopy(pmove->origin, original);
	VectorCopy(pmove->velocity, originalvel);

	// slide move
	PM_FlyMove();

	VectorCopy(pmove->origin, down);
	VectorCopy(pmove->velocity, downvel);

	VectorCopy(original, pmove->origin);
	VectorCopy(originalvel, pmove->velocity);

	// move up a stair height
	VectorCopy(pmove->origin, dest);
	dest[2] += pmove->movevars->stepsize;
	trace = pmove->PM_PlayerTrace(pmove->origin, dest, PM_NORMAL, -1);
	if(!trace.startsolid && !trace.allsolid)
		VectorCopy(trace.endpos, pmove->origin);

	// slide move
	PM_FlyMove();

	// press down the stepheight
	VectorCopy(pmove->origin, dest);
	dest[2] -= pmove->movevars->stepsize;
	trace = pmove->PM_PlayerTrace(pmove->origin, dest, PM_NORMAL, -1);
	if(trace.plane.normal[2] < 0.7)
		goto usedown;
	if(!trace.startsolid && !trace.allsolid)
		VectorCopy(trace.endpos, pmove->origin);
	
	VectorCopy(pmove->origin, up);

	// decide which one went farther
	downdist = (down[0] - original[0]) * (down[0] - original[0]) + (down[1] - original[1]) * (down[1] - original[1]);
	updist = (up[0] - original[0]) * (up[0] - original[0]) + (up[1] - original[1]) * (up[1] - original[1]);

	if(downdist > updist)
	{
	usedown:
		VectorCopy(down, pmove->origin);
		VectorCopy(downvel, pmove->velocity);
	}
	else // copy z value from slide move
		pmove->velocity[2] = downvel[2];

	// if at a dead stop, retry the move with nudges to get around lips
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
	vec3_t start, stop;
	pmtrace_t trace;

	if(pmove->waterjumptime)
		return;

	vel = pmove->velocity;

	speed = sqrt(vel[0] * vel[0] + vel[1] * vel[1] + vel[2] * vel[2]);
	if(speed < 1)
	{
		vel[0] = 0;
		vel[1] = 0;
		return;
	};

	friction = pmove->movevars->friction;

	// if the leading edge is over a dropoff, increase friction
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

	drop = 0;

	if(pmove->waterlevel >= 2) // apply water friction
		drop += speed * pmove->movevars->waterfriction * pmove->waterlevel * pmove->frametime;
	else if(pmove->onground != -1) // apply ground friction
	{
		control = speed < pmove->movevars->stopspeed ? pmove->movevars->stopspeed : speed;
		drop += control * friction * pmove->frametime;
	};

	// scale the velocity
	newspeed = speed - drop;
	if(newspeed < 0)
		newspeed = 0;
	newspeed /= speed;

	vel[0] = vel[0] * newspeed;
	vel[1] = vel[1] * newspeed;
	vel[2] = vel[2] * newspeed;
};

/*
==================
PM_AirAccelerate
==================
*/
void PM_AirAccelerate(vec3_t wishdir, float wishspeed, float accel)
{
	int i;
	float addspeed, accelspeed, currentspeed, wishspd = wishspeed;

	if(pmove->dead)
		return;
	if(pmove->waterjumptime)
		return;

	if(wishspd > 30)
		wishspd = 30;
	currentspeed = DotProduct(pmove->velocity, wishdir);
	addspeed = wishspd - currentspeed;
	if(addspeed <= 0)
		return;
	accelspeed = accel * wishspeed * pmove->frametime;
	if(accelspeed > addspeed)
		accelspeed = addspeed;

	for(i = 0; i < 3; i++)
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

// TODO: unused
/*
==================
PM_InWater
==================
*/
qboolean PM_InWater()
{
	return pmove->waterlevel > 1;
};

// TODO: unused
/*
==================
PM_CheckWater
==================
*/
qboolean PM_CheckWater()
{
	// TODO
	return true;
};

/*
=============
PM_CatagorizePosition
=============
*/
void PM_CatagorizePosition()
{
	vec3_t point;
	int cont;
	pmtrace_t tr;

	// if the player hull point one unit down is solid, the player
	// is on ground

	// see if standing on something solid
	point[0] = pmove->origin[0];
	point[1] = pmove->origin[1];
	point[2] = pmove->origin[2] - 1;
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
	cont = pmove->PM_PointContents(point, NULL);

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
==================
*/
void PM_GetRandomStuckOffsets(int nIndex, int server, vec3_t offset)
{
	// TODO
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
	int x, y, z;
	int i;
	static int sign[3] = { 0, -1, 1 };
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
	float speed, drop, friction, control, newspeed, accel;
	float currentspeed, addspeed, accelspeed;
	int i;
	vec3_t wishvel;
	float fmove, smove;
	vec3_t wishdir;
	float wishspeed;

	// friction

	speed = Length(pmove->velocity);
	if(speed < 1)
		VectorCopy(vec3_origin, pmove->velocity)
	else
	{
		drop = 0;

		friction = pmove->movevars->friction * 1.5; // extra friction
		control = speed < pmove->movevars->stopspeed ? pmove->movevars->stopspeed : speed;
		drop += control * friction * pmove->frametime;

		// scale the velocity
		newspeed = speed - drop;
		if(newspeed < 0)
			newspeed = 0;
		newspeed /= speed;

		VectorScale(pmove->velocity, newspeed, pmove->velocity);
	};

	// accelerate
	fmove = pmove->cmd.forwardmove;
	smove = pmove->cmd.sidemove;

	VectorNormalize(pmove->forward);
	VectorNormalize(pmove->right);

	for(i = 0; i < 3; i++)
		wishvel[i] = pmove->forward[i] * fmove + pmove->right[i] * smove;
	wishvel[2] += pmove->cmd.upmove;

	VectorCopy(wishvel, wishdir);
	wishspeed = VectorNormalize(wishdir);

	//
	// clamp to server defined max speed
	//
	if(wishspeed > pmove->movevars->spectatormaxspeed)
	{
		VectorScale(wishvel, pmove->movevars->spectatormaxspeed / wishspeed, wishvel);
		wishspeed = pmove->movevars->spectatormaxspeed;
	};

	currentspeed = DotProduct(pmove->velocity, wishdir);
	addspeed = wishspeed - currentspeed;
	if(addspeed <= 0)
		return;
	accelspeed = pmove->movevars->accelerate * pmove->frametime * wishspeed;
	if(accelspeed > addspeed)
		accelspeed = addspeed;

	for(i = 0; i < 3; i++)
		pmove->velocity[i] += accelspeed * wishdir[i];

	// move
	VectorMA(pmove->origin, pmove->frametime, pmove->velocity, pmove->origin);
};

// TODO: unused
/*
==================
PM_SplineFraction
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
	// TODO
};

// TODO: unused
/*
==================
PM_UnDuck
==================
*/
void PM_UnDuck()
{
	// TODO
};

// TODO: unused
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
// TODO: unused
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
//Com_Printf("wishvel[2] = %i, fwdmove = %i\n", (int)wishvel[2], (int)pmove->cmd.forwardmove );

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

// TODO: unused
/*
==================
PM_Ladder
==================
*/
physent_t *PM_Ladder()
{
	// TODO
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

// TODO: unused
/*
==================
PM_Physics_Toss

Dead player flying through air, e.g.
==================
*/
void PM_Physics_Toss()
{
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
	// TODO
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

// TODO: unused
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

// TODO: unused
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
	// TODO
};

/*
==================
PM_ReduceTimers
==================
*/
void PM_ReduceTimers()
{
	// TODO
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
	
	PM_CheckParameters();
	
	pmove->frametime = pmove->cmd.msec * 0.001;
	
	pmove->numtouch = 0;
	
	PM_ReduceTimers();

	AngleVectors(pmove->angles, pmove->forward, pmove->right, pmove->up); // TODO: pmove->angles = sv_player->v.angles/sv_player->v.v_angle

	if(pmove->spectator)
	{
		PM_SpectatorMove();
		PM_CatagorizePosition();
		return;
	};

	PM_CheckStuck();

	// take angles directly from command
	//VectorCopy(pmove->cmd.viewangles, pmove->angles);

	// set onground, watertype, and waterlevel
	PM_CatagorizePosition();

	//PM_CheckLadderMove(); // TODO
	
	// Don't run ladder code if dead
	if(!pmove->dead)
	{
		//if(pml.ladder)
			//PM_LadderMove(); // TODO
		/*else*/ if(pmove->movetype != MOVETYPE_WALK && pmove->movetype != MOVETYPE_NOCLIP)
			pmove->movetype = MOVETYPE_WALK;
	};
	
	switch(pmove->movetype)
	{
	case MOVETYPE_NONE:
		break;
	case MOVETYPE_NOCLIP:
		PM_NoClip();
		break;
	case MOVETYPE_WALK:
		if(pmove->waterjumptime) // if(pmove->flags & FL_WATERJUMP)
		{
			PM_WaterJump();
			return;
		};
		
		if(pmove->waterlevel >= 2)
		{
			if(pmove->waterlevel == 2)
				PM_CheckWaterJump();
		
			if(pmove->velocity[2] < 0 && pmove->waterjumptime)
				pmove->waterjumptime = 0;
			
			if(pmove->cmd.buttons & IN_JUMP)
				PM_Jump();
			else
				pmove->oldbuttons &= ~IN_JUMP;
			
			PM_WaterMove();
			
			// set onground, watertype, and waterlevel for final spot
			PM_CatagorizePosition();
		}
		else
		{
			if(pmove->onground != -1)
			{
				pmove->velocity[2] = 0;
				PM_Friction();
				//PM_Accelerate(wishdir, wishspeed, pmove->movevars->accelerate);
				//pmove->velocity[2] -= pmove->movevars->entgravity * pmove->movevars->gravity * pmove->frametime;
				PM_WalkMove();
			}
			else
				PM_AirMove();
			
			// set onground, watertype, and waterlevel for final spot
			PM_CatagorizePosition();
		};
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