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

#include "OptionsSubVideo.hpp"

COptionsSubVideo::COptionsSubVideo(vgui::Panel *apParent) : vgui::PropertyPage(apParent, nullptr)
{
	mbRequireRestart = false;
	
	mpBrightnessSlider = new CCvarSlider(this, "Brightness");
	mpGammaSlider = new CCvarSlider(this, "Gamma");
	
	mpResolutionCombo = new vgui::ComboBox(this, "Resolution", 8, false);
	mpAspectRatioCombo = new vgui::ComboBox(this, "AspectRatio", 6, false);
	mpRendererCombo = new vgui::ComboBox(this, "Renderer");
	
	mpVSyncCheck = new vgui::CheckButton(this, "VSync", "");
	mpWindowedCheck = new vgui::CheckButton(this, "Windowed", "");
	
	mpBrightnessLabel = new vgui::Label(this, "brightness label");
	mpGammaLabel = new vgui::Label(this, "Gamma label");
	mpResolutionLabel = new vgui::Label(this, "Label1");
	mpRendererLabel = new vgui::Label(this, "Label2");
	mpDisplayModeLabel = new vgui::Label(this, "Label4");
	mpVideoRestartLabel = new vgui::Label(this, "Label5");
	
	mpDetailTexturesCheck = new vgui::CheckButton(this, "DetailTextures", "");
	mpHDModelsCheck = new vgui::CheckButton(this, "HDModels", "");
	mpAddonsFolderCheck = new vgui::CheckButton(this, "AddonsFolder", "");
	mpLowVideoDetailCheck = new vgui::CheckButton(this, "LowVideoDetail", "");
	
	LoadControlSettings("Resource/OptionsSubVideo.res");
	
	PrepareResolutionList();
};

COptionsSubVideo::~COptionsSubVideo() = default;

void COptionsSubVideo::OnResetData()
{
};

void COptionsSubVideo::OnApplyChanges()
{
};

void COptionsSubVideo::PerformLayout()
{
	BaseClass::PerformLayout();
};

void COptionsSubVideo::OnTextChanged(vgui::Panel *apPanel, const char *asText)
{
};

void COptionsSubVideo::OnDataChanged()
{
	PostActionSignal(new KeyValues("ApplyButtonEnable"));
};

bool COptionsSubVideo::RequiresRestart()
{
	return mbRequireRestart;
};

void COptionsSubVideo::LaunchBenchmark()
{
	BasePanel()->OnOpenBenchmarkDialog();
};