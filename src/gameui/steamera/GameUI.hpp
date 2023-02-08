/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2018, 2020-2023 BlackPhrase
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

#pragma once

#include "GameUI/IGameUI.h"

class CGameUI final : public IGameUI
{
public:
	CGameUI();
	~CGameUI();
	
	void Initialize(CreateInterfaceFn *factories, int count) override;
	
	/// Called to setup the game UI
	void Start(/*cl_enginefunc_t*/ struct cl_enginefuncs_s *engineFuncs, int interfaceVersion, void /*IBaseSystem*/ *system) override;
	
	/// Called to shutdown the game UI system
	void Shutdown() override;

	int ActivateGameUI() override;
	int ActivateDemoUI() override;

	int HasExclusiveInput() override;

	void RunFrame() override;

	/// Called when the game connects to a server
	void ConnectToServer(const char *game, int IP, int port) override;
	
	/// Called when the game disconnects to a server
	void DisconnectFromServer() override;

	void HideGameUI() override;

	bool IsGameUIActive() override;

	void LoadingStarted(const char *resourceType, const char *resourceName) override;
	void LoadingFinished(const char *resourceType, const char *resourceName) override;

	void StartProgressBar(const char *progressType, int progressSteps) override;
	
	/// @return true if the screen should be updated
	int ContinueProgressBar(int progressPoint, float progressFraction) override;
	
	void StopProgressBar(bool bError, const char *failureReason, const char *extendedReason) override;

	/// Sets loading info text
	int SetProgressBarStatusText(const char *statusText) override;

	void SetSecondaryProgressBar(float progress) override;
	void SetSecondaryProgressBarText(const char *statusText) override;

	void ValidateCDKey(bool force, bool inConnect) override;

	void OnDisconnectFromServer(int eSteamLoginFailure, const char *username) override;
private:
	/// Searches for GameStartup*.mp3 files in the sound/ui folder and plays one
	void PlayGameStartupSound();
	
	/// Return false if you don't need to use the game startup music (in case it's handled by the background menu movies)
	bool ShouldUseGameStartupSound() const {return true;}
	
	void SendConnectedToGameMessage();
private:
	vgui2::IVGUI *mpVGUI{nullptr};
	vgui2::ISurface *mpVGUISurface{nullptr};
	
	IFileSystem *mpFileSystem{nullptr};
	
	IEngineVGui *mpEngineVGui{nullptr};
	
	IGameUIFuncs *mpGameUIFuncs{nullptr};
	
	IGameClientExports *mpGameClientExports{nullptr};
	
	int mnPlayGameStartupSound{0};
	
	int mnGameIP{0};
	int mnGamePort{0};
	
	bool mbActive{false};
};