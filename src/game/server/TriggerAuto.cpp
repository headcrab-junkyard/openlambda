/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2021-2023 BlackPhrase
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
#include "Util.hpp"

constexpr auto SF_AUTO_FIREONCE{0x0001};

class CTriggerAuto : public CBaseDelay
{
public:
	void Spawn() override;
	//void Precache();
	
	
	bool HandleKeyValue(ogs::tStringView asKey, ogs::tStringView asValue) override;
	
	void Think() override;
	
	int GetObjectCaps() const {return CBaseDelay::GetObjectCaps() & ~FCAP_ACROSS_TRANSITION;}
private:
	UseType meTriggerType{};
	
	string_t mnGlobalState{0};
};

LINK_ENTITY_TO_CLASS(trigger_auto, CTriggerAuto);

void CTriggerAuto::Spawn()
{
	//
	//Precache();
	//
	SetNextThink(gpGlobals->time + 0.1);
	//
};

bool CTriggerAuto::HandleKeyValue(ogs::tStringView asKey, ogs::tStringView asValue)
{
	if(asKey == "globalstate")
	{
		mnGlobalState = gpEngine->pfnAllocString(asValue.c_str());
		return true;
	}
	else if(asKey == "triggerstate")
	{
		int nType{std::stoi(asValue.data())};
		switch(nType)
		{
		case 0:
			meTriggerType = UseType::Off;
			break;
		case 2:
			meTriggerType = UseType::Toggle;
			break;
		default:
			meTriggerType = UseType::On;
			break;
		};
		return true;
	};
	
	return CBaseDelay::HandleKeyValue(asKey, asValue);
};

void CTriggerAuto::Think()
{
	if(!mnGlobalState || gGlobalState.EntityGetState(mnGlobalState) == GLOBAL_ON)
	{
		SUB_UseTargets(this, meTriggerType, 0);
		if(GetSpawnFlags() & SF_AUTO_FIREONCE)
			mpWorld->RemoveEntity(this);
	};
};