/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018-2022 BlackPhrase
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

#include "BaseEntity.hpp"
#include "GameWorld.hpp"
#include "engine.h"
//#include "const.h"
#include "weapons.hpp"

enum class TargetEngineAPI : int
{
	Legacy,
	Next
};

#ifdef OPENLAMBDA_USE_LEGACY_ENGINE_API
	constexpr auto TargetAPI{TargetEngineAPI::Legacy};
#else
	constexpr auto TargetAPI{TargetEngineAPI::Next};
#endif

edict_t *CBaseEntity::ToEdict() const
{
	if constexpr(TargetAPI == TargetEngineAPI::Next)
		return nullptr; // TODO
	else
		return gpEngine->pfnFindEntityByVars(self);
};

/*
==============================================================================
BULLETS
==============================================================================
*/

/*
================
TraceAttack
================
*/
void CBaseEntity::TraceAttack(CBaseEntity *apAttacker, float afDamage, const idVec3 &avDir, /*const*/ TraceResult &aTraceResult, int anDmgBitSum)
{
	idVec3 org{aTraceResult.vecEndPos - avDir * 4};

	if(aTraceResult.pHit->v.takedamage)
	{
		//blood_count++; // TODO
		//blood_org = org; // TODO
		mpWorld->SpawnBlood(org, GetBloodType(), afDamage);
		
		AddMultiDamage(apAttacker, ToBaseEntity(aTraceResult.pHit), afDamage, anDmgBitSum);
	}
	//else
		//puff_count++; // TODO
};

