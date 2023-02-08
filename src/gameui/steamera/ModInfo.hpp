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
/// @brief contains all the data entered about a mod in a liblist.gam/gameinfo.txt file

#pragma once

//#include <vgui/VGUI.h>

class KeyValues;

class CModInfo
{
public:
	CModInfo();
	~CModInfo();
	
	void FreeModInfo();
	
	// Loads mod info from liblist.gam/gameinfo.txt
	void LoadCurrentGameInfo();
	
	// Loads game info from null-terminated string
	void LoadGameInfoFromBuffer(const char *asBuffer);
	
	// Data accessors
	
	//const wchar_t *GetGameTitle() const;
	//const wchar_t *GetGameTitle2() const;
	const char *GetGameName() const;
	
	// TODO
	/*
	const char *GetURLInfo() const;
	const char *GetURLDl() const;
	
	const char *GetVersion() const;
	
	int GetSize() const;
	
	bool IsSvOnly() const;
	
	bool UseClDLL() const;
	
	const char *GetHLVersion() const;
	*/
	
	bool IsMultiplayerOnly() const;
	bool IsSinglePlayerOnly() const;
	
	//bool HasPortals() const;
	
	//bool NoDifficulty() const;
	bool NoModels() const;
	bool NoHiModels() const;
	//bool NoCrosshair() const;
	//bool AdvCrosshair() const;
	
	//const char *GetFallbackDir() const;
	
	//bool UseGameLogo() const;
	
	//KeyValues *GetHiddenMaps() const;
	
	const char *GetStartMap() const;
	const char *GetTrainMap() const;
	const char *GetMPEntity() const;
	
	const char *GetGameDLL() const;
	
	bool IsSecure() const;
	
	enum class GameType
	{
		MultiplayerOnly,
		SinglePlayerOnly
	};
	
	//GameType GetGameType() const;
private:
	//wchar_t msGameTitle[128]{};
	//wchar_t msGameTitle2[128]{};
	
	KeyValues *mpModData{nullptr};
};

// Singleton accessor
extern CModInfo &ModInfo();