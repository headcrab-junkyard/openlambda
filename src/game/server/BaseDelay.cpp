/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2019-2021, 2023 BlackPhrase
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
#include "Util.hpp"

//=============================================================================

LINK_ENTITY_TO_CLASS(DelayedUse, CBaseDelay);

bool CBaseDelay::HandleKeyValue(ogs::tStringView asKey, ogs::tStringView asValue)
{
	if(asKey == "delay")
	{
		mfDelay = std::stof(asValue.data());
		return true;
	}
	else if(asKey == "killtarget")
	{
		mnKillTargetString = gpEngine->pfnAllocString(asValue.c_str());
		return true;
	};
	
	return CBaseEntity::HandleKeyValue(asKey, asValue);
};

void CBaseDelay::DelayThink()
{
	SUB_UseTargets(GetEnemy(), static_cast<UseType>(self->button), 0);
	mpWorld->RemoveEntity(this);
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
void CBaseDelay::SUB_UseTargets(CBaseEntity *apActivator, UseType aeUseType, float afValue)
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
		
		//t->SetEnemy(apActivator); // TODO: ?
		//t->message = self->message; // TODO: unused
		
		t->mnKillTargetString = mnKillTargetString;
		t->SetTarget(GetTarget());
		
		t->mfDelay = 0.0f; // Prevent "recursion"
		
		t->SetOwner(nullptr);
		
		if(apActivator && apActivator->GetClassName() == "player")
			t->SetOwner(apActivator);
		
		return;
	};
	
	//
	// print the message
	//
	// TODO: UNUSED
	/*
	if (apActivator->GetClassName() == "player" && self->message != "")
	{
		gpEngine->pfnCenterPrint(apActivator, self->message);
		if(!self->noise)
			apActivator->EmitSound(CHAN_VOICE, "misc/talk.wav", 1, ATTN_NORM);
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
			t = mpWorld->FindEntityByString(t, "targetname", mnKillTargetString);
			if(!t)
				return;
			mpWorld->RemoveEntity(t);
		}
		while(1);
	};
	
	//
	// fire targets
	//
	CBaseEntity *stemp, *otemp, *act;
	
	if(GetTarget())
	{
		act = apActivator;
		t = world;
		
		do
		{
			t = gpEngine->pfnFindEntityByString(t, "targetname", GetTarget());
			
			if(!t)
				return;
			
			stemp = self;
			otemp = other;
			self = t;
			other = stemp;
			
			if(!t->HasFlags(FL_KILLME))
				t->Use(apActivator, this, aeUseType, afValue);
			
			self = stemp;
			other = otemp;
			apActivator = act;
		}
		while(1);
	};
};