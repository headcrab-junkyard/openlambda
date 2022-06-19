/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2019, 2021-2022 BlackPhrase
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

#include "BaseAmmo.hpp"
#include "Game.hpp"
#include "IGameRules.hpp"

void CBaseAmmo::Spawn()
{
	SetMoveType(MOVETYPE_TOSS);
	SetSolidity(SOLID_TRIGGER);
	SetSize(idVec3(-16, -16, 0), idVec3(16, 16, 16));
	SetOrigin(GetOrigin());
	
	SetTouchCallback(CBaseAmmo::Touch);
	
	PostSpawn();
};

void CBaseAmmo::Respawn()
{
	// Remove it in single player, or setup for respawning in deathmatch
	AddEffects(EF_NODRAW); // TODO: was SetModel(string_null);
	SetSolidity(SOLID_NOT);

	//if(gpGlobals->deathmatch != 2)
		//SetNextThink(gpGlobals->time + 30);

// Xian -- If playing in DM 3.0 mode, halve the time ammo respawns        

	//if(gpGlobals->deathmatch == 3 || gpGlobals->deathmatch == 5)        
		//SetNextThink(gpGlobals->time + 15);
	
	SetNextThink(gpGame->GetRules()->GetAmmoRespawnTime(this));

	SetThinkCallback(SUB_regen);
};

void CBaseAmmo::SUB_regen()
{
	SetTouchCallback(CBaseAmmo::Touch);
};

void CBaseAmmo::Touch(CBaseEntity *apOther)
{
	if(apOther->GetClassName() != "player")
		return;
	
	if(apOther->GetHealth() <= 0)
		return;

	if(GiveTo(apOther)) // TODO: OnPickup?
	{
		if(false)
			; //Respawn();
		else
		{
			SetTouchCallback(nullptr);
			SetThinkCallback(CBaseAmmo::SUB_Remove);
			SetNextThink(gpGlobals->time + 0.1);
		};
	};

/*
	// if the player was using his best weapon, change up to the new one if better          
	float best = apOther->GetBestWeapon();

	bound_other_ammo ();
	
	sprint (apOther, PRINT_LOW, "You got the ");
	sprint (apOther, PRINT_LOW, self->netname);
	sprint (other, PRINT_LOW, "\n");

	stuffcmd (apOther, "bf\n");

// change to a better weapon if appropriate

	if ( apOther->weapon == best )
	{
		self->weapon = apOther->GetBestWeapon();
		apOther->SetCurrentAmmo ();
	}

// if changed current ammo, update it
	apOther->SetCurrentAmmo();

	activator = apOther;
	SUB_UseTargets(); // fire all targets / killtargets
*/
};