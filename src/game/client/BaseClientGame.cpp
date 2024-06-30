/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2018-2024 BlackPhrase
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

#include <next/engine/ISystem.hpp>
#include <next/filesystem/IFileSystem.hpp>
#include <next/inputsystem/IInputSystem.hpp>
#include <next/physics/IPhysicsSystem.hpp>
#include <next/scriptsystem/IScriptSystem.hpp>

#include "BaseClientGame.hpp"

#include "HUD.hpp"

void InitInput();

bool CBaseClientGame::Connect(CreateInterfaceFn afnEngineFactory)
{
	if(!afnEngineFactory)
		return false;
	
	mpSystem = reinterpret_cast<ISystem*>(afnEngineFactory(OGS_SYSTEM_INTERFACE_VERSION, nullptr));
	
	if(!mpSystem)
		return false;
	
	mpFileSystem = reinterpret_cast<IFileSystem*>(afnEngineFactory(OGS_FILESYSTEM_INTERFACE_VERSION, nullptr));
	
	if(!mpFileSystem)
		return false;
	
	mpInputSystem = reinterpret_cast<IInputSystem*>(afnEngineFactory(OGS_INPUTSYSTEM_INTERFACE_VERSION, nullptr));
	
	if(!mpInputSystem)
		return false;
	
	mpPhysicsSystem = reinterpret_cast<IPhysicsSystem*>(afnEngineFactory(OGS_PHYSICSSYSTEM_INTERFACE_VERSION, nullptr));
	
	if(!mpPhysicsSystem)
		return false;
	
	mpScriptSystem = reinterpret_cast<IScriptSystem*>(afnEngineFactory(OGS_SCRIPTSYSTEM_INTERFACE_VERSION, nullptr));
	
	if(!mpScriptSystem)
		return false;
	
	bool bUseVGUI3{false};
	
	if(bUseVGUI3)
		mpVGui = reinterpret_cast<vgui3::IVGui*>afnEngineFactory(VGUI3_IVGUI_INTERFACE_VERSION, nullptr);
	else
		mpVGui = reinterpret_cast<vgui2::IVGui*>afnEngineFactory(VGUI_IVGUI_INTERFACE_VERSION, nullptr);
	
	if(!mpVGui)
		return false;
	
	return true;
};

void CBaseClientGame::Disconnect()
{
	mpVGui = nullptr;
	mpScriptSystem = nullptr;
	mpPhysicsSystem = nullptr;
	mpInputSystem = nullptr;
	mpFileSystem = nullptr;
	mpSystem = nullptr;
};

bool CBaseClientGame::Init()
{
	RegisterConVars();
	
	// TODO
	if(!VGui_Startup(afnEngineFactory))
		return false;
	
	mpHUD = new CHUD(); // TODO
	
	mpHUD->Init();
	
	InitInput(); // TODO: wrong place?
	
	HookEvents();
	
	// TODO
	return true;
};

void CBaseClientGame::Shutdown()
{
	ShutdownInput();
	
	//mpHUD->Shutdown();
	VGui_Shutdown();
	
	UnregisterConVars();
};

void CBaseClientGame::Update(float afTimeStep)
{
	ServersThink(time);
	
	GetClientVoiceMgr()->Frame(time);
};

void CBaseClientGame::RegisterConVars()
{
	// TODO
};

void CBaseClientGame::UnregisterConVars()
{
	// TODO
};