/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2019, 2021 BlackPhrase
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

#include "BaseAmmo.hpp"

class CAmmo357 : public CBaseAmmo
{
public:
	bool GiveTo(CBaseEntity *apOther) override;
private:
	void PostSpawn() override;
};

LINK_ENTITY_TO_CLASS(ammo_357, CAmmo357);

void CAmmo357::PostSpawn()
{
	// Precache()
	gpEngine->pfnPrecacheModel("models/w_357ammobox.mdl");
	gpEngine->pfnPrecacheSound("items/9mmclip1.wav");
	//
	
	SetModel("models/w_357ammobox.mdl");
};

bool CAmmo357::GiveTo(CBaseEntity *apOther)
{
	if(apOther->GiveAmmo(AMMO_357BOX_GIVE, "357", _357_MAX_CARRY) != -1)
	{
		apOther->EmitSound(CHAN_ITEM, "items/9mmclip1.wav", 1, ATTN_NORM);
		return true;
	};
	return false;
};