/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2019, 2021 BlackPhrase
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
/// @brief generic delay entity

#include "BaseEntity.hpp"

class CBaseDelay : public CBaseEntity
{
public:
	/*virtual*/ bool HandleKeyValue(const std::string &asKey, const std::string &asValue) override;
	
	// /*virtual*/ int Save(CSaveData &aSaveData);
	// /*virtual*/ int Restore(CRestoreData &aRestoreData);
	
	void DelayThink();
	
	void SUB_UseTargets(CBaseEntity *apActivator, UseType aeUseType, float afValue);
public: // TODO: private:
	float mfDelay{0.0f};
	
	int mnKillTargetString{-1};
};