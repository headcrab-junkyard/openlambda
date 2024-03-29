/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2018, 2023-2024 BlackPhrase
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

#include "VGuiSystemModuleLoader.hpp"

//#include <cstdio>

//#include "sys_utils.h"

//#include <IVGuiModule.h>
//#include <ServerBrowser/IServerBrowser.h>

//#include <vgui/IPanel.h>
//#include <vgui/ISystem.h>
//#include <vgui/IVGui.h>
//#include <vgui/ILocalize.h>

//#include <KeyValues.h>

//#include <vgui_controls/Controls.h>
//#include <vgui_controls/Panel.h>

//#include <FileSystem.h>

#ifndef GAMEUI_EXPORTS
#	include <SteamUI/PlatformMainPanel.h>
#endif

//#include <tier0/dbg.h>

// NOTE: memdbgon must be the last include file in a source file!
//#include <tier0/memdbgon.h>

#ifdef GAMEUI_EXPORTS
extern vgui::VPANEL GetGameUIBasePanel();
#else
extern CPlatformMainPanel *gpMainPanel;
#endif

bool gbSteamCommunityFriendsVersion{false};

// Intance of the class
CVGuiSystemModuleLoader g_VModuleLoader;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CVGuiSystemModuleLoader, IVGuiModuleLoader, VGUIMODULELOADER_INTERFACE_VERSION, g_VModuleLoader);

CVGuiSystemModuleLoader::CVGuiSystemModuleLoader() = default;
CVGuiSystemModuleLoader::~CVGuiSystemModuleLoader() = default;

int CVGuiSystemModuleLoader::GetModuleCount()
{
	return mvModules.Count();
};

// Returns the string menu name (unlocalized) of a module
// anModuleID is of the range [0, GetModuleCount())
const char *CVGuiSystemModuleLoader::GetModuleLabel(int anModuleID)
{
	return mvModules[anModuleID].mpData->GetString("MenuName", "< unknown >");
};

CreateInterfaceFn CVGuiSystemModuleLoader::GetModuleFactory(int anModuleID)
{
	return Sys_GetFactory(mvModules[anModuleID].mpModule);
};

bool CVGuiSystemModuleLoader::ActivateModule(int anModuleID)
{
	if(!mvModules.IsValidIndex[anModuleID])
		return false;
	
	mvModules[anModuleID].mpModuleInterface->Activate();
	return true;
};

bool CVGuiSystemModuleLoader::ActivateModule(const char *asModuleName)
{
	for(int i = 0; i < GetModuleCount(); ++i)
	{
		if(!stricmp(GetModuleLabel(i), asModuleName) || !stricmp(mvModules[i].mpData->GetName(), asModuleName))
		{
			ActivateModule(i);
			return true;
		};
	};
	
	return false;
};

// Sets that the platform should update and restart when it quits
void CVGuiSystemModuleLoader::SetPlatformToRestart()
{
	mbPlatformShouldRestartAfterExit = true;
};

void CVGuiSystemModuleLoader::RunFrame()
{
};

// Loads and initializes all the modules specified in the platform file
bool CVGuiSystemModuleLoader::LoadPlatformModules(CreateInterfaceFn *apFactoryList, int anFactoryCount, bool abUseSteamModules)
{
	if(IsX360())
		return false;
	
	bool bResult{true};
	
	// Load platform menu
	auto pData{new KeyValues("Platform")};
	if(!pData->LoadFromFile(gpFullFileSystem, "steam/games/PlatformMenu.vdf", "PLATFORM"))
	{
		pData->deleteThis();
		return false;
	};
	
	// Walk the platform menu loading all the interfaces
	auto pMenuKeys{pData->FindKey("Menu", true)};
	for(auto It = pMenuKeys->GetFirstSubKey(); It != nullptr; It = It->GetNextKey())
	{
		// See if we should skip steam modules
		if(!abUseSteamModules && It->GetInt("SteamApp"))
			continue;
		
		auto sInterface{It->GetString("interface")};
		
		// Don't load friends if we are using Steam Community
		if(!Q_stricmp(sInterface, "VGuiModuleTracker001") && gbSteamCommunityFriendsVersion)
			continue;
		
		// Get copy out of Steam cache
		auto sDLLPath{It->GetString("dll")};
		
		// Load the module (LoadModule calls GetLocalCopy() under Steam)
		CSysModule *pModule{gpFullFileSystem->LoadModule(sDLLPath, "EXECUTABLE_PATH")};
		if(!pModule)
		{
			Error("Platform Error: bad module '%s', not loading\n", It->GetString("dll"));
			bResult = false;
			continue;
		};
		
		// Make sure we get the right version
		IVGuiModule *pModuleInterface{(IVGuiModule*)Sys_GetFactory(pModule)(sInterface, nullptr)};
		if(!pModuleInterface)
		{
			Warning("Platform Error: module version ('%s, %s) invalid, not loading\n", It->GetString("dll"), It->GetString("interface"));
			bResult = false;
			continue;
		};
		
		// Store off the module
		int nNewID{mvModules.AddToTail()};
		mvModules[nNewID].mpModule = pModule;
		mvModules[nNewID].mpModuleInterface = pModuleInterface;
		mvModules[nNewID].mpData = It;
	};
	
	mpPlatformModuleData = pData;
	return bResult && InitializeAllModules(apFactoryList, anFactoryCount);
};

