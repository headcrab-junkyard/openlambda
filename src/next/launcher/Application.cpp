/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2018-2020, 2024 BlackPhrase
 *
 * OGSNext Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OGSNext Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OGSNext Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#include <stdexcept>

#include "Application.hpp"

#include <next/filesystem/IFileSystem.hpp>
#include <next/networksystem/INetworkSystem.hpp>
#include <next/physics/IPhysics.hpp>
#include <next/scriptsystem/IScriptSystem.hpp>
#include <next/engineclient/IEngineClient.hpp>
#include <next/inputsystem/IInputSystem.hpp>
//#include <next/soundsystem/ISoundSystem.hpp>

//#include <vgui3/IVGUI3.hpp>

CreateInterfaceFn gfnFSFactory{nullptr};
CreateInterfaceFn gfnNetworkSystemFactory{nullptr};
CreateInterfaceFn gfnPhysicsFactory{nullptr};
CreateInterfaceFn gfnScriptSystemFactory{nullptr};
CreateInterfaceFn gfnEngineClientFactory{nullptr};
CreateInterfaceFn gfnInputSystemFactory{nullptr};
CreateInterfaceFn gfnSoundSystemFactory{nullptr};

CreateInterfaceFn gfnVGUI3Factory{nullptr};

IBaseInterface *LauncherFactory(const char *name, int *retval)
{
	if(!strcmp(name, OGS_FILESYSTEM_INTERFACE_VERSION))
		return gfnFSFactory(name, retval);
	
	if(!strcmp(name, OGS_NETWORKSYSTEM_INTERFACE_VERSION))
		return gfnNetworkSystemFactory(name, retval);
	
	if(!strcmp(name, OGS_PHYSICS_INTERFACE_VERSION))
		return gfnPhysicsFactory(name, retval);
	
	if(!strcmp(name, OGS_SCRIPTSYSTEM_INTERFACE_VERSION))
		return gfnScriptSystemFactory(name, retval);
	
	if(!strcmp(name, OGS_ENGINECLIENT_INTERFACE_VERSION))
		return gfnEngineClientFactory(name, retval);
	
	if(!strcmp(name, OGS_INPUTSYSTEM_INTERFACE_VERSION))
		return gfnInputSystemFactory(name, retval);
	
	//if(!strcmp(name, OGS_SOUNDSYSTEM_INTERFACE_VERSION))
		//return gfnSoundSystemFactory(name, retval);
	
	//if(!strcmp(name, OGS_VGUI3_INTERFACE_VERSION))
		//return gfnVGUI3Factory(name, retval);
	
	auto fnThisFactory{Sys_GetFactoryThis()};
	return fnThisFactory(name, retval);
};

CApplication::CApplication() = default;
CApplication::~CApplication() = default;

int CApplication::Run()
{
	do
	{
		if(!Init())
			return EXIT_FAILURE;
		
		bool bRunning{true};
		
		// main loop
		while(bRunning)
			bRunning = mpEngine->Frame();
		
		mbRestart = false; // TODO
		
		Shutdown();
	}
	while(mbRestart);
	
	// return success of application
	return EXIT_SUCCESS;
};

bool CApplication::Init()
{
	// Try to load the file system module
	if(!LoadFileSystemModule("filesystem_stdio"))
		throw std::runtime_error(std::string("Failed to load the filesystem module!"));
	
	// Try to load the network system module
	if(!LoadNetworkSystemModule("networksystem"))
		throw std::runtime_error(std::string("Failed to load the network system module!"));
	
	// Try to load the physics module
	if(!LoadPhysicsModule("physics"))
		throw std::runtime_error(std::string("Failed to load the physics module!"));
	
	// Try to load the script system module
	if(!LoadScriptSystemModule("scriptsystem"))
		throw std::runtime_error(std::string("Failed to load the script system module!"));
	
	// Try to load the engine client module
	if(!LoadEngineClientModule("engineclient"))
		throw std::runtime_error(std::string("Failed to load the engine client module!"));
	
	// Try to load the input system module
	if(!LoadInputSystemModule("inputsystem"))
		throw std::runtime_error(std::string("Failed to load the input system module!"));
	
	// Try to load the sound system module
	// TODO: -nosound?
	//if(!LoadInputSystemModule("soundsystem"))
		//throw std::runtime_error(std::string("Failed to load the soundsystem system module!"));
	
	// Try to load the VGUI3 module
	//if(!LoadVGUI3Module("vgui3"))
		//throw std::runtime_error(std::string("Failed to load the vgui3 module!"));
	
	// Try to load the engine module
	LoadEngineModule("engine");
	
	mpEngine = reinterpret_cast<IEngine*>(pEngineFactory(OGS_ENGINE_INTERFACE_VERSION, nullptr));
	
	if(!mpEngine)
		return false;
	
	IEngine::InitParams InitParams{};
	
	InitParams.sGameDir = ".";
	InitParams.sCmdLine = "";
	//InitParams.sPostRestartCmdLine = ""; // TODO
	InitParams.fnLauncherFactory = LauncherFactory;
	InitParams.bDedicated = false;
	
	if(!mpEngine->Init(InitParams))
		return false;
	
	return PostInit();
};

