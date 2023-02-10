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
/// @brief server options page of the create game server dialog

#pragma once

#include <vgui_controls/PropertyPage.h>

//#include "CvarToggleCheckButton.hpp"

class CCreateMultiplayerGameServerPage : public vgui::PropertyPage
{
	DECLARE_CLASS_SIMPLE(CCreateMultiplayerGameServerPage, vgui::PropertyPage);
public:
	CCreateMultiplayerGameServerPage(vgui::Panel *apParent, const char *asName);
	~CCreateMultiplayerGameServerPage();
	
	/// Sets currently selected map the map combo box
	void SetMap(const char *asName);
	
	bool IsRandomMapSelected() const;
	
	const char *GetMapName() const;
	
	// (CS) Bots
	void EnableBots(KeyValues *apData);
	int GetBotQuota() const;
	bool GetBotsEnabled() const;
protected:
	/// Called to get the info from the dialog
	/*virtual*/ void OnApplyChanges() override;
	
	MESSAGE_FUNC(OnCheckButtonChecked, "CheckButtonChecked");
private:
	/// Loads the list of available maps into the map list
	void LoadMapList();
	
	/// Loads the list of available maps into the map list
	void LoadMaps(const char *asPathID);
private:
	static constexpr auto DATA_STR_LENGTH{64};
	
	char msMapName[DATA_STR_LENGTH]{};
	
	vgui::ComboBox *mpMapList{nullptr};
	
	vgui::CheckButton *mpEnableBotsCheckButton{nullptr};
	
	CCvarToggleCheckButton *mpEnableTutorCheckButton{nullptr};
	
	KeyValues *mpSavedData{nullptr};
};