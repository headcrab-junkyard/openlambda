/*
 * This file is part of OpenLambda Project
 *
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

#include "BaseTrigger.hpp"
#include "Util.hpp"

class CTriggerEndSection : public CBaseTrigger
{
public:
	bool PreSpawn() override;
	void Spawn() override;
	
	bool HandleKeyValue(const std::string &asKey, const std::string &asValue) override;
	
	void Touch(CBaseEntity *apOther) override;
	
	void Use(CBaseEntity *apActivator, CBaseEntity *apCaller, UseType aeUseType, float afValue) override;
private:
	void TouchCommon();
};

LINK_ENTITY_TO_CLASS(trigger_endsection, CTriggerEndSection);

bool CTriggerEndSection::PreSpawn()
{
	if(mpGame->GetRules()->IsDeathmatch())
		return false;
	
	return true;
};

void CTriggerEndSection::Spawn()
{
	InitTrigger();
	
	SetUseCallback(CTriggerEndSection::Use);
	
	if()
		SetTouchCallback(CTriggerEndSection::Touch);
};

bool CTriggerEndSection::HandleKeyValue(const std::string &asKey, const std::string &asValue)
{
	if(asKey == "section")
	{
		self->message = gpEngine->pfnMakeString(asValue.c_str());
		return true;
	};
	
	return CBaseTrigger::HandleKeyValue(asKey, asValue);
};

void CTriggerEndSection::Touch(CBaseEntity *apOther)
{
	if(!apOther->IsNetClient())
		return;
	
	TouchCommon();
};

void CTriggerEndSection::Use(CBaseEntity *apActivator, CBaseEntity *apCaller, UseType aeUseType, float afValue)
{
	if(apActivator && !apActivator->IsNetClient())
		return;
	
	TouchCommon();
};

void CTriggerEndSection::TouchCommon()
{
	SetUseCallback(nullptr);
	
	if(self->message)
		gpEngine->pfnEndSection(gpEngine->pfnSzFromIndex(self->message));
	
	mpWorld->RemoveEntity(this);
};