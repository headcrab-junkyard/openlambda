/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2019-2022 BlackPhrase
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

#include "next/game/server/IGame.hpp"

interface IFileSystem;
interface IVoiceServer;
interface IPhysicsSystem;
interface IScriptSystem;

//interface IGameSetup;
interface IGameRules;

class IGameWorld;

class CBaseGame /*final*/ : public IGame
{
public:
	CBaseGame();
	//CBaseGame(IGameRules *apRules) : mpRules(apRules){}
	//CBaseGame(IGameRules *apRules, IGameWorld *apWorld);
	//~CBaseGame();
public: // IGame interface impl
	bool Init(CreateInterfaceFn afnEngineFactory) override;
	void Shutdown() override;
	
	void Frame(double afFrameTime) override;
public: // Public methods
	virtual void InstallRules() = 0;
	
	//void SetRules(IGameRules *apRules){mpRules = apRules;}
	IGameRules *GetRules() const {return mpRules;}
	
	IGameWorld *GetWorld() const {return mpWorld;}
protected:
	virtual void RegisterEvents(){}
private:
	IFileSystem *mpFileSystem{nullptr};
	IVoiceServer *mpVoiceServer{nullptr};
	IPhysicsSystem *mpPhysics{nullptr};
	IScriptSystem *mpScript{nullptr};
	
	IGameRules *mpRules{nullptr};
	IGameWorld *mpWorld{nullptr};
	//CBasePlayer *GetPlayer(int anIndex) const; // TODO
	
	unsigned long framecount{0};
};

extern CBaseGame *gpGame; // BP: oof...