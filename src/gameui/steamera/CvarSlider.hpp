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

#include <vgui2/controls/Slider.h>

class CCvarSlider : public vgui2::Slider
{
	DECLARE_CLASS_SIMPLE(CCvarSlider, vgui2::Slider);
public:
	CCvarSlider(vgui2::Panel *apParent, const char *asPanelName);
	CCvarSlider(vgui2::Panel *apParent, const char *asPanelName, const char *asCaption,
	float afMinValue, float afMaxValue, const char *asCvarName, bool abAllowOutOfRange = false);
	~CCvarSlider() = default;
	
	void SetupSlider(float afMinValue, float afMaxValue, const char *asName, bool abAllowOutOfRange);
	
	void Reset();
	
	/*virtual*/ void Paint();
	
	/*virtual*/ void ApplySettings(KeyValues *apData);
	
	/// Get control settings for editing
	/*virtual*/ void GetSettings(KeyValues *apData);
	
	void ApplyChanges();
	
	void SetCvarName(const char *asName);
	
	void SetMinMaxValues(float afMinValue, float afMaxValue, bool abSetTickDisplay = true);
	
	void SetTickColor(Color aColor); // TODO: const Color &aColor?
	
	void SetSliderValue(float afValue);
	float GetSliderValue() const;
	
	bool HasBeenModified() const;
private:
	MESSAGE_FUNC(OnSliderMoved, "SliderMoved");
	MESSAGE_FUNC(OnApplyChanges, "ApplyChanges");
private:
	char msCvarName[64]{};
	
	float mfStartValue{0.0f};
	
	float mfMinValue{0.0f};
	float mfMaxValue{0.0f};
	
	float mfCurrentValue{0.0f};
	
	int mnStartValue{0};
	int mnLastSliderValue{0};
	
	bool mbAllowOutOfRange{false};
	bool mbModifiedOnce{false};
	bool mbCreatedInCode{false};
};