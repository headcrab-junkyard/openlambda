/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2018-2023 BlackPhrase
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

#include <cstdlib>
#include "GameUI.hpp"

IEngineVGui *gpEngineVGui{nullptr};

ILocalize *gpVGuiLocalize{nullptr}:

static IGameClientExports *gpGameClientExports{nullptr};

// TODO: _GLOBALVAR?
EXPOSE_SINGLE_INTERFACE(CGameUI, IGameUI, GAMEUI_INTERFACE_VERSION);

CGameUI::CGameUI() = default;
CGameUI::~CGameUI() = default;

void CGameUI::Initialize(CreateInterfaceFn *factories, int count)
{
	// TODO
	
	// Load localization file
	gpVGuiLocalize->AddFile("resource/gameui_%language%.txt", "GAME", true);
	
	// Load mod info
	//ModInfo().LoadCurrentGameInfo();
	
	// Load localization file for kb_act.lst
	gpVGuiLocalize->AddFile("resource/valve_%language%.txt", "GAME", true);
	
	for(int i = 0; i < count; ++i)
	{
		mpVGUI = reinterpret_cast<vgui2::IVGUI*>(*factories[i](VGUI_INTERFACE_VERSION, nullptr));
		mpVGUISurface = reinterpret_cast<vgui2::ISurface*>(*factories[i](VGUI_SURFACE_INTERFACE_VERSION, nullptr));
		mpFileSystem = reinterpret_cast<IFileSystem*>(*factories[i](FILESYSTEM_INTERFACE_VERSION, nullptr));
		mpEngineVGui = reinterpret_cast<IEngineVGui*>(*factories[i](VENGINE_VGUI_VERSION, nullptr));
		mpGameUIFuncs = reinterpret_cast<IGameUIFuncs*>(*factories[i](GAMEUIFUNCS_INTERFACE_VERSION, nullptr));
		mpGameClientExports = reinterpret_cast<IGameClientExports*>(*factories[i](GAMECLIENTEXPORTS_INTERFACE_VERSION, nullptr));
	};
	
	bool bFailed{false};
	
	bFailed = !mpVGUISurface || !mpGameUIFuncs || !mpEngineVGui;
	
	if(bFailed)
		Sys_Error("CGameUI::Initialize() failed to get necessary interfaces\n");
	
	if(!mpGameClientExports)
		mpGameClientExports = new CDefaultGameClientExports();
	
	// Setup base panel
	UI_BASEMOD_PANEL_CLASS &FactoryBasePanel{ConstructUIBaseModPanelClass()}; // Explicit singleton instantiation
	
	FactoryBasePanel.SetBounds(0, 0, 640, 480);
	FactoryBasePanel.SetPaintBorderEnabled(false);
	FactoryBasePanel.SetPaintBackgroundEnabled(true);
	FactoryBasePanel.SetPaintEnabled(true);
	FactoryBasePanel.SetVisible(true);
	
	FactoryBasePanel.SetMouseInputEnabled(IsPC());
	FactoryBasePanel.SetKeyboardInputEnabled(true); // TODO: IsPC()?
	
	auto nRootPanel{mpEngineVGui->GetPanel(PANEL_GAMEUIDLL)};
	FactoryBasePanel.SetParent(nRootPanel);
};

void CGameUI::Start(/*cl_enginefunc_t*/ struct cl_enginefuncs_s *engineFuncs, int interfaceVersion, void /*IBaseSystem*/ *system)
{
	if(IsPC())
	{
	};
	
	// Localization
	gpVGuiLocalize->AddFile("resource/platform_%language%.txt");
	gpVGuiLocalize->AddFile("resource/vgui_%language%.txt");
};

void CGameUI::Shutdown()
{
	// Notify all the modules about the shutdown
	gVModuleLoader.ShutdownPlatformModules();
	
	// Unload the modules from memory
	gVModuleLoader.UnloadPlatformModules();
	
	ModInfo().FreeModInfo();
	
	// Release platform mutex
	// Close the mutex
	if(ghMutex)
		Sys_ReleaseMutex(ghMutex);
	
	if(ghWaitMutex)
		Sys_ReleaseMutex(ghWaitMutex);
	
	steamapicontext->Clear();
	
#ifndef _X360
	//SteamAPI_Shutdown(); // Controlled by engine
#endif

	ConVar_Unregister();
	DisconnectTier3Libraries();
	DisconnectTier2Libraries();
	DisconnectTier1Libraries();
};

int CGameUI::ActivateGameUI()
{
	mbActive = true;
	return true;
};

int CGameUI::ActivateDemoUI()
{
	return false;
};

int CGameUI::HasExclusiveInput()
{
	return false;
};

void CGameUI::RunFrame()
{
	// Play the startup music the first time we run frame
	if(mnPlayGameStartupSound > 0)
	{
		--mnPlayGameStartupSound;
		if(!mnPlayGameStartupSound)
			PlayGameStartupSound();
	};
};

void CGameUI::ConnectToServer(const char *game, int IP, int port)
{
	mnGameIP = IP;
	mnGamePort = port;
	
	SendConnectedToGameMessage();
};

void CGameUI::DisconnectFromServer()
{
	mnGameIP = 0;
	mnGamePort = 0;
	
	if(ghLoadingBackgroundDialog)
		vgui2::ivgui()->PostMessage(ghLoadingBackgroundDialog, new KeyValues("DisconnectedFromGame"), nullptr);
	
	gVModuleLoader.PostMessageToAllModules(new KeyValues("DisconnectedFromGame"));
};

void CGameUI::HideGameUI()
{
	mbActive = false;
};

bool CGameUI::IsGameUIActive()
{
	return mbActive;
};

