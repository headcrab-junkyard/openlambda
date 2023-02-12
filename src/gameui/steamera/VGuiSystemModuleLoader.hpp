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
/// @brief Handles loading/unloading of different vgui modules into a shared context

#pragma once

#include <vgui/controls/PHandle.h>

#include <GameUI/IVGuiModuleLoader.h>

#include <tier1/UtlVector.h>

class KeyValues;
struct IVGuiModule;

class CVGuiSystemModuleLoader final : public IVGuiModuleLoader
{
public:
	CVGuiSystemModuleLoader();
	~CVGuiSystemModuleLoader();
public: // IVGuiModuleLoader interface implementation
	// Returns number of modules loaded
	int GetModuleCount() override;
	
	// Returns the string menu name (unlocalized) of a module
	// anModuleID is of the range [0, GetModuleCount())
	const char *GetModuleLabel(int anModuleID) override;
	
	// Returns a module interface factory
	CreateInterfaceFn GetModuleFactory(int anModuleID) override;
	
	// Brings the specified module to the foreground
	bool ActivateModule(int anModuleID) override;
	
	// Activates a module by name
	bool ActivateModule(const char *asModuleName) override;
	
	// Sets that the platform should update and restart when it quits
	void SetPlatformToRestart() override;
public: // The rest of the public methods
	// Needs to be called every frame - updates all the modules states
	void RunFrame();
	
	// Loads all the modules in the platform
	bool LoadPlatformModules(CreateInterfaceFn *apFactoryList, int anFactoryCount, bool abUseSteamModules);
	
	// Deactivates all the modules (puts them into inactive but recoverable state)
	void DeactivatePlatformModules();
	
	// Reenables all the deactivated platform modules
	void ReactivatePlatformModules();
	
	// Shuts down all the platform modules
	void ShutdownPlatformModules();
	
	// Unload all active platform modules/dlls from memory
	void UnloadPlatformModules();
	
	// Posts a message to all active modules
	void PostMessageToAllModules(KeyValues *apMessage);
	
	// Returns true if the module loader has (acquired the platform mutex and) loaded the modules
	bool IsPlatformReady() const;
	
	// Returns true if the platform should restart after exit
	bool ShouldPlatformRestart() const;
	
	bool IsModuleHidden(int anModuleID) const;
	bool IsModuleVisible(int anModuleID) const;
private:
	// Sets up all the modules for use
	bool InitializeAllModules(CreateInterfaceFn *apFactoryList, int anFactoryCount);
private:
	struct Module
	{
		CSysModule *mpModule{nullptr};
		IVGuiModule *mpModuleInterface{nullptr};
		KeyValues *mpData{nullptr};
	};
	
	CUtlVector<Module> mvModules;
	
	KeyValues *mpPlatformModuleData{nullptr};
	
	bool mbModulesInitialized{false};
	bool mbPlatformShouldRestartAfterExit{false};
};

extern CVGuiSystemModuleLoader g_VModuleLoader;