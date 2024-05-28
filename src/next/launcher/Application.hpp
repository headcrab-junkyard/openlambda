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

#pragma once

#include <cstdlib>
#include <cstring>

#include <engine/IEngine.hpp>

class CApplication
{
public:
	CApplication();
	~CApplication();
	
	int Run();
protected:
	virtual bool PostInit(){return true;}
private:
	bool Init();
	void Shutdown();
	
	bool LoadFileSystemModule(const char *asName);
	void LoadNetworkSystemModule(const char *asName);
	void LoadPhysicsModule(const char *asName);
	void LoadScriptSystemModule(const char *asName);
	void LoadEngineClientModule(const char *asName);
	void LoadInputSystemModule(const char *asName);
	void LoadSoundSystemModule(const char *asName);
	void LoadVGUI3Module(const char *asName);
	void LoadEngineModule(const char *asName);
private:
	void *mpFSLib{nullptr};
	void *mpNetworkSystemLib{nullptr};
	void *mpPhysicsLib{nullptr};
	void *mpScriptSystemLib{nullptr};
	void *mpEngineClientLib{nullptr};
	void *mpInputSystemLib{nullptr};
	void *mpSoundSystemLib{nullptr};
	void *mpVGUI3Lib{nullptr};
	void *mpEngineLib{nullptr};
	
	CreateInterfaceFn mpEngineFactory{nullptr};
	
	IEngine *mpEngine{nullptr};
	
	bool mbRestart{false};
};