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

#include "TriggerPush.hpp"

//============================================================================

const float PUSH_ONCE = 1;

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
	if(!self->GetSpeed())
		self->speed = 1000;
};

void CTriggerPush::Touch(CBaseEntity *apOther)
{
	if(other->GetClassName() == "grenade")
		other->SetVelocity(self->GetSpeed() * self->GetMoveDir() * 10);
	else if(other->GetHealth() > 0)
	{
		other->SetVelocity(self->GetSpeed() * self->GetMoveDir() * 10);
		if(other->GetClassName() == "player")
		{
			if(other->fly_sound < gpGlobals->time)
			{
				other->fly_sound = gpGlobals->time + 1.5;
				other->EmitSound(CHAN_AUTO, "ambience/windfly.wav", 1, ATTN_NORM);
			};
		};
	};
	
	if(self->spawnflags & PUSH_ONCE)
		gpEngine->pfnRemove(ToEdict());
};