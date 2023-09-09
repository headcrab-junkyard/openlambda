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

#include "BaseAnimated.hpp"

//=============================================================================

TYPEDESCRIPTION CBaseAnimated::mSaveData[]{};

int CBaseAnimated::Save(CGameSave &aGameSave)
{
	return 0;
};

int CBaseAnimated::Restore(const CGameSave &aGameSave)
{
	return 0;
};

void CBaseAnimated::InitBoneControllers()
{
	auto pModel{GetModel()};
	
	SetController(pModel, self, 0, 0.0);
	SetController(pModel, self, 1, 0.0);
	SetController(pModel, self, 2, 0.0);
	SetController(pModel, self, 3, 0.0);
};

float CBaseAnimated::StudioFrameAdvance(float afInterval)
{
	if(afInterval == 0.0f)
	{
		afInterval = gpGlobals->time - self->animtime;
		
		if(afInterval <= 0.001f)
		{
			self->animtime = gpGlobals->time;
			return 0.0f;
		};
	};
	
	if(!self->animtime)
		afInterval = 0.0f;
	
	self->frame += afInterval * mfFrameRate * self->framerate;
	self->animtime = gpGlobals->time;
	
	if(self->frame < 0.0f || self->frame >= 256.0f)
	{
		if(mbSequenceLoops)
			self->frame -= (int)(self->frame / 256.0f) * 256.0f;
		else
			self->frame = (self->frame < 0.0f) ? 0.0f : 255.0f;
		
		mbSequenceFinished = true; // Just in case it wasn't caught in GetEvents
	};
	
	return afInterval;
};

void CBaseAnimated::ResetSequenceInfo()
{
	GetSequenceInfo(GetModelPtr(), self, &mfFrameRate, &mfGroundSpeed);
	
	mbSequenceLoops = ((GetSequenceFlags() & STUDIO_LOOPING) != 0);
	
	self->SetAnimTime(gpGlobals->time);
	self->SetFrameRate(1.0f);
	
	mbSequenceFinished = false;
	mfLastEventCheck = gpGlobals->time;
};

void CBaseAnimated::DispatchAnimEvents(float afFutureInterval)
{
	auto pModel{GetModelPtr()};
	
	if(!pModel)
	{
		ALERT(at_aiconsole, "Gibbed monster is thinking!\n");
		return;
	};
	
	// FIXME: I have to do this or some events get missed, and this is probably causing the problem below
	afFutureInterval = 0.1f;
	
	// TODO: this still sometimes hits events twice
	float fStart{self->frame + (mfLastEventCheck - self->animtime) * mfFrameRate * self->framerate};
	float fEnd{self->frame + afFutureInterval * mfFrameRate * self->framerate};
	
	mfLastEventCheck = self->animtime + afFutureInterval;
	mbSequenceFinished = false;
	
	if(fEnd <= 0.0f || fEnd >= 256.0f)
		mbSequenceFinished = true;
	
	int nIndex{0};
	
	MonsterEvent_t Event{};
	
	while((nIndex = GetAnimationEvent(pModel, self, &Event, fStart, fEnd, nIndex)) != 0)
		HandleAnimEvent(&Event);
};

int CBaseAnimated::GetSequenceFlags() const
{
	return ::GetSequenceFlags(GetModelPtr(), self);
};

int CBaseAnimated::LookupActivity(int anActivity, bool abHeaviest) const
{
	if(!abHeaviest)
		ASSERT(anActivity != 0);
	
	return ::LookupActivity(GetModelPtr(), self, anActivity, abHeaviest);
};

int CBaseAnimated::LookupSequence(const char *asLabel) const
{
	return ::LookupSequence(GetModelPtr(), asLabel);
};

float CBaseAnimated::SetBoneController(int anController, float afValue)
{
	return SetController(GetModelPtr(), self, anController, afValue);
};

float CBaseAnimated::SetBlending(int anBlender, float afValue)
{
	return ::SetBlending(GetModelPtr(), self, anBlender, afValue);
};

