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

#include "BaseAmmo.hpp"

class CAmmoMP5Grenade : public CBaseAmmo // TODO: CBaseWeaponAmmo?
{
public:
	void Spawn() override;
	
	bool GiveTo(CBaseEntity *apOther) override;
};

LINK_ENTITY_TO_CLASS(ammo_mp5grenades, CAmmoMP5Grenade);
LINK_ENTITY_TO_CLASS(ammo_ARgrenades, CAmmoMP5Grenade);

void CAmmoMP5Grenade::Spawn()
{
	// Precache()
	gpEngine->pfnPrecacheModel("models/w_ARgrenade.mdl");
	gpEngine->pfnPrecacheSound("items/9mmclip1.wav");
	//
	
	SetModel("models/w_ARgrenade.mdl");
	
	CBaseAmmo::Spawn();
};

bool CAmmoMP5Grenade::GiveTo(CBaseEntity *apOther)
{
	bool bResult{apOther->GiveAmmo(AMMO_M203BOX_GIVE, "ARgrenades", M203_GRENADE_MAX_CARRY) != -1};
	
	if(bResult)
		EmitSound(CHAN_ITEM, "items/9mmclip1.wav", 1, ATTN_NORM);
	
	return bResult;
};