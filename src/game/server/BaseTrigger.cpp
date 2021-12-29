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

#include "BaseTrigger.hpp"
#include "exports.h"
#include "Util.hpp"

extern void SetMovedir(entvars_t *self);

const int SPAWNFLAG_NOMESSAGE{1};
const int SPAWNFLAG_NOTOUCH{1};

const int PLAYER_ONLY{1};

LINK_ENTITY_TO_CLASS(trigger, CBaseTrigger);

/*
================
InitTrigger
================
*/
void CBaseTrigger::InitTrigger()
{
	// trigger angles are used for one-way touches.  An angle of 0 is assumed
	// to mean no restrictions, so use a yaw of 360 instead.
	if (GetAngles() != idVec3::Origin)
		SetMovedir(self);
	SetSolidity(SOLID_TRIGGER);
	SetModel(GetModel()); // set size and link into world
	SetMoveType(MOVETYPE_NONE);
	//self->v.modelindex = 0;
	//SetModel("");
};

void CBaseTrigger::ToggleUse(CBaseEntity *apActivator, CBaseEntity *apCaller, UseType aeUseType, float afValue)
{
	if(GetSolidity() == SOLID_NOT)
	{
		SetSolidity(SOLID_TRIGGER);
		
		++gpGlobals->force_retouch;
	}
	else
		SetSolidity(SOLID_NOT);
	
	SetOrigin(GetOrigin());
};

void CBaseTrigger::CounterUse(CBaseEntity *apActivator, CBaseEntity *apCaller, UseType aeUseType, float afValue)
{
	--mnCount;
	
	if(mnCount < 0)
		return;
	
	if(mnCount != 0)
	{
		if (apActivator->GetClassName() == "player"
		&& (self->spawnflags & SPAWNFLAG_NOMESSAGE) == 0)
		{
			if (mnCount >= 4)
				gpEngine->pfnAlertMessage(at_console, "There are more to go...");
			else if (mnCount == 3)
				gpEngine->pfnAlertMessage(at_console, "Only 3 more to go...");
			else if (mnCount == 2)
				gpEngine->pfnAlertMessage(at_console, "Only 2 more to go...");
			else
				gpEngine->pfnAlertMessage(at_console, "Only 1 more to go...");
		};
		return;
	};
	
	if (apActivator->GetClassName() == "player"
	&& (self->spawnflags & SPAWNFLAG_NOMESSAGE) == 0)
		gpEngine->pfnAlertMessage(at_console, "Sequence completed!");
	
	SetEnemy(apActivator);
	ActivateMultiTrigger(mhActivator);
};

void CBaseTrigger::MultiTouch(CBaseEntity *apOther)
{
	if(apOther->GetClassName() != "player")
		return;
	
// if the trigger has an angles field, check player's facing direction
	if(GetMoveDir() != idVec3::Origin)
	{
		gpEngine->pfnMakeVectors(apOther->GetAngles());
		if(idVec3(gpGlobals->v_forward) * GetMoveDir() < 0)
			return; // not facing the right way
	};
	
	SetEnemy(apOther);
	ActivateMultiTrigger(apOther);
};

void CBaseTrigger::HurtTouch(CBaseEntity *apOther)
{
	if(apOther->IsDamageable())
	{
		SetSolidity(SOLID_NOT);
		
		int mnDmgBitSum = 0; // TODO
		
		float fDmg{self->dmg}; // TODO: multiply by 0.5
		
		if(fDmg < 0)
			apOther->TakeHealth(-fDmg, mnDmgBitSum);
		else
			apOther->TakeDamage(/*self*/ *this, /*self*/ *this, fDmg, mnDmgBitSum);
		
		//SetThinkCallback(hurt_on);
		SetNextThink(gpGlobals->time + 1);
	};

	return;
};

