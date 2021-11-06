/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2020-2021 BlackPhrase
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

#include "PlayerMove.hpp"

#include <pm_shared/pm_shared.h>

EXPOSE_SINGLE_INTERFACE(CPlayerMove, IPlayerMove, OGS_PLAYERMOVE_SERVER_INTERFACE_VERSION);

void CPlayerMove::Init(struct playermove_s *ppmove)
{
	PM_Init(ppmove);
};

void CPlayerMove::Move(struct playermove_s *ppmove)
{
	PM_Move(ppmove, true);
};

char CPlayerMove::GetTextureType(const char *name) const
{
	return PM_GetTextureType(name);
};