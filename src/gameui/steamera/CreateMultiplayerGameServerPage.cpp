/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2018, 2021, 2023 BlackPhrase
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

#include "CreateMultiplayerGameServerPage.hpp"
#include "ModInfo.hpp"

constexpr auto RANDOM_MAP{"#GameUI_RandomMap"};

CCreateMultiplayerGameServerPage::CCreateMultiplayerGameServerPage(vgui::Panel *apParent, const char *asName)
	: BaseClass(apParent, asName)
{
	// We can use this if we decide that we want to put the "listen server" at the end of the game name
	mpMapList = new vgui::ComboBox(this, "MapList", 12, false);
	
	mpEnableBotsCheckButton = new vgui::CheckButton(this, "EnableBotsCheck", "");
	mpEnableBotsCheckButton->SetVisible(false);
	mpEnableBotsCheckButton->SetEnabled(false);
	
	LoadControlSettings("Resource/CreateMultiplayerGameServerPage.res");
	
	LoadMapList();
	
	msMapName[0] = '\0';
	
	// Initialize hostname
	SetControlString("ServerNameEdit", ModInfo().GetGameName() /*sHostName*/);
	
	// Initialize password
	//SetControlString("PasswordEdit", gpEngine->pfnGetCvarString("sv_password"));
	ConVarRef sv_password("sv_password");
	if(sv_password.IsValid())
		SetControlString("PasswordEdit", sv_password.GetString());
};

CCreateMultiplayerGameServerPage::~CCreateMultiplayerGameServerPage() = default;

void CCreateMultiplayerGameServerPage::SetMap(const char *asName)
{
	for(int i = 0; i < mpMapList->GetItemCount(); ++i)
	{
		if(!mpMapList->IsItemIDValid(i))
			continue;
		
		if(!stricmp(mpMapList->GetItemUserData(i)->GetString("mapname"), asName))
		{
			mpMapList->ActivateItem(i);
			break;
		};
	};
};

bool CCreateMultiplayerGameServerPage::IsRandomMapSelected() const
{
	const char *sMapName{mpMapList->GetActiveItemUserData()->GetString("mapname")};
	
	if(!stricmp(sMapName, RANDOM_MAP))
		return true;
	
	return false;
};

const char *CCreateMultiplayerGameServerPage::GetMapName() const
{
	int nMapCount{mpMapList->GetItemCount()};
	
	// Is there is only one entry then it's the special "select random map" entry
	if(nMapCount <= 1)
		return nullptr;
	
	const char *sMapName{mpMapList->GetActiveItemUserData()->GetString("mapname")};
	
	if(!strcmp(sMapName, RANDOM_MAP))
	{
		int nMapID{RandomInt(1, nMapCount - 1)};
		sMapName = mpMapList->GetItemUserData(nMapID)->GetString("mapname");
	};
	
	return sMapName;
};

void CCreateMultiplayerGameServerPage::EnableBots(KeyValues *apData)
{
	mpSavedData = apData;
	
	int nQuota{mpData->GetInt("bot_quota", 0)};
	SetControlInt("BotQuotaCombo", nQuota);
	mpEnableBotsCheckButton->SetSelected(nQuota > 0);
	
	int nDifficulty{apData->GetInt("bot_difficulty", 0)};
	nDifficulty = std::max(nDifficulty, 0);
	nDifficulty = std::min(nDifficulty, 3);
	
	char sButtonName[64]{};
	Q_snprintf(sButtonName, sizeof(sButtonName), "SkillLevel%d", nDifficulty);
	
	auto pButton{dynamic_cast<vgui::RadioButton*>(FindChildByName(sButtonName))};
	if(pButton)
		pButton->SetSelected(true);
};

int CCreateMultiplayerGameServerPage::GetBotQuota() const
{
};

bool CCreateMultiplayerGameServerPage::GetBotsEnabled() const
{
};

void CCreateMultiplayerGameServerPage::OnApplyChanges()
{
	KeyValues *pKeyValues{mpMapList->GetActiveItemUserData()};
	
	Q_strncpy(msMapName, pKeyValues->GetString("mapname", ""), DATA_STR_LENGTH);
	
	if(mpSavedData)
	{
		int nBotQuota{GetControlInt("BotQuotaCombo", 0)};
		
		if(!mpEnableBotsCheckButton->IsSelected())
			nBotQuota = 0;
		
		mpSavedData->SetInt("bot_quota", nBotQuota);
		ConVarRef bot_quota("bot_quota");
		bot_quota.SetValue(nBotQuota);
		
		int nBotDifficuly{0};
		
		char sButtonName[64]{};
		
		for(int i = 0; i < 4; ++i)
		{
			Q_snprintf(sButtonName, sizeof(sButtonName), "SkillLevel%d", i);
			
			auto pButton{dynamic_cast<vgui::RadioButton*>(FindChildByName(sButtonName))};
			if(pButton)
				if(pButton->IsSelected())
				{
					nBotDifficuly = i;
					break;
				};
		};
		
		mpSavedData->SetInt("bot_difficulty", nBotDifficuly);
		ConVarRef bot_difficulty("bot_difficulty");
		bot_difficulty.SetValue(nBotDifficuly);
	};
};

