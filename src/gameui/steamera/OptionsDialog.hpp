/*
 * This file is part of OGS Engine
 * Copyright (C) 2018, 2022 BlackPhrase
 *
 * OGS Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OGS Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OGS Engine. If not, see <http://www.gnu.org/licenses/>.
 */

/// @file

#pragma once

class COptionsDialog
{
public:
	COptionsDialog();
	~COptionsDialog();
	
	/// Brings the dialog to the foreground
	void Activate();
	
	/// Opens the dialog
	void Run();
	
	/// Opens the gamma dialog directly
	void OpenGammaDialog();
	
	/// Called when the game UI is hidden
	void OnGameUIHidden();
};