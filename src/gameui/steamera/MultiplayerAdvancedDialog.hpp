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
/// @brief Displays a game-specific list of options

#pragma once

#include <vgui/controls/Frame.h>

#include "ScriptObject.hpp"

//#include <vgui/KeyCode.h>

class CMultiplayerAdvancedDialog : public vgui2::Frame
{
	DECLARE_CLASS_SIMPLE(CMultiplayerAdvancedDialog, vgui2::Frame);
public:
	CMultiplayerAdvancedDialog(vgui::Panel *apParent);
	~CMultiplayerAdvancedDialog();
	
	virtual void Activate();
private:
	void CreateControls();
	void DestroyControls();
	
	void GatherCurrentValues();
	void SaveValues();
	
	virtual void OnCommand(const char *asCmd);
	virtual void OnKeyCodeTyped(vgui2::KeyCode aeCode);
	virtual void OnClose();
private:
	CInfoDescription *mpDescription{nullptr};
	
	mpcontrol_t *mpList{nullptr};
	
	CPanelListPanel *mpListPanel{nullptr};
};