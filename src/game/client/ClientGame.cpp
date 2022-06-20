/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2018-2022 BlackPhrase
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

#include "ClientGame.hpp"

#include <next/filesystem/IFileSystem.hpp>
//#include <next/inputsystem/IInputSystem.hpp>
//#include <next/physics/IPhysicsSystem.hpp>
//#include <next/scriptsystem/IScriptSystem.hpp>

void InitInput();

bool CClientGame::Init(CreateInterfaceFn afnEngineFactory)
{
	if(!afnEngineFactory)
		return false;
	
	//auto pVGui{(vgui2::IVGui*)afnEngineFactory(VGUI_IVGUI_INTERFACE_VERSION, nullptr)};
	//mpFileSystem = reinterpret_cast<IFileSystem*>(afnEngineFactory(OGS_FILESYSTEM_INTERFACE_VERSION, nullptr));
	//mpInputSystem = reinterpret_cast<IInputSystem*>(afnEngineFactory(OGS_INPUTSYSTEM_INTERFACE_VERSION, nullptr));
	//mpPhysicsSystem = reinterpret_cast<IPhysicsSystem*>(afnEngineFactory(OGS_PHYSICSSYSTEM_INTERFACE_VERSION, nullptr));
	//mpScriptSystem = reinterpret_cast<IScriptSystem*>(afnEngineFactory(OGS_SCRIPTSYSTEM_INTERFACE_VERSION, nullptr));
	
	//if(!pVGui)
		//return false;
	
	//if(!mpFileSystem)
		//return false;
	
	//if(!mpInputSystem)
		//return false;
	
	//if(!mpPhysicsSystem)
		//return false;
	
	//if(!mpScriptSystem)
		//return false;
	
	InitInput();
	
	// TODO
	return true;
};

void CClientGame::Shutdown()
{
	// TODO
};

void CClientGame::Update(/*double afTime*/)
{
	// TODO
};