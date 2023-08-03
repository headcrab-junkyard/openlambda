/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2019, 2021, 2023 BlackPhrase
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

#include "BaseGameRules.hpp"

bool CBaseGameRules::ShouldCollide(const CBaseEntity &lhs, const CBaseEntity &rhs) const
{
	// TODO
	return true;
};

/*
============
CanDamage

Returns true if the inflictor can directly damage the target.  Used for
explosions and melee attacks.
============
*/
/*
bool CBaseGameRules::CanDamage(CBaseEntity *targ, CBaseEntity *inflictor)
{
	// bmodels need special checking because their origin is 0,0,0
	if (targ->GetMoveType() == MOVETYPE_PUSH)
	{
		traceline(inflictor->GetOrigin(), 0.5 * (targ.absmin + targ.absmax), TRUE, self);
		if (trace_fraction == 1)
			return true;
		if (trace_ent == targ)
			return true;
		return false;
	};
	
	traceline(inflictor->GetOrigin(), targ->GetOrigin(), TRUE, self);
	if (trace_fraction == 1)
		return true;
	traceline(inflictor->GetOrigin(), targ->GetOrigin() + '15 15 0', TRUE, self);
	if (trace_fraction == 1)
		return true;
	traceline(inflictor->GetOrigin(), targ->GetOrigin() + '-15 -15 0', TRUE, self);
	if (trace_fraction == 1)
		return true;
	traceline(inflictor->GetOrigin(), targ->GetOrigin() + '-15 15 0', TRUE, self);
	if (trace_fraction == 1)
		return true;
	traceline(inflictor->GetOrigin(), targ->GetOrigin() + '15 -15 0', TRUE, self);
	if (trace_fraction == 1)
		return true;

	return false;
};
*/

bool CBaseGameRules::HandleClientConnect(CBaseEntity *apEntity, const char *asName, const char *asAdr, char sRejectReason[128])
{
	//BroadcastPrint(PRINT_HIGH, va("%s entered the game\n", pEntity->v.netname));

	// A client connecting during an intermission can cause problems
	//if(intermission_running)
		//GotoNextMap();
	
	return true;
};

void CBaseGameRules::HandleClientDisconnect(CBaseEntity *apEntity)
{
/*
	// Let everyone else know
	BroadcastPrint(PRINT_HIGH, self->v.netname);
	BroadcastPrint(PRINT_HIGH, " left the game with ");
	BroadcastPrint(PRINT_HIGH, ftos(self->GetFrags()));
	BroadcastPrint(PRINT_HIGH, " frags\n");
	
	apEntity->EmitSound(CHAN_BODY, "player/tornoff2.wav", 1, ATTN_NONE);
	set_suicide_frame(self->v);
*/
};