void CApplication::Shutdown()
{
	mpEngine->Shutdown();
};

bool CApplication::LoadFileSystemModule(const char *asName)
{
	if(!asName || !*asName)
	{
		//throw std::invalid_argument(std::string("Argument passed to CApplication::LoadFileSystemModule is invalid! (").append(asName).append(")"));
		return false;
	};
	
	mpFSLib = Sys_LoadModule(asName);
	
	if(!mpFSLib)
		return false;
	
	gfnFSFactory = Sys_GetFactory(mpFSLib);
	
	if(!gfnFSFactory)
		return false;
	
	return true;
};

bool CApplication::LoadNetworkSystemModule(const char *asName)
{
	if(!asName || !*asName)
	{
		//throw std::invalid_argument(std::string("Argument passed to CApplication::LoadNetworkSystemModule is invalid! (").append(asName).append(")"));
		return false;
	};
	
	mpNetworkSystemLib = Sys_LoadModule(asName);
	
	if(!mpNetworkSystemLib)
		return false;
	
	gfnNetworkSystemFactory = Sys_GetFactory(mpNetworkSystemLib);
	
	if(!gfnNetworkSystemFactory)
		return false;
	
	return true;
};

bool CApplication::LoadPhysicsModule(const char *asName)
{
	if(!asName || !*asName)
	{
		//throw std::invalid_argument(std::string("Argument passed to CApplication::LoadPhysicsModule is invalid! (").append(asName).append(")"));
		return false;
	};
	
	mpPhysicsLib = Sys_LoadModule(asName);
	
	if(!mpPhysicsLib)
		return false;
	
	gfnPhysicsFactory = Sys_GetFactory(mpPhysicsLib);
	
	if(!gfnPhysicsFactory)
		return false;
	
	return true;
};

bool CApplication::LoadScriptSystemModule(const char *asName)
{
	if(!asName || !*asName)
	{
		//throw std::invalid_argument(std::string("Argument passed to CApplication::LoadScriptSystemModule is invalid! (").append(asName).append(")"));
		return false;
	};
	
	mpScriptSystemLib = Sys_LoadModule(asName);
	
	if(!mpScriptSystemLib)
		return false;
	
	gfnScriptSystemFactory = Sys_GetFactory(mpScriptSystemLib);
	
	if(!gfnScriptSystemFactory)
		return false;
	
	return true;
};

bool CApplication::LoadEngineClientModule(const char *asName)
{
	if(!asName || !*asName)
	{
		//throw std::invalid_argument(std::string("Argument passed to CApplication::LoadEngineClientModule is invalid! (").append(asName).append(")"));
		return false;
	};
	
	mpEngineClientLib = Sys_LoadModule(asName);
	
	if(!mpEngineClientLib)
		return false;
	
	gfnEngineClientFactory = Sys_GetFactory(mpEngineClientLib);
	
	if(!gfnEngineClientFactory)
		return false;
	
	return true;
};

bool CApplication::LoadInputSystemModule(const char *asName)
{
	if(!asName || !*asName)
	{
		//throw std::invalid_argument(std::string("Argument passed to CApplication::LoadInputSystemModule is invalid! (").append(asName).append(")"));
		return false;
	};
	
	mpInputSystemLib = Sys_LoadModule(asName);
	
	if(!mpInputSystemLib)
		return false;
	
	gfnInputSystemFactory = Sys_GetFactory(mpInputSystemLib);
	
	if(!gfnInputSystemFactory)
		return false;
	
	return true;
};

bool CApplication::LoadSoundSystemModule(const char *asName)
{
	if(!asName || !*asName)
	{
		//throw std::invalid_argument(std::string("Argument passed to CApplication::LoadSoundSystemModule is invalid! (").append(asName).append(")"));
		return false;
	};
	
	mpSoundSystemLib = Sys_LoadModule(asName);
	
	if(!mpSoundSystemLib)
		return false;
	
	gfnSoundSystemFactory = Sys_GetFactory(mpSoundSystemLib);
	
	if(!gfnSoundSystemFactory)
		return false;
	
	return true;
};

bool CApplication::LoadVGUI3Module(const char *asName)
{
	if(!asName || !*asName)
	{
		//throw std::invalid_argument(std::string("Argument passed to CApplication::LoadVGUI3Module is invalid! (").append(asName).append(")"));
		return false;
	};
	
	mpVGUI3Lib = Sys_LoadModule(asName);
	
	if(!mpVGUI3Lib)
		return false;
	
	gfnVGUI3Factory = Sys_GetFactory(mpVGUI3Lib);
	
	if(!gfnVGUI3Factory)
		return false;
	
	return true;
};

void CApplication::LoadEngineModule(const char *asName)
{
	mpEngineLib = Sys_LoadModule(asName);
	
	if(!mpEngineLib)
		throw std::runtime_error(std::string("Failed to load the engine module!"));
	
	auto pEngineFactory{Sys_GetFactory(mpEngineLib)};
	
	if(!pEngineFactory)
		throw std::runtime_error(std::string("Failed to get the factory from the engine module!"));
	
	mpEngineFactory = pEngineFactory;
};