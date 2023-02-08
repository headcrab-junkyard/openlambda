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

class CDescription;
class mpcontrol_t;
class CPanelListPanel;

class CCreateMultiplayerGameGameplayPage : public vgui::PropertyPage
{
	DECLARE_CLASS_SIMPLE(CCreateMultiplayerGameGameplayPage, vgui::PropertyPage);
public:
	CCreateMultiplayerGameGameplayPage(vgui::Panel *apParent, const char *asName);
	~CCreateMultiplayerGameGameplayPage();
	
	/// @return currently entered information about the server
	const char *GetHostName() const;
	const char *GetPassword() const;
	int GetMapPlayers() const;
protected:
	/// Called to get data from the page
	/*virtual*/ void OnApplyChanges() override;
private:
	const char *GetValue(const char *asCvarName, const char *asDefaultValue) const;
	
	void LoadGameOptionsList();
	
	/// Applies all the values in the page
	void GatherCurrentValues();
private:
	CDescription *mpDescription{nullptr};
	mpcontrol_t *mpList{nullptr};
	CPanelListPanel *mpOptionsList{nullptr};
};