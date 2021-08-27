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

#include "CareerUI.hpp"

bool CCareerUI::IsPlayingMatch()
{
	return false;
};

ITaskVec *CCareerUI::GetCurrentTaskVec()
{
	return nullptr;
};

bool CCareerUI::PlayAsCT()
{
	return false;
};

int CCareerUI::GetReputationGained()
{
	return 0;
};

int CCareerUI::GetNumMapsUnlocked()
{
	return 0;
};

bool CCareerUI::DoesWinUnlockAll()
{
	return false;
};

int CCareerUI::GetRoundTimeLength()
{
	return 0;
};

int CCareerUI::GetWinfastLength()
{
	return 0;
};

CareerDifficultyType CCareerUI::GetDifficulty()
{
	return 0;
};

int CCareerUI::GetCurrentMapTriplet(MapInfo *maps)
{
	return 0;
};

void CCareerUI::OnRoundEndMenuOpen(bool didWin){};

void CCareerUI::OnMatchEndMenuOpen(bool didWin){};

void CCareerUI::OnRoundEndMenuClose(bool stillPlaying){};

void CCareerUI::OnMatchEndMenuClose(bool stillPlaying){};