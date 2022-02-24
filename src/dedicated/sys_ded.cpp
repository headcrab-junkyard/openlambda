/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018-2022 BlackPhrase
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

#include <cstdio>
#include <stdexcept>

#ifdef _WIN32
	#include <windows.h>
	#include <conio.h>
	#include "win/conproc.h"
#endif

#include "tier1/interface.h"

#include "Engine.hpp"
#include "EngineLegacy.hpp"

#include "filesystem/IFileSystem.hpp"

#ifdef _WIN32
static bool sc_return_on_enter{false};
HANDLE hinput, houtput;
#endif

void *gpFSLib{nullptr};
CreateInterfaceFn gfnFSFactory{nullptr};

void *gpEngineLib{nullptr};
CreateInterfaceFn gfnEngineFactory{nullptr};

CEngine *gpEngine{nullptr}; // TODO: hacky way to access the command buffer...


IBaseInterface *LauncherFactory(const char *name, int *retval)
{
	// Filesystem module factory
	if(!strcmp(name, OGS_FILESYSTEM_INTERFACE_VERSION))
		return gfnFSFactory(name, retval);
	
	auto fnThisFactory{Sys_GetFactoryThis()};
	return fnThisFactory(name, retval);
};

int CreateConsoleWindow()
{
#ifdef _WIN32
	if(!AllocConsole())
		return 0; // TODO: was Sys_Error("Couldn't create dedicated server console");

	hinput = GetStdHandle(STD_INPUT_HANDLE);
	houtput = GetStdHandle(STD_OUTPUT_HANDLE);

	InitConProc();
#endif // _WIN32

	return 1;
};

void DestroyConsoleWindow()
{
#ifdef _WIN32
	FreeConsole();
	
	// shutdown QHOST hooks if necessary
	DeinitConProc();
#endif
};

#ifdef _WIN32
/*
===================
UpdateStatus

Update the status line at the top of the console window if engine is running
===================
*/
void UpdateStatus(bool abForce)
{
	static double fTimeLast = 0.0;
	double fTimeCurrent{0.0};
	
	float fFPS{0.0f};
	int nActivePlayers{0}, nMaxPlayers{0};
	char sMap[32]{};
	
	fTimeCurrent = (float)(timeGetTime() / 1000.0f);
	
	gpEngine->UpdateStatus(&fFPS, &nActivePlayers, &nMaxPlayers, sMap); // TODO
	
	if(!abForce)
		if((fTimeCurrent - fTimeLast) < 0.5f)
			return;
	
	fTimeLast = fTimeCurrent;
	
	char sPrompt[256]{};
	
	snprintf(sPrompt, sizeof(sPrompt), "%.1f fps %2d/%2d on %16s", (float)fFPS, nActivePlayers, nMaxPlayers, sMap);
	
	WriteStatusText(sPrompt);
};
#endif
char *Sys_ConsoleInput()
{
#ifdef _WIN32

//#ifdef SWDS
	static char text[256];
	static int len;
	int c;

	// read a line out
	while(_kbhit())
	{
		c = _getch();
		putch(c);
		if(c == '\r')
		{
			text[len] = 0;
			putch('\n');
			len = 0;
			return text;
		};

		if(c == 8)
		{
			//if(len) // TODO
			{
				putch(' ');
				putch(c);
				len--;
				text[len] = 0;
			};
			continue;
		};

		text[len] = c;
		len++;
		text[len] = 0;

		if(len == sizeof(text))
			len = 0;
	};

	return nullptr;
/*
#else // if not SWDS
	static char text[256]{};
	static int len;
	INPUT_RECORD recs[1024]{};
	int count;
	int i, dummy;
	int ch, numread, numevents;

	for(;;)
	{
		if(!GetNumberOfConsoleInputEvents(hinput, &numevents))
			Sys_Error("Error getting # of console events");

		if(numevents <= 0)
			break;

		if(!ReadConsoleInput(hinput, recs, 1, &numread))
			Sys_Error("Error reading console input");

		if(numread != 1)
			Sys_Error("Couldn't read console input");

		if(recs[0].EventType == KEY_EVENT)
		{
			if(!recs[0].Event.KeyEvent.bKeyDown)
			{
				ch = recs[0].Event.KeyEvent.uChar.AsciiChar;

				switch(ch)
				{
				case '\r':
					WriteFile(houtput, "\r\n", 2, &dummy, nullptr);

					if(len)
					{
						text[len] = 0;
						len = 0;
						return text;
					}
					else if(sc_return_on_enter)
					{
						// special case to allow exiting from the error handler on Enter
						text[0] = '\r';
						len = 0;
						return text;
					};

					break;

				case '\b':
					WriteFile(houtput, "\b \b", 3, &dummy, nullptr);
					if(len)
						len--;

					break;

				default:
					if(ch >= ' ')
					{
						WriteFile(houtput, &ch, 1, &dummy, nullptr);
						text[len] = ch;
						len = (len + 1) & 0xff;
					};

					break;
				};
			};
		};
	};

	return nullptr;
#endif // SWDS
*/

#elif __linux__
	static char text[256];
	int len;
	fd_set fdset;
	struct timeval timeout;

	FD_ZERO(&fdset);
	FD_SET(0, &fdset); // stdin
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	if(select(1, &fdset, nullptr, nullptr, &timeout) == -1 || !FD_ISSET(0, &fdset))
		return nullptr;

	len = read(0, text, sizeof(text));
	if(len < 1)
		return nullptr;
	text[len - 1] = 0; // rip off the /n and terminate

	return text;
#elif sun
	static char text[256];
	int len;
	fd_set readfds;
	int ready;
	struct timeval timeout;

	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	FD_ZERO(&readfds);
	FD_SET(0, &readfds);
	ready = select(1, &readfds, 0, 0, &timeout);

	if(ready > 0)
	{
		len = read(0, text, sizeof(text));
		if(len >= 1)
		{
			text[len - 1] = 0; // rip off the /n and terminate
			return text;
		};
	};

	return 0;
#endif // _WIN32
};

