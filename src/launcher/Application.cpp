/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2018-2019, 2021-2022 BlackPhrase
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

#include <stdexcept>
#include "Application.hpp"
#include "AppConfig.hpp"

int CApplication::Run()
{
	do
	{
		if(!Init())
			return EXIT_FAILURE;
		
		int eEngineResult{mpEngine->Run(mhInstance, ".", msCmdLine, msCmdLinePostRestart, Sys_GetFactoryThis(), mfnFSFactory)};
		mbRestart = false;
		
		Shutdown();
		
		switch(eEngineResult)
		{
		case ENGINE_RESULT_UNSUPPORTEDVIDEO:
			return EXIT_FAILURE;
		case ENGINE_RESULT_RESTART:
			mbRestart = true;
			//strncpy(msCmdLine, msCmdLinePostRestart, sizeof(msCmdLine)); // Re-init the cmdline string with new args // TODO
		};
	}
	while(mbRestart);
	
	//setenv("OGS_LAST_INIT_OK", "1", 1); // TODO: use IRegistry or something
	
	// Return success of application
	return EXIT_SUCCESS;
};