/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2023 BlackPhrase
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
/// @brief Base class for the left and right in-game menus that lay on the background

#pragma once

#include <vgui/controls/Button.h>

class CBackgroundMenuButton : public vgui::Button
{
	DECLARE_CLASS_SIMPLE(CBackgroundMenuButton, vgui::Button);
public:
	CBackgroundMenuButton(vgui::Panel *apParent, const char *asName);
	~CBackgroundMenuButton();
	
	virtual void OnCommand(const char *asCmd) override;
protected:
	vgui::Menu *RecursiveLoadGameMenu(KeyValues *apDataFile);
	
	// Makes the button transparent
	virtual void ApplySchemeSettings(vgui::IScheme *apScheme);
protected:
	vgui::Menu *mpMenu{nullptr};
private:
	vgui::IImage *mpImage{nullptr};
	vgui::IImage *mpMouseOverImage{nullptr};
};