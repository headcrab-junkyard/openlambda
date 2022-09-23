/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2018, 2022 BlackPhrase
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
/// @brief game module game cliient event listener

#pragma once

#include <next/engine/IGameClientEventListener.hpp>

class CGameClientEventListener final : public IGameClientEventListener
{
public:
	CGameClientEventListener();
	~CGameClientEventListener();

	//void Release() override {delete this;}

	bool OnClientConnect(int anClientID, const char *asName, const char *asNetAdr, char asRejectReason[128]) override;
	void OnClientDisconnect(int anClientID) override;

	void OnClientPutInServer(int anClientID) override;

	void OnClientCommand(int anClientID, const ICmdArgs &aArgs) override;

	void OnClientUserInfoChanged(int anClientID, char *asUserInfo) override;
	
	int OnInconsistentFileFound(int anClientID, const char *asFileName, char *asDisconnectMsg) override;
public: //private:
	/// Called when a client sent a "kill" command (handled by OnClientCommand)
	void OnClientKill(int anClientID);
};