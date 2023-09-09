/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2021, 2023 BlackPhrase
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

#include "BaseTrigger.hpp"
#include "Util.hpp"

//============================================================================

class CTriggerAutoSave : public CBaseTrigger
{
public:
	bool PreSpawn() override;
	void Spawn() override;
	
	void Touch(CBaseEntity *apOther) override;
};

LINK_ENTITY_TO_CLASS(trigger_autosave, CTriggerAutoSave);

bool CTriggerAutoSave::PreSpawn()
{
	if(mpGame->GetRules()->IsDeathmatch())
		return false;
	
	return true;
};

void CTriggerAutoSave::Spawn()
{
	InitTrigger();
	SetTouchCallback(CTriggerAutoSave::Touch);
};

void CTriggerAutoSave::Touch(CBaseEntity *apOther)
{
	if(!UTIL_IsMasterTriggered(msMaster, apOther))
		return;
	
	if(!apOther->IsPlayer())
		return;
	
	SetTouchCallback(nullptr);
	mpWorld->RemoveEntity(this);
	gpEngine->pfnServerCmd("autosave\n");
};