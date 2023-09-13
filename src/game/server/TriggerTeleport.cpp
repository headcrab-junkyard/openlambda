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
/// @brief teleport trigger code

#include "BaseTrigger.hpp"
#include "Util.hpp"

//============================================================================

const int SILENT{2};

/*QUAKED trigger_teleport (.5 .5 .5) ? PLAYER_ONLY SILENT
Any object touching this will be transported to the corresponding info_teleport_destination entity. You must set the "target" field, and create an object with a "targetname" field that matches.

If the trigger_teleport has a targetname, it will only teleport entities when it has been fired.
*/
class CTriggerTeleport : public CBaseTrigger
{
public:
	void Spawn() override;
	
	void Use(CBaseEntity *apActivator, CBaseEntity *apCaller, UseType aeUseType, float afValue) override;
};

LINK_ENTITY_TO_CLASS(trigger_teleport, CTriggerTeleport);

void CTriggerTeleport::Spawn()
{
	InitTrigger();
	
	SetTouchCallback(CTriggerTeleport::TeleportTouch);
	
	// Find the destination 
	//if(!self->target)
		//objerror("no target");
	
	//SetUseCallback(CTriggerTeleport::Use);

	//if(!(GetSpawnFlags() & SILENT))
	{
		//PrecacheSound("ambience/hum1.wav");
		//idVec3 vOrigin{(self->mins + self->maxs) * 0.5};
		//gpEngine->pfnEmitAmbientSound(vOrigin, "ambience/hum1.wav",0.5 , ATTN_STATIC);
	};
};

void CTriggerTeleport::Use(CBaseEntity *apActivator, CBaseEntity *apCaller, UseType aeUseType, float afValue)
{
	SetNextThink(gpGlobals->time + 0.2);
	gpGlobals->force_retouch = 2; // Make sure even still objects get hit
	SetThinkCallback(SUB_Null);
};