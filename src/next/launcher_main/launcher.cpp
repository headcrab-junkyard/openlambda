/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018, 2020, 2024 BlackPhrase
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
#include <windows.h>
#endif

extern int AppMain(int argc, char **argv);

#ifdef _WIN32
//HINSTANCE global_hInstance;
//int global_nCmdShow;

/*
==================
WinMain
==================
*/
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Previous instances do not exist in Win32
	if(hPrevInstance)
		return 0;
	
	//global_hInstance = hInstance;
	//global_nCmdShow = nCmdShow;
	
	return AppMain(__argc, __argv);
};
#else // if not _WIN32
int main(int argc, char **argv)
{
	return AppMain(argc, argv);
};
#endif // _WIN32