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

#include "PlayerListDialog.hpp"
//#include "EngineInterface.hpp"
//#include "GameUI_Interface.hpp"

#include <KeyValues.h>

//#include <vgui/ILocalize.h>
//#include <vgui/ISurface.h>

//#include <vgui/controls/ListPanel.h>
//#include <vgui/controls/Label.h>
//#include <vgui/controls/Button.h>
//#include <vgui/controls/MessageBox.h>

#include <cl_dll/IGameClientExports.h> //<game/client/IGameClientExports.h>

//#include <steam/steam_api.h>

// NOTE: memdbgon must be the last include file in a .cpp file!
//#include <tier0/memdbgon.h>

//using namespace vgui;

CPlayerListDialog::CPlayerListDialog(vgui::Panel *apParent)
	: BaseClass(apParent, "PlayerListDialog")
{
	SetSize(320, 240);
	SetTitle("#GameUI_CurrentPlayers", true);
	
	mpMuteButton = new vgui::Button(this, "MuteButton", "");
	
	mpPlayerList = new ListPanel(this, "PlayerList");
	
	mpPlayerList->AddColumnHeader(0, "Name", "#GameUI_PlayerName", 180);
	mpPlayerList->AddColumnHeader(1, "Properties", "#GameUI_Properties", 80);
	
	mpPlayerList->SetEmptyListText("#GameUI_NoOtherPlayersInGame");
	
	LoadControlSettings("resource/PlayerListDialog.res"); // TODO: Resource
};

CPlayerListDialog::~CPlayerListDialog() = default;

void CPlayerListDialog::Activate()
{
	BaseClass::Activate();
	
	// Refresh player list
	mpPlayerList->DeleteAllItems();
	auto nMaxClients{gpEngine->GetMaxClients()};
	for(int i = 1; i <= nMaxClients; ++i)
	{
		// Get the player info from the engine
		player_info_t PlayerInfo{};
		
		if(!gpEngine->GetPlayerInfo(i, &PlayerInfo))
			continue;
		
		char sPlayerID[32]{};
		Q_snprintf(sPlayerID, sizeof(sPlayerID), "%d", i);
		
		// Collate user data then add it to the table
		auto pData{new KeyValues(sPlayerID)};
		
		data->SetString("Name", PlayerInfo.name);
		data->SetInt("index", i);
		
		// Add to the list
		mpPlayerList->AddItem(pData, 0, false, false);
	};
	
	// Refresh player properties info
	RefreshPlayerProperties();
	
	// Select the first item by default
	mpPlayerList->SetSingleSelectedItem(mpPlayerList->GetItemIDFromRow(0));
	
	// Toggle button states
	OnItemSelected();
};

void CPlayerListDialog::OnItemSelected()
{
	// Make sure the data is up-to-date
	RefreshPlayerProperties();
	
	// Set the button state based on the selected item
	bool bMuteButtonEnabled{false};
	
	if(mpPlayerList->GetSelectedItemsCount() > 0)
	{
		auto pData{mpPlayerList->GetItem(mpPlayerList->GetSelectedItem(0))};
		
		// TODO: check for pData here?
		
		player_info_t PlayerInfo{};
		
		int nLocalPlayer{gpEngine->GetLocalPlayer()};
		
		int nPlayerID{pData->GetInt("index")};
		bool bValidPlayer{gpEngine->GetPlayerInfo(nPlayerID, &PlayerInfo)};
		
		// Make sure the player is not a bot or the local player
		if(PlayerInfo.fakeplayer || nPlayerID == nLocalPlayer) // || PlayerInfo.friendsID == gpFriendsUser->GetFriendsID())
			bValidPlayer = false;
		
		if(bValidPlayer && GameClientExports() && GameClientExports()->IsPlayerGameVoiceMuted(nPlayerID))
			mpMuteButton->SetText("#GameUI_UnmuteIngameVoice");
		else
			mpMuteButton->SetText("#GameUI_MuteIngameVoice");
		
		if(GameClientExports() && bValidPlayer)
			bMuteButtonEnabled = true;
	}
	else
		mpMuteButton->SetText("#GameUI_MuteIngameVoice");
	
	mpMuteButton->SetEnabled(bMuteButtonEnabled);
};

void CPlayerListDialog::OnCommand(const char *asCmd)
{
	if(!stricmp(asCmd, "Mute"))
		ToggleMuteStateOfSelectedUser();
	else
		BaseClass::OnCommand(asCmd);
};

void CPlayerListDialog::ToggleMuteStateOfSelectedUser()
{
	if(!GameClientExports())
		return;
	
	for(int nSelectedItem = 0; nSelectedItem < mpPlayerList->GetSelectedItemsCount(); ++nSelectedItem)
	{
		auto pData{mpPlayerList->GetItem(mpPlayerList->GetSelectedItem(nSelectedItem))};
		if(!pData)
			return;
		
		auto nPlayerID{pData->GetInt("index")};
		Assert(nPlayerID);
		
		if(GameClientExports()->IsPlayerGameVoiceMuted(nPlayerID))
			GameClientExports()->UnmutePlayerGameVoice(nPlayerID);
		else
			GameClientExports()->MutePlayerGameVoice(nPlayerID);
	};
	
	RefreshPlayerProperties();
	OnItemSelected();
};

void CPlayerListDialog::RefreshPlayerProperties()
{
	for(int i = 0; i <= mpPlayerList->GetItemCount(); ++i)
	{
		auto pData{mpPlayerList->GetItem(i)};
		if(!pData)
			continue;
		
		// Assemble properties
		auto nPlayerID{pData->GetInt("index")};
		player_info_t PlayerInfo{};
		
		if(!gpEngine->GetPlayerInfo(nPlayerID, &PlayerInfo))
		{
			// Disconnected
			pData->SetString("properties", "Disconnected");
			continue;
		};
		
		pData->SetString("name", PlayerInfo.name);
		
		bool bMuted{false};
		
		if(GameClientExports() && GameClientExports()->IsPlayerGameVoiceMuted(nPlayerID))
			bMuted = true;
		
		bool bFriends{false}; // TODO: always false
		
		bool bBot{PlayerInfo.fakeplayer};
		
		pData->SetString("properties", "");
		
		if(bBot)
			pData->SetString("properties", "CPU Player");
		else if(bMuted && bFriends)
			pData->SetString("properties", "Friend; Muted");
		else if(bMuted)
			pData->SetString("properties", "Muted");
		else if(bFriends)
			pData->SetSting("properties", "Friend");
	};
	
	mpPlayerList->RereadAllItems();
};