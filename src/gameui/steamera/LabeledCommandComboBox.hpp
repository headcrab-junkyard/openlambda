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

#pragma once

#include <vgui/controls/ComboBox.h>
//#include <vgui/controls/Label.h>
//#include <vgui/controls/Panel.h>

#include <tier1/UtlVector.h>

class CLabeledCommandComboBox : public vgui::ComboBox
{
	DECLARE_CLASS_SIMPLE(CLabeledCommandComboBox, vgui::ComboBox);
public:
	CLabeledCommandComboBox(vgui::Panel *apParent, const char *asPanelName);
	~CLabeledCommandComboBox();
	
	void Reset();
	
	void ApplyChanges();
	
	virtual void AddItem(const char *asText, const char *asEngineCmd);
	virtual void DeleteAllItems();
	
	virtual void ActivateItem(int anItemID);
	
	bool HasBeenModified();
	
	void SetInitialItem(int anItemID);
	
	const char *GetActiveItemCommand() const;
private:
	MESSAGE_FUNC_CHARPTR(OnTextChanged, "TextChanged", text);
private:
	constexpr auto MAX_NAME_LEN{256};
	constexpr auto MAX_COMMAND_LEN{256};
	
	struct CommandItem
	{
		char msName[MAX_NAME_LEN]{};
		char msCommand[MAX_COMMAND_LEN]{};
		int mnComboBoxID{0};
	};
	
	CUtlVector<CommandItem> mvItems;
	
	int mnCurrentSelection{-1};
	int mnStartSelection{-1};
};