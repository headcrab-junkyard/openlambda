/*
 * This file is part of OGS Engine
 * Copyright (C) 2018, 2022 BlackPhrase
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

#include "OptionsDialog.hpp"

using namespace vgui;

COptionsDialog::COptionsDialog(Panel *apParent, OptionsDialogTabStyle aeTabStyle) : PropertyDialog(apParent, "OptionsDialog")
{
	SetProportional(true);
	SetDeleteSelfOnClose(true);
	SetBounds();
	SetSizeable(false);
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
	for(int i = 0; i < GetChildCount(); ++i)
	{
	};
};