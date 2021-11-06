/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2019-2021 BlackPhrase
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

#include "BaseAnimating.hpp"

class CBaseToggle : public CBaseAnimating
{
public:
	bool HandleKeyValue(const std::string &asKey, const std::string &asValue) override;
	
	int GetState() const {return mnState;}
	/*virtual*/ float GetDelay() const {return mfWaitTime;} // TODO: GetWaitTime?
	
	using pfnMoveDoneCallback = void (CBaseToggle::*)();
	
	void SetMoveDoneCallback(pfnMoveDoneCallback afnCallback){mfnMoveDoneCallback = afnCallback;}
	
	template<typename T>
	inline void SetMoveDoneCallback(T aTCallback){SetMoveDoneCallback(static_cast<pfnMoveDoneCallback>(aTCallback));}
	
	void LinearMove(const idVec3 &tdest, float tspeed); // TODO: was SUB_CalcMove
	void LinearMoveDone(); // TODO: was SUB_CalcMoveDone
	
	void SUB_CalcAngleMove(const idVec3 &destangle, float tspeed); // TODO: AngularMove
	void SUB_CalcAngleMoveDone(); // TODO: AngularMoveDone
	
	bool IsLockedByMaster() const;
protected:
	void SetDelay(float afTime){mfWaitTime = afTime;}
protected:
	idVec3 mvPos1{idVec3::Origin};
	idVec3 mvPos2{idVec3::Origin};
	
	float mfWaitTime{0.0f};
	float mfLip{0.0f};
	
	int mnCount{0};
private:
	idVec3 mvFinalDest{idVec3::Origin};
	idVec3 mvFinalAngle{idVec3::Origin};
	
	pfnMoveDoneCallback mfnMoveDoneCallback{nullptr};
	
	int mnState{-1};
};