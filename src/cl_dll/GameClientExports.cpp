/*
 * This file is part of OGS Engine
 * Copyright (C) 2018, 2020, 2023 BlackPhrase
 *
 * OGS Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OGS Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OGS Engine. If not, see <http://www.gnu.org/licenses/>.
 */

/// @file

#include "GameClientExports.hpp"

EXPOSE_SINGLE_INTERFACE(CGameClientExports, IGameClientExports, GAMECLIENTEXPORTS_INTERFACE_VERSION);

const char *CGameClientExports::GetServerHostName()
{
	//if(gpViewPortInterface)
		//return gpViewPortInterface->GetServerName();
	return "";
};

bool CGameClientExports::IsPlayerGameVoiceMuted(int anPlayerID)
{
	if(GetClientVoiceMgr())
		return GetClientVoiceMgr()->IsPlayerBlocked(anPlayerID);
	return false;
};

void CGameClientExports::MutePlayerGameVoice(int anPlayerID)
{
	if(GetClientVoiceMgr())
		GetClientVoiceMgr()->SetPlayerBlockedState(anPlayerID, true);
};

void CGameClientExports::UnmutePlayerGameVoice(int anPlayerID)
{
	if(GetClientVoiceMgr())
		GetClientVoiceMgr()->SetPlayerBlockedState(anPlayerID, false);
};