/*
============
T_Damage

The damage is coming from inflictor, but get mad at attacker
This should be the only function that ever reduces health.
============
*/
int CBaseEntity::TakeDamage(CBaseEntity *inflictor, CBaseEntity *attacker, float afDamage, int anDmgBitSum)
{
	idVec3 dir;

	if(!IsDamageable())
		return 0;

	//----(SA)	added
	//if( g_gametype.integer == GT_SINGLE_PLAYER && !targ->aiCharacter && targ->client && targ->client->cameraPortal )
	{
		// get out of damage in sp if in cutscene.
		//return;
	};
	//----(SA)	end

//	if (reloading || saveGamePending) {	// map transition is happening, don't do anything
	//if ( g_reloading.integer || saveGamePending )
		//return;

	// the intermission has allready been qualified for, so don't
	// allow any extra scoring
	//if ( level.intermissionQueued )
		//return;
	
// used by buttons and triggers to set activator for target firing
	//damage_attacker = attacker;

/*
// check for quad damage powerup on the attacker
	if(attacker->super_damage_finished > gpGlobals->time && inflictor->GetClassName() != "door")
	{
		if(deathmatch == 4)
			afDamage *= 8;
		else
			afDamage *= 4;
	};
*/

// save damage based on the target's armor level

	float save = ceil(GetArmorType() * afDamage); // TODO: int?
/*
	if (save >= GetArmorValue())
	{
		save = GetArmorValue();
		SetArmorType(0);     // lost all armor
		self->items = self->items - (self->items & (IT_ARMOR1 | IT_ARMOR2 | IT_ARMOR3));
	};
	
	SetArmorValue(GetArmorValue() - save);
*/

	float take = ceil(afDamage - save); // TODO: int?

/*
// add to the damage total for clients, which will be sent as a single
// message at the end of the frame
// FIXME: remove after combining shotgun blasts?
	if (GetFlags() & FL_CLIENT)
	{
		self->dmg_take += take;
		self->dmg_save += save;
		self->dmg_inflictor = inflictor;
	};

	damage_inflictor = inflictor;
*/

	// figure momentum add
	if((inflictor != world) && (GetMoveType() == MOVETYPE_WALK))
	{
		dir = GetOrigin() - (inflictor->GetAbsMin() + inflictor->GetAbsMax()) * 0.5;
		dir = dir.Normalize();
		
		// Set kickback for smaller weapons
//Zoid -- use normal NQ kickback
//		// Read: only if it's not yourself doing the damage
//		if ( (afDamage < 60) & ((attacker->v.classname == "player") & (self->v.classname == "player")) & ( attacker->v.netname != self->v.netname)) 
//			self->v.velocity = self->v.velocity + dir * afDamage * 11;
//		else
		{
			// Otherwise, these rules apply to rockets and grenades                        
			// for blast velocity
			
			float fForce{afDamage * 8};
			
			if(fForce > 1000.0f)
				fForce = 1000.0f;
			
			SetVelocity(GetVelocity() + dir * fForce);
		};
		
		// Rocket Jump modifiers
		//if ( (rj > 1) & ((attacker->GetClassName() == "player") & (GetClassName() == "player")) & ( attacker->v.netname == self->v.netname)) 
			//SetVelocity(GetVelocity() + dir * afDamage * rj);
	};

/*
	// check for godmode or invincibility
	if(GetFlags() & FL_GODMODE)
		return 0;
	
	if(self->invincible_finished >= gpGlobals->time)
	{
		if(self->invincible_sound < gpGlobals->time)
		{
			EmitSound(CHAN_ITEM, "items/protect3.wav", 1, ATTN_NORM);
			self->invincible_sound = gpGlobals->time + 2;
		};
		return 0;
	};

// team play damage avoidance
//ZOID 12-13-96: self.team doesn't work in QW.  Use keys
	char *attackerteam = gpEngine->pfnInfoKeyValue(gpEngine->pfnGetInfoKeyBuffer(attacker->ToEdict()), "team");
	char *targteam = gpEngine->pfnInfoKeyValue(gpEngine->pfnGetInfoKeyBuffer(gpEngine->pfnEntOfEntVars(self)), "team");

	if ((teamplay == 1) && (targteam == attackerteam) &&
		(attacker->GetClassName() == "player") && (attackerteam != "") &&
		inflictor->GetClassName() != "door")
		return 0;

	if ((teamplay == 3) && (targteam == attackerteam) &&
		(attacker->GetClassName() == "player") && (attackerteam != "") &&
		(self != attacker)&& inflictor->GetClassName() != "door")
		return 0;
*/
	
	// do the damage
	AddHealth(-take);

	if(GetHealth() <= 0)
	{
		Killed(attacker, inflictor, GIB_NORMAL); // TODO
		return 0;
	};

	// UNUSED
	
	// react to the damage
	//CBaseEntity *oldself{self};

/*SERVER
	if ( (self.flags & FL_MONSTER) && attacker != world)
	{
	// get mad unless of the same class (except for soldiers)
		if (self != attacker && attacker != self->GetEnemy())
		{
			if ( (self.classname != attacker.classname) 
			|| (self.classname == "monster_army" ) )
			{
				if (GetEnemy()->GetClassName() == "player")
					self.oldenemy = GetEnemy();
				SetEnemy(attacker);
				FoundTarget ();
			}
		}
	}
*/
	//if(self->th_pain)
		//self->th_pain(attacker, take);

	//self = oldself;
	return 1;
};

/*
============
Killed
============
*/
void CBaseEntity::Killed(CBaseEntity *apAttacker, CBaseEntity *apLastInflictor, int anGib)
{
	//CBaseEntity *oself{self};

	/*
	if(GetMoveType() == MOVETYPE_PUSH || GetMoveType() == MOVETYPE_NONE)
	{
		// doors, triggers, etc
		self->th_die();
		self = oself;
		return;
	};
	*/
	
	/*
	// bump the monster counter
	if(GetFlags() & FL_MONSTER)
	{
		gpGlobals->killed_monsters++;
		gpEngine->pfnMessageBegin(MSG_ALL, SVC_KILLEDMONSTER);
		//gpEngine->pfnWriteByte();
		gpEngine->pfnMessageEnd();
	};
	*/

	//ClientObituary(self, apAttacker);
	
	SetDamageable(DAMAGE_NO);
	//SetTouchCallback(CBaseEntity::SUB_Null);
	//SetEffects(0);

/*SERVER
	monster_death_use();
*/
	//self->th_die();
	
	//self = oself;
};

