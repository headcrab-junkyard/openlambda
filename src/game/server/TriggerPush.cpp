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
/// @brief push trigger code

//#include "TriggerPush.hpp"
#include "BaseTrigger.hpp"
#include "Util.hpp"

//============================================================================

const int PUSH_ONCE{1};

/*QUAKED trigger_push (.5 .5 .5) ? PUSH_ONCE
Pushes the player
*/
class CTriggerPush : public CBaseTrigger
{
public:
	void Spawn() override;
	
	void Touch(CBaseEntity *apOther) override;
};

LINK_ENTITY_TO_CLASS(trigger_push, CTriggerPush);

void CTriggerPush::Spawn()
{
	InitTrigger();
	gpEngine->pfnPrecacheSound("ambience/windfly.wav");
	SetTouchCallback(CTriggerPush::Touch);
	if(!GetSpeed())
		SetSpeed(1000);
};

void CTriggerPush::Touch(CBaseEntity *apOther)
{
	if(apOther->GetClassName() == "grenade")
		apOther->SetVelocity(GetSpeed() * GetMoveDir() * 10);
	else if(apOther->GetHealth() > 0)
	{
		apOther->SetVelocity(GetSpeed() * GetMoveDir() * 10);
		if(apOther->GetClassName() == "player")
		{
			if(apOther->fly_sound < gpGlobals->time)
			{
				apOther->fly_sound = gpGlobals->time + 1.5;
				apOther->EmitSound(CHAN_AUTO, "ambience/windfly.wav", 1, ATTN_NORM);
			};
		};
	};
	
	if(self->spawnflags & PUSH_ONCE)
		gpEngine->pfnRemove(ToEdict());
};