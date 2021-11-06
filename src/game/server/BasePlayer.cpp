/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-2001 Id Software, Inc.
 * Copyright (C) 2018-2021 BlackPhrase
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
#include "IGameRules.hpp"
#include "BaseItem.hpp"

unsigned long modelindex_player{0};

CBasePlayer::CBasePlayer() : mpGame(gpGame){}

void CBasePlayer::Spawn()
{
	SetClassName("player");
	SetMaxHealth(100);
	SetHealth(GetMaxHealth());
	SetArmorValue(0);
	self->takedamage = DAMAGE_AIM;
	SetSolidity(SOLID_SLIDEBOX);
	SetMoveType(MOVETYPE_WALK);
	//self->show_hostile = 0; // TODO
	SetFlags(FL_CLIENT);
	self->air_finished = gpGlobals->time + 12; // TODO: level.time in q2
	self->dmg = 2; // initial water damage
	
	self->dmg_take = 0;
	self->dmg_save = 0;
	
	SetFriction(1.0);
	SetGravity(1.0);
	
/*
	self->super_damage_finished = 0;
	self->radsuit_finished = 0;
	self->invisible_finished = 0;
	self->invincible_finished = 0;
*/
	SetEffects(0); // TODO: ClearEffects?
	//self->invincible_time = 0; // TODO
	
	//DecodeLevelParms(); // TODO
	
	//W_SetCurrentAmmo(); // TODO

	//self->attack_finished = gpGlobals->time; // TODO
	//self->th_pain = player_pain; // TODO
	//self->th_die = PlayerDie; // TODO
	
	self->deadflag = DEAD_NO;

	// pausetime is set by teleporters to keep the player from moving a while
	//self->pausetime = 0; // TODO
	
	// oh, this is a hack!
	SetModel("models/eyes.mdl");
	//modelindex_eyes = self->modelindex; // TODO

	SetModel("models/player.mdl");
	modelindex_player = self->modelindex;

	SetSize(VEC_HULL_MIN, VEC_HULL_MAX);
	
	mpGame->GetRules()->OnPlayerSpawn(this);
};

int CBasePlayer::TakeDamage(const CBaseEntity &aInflictor, const CBaseEntity &aAttacker, float afDamage, int anDmgTypeBitSum)
{
	// Already ded :P
	if(!IsAlive())
		return 0;
	
	if(!mpGame->GetRules()->PlayerCanTakeDamage(*this, aAttacker)) // TODO: this also can be moved inside the game rules' PlayerTakeDamage method
		return 0;
	
	float fDmgTaken{0.0f};
	
	fDmgTaken = mpGame->GetRules()->PlayerTakeDamage(*this, aInflictor, aAttacker, afDamage, anDmgTypeBitSum);
	
	return fDmgTaken;
};

void CBasePlayer::RemoveAllItems(int anFlags)
{
};

void CBasePlayer::Killed(const CBaseEntity &aAttacker, const CBaseEntity &aLastInflictor, int anGib)
{
	mpGame->GetRules()->OnPlayerKilled(this, aAttacker, aLastInflictor);
	
	SetAnimation(PLAYER_DIE);
	
	DeathSound();
	
	SetThinkCallback(CBasePlayer::DeathThink);
	SetNextThink(gpGlobals->time + 0.1);
};

void CBasePlayer::SetAnimation(eAnimType aeAnimType)
{
	if(self->flags & FL_FROZEN)
	{
		aeAnimType = PLAYER_IDLE;
	};
	
	switch(aeAnimType)
	{
		// TODO:
	};
};

void CBasePlayer::DeathThink()
{
	respawn(self);
	SetNextThink(-1);
};

void CBasePlayer::Duck()
{
	if((self->button & IN_DUCK) && GetIdealActivity() != ActivityType::Leap)
		SetAnimation(PLAYER_WALK);
};

void CBasePlayer::PreThink()
{
	mpGame->GetRules()->OnPlayerThink(this);
	
	ItemPreFrame();
	WaterMove();
	
	if(self->deadflag >= DEAD_DYING)
	{
		DeathThink();
		return;
	};
};

void CBasePlayer::PostThink()
{
	ItemPostFrame();
};

void CBasePlayer::SelectNextItem(int anItemID)
{
};

void CBasePlayer::SelectPrevItem(int anItemID)
{
	// NOTE: nothing
};

void CBasePlayer::SelectItem(const char *asItem)
{
};

void CBasePlayer::SelectLastItem()
{
};

void CBasePlayer::GiveNamedItem(const char *asName)
{
};

void CBasePlayer::HandleImpulseCommands()
{
	switch(self->impulse)
	{
	default:
		HandleCheatImpulse(self->impulse);
		break;
	};
	
	self->impulse = 0;
};

void CBasePlayer::HandleCheatImpulse(int anImpulse)
{
};

bool CBasePlayer::AddItem(const CBaseItem &aItem)
{
	return false;
};

bool CBasePlayer::RemoveItem(const CBaseItem &aItem)
{
	return false;
};

void CBasePlayer::ItemPreFrame()
{
	if(mpActiveItem)
		mpActiveItem->ItemPreFrame();
};

void CBasePlayer::ItemPostFrame()
{
	if(mpActiveItem)
		mpActiveItem->ItemPostFrame();
};