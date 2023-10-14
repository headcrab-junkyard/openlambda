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

constexpr auto MAX_SPRITE_NAME_LENGTH{24};

class CHUDStatusIcons : public CHUDElement
{
public:
	int Init() override;
	int VidInit() override;
	
	void Reset() override;
	
	int Draw(float afTime) override;
public:
	int MsgFunc_StatusIcon(const char *asName, int anSize, void *apBuf);
	
	// Had to make these public so CHUD could access them (to enable concussion icon)
	// Could use a friend declaration instead...
	void EnableIcon(const char *asIconName, byte red, byte green, byte blue);
	void DisableIcon(const char *asIconName);
public:
	constexpr auto MAX_ICONSPRITENAME_LENGTH{MAX_SPRITE_NAME_LENGTH};
	constexpr auto MAX_ICONSPRITES{4};
private:
	struct IconSprite
	{
		char msSpriteName[MAX_ICONSPRITENAME_LENGTH];
		//
		CHUDTexture *mpIcon{nullptr};
		//
		//SpriteHandle_t mhSprite{};
		//wrect_t mSpriteRect;
		//
		byte r, g, b;
	};
	
	IconSprite mIconList[MAX_ICONSPRITES]{};
};