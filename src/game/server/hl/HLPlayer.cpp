/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2020, 2022-2023 BlackPhrase
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

#include "HLPlayer.hpp"
#include "HEVSuit.hpp"

LINK_ENTITY_TO_CLASS(player, CHLPlayer);

void CHLPlayer::UpdateGeigerCounter()
{
};

void CHLPlayer::CheckSuitUpdate()
{
	UpdateGeigerCounter();
};

void CHLPlayer::SetSuitUpdate(const char *asName, bool abGroup, int anNoRepeatTime)
{
	// Ignore suit updates if no suit
	if(!(self->weapons & (1 << WEAPON_SUIT)))
		return;
	
	// Due to static channel design (and laziness) we don't play HEV sounds in multiplayer right now
	if(mpGame->GetRules()->IsMultiplayer())
		return;
	
	// If asName == nullptr, clear out the queue
	if(!asName)
	{
		for(int i = 0; i < CSUITPLAYLIST; ++i)
			m_rgSuitPlayList[i] = 0;
		return;
	};
	
	// Mark group number as negative
	int nSentence{-SENTENCEG_GetIndex(asName)};
	
	// Get sentence or group number
	if(!abGroup)
	{
		nSentence = SENTENCEG_Lookup(asName, nullptr);
		if(nSentence < 0)
			return;
	};
	
	// Check the "norepeat" list - this list lets us cancel the playback of words or
	// sentences that have already been played within a certain amount of time
	
	int nEmpty{-1};
	
	for(int i = 0; i < CSUITNOREPEAT; ++i)
	{
		if(nSentence == m_rgiSuitNoRepeat[i])
		{
			// This sentence or group is already in the "norepeat" list
			
			if(m_rgflSuitNoRepeatTime[i] < gpGlobals->time)
			{
				// Norepeat time has expired, clear it out
				m_rgSuitPlayList[i] = 0;
				m_rgflSuitNoRepeatTime[i] = 0.0f;
				nEmpty = i;
				break;
			}
			else
				return; // Don't play, still marked as norepeat
		};
		
		// Keep track of empty slot
		if(!m_rgiSuitNoRepeat[i])
			nEmpty = i;
	};
	
	// Sentence if in "norepeat" list, save if norepeat time was given
	
	if(anNoRepeatTime)
	{
		if(nEmpty < 0)
			nEmpty = gpEngine->pfnRandomLong(0, CSUITNOREPEAT - 1); // Pick random slot to take over
		
		m_rgiSuitNoRepeat[nEmpty] = nSentence;
		m_rgflSuitNoRepeatTime[nEmpty] = gpGlobals->time + anNoRepeatTime;
	};
	
	// Find empty spot in queue or overwrite the last spot
	m_rgSuitPlayList[mnSuitPlayNext] = nSentence;
	++mnSuitPlayNext;
	if(mnSuitPlayNext == CSUITPLAYLIST)
		mnSuitPlayNext = 0;
	
	if(mfSuitUpdate <= gpGlobals->time)
	{
		mfSuitUpdate = gpGlobals->time + SUITUPDATETIME:
		
		// Play queue is empty, don't delay too long before playback
		if(mfSuitUpdate == 0)
			mfSuitUpdate = gpGlobals->time + SUITFIRSTUPDATETIME;
	};
};

void CHLPlayer::BarnacleVictimBitten(CBaseEntity *apBarnacle)
{
	TakeDamage(apBarnacle, apBarnacle, GetHealth() + GetArmorValue(), DMG_SLASH | DMG_ALWAYSGIB);
};

void CHLPlayer::BarnacleVictimReleased()
{
};