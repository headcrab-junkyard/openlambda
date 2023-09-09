/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2019-2021 BlackPhrase
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
/// @brief button and multiple button code

#include "BaseButton.hpp"
#include "Util.hpp"

extern void SetMovedir(entvars_t *self);

/*QUAKED func_button (0 .5 .8) ?
When a button is touched, it moves some distance in the direction of it's angle, triggers all of it's targets, waits some time, then returns to it's original position where it can be triggered again.

"angle"		determines the opening direction
"target"	all entities with a matching targetname will be used
"speed"		override the default 40 speed
"wait"		override the default 1 second wait (-1 = never return)
"lip"		override the default 4 pixel lip remaining at end of move
"health"	if set, the button must be killed instead of touched
"sounds"
0) steam metal
1) wooden clunk
2) metallic click
3) in-out
*/
LINK_ENTITY_TO_CLASS(func_button, CBaseButton);

void CBaseButton::Spawn()
{
	//
	//Precache();
	//
	switch(mnSounds)
	{
	case 0:
		PrecacheSound("buttons/airbut1.wav");
		self->noise = gpEngine->pfnAllocString("buttons/airbut1.wav");
		break;
	case 1:
		PrecacheSound("buttons/switch21.wav");
		self->noise = gpEngine->pfnAllocString("buttons/switch21.wav");
		break;
	case 2:
		PrecacheSound("buttons/switch02.wav");
		self->noise = gpEngine->pfnAllocString("buttons/switch02.wav");
		break;
	case 3:
		PrecacheSound("buttons/switch04.wav");
		self->noise = gpEngine->pfnAllocString("buttons/switch04.wav");
		break;
	};
	//
	
	SetMovedir(self);

	SetMoveType(MOVETYPE_PUSH);
	SetSolidity(SOLID_BSP);
	SetModel(GetModel());

	//SetBlockedCallback(CBaseButton::Blocked);
	//SetUseCallback(CBaseButton::Use);

	if(GetHealth() > 0)
	{
		//SetMaxHealth(GetHealth());
		//self->th_die = CBaseButton::Killed;
		SetDamageable(DAMAGE_YES);
	}
	//else
		//SetTouchCallback(CBaseButton::Touch);

	if(!GetSpeed())
		SetSpeed(40);
	
	if(!GetDelay())
		SetDelay(1);
	
	if(!mfLip)
		mfLip = 4;

	meState = State::Bottom;

	mvPos1 = GetOrigin();
	mvPos2 = mvPos1 + GetMoveDir() * (fabs(GetMoveDir() * self->size) - mfLip);
};

void CBaseButton::Use(CBaseEntity *apActivator, CBaseEntity *apCaller, UseType aeUseType, float afValue)
{
	SetEnemy(apActivator);
	Fire();
};

void CBaseButton::Touch(CBaseEntity *apOther)
{
	if(apOther->GetClassName() != "player")
		return;
	
	mhActivator = apOther;
	
	SetEnemy(apOther);
	
	Fire();
};

void CBaseButton::Blocked(CBaseEntity *other)
{
	// Do nothing, just don't ome all the way back out
};

void CBaseButton::Wait()
{
	SetState(State::Top);
	
	{
		SetNextThink(self->ltime + GetDelay());
		SetThinkCallback(CBaseButton::Return);
	};
	
	SUB_UseTargets(GetEnemy(), UseType::Toggle, 0);
	
	self->frame = 1; // use alternate textures
};

void CBaseButton::Done()
{
	SetState(State::Bottom);
};

void CBaseButton::Return()
{
	SetState(State::GoingDown);
	SetMoveDoneCallback(CBaseButton::Done);
	LinearMove(mvPos1, GetSpeed());
	self->frame = 0; // use normal textures
	//if(GetHealth())
		//SetDamageable(DAMAGE_YES); // can be shot again
};

void CBaseButton::Fire()
{
	if(GetState() == State::GoingUp || GetState() == State::Top)
		return;

	EmitSound(CHAN_VOICE, GetNoise(), 1, ATTN_NORM);

	SetState(State::GoingUp);
	SetMoveDoneCallback(CBaseButton::Wait);
	LinearMove(mvPos2, GetSpeed());
};

/*
void CBaseButton::Killed(CBaseEntity *apAttacker)
{
	SetEnemy(damage_attacker);
	SetHealth(GetMaxHealth());
	SetDamageable(DAMAGE_NO); // wil be reset upon return
	Fire();
};
*/