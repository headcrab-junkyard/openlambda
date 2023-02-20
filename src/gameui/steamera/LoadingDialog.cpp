/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2018, 2023 BlackPhrase
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

#include "LoadingDialog.hpp"
#include "EngineInterface.hpp"

#include <IGameUIFuncs.h>

//using namespace vgui;

CLoadingDialog::CLoadingDialog(vgui2::Panel *apParent)
{
};

CLoadingDialog::~CLoadingDialog() = default;

void CLoadingDialog::Open()
{
};

void CLoadingDialog::DisplayGenericError(const char *asFailureReason, const char *asExtendedReason)
{
};

void CLoadingDialog::DisplayVACBannedError()
{
	if(mbConsoleStyle)
		return;
	
	SetupControlSettingsForErrorDisplay("Resource/LoadingDialogErrorVACBanned.res");
	SetTitle("#VAC_ConnectionRefusedTitle", true);
};

void CLoadingDialog::DisplayNoSteamConnectionError()
{
	if(mbConsoleStyle)
		return;
	
	SetupControlSettingsForErrorDisplay("Resource/LoadingDialogErrorNoSteamConnection.res");
};

void CLoadingDialog::DisplayLoggedInElsewhereError()
{
	if(mbConsoleStyle)
		return;
	
	SetupControlSettingsForErrorDisplay("Resource/LoadingDialogErrorLoggedInElsewhere.res");
	mpCancelButton->SetText("#GameUI_RefreshLogin_Login");
	mpCancelButton->SetCommand("Login");
};

bool CLoadingDialog::SetProgressPoint(float afFraction)
{
};

void CLoadingDialog::SetStatusText(const char *asText)
{
	if(mbConsoleStyle)
		return;
	
	mpInfoLabel->SetText(asText);
};

void CLoadingDialog::SetSecondaryProgress(float afProgress)
{
};

void CLoadingDialog::SetSecondaryProgressText(const char *asText)
{
	if(mbConsoleStyle)
		return;
	
	SetControlString("SecondaryProgressLabel", asText);
};

bool CLoadingDialog::SetShowProgressText(bool abState)
{
	if(mbConsoleStyle)
		return false;
	
	bool bResult{mpInfoLabel->IsVisible()};
	if(bResult != abState)
	{
		SetupControlSettings(abState);
		mpInfoLabel->SetVisible(abState);
	};
	return bResult;
};

void CLoadingDialog::OnThink()
{
};

void CLoadingDialog::OnKeyCodePressed(vgui2::KeyCode aeCode)
{
	if(mbConsoleStyle)
		return;
	
	if(aeCode == KEY_ESCAPE)
		OnCommand("Cancel");
	else
		BaseClass::OnKeyCodePressed(aeCode);
};

void CLoadingDialog::OnCommand(const char *asCmd)
{
	if(!stricmp(asCmd, "Cancel"))
	{
		// Disconnect from the server
		gpEngine->ClientCmd_Unrestricted("disconnect\n");
		
		Close();
	}
	else
		BaseClass::OnCommand(asCmd);
};

void CLoadingDialog::OnClose()
{
	// Remove any rendering restrictions
	HideOtherDialogs(false);
	
	BaseClass::OnClose();
};

void CLoadingDialog::PerformLayout()
{
};

void CLoadingDialog::PaintBackground()
{
};