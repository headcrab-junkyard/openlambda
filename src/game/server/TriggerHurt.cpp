/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2021 BlackPhrase
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
/// @brief hurt trigger code

#include "BaseTrigger.hpp"
#include "Util.hpp"

//============================================================================

/*QUAKED trigger_hurt (.5 .5 .5) ?
Any object touching this will be hurt
set dmg to damage amount
defalt dmg = 5
*/
class CTriggerHurt : public CBaseTrigger
{
public:
	void Spawn() override;
	
	void Touch(CBaseEntity *apOther) override;
	
	void HurtOn();
private:
};

LINK_ENTITY_TO_CLASS(trigger_hurt, CTriggerHurt);

void CTriggerHurt::Spawn()
{
	InitTrigger();
	SetTouchCallback(CTriggerHurt::Touch);
	
	if(!self->dmg)
		self->dmg = 5;
};

void CTriggerHurt::HurtOn()
{
	SetSolidity(SOLID_TRIGGER);
	SetNextThink(-1);
};