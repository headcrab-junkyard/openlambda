/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2021 BlackPhrase
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
/// @brief multiple trigger code

//#include "TriggerMulti.hpp"
#include "BaseTrigger.hpp"

//=============================================================================

const float	SPAWNFLAG_NOMESSAGE{1};
const float	SPAWNFLAG_NOTOUCH{1};

/*QUAKED trigger_multiple (.5 .5 .5) ? notouch
Variable sized repeatable trigger.  Must be targeted at one or more entities.  If "health" is set, the trigger must be killed to activate each time.
If "delay" is set, the trigger waits some time after activating before firing.
"wait" : Seconds between triggerings. (.2 default)
If notouch is set, the trigger is only fired by other entities, not by touching.
NOTOUCH has been obsoleted by trigger_relay!
sounds
1)	secret
2)	beep beep
3)	large switch
4)
set "message" to text string
*/
class CTriggerMulti : public CBaseTrigger
{
public:
	void Spawn() override;
	
	void Use(CBaseEntity *apActivator) override;
	
	void Killed() override;
};

LINK_ENTITY_TO_CLASS(trigger_multiple, CTriggerMulti);

void CTriggerMulti::Spawn()
{
	/*
	if(self->sounds == 1)
	{
		gpEngine->pfnPrecacheSound ("misc/secret.wav");
		self->noise = "misc/secret.wav";
	}
	else if(self->sounds == 2)
	{
		gpEngine->pfnPrecacheSound ("misc/talk.wav");
		self->noise = "misc/talk.wav";
	}
	else if(self->sounds == 3)
	{
		gpEngine->pfnPrecacheSound ("misc/trigger1.wav");
		self->noise = "misc/trigger1.wav";
	};
	*/
	
	if(!mfWait)
		mfWait = 0.2;
	
	SetUseCallback(CTriggerMulti::Use);

	InitTrigger();

	if(GetHealth() > 0)
	{
		if(self->spawnflags & SPAWNFLAG_NOTOUCH)
			objerror("health and notouch don't make sense\n");
		SetMaxHealth(GetHealth());
		self->th_die = CTriggerMulti::Killed;
		SetDamageable(DAMAGE_YES);
		SetSolidity(SOLID_BBOX);
		SetOrigin(GetOrigin());	// make sure it links into the world
	}
	else
	{
		if(!(self.spawnflags & SPAWNFLAG_NOTOUCH))
			SetTouchCallback(CTriggerMulti::Touch);
	};
};

void CTriggerMulti::Killed()
{
	SetEnemy(damage_attacker);
	multi_trigger();
};

void CTriggerMulti::Use(CBaseEntity *apActivator)
{
	SetEnemy(apActivator);
	multi_trigger();
};