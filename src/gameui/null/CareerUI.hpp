/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2017, 2021 BlackPhrase
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

#include "ICareerUI.h"

class CCareerUI : public ICareerUI
{
public:
	bool IsPlayingMatch();

	ITaskVec *GetCurrentTaskVec();

	bool PlayAsCT();

	int GetReputationGained();
	int GetNumMapsUnlocked();

	bool DoesWinUnlockAll();

	int GetRoundTimeLength();
	int GetWinfastLength();

	CareerDifficultyType GetDifficulty();

	int GetCurrentMapTriplet(MapInfo *maps);

	void OnRoundEndMenuOpen(bool didWin);
	void OnMatchEndMenuOpen(bool didWin);

	void OnRoundEndMenuClose(bool stillPlaying);
	void OnMatchEndMenuClose(bool stillPlaying);
};