/*
================
FireBullets

Used by shotgun, super shotgun, and enemy soldier firing
Go to the trouble of combining multiple pellets into a single damage call.
================
*/
void CBaseEntity::FireBullets(float shotcount, const idVec3 &avDir, const idVec3 &avSpread, CBaseEntity *apAttacker)
{
	if(apAttacker == nullptr)
		apAttacker = this;
	
	ClearMultiDamage();
	
	gpEngine->pfnMakeVectors(self->v_angle);

	idVec3 src{GetOrigin() + gpGlobals->v_forward * 10};
	src.z = GetAbsMin().z + self->size[2] * 0.7;

	TraceResult hit{};
	mpWorld->TraceLine(src, src + avDir * 2048, false, this, &hit);
	
	puff_org = hit.vecEndPos - avDir * 4;

	idVec3 direction;
	
	while(shotcount > 0)
	{
		direction = avDir + crandom() * avSpread.x * gpGlobals->v_right + crandom() * avSpread.y * gpGlobals->v_up;
		
		TraceResult hit{};
		mpWorld->TraceLine(src, src + direction * 2048, false, this, &hit);
		
		if(hit.flFraction != 1.0)
			TraceAttack(apAttacker, 4, direction, hit);

		--shotcount;
	};
	
	ApplyMultiDamage(self, apAttacker);
};
	
int CBaseEntity::GetIndex() const
{
	if constexpr(TargetAPI == TargetEngineAPI::Next)
		return 0; // TODO
	else
		return gpEngine->pfnIndexOfEdict(ToEdict());
};

void CBaseEntity::SetModel(const std::string &asName)
{
	if constexpr(TargetAPI == TargetEngineAPI::Next)
		; // TODO
	else
		gpEngine->pfnSetModel(ToEdict(), asName.c_str());
};

void CBaseEntity::SetOrigin(const idVec3 &avOrigin)
{
	mvOrigin = avOrigin;
	
	if constexpr(TargetAPI == TargetEngineAPI::Next)
		; // TODO
	else
		gpEngine->pfnSetOrigin(ToEdict(), avOrigin);
};

void CBaseEntity::SetSize(const idVec3 &avMins, const idVec3 &avMaxs)
{
	mSize.mins = avMins;
	mSize.maxs = avMaxs;
	
	if constexpr(TargetAPI == TargetEngineAPI::Next)
		; // TODO
	else
		gpEngine->pfnSetSize(ToEdict(), avMins, avMaxs);
};

void CBaseEntity::EmitSound(int anChannel, const std::string &asSample, float afVolume, float afAttenuation, int anFlags, int anPitch)
{
	if constexpr(TargetAPI == TargetEngineAPI::Next)
		; // TODO
	else
		gpEngine->pfnEmitSound(ToEdict(), anChannel, asSample.c_str(), afVolume, afAttenuation, anFlags, anPitch);
};

void CBaseEntity::MakeStatic()
{
	if constexpr(TargetAPI == TargetEngineAPI::Next)
		; // TODO
	else
		gpEngine->pfnMakeStatic(ToEdict());
};

const std::string &CBaseEntity::GetNoise() const
{
	if constexpr(TargetAPI == TargetEngineAPI::Next)
		return "TODO"; // TODO
	else
		return gpEngine->pfnSzFromIndex(self->noise);
};

void CBaseEntity::SetTarget(const char *asTarget)
{
	if constexpr(TargetAPI == TargetEngineAPI::Next)
		; // TODO
	else
		self->target = gpEngine->pfnMakeString(asTarget);
};

void CBaseEntity::SUB_Remove()
{
	if(GetHealth() > 0)
	{
		SetHealth(0);
	};
	
	gpEngine->pfnRemoveEntity(ToEdict()); // TODO: mpWorld->RemoveEntity(this);
};