/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-2001 Id Software, Inc.
 * Copyright (C) 2018-2020 BlackPhrase
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
/// @brief player functions/definitions

#include "BasePlayer.hpp"
#include "Game.hpp"
#include "Util.hpp"

LINK_ENTITY_TO_CLASS(player, CBasePlayer)

CBasePlayer::CBasePlayer() : mpGame(gpGame){}

void CBasePlayer::Spawn()
{
	self->classname = "player";
	SetMaxHealth(100);
	SetHealth(GetMaxHealth());
	self->takedamage = DAMAGE_AIM;
	SetSolidity(SOLID_SLIDEBOX);
	SetMoveType(MOVETYPE_WALK);
	self->show_hostile = 0;
	SetFlags(FL_CLIENT);
	self->air_finished = gpGlobals->time + 12;
	self->dmg = 2; // initial water damage
	
/*
	self->super_damage_finished = 0;
	self->radsuit_finished = 0;
	self->invisible_finished = 0;
	self->invincible_finished = 0;
*/
	SetEffects(0); // TODO: ClearEffects?
	self->invincible_time = 0;
	
	DecodeLevelParms();
	
	W_SetCurrentAmmo();

	self->attack_finished = gpGlobals->time;
	self->th_pain = player_pain;
	self->th_die = PlayerDie;
	
	self->deadflag = DEAD_NO;

	// pausetime is set by teleporters to keep the player from moving a while
	self->pausetime = 0;
	
	// oh, this is a hack!
	SetModel("models/eyes.mdl");
	modelindex_eyes = self->modelindex;

	SetModel("models/player.mdl");
	modelindex_player = self->modelindex;

	SetSize(VEC_HULL_MIN, VEC_HULL_MAX);
	
	mpGame->GetRules()->OnPlayerSpawn(this);
};