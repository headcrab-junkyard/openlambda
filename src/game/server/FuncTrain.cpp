/*
	plats.qc

	platform functions

	Copyright (C) 1996-1997  Id Software, Inc.

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

	See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to:

		Free Software Foundation, Inc.
		59 Temple Place - Suite 330
		Boston, MA  02111-1307, USA

*/

/// @file

#include "BaseEntity.hpp"

/*QUAKED func_train (0 .5 .8) ?
Trains are moving platforms that players can ride.
The targets origin specifies the min point of the train at each corner.
The train spawns at the first target it is pointing at.
If the train is the target of a button or trigger, it will not begin moving until activated.
speed	default 100
dmg		default	2
sounds
1) ratchet metal

*/
class CFuncTrain : public CBaseEntity
{
public:
	//CFuncTrain();
	//~CFuncTrain();
	
	void Spawn() override;
	
	void Blocked(CBaseEntity *apOther) override;
	void Use(CBaseEntity *apActivator) override;
	
	void Wait();
	
	bool HandleKeyValue(const std::string &asKey, const std::string &asValue) override;
private:
	void Next();
};

LINK_ENTITY_TO_CLASS(func_train, CFuncTrain);

//CFuncTrain::CFuncTrain() = default;
//CFuncTrain::~CFuncTrain() = default;

void CFuncTrain::Spawn()
{
	//
	//Precache();
	//
	switch(self->sounds)
	{
	case 0:
		gpEngine->pfnPrecacheSound("misc/null.wav");
		self->noise = "misc/null.wav";
		self->noise1 = "misc/null.wav";
		break;
	case 1:
		gpEngine->pfnPrecacheSound("plats/train1.wav");
		gpEngine->pfnPrecacheSound("plats/train2.wav");
		self->noise = gpEngine->pfnMakeString("plats/train2.wav");
		self->noise1 = gpEngine->pfnMakeString("plats/train1.wav");
		break;
	case 2:
		gpEngine->pfnPrecacheSound("plats/platmove1.wav");
		gpEngine->pfnPrecacheSound("plats/platstop1.wav");
		self->noise = gpEngine->pfnMakeString("plats/platstop1.wav");
		self->noise1 = gpEngine->pfnMakeString("plats/platmove1.wav");
		break;
	};
	//
	
	if(!GetSpeed())
		SetSpeed(100);
	
	if(!self->target)
		objerror("func_train without a target");
	
	if(!self->dmg)
		self->dmg = 2;

	//self->cnt = 1;
	SetSolidity(SOLID_BSP);
	
	if(self->spawnflags & SF_TRACTTRAIN_PASSABLE)
		SetSolidity(SOLID_NOT);
	
	SetMoveType(MOVETYPE_PUSH);
	
	//SetBlockedCallback(train_blocked);
	//SetUseCallback(train_use);
	
	//SetClassName("train");

	SetModel(GetModel());
	SetSize(self->mins , self->maxs);
	SetOrigin(GetOrigin());

	// start trains on the second frame, to make sure their targets have had a chance to spawn
	//SetNextThink(self->ltime + 0.1);
	//SetThinkCallback(func_train_find);
	
	mbActivated = false;
	
	if(mfVolume == 0.0)
		mfVolume = 0.85;
};

void CFuncTrain::Blocked(CBaseEntity *other)
{
	if(gpGlobals->time < self->attack_finished)
		return;
	self->attack_finished = gpGlobals->time + 0.5;
	//other->deathtype = "squish";
	other->TakeDamage(self, self, self->dmg, DMG_CRUSH);
};

void CFuncTrain::Use(CBaseEntity *apActivator, CBaseEntity *apCaller, UseType aeUseType, float afValue)
{
	if(self->think != func_train_find)
		return; // already activated
	Next();
};

void CFuncTrain::Wait()
{
	if(self->wait)
	{
		SetNextThink(self->ltime + self->wait);
		EmitSound(CHAN_NO_PHS_ADD + CHAN_VOICE, self->noise, 1, ATTN_NORM);
	}
	else
		SetNextThink(self->ltime + 0.1);
	
	SetThinkCallback(CFuncTrain::Next);
};

void CFuncTrain::Next()
{
	CBaseEntity *targ{gpEngine->pfnFindEntityByString(world, targetname, self->target)};
	self->target = targ->target;
	if(!self->target)
		objerror("train_next: no next target");
	if(targ->wait)
		self->wait = targ->wait;
	else
		self->wait = 0;
	EmitSound(CHAN_VOICE, self->noise1, 1, ATTN_NORM);
	SUB_CalcMove(targ->GetOrigin() - self->mins, GetSpeed(), train_wait);
};

bool CFuncTrain::HandleKeyValue(const std::string &asKey, const std::string &asValue)
{
	if(asKey == "sounds")
	{
		mSounds = std::stoi(asValue);
		return true;
	};
	
	return CBasePlatTrain::HandleKeyValue(asKey, asValue);
};