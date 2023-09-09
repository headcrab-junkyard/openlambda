/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2019, 2021, 2023 BlackPhrase
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
/// @brief button and multiple button

#include "BaseToggle.hpp"

//=============================================================================

class CBaseButton : public CBaseToggle
{
public:
	void Spawn() override;
	
	void Use(CBaseEntity *apActivator, CBaseEntity *apCaller, UseType aeUseType, float afValue) override;
	void Touch(CBaseEntity *apOther) override; // TODO: ButtonTouch?
	void Blocked(CBaseEntity *apOther) override;
	
	bool HandleKeyValue(ogs::tStringView asKey, ogs::tStringView asValue) override;
	
	void Wait();
	void Done();
	void Return();
	
	void Fire();
	
	//void Killed(CBaseEntity *apAttacker);
private:
	enum class State : int
	{
		Top,
		GoingUp,
		GoingDown,
		Bottom
	};
	
	void SetState(State aeState){meState = aeState;}
	State GetState() const {return meState;}
private:
	CEntityHandle mpActivator{nullptr};
	
	State meState{};
	
	int state{0};
	int mnSounds{0};
};