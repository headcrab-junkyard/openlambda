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
/// @brief train entity code

#include "BaseToggle.hpp"
#include "Util.hpp"

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
class CFuncTrain : public CBaseToggle
{
public:
	//CFuncTrain();
	//~CFuncTrain();
	
	void Spawn() override;
	
	void Blocked(CBaseEntity *apOther) override;
	void Use(CBaseEntity *apActivator, CBaseEntity *apCaller, UseType aeUseType, float afValue) override;
	
	void Wait();
	
	bool HandleKeyValue(ogs::tStringView asKey, ogs::tStringView asValue) override;
private:
	void Next();
private: // TODO: public?
	int mnSounds{0};
	
	bool mbActivated{false};
};

LINK_ENTITY_TO_CLASS(func_train, CFuncTrain);

//CFuncTrain::CFuncTrain() = default;
//CFuncTrain::~CFuncTrain() = default;

void CFuncTrain::Spawn()
{
	//
	//Precache();
	//
	switch(mnSounds)
	{
	case 0:
		PrecacheSound("misc/null.wav");
		
		self->noise = gpEngine->pfnAllocString("misc/null.wav");
		self->noise1 = gpEngine->pfnAllocString("misc/null.wav");
		break;
	case 1:
		PrecacheSound("plats/train1.wav");
		PrecacheSound("plats/train2.wav");
		
		self->noise = gpEngine->pfnAllocString("plats/train2.wav");
		self->noise1 = gpEngine->pfnAllocString("plats/train1.wav");
		break;
	case 2:
		PrecacheSound("plats/platmove1.wav");
		PrecacheSound("plats/platstop1.wav");
		
		self->noise = gpEngine->pfnAllocString("plats/platstop1.wav");
		self->noise1 = gpEngine->pfnAllocString("plats/platmove1.wav");
		break;
	};
	//
	
	if(!GetSpeed())
		SetSpeed(100);
	
	if(!GetTarget())
		objerror("func_train without a target");
	
	if(!self->dmg)
		self->dmg = 2;

	//self->cnt = 1;
	SetSolidity(SOLID_BSP);
	
	if(self->spawnflags & SF_TRACTTRAIN_PASSABLE)
		SetSolidity(SOLID_NOT);
	
	SetMoveType(MOVETYPE_PUSH);
	
	//SetBlockedCallback(CFuncTrain::Blocked);
	//SetUseCallback(CFuncTrain::Use);
	
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

void CFuncTrain::Blocked(CBaseEntity *apOther)
{
	if(gpGlobals->time < self->attack_finished)
		return;
	self->attack_finished = gpGlobals->time + 0.5;
	//apOther->deathtype = "squish";
	apOther->TakeDamage(self, self, self->dmg, DMG_CRUSH);
};

void CFuncTrain::Use(CBaseEntity *apActivator, CBaseEntity *apCaller, UseType aeUseType, float afValue)
{
	if(self->think != func_train_find)
		return; // already activated
	Next();
};

void CFuncTrain::Wait()
{
	if(GetDelay())
	{
		SetNextThink(self->ltime + GetDelay());
		EmitSound(CHAN_NO_PHS_ADD + CHAN_VOICE, self->noise, 1, ATTN_NORM);
	}
	else
		SetNextThink(self->ltime + 0.1);
	
	SetThinkCallback(CFuncTrain::Next);
};

void CFuncTrain::Next()
{
	CBaseEntity *pTargetEnt{gpEngine->pfnFindEntityByString(world, "targetname", GetTarget())};
	SetTarget(pTargetEnt->GetTarget());
	if(!GetTarget())
		objerror("train_next: no next target");
	if(pTargetEnt->GetDelay())
		SetDelay(pTargetEnt->GetDelay());
	else
		SetDelay(0);
	EmitSound(CHAN_VOICE, self->noise1, 1, ATTN_NORM);
	SetMoveDoneCallback(CFuncTrain::Wait);
	LinearMove(pTargetEnt->GetOrigin() - GetSize().mins, GetSpeed());
};

bool CFuncTrain::HandleKeyValue(const std::string &asKey, const std::string &asValue)
{
	if(asKey == "sounds")
	{
		mnSounds = std::stoi(asValue);
		return true;
	};
	
	return CBasePlatTrain::HandleKeyValue(asKey, asValue);
};