/*
===================
Host_GetConsoleCommands

Add them exactly as if they had been typed at the console
===================
*/
void Host_GetConsoleCommands()
{
	char *cmd{nullptr};

	while(1)
	{
		cmd = Sys_ConsoleInput();
		if(!cmd)
			break;
		gpEngine->AddConsoleText(cmd);
	};
};

void LoadFileSystemModule(const char *asName)
{
	auto pFSLib{Sys_LoadModule(asName)};
	
	if(!pFSLib)
		throw std::runtime_error(std::string("Failed to load the filesystem module (") + asName + ")!");
	
	gpFSLib = pFSLib;

	auto pFSFactory{Sys_GetFactory(pFSLib)};
	
	if(!pFSFactory)
		throw std::runtime_error(std::string("Failed to get the filesystem module factory (") + asName + ")!");
	
	gfnFSFactory = pFSFactory;
};

	

void LoadEngineModule(const char *asName)
{
	auto pEngineLib{Sys_LoadModule(asName)};

	if(!pEngineLib)
		throw std::runtime_error(std::string("Failed to load the engine module (") + asName + ")!");
	
	gpEngineLib = pEngineLib;
	
	auto pEngineFactory{Sys_GetFactory(pEngineLib)};
	
	if(!pEngineFactory)
		throw std::runtime_error(std::string("Failed to get the engine module factory (") + asName + ")!");
	
	gfnEngineFactory = pEngineFactory;
};

	{
	};
	

/*
===================
RunServer
===================
*/
int RunServer() // void?
{
		// File system module name to load
		const char *sFSModuleName{Config::Defaults::FSModuleName};

		LoadFileSystemModule(sFSModuleName);

		// Engine module name to load
		const char *sEngineModuleName{ChooseEngineModuleName()};

		LoadEngineModule(sEngineModuleName);
		
		auto pEngine{CreateEngine(gfnEngineFactory)};

		gpEngine = pEngine;

		IEngine::InitParams InitParams{};
		
		InitParams.fnLauncherFactory = Sys_GetFactoryThis();
		InitParams.sGameDir = "."; // TODO: goldsrctest?
		InitParams.sCmdLine = "TODO";
		InitParams.bDedicated = true;
		
		/*
		if(!pEngine->Init(InitParams))
			return EXIT_FAILURE;
		*/

		auto eResult{pEngine->Run(InitParams)};


	return EXIT_SUCCESS;
};

/*
==================
main

==================
*/
//char *newargv[256]; // TODO: unused?
int main(int argc, char **argv)
{
	//CCmdLine CmdLine(argc, argv); // TODO
	
	if(!CreateConsoleWindow())
		return EXIT_FAILURE;
	
	if(!RunServer())
		return EXIT_FAILURE;
	
	DestroyConsoleWindow();

	// return success of application
	return EXIT_SUCCESS;
};