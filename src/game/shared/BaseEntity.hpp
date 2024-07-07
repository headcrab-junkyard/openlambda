/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018-2024 BlackPhrase
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
/// @brief base entity class, all other entities derive from this one

#pragma once

#include <string>

#include <engine/eiface.h>
#include <engine/edict.h>
#include <engine/progdefs.h>

#include <mathlib/vec3.h>
#include <mathlib/bounds.h>

#include <next/CommonTypes.hpp>

//using edict_t = struct edict_s;

extern globalvars_t *gpGlobals;

class Bounds;
class CBaseGame;
//class IGameWorld;

class CGameSave;

/*
class CEntityComponent
{
public:
	CEntityComponent(entvars_t *apVars = nullptr){self = apVars;}
protected:
	entvars_t *self{nullptr};
};

class CDamageable : public CEntityComponent
{
public:
	CDamageable(entvars_t *apVars) : CEntityComponent(apVars){}
	
	void SetHealth(float afValue)
	{
		if(self)
			self->health = afValue;
	};
	
	float GetHealth() const
	{
		if(self)
			mfHealth = self->health;
		
		return mfHealth;
	};
	
	void TakeDamage();
	
	void TakeHealth();
private:
	float mfHealth{0.0f};
};
*/

class CBaseEntityComponent;
using tBaseEntityComponentVec = std::vector<CBaseEntityComponent*>;

class CBaseEntity
{
public:
	//CBaseEntity(entvars_t *apData);
	virtual ~CBaseEntity() = default;
	
	// Init methods
	
	/// @return false if the entity shouldn't be spawned at all (instead of deleting itself on spawn)
	virtual bool PreSpawn() const {return true;} // TODO: should this be const?
	
	virtual void Spawn(){}
	
	// TODO: virtual void Precache(){}?
	
	// Activation methods
	
	// TODO
	//virtual void Activate(){}
	void SetActive(bool abActive){mbActive = abActive;}
	bool IsActive() const {return mbActive;}
	
	//CBaseGame *GetGame() const {return mpGame;}
	IGameWorld *GetWorld() const {return mpWorld;}
	
	//SEntityTransform &GetTransform() const {return mTransform;} // TODO
public: // BP: Some entity components experiments
	template<typename T, typename... Args>
	T *AddComponent(Args... aArgs);
	
	template<typename T>
	void RemoveComponent(T *apComponent);
	
	template<typename T>
	T *GetComponent() const;
	
	template<typename T>
	T *GetComponentAt(int anIndex) const; // TODO: GetComponent?
	
	template<typename T>
	bool TryGetComponent(T *apComponent) const;
	
	tBaseEntityComponentVec &GetComponents() const;
	
	int GetComponentCount() const;
	
	template<typename T>
	bool HasComponent() const;
private:
	tBaseEntityComponentVec mvComponents;
protected:
	void SetClassName(const char *asName){/*self->classname = asName;*/} // TODO: gpEngine->pfnMakeString // TODO
private: // TODO: public?
	/// This updates global tables that need to know about entities being removed
	void UpdateOnRemove();
private:
	static TYPEDESCRIPTION mSaveData[]; // TODO: public?
	
	Bounds mSize{};
	
	idVec3 mvOrigin{idVec3::Origin};
	idVec3 mvAngles{idVec3::Origin};
	
	idVec3 mvAbsMin{idVec3::Origin};
	idVec3 mvAbsMax{idVec3::Origin};
public:
	CBaseGame *mpGame{nullptr};
private:
	//IGameWorld *mpWorld{nullptr}; // TODO
	
	bool mbActive{false};
};

class CFatAzzEntity : public CBaseEntity
{
public:
	// Enums & public defs
	
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
	
	//enum Capabilities : int // TODO: Caps?
	enum
	{
		FCAP_ACROSS_TRANSITION
	};
	
	enum class BloodType // TODO: BloodColor?
	{
		DontBleed = -1,
	};
	
