/*
 * This file is part of OGS Engine
 * Copyright (C) 2018, 2022 BlackPhrase
 *
 * OGS Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OGS Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OGS Engine. If not, see <http://www.gnu.org/licenses/>.
 */

/// @file

#include "OptionsSubMouse.hpp"

#include "CvarNegateCheckButton.hpp"
#include "CvarToggleCheckButton.hpp"
#include "CvarSlider.hpp"

using namespace vgui;

COptionsSubMouse::COptionsSubMouse(Panel *apParent) : PropertyPage(apParent, nullptr)
{
	mpReverseMouseCheckBox = new CCvarNegateCheckButton(
		this,
		"ReverseMouse",
		"#GameUI_ReverseMouse",
		"m_pitch"
	);
	
	mpMouseFilterCheckBox = new CCvarToggleCheckButton(
		this,
		"MouseFilter",
		"#GameUI_MouseFilter",
		"m_filter"
	);
	
	mpJoystickCheckBox = new CCvarToggleCheckButton(
		this,
		"Joystick",
		"#GameUI_Joystick",
		"joystick"
	);
	
	mpJoystickSouthpawCheckBox = new CCvarToggleCheckButton(
		this,
		"JoystickSouthpaw",
		"#GameUI_JoystickSouthpaw",
		"joy_movement_stick"
	);
	
	mpReverseJoystickCheckBox = new CCvarToggleCheckButton(
		this,
		"ReverseJoystick",
		"#GameUI_ReverseJoystick",
		"joy_inverty"
	);
	
	mpQuickInfoCheckBox = new CCvarToggleCheckButton(
		this,
		"HudQuickInfo",
		"#GameUI_HudQuickInfo",
		"hud_quickinfo"
	);
	
	mpMouseSensitivitySlider = new CCvarSlider(
		this,
		"Slider",
		"#GameUI_MouseSensitivity",
		1.0f,
		20.0f,
		"sensitivity",
		true
	);
	
	mpMouseSensitivityLabel = new TextEntry(
		this,
		"SensitivityLabel"
	);
	mpMouseSensitivityLabel->AddActionSignalTarget(this);
	
	mpJoyYawSensitivitySlider = new CCvarSlider(
		this,
		"JoystickYawSlider",
		"#GameUI_JoystickYawSensitivity",
		-0.5f,
		-7.0f,
		"joy_yawsensitivity",
		true
	);
	
	mpJoyYawSensitivityPreLabel = new Label(
		this,
		"JoystickYawSensitivityPreLabel",
		"#GameUI_JoystickLookSpeedYaw",
	);
	
	mpJoyPitchSensitivitySlider = new CCvarSlider(
		this,
		"JoystickPitchSlider",
		"#GameUI_JoystickPitchSensitivity",
		0.5f,
		7.0f,
		"joy_pitchsensitivity",
		true
	);
	
	mpJoyPitchSensitivityPreLabel = new Label(
		this,
		"JoystickPitchSensitivityPreLabel",
		"#GameUI_JoystickLookSpeedPitch"
	);
	
	LoadControlSettings("Resource\\OptionsSubMouse.res");
	
	//float fSensitivity{gpEngine->pfnGetCvarFloat("sensitivity")};
	ConVarRef var("sensitivity");
	if(var.IsValid())
	{
		float fSensitivity{var.GetFloat()};
		
		char sBuf[64]{};
		Q_snprintf(sBuf, sizeof(sBuf), " %.1f", fSensitivity);
		mpMouseSensitivityLabel->SetText(sBuf);
	};
	
	UpdateJoystickPanels();
};

COptionsSubMouse::~COptionsSubMouse() = default;

void COptionsSubMouse::OnResetData()
{
	mpReverseMouseCheckBox->Reset();
	mpMouseFilterCheckBox->Reset();
	mpJoystickCheckBox->Reset();
	mpJoystickSouthpawCheckBox->Reset();
	mpMouseSensitivitySlider->Reset();
	mpQuickInfoCheckBox->Reset();
	mpReverseJoystickCheckBox->Reset();
	mpJoyYawSensitivitySlider->Reset();
	mpJoyPitchSensitivitySlider->Reset();
};

void COptionsSubMouse::OnApplyChanges()
{
	mpReverseMouseCheckBox->ApplyChanges();
	mpMouseFilterCheckBox->ApplyChanges();
	mpJoystickCheckBox->ApplyChanges();
	mpJoystickSouthpawCheckBox->ApplyChanges();
	mpMouseSensitivitySlider->ApplyChanges();
	mpQuickInfoCheckBox->ApplyChanges();
	mpReverseJoystickCheckBox->ApplyChanges();
	mpJoyYawSensitivitySlider->ApplyChanges();
	mpJoyPitchSensitivitySlider->ApplyChanges();
	
	gpEngine->ClientCmd_Unrestricted("joyadvancedupdate");
};

void COptionsSubMouse::ApplySchemeSettings(IScheme *apScheme)
{
	BaseClass::ApplySchemeSettings(apScheme);
};

void COptionsSubMouse::OnControlModified(Panel *apPanel)
{
	PostActionSignal(new KeyValues("ApplyButtonEnable"));
	
	// The HasBeenModified() check is so that if the value is outside
	// of the range of the slider, it won't use the slider to determine
	// the display value but leave the real value that we determined
	// in the constructor
	if(apPanel == mpMouseSensitivitySlider && mpMouseSensitivitySlider->HasBeenModified())
		UpdateSensitivityLabel();
	else if(apPanel == mpJoystickCheckBox)
		UpdateJoystickPanels();
};

void COptionsSubMouse::OnTextChanged(Panel *apPanel)
{
	if(apPanel == mpMouseSensitivityLabel)
	{
		char sBuf[64]{};
		mpMouseSensitivityLabel->GetText(sBuf, sizeof(sBuf));
		
		float fValue{atof(sBuf)};
		if(fValue >= 1.0)
		{
			mpMouseSensitivitySlider->SetSliderValue(fValue);
			PostActionSignal(new KeyValues("ApplyButtonEnable"));
		};
	};
};

void COptionsSubMouse::UpdateSensitivityLabel()
{
	char sBuf[64]{};
	Q_snprintf(sBuf, sizeof(sBuf), " %.1f", mpMouseSensitivitySlider->GetSliderValue());
	mpMouseSensitivityLabel->SetText(sBuf);
};

void COptionsSubMouse::UpdateJoystickPanels()
{
	bool bEnabled{mpJoystickCheckBox->IsSelected()};
	
	mpReverseJoystickCheckBox->SetEnabled(bEnabled);
	mpJoystickSouthpawCheckBox->SetEnabled(bEnabled);
	mpQuickInfoCheckBox->SetEnabled(bEnabled);
	mpJoyYawSensitivitySlider->SetEnabled(bEnabled);
	mpJoyYawSensitivityPreLabel->SetEnabled(bEnabled);
	mpJoyPitchSensitivitySlider->SetEnabled(bEnabled);
	mpJoyPitchSensitivityPreLabel->SetEnabled(bEnabled);
};