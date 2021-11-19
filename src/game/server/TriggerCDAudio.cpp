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

class CTriggerCDAudio : public CBaseTrigger
{
public:
	void Spawn() override;
	
	void Touch(CBaseEntity *apOther) override;
	
	/*virtual*/ void Use(CBaseEntity *apActivator, CBaseEntity *apCaller, UseType aeUseType, float afValue) override;
private: // TODO: public?
	void PlayTrack();
};

LINK_ENTITY_TO_CLASS(trigger_cdaudio, CTriggerCDAudio);

void CTriggerCDAudio::Spawn()
{
	InitTrigger();
};

void CTriggerCDAudio::Touch(CBaseEntity *apOther)
{
	if(!apOther->IsPlayer())
		return;
	
	PlayTrack();
};

void CTriggerCDAudio::Use(CBaseEntity *apActivator, CBaseEntity *apCaller, UseType aeUseType, float afValue)
{
	PlayTrack();
};

void CTriggerCDAudio::PlayTrack()
{
	PlayCDTrack(self->health);
	
	SetTouchCallback(nullptr);
	mpWorld->RemoveEntity(this);
};