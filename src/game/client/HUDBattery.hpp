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

class CHUDTexture;

class CHUDBattery : public CHUDElement
{
public:
	int Init();
	int VidInit();
	
	int Draw(float afTime);
	
	int MsgFunc_Battery(const char *asName, int anSize, void *apBuf);
private:
	CHUDTexture *mpSprite1{nullptr};
	CHUDTexture *mpSprite2{nullptr};
	
	float mfFade{0.0f};
	
	int mnBat{0};
	int mnBatMax{0};
	
	int mnHeight{0}; ///< Width of the battery innards
};