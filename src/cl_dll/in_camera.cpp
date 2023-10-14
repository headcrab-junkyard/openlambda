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

void /*CL_DLLEXPORT*/ CAM_Think()
{
	//RecClCamThink();
	
	vec3_t origin;
	vec3_t ext, pnt, camForward, camRight, camUp;
	moveclip_t clip;
	float dist;
	vec3_t camAngles;
	float flSensitivity;
#ifdef LATER
	int i;
#endif
	vec3_t viewangles;
	
	switch()
	{
	};
	
	if(!cam_thirdperson)
		return;
	
#ifdef LATER
	if(cam_contain->value)
	{
		gpEngine->GetClientOrigin(origin);
		ext[0] = ext[1] = ext[2] = 0.0f;
	};
#endif
	
	camAngles[PITCH] = cam_idealpitch->value;
	camAngles[YAW] = cam_idealyaw->value;
	dist = cam_idealdist->value;
	
	// Movement of the camera with the mouse
	if(cam_mousemove)
	{
		SDL_GetCursorPos(&cam_mouse);
		
		if(!cam_distancemove)
		{
		};
	};
	
	if()
		camAngles[PITCH]
	else if()
		camAngles[PITCH]
};

int /*CL_DLLEXPORT*/ CL_IsThirdPerson()
{
	//RecClIsThirdPerson();
	
	return (cam_thirdperson ? 1 : 0) || (g_iUser1 && (g_iUser2 == gpEngine->GetLocalPlayer()->index));
};

void /*CL_DLLEXPORT*/ CL_GetCameraOffsets(float *ofs)
{
	//RecClGetCameraOffsets(ofs);
	
	//if(!ofs)
		//return;
	
	VectorCopy(cam_ofs, ofs);
};