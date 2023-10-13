/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2023 BlackPhrase
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

#pragma once

#include <tier1/interface.h>

#include <vgui2/VGUI.h>

namespace vgui // TODO: vgui2?
{
class Panel;
};

//extern "C"
//{

//void VGui_Startup(); // gs version
bool VGui_Startup(CreateInterfaceFn afnEngineFactory);
void VGui_Shutdown();

// Only safe to call from inside the subclass of Panel:paintBackground
//void VGui_ViewportPaintBackground(int extents[4];

//};

// Below: src only

void VGui_CreateGlobalPanels();

vgui::VPANEL VGui_GetClientDLLRootPanel();

void VGui_CreateClientDLLRootPanel();
void VGui_DestroyClientDLLRootPanel();

/// Things to do before rendering vgui stuff
void VGui_PreRender();

void VGui_PostRender();