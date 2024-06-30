/*
 * This file is part of OpenLambda Project
 * Copyright (C) 2024 BlackPhrase
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
/// @brief defines and such for a 3rd person camera

#pragma once

// NOTE: must include quakedef.h first

// Pitch, yaw, dist
extern vec3_t cam_ofs;

// Using third person camera
extern int cam_thirdperson;

void CAM_Init();
void CAM_ClearStates();

void CAM_StartMouseMove();
void CAM_EndMouseMove();