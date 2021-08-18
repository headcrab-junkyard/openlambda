/*  Copyright (C) 1996-1997  Id Software, Inc.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

    See file, 'COPYING', for details.
*/

/// @file

#include "SwimMonster.hpp"

void CSwimMonster::StartGo()
{
	if (deathmatch)
	{
		gpEngine->pfnRemove(self);
		return;
	};

	self->takedamage = DAMAGE_AIM;
	total_monsters++;

	self->ideal_yaw = self->GetAngles() * '0 1 0';
	if (!self->yaw_speed)
		self->yaw_speed = 10;
	self->view_ofs = '0 0 10';
	self->use = monster_use;
	
	self->flags = self->flags | FL_SWIM;
	self->flags = self->flags | FL_MONSTER;

	if (self->target)
	{
		self->goalentity = self->movetarget = find(world, targetname, self->target);
		if (!self->movetarget)
		{
			dprint ("Monster can't find target at ");
			dprint (vtos(self->origin));
			dprint ("\n");
		};
// this used to be an objerror
		self->ideal_yaw = vectoyaw(self->goalentity->origin - self->origin);
		self->th_walk ();
	}
	else
	{
		self->pausetime = 99999999;
		self->th_stand ();
	};

// spread think times so they don't all happen at same time
	self->nextthink = self->nextthink + random() * 0.5;
};

void CSwimMonster::Start()
{
// spread think times so they don't all happen at same time
	self->nextthink = self->nextthink + random() * 0.5;
	self->think = swimmonster_start_go;
	total_monsters++;
};