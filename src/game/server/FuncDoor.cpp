/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2021, 2023 BlackPhrase
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
/// @brief door functions

#include "FuncDoor.hpp"
#include "Util.hpp"

//=============================================================================

LINK_ENTITY_TO_CLASS(func_door, CFuncDoor);

void CFuncDoor::Spawn()
{
	if(world->v.worldtype == 0)
	{
		PrecacheSound("doors/medtry.wav");
		PrecacheSound("doors/meduse.wav");
		
		self->noise3 = "doors/medtry.wav";
		self->noise4 = "doors/meduse.wav";
	}
	else if(world->v.worldtype == 1)
	{
		PrecacheSound("doors/runetry.wav");
		PrecacheSound("doors/runeuse.wav");
		
		self->noise3 = "doors/runetry.wav";
		self->noise4 = "doors/runeuse.wav";
	}
	else if(world->v.worldtype == 2)
	{
		PrecacheSound("doors/basetry.wav");
		PrecacheSound("doors/baseuse.wav");
		
		self->noise3 = "doors/basetry.wav";
		self->noise4 = "doors/baseuse.wav";
	}
	else
		dprint("no worldtype set!\n");
	
	if(self->sounds == 0)
	{
		PrecacheSound("misc/null.wav");
		
		self->noise1 = "misc/null.wav";
		self->noise2 = "misc/null.wav";
	};
	
	if(self->sounds == 1)
	{
		PrecacheSound("doors/drclos4.wav");
		PrecacheSound("doors/doormv1.wav");
		
		self->noise1 = "doors/drclos4.wav";
		self->noise2 = "doors/doormv1.wav";
	};
	
	if(self->sounds == 2)
	{
		PrecacheSound("doors/hydro1.wav");
		PrecacheSound("doors/hydro2.wav");
		
		self->noise2 = "doors/hydro1.wav";
		self->noise1 = "doors/hydro2.wav";
	};
	
	if(self->sounds == 3)
	{
		PrecacheSound("doors/stndr1.wav");
		PrecacheSound("doors/stndr2.wav");
		
		self->noise2 = "doors/stndr1.wav";
		self->noise1 = "doors/stndr2.wav";
	};
	
	if(self->sounds == 4)
	{
		PrecacheSound("doors/ddoor1.wav");
		PrecacheSound("doors/ddoor2.wav");
		
		self->noise1 = "doors/ddoor2.wav";
		self->noise2 = "doors/ddoor1.wav";
	};
	
	SetMovedir(self);
	
	//SetMaxHealth(GetHealth());
	SetSolidity(CBaseEntity::Solidity::BSP);
	SetMoveType(CBaseEntity::MoveType::Push);
	
	SetOrigin(GetOrigin());  
	SetModel(GetModel());
	
	SetClassName("door");
	
	SetBlockedCallback(CFuncDoor::Blocked);
	SetUseCallback(CFuncDoor::Use);
	
	//if(GetSpawnFlags() & DOOR_SILVER_KEY)
		//self->items = IT_KEY1;
	//if(GetSpawnFlags() & DOOR_GOLD_KEY)
		//self->items = IT_KEY2;
	
	if(!GetSpeed())
		SetSpeed(100);
	//if(!self->wait)
		//self->wait = 3;
	//if(!self->lip)
		//self->lip = 8;
	//if(!self->dmg)
		//self->dmg = 2;

	self->pos1 = self->GetOrigin();
	self->pos2 = self->pos1 + self->movedir * (fabs(self->movedir * self->size) - self->lip);

	// DOOR_START_OPEN is to allow an entity to be lighted in the closed position
	// but spawn in the open position
	if(GetSpawnFlags() & DOOR_START_OPEN)
	{
		SetOrigin(self->pos2);
		self->pos2 = self->pos1;
		self->pos1 = self->origin;
	};
	
	self->state = STATE_BOTTOM;
	
	//if(GetHealth())
	{
		//self->takedamage = DAMAGE_YES;
		//self->th_die = door_killed;
	};
	
	//if (self->items)
		//self->wait = -1;
	
	SetTouchCallback(CFuncDoor::Touch);
	
	// LinkDoors can't be done until all of the doors have been spawned, so
	// the sizes can be detected properly.
	//SetThinkCallback(LinkDoors);
	//SetNextThink(self->ltime + 0.1);
};