void CGameUI::LoadingStarted(const char *resourceType, const char *resourceName)
{
	gVModuleLoader.PostMessageToAllModules(new KeyValues("LoadingStarted"));
	
	ShowLoadingBackgroundDialog();
	
	StartProgressBar();
	
	mnPlayGameStartupSound = 0;
};

void CGameUI::LoadingFinished(const char *resourceType, const char *resourceName)
{
	StopProgressBar(false, "", "");
	
	gVModuleLoader.PostMessageToAllModules(new KeyValues("LoadingFinished"));
	
	HideLoadingBackgroundDialog();
};

void CGameUI::StartProgressBar(const char *progressType, int progressSteps)
{
	ghLoadingDialog->Activate();
	
	// TODO
};

int CGameUI::ContinueProgressBar(int progressPoint, float progressFraction)
{
	if(!ghLoadingDialog.Get())
		return 0;
	
	ghLoadingDialog->Activate();
	return ghLoadingDialog->SetProgressPoint(progressPoint, progressFraction);
};

void CGameUI::StopProgressBar(bool bError, const char *failureReason, const char *extendedReason)
{
	if(!ghLoadingDialog.Get())
		return;
	
	if(bError)
	{
		ghLoadingDialog->DisplayGenericError(failureReason, extendedReason);
	}
	else
	{
		// Close the loading dialog
		ghLoadingDialog->Close();
		ghLoadingDialog = nullptr;
	};
};

int CGameUI::SetProgressBarStatusText(const char *statusText)
{
	if(!ghLoadingDialog.Get())
		return 0;
	
	if(!statusText)
		return 0;
	
	if(!stricmp(statusText, msPrevStatusText))
		return 0;
	
	ghLoadingDialog->SetStatusText(statusText);
	Q_strncpy(msPrevStatusText, statusText, sizeof(msPrevStatusText));
	return 1;
};

// NOTE: progress is in range [0...1]
void CGameUI::SetSecondaryProgressBar(float progress)
{
	if(ghLoadingDialog.Get())
		ghLoadingDialog->SetSecondaryProgress(progress);
};

void CGameUI::SetSecondaryProgressBarText(const char *statusText)
{
	if(ghLoadingDialog.Get())
		ghLoadingDialog->SetSecondaryProgressText(statusText);
};

void CGameUI::ValidateCDKey(bool force, bool inConnect)
{
	// UNUSED
};

// TODO: uint8 eSteamLoginFailure in Src SDK
void CGameUI::OnDisconnectFromServer(int eSteamLoginFailure, const char *username)
{
	if(ghLoadingBackgroundDialog)
		vgui::ivgui()->PostMessage(ghLoadingBackgroundDialog, new KeyValues("DisconnectedFromGame"), nullptr);
	
	if(!ghLoadingDialog)
		return;
	
	switch(eSteamLoginFailure)
	{
	case STEAMLOGINFAILURE_NOSTEAMLOGIN:
		ghLoadingDialog->DisplayNoSteamConnectionError();
		break;
	case STEAMLOGINFAILURE_VACBANNED:
		ghLoadingDialog->DisplayVACBannedError();
		break;
	case STEAMLOGINFAILURE_LOGGED_IN_ELSEWHERE:
		ghLoadingDialog->DisplayLoggedInElsewhereError();
		break;
	};
};

void CGameUI::PlayGameStartupSound()
{
	if(!ShouldUseGameStartupSound())
		return;
	
	//if(IsX360())
		//return;
	
	if(CommandLine()->FindParm("-nostartupsound"))
		return;
	
	char sPath[512]{};
	Q_snprintf(sPath, sizeof(sPath), "sound/ui/gamestartup*.mp3");
	Q_FixSlashes(sPath);
	
	FileFindHandle_t FileHandle{};
	CUtlVector<char*> vFileNames;
	
	const char *sFileName{gpFullFileSystem->FindFirstEx(sPath, "MOD", &FileHandle)};
	if(sFileName)
	{
		do
		{
			char sExt[10]{};
			Q_ExtractFileExtension(sFileName, sExt, sizeof(sExt));
			
			if(!Q_stricmp(sExt, "mp3"))
			{
				char sTemp[512]{};
				Q_snprintf(sTemp, sizeof(sTemp), "ui/%s", sFileName);
				
				char *sFound{new char[Q_strlen(sTemp) + 1]};
				Q_strncpy(sFound, sTemp, Q_strlen(sTemp) + 1);
				
				Q_FixSlashes(sFound);
				vFileNames.AddToTail(sFound);
			};
			
			sFileName = gpFullFileSystem->FindNext(FileHandle);
		}
		while(sFileName);
		
		gpFullFileSystem->FindClose(FileHandle);
	};
	
	// Did we find any?
	if(vFileNames.Count() > 0)
	{
		SYSTEMTIME SystemTime{};
		GetSystemTime(&SystemTime);
		
		int nIndex{SystemTime.wMilliseconds % vFileNames.Count()};
		
		if(vFileNames.IsValidIndex(nIndex) && vFileNames[nIndex])
		{
			char sFound[512]{};
			
			// Escape chars "*#" make it stream, and be affected by snd_musicvolume
			Q_snprintf(sFound, sizeof(sFound), "play *#%s", vFileNames[nIndex]);
			
			gpEngine->ClientCmd_Unrestricted(sFound);
		};
		
		vFileNames.PurgeAndDeleteElements();
	};
};

void CGameUI::SendConnectedToGameMessage()
{
	//MEM_ALLOC_CREDIT(); // TODO
	
	KeyValues *pKV{new KeyValues("ConnectedToGame")};
	pKV->SetInt("ip", mnGameIP);
	pKV->SetInt("connectionport", mnGamePort);
	pKV->SetInt("queryport", mnGamePort);
	
	gVModuleLoader.PostMessageToAllModules(pKV);
};