/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2016-2017, 2021 BlackPhrase
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

#include "GameUI/IGameUI.hpp"

class CGameUI : public IGameUI
{
public:
	void Initialize(CreateInterfaceFn *factories, int count);
	void Start(struct cl_enginefuncs_s *engineFuncs, int interfaceVersion, void *system);
	void Shutdown();

	int ActivateGameUI();
	int ActivateDemoUI();

	int HasExclusiveInput();

	void RunFrame();

	void ConnectToServer(const char *game, int IP, int port);
	void DisconnectFromServer();

	void HideGameUI();
	bool IsGameUIActive();

	void LoadingStarted(const char *resourceType, const char *resourceName);
	void LoadingFinished(const char *resourceType, const char *resourceName);

	void StartProgressBar(const char *progressType, int progressSteps);
	int ContinueProgressBar(int progressPoint, float progressFraction);
	void StopProgressBar(bool bError, const char *failureReason, const char *extendedReason = NULL);

	int SetProgressBarStatusText(const char *statusText);

	void SetSecondaryProgressBar(float progress);
	void SetSecondaryProgressBarText(const char *statusText);

private:
	bool mbActive;
};