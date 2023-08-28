/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2021, 2023 BlackPhrase
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
/// @brief base animated entity

#pragma once

#include "BaseDelay.hpp"

class CBaseAnimated : public CBaseDelay
{
public:
	virtual int Save(CGameSave &aGameSave);
	virtual int Restore(const CGameSave &aGameSave);
	
	// Basic character animation functionality
	
	void InitBoneControllers();
	
	/// Advance the animation frame up to the current time
	/// Accumulate animation frame time from last time called until now
	/// If an afInterval is passed in, only advance animation that number of seconds
	float StudioFrameAdvance(float afInterval = 0.0f);
	
	void ResetSequenceInfo();
	
	/// Handle events that have happend since last time called up until X seconds into the future
	void DispatchAnimEvents(float afFutureInterval = 0.1f);
	
	virtual void HandleAnimEvent(MonsterEvent_t *apEvent){}
	
	int GetSequenceFlags() const;
	
	/// If abHeaviest - get activity with highest 'weight'
	int LookupActivity(int anActivity, bool abHeaviest = false) const;
	
	int LookupSequence(const char *asLabel) const;
	
	float SetBoneController(int anController, float afValue);
	
	float SetBlending(int anBlender, float afValue);
	
	void GetBonePosition(int anBone, idVec3 &avOrigin, idVec3 &avAngles) const;
	
	void GetAutoMovement(idVec3 &avOrigin, idVec3 &avAngles, float afInterval = 0.1f);
	
	int FindTransition(int anEndingSequence, int anGoalSequence, int *apDir);
	
	void GetAttachment(int anAttachment, idVec3 &avOrigin, idVec3 &avAngles) const;
	
	void SetBodyGroup(int anGroup, int anValue);
	int GetBodyGroup(int anGroup) const;
	
	int ExtractBoundingBox(int anSequence, float *avMins, float *avMaxs);
	
	void SetSequenceBox();
public: // TODO: private:
	static TYPEDESCRIPTION mSaveData[];
	
	float mfFrameRate{0.0f}; ///< Computed FPS for current sequence
	float mfGroundSpeed{0.0f}; ///< Computed linear movement rate for current sequence
	
	float mfLastEventCheck{0.0f}; ///< Last time the event list was checked
	
	bool mbSequenceFinished{false}; ///< Flag set when StudioAdvanceFrame moves across a frame boundary
	bool mbSequenceLoops{false}; ///< True if the sequence loops
private:
	void *GetModel() const;
};