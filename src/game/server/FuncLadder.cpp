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
/// @brief ladder entity code

#include "BaseTrigger.hpp"
#include "Util.hpp"

//=============================================================================

class CFuncLadder : public CBaseTrigger
{
public:
	void Spawn() override;
	//void Precache();
	
	bool HandleKeyValue(ogs::tStringView asKey, ogs::tStringView asValue) override;
};

LINK_ENTITY_TO_CLASS(func_ladder, CFuncLadder);

void CFuncLadder::Spawn()
{
	SetSolidity(CBaseEntity::Solidity::None);
	SetSkin(CONTENTS_LADDER);
	
	//SetEffects();
	
	SetModel(gpEngine->pfnSzFromIndex(self->model));
	SetMoveType(CBaseEntity::MoveType::Push);
};

bool CFuncLadder::HandleKeyValue(ogs::tStringView asKey, ogs::tStringView asValue)
{
	return CBaseTrigger::HandleKeyValue(asKey, asValue);
};