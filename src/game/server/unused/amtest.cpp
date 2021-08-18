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
/*~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>
~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~*/

#include "BaseEntity.hpp"

class CTestTeleport : public CBaseEntity
{
public:
	void Spawn() override;
	
	void Touch(CBaseEntity *other) override;
	
	void tele_done();
};

/*QUAKED test_teleport (0 .5 .8) ?
Teleporter testing
*/
LINK_ENTITY_TO_CLASS(test_teleport, CTestTeleport)

void CTestTeleport::Spawn()
{
	gpEngine->pfnPrecacheModel ("sprites/s_aball.spr");
	self->SetSize (self.mins, self.maxs);	
	self->SetTouchCallback(test_teleport_touch);
	self.solid = 1;
	
	if (!self.target)
		objerror ("no target\n");
};

void CTestTeleport::Touch(CBaseEntity *other)
{
	entity oldself;
	other.movetype = MOVETYPE_TOSS;
//	other.solid = SOLID_NOT;
	other.dest = '256 -128 -128';
	oldself = self;
	self = other;
//	SUB_CalcMove (self.dest, 200, tele_done);
	self.velocity = '1000 0 0 ';
	self = oldself;
};

void CTestTeleport::tele_done()
{
	self->SetMoveType(MOVETYPE_WALK);
	self->SetSolidity(SOLID_SLIDEBOX);
};

/*~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>
~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~*/

void test_goaway();
void test_spawn();

/*QUAKED test_fodder (0 .5 .8) ?
beating guy
*/
C_EXPORT void test_fodder(entvars_t *self)
{
	self.nextthink = gpGlobals->time + 3;
	self.think = test_spawn;
};

void test_spawn()
{
	entity	body;
	makevectors (self.angles);

	body = spawn();
	setmodel (body, "progs/soldier.mdl");
	setorigin (body, self.origin);
	body.classname = "player";
	body.health = 1000;
	body.frags = 0;
	body.takedamage = DAMAGE_AIM;
	body.solid = SOLID_SLIDEBOX;
	body.movetype = MOVETYPE_WALK;
	body.show_hostile = 0;
	body.weapon = 1;
	body.velocity = v_forward * 200;

	body.nextthink = time + 5;
	body.think = test_goaway;

	self.nextthink = time + 3;
	self.think = test_spawn;
};

void test_goaway()
{
	gpEngine->pfnRemove (self);
};