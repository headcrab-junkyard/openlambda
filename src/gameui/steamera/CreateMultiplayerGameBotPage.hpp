/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2018, 2021, 2023 BlackPhrase
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
/// @brief advanced bot properties page of the create game server dialog

#pragma once

#include <vgui_controls/PropertyPage.h>

class CCvarToggleCheckButton;
//class CPanelListPanel;
//class CDescription;
//class mpcontrol_t;

class CCreateMultiplayerGameBotPage : public vgui::PropertyPage
{
	DECLARE_CLASS_SIMPLE(CCreateMultiplayerGameBotPage, vgui::PropertyPage);
public:
	CCreateMultiplayerGameBotPage(vgui::Panel *apParent, const char *asName, KeyValues *apBotKeys);
	~CCreateMultiplayerGameBotPage();
protected:
	// Reset values
	/*virtual*/ void OnResetChanges();
	
	// Called to get data from the page
	/*virtual*/ void OnApplyChanges();
private:
	void SetJoinTeamCombo(const char *asValue);
	
	void SetChatterCombo(const char *asValue);
private:
	KeyValues *mpSavedData{nullptr};
	
	CCvarToggleCheckButton *mpAllowRoguesCheckBox{nullptr};
	
	CCvarToggleCheckButton *mpAllowPistolsCheckBox{nullptr};
	CCvarToggleCheckButton *mpAllowShotgunsCheckBox{nullptr};
	CCvarToggleCheckButton *mpAllowSMGsCheckBox{nullptr};
	CCvarToggleCheckButton *mpAllowRiflesCheckBox{nullptr};
	CCvarToggleCheckButton *mpAllowSnipersCheckBox{nullptr};
	CCvarToggleCheckButton *mpAllowMachineGunsCheckBox{nullptr};
	CCvarToggleCheckButton *mpAllowGrenadesCheckBox{nullptr};
	
#ifdef OPENLAMBDA_CS_SHIELD_ENABLED
	CCvarToggleCheckButton *mpAllowShieldsCheckBox{nullptr};
#endif
	
	CCvarToggleCheckButton *mpJoinAfterPlayerCheckBox{nullptr};
	CCvarToggleCheckButton *mpDeferToHumanCheckBox{nullptr};
	
	vgui::ComboBox *mpJoinTeamCombo{nullptr};
	vgui::ComboBox *mpChatterCombo{nullptr};
	
	vgui::TextEntry *mpPrefixEntry{nullptr};
};