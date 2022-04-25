/*
 * This file is part of OpenLambda Project
 *
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

#pragma once

#include <string>

#include "eiface.h"
#include "edict.h"
#include "mathlib/vec3.h"
#include "mathlib/bounds.h"
#include "progdefs.h"

//using edict_t = struct edict_s;

extern globalvars_t *gpGlobals;

class Bounds;
class CBaseGame;
//class IGameWorld;

class CBaseEntity
{
public:
/*
	enum class SpawnFlags : int
	{
		NoMessage = 1,
		NoTouch = 1,
		DroidOnly = 4,
		UseOnly = 1
	};
*/
	
	enum class UseType : int
	{
		Off = 0,
		On,
		Set,
		Toggle
	};
	
	using pfnThinkCallback = void (CBaseEntity::*)();
	using pfnTouchCallback = void (CBaseEntity::*)(CBaseEntity *apOther);
	using pfnUseCallback = void (CBaseEntity::*)(CBaseEntity *apActivator, CBaseEntity *apCaller, UseType aeUseType, float afValue);
	using pfnBlockedCallback = void (CBaseEntity::*)(CBaseEntity *apOther);
public:
	//CBaseEntity(entvars_t *apData);
	virtual ~CBaseEntity() = default;
	
	edict_t *ToEdict() const; // TODO: GetDict()?
	
	virtual void Think()
	{
		if(mfnThinkCallback)
			(this->*mfnThinkCallback)();
	};
	virtual void Touch(CBaseEntity *other)
	{
		if(mfnTouchCallback)
			(this->*mfnTouchCallback)(other);
	};
	virtual void Use(CBaseEntity *apActivator, CBaseEntity *apCaller, UseType aeUseType, float afValue)
	{
		if(mfnUseCallback)
			(this->*mfnUseCallback)(apActivator, apCaller, aeUseType, afValue);
	};
	virtual void Blocked(CBaseEntity *other)
	{
		if(mfnBlockedCallback)
			(this->*mfnBlockedCallback)(other);
	};
	
	/// @return false if the entity shouldn't be spawned at all (instead of deleting itself on spawn)
	virtual bool PreSpawn() const {return true;} // TODO: should this be const?
	
	virtual void Spawn(){}
	
	virtual void TraceAttack(CBaseEntity *apAttacker, float afDamage, const idVec3 &dir, TraceResult &aTraceResult, int anDmgBitSum);
	
	virtual int TakeDamage(CBaseEntity *apInflictor, CBaseEntity *apAttacker, float afDamage, int anDmgBitSum); // TODO: was T_Damage
	
	virtual float TakeHealth(float afValue, float afIgnore);
	
	virtual void Killed(CBaseEntity *apAttacker, CBaseEntity *apLastInflictor, int anGibType);
	
	void FireBullets(float shotcount, const idVec3 &dir, const idVec3 &spread);
	
	void SetName(const std::string &asName);
	const std::string &GetName() const;
	
	const std::string &GetClassName() const;
	
	template<typename T>
	inline void SetThinkCallback(T aTCallback){SetThinkCallback(static_cast<pfnThinkCallback>(aTCallback));}
	
	template<typename T>
	inline void SetTouchCallback(T aTCallback){SetTouchCallback(static_cast<pfnTouchCallback>(aTCallback));}
	
	template<typename T>
	inline void SetUseCallback(T aTCallback){SetUseCallback(static_cast<pfnUseCallback>(aTCallback));}
	
	template<typename T>
	inline void SetBlockedCallback(T aTCallback){SetBlockedCallback(static_cast<pfnBlockedCallback>(aTCallback));}
	
	void SetThinkCallback(pfnThinkCallback afnCallback); // TODO: IEntityThinkCallback?
	void SetTouchCallback(pfnTouchCallback afnCallback); // TODO: IEntityTouchCallback?
	void SetUseCallback(pfnUseCallback afnCallback); // TODO: IEntityUseCallback?
	void SetBlockedCallback(pfnBlockedCallback afnCallback); // TODO: IEntityBlockedCallback?
	
	void SetNextThink(float afTime){self->nextthink = afTime;}
	float GetNextThink() const {return self->nextthink;}
	
	//void SetKeyValue(const std::string &asKey, const std::string &asValue);
	//const std::string &GetKeyValue(const std::string &asKey) const;
	virtual bool HandleKeyValue(const std::string &asKey, const std::string &asValue){return false;}
	
	bool IsValid() const {return (!ToEdict() || ToEdict()->free || MarkedForDeletion()) ? false : true;}
	
	int GetIndex() const;
	
	void SetHealth(float afHealth){self->health = afHealth;}
	void AddHealth(float afHealth){self->health += afHealth;}
	float GetHealth() const {return self->health;}
	
	// TODO: should these be here?
	void SetMaxHealth(float afValue){self->max_health = afValue;}
	float GetMaxHealth() const {return self->max_health;}
	
	void SetArmorType(int anType){self->armortype = anType;}
	int GetArmorType() const {return self->armortype;}
	
	void SetArmorValue(int anValue){self->armorvalue = anValue;}
	int GetArmorValue() const {return self->armorvalue;}
	
	void SetVelocity(const idVec3 &avVelocity)
	{
		self->velocity[0] = avVelocity.x;
		self->velocity[1] = avVelocity.y;
		self->velocity[2] = avVelocity.z;
	};
	
	const idVec3 &GetVelocity() /*const*/
	{
		mvVelocity = self->velocity;
		return mvVelocity;
	};
	
	void SetAngularVelocity(const idVec3 &avVelocity)
	{
		mvAngularVelocity = avVelocity;
		
		self->avelocity[0] = avVelocity.x;
		self->avelocity[1] = avVelocity.y;
		self->avelocity[2] = avVelocity.z;
	};
	
	const idVec3 &GetAngularVelocity() /*const*/
	{
		mvAngularVelocity = self->avelocity;
		return mvAngularVelocity;
	};
	
	void SetAngles(const idVec3 &avAngles)
	{
		mvAngles = avAngles;
		
		self->angles[0] = avAngles.x;
		self->angles[1] = avAngles.y;
		self->angles[2] = avAngles.z;
	};
	
	const idVec3 &GetAngles() /*const*/
	{
		mvAngles = self->angles;
		return mvAngles;
	};
	
	void SetGravity(float afY){
		self->gravity = afY;
		//mvGravity.y = afY;
	};
	const float GetGravity() const {return self->gravity;}
	//const idVec3 &GetGravity() const {return idVec3(0.0f, self->gravity, 0.0f);}
	
	void SetModel(const std::string &asName);
	const std::string &GetModel() const;
	
	void SetModelInde(int anIndex){self->modelindex = anIndex;}
	int GetModelIndex() const {return self->modelindex;}
	
	void SetOrigin(const idVec3 &avOrigin);
	
	const idVec3 &GetOrigin() /*const*/
	{
		mvOrigin = self->origin;
		return mvOrigin;
	};
	
	void SetSize(const idVec3 &avMins, const idVec3 &avMaxs);
	
	void SetSize(const Bounds &aSize)
	{
		SetSize(aSize.mins, aSize.maxs);
	};
	
	const Bounds &GetSize() const {return mSize;}
	
	void SetMoveType(int anType){self->movetype = anType;}
	int GetMoveType() const {return self->movetype;}
	
	void SetSolidity(int anSolidity);
	int GetSolidity() const;
	
	void SetFlags(int anFlags){self->flags = anFlags;}
	int GetFlags() const {return self->flags;}
	
	bool HasFlags(int anFlags) const {return self->flags & anFlags;}
	
	void AddFlags(int anFlags){self->flags |= anFlags;}
	void RemoveFlags(int anFlags){self->flags &= ~anFlags;}
	
	void SetEffects(int anEffects){self->effects = anEffects;}
	//void AddEffects(int anEffects){self->effects |= anEffects;}
	//void RemoveEffects(int anEffects){self->effects &= ~anEffects;}
	int GetEffects() const {return self->effects;}
	
	void SetSkin(int anSkin){self->skin = anSkin;}
	int GetSkin() const {return self->skin;}
	
	void EmitSound(int anChannel, const std::string &asSample, float afVolume, float afAttenuation, int anFlags = 0, int anPitch = PITCH_NORM);
	
	void MarkForDeletion()
	{
		self->flags |= FL_KILLME;
	};
	
	bool MarkedForDeletion() const {return self->flags & FL_KILLME;}

	void MakeStatic();

	int GetWaterType() const {return self->watertype;}
	int GetWaterLevel() const {return self->waterlevel;}
	
	void SetOwner(CBaseEntity *apOwner){mpOwner = apOwner;}
	CBaseEntity *GetOwner() const {return mpOwner;}
	
	void SetEnemy(CBaseEntity *apEnemy){mpEnemy = apEnemy;}
	CBaseEntity *GetEnemy() const {return mpEnemy;}
	
	void SetGoal(CBaseEntity *apGoal){mpGoal = apGoal;}
	CBaseEntity *GetGoal() const {return mpGoal;} // TODO: GetGoalEnt(ity)?
	
	void SetFriction(float afFriction);
	
	void SetSpeed(float afSpeed){self->speed = afSpeed;}
	float GetSpeed() const {return self->speed;}
	
	void SetMoveDir(const idVec3 &avMoveDir)
	{
		mvMoveDir = avMoveDir;
		
		self->movedir[0] = mvMoveDir.x;
		self->movedir[1] = mvMoveDir.y;
		self->movedir[2] = mvMoveDir.z;
	};
	
	const idVec3 &GetMoveDir() /*const*/
	{
		mvMoveDir = self->movedir;
		return mvMoveDir;
	};
	
	void SetDamageable(int anDamageable){self->takedamage = anDamageable;}
	int GetDamageable() const {return self->takedamage;}
	
	// TODO: ShouldTakeDamage?
	bool IsDamageable() const {return GetDamageable() > DAMAGE_NO;}
	
	const std::string &GetNoise() const;
	
	void SetTarget(const char *asTarget);
	const char *GetTarget() const {return reinterpret_cast<const char *>(self->target);}
	
	void SetIdealYaw(float afValue){self->ideal_yaw = afValue;}
	
	void SUB_Remove();
	void SUB_Null(){}
	
	enum
	{
		FCAP_ACROSS_TRANSITION
	};
	
	virtual int GetObjectCaps() const {return 0;}
	
	enum class BloodType // TODO: BloodColor?
	{
		DontBleed = -1,
	};
	
	virtual BloodType GetBloodType() const {return BloodType::DontBleed;}
	
	const idVec3 &GetAbsMin() /*const*/
	{
		mvAbsMin = self->absmin;
		return mvAbsMin;
	};
	
	const idVec3 &GetAbsMax() /*const*/
	{
		mvAbsMax = self->absmax;
		return mvAbsMax;
	};
protected:
	void SetClassName(const char *asName){/*self->classname = asName;*/} // TODO: gpEngine->pfnMakeString
private:
	Bounds mSize{};
	
	idVec3 mvOrigin{idVec3::Origin};
	idVec3 mvAngles{idVec3::Origin};
	idVec3 mvVelocity{idVec3::Origin};
	idVec3 mvAngularVelocity{idVec3::Origin};
	idVec3 mvMoveDir{idVec3::Origin};
	//idVec3 mvGravity{idVec3::Origin};
	idVec3 mvAbsMin{idVec3::Origin};
	idVec3 mvAbsMax{idVec3::Origin};
public:
	entvars_t *self{nullptr};
	CBaseGame *mpGame{nullptr};
	//IGameWorld *mpWorld{nullptr};
private:	
	pfnThinkCallback mfnThinkCallback{nullptr};
	pfnTouchCallback mfnTouchCallback{nullptr};
	pfnUseCallback mfnUseCallback{nullptr};
	pfnBlockedCallback mfnBlockedCallback{nullptr};
	
	CBaseEntity *mpOwner{nullptr};
	CBaseEntity *mpEnemy{nullptr};
	CBaseEntity *mpGoal{nullptr};
};