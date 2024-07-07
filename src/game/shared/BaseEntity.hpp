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
#include <vector>
#include <memory>

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

class CBaseEntityComponent;
//using tBaseEntityComponentVec = std::vector<CBaseEntityComponent*>;
using tBaseEntityComponentVec = std::vector<std::unique_ptr<CBaseEntityComponent>>;

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
public: // BP: Some entity components experiments
	template<typename T, typename... Args>
	T *AddComponent(Args... aArgs);
	
	//template<typename T>
	//void RemoveComponent(T *apComponent);
	
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
	
	using pfnThinkCallback = void (CBaseEntity::*)();
	
	using pfnUseCallback = void (CBaseEntity::*)(CBaseEntity *apActivator, CBaseEntity *apCaller, UseType aeUseType, float afValue);
public:
	// Save & Load methods
	
	virtual int Save(CGameSave &aGameSave); // TODO: CSave &aSave in original
	virtual int Restore(const CGameSave &aGameSave); // TODO: CRestore &aRestore in original
	
	// Callback methods
	
	template<typename T>
	inline void SetThinkCallback(T aTCallback){SetThinkCallback(static_cast<pfnThinkCallback>(aTCallback));}
	
	template<typename T>
	inline void SetUseCallback(T aTCallback){SetUseCallback(static_cast<pfnUseCallback>(aTCallback));}
	
	void SetThinkCallback(pfnThinkCallback afnCallback){mfnThinkCallback = afnCallback;} // TODO: IEntityThinkCallback?
	
	void SetUseCallback(pfnUseCallback afnCallback){mfnUseCallback = afnCallback;} // TODO: IEntityUseCallback?
	
	virtual void Think()
	{
		if(mfnThinkCallback)
			(this->*mfnThinkCallback)();
	};
	
	virtual void Use(CBaseEntity *apActivator, CBaseEntity *apCaller, UseType aeUseType, float afValue)
	{
		if(mfnUseCallback)
			(this->*mfnUseCallback)(apActivator, apCaller, aeUseType, afValue);
	};
	
	//
	
	virtual void TraceAttack(CBaseEntity *apAttacker, float afDamage, const idVec3 &dir, TraceResult &aTraceResult, int anDmgBitSum);
	
	///
	// TODO: TraceBleed?
	virtual void TraceBlood(float afDamage, const idVec3 &avDir, TraceResult *apTraceResult, int anDmgBitSum);
	
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
	
	void SetArmorType(int anType){self->armortype = anType;}
	int GetArmorType() const {return self->armortype;}
	
	void SetArmorValue(int anValue){self->armorvalue = anValue;}
	int GetArmorValue() const {return self->armorvalue;}
	
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
	
	void SetModel(const std::string &asName);
	const std::string &GetModel() const;
	
	void SetModelIndex(int anIndex){self->modelindex = anIndex;}
	int GetModelIndex() const {return self->modelindex;}
	
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
	pfnUseCallback mfnUseCallback{nullptr};
	
	CBaseEntity *mpOwner{nullptr};
	CBaseEntity *mpEnemy{nullptr};
	CBaseEntity *mpGoal{nullptr}; ///< A movetarget or an enemy
public:
	entvars_t *self{nullptr};
public:
	//float	ammo_shells{0};
	//float	ammo_nails{0};
	//float	ammo_rockets{0};
	//float	ammo_cells{0};
};