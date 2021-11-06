/*
	subs.qc

	sub-functions, mostly movement related

	Copyright (C) 1996-1997  Id Software, Inc.

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

	See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to:

		Free Software Foundation, Inc.
		59 Temple Place - Suite 330
		Boston, MA  02111-1307, USA

*/

/// @file

#include "BaseToggle.hpp"

/*
=============
SUB_CalcMove

calculate self->velocity and self->nextthink to reach dest from
self->origin traveling at speed
===============
*/
/*
void CBaseToggle::SUB_CalcMoveEnt(edict_t *ent, const idVec3 &tdest, float tspeed, void (*func)())
{
	edict_t *stemp;
	stemp = *self;
	self = ent;

	LinearMove (tdest, tspeed, func);
	self = stemp;
};
*/

void CBaseToggle::LinearMove(const idVec3 &tdest, float tspeed)
{
	if (!tspeed)
		objerror("No speed is defined!");

	//this->mpfnMoveDoneCallback = func;
	mvFinalDest = tdest;
	SetThinkCallback(CBaseToggle::LinearMoveDone);

	if(tdest == GetOrigin())
	{
		SetVelocity(idVec3::Origin);
		SetNextThink(self->ltime + 0.1);
		return;
	};
		
	// Set destdelta to the vector needed to move
	idVec3 vdestdelta{tdest - GetOrigin()};
	
	// Divide by speed to get time to reach dest
	float traveltime{vdestdelta.Length() / tspeed};

	if(traveltime < 0.03)
		traveltime = 0.03;
	
	// Set nextthink to trigger a think when dest is reached
	SetNextThink(self->ltime + traveltime);

	// Scale the destdelta vector by the time spent traveling to get velocity
	SetVelocity(vdestdelta * (1 / traveltime));	// qcc won't take vec/float	
};

/*
============
After moving, set origin to exact final destination
============
*/
void CBaseToggle::LinearMoveDone()
{
	SetOrigin(mvFinalDest);
	SetVelocity(idVec3::Origin);
	SetNextThink(-1);
	if(mfnMoveDoneCallback)
		(this->*mfnMoveDoneCallback)();
};

/*
=============
SUB_CalcAngleMove

calculate self->avelocity and self->nextthink to reach destangle from
self->angles rotating 

The calling function should make sure self->think is valid
===============
*/
/*
void CBaseToggle::SUB_CalcAngleMoveEnt(entvars_t *ent, const idVec3 &destangle, float tspeed, void (*func)())
{
	entvars_t *stemp = self;
	self = ent;
	SUB_CalcAngleMove(destangle, tspeed, func);
	self = stemp;
};
*/

void CBaseToggle::SUB_CalcAngleMove(const idVec3 &destangle, float tspeed)
{
	if(!tspeed)
		objerror("No speed is defined!");
		
	// Set destdelta to the vector needed to move
	idVec3 destdelta{destangle - GetAngles()};
	
	// Divide by speed to get time to reach dest
	float traveltime{destdelta.Length() / tspeed};

	// Set nextthink to trigger a think when dest is reached
	SetNextThink(self->ltime + traveltime);

	// Scale the destdelta vector by the time spent traveling to get velocity
	SetAngularVelocity(destdelta * (1 / traveltime));
	
	//this->mpfnMoveDoneCallback = func;
	mvFinalAngle = destangle;
	SetThinkCallback(SUB_CalcAngleMoveDone);
};

/*
============
After rotating, set angle to exact final angle
============
*/
void CBaseToggle::SUB_CalcAngleMoveDone() // TODO: AngularMoveDone in gs?
{
	SetAngles(mvFinalAngle);
	SetAngularVelocity(idVec3::Origin);
	SetNextThink(-1);
	if(mfnMoveDoneCallback)
		(this->*mfnMoveDoneCallback)();
};