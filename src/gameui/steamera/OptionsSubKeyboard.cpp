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

#include "OptionsSubKeyboard.hpp"

using namespace vgui;

COptionsSubKeyboard::COptionsSubKeyboard()
{
};

COptionsSubKeyboard::~COptionsSubKeyboard()
{
	DeleteSavedBindings();
};

void COptionsSubKeyboard::OnResetData()
{
	FillInCurrentBindings();
	if(IsVisible())
		mpKeyBindList->SetSelectedItem(0);
};

void COptionsSubKeyboard::OnApplyChanges()
{
	ApplyAllBindings();
	
	CGameUIConVarRef con_enable("con_enable");
	con_enable.SetValue(GetControlInt("ConsoleCheck", 0));
};

void COptionsSubKeyboard::CreateKeyBindingList()
{
};

void COptionsSubKeyboard::OnKeyCodeTyped(KeyCode aeCode)
{
	if(aeCode == KEY_ENTER)
		OnCommand("ChangeKey");
	else
		BaseClass::OnKeyCodeTyped(aeCode);
};

void COptionsSubKeyboard::OnCommand(const char *asCmd)
{
};

void COptionsSubKeyboard::ParseActionDescriptions()
{
};

KeyValues *COptionsSubKeyboard::GetItemForBinding(const char *asBinding)
{
};

void COptionsSubKeyboard::AddBinding(KeyValues *apItems, const char *asKeyName)
{
};

void COptionsSubKeyboard::ClearBindItems()
{
};

void COptionsSubKeyboard::RemoveKeyFromBindItems(KeyValues *apItems, const char *asKey)
{
};

void COptionsSubKeyboard::FillInCurrentBindings()
{
};

void COptionsSubKeyboard::DeleteSavedBindings()
{
};

void COptionsSubKeyboard::SaveCurrentBindings()
{
};

void COptionsSubKeyboard::BindKey(ButtonCode_t aeKey, const char *asBinding)
{
};

void COptionsSubKeyboard::UnbindKey(ButtonCode_t aeKey)
{
};

void COptionsSubKeyboard::ApplyAllBindings()
{
};

void COptionsSubKeyboard::FillInCurrentBindings()
{
};

void COptionsSubKeyboard::ItemSelected(int anItemID)
{
};

void COptionsSubKeyboard::Finish(ButtonCode_t aeCode)
{
};

void COptionsSubKeyboard::OnThink()
{
};

void COptionsSubKeyboard::OnKeyCodePressed(KeyCode aeCode)
{
};

void COptionsSubKeyboard::OpenKeyboardAdvancedDialog()
{
};