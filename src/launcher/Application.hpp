/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2018-2021 BlackPhrase
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

#include <cstdlib>
#include <cstring>

#include "tier1/interface.h"
#include "engine_launcher_api.h"

class CApplication
{
public:
	CApplication();
	~CApplication();
	
	int Run();
private:
	bool Init();
	void Shutdown();
	
	void LoadFileSystemModule(const char *asName);
	void LoadEngineModule(const char *asName);
	
	void *mpFSLib{nullptr};
	void *mpEngineLib{nullptr};
	
	CreateInterfaceFn mfnFSFactory{nullptr};
	CreateInterfaceFn mfnEngineFactory{nullptr};
	
	IEngineAPI *mpEngine{nullptr};
	
	bool mbRestart{false};
protected:
	virtual bool PostInit(){return true;}
	
	char msCmdLine[512]{}; // TODO: fix magic
	char msCmdLinePostRestart[512]{}; // TODO: fix magic
	
	void *mhInstance{nullptr};
};