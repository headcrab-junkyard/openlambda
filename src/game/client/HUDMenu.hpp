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

class CHUDMenu : public CHUDElement
{
public:
	int Init();
	int VidInit();
	
	void InitHUDData(); // TODO: make private?
	
	void Reset();
	
	int Draw(float afTime);
	
	int MsgFunc_ShowMenu(const char *asName, int anSize, void *apBuf);
	
	void SelectMenuItem(int anItem);
public:
	float mfShutOffTime{0.0f};
	
	int mnBitsValidSlots{0};
	
	bool mbMenuDisplayed{false}; // TODO: was int
	bool mbWaitingForMore{false}; // TODO: was int
};