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

enum class eDmgTypes : int
{
	NUM_DMG_TYPES = 12
};

class CHUDHealth : public CHUDElement
{
public:
	virtual int Init();
	virtual int VidInit();
	
	virtual void Reset();
	
	virtual int Draw(float afTime);
	
	int MsgFunc_Health(const char *asName, int anSize, void *apBuffer);
	int MsgFunc_Damage(const char *asName, int anSize, void *apBuffer);
	
	void GetPainColor(int &r, int &g, int &b);
public:
	float mfAttackFront{0.0f};
	float mfAttackRear{0.0f};
	float mfAttackLeft{0.0f};
	float mfAttackRight{0.0f};
	
	float mfFade{0.0f};
	
	int mnHealth{0};
	int mnDmgBio{0};
	int mnCross{0};
private:
	int DrawPain(float afTime);
	int DrawDamage(float afTime);
	
	void CalcDamageDirection(const idVec3 &avFrom);
	
	void UpdateTiles(float afTime, long anBits);
private:
	SpriteHandle_t mhSprite{};
	SpriteHandle_t mhDamage{};
	
	struct SDmgImage
	{
		float mfExpireTime{0.0f};
		float mfBaseline{0.0f};
		
		int x{0};
		int y{0};
	};
	
	SDmgImage mvDmgImages[NUM_DMG_TYPES]{};
};