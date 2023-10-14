/*
 * This file is part of OpenLambda Project
 * Copyright (C) 2018-2021, 2023 BlackPhrase
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

struct kbutton_s /*CL_DLLEXPORT*/ *KB_FindKey(const char *name)
{
	//RecClFindKey(name);
	
	kblist_t *pList{g_kbkeys};
	
	while(pList)
	{
		if(!stricmp(name, pList->name))
			return pList->pkey;
		
		pList = pList->next;
	};
	
	return nullptr;
};

// TODO: CL_CreateMove

int /*CL_DLLEXPORT*/ HUD_Key_Event(int down, int keynum, const char *pszCurrentBinding)
{
	//RecClKeyEvent(down, keynum, pszCurrentBinding);
	
	if(gpViewPort)
		gpViewPort->HandleKeyInput(down, keynum, pszCurrentBinding);
	
	return 1;
};