void CFuncDoor::Use(CBaseEntity *apActivator, CBaseEntity *apCaller, UseType aeUseType, float afValue)
{
	self->message = ""; // Door message are for touch only
	GetOwner()->message = "";        
	GetEnemy()->message = "";

	CBaseEntity *oself{self};
	self = self->GetOwner();
	Fire(apActivator);
	self = oself;
};

void CFuncDoor::Blocked(CBaseEntity *apOther)
{
	//apOther->deathtype = "squish";
	apOther->TakeDamage(self, self->goalentity, self->dmg, DMG_CRUSH);

	// if a door has a negative wait, it would never come back if blocked,
	// so let it just squash the object to death real fast
	if(GetDelay() >= 0)
	{
		if(self->state == STATE_DOWN)
			this->go_up();
		else
			this->go_down();
	};
};

/*
================
door_touch

Prints messages and opens key doors
================
*/
void CFuncDoor::Touch(CBaseEntity *other)
{
	// Ignore touches by anything but players
	if(other->GetClassName() != "player")
		return;

/*	
	if(GetOwner()->attack_finished > gpGlobals->time)
		return;

	GetOwner()->attack_finished = gpGlobals->time + 2;

	if(self->owner->message != "")
	{
		centerprint(other, GetOwner()->message);
		other->EmitSound(CHAN_VOICE, "misc/talk.wav", 1, ATTN_NORM);
	};

	// key door stuff
	if(!self->GetItems())
		return;

	// FIXME: blink key on player's status bar
	if((self->items & other.items) != self.items)
	{
		if (self.owner.items == IT_KEY1)
		{
			if (world.worldtype == 2)
			{
				centerprint (other, "You need the silver keycard");
				gpEngine->pfnEmitSound (self, CHAN_VOICE, self.noise3, 1, ATTN_NORM);
			}
			else if (world.worldtype == 1)
			{
				centerprint (other, "You need the silver runekey");
				gpEngine->pfnEmitSound (self, CHAN_VOICE, self.noise3, 1, ATTN_NORM);
			}
			else if (world.worldtype == 0)
			{
				centerprint (other, "You need the silver key");
				gpEngine->pfnEmitSound (self, CHAN_VOICE, self.noise3, 1, ATTN_NORM);
			};
		}
		else
		{
			if (world.worldtype == 2)
			{
				centerprint (other, "You need the gold keycard");
				gpEngine->pfnEmitSound (self, CHAN_VOICE, self.noise3, 1, ATTN_NORM);
			}
			else if (world.worldtype == 1)
			{
				centerprint (other, "You need the gold runekey");
				gpEngine->pfnEmitSound (self, CHAN_VOICE, self.noise3, 1, ATTN_NORM);                    
			}
			else if (world.worldtype == 0)
			{
				centerprint (other, "You need the gold key");
				gpEngine->pfnEmitSound (self, CHAN_VOICE, self.noise3, 1, ATTN_NORM);
			};
		};
		return;
	};
*/

	//other->SetItems(other->GetItems() - self->GetItems());
	
	SetTouchCallback(SUB_Null);
	if(self->GetEnemy())
		self->GetEnemy()->SetTouchCallback(SUB_Null); // get paired door
	Use();
};

CBaseEntity *spawn_field(const idVec3 &fmins, const idVec3 &fmaxs)
{
	CBaseEntity *trigger{gpEngine->pfnSpawn()};
	
	trigger->SetMoveType(CBaseEntity::MoveType::None);
	trigger->SetSolidity(CBaseEntity::Solidity::Trigger);
	trigger->SetOwner(self);
	trigger->SetTouchCallback(door_trigger_touch);

	idVec3 t1 = fmins;
	idVec3 t2 = fmaxs;
	
	trigger->SetSize(t1 - idVec3(60, 60, 8), t2 + idVec3(60, 60, 8));
	
	return trigger;
};

bool EntitiesTouching(edict_t *e1, edict_t *e2)
{
	if (e1->v.mins_x > e2->v.maxs_x)
		return false;
	if (e1->v.mins_y > e2->v.maxs_y)
		return false;
	if (e1->v.mins_z > e2->v.maxs_z)
		return false;
	if (e1->v.maxs_x < e2->v.mins_x)
		return false;
	if (e1->v.maxs_y < e2->v.mins_y)
		return false;
	if (e1->v.maxs_z < e2->v.mins_z)
		return false;
	return true;
};

