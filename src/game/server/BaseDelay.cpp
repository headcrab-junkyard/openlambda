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

#include "BaseDelay.hpp"

//=============================================================================

LINK_ENTITY_TO_CLASS(DelayedUse, CBaseDelay);

bool CBaseDelay::HandleKeyValue(const std::string &asKey, const std::string &asValue)
{
	if(asKey == "delay")
	{
		mfDelay = std::stof(asValue);
		return true;
	}
	else if(asKey == "killtarget")
	{
		mnKillTargetString = gpEngine->pfnAllocString(asValue);
		return true;
	};
	
	return CBaseEntity::HandleKeyValue(asKey, asValue);
};

void CBaseDelay::DelayThink()
{
	SUB_UseTargets(GetEnemy());
	gpEngine->pfnRemove(ToEdict());
};

/*
==============================
SUB_UseTargets

the global "activator" should be set to the entity that initiated the firing.

If self.delay is set, a DelayedUse entity will be created that will actually
do the SUB_UseTargets after that many seconds have passed.

Centerprints any self.message to the activator.

Removes all entities with a targetname that match self.killtarget,
and removes them, so some events can remove other triggers.

Search for (string)targetname in all entities that
match (string)self.target and call their .use function

==============================
*/
void CBaseDelay::SUB_UseTargets(CBaseEntity *activator, UseType aeUseType, float afValue)
{
	//
	// check for a delay
	//
	if(mfDelay != 0.0f)
	{
		// create a temp object to fire at a later time
		CBaseDelay *t{mpWorld->SpawnEntity()};
		
		t->SetClassName("DelayedUse");
		t->SetNextThink(gpGlobals->time + mfDelay);
		t->SetThinkCallback(CBaseDelay::DelayThink);
		
		// Save the use type
		t->self->button = (int)aeUseType;
		
		//t->SetEnemy(activator); // TODO: ?
		//t->message = self->message; // TODO: unused
		
		t->mnKillTargetString = mnKillTargetString;
		t->SetTarget(self->target);
		
		t->mfDelay = 0.0f; // Prevent "recursion"
		
		t->SetOwner(nullptr);
		
		if(false)
			t->SetOwner(activator);
		
		return;
	};
	
	//
	// print the message
	//
	// TODO: UNUSED
	/*
	if (activator->GetClassName() == "player" && self.message != "")
	{
		gpEngine->pfnCenterPrint (activator, self->message);
		if (!self->noise)
			activator->EmitSound(CHAN_VOICE, "misc/talk.wav", 1, ATTN_NORM, PITCH_NORM);
	};
	*/

	//
	// kill the killtagets
	//
	if(mnKillTargetString)
	{
		t = world;
		do
		{
			t = gpEngine->pfnFind(t, targetname, mnKillTargetString);
			if(!t)
				return;
			gpEngine->pfnRemove(t);
		}
		while(1);
	};
	
	//
	// fire targets
	//
	CBaseEntity *stemp, *otemp, *act;
	
	if(self->target)
	{
		act = activator;
		t = world;
		
		do
		{
			t = gpEngine->pfnFind(t, targetname, self->target);
			
			if(!t)
				return;
			
			stemp = self;
			otemp = other;
			self = t;
			other = stemp;
			
			if(self->GetUseCallback() != SUB_Null)
				self->Use();
			
			self = stemp;
			other = otemp;
			activator = act;
		}
		while(1);
	};
};