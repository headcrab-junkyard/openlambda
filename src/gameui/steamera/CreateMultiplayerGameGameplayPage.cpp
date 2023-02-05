/*
 * This file is part of OGS Engine
 * Copyright (C) 2018, 2021, 2023 BlackPhrase
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

#include "CreateMultiplayerGameGameplayPage.hpp"

constexpr auto OPTIONS_DIR{"cfg"};
constexpr auto DEFAULT_OPTIONS_FILE{OPTIONS_DIR + "/settings_default.scr"};
constexpr auto OPTIONS_FILE{OPTIONS_DIR + "settings.scr"};

class CServerDescription : public CDescription
{
public:
	CServerDescription();
};

CCreateMultiplayerGameGameplayPage::CCreateMultiplayerGameGameplayPage(vgui::Panel *apParent, const char *asName)
	: BaseClass(apParent, asName)
{
	mpOptionsList = new CPanelListPanel(this, "GameOptions");
	
	mpDescription = new CServerDescription(mpOptionsList);
	mpDescription->InitFromFile(DEFAULT_OPTIONS_FILE);
	mpDescription->InitFromFile(OPTIONS_FILE);
	
	LoadControlSettings("Resource/CreateMultiplayerGameGameplayPage.res");
	
	LoadGameOptionsList();
};

CCreateMultiplayerGameGameplayPage::~CCreateMultiplayerGameGameplayPage()
{
	delete mpDescription;
};

const char *CCreateMultiplayerGameGameplayPage::GetHostName() const
{
	return GetValue("hostname", "Half-Life");
};

const char *CCreateMultiplayerGameGameplayPage::GetPassword() const
{
	return GetValue("sv_password", "");
};

int CCreateMultiplayerGameGameplayPage::GetMapPlayers() const
{
	return atoi(GetValue("maxplayers", "32"));
};

void CCreateMultiplayerGameGameplayPage::OnApplyChanges()
{
};

const char *CCreateMultiplayerGameGameplayPage::GetValue(const char *asCvarName, const char *asDefaultValue) const
{
};

void CCreateMultiplayerGameGameplayPage::LoadGameOptionsList()
{
};

void CCreateMultiplayerGameGameplayPage::GatherCurrentValues()
{
	if(!mpDescription)
		return;
};