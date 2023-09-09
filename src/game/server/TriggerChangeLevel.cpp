/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2019, 2021-2023 BlackPhrase
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
/// @brief a trigger that is used for changing levels

#include "BaseTrigger.hpp"

/*QUAKED trigger_changelevel (0.5 0.5 0.5) ? NO_INTERMISSION
When the player touches this, he gets sent to the map listed in the "map" variable.  Unless the NO_INTERMISSION flag is set, the view will go to the info_intermission spot and display stats.
*/
class CTriggerChangeLevel : public CBaseTrigger
{
public:
	void Spawn() override;
	
	bool HandleKeyValue(ogs::tStringView asKey, ogs::tStringView asValue) override;
	
	void Touch(CBaseEntity *other) override;
private:
	void Execute();
};

LINK_ENTITY_TO_CLASS(trigger_changelevel, CTriggerChangeLevel);

void CTriggerChangeLevel::Spawn()
{
	if(!self->map)
		objerror("chagnelevel trigger doesn't have map");
	
	InitTrigger(); // TODO: CBaseTrigger::Spawn?
	SetTouchCallback(CTriggerChangeLevel::Touch);
};

bool CTriggerChangeLevel::HandleKeyValue(ogs::tStringView asKey, ogs::tStringView asValue)
{
	if(asKey == "map")
	{
		return true;
	}
	else if(asKey == "landmark")
	{
		return true;
	}
	else if(asKey == "changetarget")
	{
		return true;
	}
	else if(asKey == "changedelay")
	{
		return true;
	};
	
	return CBaseTrigger::HandleKeyValue(asKey, asValue);
};

void CTriggerChangeLevel::Touch(CBaseEntity *apOther)
{
	if(apOther->GetClassName() != "player")
		return;

	//if(!gamerules->CanChangeLevel())
		//return;
	
// if "noexit" is set, blow up the player trying to leave
//ZOID, 12-13-96, noexit isn't supported in QW.  Overload samelevel
//      if ((cvar("noexit") == 1) || ((cvar("noexit") == 2) && (mapname != "start")))
	if ((cvar("samelevel") == 2) || ((cvar("samelevel") == 3) && (mapname != "start")))
	{
		apOther->TakeDamage(this, this, 50000);
		return;
	};

	gpEngine->pfnBroadcastPrint(PRINT_HIGH, apOther->v.netname);
	gpEngine->pfnBroadcastPrint(PRINT_HIGH," exited the level\n");
	
	nextmap = self->map;

	SUB_UseTargets(apOther, USE_TOGGLE, 0);

	SetTouchCallback(SUB_Null);

	// we can't move people right now, because touch functions are called
	// in the middle of C movement code, so set a think time to do it
	SetThinkCallback(CTriggerChangeLevel::Execute);
	SetNextThink(gpGlobals->time + 0.1);
};

/*
============
execute_changelevel

The global "nextmap" has been set previously.
Take the players to the intermission spot
============
*/
void CTriggerChangeLevel::Execute()
{
	intermission_running = 1;
	
	// enforce a wait time before allowing changelevel
	intermission_exittime = gpGlobals->time + 5;

	// play intermission music
	gpEngine->pfnMessageBegin(MSG_ALL, SVC_CDTRACK);
	gpEngine->pfnWriteByte(3);
	gpEngine->pfnWriteByte(3);
	gpEngine->pfnMessageEnd();

	CBaseEntity *pos{FindIntermission()};
	
	gpEngine->pfnWriteByte(MSG_ALL, SVC_INTERMISSION);
	gpEngine->pfnWriteCoord(MSG_ALL, pos->GetOrigin().x);
	gpEngine->pfnWriteCoord(MSG_ALL, pos->GetOrigin().y);
	gpEngine->pfnWriteCoord(MSG_ALL, pos->GetOrigin().z);
	gpEngine->pfnWriteAngle(MSG_ALL, pos->mangle_x);
	gpEngine->pfnWriteAngle(MSG_ALL, pos->mangle_y);
	gpEngine->pfnWriteAngle(MSG_ALL, pos->mangle_z);
	gpEngine->pfnMessageEnd();
	
	CBaseEntity *other{mpWorld->FindEntityByString(world, "classname", "player")};
	while(other != world)
	{
		other->SetDamageable(DAMAGE_NO);
		other->SetSolidity(SOLID_NOT);
		other->SetMoveType(MOVETYPE_NONE);
		other->SetModelIndex(0);
		other = mpWorld->FindEntityByString(other, "classname", "player");
	};
};