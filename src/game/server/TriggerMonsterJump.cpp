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
/// @brief monster jump trigger code

#include "TriggerMonsterJump.hpp"

//============================================================================

/*QUAKED trigger_monsterjump (.5 .5 .5) ?
Walking monsters that touch this will jump in the direction of the trigger's angle
"speed" default to 200, the speed thrown forward
"height" default to 200, the speed thrown upwards
*/
class CTriggerMonsterJump : public CBaseTrigger
{
public:
	void Spawn() override;
	
	void Touch(CBaseEntity *apOther) override;
};

LINK_ENTITY_TO_CLASS(trigger_monsterjump, CTriggerMonsterJump);

void CTriggerMonsterJump::Spawn()
{
	if(!self->speed)
		self->speed = 200;
	if(!self->height)
		self->height = 200;
	if(self->angles == '0 0 0')
		self->angles = '0 360 0';
	InitTrigger();
	SetTouchCallback(CTriggerMonsterJump::Touch);
};

void CTriggerMonsterJump::Touch(CBaseEntity *apOther)
{
	if ( apOther->GetFlags() & (FL_MONSTER | FL_FLY | FL_SWIM) != FL_MONSTER )
		return;

// set XY even if not on ground, so the jump will clear lips
	apOther->velocity_x = self->movedir_x * self->GetSpeed();
	apOther->velocity_y = self->movedir_y * self->GetSpeed();
	
	if(!(apOther->GetFlags() & FL_ONGROUND))
		return;
	
	apOther->flags = apOther->GetFlags() - FL_ONGROUND;

	apOther->velocity_z = self->height;
};