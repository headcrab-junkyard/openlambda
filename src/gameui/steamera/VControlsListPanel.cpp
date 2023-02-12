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

#include "VControlsListPanel.hpp"
#include "InlineEditPanel.hpp"

//#include "GameUI_Interface.hpp"
//#include "EngineInterface.hpp"

//#include <vgui/IInput.h>
//#include <vgui/IScheme.h>
//#include <vgui/ISurface.h>
//#include <vgui/IVGui.h>
//#include <vgui/Cursor.h>

//#include <KeyValues.h>

// NOTE: memdbgon must be the last include file in a .cpp file!
//#include <tier0/memdbgon.h>

//using namespace vgui;

VControlsListPanel::VControlsListPanel(vgui2::Panel *apParent, const char *asListName) : BaseClass(apParent, asListName)
{
	mpInlineEditPanel = new CInlineEditPanel();
};

VControlsListPanel::~VControlsListPanel()
{
	mpInlineEditPanel->MarkForDeletion();
};

void VControlsListPanel::StartCaptureMode(vgui::HCursor ahCursor)
{
	mbCaptureMode = true;
	EnterEditMode(mnClickRow, 1, mpInlineEditPanel);
	vgui::input()->SetMouseFocus(mpInlineEditPanel->GetVPanel());
	vgui::input()->SetMouseCapture(mpInlineEditPanel->GetVPanel());
	
	engine->StartKeyTrapMode();
	
	if(ahCursor)
	{
		mpInlineEditPanel->SetCursor(ahCursor);
		
		// Save off the cursor position so we can restore it
		vgui::input()->GetCursorPos(mnMouseX, mnMouseY);
	};
};

void VControlsListPanel::EndCaptureMode(vgui::HCursor ahCursor)
{
	mbCaptureMode = false;
	vgui::input()->SetMouseCapture(nullptr);
	LeaveEditMode();
	RequestFocus();
	vgui::input()->SetMouseFocus(GetVPanel());
	
	if(ahCursor)
	{
		mpInlineEditPanel->SetCursor(ahCursor);
		
		vgui::surface()->SetCursor(ahCursor);
		
		if(ahCursor != dc_none)
			vgui::input()->SetCursorPos(mnMouseX, mnMouseY);
	};
};

bool VControlsListPanel::IsCapturing() const
{
	return mbCaptureMode;
};

void VControlsListPanel::OnMousePressed(vgui::MouseCode aeCode)
{
	if(IsCapturing())
	{
		// Forward up mouse pressed messages to be handled by the key options
		if(GetParent())
			GetParent()->OnMousePressed(aeCode);
	}
	else
		BaseClass::OnMousePressed(aeCode);
};

void VControlsListPanel::OnMouseDoublePressed(vgui::MouseCode aeCode)
{
	if(IsItemIDValid(GetSelectedItem()))
	{
		// Enter capture mode
		OnKeyCodePressed(KEY_ENTER);
	}
	else
		BaseClass::OnMouseDoublePressed(aeCode);
};

void VControlsListPanel::SetItemOfInterest(int anItemID)
{
	mnClickRow = anItemID;
};

int VControlsListPanel::GetItemOfInterest() const
{
	return mnClickRow;
};

void VControlsListPanel::ApplySchemeSettings(vgui::IScheme *apScheme)
{
	BaseClass::ApplySchemeSettings(apScheme);
	mhFont = apScheme->GetFont("Default", IsProportional());
};