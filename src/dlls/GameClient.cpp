/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2022-2023 BlackPhrase
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

#include "GameClient.hpp"

// TODO: maybe some of these methods should actually be part of the CBasePlayer class...

void CGameClient::SendCmd(const char *asCmd, ...)
{
	gpEngine->pfnClientCommand(ToEdict(this), asCmd); // TODO
};

void CGameClient::Printf(PRINT_TYPE aeType, const char *asMsg)
{
	gpEngine->pfnClientPrintf(ToEdict(this), aeType, asMsg);
};

int CGameClient::GetUserID() const
{
	return gpEngine->pfnGetPlayerUserId(ToEdict(this));
};

uint CGameClient::GetWONID() const
{
	return gpEngine->pfnGetPlayerWONId(ToEdict(this));
};

const char *CGameClient::GetAuthID() const
{
	return gpEngine->pfnGetPlayerAuthId(ToEdict(this));
};