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
/// @brief List of players, their in-game name and their friends name

#pragma once

#include <vgui_controls/Frame.h>

class CPlayerListDialog : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE(CPlayerListDialog, vgui::Frame);
public:
	CPlayerListDialog(vgui::Panel *apParent);
	~CPlayerListDialog();
	
	/*virtual*/ void Activate();
private:
	MESSAGE_FUNC(OnItemSelected, "ItemSelected");
	
	/// Handles the AddFriend command
	/*virtual*/ void OnCommand(const char *asCmd);
	
	/// Toggles whether a user is muted or not
	void ToggleMuteStateOfSelectedUser();
	
	/// Walks the players and sets their info display in the list
	void RefreshPlayerProperties();
private:
	vgui::ListPanel *mpPlayerList{nullptr};
	vgui::Button *mpMuteButton{nullptr};
};