/*
 * This file is part of OpenLambda Project
 *
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

#include "IEngineHooks.hpp"

enum class CEngine::Result;

struct IEngine::InitParams;

interface IEngine;
interface IEngineAPI;

interface IEngineExecMode
{
    ///
    virtual CEngine::Result Run(const IEngine::InitParams &aInitParams) = 0;
};

class CEngineExecMode_Manual final : public IEngineExecMode
{
public:
    CEngineExecMode_Manual(IEngine *apEngine, IEngineHooks *apHooks) : mpEngine(apEngine), mpHooks(apHooks){}

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
private:
    bool Init(const IEngine::InitParams &aInitParams)
    {
        return mpEngine->Init(aInitParams);
    };

    void Shutdown()
    {
        mpEngine->Shutdown();
    };

    void PreFrame()
    {
        if(mpHooks)
            mpHooks->PreFrame();
    };

    bool Frame()
    {
        return mpEngine->Frame();
    };

    void PostFrame()
    {
        if(mpHooks)
            mpHooks->PostFrame();
    };
private:
    IEngine *mpEngine{nullptr};
    IEngineHooks *mpHooks{nullptr};
};

class CEngineExecMode_Auto final : public IEngineExecMode
{
public:
    CEngineExecMode_Auto(IEngineAPI *apEngine) : mpEngine(apEngine){}

    CEngine::Result Run(const IEngine::InitParams &aInitParams) override
    {
        //void *instance, const char *basedir, const char *cmdline, char *postRestartCmdLineArgs, CreateInterfaceFn launcherFactory, CreateInterfaceFn filesystemFactory
        // TODO: hinstance support?
        return mpEngine->Run(nullptr, aInitParams.sGameDir, aInitParams.sCmdLine, aInitParams.sPostRestartCmdLine, aInitParams.fnLauncherFactory, mfnFSFactory);
    };
private:
    IEngineAPI *mpEngine{nullptr};
};