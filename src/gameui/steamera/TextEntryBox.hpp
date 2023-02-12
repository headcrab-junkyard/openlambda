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
/// @brief Popup dialog with a text entry, extending the QueryBox, which extends the MessageBox

#pragma once

#include <vgui/controls/QueryBox.h>

#include <KeyValues.h>

class vgui::Frame;
class vgui::TextEntry;
class vgui::Panel;

class CCvarTextEntry;

class CTextEntryBox : public vgui::QueryBox
{
	DECLARE_CLASS_SIMPLE(CTextEntryBox, vgui::QueryBox);
public:
	CTextEntryBox(const char *asTitle, const char *asLabelText, const char *asEntryText, bool abIsCvar, vgui::Panel *apParent = nullptr);
	virtual ~CTextEntryBox();
	
	/// Layout override to position the label and text entry
	virtual void PerformLayout() override;
	
	/// Show window override to give focus to text entry
	virtual void ShowWindow(vgui::Frame *apFrameOver) override;
protected:
	virtual void OnKeyCodeTyped(vgui::KeyCode aeCode);
	
	/// Handle button presses
	void OnCommand(const char *asCmd);
protected:
	CCvarTextEntry *mpCvarEntry{nullptr};
	vgui::TextEntry *mpEntry{nullptr};
};