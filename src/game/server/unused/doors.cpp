/*
	doors.qc

	door functions

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

const float DOOR_START_OPEN = 1;
const float DOOR_DONT_LINK = 4;
const float DOOR_GOLD_KEY = 8;
const float DOOR_SILVER_KEY = 16;
const float DOOR_TOGGLE = 32;

/*

Doors are similar to buttons, but can spawn a fat trigger field around them
to open without a touch, and they link together to form simultanious
double/quad doors.
 
Door.owner is the master door.  If there is only one door, it points to itself.
If multiple doors, all will point to a single one.

Door.enemy chains from the master door through all doors linked in the chain.

*/

void CDoor::trigger_touch(CBaseEntity *other)
{
	if (other.health <= 0)
		return;

	if (time < self.attack_finished)
		return;
	self.attack_finished = time + 1;

	activator = other;

	self = self.owner;
	Use ();
};

void CDoor::Killed()
{
	entvars_t *oself = self;
	self = self->GetOwner();
	self->SetHealth(self->GetMaxHealth());
	self->takedamage = DAMAGE_NO;    // wil be reset upon return
	Use();
	self = oself;
};

/*
=============================================================================

SPAWNING FUNCTIONS

=============================================================================
*/

/*
=============================================================================

SECRET DOORS

=============================================================================
*/

void fd_secret_move1();
void fd_secret_move2();
void fd_secret_move3();
void fd_secret_move4();
void fd_secret_move5();
void fd_secret_move6();
void fd_secret_done();

const float SECRET_OPEN_ONCE = 1;             // stays open
const float SECRET_1ST_LEFT = 2;              // 1st move is left of arrow
const float SECRET_1ST_DOWN = 4;              // 1st move is down from arrow
const float SECRET_NO_SHOOT = 8;              // only opened by trigger
const float SECRET_YES_SHOOT = 16;    // shootable even if targeted

// Wait after first movement...
void fd_secret_move1()
{
	self->v.nextthink = self->v.ltime + 1.0;
	self->v.think = fd_secret_move2;
	gpEngine->pfnEmitSound(self, CHAN_VOICE, self->v.noise3, 1, ATTN_NORM);
};

// Start moving sideways w/sound...
void fd_secret_move2()
{
	gpEngine->pfnEmitSound(self, CHAN_VOICE, self.noise2, 1, ATTN_NORM);
	SUB_CalcMove(self.dest2, self.speed, fd_secret_move3);
};

// Wait here until time to go back...
void fd_secret_move3()
{
	gpEngine->pfnEmitSound(self, CHAN_VOICE, self.noise3, 1, ATTN_NORM);
	if (!(self.spawnflags & SECRET_OPEN_ONCE))
	{
		self.nextthink = self.ltime + self.wait;
		self.think = fd_secret_move4;
	};
};

// Move backward...
void fd_secret_move4()
{
	gpEngine->pfnEmitSound(self, CHAN_VOICE, self.noise2, 1, ATTN_NORM);
	SUB_CalcMove(self.dest1, self.speed, fd_secret_move5);          
};

// Wait 1 second...
void fd_secret_move5()
{
	self.nextthink = self.ltime + 1.0;
	self.think = fd_secret_move6;
	gpEngine->pfnEmitSound(self, CHAN_VOICE, self.noise3, 1, ATTN_NORM);
};

void fd_secret_move6()
{
	gpEngine->pfnEmitSound(self, CHAN_VOICE, self.noise2, 1, ATTN_NORM);
	SUB_CalcMove(self.oldorigin, self.speed, fd_secret_done);
};

void fd_secret_done()
{
	if (!self.targetname || self.spawnflags&SECRET_YES_SHOOT)
	{
		self.health = 10000;
		self.takedamage = DAMAGE_YES;
		self.th_pain = fd_secret_use;   
		self.th_die = fd_secret_use;
	};
	gpEngine->pfnEmitSound(self, CHAN_NO_PHS_ADD+CHAN_VOICE, self.noise3, 1, ATTN_NORM);
};

/*QUAKED func_door_secret (0 .5 .8) ? open_once 1st_left 1st_down no_shoot always_shoot
Basic secret door. Slides back, then to the side. Angle determines direction.
wait  = # of seconds before coming back
1st_left = 1st move is left of arrow
1st_down = 1st move is down from arrow
always_shoot = even if targeted, keep shootable
t_width = override WIDTH to move back (or height if going down)
t_length = override LENGTH to move sideways
"dmg"           damage to inflict when blocked (2 default)

If a secret door has a targetname, it will only be opened by it's botton or trigger, not by damage.
"sounds"
1) medieval
2) metal
3) base
*/
class CFuncDoorSecret : public CBaseEntity
{
public:
	void Spawn() override;
	
