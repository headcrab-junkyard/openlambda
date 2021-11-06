/*
	buttons.qc

	button and multiple button

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

#include "BaseToggle.hpp"

class CBaseButton : public CBaseToggle
{
public:
	void Spawn() override;
	
	void Use(CBaseEntity *apActivator, CBaseEntity *apCaller, UseType aeUseType, float afValue) override;
	void Touch(CBaseEntity *apOther) override; // TODO: ButtonTouch?
	void Blocked(CBaseEntity *apOther) override;
	
	bool HandleKeyValue(const std::string &asKey, const std::string &asValue) override;
	
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