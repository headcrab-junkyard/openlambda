/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2021-2023 BlackPhrase
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

//=============================================================================

enum eAnimType // TODO: ePlayerAnimTypes? PLAYER_ANIM in original code
{
	PLAYER_IDLE = 0,
	PLAYER_WALK,
	PLAYER_JUMP,
	PLAYER_SUPERJUMP,
	PLAYER_DIE,
	PLAYER_ATTACK1,
};

enum eActivityTypes
{
	ACT_RESET = 0,
	
	ACT_IDLE = 1,
	ACT_GUARD,
	ACT_WALK,
	ACT_RUN,
	ACT_FLY,
	ACT_SWIM,
	ACT_HOP,
	ACT_LEAP,
	ACT_FALL,
	ACT_LAND,
};

using tActivityMap = std::map<eActivityTypes, const char *>;