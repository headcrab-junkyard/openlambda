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

#include "HUDStatusIcons.hpp"

int CHUDStatusIcons::Init()
{
	return 0;
};

int CHUDStatusIcons::VidInit()
{
	return 0;
};

void CHUDStatusIcons::Reset()
{
};

int CHUDStatusIcons::Draw(float afTime)
{
	return 0;
};

int CHUDStatusIcons::MsgFunc_StatusIcon(const char *asName, int anSize, void *apBuf)
{
	return 0;
};

void CHUDStatusIcons::EnableIcon(const char *asIconName, byte red, byte green, byte blue)
{
};

void CHUDStatusIcons::DisableIcon(const char *asIconName)
{
};