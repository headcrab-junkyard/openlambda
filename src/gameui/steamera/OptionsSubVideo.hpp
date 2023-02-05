/*
 * This file is part of OGS Engine
 * Copyright (C) 2018, 2021, 2023 BlackPhrase
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

#pragma once

#include <vgui_controls/PropertyPage.h>

class COptionsSubVideo : public vgui::PropertyPage
{
	DECLARE_CLASS_SIMPLE(COptionsSubVideo, vgui::PropertyPage);
public:
	COptionsSubVideo(vgui::Panel *apParent);
	~COptionsSubVideo();
private:
	void SetCurrentResolutionComboItem();
	
	void PrepareResolutionList();
private:
	CCvarSlider *mpBrightnessSlider{nullptr};
	CCvarSlider *mpGammaSlider{nullptr};
	
	vgui::ComboBox *mpResolutionCombo{nullptr};
	vgui::ComboBox *mpAspectRatioCombo{nullptr};
	vgui::ComboBox *mpRendererCombo{nullptr};
	
	vgui::CheckButton *mpVSyncCheck{nullptr};
	vgui::CheckButton *mpWindowedCheck{nullptr};
	
	vgui::Label *mpBrightnessLabel{nullptr};
	vgui::Label *mpGammaLabel{nullptr};
	vgui::Label *mpResolutionLabel{nullptr};
	vgui::Label *mpRendererLabel{nullptr};
	vgui::Label *mpDisplayModeLabel{nullptr};
	vgui::Label *mpVideoRestartLabel{nullptr};
	
	vgui::CheckButton *mpDetailTexturesCheck{nullptr};
	vgui::CheckButton *mpHDModelsCheck{nullptr};
	vgui::CheckButton *mpAddonsFolderCheck{nullptr};
	vgui::CheckButton *mpLowVideoDetailCheck{nullptr};
	
	int mnSelectedMode{-1};
};