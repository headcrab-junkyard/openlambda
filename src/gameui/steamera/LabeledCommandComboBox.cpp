/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2023-2024 BlackPhrase
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

//#include "EngineInterface.h"
//#include <KeyValues.h>
//#include <vgui/ILocalize.h>

// NOTE: memdbgon must be the last include file in a .cpp file!
#include <tier0/memdbgon.h>

//using namespace vgui;

CLabeledCommandComboBox::CLabeledCommandComboBox(vgui::Panel *apParent, const char *asPanelName) : vgui::ComboBox(apParent, asPanelName, 6, false)
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
	int nIndex{mvItems.AddToTail()};
	COMMANDITEM *pItem{&mvItems[nIndex]};
	
	pItem->comboBoxID = BaseClass::AddItem(asText, nullptr);
	
	Q_strncpy(pItem->name, asText, sizeof(pItem->name));
	
	if(asText[0] == '#')
	{
		// Need to localize the string
		wchar_t *sLocalized{gpVGuiLocalize->Find(asText)};
		if(sLocalized)
			gpVGuiLocalize->ConvertUnicodeToANSI(sLocalized, pItem->name, sizeof(pItem->name));
	};
	
	Q_strncpy(pItem->command, asEngineCmd, sizeof(pItem->command));
};

void CLabeledCommandComboBox::DeleteAllItems()
{
	BaseClass::DeleteAllItems();
	mvItems.RemoveAll();
};

void CLabeledCommandComboBox::ActivateItem(int anItemID)
{
	if(anItemID < mvItems.Count())
	{
		int nComboBoxID{mvItems[anItemID].comboBoxID};
		BaseClass::ActivateItem(anItemID);
		mnCurrentSelection = anItemID;
	};
};

bool CLabeledCommandComboBox::HasBeenModified()
{
	return mnCurrentSelection != mnStartSelection;
};

void CLabeledCommandComboBox::SetInitialItem(int anItemID)
{
	if(anItemID < mvItems.Count())
	{
		mnStartSelection = anItemID;
		int nComboBoxID{mvItems[anItemID].comboBoxID};
		ActivateItem(nComboBoxID);
	};
};

const char *CLabeledCommandComboBox::GetActiveItemCommand() const
{
	if(mnCurrentSelection == -1)
		return nullptr;
	
	COMMANDITEM *pItem{&mvItems[mnCurrentSelection]};
	return pItem->command;
};

void CLabeledCommandComboBox::OnTextChanged(const char *asText)
{
	for(int i = 0; i < mvItems.Count(); ++i)
	{
		COMMANDITEM *pItem{&mvItems[i]};
		if(!stricmp(pItem->name, asText))
		{
			//gpEngine->pfnClientCmd(pItem->command);
			mnCurrentSelection = i;
			break;
		};
	};
	
	if(HasBeenModified())
		PostActionSignal(new KeyValues("ControlModified"));
	
	//PostMessage(GetParent()->GetVPanel(), new vgui::KeyValues("TextChanged", "text", asText));
};