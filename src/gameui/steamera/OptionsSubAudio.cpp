/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2018, 2021-2023 BlackPhrase
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

#include "OptionsSubAudio.hpp"

using namespace vgui;

COptionsSubAudio::COptionsSubAudio(Panel *apParent)
	: PropertyPage(apParent, nullptr)
{
	mpSFXSlider = new CCvarSlider(this, "SFXSlider", "#GameUI_SoundEffectVolume", 0.0f, 1.0f, "volume");
	mpMusicSlider = new CCvarSlider(this, "MusicSlider", "#GameUI_MusicVolume", 0.0f, 1.0f, "Snd_MusicVolume");
	
	mpCloseCaptionCombo = new ComboBox(this, "CloseCaptionCheck", 6, false);
	mpCloseCaptionCombo->AddItem("#GameUI_NoClosedCaptions", nullptr);
	mpCloseCaptionCombo->AddItem("#GameUI_SubtitlesAndSoundEffects", nullptr);
	mpCloseCaptionCombo->AddItem("#GameUI_Subtitles", nullptr);
	
	mpSoundQualityCombo = new ComboBox(this, "SoundQuality", 6, false);
	mpSoundQualityCombo->AddItem("#GameUI_High", new KeyValues("SoundQuality", "quality", SOUNDQUALITY_HIGH));
	mpSoundQualityCombo->AddItem("#GameUI_Medium", new KeyValues("SoundQuality", "quality", SOUNDQUALITY_MEDIUM));
	mpSoundQualityCombo->AddItem("#GameUI_Low", new KeyValues("SoundQuality", "quality", SOUNDQUALITY_LOW));
	
	mpSpeakerSetupCombo = new ComboBox(this, "SpeakerSetup", 6, false);
	mpSpeakerSetupCombo->AddItem("#GameUI_Headphones", new KeyValues("SpeakerSetup", "speakers", 0));
	mpSpeakerSetupCombo->AddItem("#GameUI_2Speakers", new KeyValues("SpeakerSetup", "speakers", 2));
	mpSpeakerSetupCombo->AddItem("#GameUI_4Speakers", new KeyValues("SpeakerSetup", "speakers", 4));
	mpSpeakerSetupCombo->AddItem("#GameUI_5Speakers", new KeyValues("SpeakerSetup", "speakers", 5));
	mpSpeakerSetupCombo->AddItem("#GameUI_7Speakers", new KeyValues("SpeakerSetup", "speakers", 7));
	
	mpSpokenLanguageCombo = new ComboBox(this, "AudioSpokenLanguage", 6, false);
	
	LoadControlSettings("Resource\\OptionsSubAudio.res");
};

COptionsSubAudio::~COptionsSubAudio() = default;

void COptionsSubAudio::OnResetData()
{
	mbRequireRestart = false;
	
	mpSFXSlider->Reset();
	mpMusicSlider->Reset();
	
	// Reset the combo boxes
	
	// Close captions
	if(false)
		;
	else
		;
};

void COptionsSubAudio::OnApplyChanges()
{
	mpSFXSlider->ApplyChanges();
	mpMusicSlider->ApplyChanges();
};

void COptionsSubAudio::OnCommand(const char *asCmd)
{
	BaseClass::OnCommand(asCmd);
};

bool COptionsSubAudio::RequiresRestart() const
{
	// Nothing in audio required a restart right now
	return false;
};

void COptionsSubAudio::OnControlModified()
{
	PostActionSignal(new KeyValues("ApplyButtonEnable"));
};

void COptionsSubAudio::RunTestSpeakers()
{
};

void COptionsSubAudio::OpenThirdPartySoundCreditsDialog()
{
};