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
#include "BaseGame.hpp"
#include "IStringPool.hpp"
#include "Util.hpp"

//=============================================================================

// TODO: CLadder?
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
	//
	// Precache()
	//
	SetSolidity(CBaseEntity::Solidity::None);
	SetSkin(CONTENTS_LADDER);
	
	if(gpEngine->pfnCVarGetFloat("showtriggers") == 0.0f)
	{
		self->rendermode = kRenderTransTexture;
		self->renderamt = 0;
	};
	
	RemoveEffects(EF_NODRAW); // self->effects &= ~EF_NODRAW
	//
	
	// Set size and link into world
	SetModel(GetModel());
	
	//SetMoveType(CBaseEntity::MoveType::Push); // TODO
};

bool CFuncLadder::HandleKeyValue(ogs::tStringView asKey, ogs::tStringView asValue)
{
	return CBaseTrigger::HandleKeyValue(asKey, asValue);
};