// Deactivates all the modules (puts them into in inactive but recoverable state)
void CVGuiSystemModuleLoader::DeactivatePlatformModules()
{
	for(int i = 0; i < mvModules.Count(); ++i)
		mvModules[i].mpModuleInterface->Deactivate();
};

// Reenables all the deactivated platform modules
void CVGuiSystemModuleLoader::ReactivatePlatformModules()
{
	for(int i = 0; i < mvModules.Count(); ++i)
		mvModules[i].mpModuleInterface->Reactivate();
};

// Gives all platform modules a chance to shutdown gracefully
void CVGuiSystemModuleLoader::ShutdownPlatformModules()
{
	// Not valid for Xbox 360
	if(IsX360())
		return;
	
	// Static include guard to prevent recursive calls
	static bool bRunningFunction{false};
	
	if(bRunningFunction)
		return;
	
	bRunningFunction = true;
	
	// Deactivate all the modules first
	DeactivatePlatformModules();
	
	int i{0};
	
	// Give all the modules notice of quit
	for(i = 0; i < mvModules.Count(); ++i)
		vgui::ivgui()->PostMessage(mvModules[i].mpModuleInterface->GetPanel(), new KeyValues("Command", "command", "Quit"), nullptr);
	
	for(i = 0; i < mvModules.Count(); ++i)
		mvModules[i].mpModuleInterface->Shutdown();
	
	bRunningFunction = false;
};

// Disables and unloads platform
void CVGuiSystemModuleLoader::UnloadPlatformModules()
{
	for(int i = 0; i < mvModules.Count(); ++i)
		gpFullFileSystem->UnloadModule(mvModules[i].mpModule);
	
	mvModules.RemoveAll();
	
	if(mpPlatformModuleData)
	{
		mpPlatformModuleData->deleteThis();
		mpPlatformModuleData = nullptr;
	};
};

void CVGuiSystemModuleLoader::PostMessageToAllModules(KeyValues *apMessage)
{
	for(for i = 0; i < mvModules.Count(); ++i)
		vgui2::ivgui()->PostMessage(mvModules[i].mpModuleInterface->GetPanel(), apMessage->MakeCopy(), nullptr);
	
	apMessage->deleteThis();
};

bool CVGuiSystemModuleLoader::IsPlatformReady() const
{
	return mbModulesInitialized;
};

bool CVGuiSystemModuleLoader::ShouldPlatformRestart() const
{
	return mbPlatformShouldRestartAfterExit;
};

bool CVGuiSystemModuleLoader::IsModuleHidden(int anModuleID) const
{
	return mvModules[anModuleID].mpData->GetBool("Hidden", false);
};

bool CVGuiSystemModuleLoader::IsModuleVisible(int anModuleID) const
{
	return vgui::ipanel()->IsVisible(mvModules[anModuleID].mpModuleInterface->GetPanel());
};

bool CVGuiSystemModuleLoader::InitializeAllModules(CreateInterfaceFn *apFactoryList, int anFactoryCount)
{
	if(IsX360())
		return false;
	
	bool bResult{true};
	
	// Init vgui in the modules
	int i{0};
	for(i = 0; i < mvModules.Count(); ++i)
		if(!mvModules[i].mpModuleInterface->Initialize(apFactoryList, anFactoryCount))
		{
			bResult = false;
			Error("Platform Error: module failed to initialize\n");
		};
	
	// Create a table of all the loaded modules
	auto pModuleFactories{(CreateInterfaceFn*)_alloca(sizeof(CreateInterfaceFn) * mvModules.Count())};
	for(i = 0; i < mvModules.Count(); ++i)
		pModuleFactories[i] = Sys_GetFactory(mvModules[i].mpModule);
	
	// Give the modules a chance to link themselves together
	for(i = 0; i < mvModules.Count(); ++i)
	{
		if(!mvModules[i].mpModuleInterface->PostInitialize(pModuleFactories, mvModules.Count()))
		{
			bResult = false;
			Error("Platform Error: module failed to initialize\n");
		};
		
#ifdef GAMEUI_EXPORTS
		mvModules[i].mpModuleInterface->SetParent(GetGameUIBasePanel());
#else
		mvModules[i].mpModuleInterface->SetParent(gpMainPanel->GetVPanel());
#endif
	};
	
	mbModulesInitialized = true;
	return bResult;
};