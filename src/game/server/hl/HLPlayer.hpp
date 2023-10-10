/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2020, 2023 BlackPhrase
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

#include "BasePlayer.hpp"

class CHEVSuit;

class CHLPlayer : public CBasePlayer
{
public:
	void Spawn() override;
	
	void UpdateGeigerCounter();
	void CheckSuitUpdate();
	
	/// Add sentence to suit playlist queue. If abGroup is true, then name is a sentence
	// group (HEV_AA), otherwise name is a specific sentence name, i.e.: !HEV_AA0
	// If anNoRepeatTime is specified in seconds, we don't repeat playback of this word
	// or sentence for at least that number of seconds
	void SetSuitUpdate(const char *asName, bool abGroup, int anNoRepeatTime);
	
	void BarnacleVictimBitten(CBaseEntity *apBarnacle);
	void BarnacleVictimReleased();
	
	CHEVSuit *GetSuit() const {return mpSuit;}
	bool HasSuit() const {return mpSuit != nullptr;}
private:
	CHEVSuit *mpSuit{nullptr};
};