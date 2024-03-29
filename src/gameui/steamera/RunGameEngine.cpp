/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2018, 2020, 2023 BlackPhrase
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

#include "RunGameEngine.hpp"

#include "igameuifuncs.h"

EXPOSE_SINGLE_INTERFACE(CRunGameEngine, IRunGameEngine, RUNGAMEENGINE_INTERFACE_VERSION);

CRunGameEngine::CRunGameEngine() = default;
CRunGameEngine::~CRunGameEngine() = default;

bool CRunGameEngine::IsRunning()
{
	return true; // TODO: check for gpEngine?
};

bool CRunGameEngine::AddTextCommand(const char *text)
{
	gpEngine->ClientCmd(text); // NOTE: ClientCmd_Unrestricted
	return true;
};

bool CRunGameEngine::RunEngine(const char *gameDir, const char *commandLineParams)
{
	return false;
};

bool CRunGameEngine::IsInGame()
{
	return gpEngine->GetLevelName() && Q_strlen(gpEngine->GetLevelName()) > 0;
};

bool CRunGameEngine::GetGameInfo(char *infoBuffer, int bufferSize)
{
	// TODO: need to implement
	return false;
};

void CRunGameEngine::SetTrackerUserID(int trackerID, const char *trackerName)
{
	gpGameUIFuncs->SetFriendsID(trackerID, trackerName);
	
	// Update the player's name if necessary
	ConVarRef name("name");
	if(name.IsValid() && trackerName && *trackerName && !Q_strcmp(name.GetString(), "unnamed"))
		name.SetValue(trackerName);
};

int CRunGameEngine::GetPlayerCount()
{
	return gpEngine->GetMaxClients();
};

unsigned int CRunGameEngine::GetPlayerFriendsID(int playerIndex)
{
	player_info_t PlayerInfo{};
	
	if(gpEngine->GetPlayerInfo(playerIndex, &PlayerInfo))
		return PlayerInfo.friendsID;
	
	return 0;
};

const char *CRunGameEngine::GetPlayerName(int friendsID)
{
	// Find the player by its tracker id
	player_info_t PlayerInfo{};
	
	for(int i = 0; i < GetPlayerCount(); ++i)
		if(gpEngine->GetPlayerInfo(playerIndex, &PlayerInfo))
			if(PlayerInfo.friendsID == friendsID)
				return PlayerInfo.name;
	
	return nullptr;
};

const char *CRunGameEngine::GetPlayerFriendsName(int friendsID)
{
	// Find the player by its tracker id
	player_info_t PlayerInfo{};
	
	for(int i = 0; i < GetPlayerCount(); ++i)
		if(gpEngine->GetPlayerInfo(playerIndex, &PlayerInfo))
			if(PlayerInfo.friendsID == friendsID)
				return PlayerInfo.friendsName;
	
	return nullptr;
};

unsigned int CRunGameEngine::GetEngineBuildNumber()
{
	return gpEngine->GetEngineBuildNumber();
};

const char *CRunGameEngine::GetProductVersionString()
{
	return gpEngine->GetProductVersionString();
};

unsigned int CRunGameEngine::GetPlayerUserID(int friendsID)
{
	player_info_t PlayerInfo{};
	
	for(int i = 0; i < GetPlayerCount(); ++i)
		if(gpEngine->GetPlayerInfo(playerIndex, &PlayerInfo))
			if(PlayerInfo.friendsID == friendsID)
				return PlayerInfo.userid;
	
	return 0;
};