	enum class MoveType : int
	{
		None = MOVETYPE_NONE,
		
		Walk = MOVETYPE_WALK,
		Step,
		Fly,
		Toss,
		Push,
		NoClip,
		FlyMissile,
		Bounce,
		BounceMissile,
		Follow,
		PushStep
	};
	
	enum class Solidity : int
	{
		None = SOLID_NOT,
		Trigger,
		BoundingBox,
		SlideBox,
		BSP
	};
	
	enum class Damageable : int
	{
		No = DAMAGE_NO,
		Yes,
		Aim
	};
	
	enum class DmgType : int
	{
		Generic = 0,
	};
	
	enum class GibType : int
	{
		Normal = 0, ///< Gib if entity was overkilled
		Never, ///< Never gib, no matter how much dmg is done (used for freezing and such)
		Always ///< Always gib (Houndeye Shock, Barnacle Bite and such)
	};
	
	using pfnThinkCallback = void (CBaseEntity::*)();
	using pfnTouchCallback = void (CBaseEntity::*)(CBaseEntity *apOther);
	using pfnUseCallback = void (CBaseEntity::*)(CBaseEntity *apActivator, CBaseEntity *apCaller, UseType aeUseType, float afValue);
	using pfnBlockedCallback = void (CBaseEntity::*)(CBaseEntity *apOther);
public:
	// Save & Load methods
	
	virtual int Save(CGameSave &aGameSave); // TODO: CSave &aSave in original
	virtual int Restore(const CGameSave &aGameSave); // TODO: CRestore &aRestore in original
	
	// Callback methods
	
	template<typename T>
	inline void SetThinkCallback(T aTCallback){SetThinkCallback(static_cast<pfnThinkCallback>(aTCallback));}
	
	template<typename T>
	inline void SetTouchCallback(T aTCallback){SetTouchCallback(static_cast<pfnTouchCallback>(aTCallback));}
	
	template<typename T>
	inline void SetUseCallback(T aTCallback){SetUseCallback(static_cast<pfnUseCallback>(aTCallback));}
	
	template<typename T>
	inline void SetBlockedCallback(T aTCallback){SetBlockedCallback(static_cast<pfnBlockedCallback>(aTCallback));}
	
	void SetThinkCallback(pfnThinkCallback afnCallback){mfnThinkCallback = afnCallback;} // TODO: IEntityThinkCallback?
	void SetTouchCallback(pfnTouchCallback afnCallback){mfnTouchCallback = afnCallback;} // TODO: IEntityTouchCallback?
	void SetUseCallback(pfnUseCallback afnCallback){mfnUseCallback = afnCallback;} // TODO: IEntityUseCallback?
	void SetBlockedCallback(pfnBlockedCallback afnCallback){mfnBlockedCallback = afnCallback;} // TODO: IEntityBlockedCallback?
	
	virtual void Think()
	{
		if(mfnThinkCallback)
			(this->*mfnThinkCallback)();
	};
	
	virtual void Touch(CBaseEntity *pOther)
	{
		if(mfnTouchCallback)
			(this->*mfnTouchCallback)(pOther);
	};
	
	virtual void Use(CBaseEntity *apActivator, CBaseEntity *apCaller, UseType aeUseType, float afValue)
	{
		if(mfnUseCallback)
			(this->*mfnUseCallback)(apActivator, apCaller, aeUseType, afValue);
	};
	
	virtual void Blocked(CBaseEntity *pOther)
	{
		if(mfnBlockedCallback)
			(this->*mfnBlockedCallback)(pOther);
	};
	
	//
	
	virtual void TraceAttack(CBaseEntity *apAttacker, float afDamage, const idVec3 &dir, TraceResult &aTraceResult, int anDmgBitSum);
	
	///
	// TODO: TraceBleed?
	virtual void TraceBlood(float afDamage, const idVec3 &avDir, TraceResult *apTraceResult, int anDmgBitSum);
	
