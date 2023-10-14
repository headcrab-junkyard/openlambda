/*
 * This file is part of OpenLambda Project
 * Copyright (C) 2018-2021, 2023 BlackPhrase
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

void /*CL_DLLEXPORT*/ HUD_PostRunCmd(struct local_state_s *from, struct local_state_s *to, struct usercmd_s *cmd, int runfuncs, double time, unsigned int random_seed)
{
	//RecClPostRunCmd(from, to, cmd, runfuncs, time, random_seed);
	
	g_runfuncs = runfuncs;
	
#ifdef CLIENT_WEAPONS
	if(cl_lw && cl_lw->value)
		HUD_WeaponsPostThink(from, to, cd, time, random_seed);
	else
#endif
		to->client.fov = g_lastFOV;
	
	if(g_irunninggausspred == 1)
	{
		idVec3 vForward{};
		gpEngine->pfnAngleVectors(v_angles, vForward, nullptr, nullptr);
		to->client.velocity = to->client.velocity - vForward * g_flApplyVel * 5;
		g_irunninggausspred = false;
	};
	
	// All games can use FOV state
	g_lastFOV = to->client.fov;
};