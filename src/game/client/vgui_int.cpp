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

#include "vgui_int.h"

void MP3Player_Create(vgui::VPANEL aParent);
void MP3Player_Destroy();

static CHudTextureHandleProperty gTextureHandleConverter;

static void VGui_OneTimeInit()
{
	static bool bInitialized{false};
	
	if(bInitialized)
		return;
	
	vgui::Panel::AddPropertyConverter("CHudTextureHandle", &gTextureHandleConverter);
	
	bInitialized = true;
};

//void VGui_Startup(); // gs version
bool VGui_Startup(CreateInterfaceFn afnEngineFactory)
{
	if(!vgui::VGui_InitInterfacesList("CLIENT", &afnEngineFactory, 1))
		return false;
	
	//if(!vgui::VGui_InitMatSysInterfacesList("CLIENT", &afnEngineFactory, 1))
		//return false;
	
	gpInputInternal = reinterpret_cast<IInputInternal*>(afnEngineFactory(VGUI_INPUTINTERNAL_INTERFACE_VERSION, nullptr));
	
	if(!gpInputInternal)
		return false; // c_vguiscreen needs this!
	
	VGui_OneTimeInit();
	
	// Create any root panels for the module
	VGUI_CreateClientDLLRootPanel();
	
	// Make sure we have a panel
	VPANEL root{VGui_GetClientDLLRootPanel()};
	if(!root)
		return false;
	
	//gpVGUILocalize->AddFile();
	//gpVGUILocalize->AddFile();
	gpVGUILocalize->AddFile("resource/sdktemplate_%language%.txt", "MOD");
	// TODO: PostInit() ?
	return true;
};

void VGui_Shutdown()
{
	VGUI_DestroyClientDLLRootPanel();
	
#ifndef _X360
	MP3Player_Destroy();
#endif
	
	netgraphpanel->Destroy();
	debugoverlaypanel->Destroy();

#ifdef TRACK_BLOCKING_IO
	iopanel->Destroy();
#endif
	
	fps->Destroy();
	messagechars->Destroy();
	loadingdisc->Destroy();
	internalCenterPrint->Destroy();
	
	if(gpClientMode)
		gpClientMode->VGui_Shutdown();
	
	// Make sure anything "marked for deletion" actually gets deleted before this module goes away
	vgui::ivgui()->RunFrame();
};

//void VGui_ViewportPaintBackground(int extents[4];

void VGui_CreateGlobalPanels()
{
	VPANEL gameToolParent = gpEngineVGUI->GetPanel(PANEL_CLIENTDLL_TOOLS);
	VPANEL toolParent = gpEngineVGUI->GetPanel(PANEL_TOOLS);
	
#ifdef TRACK_BLOCKING_IO
	VPANEL gameDLLPanel = gpEngineVGUI->GetPanel(PANEL_GAMEDLL);
#endif
	
	// Part of game
	internalCenterPrint->Create(gameToolParent);
	loadingdisc->Create(gameToolParent);
	messagechars->Create(gameToolParent);
	
	// Debugging or related tool
	fps->Create(toolParent);
	
#ifdef TRACK_BLOCKING_IO
	iopanel->Create(gameDLLPanel);
#endif
	
	netgraphpanel->Create(toolParent);
	debugoverlaypanel->Create(gameToolParent);
	
#ifndef _X360
	// Create mp3 player off of tool parent panel
	MP3Player_Create(toolParent);
#endif
};

/*
vgui::VPANEL VGui_GetClientDLLRootPanel()
{
};

void VGui_CreateClientDLLRootPanel()
{
};

void VGui_DestroyClientDLLRootPanel()
{
};
*/

void VGui_PreRender()
{
	//VPROF("VGui_PreRender");
	
	if(IsPC())
	{
		loadingdisc->SetLoadingVisible(gpEngine->IsDrawingLoadingImage() && !gpEngine->IsPlayingDemo());
		loadingdisc->SetPausedVisible(!gpEngineVGUI->IsGameUIVisible() && cl_showpausedimage.GetBool() && gpEngine->IsPaused() && !gpEngine->IsTakingScreenshot() && !gpEngine->IsPlayingDemo());
	};
};

void VGui_PostRender()
{
	// Nothing
};