void CBaseTrigger::TeleportTouch(CBaseEntity *apOther)
{
	if(self->targetname)
		if(GetNextThink() < gpGlobals->time)
			return; // not fired yet

	if(self->spawnflags & PLAYER_ONLY)
		if(apOther->GetClassName() != "player")
			return;

	// only teleport living creatures
	if(apOther->GetHealth() <= 0 || apOther->GetSolidity() != SOLID_SLIDEBOX)
		return;

	//SUB_UseTargets();

	// put a tfog where the player was
	//spawn_tfog(apOther->GetOrigin());

	CBaseEntity *t{nullptr}; // For the first time it will be the world entity
	t = mpGame->GetWorld()->FindEntityByString(t, "targetname", GetTarget());
	if(!t)
		objerror("couldn't find target");
		
	// spawn a tfog flash in front of the destination
	//gpEngine->pfnMakeVectors(t->mangle);
	//idVec3 org{t->GetOrigin() + 32 * gpGlobals->v_forward};

	//spawn_tfog(org);
	//spawn_tdeath(t->GetOrigin(), apOther);

	// move the player and lock him down for a little while
	if(!apOther->GetHealth())
	{
		apOther->SetOrigin(t->GetOrigin());
		apOther->SetVelocity((idVec3(gpGlobals->v_forward) * apOther->GetVelocity().x) + (idVec3(gpGlobals->v_forward) * apOther->GetVelocity().y));
		return;
	};

	apOther->SetOrigin(t->GetOrigin());
	apOther->SetAngles(t->GetAngles() /*t->mangle*/);
	
	if (apOther->GetClassName() == "player")
	{
		apOther->self->fixangle = 1; // turn this way immediately // TODO
		apOther->self->teleport_time = gpGlobals->time + 0.7; // TODO
		if(apOther->HasFlags(FL_ONGROUND))
			apOther->RemoveFlags(FL_ONGROUND);
		apOther->SetVelocity(idVec3(gpGlobals->v_forward) * 300);
	};
	
	apOther->SetFlags(apOther->GetFlags() - apOther->GetFlags() & FL_ONGROUND);
};

void CBaseTrigger::CDAudioTouch(CBaseEntity *apOther)
{
};

// the trigger was just touched/killed/used
// self.enemy should be set to the activator so it can be held through a delay
// so wait for the delay time before firing
void CBaseTrigger::ActivateMultiTrigger(CBaseEntity *apActivator)
{
	// already been triggered
	if(GetNextThink() > gpGlobals->time)
		return;

	if(GetClassName() == "trigger_secret")
	{
		if(GetEnemy()->GetClassName() != "player")
			return;
		gpGlobals->found_secrets++;
		//gpEngine->pfnWriteByte(MSG_ALL, SVC_FOUNDSECRET);
	};

	if(self->noise)
		EmitSound(CHAN_VOICE, GetNoise(), 1, ATTN_NORM);

	// don't trigger again until reset
	SetDamageable(DAMAGE_NO);

	mhActivator = apActivator;
	
	SUB_UseTargets(mhActivator, UseType::Toggle, 0);

	if(mfDelay > 0)
	{
		SetThinkCallback(CBaseTrigger::MultiWaitOver);
		SetNextThink(gpGlobals->time + mfDelay);
	}
	else
	{
		// we can't just remove (self) here, because this is a touch function
		// called while C code is looping through area links...
		SetTouchCallback(nullptr); // TODO: was SUB_Null but args are different
		SetNextThink(gpGlobals->time + 0.1);
		SetThinkCallback(CBaseTrigger::SUB_Remove);
	};
};

// the wait time has passed, so set back up for another activation
void CBaseTrigger::MultiWaitOver()
{
	//if(GetMaxHealth())
	{
		//SetHealth(GetMaxHealth());
		//SetDamageable(DAMAGE_YES);
		//SetSolidity(SOLID_BBOX);
	};
	
	SetThinkCallback(nullptr);
};

bool CBaseTrigger::HandleKeyValue(const std::string &asKey, const std::string &asValue)
{
	if(asKey == "damage")
	{
		self->dmg = std::stof(asValue);
		return true;
	}
	else if(asKey == "count")
	{
		// TODO
		return true;
	}
	else if(asKey == "damagetype")
	{
		// TODO
		return true;
	};
	
	return CBaseToggle::HandleKeyValue(asKey, asValue);
};