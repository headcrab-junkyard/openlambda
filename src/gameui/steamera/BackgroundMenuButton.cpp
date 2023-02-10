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

#include "BackgroundMenuButton.hpp"

//#include <KeyValues.h>

//#include <vgui/IImage.h>
//#include <vgui/IScheme.h>

//#include <vgui/controls/Menu.h>
//#include <vgui/controls/MenuItem.h>

// memdbgon must be the last include file in a .cpp file!
//#include <tier0/memdbgon.h>

//using namespace vgui;

CBackgroundMenuButton::CBackgroundMenuButton(vgui::Panel *apParent, const char *asName) : BaseClass(apParent, asName, ""){}

CBackgroundMenuButton::~CBackgroundMenuButton() = default;

void CBackgroundMenuButton::OnCommand(const char *asCmd)
{
	BaseClass::OnCommand(asCmd);
};

vgui::Menu *CBackgroundMenuButton::RecursiveLoadGameMenu(KeyValues *apDataFile)
{
};

void CBackgroundMenuButton::ApplySchemeSettings(vgui::IScheme *apScheme)
{
	BaseClass::ApplySchemeSettings(apScheme);
	
	// Hack some colors in
	SetFgColor(Color(255, 255, 255, 255));
	SetBgColor(Color(0, 0, 0, 0));
	SetDefaultColor(Color(255, 255, 255, 255), Color(0, 0, 0, 0));
	SetArmedColor(Color(255, 255, 0, 255), Color(0, 0, 0, 0));
	SetDepressedColor(Color(255, 255, 0, 255), Color(0, 0, 0, 0));
	
	SetContentAlignment(Label::a_west);
	
	SetBorder(nullptr);
	SetDefaultBorder(nullptr);
	SetDepressedBorder(nullptr);
	SetKeyFocusBorder(nullptr);
	
	SetTextInset(0, 0);
	SetAlpha(0);
	
	// TODO: uncomment?
/*
	// Sounds disabled for this button (since it's so big now)
	SetArmedSound("UI/buttonrollover.wav");
	SetDepressedSound("UI/buttonclick.wav");
	SetReleasedSound("UI/buttonclickrelease.wav");
*/
};