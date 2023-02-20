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

#include "MultiplayerAdvancedDialog.hpp"

//using namespace vgui;

CMultiplayerAdvancedDialog::CMultiplayerAdvancedDialog(vgui::Panel *apParent) : BaseClass(nullptr, "MultiplayerAdvancedDialog")
{
	SetBounds(0, 0, 372, 160);
	SetSizeable(false);
	
	SetTitle("#GameUI_MultiplayerAdvanced", true);
	
	auto pCancelButton{new Button(this, "Cancel", "#GameUI_Cancel")};
	pCancelButton->SetCommand("Close");
	
	auto pOkButton{new Button(this, "OK", "#GameUI_OK")};
	pOkButton->SetCommand("Ok");
	
	mpListPanel = new CPanelListPanel(this, "PanelListPanel");
	
	mpDescription = new CInfoDescription(mpListPanel);
	mpDescription->InitFromFile(DEFAULT_OPTIONS_FILE);
	mpDescription->InitFromFile(OPTIONS_FILE);
	mpDescription->TransferCurrentValues(nullptr);
	
	LoadControlSettings("Resource\\MultiplayerAdvancedDialog.res");
	CreateControls();
	
	MoveToCenterOfScreen();
	SetSizeable(false);
	SetDeleteSelfOnClose(true);
};

CMultiplayerAdvancedDialog::~CMultiplayerAdvancedDialog()
{
	delete mpDescription;
};

void CMultiplayerAdvancedDialog::Activate()
{
	BaseClass::Activate();
	vgui2::input()->SetAppModalSurface(GetVPanel());
};

void CMultiplayerAdvancedDialog::CreateControls()
{
};

void CMultiplayerAdvancedDialog::DestroyControls()
{
	mpcontrol_t *p{mpList};
	mpcontrol_t *n{nullptr};
	
	while(p)
	{
		n = p->next;
		
		delete p->pControl;
		delete p->pPrompt;
		
		delete p;
		p = n;
	};
	
	mpList = nullptr;
};

void CMultiplayerAdvancedDialog::GatherCurrentValues()
{
};

void CMultiplayerAdvancedDialog::SaveValues()
{
	// Get the values from the controls
	GatherCurrentValues();
	
	// Create the game.cfg file
	if(mpDescription)
	{
		// Add settings to config.cfg
		mpDescription->WriteToConfig();
		
		gpFullFileSystem->CreateDirHierarchy(OPTIONS_DIR);
		FileHandle_t OptionsFile{gpFullFileSystem->Open(OPTIONS_FILE, "wb")};
		if(OptionsFile)
		{
			mpDescription->WriteToScriptFile(OptionsFile);
			gpFullFileSystem->Close(OptionsFile);
		};
	};
};

void CMultiplayerAdvancedDialog::OnCommand(const char *asCmd)
{
	if(!stricmp(asCmd, "Ok"))
	{
		//OnApplyChanges();
		SaveValues();
		OnClose();
		return;
	};
	
	BaseClass::OnCommand(asCmd);
};

void CMultiplayerAdvancedDialog::OnKeyCodeTyped(vgui2::KeyCode aeCode)
{
	// Force ourselves to be closed if the escape key is pressed
	if(aeCode == KEY_ESCAPE)
		Close();
	else
		BaseClass::OnKeyCodeTyped(aeCode);
};

void CMultiplayerAdvancedDialog::OnClose()
{
	BaseClass::OnClose();
	MarkForDeletion();
};