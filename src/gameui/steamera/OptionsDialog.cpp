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

#include "OptionsDialog.hpp"

//#include <vgui/controls/Button.h>
//#include <vgui/controls/CheckButton.h>
//#include <vgui/controls/ProperySheet.h>
//#include <vgui/controls/Label.h>
//#include <vgui/controls/QueryBox.h>

//#include <vgui/ILocalize.h>
//#include <vgui/ISurface.h>
//#include <vgui/ISystem.h>
//#include <vgui/IVGui.h>

#include <KeyValues.h>

#include <OptionsSubKeyboard.h>
#include <OptionsSubMouse.h>
#include <OptionsSubAudio.h>
#include <OptionsSubVideo.h>
#include <OptionsSubVoice.h>
#include <OptionsSubMultiplayer.h>
//#include <OptionsSubDifficulty.h>

//#include <ModInfo.h>

// NOTE: memdbgon must be the last include file in a .cpp file!
//#include <tier0/memdbgon.h>

//using namespace vgui;

COptionsDialog::COptionsDialog(Panel *apParent, OptionsDialogTabStyle aeTabStyle) : BaseClass(apParent, "OptionsDialog")
{
	SetProportional(true);
	SetDeleteSelfOnClose(true);
	SetBounds(
		0, 0,
		vgui2::scheme()->GetProportionalScaledValueEx(GetScheme(), 512),
		vgui2::scheme()->GetProportionalScaledValueEx(GetScheme(), 415)
	);
	SetSizeable(false);
	
	// Debug timing code, this function takes too long
	//double s4{vgui2::system()->GetCurrentTime()};
	
	if(aeTabStyle == OPTIONS_DIALOG_ALL_TABS)
	{
		SetTitle("#GameUI_Options", true);
		
		//if(ModInfo().IsSinglePlayerOnly() && !ModInfo.NoDifficulty())
			//AddPage(new COptionsSubDifficulty(this), "#GameUI_Difficulty");
		
		AddPage(new COptionsSubKeyboard(this), "#GameUI_Keyboard");
		AddPage(new COptionsSubMouse(this), "#GameUI_Mouse");
		
		mpOptionsSubAudio = new COptionsSubAudio(this);
		AddPage(mpOptionsSubAudio, "#GameUI_Audio");
		
		mpOptionsSubVideo = new COptionsSubVideo(this);
		AddPage(mpOptionsSubVideo, "#GameUI_Video");
		
		if(!ModInfo().IsSinglePlayerOnly())
			AddPage(new COptionsSubVoice(this), "#GameUI_Voice");
		
		// Add the multiplayer page last, if we're combo single/multiplayer or just multiplayer
		if(
		(ModInfo.IsMultiplayerOnly() && !ModInfo.IsSinglePlayerOnly())
		|| 
		(!ModInfo.IsMultiplayerOnly() && !ModInfo.IsSinglePlayerOnly())
		)
			AddPage(new COptionsSubMultiplayer(this), "#GameUI_Multiplayer");
	}
	else if(aeTabStyle == OPTIONS_DIALOG_ONLY_BINDING_TABS)
	{
		SetTitle("#L4D360UI_Controller_Edit_Keys_Buttons", true);
		
		AddPage(new COptionsSubKeyboard(this), "#GameUI_Console_UserSettings");
	};
	
	//double s5{vgui2::system()->GetCurrentTime};
	//Msg("COptionsDialog::COptionsDialog(): %.3ms\n", (float)(s5 - s4) * 1000.0f);
	
	SetApplyButtonVisible(true);
	GetPropertySheet()->SetTabWidth(84);
};

COptionsDialog::~COptionsDialog() = default;

void COptionsDialog::Activate()
{
	BaseClass::Activate();
	EnableApplyButton(false);
};

void COptionsDialog::Run()
{
	SetTitle("#GameUI_Options", true);
	Activate();
};

void COptionsDialog::OpenGammaDialog()
{
	mpOptionsSubVideo->OpenGammaDialog();
};

void COptionsDialog::OnGameUIHidden()
{
	// Tell our children about it
	for(int i = 0; i < GetChildCount(); ++i)
	{
		auto pChild{GetChild(i)};
		if(pChild)
			PostMessage(pChild, new KeyValues("GameUIHidden"));
	};
};