/*
	triggers.qc

	trigger functions

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

entity stemp, otemp, s, old;

void trigger_reactivate()
{
	self->SetSolidity(SOLID_TRIGGER);
};

//=============================================================================

/*QUAKED trigger_secret (.5 .5 .5) ?
secret counter trigger
sounds
1)	secret
2)	beep beep
3)
4)
set "message" to text string
*/
void trigger_secret()
{
	total_secrets = total_secrets + 1;
	self.wait = -1;
	if (!self.message)
		self.message = "You found a secret area!";
	if (!self.sounds)
		self.sounds = 1;
	
	if (self.sounds == 1)
	{
		precache_sound ("misc/secret.wav");
		self.noise = "misc/secret.wav";
	}
	else if (self.sounds == 2)
	{
		precache_sound ("misc/talk.wav");
		self.noise = "misc/talk.wav";
	}

	trigger_multiple ();
};

/*
==============================================================================

TELEPORT TRIGGERS

==============================================================================
*/

const float	PLAYER_ONLY	= 1;

void play_teleport()
{
	float v;
	string_t tmpstr;

	v = random() * 5;
	if (v < 1)
		tmpstr = "misc/r_tele1.wav";
	else if (v < 2)
		tmpstr = "misc/r_tele2.wav";
	else if (v < 3)
		tmpstr = "misc/r_tele3.wav";
	else if (v < 4)
		tmpstr = "misc/r_tele4.wav";
	else
		tmpstr = "misc/r_tele5.wav";

	sound (self, CHAN_VOICE, tmpstr, 1, ATTN_NORM);
	remove (self);
};

void spawn_tfog(const idVec3 &org)
{
	s = gpEngine->pfnCreateEntity ();
	s.origin = org;
	s.nextthink = time + 0.2;
	s.think = play_teleport;

	WriteByte (MSG_MULTICAST, SVC_TEMPENTITY);
	WriteByte (MSG_MULTICAST, TE_TELEPORT);
	WriteCoord (MSG_MULTICAST, org_x);
	WriteCoord (MSG_MULTICAST, org_y);
	WriteCoord (MSG_MULTICAST, org_z);
	multicast (org, MULTICAST_PHS);
};

void tdeath_touch()
{
	entity other2;

	if (other == self.owner)
		return;

// frag anyone who teleports in on top of an invincible player
	if (other.classname == "player")
	{
		if (other.invincible_finished > time &&
			self.owner.invincible_finished > time) {
			self.classname = "teledeath3";
			other.invincible_finished = 0;
			self.owner.invincible_finished = 0;
			T_Damage (other, self, self, 50000);
			other2 = self.owner;
			self.owner = other;
			T_Damage (other2, self, self, 50000);
		};
			
		if (other.invincible_finished > time)
		{
			self->SetClassName("teledeath2");
			self->GetOwner()->TakeDamage (self, self, 50000);
			return;
		};
	};

	if (other.health)
		other->TakeDamage(self, self, 50000);
};

void spawn_tdeath(const idVec3 &org, CBaseEntity *death_owner)
{
	CBaseEntity *death = mpWorld->SpawnEntity();
	death->SetClassName("teledeath");
	death->SetMoveType(MOVETYPE_NONE);
	death.solid = SOLID_TRIGGER;
	death->SetAngles('0 0 0');
	death->SetSize(death_owner->GetSize().mins - idVec3(1.0f), death_owner->GetSize().maxs + idVec3(1.0f));
	death->SetOrigin(org);
	death.touch = tdeath_touch;
	death.nextthink = time + 0.2;
	death.think = SUB_Remove;
	death.owner = death_owner;
	
	force_retouch = 2; // make sure even still objects get hit
};

/*QUAKED info_teleport_destination (.5 .5 .5) (-8 -8 -8) (8 8 32)
This is the destination marker for a teleporter.  It should have a "targetname" field with the same value as a teleporter's "target" field.
*/
void info_teleport_destination()
{
// this does nothing, just serves as a target spot
	self.mangle = self.angles;
	self.angles = '0 0 0';
	self.model = "";
	self.origin = self.origin + '0 0 27';
	if (!self.targetname)
		objerror ("no targetname");
};

/*
==============================================================================

trigger_setskill

==============================================================================
*/

/*QUAKED trigger_setskill (.5 .5 .5) ?
sets skill level to the value of "message".
Only used on start map.
*/
void trigger_setskill()
{
	remove (self);
};


/*
==============================================================================

ONLY REGISTERED TRIGGERS

==============================================================================
*/

void trigger_onlyregistered_touch()
{
	if (other.classname != "player")
		return;
	if (self.attack_finished > time)
		return;

	self.attack_finished = time + 2;
	if (cvar("registered"))
	{
		self.message = "";
		SUB_UseTargets ();
		remove (self);
	}
	else
	{
		if (self.message != "")
		{
			centerprint (other, self.message);
			sound (other, CHAN_BODY, "misc/talk.wav", 1, ATTN_NORM);
		}
	}
};

/*QUAKED trigger_onlyregistered (.5 .5 .5) ?
Only fires if playing the registered version, otherwise prints the message
*/
void trigger_onlyregistered()
{
	precache_sound ("misc/talk.wav");
	InitTrigger ();
	self.touch = trigger_onlyregistered_touch;
};