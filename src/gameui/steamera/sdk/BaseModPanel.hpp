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

namespace BaseModUI
{

enum class eWindowType : int
{
	None = 0,
	
	SizeOf // NOTE: Must be last in the list!
};

class CBasePanel : public vgui::EditablePanel//, public IMatchEventsSink
{
	DECLARE_CLASS_SIMPLE(CBasePanel, vgui::EditablePanel);
public:
	CBasePanel();
	~CBasePanel();
	
	static CBasePanel &GetSingleton();
	static CBasePanel *GetSingletonPtr();
	
	void ReloadScheme();
	
	void RunFrame();
protected:	
private:
};

}; // namespace BaseModUI