/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2018-2020, 2024 BlackPhrase
 *
 * OGSNext Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OGSNext Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OGSNext Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#ifdef _WIN32
	#define EXPORT [[gnu::dllexport]]
#else
	#define EXPORT [[gnu::visibility("default")]]
#endif

// Hints to prefer the dedicated GPUs over the iGPUs if both are present
// NOTE: This needs to be placed inside of the executable or it won't work
extern "C"
{

// https://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
EXPORT unsigned long NvOptimusEnablement{0x00000001};

// https://community.amd.com/thread/169965
EXPORT int AmdPowerXpressRequestHighPerformance{1};

};