/*
=============
LinkDoors


=============
*/
void LinkDoors()
{
	entvars_t t, starte;
	idVec3 cmins, cmaxs;
	
	// already linked by another door
	if(self->GetEnemy())
		return;
	
	if(self->GetSpawnFlags() & 4)
	{
		self->owner = self->GetEnemy() = self;
		return; // don't want to link this door
	};

	cmins = self->GetMins();
	cmaxs = self->GetMaxs();
	
	starte = self;
	t = self;
	
	do
	{
		self.owner = starte;                    // master door

		if (self.health)
			starte.health = self.health;
		if (self.targetname)
			starte.targetname = self.targetname;
		if (self.message != "")
			starte.message = self.message;

		t = find (t, classname, self.classname);        
		if (!t)
		{
			self.enemy = starte; // make the chain a loop

		// shootable, fired, or key doors just needed the owner/enemy links,
		// they don't spawn a field
	
			self = self->GetOwner();

			if(self->GetHealth())
				return;
			if(self->targetname)
				return;
			if(self->GetItems())
				return;

			self->GetOwner()->trigger_field = spawn_field(cmins, cmaxs);

			return;
		};

		if(EntitiesTouching(self, t))
		{
			if(t.enemy)
				objerror ("cross connected doors");
			
			self.enemy = t;
			self = t;

			if (t.mins_x < cmins_x)
				cmins_x = t.mins_x;
			if (t.mins_y < cmins_y)
				cmins_y = t.mins_y;
			if (t.mins_z < cmins_z)
				cmins_z = t.mins_z;
			if (t.maxs_x > cmaxs_x)
				cmaxs_x = t.maxs_x;
			if (t.maxs_y > cmaxs_y)
				cmaxs_y = t.maxs_y;
			if (t.maxs_z > cmaxs_z)
				cmaxs_z = t.maxs_z;
		};
	}
	while(1);
};

/*
=============================================================================

THINK FUNCTIONS

=============================================================================
*/

void CFuncDoor::GoUp()
{
	if(self->state == STATE_UP)
		return; // Already going up
	
	if(self->state == STATE_TOP)
	{
		// Reset top wait time
		SetNextThink(self->ltime + GetDelay());
		return;
	};
	
	EmitSound(CHAN_VOICE, self->noise2, 1, ATTN_NORM);
	self->state = STATE_UP;
	SetMoveDoneCallback(CFuncDoor::HitTop);
	LinearMove(self->pos2, self->speed);

	SUB_UseTargets();
};

void CFuncDoor::GoDown()
{
	EmitSound(CHAN_VOICE, self->noise2, 1, ATTN_NORM);
	if(GetMaxHealth())
	{
		SetDamageable(DAMAGE_YES);
		SetHealth(GetMaxHealth());
	};
	
	self->state = STATE_DOWN;
	SetMoveDoneCallback(CFuncDoor::HitBottom);
	LinearMove(self->pos1, self->speed);
};

void CFuncDoor::HitTop()
{
	EmitSound(CHAN_NO_PHS_ADD + CHAN_VOICE, self->noise1, 1, ATTN_NORM);
	self->state = STATE_TOP;
	if(GetSpawnFlags() & DOOR_TOGGLE)
		return; // Don't come down automatically
	SetThinkCallback(CFuncDoor::GoDown);
	SetNextThink(self->ltime + GetDelay());
};

void CFuncDoor::HitBottom()
{
	EmitSound(CHAN_NO_PHS_ADD + CHAN_VOICE, self->noise1, 1, ATTN_NORM);
	self->state = STATE_BOTTOM;
};

/*
=============================================================================

ACTIVATION FUNCTIONS

=============================================================================
*/

void CFuncDoor::Fire(CBaseEntity *activator)
{
	entity oself;
	entity starte;
	
	if(GetOwner() != self)
		objerror("door_fire: self.owner != self");
	
	// Play use key sound
	
	if(self->items)
		EmitSound(CHAN_VOICE, self->noise4, 1, ATTN_NORM);
	
	self->message = string_null; // No more message
	oself = self;
	
	if(GetSpawnFlags() & DOOR_TOGGLE)
	{
		if(self->GetState() == STATE_UP || self->GetState() == STATE_TOP)
		{
			starte = self;
			do
			{
				this->go_down();
				self = self->GetEnemy();
			}
			while((self != starte) && (self != world));
			
			self = oself;
			return;
		};
	};
	
	// Trigger all paired doors
	starte = self;
	
	do
	{
		SetGoalEntity(activator); // Who fired us
		this->go_up();
		self = self->GetEnemy();
	}
	while((self != starte) && (self != world));
	
	self = oself;
};