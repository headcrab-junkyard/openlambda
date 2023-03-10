/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2016-2018, 2023 BlackPhrase
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

#include "SamplePlugin.hpp"

//#include <debug/Debug.hpp>
//#include <core/IEngineCore.hpp>
#include <engine/ISystem.hpp>

EXPOSE_SINGLE_INTERFACE(CSamplePlugin, IEnginePlugin, ENGINEPLUGINCALLBACKS_INTERFACE_VERSION);

bool CSamplePlugin::Load(CreateInterfaceFn afnModuleFactory)
{
	mpSystem = reinterpret_cast<ISystem*>(afnModuleFactory(OGS_SYSTEM_INTERFACE_VERSION, nullptr));
	
	if(!mpSystem)
		return false;
	
	mpSystem->DevMsg(eMsgType::Info, "Hello from sample plugin! (afnModuleFactory is %d)\n", afnModuleFactory);
	return true;
};

void CSamplePlugin::Unload()
{
	mpSystem->LogMsg(eMsgType::Info, "Goodbye from sample plugin!\n");
};

void CSamplePlugin::GetInfo(TPluginInfo &apInfo)
{
	apInfo.msName = "Sample Plugin";
	apInfo.msVersion = "1.0";
	apInfo.msAuthor = "Headcrab Garage";
	apInfo.msDesc = "Sample engine plugin";
};