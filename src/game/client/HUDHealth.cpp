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

#include "HUDHealth.hpp"

int CHUDHealth::Init()
{
	return 0;
};

int CHUDHealth::VidInit()
{
	return 0;
};

void CHUDHealth::Reset()
{
};

int CHUDHealth::Draw(float afTime)
{
	return 0;
};

int CHUDHealth::MsgFunc_Health(const char *asName, int anSize, void *apBuffer)
{
	return 0;
};

int CHUDHealth::MsgFunc_Damage(const char *asName, int anSize, void *apBuffer)
{
	return 0;
};

void CHUDHealth::GetPainColor(int &r, int &g, int &b)
{
};