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
/// @brief engine execution mode interface

#pragma once

#include "CommonTypes.hpp"

#include "next/engine/IEngine.hpp"

#include "engine_launcher_api.h"

enum class CEngine::Result;

struct IEngine::InitParams;

interface IEngine;
interface IEngineAPI;

void Host_GetConsoleCommands();

interface IEngineExecMode
{
    ///
    virtual CEngine::Result Run(const IEngine::InitParams &aInitParams) = 0;
};

class CEngineExecMode_Manual : public IEngineExecMode
{
public:
    CEngineExecMode_Manual(IEngine *apEngine) : mpEngine(apEngine){}

    CEngine::Result Run(const IEngine::InitParams &aInitParams) override
    {
        if(!Init(aInitParams))
            return CEngine::Result::None;
            //return CEngine::Result::UnsupportedVideo; // TODO

        // Main loop
        bool bRunning{true};
        while(bRunning)
        {
            PreFrame();
            
            bRunning = Frame();

            PostFrame();
        };

        Shutdown();

        // TODO: handle Result::Restart
        return CEngine::Result::None;
    };
protected:
    virtual void PreFrame(){}
    virtual void PostFrame(){}
private:
    bool Init(const IEngine::InitParams &aInitParams)
    {
        return mpEngine->Init(aInitParams);
    };

    void Shutdown()
    {
        mpEngine->Shutdown();
    };

    bool Frame()
    {
        return mpEngine->Frame();
    };
private:
    IEngine *mpEngine{nullptr};
};

class CEngineExecMode_Dedicated final : public CEngineExecMode_Manual
{
public:
    CEngineExecMode_Dedicated(IEngine *apEngine) : CEngineExecMode_Manual(apEngine){}
private:
    void PreFrame() override
    {
        // Check for commands typed to the host
        Host_GetConsoleCommands();
    };

    void PostFrame() override
    {
#ifdef _WIN32		
        UpdateStatus(0);
#endif
    };
};

class CEngineExecMode_Auto final : public IEngineExecMode
{
public:
    CEngineExecMode_Auto(IEngineAPI *apEngine) : mpEngine(apEngine){}

    CEngine::Result Run(const IEngine::InitParams &aInitParams) override
    {
        // TODO
        return mpEngine->Run();
    };
private:
    IEngineAPI *mpEngine{nullptr};
};