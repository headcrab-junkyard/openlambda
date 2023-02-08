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

#include "CvarSlider.hpp"

CCvarSlider::CCvarSlider(Panel *apParent, const char *asName) : BaseClass(apParent, asName)
{
	SetupSlider(0, 1, "", false);
	
	AddActionSignalTarget(this);
};

CCvarSlider::CCvarSlider(Panel *apParent, const char *asName, const char *asCaption, float afMinValue, float afMaxValue, const char *asCvarName, bool abAllowOutOfRange) : BaseClass(apParent, asName)
{
	SetupSlider(afMinValue, afMaxValue, asCvarName, abAllowOutOfRange);
	
	AddActionSignalTarget(this);
	
	mbCreatedInCode = true;
};

void CCvarSlider::SetupSlider(float afMinValue, float afMaxValue, const char *asName, bool abAllowOutOfRange)
{
	mfMinValue = afMinValue;
	mfMaxValue = afMaxValue;
	
	// Scale by scale factor
	SetRange();
	
	// TODO
	
	mbModifiedOnce = false;
	mbAllowOutOfRange = abAllowOutOfRange;
	
	// Set slider to current value
	Reset();
};

void CCvarSlider::Reset()
{
};

void CCvarSlider::Paint()
{
	// Get engine's current value
	ConVarRef var(msCvarName);
	if(!var.IsValid())
		return;
	
	auto fCurValue{var.GetFloat()};
	//float fCurValue{engine->pfnGetCvarFloat(msCvarName)};
	
	// Did it get changed from under us?
	if(fCurValue != mfStartValue)
	{
		mfStartValue = fCurValue;
		mfCurrentValue = fCurValue;
		
		auto nValue{(int)(CVARSLIDER_SCALE_FACTOR * fCurValue)};
		SetValue(nValue);
		
		mnStartValue = GetValue();
	};
	
	BaseClass::Paint();
};

void CCvarSlider::ApplySettings(KeyValues *apData)
{
	BaseClass::ApplySettings(apData);
	
	if(!mbCreatedInCode)
	{
	};
};

void CCvarSlider::GetSettings(KeyValues *apData)
{
	BaseClass::GetSettings(apData);
	
	if(!mbCreatedInCode)
	{
	};
};

void CCvarSlider::ApplyChanges()
{
	if(mbModifiedOnce)
	{
		mnStartValue = GetValue();
		mfStartValue = (float)mnStartValue / CVARSLIDER_SCALE_FACTOR;
		
		if(mbAllowOutOfRange)
			mfStartValue = mfCurrentValue;
		
		//gpEngine->Cvar_SetValue(msCvarName, mfStartValue);
		ConVarRef var(msCvarName);
		var.SetValue((float)mfStartValue);
	};
};

void CCvarSlider::SetCvarName(const char *asName)
{
	Q_strncpy(msCvarName, asName, sizeof(msCvarName));
	
	mbModifiedOnce = false;
	
	// Set slider to current value
	Reset();
};

void CCvarSlider::SetMinMaxValues(float afMinValue, float afMaxValue, bool abSetTickDisplay)
{
};

void CCvarSlider::SetTickColor(Color aColor)
{
	mTickColor = aColor;
};

void CCvarSlider::SetSliderValue(float afValue)
{
	auto nValue{(int)(CVARSLIDER_SCALE_FACTOR * afValue)};
	SetValue(nValue, false);
	
	// Remember this slider value
	mnLastSliderValue = GetValue();
	
	if(mfCurrentValue != afValue)
	{
		mfCurrentValue = afValue;
		mbModifiedOnce = true;
	};
};

float CCvarSlider::GetSliderValue() const
{
	if(mbAllowOutOfRange)
		return mfCurrentValue;
	
	return ((float)GetValue()) / CVARSLIDER_SCALE_FACTOR;
};

bool CCvarSlider::HasBeenModified() const
{
	if(GetValue() != mnStartValue)
		mbModifiedOnce = true;
	
	return mbModifiedOnce;
};

void CCvarSlider::OnSliderMoved()
{
	if(HasBeenModified())
	{
		if(mnLastSliderValue != GetValue())
		{
			mnLastSliderValue = GetValue();
			mfCurrentValue = ((float)mnLastSliderValue) / CVARSLIDER_SCALE_FACTOR;
		};
		
		// Tell parent that we've been modified
		PostActionSignal(new KeyValues("ControlModified"));
	};
};

void CCvarSlider::OnApplyChanges()
{
	if(!mbCreatedInCode)
		ApplyChanges();
};