void CCreateMultiplayerGameServerPage::OnCheckButtonChecked()
{
	SetControlEnabled("SkillLevel0", mpEnableBotsCheckButton->IsSelected());
	SetControlEnabled("SkillLevel1", mpEnableBotsCheckButton->IsSelected());
	SetControlEnabled("SkillLevel2", mpEnableBotsCheckButton->IsSelected());
	SetControlEnabled("SkillLevel3", mpEnableBotsCheckButton->IsSelected());
	SetControlEnabled("BotQuotaCombo", mpEnableBotsCheckButton->IsSelected());
	SetControlEnabled("BotQuotaLabel", mpEnableBotsCheckButton->IsSelected());
	SetControlEnabled("BotDifficultyLabel", mpEnableBotsCheckButton->IsSelected());
};

void CCreateMultiplayerGameServerPage::LoadMapList()
{
	// Clear the current list (if there's something)
	mpMapList->DeleteAllItems();
	
	// Add a special "name" to represent loading a randomly selected map
	mpMapList->AddItem(RANDOM_MAP, new KeyValues("data", "mapname", RANDOM_MAP));
	
	// Iterate the filesystem to get the list of all the files
	const char *sPathID{"MOD"};
	if(!stricmp(ModInfo().GetGameName(), "Half-Life"))
		sPathID = nullptr; // "hl" is the base dir
	
	// Load the maps from the game dir
	LoadMaps(sPathID);
	
	// If we're not the Valve dir and using a "fallback_dir" in liblist.gam then include those maps... (sPathID is nullptr if we're "Half-Life")
	const char *sFallbackDir{ModInfo().GetFallbackDir()};
	if(sPathID && sFallbackDir[0])
		LoadMaps("GAME_FALLBACK");
	
	// Set the first item to be selected
	mpMapList->ActivateItem(0);
};

void CCreateMultiplayerGameServerPage::LoadMaps(const char *asPathID)
{
	FileFindHandle_t hFile{nullptr};
	
	KeyValues *pHiddenMaps{ModInfo().GetHiddenMaps()};
	
	const char *sFileName{gpFileSystem->FindFirst("maps/*.bsp", &hFile)};
	
	char sMapName[256]{};
	
	while(sFileName)
	{
		// FindFirst ignores the asPathID, so check it here
		// TODO: this doesn't find maps in fallback dirs
		Q_snprintf(sMapName, sizeof(sMapName), "maps/%s", sFileName);
		
		if(!gpFileSystem->FileExists(sMapName, asPathID))
			goto nextFile;
		
		// Remove the text 'maps/' and '.bsp' from the file name to get the map name
		
		const char *sMaps{Q_strstr(sFileName, "maps")};
		if(sMaps)
			Q_strncpy(sMapName, sMaps + 5, sizeof(sMapName) - 1); // maps + \\ = 5
		else
			Q_strncpy(sMapName, sFileName, sizeof(sMapName) - 1);
		
		char *sExt{Q_strstr(sMapName, ".bsp")};
		if(sExt)
			*sExt = '\0';
		
		// HACK: Strip out singleplayer HL maps
		// TODO: This needs to be specified in a separate file
		if(!stricmp(ModInfo().GetGameName(), "Half-Life") && (sMapName[0] == 'c' || sMapName[0] == 't') && sMapName[1] >= '0' && sMapName[1] <= '5' && sMapName[2] == 'a')
			goto nextFile;
		
		// Strip out maps that shouldn't be displayed
		if(pHiddenMaps)
			if(pHiddenMaps->GetInt(sMapName, 0))
				goto nextFile;
		
		// Add to the map list
		mpMapList->AddItem(sMapName, new KeyValues("data", "mapname", sMapName));
		
		// Get the next file
nextFile:
		sFileName = gpFileSystem->FindNext(hFile);
	};
	
	gpFileSystem->FindClose(hFile);
};