	/// Called when the entity receives damage
	virtual int TakeDamage(CBaseEntity *apInflictor, CBaseEntity *apAttacker, float afDamage, int anDmgBitSum); // TODO: was T_Damage
	
	/// Called when the entity receives health
	virtual float TakeHealth(float afValue, float afIgnore, int anDmgBitSum); // TODO: wtf is afIgnore and where did it come from?
	
	/// Called when the entity gets killed/destroyed
	virtual void Killed(CBaseEntity *apAttacker, CBaseEntity *apLastInflictor, GibType aeGibType = GibType::Normal); // TODO: wtf if apLastInflictor?
	
	/// Used to make entity shoot in specified direction (like a turret)
	void FireBullets(float afShotCount, const idVec3 &avDir, const idVec3 &avSpread, CBaseEntity *apAttacker = nullptr);
	
	/// Used to make entity emit various sounds with specified properties
	void EmitSound(int anChannel, const std::string &asSample, float afVolume, float afAttenuation, int anFlags = 0, int anPitch = PITCH_NORM);
	
	/// Used to mark the entity for deletion (use this instead of deleting the entity)
	void MarkForDeletion(){AddFlags(FL_KILLME);}
	
	/// Make the entity static
	void MakeStatic();
	
	/// Make the entity dormant (inactive)
	void MakeDormant();
	
	/// Remove this entity (convenient way to delay removing oneself)
	void SUB_Remove();
	
	/// Does nothing
	void SUB_Null(){} // aka SUB_DoNothing
	
	/// Lets the entity handle its settings from the map file
	virtual bool HandleKeyValue(ogs::tStringView asKey, ogs::tStringView asValue){return false;}
	
	// Setter & getter (+ checks & conversion) methods
	
	edict_t *ToEdict() const; // TODO: GetDict()?
	
	void SetName(const std::string &asName); // TODO
	const std::string &GetName() const; // TODO
	
	const std::string &GetClassName() const; // TODO
	
	void SetNextThink(float afTime){self->nextthink = afTime;}
	float GetNextThink() const {return self->nextthink;}
	
	//void SetKeyValue(const std::string &asKey, const std::string &asValue);
	//const std::string &GetKeyValue(const std::string &asKey) const;
	
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
	
	void SetGravity(float afY)
	{
		self->gravity = afY;
		//mvGravity.y = afY;
	};
	
	const float GetGravity() const {return self->gravity;}
	//const idVec3 &GetGravity() const {return idVec3(0.0f, self->gravity, 0.0f);}
	
	void SetModel(const std::string &asName);
	const std::string &GetModel() const;
	
	void SetModelIndex(int anIndex){self->modelindex = anIndex;}
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
	
	void SetMoveType(MoveType aeType){self->movetype = static_cast<int>(aeType);}
	MoveType GetMoveType() const {return static_cast<MoveType>(self->movetype);}
	
	void SetSolidity(Solidity aeSolidity){self->solid = static_cast<int >(aeSolidity);}
	Solidity GetSolidity() const {return static_cast<Solidity>(self->solid);}
	
	// TODO: Replace with HasSpawnFlags?
	int GetSpawnFlags() const {return self->spawnflags;}
	
	void SetFlags(int anFlags){self->flags = anFlags;}
	int GetFlags() const {return self->flags;}
	
	bool HasFlags(int anFlags) const {return self->flags & anFlags;}
	
	void AddFlags(int anFlags){self->flags |= anFlags;}
	void RemoveFlags(int anFlags){self->flags &= ~anFlags;}
	
	void SetEffects(int anEffects){self->effects = anEffects;}
	void AddEffects(int anEffects){self->effects |= anEffects;}
	void RemoveEffects(int anEffects){self->effects &= ~anEffects;}
	int GetEffects() const {return self->effects;}
	
	void SetSkin(int anSkin){self->skin = anSkin;}
	int GetSkin() const {return self->skin;}
	
