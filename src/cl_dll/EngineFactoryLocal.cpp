/*
 * This file is part of OpenLambda Project
 * Copyright (C) 2020-2021 BlackPhrase
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

#include "tier1/interface.h"

CreateInterfaceFn gfnEngineFactory{nullptr};
CreateInterfaceFn gfnFileSystemFactory{nullptr};
CreateInterfaceFn gfnPhysicsFactory{nullptr};
CreateInterfaceFn gfnScriptFactory{nullptr};
CreateInterfaceFn gfnVGUI3Factory{nullptr};

IBaseInterface *EngineFactoryLocal(const char *name, int *retval)
{
	IBaseInterface *pInterface{nullptr};
	
	// Engine module factory
	pInterface = gfnEngineFactory(name, retval);
	if(pInterface)
		return pInterface;
	
	// File system module factory
	pInterface = gfnFileSystemFactory(name, retval);
	if(pInterface)
		return pInterface;
	
	// Physics module factory
	pInterface = gfnPhysicsFactory(name, retval);
	if(pInterface)
		return pInterface;
	
	// Script module factory
	pInterface = gfnScriptFactory(name, retval);
	if(pInterface)
		return pInterface;
	
	// VGUI3 module factory
	pInterface = gfnVGUI3Factory(name, retval);
	if(pInterface)
		return pInterface;
	
	auto fnThisFactory{Sys_GetFactoryThis()};
	return fnThisFactory(name, retval);
};