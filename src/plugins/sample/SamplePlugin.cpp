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

CSamplePlugin gSamplePlugin;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CSamplePlugin, IEnginePlugin, ENGINEPLUGINCALLBACKS_INTERFACE_VERSION, gSamplePlugin);

bool CSamplePlugin::Load(CreateInterfaceFn afnModuleFactory)
{
	DevMsg(eMsgType::Info, "Hello from sample plugin! (afnModuleFactory is %d)\n", afnModuleFactory);
	return true;
};

void CSamplePlugin::Unload()
{
	LogMsg(eMsgType::Info, "Goodbye from sample plugin!\n");
};

void CSamplePlugin::GetInfo(TPluginInfo &apInfo)
{
	apInfo.sName = "Sample Plugin";
	apInfo.sVersion = "1.0";
	apInfo.sAuthor = "Headcrab Garage";
	apInfo.sDesc = "Sample engine plugin";
};