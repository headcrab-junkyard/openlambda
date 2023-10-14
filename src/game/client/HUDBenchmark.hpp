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

#include "HUDElement.hpp"

class CHUDBenchmark : public CHUDElement
{
public:
	int Init();
	int VidInit();
	
	int Draw(float afTime);
public:
	void Think();
	
	void SetScore(float afScore);
	
	void StartNextSection(int anSection);
	
	int MsgFunc_Bench(const char *asName, int anSize, void *apBuf);
	
	void CountFrame(float afDeltaTime);
	
	int GetObjects() const {return mnObjects;}
	
	void SetCompositeScore();
	
	void Restart();
	
	int Bench_ScoreForValue(int anStage, float afRawValue);
private:
	float mfDrawTime{0.0f};
	float mnDrawScore{0.0f};
	float mfAvgScore{0.0f};
	
	float mfSendTime{0.0f};
	float mfReceiveTime{0.0f};
	
	float mfAvgFrameTime{0.0f};
	float mfAvgFrameRate{0.0f};
	
	float mfStageStarted{0.0f};
	
	float mfStoredLatency{0.0f};
	float mfStoredPacketLoss{0.0f};
	
	int mnFPSCount{0};
	
	int mnSentFinish{0};
	
	int mnStoredHopCount{0};
	int mnTraceDone{0};
	
	int mnObjects{0};
	
	int mnScoreComputed{0};
	int mnCompositeScore{0};
};