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

#include "ModInfo.hpp"
#include "KeyValues.h"

CModInfo::CModInfo()
{
	mpModData = new KeyValues("ModData");
	msGameTitle[0] = 0;
};

CModInfo::~CModInfo()
{
	FreeModInfo();
};

void CModInfo::FreeModInfo()
{
	if(mpModData)
	{
		mpModData->deleteThis();
		mpModData = nullptr;
	};
};

void CModInfo::LoadCurrentGameInfo()
{
	// Load up gameinfo for the current mod
	const char *sFileName{"liblist.gam"}; // TODO: gameinfo.txt?
	mpModData->LoadFromFile(gpFullFileSystem, sFileName);
};

void CModInfo::LoadGameInfoFromBuffer(const char *asBuffer)
{
	// Load up gameinfo for the current mod
	mpModData->LoadFromBuffer("", asBuffer);
};

//const wchar_t *CModInfo::GetGameTitle() const
//{
	//if(!msGameTitle[0])
	//{
	//};
	
	//return msGameTitle;
//};

//const wchar_t *CModInfo::GetGameTitle2() const
//{
	//if(!msGameTitle2[0])
	//{
	//};
	
	//return msGameTitle2;
//};

const char *CModInfo::GetGameName() const
{
	return mpModData->GetString("game", "");
};

bool CModInfo::IsMultiplayerOnly() const
{
	return (stricmp(mpModData->GetString("type", ""), "multiplayer_only") == 0);
};

bool CModInfo::IsSinglePlayerOnly() const
{
	return (stricmp(mpModData->GetString("type", ""), "singleplayer_only") == 0);
};

//bool CModInfo::HasPortals() const
//{
	//return (stricmp(mpModData->GetString("hasportals", "0"), "1") == 0);
//};

//bool CModInfo::NoDifficulty() const
//{
	//return (stricmp(mpModData->GetString("nodifficulty", "0"), "1") == 0);
//};

bool CModInfo::NoModels() const
{
	return (stricmp(mpModData->GetString("nomodels", "0"), "1") == 0);
};

bool CModInfo::NoHiModels() const
{
	return (stricmp(mpModData->GetString("nohimodels", "0"), "1") == 0);
};

//bool CModInfo::NoCrosshair() const
//{
	//return (stricmp(mpModData->GetString("nocrosshair", "1"), "1") == 0);
//};

//bool CModInfo::AdvCrosshair() const
//{
	//return (stricmp(mpModData->GetString("advcrosshair", "0"), "1") == 0);
//};

//const char *CModInfo::GetFallbackDir() const
//{
	//return mpModData->GetString("fallback_dir", "");
//};

//bool CModInfo::UseGameLogo() const
//{
	//return (stricmp(mpModData->GetString("gamelogo", "0"), "1") == 0);
//};

//KeyValues *CModInfo::GetHiddenMaps() const
//{
	//return mpModData->FindKey("hidden_maps");
//};

const char *CModInfo::GetStartMap() const
{
	return mpModData->GetString("startmap", "");
};

const char *CModInfo::GetTrainMap() const
{
	return mpModData->GetString("trainmap", "");
};

const char *CModInfo::GetMPEntity() const
{
	return mpModData->GetString("mpentity", "");
};

const char *CModInfo::GetGameDLL() const
{
	return mpModData->GetString("gamedll", "");
};

bool CModInfo::IsSecure() const
{
	return mpModData->GetString("secure", "0") == 0;
};

CModInfo &ModInfo()
{
	static CModInfo ModInfo;
	return ModInfo;
};