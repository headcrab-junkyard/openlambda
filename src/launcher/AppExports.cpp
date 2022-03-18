/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2019, 2021 BlackPhrase
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

#include "exports.h"

// Prefer dedicated graphics cards if present
// NOTE: This needs to be placed in the executable, it won’t work in a dll

// https://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
C_EXPORT unsigned long NvOptimusEnablement{0x00000001};

// https://community.amd.com/thread/169965
C_EXPORT int AmdPowerXpressRequestHighPerformance{1};