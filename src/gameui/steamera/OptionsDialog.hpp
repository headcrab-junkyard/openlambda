/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2018, 2022-2023 BlackPhrase
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
/// @brief Holds all the game option pages

#pragma once

#include <vgui/controls/PropertyDialog.h>

//struct OptionData_t;
class COptionsSubAudio;
class COptionsSubVideo;

constexpr auto OPTIONS_MAX_NUM_ITEMS{15};

enum class eOptionsDialogTabStyle : int
{
	AllTabs,
	OnlyBindingTabs,
	
	SizeOf
};

class COptionsDialog : public vgui::PropertyDialog
{
	DECLARE_CLASS_SIMPLE(COptionsDialog, vgui::PropertyDialog);
public:
	COptionsDialog(vgui::Panel *apParent, eOptionsDialogTabStyle aeTabStyle = eOptionsDialogTabStyle::AllTabs);
	~COptionsDialog();
	
	/// Brings the dialog to the foreground
	/*virtual*/ void Activate();
	
	/// Opens the dialog
	void Run();
	
	/// Opens the gamma dialog directly
	void OpenGammaDialog();
	
	/// Called when the game UI is hidden
	//void OnGameUIHidden();
	MESSAGE_FUNC(OnGameUIHidden, "GameUIHidden");
private:
	COptionsSubAudio *mpOptionsSubAudio{nullptr};
	COptionsSubVideo *mpOptionsSubVideo{nullptr};
};