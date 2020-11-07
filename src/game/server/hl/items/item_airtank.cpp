/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2019-2020 BlackPhrase
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

#include "Grenade.hpp"

class CItemAirTank : public CGrenade
{
public:
	void Spawn() override;
	
	void Think() override;
	
	void Touch(CBaseEntity *apOther) override;
};

void CItemAirTank::Spawn()
{
	// TODO
	
	gpEngine->pfnPrecacheModel("models/w_oxygen.mdl");
	
	SetModel("models/w_oxygen.mdl");
	
	SetThinkCallback(CItemAirTank::Think);
	SetTouchCallback(CItemAirTank::Touch);
};

LINK_ENTITY_TO_CLASS(item_airtank, CItemAirTank);