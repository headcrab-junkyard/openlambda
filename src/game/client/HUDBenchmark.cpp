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

#include "HUDBenchmark.hpp"

int CHUDBenchmark::Init()
{
	return 0;
};

int CHUDBenchmark::VidInit()
{
	return 0;
};

int CHUDBenchmark::Draw(float afTime)
{
	return 0;
};

void CHUDBenchmark::Think()
{
};

void CHUDBenchmark::SetScore(float afScore)
{
};

void CHUDBenchmark::StartNextSection(int anSection)
{
};

int CHUDBenchmark::MsgFunc_Bench(const char *asName, int anSize, void *apBuf)
{
	return 0;
};

void CHUDBenchmark::CountFrame(float afDeltaTime)
{
};

void CHUDBenchmark::SetCompositeScore()
{
};

void CHUDBenchmark::Restart()
{
};

int CHUDBenchmark::Bench_ScoreForValue(int anStage, float afRawValue)
{
	return 0;
};