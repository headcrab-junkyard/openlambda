/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2022 BlackPhrase
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

#include "GameApplication.hpp"

bool CGameApplication::Init()
{
	// File system module name to load
	const char *sFSModuleName{Config::Defaults::FSModuleName};
	
	LoadFileSystemModule(sFSModuleName);
	
	auto pFileSystem = static_cast<IFileSystem*>(mfnFSFactory(FILESYSTEM_INTERFACE_VERSION, nullptr));
	
	if(!pFileSystem)
		return false;
	
	pFileSystem->Mount();
	
	pFileSystem->AddSearchPath(".", "ROOT"); // TODO: AddSearchPathNoWrite?
	
	// TODO: add a call to pFileSystem->Unmount?
	
	// Engine module name to load
	const char *sEngineModuleName{Config::Defaults::EngineModuleName};
	
	auto pRegistry{GetRegistry()};
	
	// Do we have an engine module name config var stored on the system?
	char *sPrevEngineModule{pRegistry->GetString("EngineDLL")};
	
	// Yes? Nice, then use it!
	if(sPrevEngineModule && *sPrevEngineModule)
		sEngineModuleName = sPrevEngineModule;
	
	// Any overrides?
	if(strstr(msCmdLine, "-hw"))
		// Skip checking for sw
		sEngineModuleName = "hw";
	else
		if(strstr(msCmdLine, "-sw"))
			// Skip checking for hw
			sEngineModuleName = "sw";

	// Try to load it
	LoadEngineModule(sEngineModuleName);
	
	// Engine module was successfully loaded, so store its name
	pRegistry->SetString("EngineDLL", sEngineModuleName);
	
	mpEngine = static_cast<IEngineAPI*>(mfnEngineFactory(VENGINE_LAUNCHER_API_VERSION, nullptr));
	
	if(!mpEngine)
		throw std::runtime_error(std::string("Failed to get the engine launcher interface (") + sEngineModuleName + ")!");
	
	return PostInit();
};

void CGameApplication::LoadFileSystemModule(const char *asName)
{
	if(!asName || !*asName)
	{
		//throw std::invalid_argument(std::string("Argument passed to CApplication::LoadFileSystemModule is invalid! (").append(asName).append(")"));
		return;
	};
	
	auto pFSLib{Sys_LoadModule(asName)};
	
	if(!pFSLib)
		throw std::runtime_error(std::string("Failed to load the filesystem module (") + asName + ")!");
	
	mpFSLib = pFSLib;
	
	auto fnFSFactory{Sys_GetFactory(mpFSLib)};
	
	if(!fnFSFactory)
		throw std::runtime_error(std::string("Failed to get the filesystem module factory (") + asName + ")!");
	
	mfnFSFactory = fnFSFactory;
};

void CGameApplication::LoadEngineModule(const char *asName)
{
	if(!asName || !*asName)
	{
		//throw std::invalid_argument(std::string("Argument passed to CApplication::LoadEngineModule is invalid! (").append(asName).append(")"));
		return;
	};
	
	auto pEngineLib{Sys_LoadModule(asName)};
	
	// Rekt
	if(!pEngineLib)
		throw std::runtime_error(std::string("Failed to load the engine module (") + asName + ")!");
	
	mpEngineLib = pEngineLib;
	
	auto fnEngineFactory{Sys_GetFactory(mpEngineLib)};
	
	if(!fnEngineFactory)
		throw std::runtime_error(std::string("Failed to get the engine module factory (") + asName + ")!");
	
	mfnEngineFactory = fnEngineFactory;
};