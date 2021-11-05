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

#include "BaseDelay.hpp"

class CBaseToggle : public CBaseDelay
{
public:
	bool HandleKeyValue(KeyValueData *apKVData) override;
	
	int GetState() const {return mnState;}
	float GetDelay() const {return mfDelay;}
	
	void LinearMove(const idVec3 &tdest, float tspeed); // TODO: was SUB_CalcMove
	void LinearMoveDone(); // TODO: was SUB_CalcMoveDone
	
	void SUB_CalcAngleMove(const idVec3 &destangle, float tspeed); // TODO: AngularMove
	void SUB_CalcAngleMoveDone(); // TODO: AngularMoveDone
	
	bool IsLockedByMaster() const;
private:
	idVec3 mvFinalDest{idVec3::Origin};
	idVec3 mvFinalAngle{idVec3::Origin};
	
	void (CBaseToggle::*mpfnMoveDoneCallback)();
	
	float mfDelay{0.0f};
	
	int mnState{-1};
};