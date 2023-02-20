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
/// @brief Includes all the headers/decls necessary to access the engine interface

#pragma once

// This stupid set of includes is required to use the cdll_int interface
// TODO: fix
#include <mathlib/vector.h>
//#include <wrect.h>
#define IN_BUTTONS_H // TODO

// Engine interface
#include <engine/cdll_int.h>
#include <engine/eiface.h>

#include <icvar.h>

//#include <tier2/tier2.h>

//#include <matchmaking/imatchframework.h>

// Engine interface singleton accessors
extern struct cl_enginefuncs_s /*IVEngineClient*/ *gpEngine;
extern class IBik *gpBik;
extern class IEngineVGui *gpEngineVGuiFuncs;
extern class IGameUIFuncs *gpGameUIFuncs;
//extern class IEngineSound *gpEngineSound;
//extern class IAchievementMgr *gpAchievementMgr;
//extern class CSteamAPIContext *gpSteamAPIContext;