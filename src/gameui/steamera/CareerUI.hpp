/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2018, 2023 BlackPhrase
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

#include <GameUI/ICareerUI.h>

class CCareerUI final : public ICareerUI
{
public:
	CCareerUI();
	~CCareerUI();
	
	bool IsPlayingMatch() override;
	
	ITaskVec *GetCurrentTaskVec() override;
	
	bool PlayAsCT() override;
	
	int GetReputationGained() override;
	int GetNumMapsUnlocked() override;
	
	bool DoesWinUnlockAll() override;
	
	int GetRoundTimeLength() override;
	int GetWinfastLength() override;
	
	CareerDifficultyType GetDifficulty() const override;
	
	int GetCurrentMapTriplet(MapInfo *apMaps) override;
	
	void OnRoundEndMenuOpen(bool abDidWin) override;
	void OnMatchEndMenuOpen(bool abDidWin) override;
	
	void OnRoundEndMenuClose(bool abStillPlaying) override;
	void OnMatchEndMenuClose(bool abStillPlaying) override;
};