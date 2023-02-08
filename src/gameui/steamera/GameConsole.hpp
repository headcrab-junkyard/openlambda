/*
 * This file is part of OpenLambda Project
 *
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

#pragma once

#include "GameUI/IGameConsole.h"

class CGameConsoleDialog;

class CGameConsole final : public IGameConsole
{
public:
	CGameConsole();
	~CGameConsole();

	void Activate() override;

	void Initialize() override;

	void Hide() override;

	void Clear() override;

	bool IsConsoleVisible() override;

	void Printf(const char *format, ...) override;
	void DPrintf(const char *format, ...) override;

	void SetParent(int /*vgui2::VPANEL*/ parent) override;
private:
	CGameConsoleDialog *mpConsole{nullptr};
	
	bool mbInitialized{false};
};