	void Use(CBaseEntity *other) override;
	void Touch(CBaseEntity *other) override;
	void Think() override;
};

LINK_ENTITY_TO_CLASS(func_door_secret, CFuncDoorSecret)

void CFuncDoorSecret::Spawn()
{
	if (self->sounds == 0)
		self->sounds = 3;
	if (self->sounds == 1)
	{
		gpEngine->pfnPrecacheSound ("doors/latch2.wav");
		gpEngine->pfnPrecacheSound ("doors/winch2.wav");
		gpEngine->pfnPrecacheSound ("doors/drclos4.wav");
		self->noise1 = "doors/latch2.wav";
		self->noise2 = "doors/winch2.wav";
		self->noise3 = "doors/drclos4.wav";
	};
	if (self->sounds == 2)
	{
		gpEngine->pfnPrecacheSound ("doors/airdoor1.wav");
		gpEngine->pfnPrecacheSound ("doors/airdoor2.wav");
		self->noise2 = "doors/airdoor1.wav";
		self->noise1 = "doors/airdoor2.wav";
		self->noise3 = "doors/airdoor2.wav";
	};
	if (self->sounds == 3)
	{
		gpEngine->pfnPrecacheSound ("doors/basesec1.wav");
		gpEngine->pfnPrecacheSound ("doors/basesec2.wav");
		self->noise2 = "doors/basesec1.wav";
		self->noise1 = "doors/basesec2.wav";
		self->noise3 = "doors/basesec2.wav";
	};

	if (!self->dmg)
		self->dmg = 2;
		
	// Magic formula...
	self->mangle = self->angles;
	self->SetAngles(idVec3(0));
	self->SetSolidity(SOLID_BSP);
	self->SetMoveType(MOVETYPE_PUSH);
	self->SetClassName("door");
	
	self->SetModel(self->GetModel());
	self->SetOrigin(self->GetOrigin());  
	
	self->SetTouchCallback(secret_touch);
	self->SetBlockedCallback(secret_blocked);
	self->SetSpeed(50);
	self->SetUseCallback(fd_secret_use);
	if ( !self->targetname || self->spawnflags & SECRET_YES_SHOOT)
	{
		self->SetHealth(10000);
		self->takedamage = DAMAGE_YES;
		self->th_pain = fd_secret_use;
	};
	self->oldorigin = self->GetOrigin();
	if (!self->wait)
		self->wait = 5;          // 5 seconds before closing
};

void CFuncDoorSecret::fd_secret_use(CBaseEntity *other)
{
	float temp;
	
	self->SetHealth(10000);

	// exit if still moving around...
	if (self->GetOrigin() != self->oldorigin)
		return;
	
	self->message = string_null;             // no more message

	SUB_UseTargets();                               // fire all targets / killtargets
	
	if (!(self->spawnflags & SECRET_NO_SHOOT))
	{
		self->th_pain = SUB_Null;
		self->takedamage = DAMAGE_NO;
	};
	
	self->SetVelocity(idVec3(0));

	// Make a sound, wait a little...
	
	self->EmitSound(CHAN_VOICE, self->noise1, 1, ATTN_NORM);
	self->SetNextThink(self->ltime + 0.1);

	temp = 1 - (self->spawnflags & SECRET_1ST_LEFT); // 1 or -1
	makevectors(self->mangle);
	
	if (!self->t_width)
	{
		if (self->spawnflags & SECRET_1ST_DOWN)
			self->t_width = fabs(v_up * self->size);
		else
			self->t_width = fabs(v_right * self->size);
	};
		
	if (!self->t_length)
		self->t_length = fabs(v_forward * self->size);

	if (self->spawnflags & SECRET_1ST_DOWN)
		self->dest1 = self->origin - v_up * self->t_width;
	else
		self->dest1 = self->origin + v_right * (self->t_width * temp);
		
	self->dest2 = self->dest1 + v_forward * self->t_length;
	SUB_CalcMove(self->dest1, self->speed, fd_secret_move1);
	self->EmitSound(CHAN_VOICE, self->noise2, 1, ATTN_NORM);
};

void CFuncDoorSecret::secret_blocked(CBaseEntity *other)
{
	if (gpGlobals->time < self->attack_finished)
		return;
	self->attack_finished = gpGlobals->time + 0.5;
	other->deathtype = "squish";
	other->TakeDamage(self, self, self->dmg);
};

/*
================
secret_touch

Prints messages
================
*/
void CFuncDoorSecret::secret_touch(CBaseEntity *other)
{
	if (other->GetClassName() != "player")
		return;
	if (self->attack_finished > gpGlobals->time)
		return;

	self->attack_finished = gpGlobals->time + 2;
	
	if (self->message)
	{
		other->CenterPrint (self->message);
		other->EmitSound(CHAN_BODY, "misc/talk.wav", 1, ATTN_NORM);
	};
};