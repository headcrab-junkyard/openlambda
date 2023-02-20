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

#include "MusicManager.hpp"

EXPOSE_SINGLE_INTERFACE(MusicManager, IMusicManager, MUSICMANAGER_INTERFACE_VERSION);

MusicManager::MusicManager() = default;
MusicManager::~MusicManager() = default;

void MusicManager::StartTrack(TrackType track, bool looping)
{
};

void MusicManager::StopTrack()
{
};

void MusicManager::FadeOutTrack()
{
};

bool MusicManager::IsPlayingTrack(TrackType track)
{
	return false;
};

bool MusicManager::IsPlayingTrack()
{
	return false;
};