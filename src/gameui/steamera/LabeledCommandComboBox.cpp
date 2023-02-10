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

#include "LabeledCommandComboBox.hpp"

// NOTE: memdbgon must be the last include file in a .cpp file!
#include <tier0/memdbgon.h>

//using namespace vgui;

CLabeledCommandComboBox::CLabeledCommandComboBox(vgui::Panel *apParent, const char *asPanelName)
{
	AddActionSignalTarget(this);
};

CLabeledCommandComboBox::~CLabeledCommandComboBox() = default;

void CLabeledCommandComboBox::Reset()
{
	if(mnStartSelection != -1)
		ActivateItem(mnStartSelection);
};

void CLabeledCommandComboBox::ApplyChanges()
{
	if(mnCurrentSelection == -1)
		return;
	
	if(mvItems.Count() < 1)
		return;
	
	Assert(mnCurrentSelection < mvItems.Count());
	auto pItem{&mvItems[mnCurrentSelection]};
	engine->ClientCmd_Unrestricted(pItem->msCommand);
	mnStartSelection = mnCurrentSelection;
};

void CLabeledCommandComboBox::AddItem(const char *asText, const char *asEngineCmd)
{
};

void CLabeledCommandComboBox::DeleteAllItems()
{
	BaseClass::DeleteAllItems();
	mvItems.RemoveAll();
};

void CLabeledCommandComboBox::ActivateItem(int anItemID)
{
};

bool CLabeledCommandComboBox::HasBeenModified()
{
	return mnCurrentSelection != mnStartSelection;
};

void CLabeledCommandComboBox::SetInitialItem(int anItemID)
{
};

const char *CLabeledCommandComboBox::GetActiveItemCommand() const
{
};