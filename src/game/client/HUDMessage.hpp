/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2023 BlackPhrase
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

#include "HUDElement.hpp"

class CHUDMessage : public CHUDElement
{
public:
	int Init() override;
	int VidInit() override;
	
	void Reset() override;
	
	int Draw(float afTime) override;
public:
	float FadeBlend(float afFadeIn, float afFadeOut, float afHoldTime, float afLocalTime);
	
	void MessageAdd(const char *asName, float afTime);
	void MessageAdd(client_textmessage_t *apMsg);
	
	void MessageDrawScan(client_textmessage_t *apMsg, float afTime);
	
	void MessageScanStart();
	void MessageScanNextChar();
	
	int GetXPosition(float x, int anWidth, int anLineWidth) const;
	int GetYPosition(float y, int anHeight) const;
public:
	int MsgFunc_HudText(const char *asName, int anSize, void *apBuf);
	int MsgFunc_HudTextPro(const char *asName, int anSize, void *apBuf);
	int MsgFunc_GameTitle(const char *asName, int anSize, void *apBuf);
private:
	constexpr auto MAX_HUD_MESSAGES{16};
	
	client_textmessage_t *mpMessages[MAX_HUD_MESSAGES]{};
	
	message_parms_t mParms{};
	
	client_textmessage_t *mpGameTitle{nullptr};
	
	float mfStartTime[MAX_HUD_MESSAGES]{0.0f};
	
	float mfGameTitleTime{0.0f};
	
	int mnTitleLife{0};
	int mnTitleHalf{0};
};