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
/// @brief panel used for inline editing of key bindings

#pragma once

#include <vgui/controls/Panel.h>

class CInlineEditPanel : public vgui::Panel
{
	DECLARE_CLASS_SIMPLE(CInlineEditPanel, vgui::Panel);
public:
	CInlineEditPanel() : BaseClass(nullptr, "InlineEditPanel"){}
	
	virtual void Paint() override
	{
		int nWidth, nHeight;
		GetSize(nWidth, nHeight);
		
		int x = 0, y = 0;
		
		// Draw a white rectangle around that cell
		vgui::surface()->DrawSetColor(0, 165, 255, 255); // TODO: fix magic
		vgui::surface()->DrawFilledRect(x, y, x + nWidth, y + nHeight);
	};
	
	virtual void ApplySchemeSettings(vgui::IScheme *apScheme)
	{
		BaseClass::ApplySchemeSettings(apScheme);
		SetBorder(apScheme->GetBorder("DepressedButtonBorder"));
	};
	
	void OnMousePressed(vgui::MouseCode aeCode) override
	{
		// Forward up mouse pressed messages to be handled by the key options
		if(GetParent())
			GetParent()->OnMousePressed(aeCode);
	};
	
	virtual void OnKeyCodeTyped(vgui::KeyCode aeCode) override
	{
		// Forward up
		if(GetParent())
			GetParent()->OnKeyCodeTyped(aeCode);
	};
};