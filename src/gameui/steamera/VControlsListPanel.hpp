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
/// @brief Special list subclass to handle drawing of trap mode prompt on top of lists client area

#pragma once

#include <vgui/controls/SectionedListPanel.h>

class CInlineEditPanel;

class VControlsListPanel : public vgui2::SectionedListPanel
{
	DECLARE_CLASS_SIMPLE(VControlsListPanel, vgui2::SectionedListPanel);
public:
	VControlsListPanel(vgui2::Panel *apParent, const char *asListName);
	virtual ~VControlsListPanel();
	
	/// Start capture prompt display
	virtual void StartCaptureMode(vgui::HCursor ahCursor = nullptr);
	
	/// Finish capture prompt display
	virtual void EndCaptureMode(vgui::HCursor ahCursor = nullptr);
	
	/// @return true if we're currently waiting to capture a key
	virtual bool IsCapturing() const;
	
	/// Forwards mouse pressed messages up to keyboard page when in capture
	virtual void OnMousePressed(vgui::MouseCode aeCode) override;
	
	/// Input handler
	virtual void OnMouseDoublePressed(vgui::MouseCode aeCode) override;
	
	/// Set which item should be associated with the prompt
	virtual void SetItemOfInterest(int anItemID);
	
	/// Retrieve row, column of interest
	virtual int GetItemOfInterest() const;
private:
	void ApplySchemeSettings(vgui::IScheme *apScheme);
private:
	/// Panel used to edit
	CInlineEditPanel *mpInlineEditPanel{nullptr};
	
	/// Font to use for showing the prompt
	vgui::HFont mhFont{INVALID_FONT};
	
	int mnMouseX{0}, mnMouseY{0};
	
	// If we're showing the prompt (= in capture mode), where?
	int mnClickRow{0};
	
	// Are we showing the prompt?
	bool mbCaptureMode{false};
};