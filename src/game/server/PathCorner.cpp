/*
	movetarget.qc

	movetarget code

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

#include "PointEntity.hpp"
#include "Util.hpp"

/*
==============================================================================

MOVETARGET CODE

The angle of the movetarget effects standing and bowing direction, but has no effect on movement, which allways heads to the next target.

targetname
must be present.  The name of this movetarget.

target
the next spot to move to.  If not present, stop here for good.

pausetime
The number of seconds to spend standing or bowing for path_stand or path_bow

==============================================================================
*/

/*QUAKED path_corner (0.5 0.3 0) (-8 -8 -8) (8 8 8)
Monsters will continue walking towards the next target corner.
*/
class CPathCorner : public CPointEntity
{
public:
	void Spawn() override;
	
	bool HandleKeyValue(const std::string &asKey, const std::string &asValue) override;
	
	void Touch(CBaseEntity *apOther) override;
};

LINK_ENTITY_TO_CLASS(path_corner, CPathCorner);

void CPathCorner::Spawn()
{
	if(!self->targetname)
		objerror("monster_movetarget: no targetname");
		
	SetSolidity(SOLID_TRIGGER);
	SetTouchCallback(CPathCorner::Touch);
	SetSize(idVec3(-8,-8, -8), idVec3(8, 8, 8));
};

bool CPathCorner::HandleKeyValue(const std::string &asKey, const std::string &asValue)
{
	if(asKey == "wait")
	{
		
		return true;
	};
	
	return CPointEntity::HandleKeyValue(asKey, asValue);
};

/*
=============
t_movetarget

Something has bumped into a movetarget.  If it is a monster
moving towards it, change the next destination and continue.
==============
*/
void CPathCorner::Touch(CBaseEntity *apOther)
{
	if(apOther->GetGoal() != this)
		return;
	
	if(apOther->GetEnemy())
		return; // fighting, not following a path

	//CBaseEntity *temp = self;
	//self = apOther;
	//apOther = temp;

	if(apOther->GetClassName() == "monster_ogre")
		apOther->EmitSound(CHAN_VOICE, "ogre/ogdrag.wav", 1, ATTN_IDLE); // play chainsaw drag sound

	//dprint ("t_movetarget\n");
	apOther->SetGoal(/*apOther->movetarget =*/ gpEngine->pfnFindEntityByString(world, "targetname", apOther->GetTarget()));
	
	if(!apOther->GetGoal())
	{
		//apOther->pausetime = gpGlobals->time + 999999;
		//apOther->th_stand();
		return;
	};
};