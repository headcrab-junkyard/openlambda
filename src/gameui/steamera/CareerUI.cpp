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

#include "CareerUI.hpp"

EXPOSE_SINGLE_INTERFACE(CCareerUI, ICareerUI, CAREERUI_INTERFACE_VERSION);

CCareerUI::CCareerUI() = default;
CCareerUI::~CCareerUI() = default;

bool CCareerUI::IsPlayingMatch()
{
	// TODO
	return false;
};

ITaskVec *CCareerUI::GetCurrentTaskVec()
{
	// TODO
	return nullptr;
};

bool CCareerUI::PlayAsCT()
{
	// TODO
	return false;
};

int CCareerUI::GetReputationGained()
{
	// TODO
	return 0;
};

int CCareerUI::GetNumMapsUnlocked()
{
	// TODO
	return 0;
};

bool CCareerUI::DoesWinUnlockAll()
{
	// TODO
	return false;
};

int CCareerUI::GetRoundTimeLength()
{
	// TODO
	return 0;
};

int CCareerUI::GetWinfastLength()
{
	// TODO
	return 0;
};

CareerDifficultyType CCareerUI::GetDifficulty() const
{
	// TODO
	return CAREER_DIFFICULTY_NORMAL;
};

int CCareerUI::GetCurrentMapTriplet(MapInfo *apMaps)
{
	// TODO
	return 0;
};

void CCareerUI::OnRoundEndMenuOpen(bool abDidWin)
{
	// TODO
};

void CCareerUI::OnMatchEndMenuOpen(bool abDidWin)
{
	// TODO
};

void CCareerUI::OnRoundEndMenuClose(bool abStillPlaying)
{
	// TODO
};

void CCareerUI::OnMatchEndMenuClose(bool abStillPlaying)
{
	// TODO
};