/*
 * This file is part of OGS Engine
 * Copyright (C) 2018, 2020, 2022 BlackPhrase
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
/// @brief interface to running the engine from the UI dlls

#pragma once

#include "GameUI/IRunGameEngine.h"

class CRunGameEngine final : public IRunGameEngine
{
public:
	CRunGameEngine();
	/*virtual*/ ~CRunGameEngine();
	
	/// @return true if the engine is running, false otherwise
	/*virtual*/ bool IsRunning() override;

	/// Adds text to the engine command buffer
	/// Only works if IsRunning()
	/// @return true on success, false on failure
	/*virtual*/ bool AddTextCommand(const char *text) override;

	/// Runs the engine with the specified command line parameters
	/// Only works if !IsRunning()
	/// @return true on success, false otherwise
	/*virtual*/ bool RunEngine(const char *gameDir, const char *commandLineParams) override;

	/// @return true if the player is currently connected to a game server
	/*virtual*/ bool IsInGame() override;

	/// Gets info about the server the client is currently connected to
	/// @return true on success, false otherwise
	/*virtual*/ bool GetGameInfo(char *infoBuffer, int bufferSize) override;

	/*virtual*/ void SetTrackerUserID(int trackerID, const char *trackerName) override;

	/// Iterates users
	/// @return the number of users
	/*virtual*/ int GetPlayerCount() override;

	/// playerIndex is in the range [0, GetPlayerCount)
	/// @return a player id for a player
	/*virtual*/ unsigned int GetPlayerFriendsID(int playerIndex) override;

	/// Gets the in-game name of another user, returns nullptr if that user doesn't exist
	/*virtual*/ const char *GetPlayerName(int friendsID) override;

	/*virtual*/ const char *GetPlayerFriendsName(int friendsID) override;

	/// @return the build number of the engine
	/*virtual*/ unsigned int GetEngineBuildNumber() override;
	
	/// @return the product version of the mod being played (comes from steam.inf)
	/*virtual*/ const char *GetProductVersionString() override;
	
	unsigned int GetPlayerUserID(int friendsID) override;
};