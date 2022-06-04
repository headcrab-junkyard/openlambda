/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2021-2022 BlackPhrase
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

#include "BaseAnimated.hpp"

int CBaseAnimated::Save(CSave &aSave){return 1;}
int CBaseAnimated::Restore(CRestore &aRestore){return 1;}

int CBaseAnimated::LookupActivity(int anActivity){return 0;}
int CBaseAnimated::LookupActivityHeaviest(int anActivity){return 0;}

void CBaseAnimated::SetBodygroup(int anGroup, int anValue){}
int CBaseAnimated::GetBodygroup(int anGroup) const {return 0;}

int CBaseAnimated::LookupSequence(const char *asLabel){return 0;}

void CBaseAnimated::ResetSequenceInfo(){}

bool CBaseAnimated::GetSequenceFlags() const {return false;}

void CBaseAnimated::DispatchAnimEvents(float afInterval){}