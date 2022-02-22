/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2022 BlackPhrase
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

#include "Engine.hpp"

void Host_GetConsoleCommands();

CEngine::CEngine(IEngine *apEngine) : mpEngine(apEngine)
{
    //gpEngine = this;
};

CEngine::~CEngine()
{
    mpEngine->Shutdown();
    //gpEngine = nullptr;
};

bool CEngine::Frame()
{
    return mpEngine->Frame();
};

Result CEngine::Run(const InitParams &aInitParams)
{
    if(!Init(aInitParams))
        return Result::None;
        //return Result::UnsupportedVideo; // TODO

    // Main loop
    bool bRunning{true};
    while(bRunning)
    {
        // Check for commands typed to the host
        Host_GetConsoleCommands();
        
        bRunning = Frame();

#ifdef _WIN32		
        UpdateStatus(0);
#endif
    };

    // TODO: handle Result::Restart
    return Result::None;
};

void CEngine::AddConsoleText(const char *asText)
{
    // TODO
};

void CEngine::UpdateStatus(Status &aStatus)
{
    // TODO
};

bool CEngine::Init(const InitParams &aInitParams)
{
    return pEngine->Init(aInitParams);
};