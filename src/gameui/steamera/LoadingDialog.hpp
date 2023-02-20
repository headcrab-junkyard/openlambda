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
/// @brief Dialog for displaying level loading status

#pragma once

#include <vgui2/controls/Frame.h>

class CLoadingDialog : public vgui2::Frame
{
	DECLARE_CLASS_SIMPLE(CLoadingDialog, vgui2::Frame);
public:
	CLoadingDialog(vgui2::Panel *apParent);
	~CLoadingDialog();
	
	void Open();
	
	void DisplayGenericError(const char *asFailureReason, const char *asExtendedReason = nullptr);
	void DisplayVACBannedError();
	void DisplayNoSteamConnectionError();
	void DisplayLoggedInElsewhereError();
	
	bool SetProgressPoint(float afFraction);
	void SetStatusText(const char *asText);
	
	void SetSecondaryProgress(float afProgress);
	void SetSecondaryProgressText(const char *asText);
	
	bool SetShowProgressText(bool abState);
protected:
	virtual void OnThink();
	virtual void OnKeyCodePressed(vgui2::KeyCode aeCode);
	virtual void OnCommand(const char *asCmd);
	virtual void OnClose();
	
	virtual void PerformLayout();
	virtual void PaintBackground();
private:
	
private:
};

// Singleton accessor
CLoadingDialog *LoadingDialog();