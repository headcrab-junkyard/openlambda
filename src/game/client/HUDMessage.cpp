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

#include "HUDMessage.hpp"

int CHUDMessage::Init()
{
	return 0;
};

int CHUDMessage::VidInit()
{
	return 0;
};

void CHUDMessage::Reset()
{
};

int CHUDMessage::Draw(float afTime)
{
	return 0;
};

float CHUDMessage::FadeBlend(float afFadeIn, float afFadeOut, float afHoldTime, float afLocalTime)
{
	return 0.0f;
};

void CHUDMessage::MessageAdd(const char *asName, float afTime)
{
};

void CHUDMessage::MessageAdd(client_textmessage_t *apMsg)
{
};

void CHUDMessage::MessageDrawScan(client_textmessage_t *apMsg, float afTime)
{
};

void CHUDMessage::MessageScanStart()
{
};

void CHUDMessage::MessageScanNextChar()
{
};

int CHUDMessage::GetXPosition(float x, int anWidth, int anLineWidth) const
{
	return;
};

int CHUDMessage::GetYPosition(float y, int anHeight) const
{
	return 0;
};

int CHUDMessage::MsgFunc_HudText(const char *asName, int anSize, void *apBuf)
{
	return 0;
};

int CHUDMessage::MsgFunc_HudTextPro(const char *asName, int anSize, void *apBuf)
{
	return 0;
};

int CHUDMessage::MsgFunc_GameTitle(const char *asName, int anSize, void *apBuf)
{
	return 0;
};