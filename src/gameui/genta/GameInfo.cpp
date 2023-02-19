/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2022-2023 BlackPhrase
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

#include "GameInfo.hpp"
#include "KeyValues.h"

CGameInfo::CGameInfo()
{
	mpModData = new KeyValues("ModData");
	msGameTitle[0] = 0;
};

CGameInfo::~CGameInfo()
{
	FreeModInfo();
};

void CGameInfo::FreeModInfo()
{
	if(mpModData)
	{
		mpModData->deleteThis();
		mpModData = nullptr;
	};
};

void CGameInfo::LoadCurrentGameInfo()
{
	// Load up gameinfo for the current mod
	const char *sFileName{"liblist.gam"}; // TODO: gameinfo.txt?
	mpModData->LoadFromFile(gpFullFileSystem, sFileName);
};

void CGameInfo::LoadGameInfoFromBuffer(const char *asBuffer)
{
	// Load up gameinfo for the current mod
	mpModData->LoadFromBuffer("", asBuffer);
};

//const wchar_t *CGameInfo::GetGameTitle() const
//{
	//if(!msGameTitle[0])
	//{
	//};
	
	//return msGameTitle;
//};

//const wchar_t *CGameInfo::GetGameTitle2() const
//{
	//if(!msGameTitle2[0])
	//{
	//};
	
	//return msGameTitle2;
//};

const char *CGameInfo::GetGameName() const
{
	return mpModData->GetString("game", "");
};

bool CGameInfo::IsMultiplayerOnly() const
{
	return (stricmp(mpModData->GetString("type", ""), "multiplayer_only") == 0);
};

bool CGameInfo::IsSinglePlayerOnly() const
{
	return (stricmp(mpModData->GetString("type", ""), "singleplayer_only") == 0);
};

//bool CGameInfo::HasPortals() const
//{
	//return (stricmp(mpModData->GetString("hasportals", "0"), "1") == 0);
//};

//bool CGameInfo::NoDifficulty() const
//{
	//return (stricmp(mpModData->GetString("nodifficulty", "0"), "1") == 0);
//};

bool CGameInfo::NoModels() const
{
	return (stricmp(mpModData->GetString("nomodels", "0"), "1") == 0);
};

bool CGameInfo::NoHiModels() const
{
	return (stricmp(mpModData->GetString("nohimodels", "0"), "1") == 0);
};

//bool CGameInfo::NoCrosshair() const
//{
	//return (stricmp(mpModData->GetString("nocrosshair", "1"), "1") == 0);
//};

//bool CGameInfo::AdvCrosshair() const
//{
	//return (stricmp(mpModData->GetString("advcrosshair", "0"), "1") == 0);
//};

//const char *CGameInfo::GetFallbackDir() const
//{
	//return mpModData->GetString("fallback_dir", "");
//};

//bool CGameInfo::UseGameLogo() const
//{
	//return (stricmp(mpModData->GetString("gamelogo", "0"), "1") == 0);
//};

//KeyValues *CGameInfo::GetHiddenMaps() const
//{
	//return mpModData->FindKey("hidden_maps");
//};

const char *CGameInfo::GetStartMap() const
{
	return mpModData->GetString("startmap", "");
};

const char *CGameInfo::GetTrainMap() const
{
	return mpModData->GetString("trainmap", "");
};

const char *CGameInfo::GetMPEntity() const
{
	return mpModData->GetString("mpentity", "");
};

const char *CGameInfo::GetGameDLL() const
{
	return mpModData->GetString("gamedll", "");
};

bool CGameInfo::IsSecure() const
{
	return (stricmp(mpModData->GetString("secure", "0"), "1") == 0);
};

CGameInfo &GameInfo()
{
	static CGameInfo GameInfo;
	return GameInfo;
};