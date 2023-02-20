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

#include "CreateMultiplayerGameDialog.hpp"
//#include "CreateMultiplayerGameServerPage.hpp"
//#include "CreateMultiplayerGameGameplayPage.hpp"
//#include "CreateMultiplayerGameBotPage.hpp"

//#include "EngineInterface.hpp"
//#include "ModInfo.hpp"
//#include "GameUI_Interface.hpp"

//#include <vgui/ILocalize.h>

//#include <FileSystem.h>
#include <KeyValues.h>

//#include <cstdio>

// NOTE: memdbgon must be the last include file in a .cpp file!
//#include <tier0/memdbgon.h>

//using namespace vgui;

CCreateMultiplayerGameDialog::CCreateMultiplayerGameDialog(vgui::Panel *apParent) : BaseClass(apParent, "CreateMultiplayerGameDialog")
{
	SetDeleteSelfOnClose(true);
	SetSize(348, 460);
	
	SetTitle("#GameUI_CreateServer", true);
	SetOKButtonText("#GameUI_Start");
	
	// BP: genius
	if(
	!stricmp(ModInfo().GetGameName(), "Counter-Strike")
	||
	!stricmp(ModInfo().GetGameName(), "Counter-Strike Source") // BP: hmm
	)
		mbBotsEnabled = true;
	
	mpServerPage = new CCreateMultiplayerGameServerPage(this, "ServerPage");
	mpGameplayPage = new CCreateMultiplayerGameGameplayPage(this, "GameplayPage");
	
	AddPage(mpServerPage, "#GameUI_Server");
	AddPage(mpGameplayPage, "#GameUI_Game");
	
	// Create a KeyValues object to load/save config options
	mpSavedData = new KeyValues("ServerConfig");
	
	// Load the config data
	if(mpSavedData)
	{
		// This is game-specific data, so it should live in GAME, not CONFIG
		mpSavedData->LoadFromFile(gpFullFileSystem, "ServerConfig.vdf", "Game");
		
		auto sStartMap{mpSavedData->GetString("map", "")};
		if(sStartMap[0])
			mpServerPage->SetMap(sStartMap);
	};
	
	if(mbBotsEnabled)
	{
		// Add a page of advanced bot controls
		// NOTE: these controls will use the bot keys to initialize their values
		mpBotPage = new CCreateMultiplayerGameBotPage(this, "BotPage", mpSavedData);
		AddPage(mpBotPage, "#GameUI_CPUPlayerOptions");
		mpServerPage->EnableBots(mpSavedData);
	};
};

CCreateMultiplayerGameDialog::~CCreateMultiplayerGameDialog()
{
	if(mpSavedData)
	{
		mpSavedData->deleteThis();
		mpSavedData = nullptr;
	};
};

bool CCreateMultiplayerGameDialog::OnOK(bool abApplyOnly)
{
	// Reset server enforced cvars
	gpCVars->RevertFlaggedConVars(FCVAR_REPLICATED);
	
	// Cheats were disabled; revert all cheat cvars to their default values
	// This must be done heading into multiplayer games because people can play demon etc and set cheat cvars with sv_cheats 0
	gpCVars->RevertFlaggedConVars(FCVAR_CHEAT);
	
	DevMsg("FCVAR_CHEAT cvars reverted to defaults.\n");
	
	BaseClass::OnOK(abApplyOnly);
	
	// Get these values from the server page and store them temporarily
	char sMapName[64]{};
	Q_strncpy(sMapName, mpServerPage->GetMapName(), sizeof(sMapName));
	
	char sHostName[64]{};
	Q_strncpy(sHostName, mpGameplayPage->GetHostName(), sizeof(sHostName));
	
	char sPassword[64]{};
	Q_strncpy(sPassword, mpGameplayPage->GetPassword(), sizeof(sPassword));
	
	// Save the config data
	if(mpSavedData)
	{
		mpSavedData->SetString("map", "");
		
		if(!mpServerPage->IsRandomMapSelected())
			mpSavedData->SetString("map", sMapName);
		
		// Save config to a file
		mpSavedData->SaveToFile(gpFullFileSystem, "ServerConfig.vdf", "GAME");
	};
	
	// Create the command to execute
	char sMapCmd[1024]{};
	Q_snprintf(sMapCmd, sizeof(sMapCmd), "disconnect\nwait\nwait\nsv_lan 1\nsetmaster enable\nmaxplayers %d\nsv_password \"%s\"\nhostname \"%s\"\nprogress_enable\nmap %s\n", mpGameplayPage->GetMaxPlayers(), sPassword, sHostName, sMapName);
	
	// And exec it
	gpEngine->ClientCmd_Unrestricted(sMapCmd);
	return true;
};