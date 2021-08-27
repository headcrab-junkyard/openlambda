/*
 * This file is part of OGS Engine
 * Copyright (C) 2018, 2021 BlackPhrase
 *
 * OGS Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OGS Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OGS Engine. If not, see <http://www.gnu.org/licenses/>.
 */

/// @file

#include "NewGameDialog.hpp"

CNewGameDialog::CNewGameDialog(vgui::Panel *apParent)
	: BaseClass(apParent, "NewGameDialog")
{
	mpHelpText = new vgui::Label(this, "HelpText", "#GameUI_NewGameHelpText");
	
	mpTrainingRadio = new vgui::RadioButton(this, "Training", "#GameUI_TrainingRoom");
	
	mpEasyRadio = new vgui::RadioButton(this, "Easy", "#GameUI_Easy");
	mpMediumRadio = new vgui::RadioButton(this, "Medium", "#GameUI_Medium");
	mpHardRadio = new vgui::RadioButton(this, "Hard", "#GameUI_Hard");
	
	mpPlayButton = vgui::Button(this, "Play", "#GameUI_Play");
	
	auto pCancelButton{new vgui::Button(this, "Cancel", "#GameUI_Cancel")};
	
	LoadControlSettings("resource/NewGameDialog.res");
};

CNewGameDialog::~CNewGameDialog() = default;