	// TODO: IsMarkedForDeletion?
	bool MarkedForDeletion() const {return HasFlags(FL_KILLME);}

	int GetWaterType() const {return self->watertype;}
	int GetWaterLevel() const {return self->waterlevel;}
	
	void SetOwner(CBaseEntity *apOwner){mpOwner = apOwner;} // TODO
	CBaseEntity *GetOwner() const {return mpOwner;} // TODO
	
	void SetEnemy(CBaseEntity *apEnemy){mpEnemy = apEnemy;} // TODO
	CBaseEntity *GetEnemy() const {return mpEnemy;} // TODO
	
	void SetGoal(CBaseEntity *apGoal){mpGoal = apGoal;} // TODO
	CBaseEntity *GetGoal() const {return mpGoal;} // TODO: GetGoalEnt(ity)? // TODO
	
	void SetFriction(float afFriction){self->friction = afFriction;}
	
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
	
	void SetDamageable(Damageable aeDamageable){self->takedamage = static_cast<int>(aeDamageable);}
	Damageable GetDamageable() const {return static_cast<Damageable>(self->takedamage);}
	
	// TODO: ShouldTakeDamage?
	bool IsDamageable() const {return GetDamageable() > Damageable::No;}
	
	/// @return true if the entity is dormant/inactive, false otherwise
	bool IsDormant() const {return HasFlags(FL_DORMANT);}
	
	const std::string &GetNoise() const;
	
	void SetTarget(const char *asTarget);
	const char *GetTarget() const {return reinterpret_cast<const char *>(self->target);} // TODO: mpGame->GetStringPool()->GetByIndex(self->target)
	
	void SetIdealYaw(float afValue){self->ideal_yaw = afValue;}
	
	/// @return entity's capabilities
	virtual int GetObjectCaps() const {return FCAP_ACROSS_TRANSITION;}
	
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
	
	void SetCollisionBox(); // TODO: SetupCollisionBox?
	
	//
	
	// TODO: move some of those over to CBasePlayer/CBaseCharacter?
	
	void SaveSpawnParms();
	void GetSpawnParms() const;
	
	void SetCrosshairAngle(float afPitch, float afYaw);
	
	void MoveTo(const idVec3 &avGoal, float afDist, MoveType aeMoveType);
	void WalkMove(float afYaw, float afDist, int anMode);
	
	void AnimationAutomove(float afTime);
	
	void ChangeYaw();
	void ChangePitch();
	
	int DropToFloor();
	int IsOnFloor() const;
	
	void *GetModelPtr() const;
	
	/*virtual*/ int GetIllum() const; // TODO: GetIllumination?
	
	void GetAttachment(int anAttachment, idVec3 &avOrigin, idVec3 &avAngles) const;
	void GetBonePosition(int anBone, idVec3 &avOrigin, idVec3 &avAngles) const;
	
	void GetAimVector(float afSpeed, idVec3 &avReturn) const;
public: // TODO: protected?
	int PrecacheModel(const char *asName);
	bool PrecacheSound(const char *asName);
private:	
	pfnThinkCallback mfnThinkCallback{nullptr};
	pfnTouchCallback mfnTouchCallback{nullptr};
	pfnUseCallback mfnUseCallback{nullptr};
	pfnBlockedCallback mfnBlockedCallback{nullptr};
	
	CBaseEntity *mpOwner{nullptr};
	CBaseEntity *mpEnemy{nullptr};
	CBaseEntity *mpGoal{nullptr}; ///< A movetarget or an enemy
private:
	idVec3 mvVelocity{idVec3::Origin};
	idVec3 mvAngularVelocity{idVec3::Origin};
	idVec3 mvMoveDir{idVec3::Origin};
	//idVec3 mvGravity{idVec3::Origin};
public:
	entvars_t *self{nullptr};
public:
	//float	ammo_shells{0};
	//float	ammo_nails{0};
	//float	ammo_rockets{0};
	//float	ammo_cells{0};
};