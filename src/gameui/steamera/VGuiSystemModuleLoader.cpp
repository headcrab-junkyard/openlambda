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

#include "VGuiSystemModuleLoader.hpp"

//#include <tier0/memdbgon.h>

// Intance of the class
CVGuiSystemModuleLoader g_VModuleLoader;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CVGuiSystemModuleLoader, IVGuiModuleLoader, VGUIMODULELOADER_INTERFACE_VERSION, g_VModuleLoader);

CVGuiSystemModuleLoader::CVGuiSystemModuleLoader() = default;
CVGuiSystemModuleLoader::~CVGuiSystemModuleLoader() = default;

int CVGuiSystemModuleLoader::GetModuleCount()
{
	return mvModules.Count();
};

const char *CVGuiSystemModuleLoader::GetModuleLabel(int anModuleID)
{
	return "";
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

void CVGuiSystemModuleLoader::SetPlatformToRestart()
{
	mbPlatformShouldRestartAfterExit = true;
};

void CVGuiSystemModuleLoader::RunFrame()
{
};

bool CVGuiSystemModuleLoader::LoadPlatformModules(CreateInterfaceFn *apFactoryList, int anFactoryCount, bool abUseSteamModules)
{
	return true;
};

void CVGuiSystemModuleLoader::DeactivatePlatformModules()
{
	for(int i = 0; i < mvModules.Count(); ++i)
		mvModules[i].mpModuleInterface->Deactivate();
};

void CVGuiSystemModuleLoader::ReactivatePlatformModules()
{
	for(int i = 0; i < mvModules.Count(); ++i)
		mvModules[i].mpModuleInterface->Reactivate();
};

void CVGuiSystemModuleLoader::ShutdownPlatformModules()
{
};

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
	return true;
};