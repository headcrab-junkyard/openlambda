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
#include "BaseGame.hpp"
#include "IGameRules.hpp"
#include "IGameWorld.hpp"
#include "IStringPool.hpp"
#include "Util.hpp"

constexpr auto SF_ENDSECTION_USEONLY{0x0001};

//============================================================================

class CTriggerEndSection : public CBaseTrigger
{
public:
	bool PreSpawn() const override;
	void Spawn() override;
	
	bool HandleKeyValue(ogs::tStringView asKey, ogs::tStringView asValue) override;
	
	void Touch(CBaseEntity *apOther) override;
	
	void Use(CBaseEntity *apActivator, CBaseEntity *apCaller, UseType aeUseType, float afValue) override;
private:
	void TouchCommon();
};

LINK_ENTITY_TO_CLASS(trigger_endsection, CTriggerEndSection);

bool CTriggerEndSection::PreSpawn() const
{
	if(mpGame->GetRules()->IsDeathmatch())
		return false;
	
	return true;
};

void CTriggerEndSection::Spawn()
{
	InitTrigger();
	
	SetUseCallback(CTriggerEndSection::Use);
	
	// If it's a "use only" trigger, don't set the touch callback
	if(!(GetSpawnFlags() & SF_ENDSECTION_USEONLY))
		SetTouchCallback(CTriggerEndSection::Touch);
};

bool CTriggerEndSection::HandleKeyValue(ogs::tStringView asKey, ogs::tStringView asValue)
{
	if(asKey == "section")
	{
		// Store this in message so we don't have to write save/restore for this entity
		self->message = gpEngine->pfnMakeString(asValue.c_str());
		return true;
	};
	
	return CBaseTrigger::HandleKeyValue(asKey, asValue);
};

void CTriggerEndSection::Touch(CBaseEntity *apOther)
{
	// Only save on clients
	if(!apOther->IsNetClient()) // TODO
		return;
	
	TouchCommon();
};

void CTriggerEndSection::Use(CBaseEntity *apActivator, CBaseEntity *apCaller, UseType aeUseType, float afValue)
{
	if(apActivator && !apActivator->IsNetClient()) // TODO
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