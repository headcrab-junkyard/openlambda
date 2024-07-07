/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2024 BlackPhrase
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

#include "LegacyEntityComponent.hpp"

// TODO: CLegacyRigidBody?
class CLegacyPhysicsBody : public CLegacyEntityComponent
{
public:
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
	
	using pfnTouchCallback = void (CBaseEntity::*)(CBaseEntity *apOther);
	using pfnBlockedCallback = void (CBaseEntity::*)(CBaseEntity *apOther);
public:
	CLegacyPhysicsBody(entvars_t *apVars) : CLegacyEntityComponent(apVars){}
public: // Callback methods
	template<typename T>
	inline void SetTouchCallback(T aTCallback){SetTouchCallback(static_cast<pfnTouchCallback>(aTCallback));}
	
	template<typename T>
	inline void SetBlockedCallback(T aTCallback){SetBlockedCallback(static_cast<pfnBlockedCallback>(aTCallback));}
	
	void SetTouchCallback(pfnTouchCallback afnCallback){mfnTouchCallback = afnCallback;} // TODO: IEntityTouchCallback?
	
	void SetBlockedCallback(pfnBlockedCallback afnCallback){mfnBlockedCallback = afnCallback;} // TODO: IEntityBlockedCallback?
public:
	virtual void Touch(CBaseEntity *pOther)
	{
		if(mfnTouchCallback)
			(this->*mfnTouchCallback)(pOther);
	};
	
	virtual void Blocked(CBaseEntity *pOther)
	{
		if(mfnBlockedCallback)
			(this->*mfnBlockedCallback)(pOther);
	};
	
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
	
	void SetGravity(float afY)
	{
		self->gravity = afY;
		//mvGravity.y = afY;
	};
	
	const float GetGravity() const {return self->gravity;}
	//const idVec3 &GetGravity() const {return idVec3(0.0f, self->gravity, 0.0f);}
	
	void SetMoveType(MoveType aeType){self->movetype = static_cast<int>(aeType);}
	MoveType GetMoveType() const {return static_cast<MoveType>(self->movetype);}
	
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
private:
	idVec3 mvVelocity{idVec3::Origin};
	idVec3 mvAngularVelocity{idVec3::Origin};
	idVec3 mvMoveDir{idVec3::Origin};
	//idVec3 mvGravity{idVec3::Origin};
private:
	pfnTouchCallback mfnTouchCallback{nullptr};
	pfnBlockedCallback mfnBlockedCallback{nullptr};
};