void CBaseAnimated::GetBonePosition(int anBone, idVec3 &avOrigin, idVec3 &avAngles) const
{
	gpEngine->pfnGetBonePosition(ToEdict(), anBone,, avOrigin, avAngles);
};

void CBaseAnimated::GetAutoMovement(idVec3 &avOrigin, idVec3 &avAngles, float afInterval)
{
	// Nothing
};

int CBaseAnimated::FindTransition(int anEndingSequence, int anGoalSequence, int *apDir)
{
	auto pModel{GetModelPtr()};
	
	if(!apDir)
	{
		int nDir;
		int nSequence{::FindTransition(pModel, anEndingSequence, anGoalSequence, &nDir)};
		if(nDir != 1)
			return -1;
		else
			return nSequence;
	};
	
	return ::FindTransition(pModel, anEndingSequence, anGoalSequence, apDir);
};

void CBaseAnimated::GetAttachment(int anAttachment, idVec3 &avOrigin, idVec3 &avAngles) const
{
	gpEngine->pfnGetAttachment(ToEdict(), anAttachment, avOrigin, avAngles);
};

void CBaseAnimated::SetBodyGroup(int anGroup, int anValue)
{
	::SetBodyGroup(GetModelPtr(), self, anGroup, anValue);
};

int CBaseAnimated::GetBodyGroup(int anGroup) const
{
	return ::GetBodyGroup(GetModelPtr(), self, anGroup);
};

int CBaseAnimated::ExtractBoundingBox(int anSequence, float *avMins, float *avMaxs)
{
	return ::ExtractBoundingBox(GetModelPtr(), anSequence, avMins, avMaxs);
};

void CBaseAnimated::SetSequenceBox()
{
	idVec3 vMins, vMaxs;
	
	// Get the sequence bounding box
	if(ExtractBoundingBox(self->sequence, vMins, vMaxs))
	{
		// Expand box for rotation
		// Find min & max for rotations
		float fYaw{self->angles.y * (M_PI / 180.0f)};
		
		idVec3 vX, vY;
		
		vX.x = cos(fYaw);
		vX.y = sin(fYaw);
		
		vY.x = -sin(fYaw);
		vY.y = cos(fYaw);
		
		idVec3 vBounds[2];
		vBounds[0] = vMins;
		vBounds[1] = vMaxs;
		
		idVec3 vRMin(9999, 9999, 9999);
		idVec3 vRMax(-9999, -9999, -9999);
		
		idVec3 vBase, vTransformed;
		
		for(int i = 0; i <= 1; ++i)
		{
			vBase.x = vBounds[i].x;
			
			for(int j = 0; j <= 1; ++j)
			{
				vBase.y = vBounds[j].y;
				
				for(int k = 0; k <= 1; ++k)
				{
					vBase.z = vBounds[k].z;
					
					// Transform the point
					vTransformed.x = vX.x * vBase.x + vY.x * vBase.y;
					vTransformed.y = vX.y * vBase.x + vY.y * vBase.y;
					vTransformed.z = vBase.z;
					
					if(vTransformed.x < vRMin.x)
						vRMin.x = vTransformed.x;
					
					if(vTransformed.x > vRMax.x)
						vRMax.x = vTransformed.x;
					
					if(vTransformed.y < vRMin.y)
						vRMin.y = vTransformed.y;
					
					if(vTransformed.y > vRMax.y)
						vRMax.y = vTransformed.y;
					
					if(vTransformed.z < vRMin.z)
						vRMin.z = vTransformed.z;
					
					if(vTransformed.z > vRMax.z)
						vRMax.z = vTransformed.z;
				};
			};
		};
		
		vRMin.z = 0;
		vRMax.z = vRMin.z + 1;
		
		SetSize(vRMin, vRMax);
	};
};

void *CBaseAnimated::GetModelPtr() const
{
	return gpEngine->pfnGetModelPtr(ToEdict());
};