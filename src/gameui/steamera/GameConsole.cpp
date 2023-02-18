/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018, 2020, 2023 BlackPhrase
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

#include "GameConsole.hpp"
#include "GameConsoleDialog.hpp"
//#include "LoadingDialog.hpp"

//#include <cstdio>

#include <KeyValues.h>

//#include <vgui/VGUI.h>
//#include <vgui/IVGUI.h>
//#include <vgui/ISurface.h>

//#include <vgui/controls/Panel.h>

//#include <tier1/convar.h>

// NOTE: memdbgon must be the last include file in a .cpp file!
//#include <tier0/memdbgon.h>

CGameConsole gGameConsole;

/*
================
Con_Dump_f

Save the console contents out to a file
================
*/
void Con_Dump_f()
{
	int l, x;
	char *line;
	FileHandle_t f;
	char buffer[1024];
	char name[MAX_OSPATH];

	if(Cmd_Argc() != 2)
	{
		Con_Printf("usage: condump <filename>\n");
		return;
	};

	snprintf(name, sizeof(name), "%s/%s.txt", com_gamedir, Cmd_Argv(1)); // TODO: COM_Gamedir()

	Con_Printf("Dumped console text to %s.\n", name);
	COM_CreatePath(name);
	f = FS_Open(name, "w"/*, "GAMEFOLDER"*/); // TODO: FS_OpenPathID?
	if(!f)
	{
		Con_Printf("ERROR: couldn't open.\n");
		return;
	}

	// skip empty lines
	for(l = con_current - con_totallines + 1; l <= con_current; l++)
	{
		line = con_text + (l % con_totallines) * con_linewidth;
		for(x = 0; x < con_linewidth; x++)
			if(line[x] != ' ')
				break;
		if(x != con_linewidth)
			break;
	}

	// write the remaining lines
	buffer[con_linewidth] = 0;
	for(; l <= con_current; l++)
	{
		line = con_text + (l % con_totallines) * con_linewidth;
		strncpy(buffer, line, con_linewidth);
		for(x = con_linewidth - 1; x >= 0; x--)
		{
			if(buffer[x] == ' ')
				buffer[x] = 0;
			else
				break;
		}
		for(x = 0; buffer[x]; x++)
			buffer[x] &= 0x7f;

		FS_FPrintf(f, "%s\n", buffer);
	}

	FS_Close(f);
}

CGameConsole &GameConsole()
{
	return gGameConsole;
};

EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CGameConsole, IGameConsole, GAMECONSOLE_INTERFACE_VERSION, gGameConsole);

CGameConsole::CGameConsole() = default;

CGameConsole::~CGameConsole()
{
	mbInitialized = false;
};

void CGameConsole::Activate()
{
	if(!mbInitialized)
		return;
	
	vgui2::surface()->RestrictPaintToSinglePanel(nullptr);
	mpConsole->Activate();
};

void CGameConsole::Initialize()
{
#if !(OPENLAMBDA_PLATFORM == OPENLAMBDA_PLATFORM_XBOX)
	if(mbInitialized)
		return;
	
	// We add text before displaying this so set it up now!
	mpConsole = vgui2::SETUP_PANEL(new CGameConsoleDialog());
	
	// Set the console to talking up most of the right-half of the screen
	int nScrWidth, nScrHeight;
	vgui2::surface()->GetScreenSize(nScrWidth, nScrHeight);
	
	auto nOffset{vgui2::scheme()->GetProportionalScaledValue(16)};
	
	mpConsole->SetBounds(
		(nScrWidth * 0.5) - (nOffset * 4),
		nOffset,
		(nScrWidth * 0.5) + (nOffset * 3),
		nScrHeight - (nOffset * 8)
	);
	
	mbInitialized = true;
#endif
};

void CGameConsole::Hide()
{
	if(!mbInitialized)
		return;
	
	mpConsole->Hide();
};

void CGameConsole::Clear()
{
	if(!mbInitialized)
		return;
	
	mpConsole->Clear();
};

bool CGameConsole::IsConsoleVisible()
{
	if(!mbInitialized)
		return false;
	
	return mpConsole->IsVisible();
};

void CGameConsole::Printf(const char *format, ...)
{
	if(!mbInitialized)
		return;
	
	// TODO
};

void CGameConsole::DPrintf(const char *format, ...)
{
	if(!mbInitialized)
		return;
	
	// TODO
};

void CGameConsole::SetParent(int /*vgui2::VPANEL*/ parent)
{
	if(!mbInitialized)
		return;
	
	mpConsole->SetParent(static_cast<vgui2::VPANEL>(parent));
};

void CGameConsole::ActivateDelayed(float afTime)
{
	if(!mbInitialized)
		return;
	
#if !(OPENLAMBDA_PLATFORM == OPENLAMBDA_PLATFORM_XBOX)
	mpConsole->PostMessage(mpConsole, new KeyValues("Activate"), afTime);
#endif
};

void CGameConsole::OnCmdCondump()
{
#if !(OPENLAMBDA_PLATFORM == OPENLAMBDA_PLATFORM_XBOX)
	Con_Dump_f(); // TODO: gGameConsole.mpConsole->DumpConsoleTextToFile();
#endif
};

#if !(OPENLAMBDA_PLATFORM == OPENLAMBDA_PLATFORM_XBOX)
CON_COMMAND(condump, "dump the text currently in the console to condumpXX.log")
{
	gGameConsole.OnCmdCondump();
};
#endif