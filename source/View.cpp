/*
 * View.cpp
 *
 *  Created on: 9 Jun 2012
 *
 *  BogDog GLES 2.0 3D Engine for Raspberry Pi
 *	Copyright (C) 2012  Richard e Collins
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 * 	You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "View.h"
#include "Common.h"

namespace BogDog
{

View::View()
{
}

void View::SetFrustum(float pFov, float pAspect, float pNear, float pFar)
{/*
	float tan_fov = (float)tan( (double)(pFov*0.5f*(float)BD_DEG2RAD));
	float cotangent = 1.0f / (tan_fov);
	float q = pFar / (pFar - pNear);

	m_projection[0]  = cotangent / pAspect;
	m_projection[1]  = 0;
	m_projection[2]  = 0;
	m_projection[3]  = 0;

	m_projection[4]  = 0;
	m_projection[5]  = cotangent;
	m_projection[6]  = 0;
	m_projection[7]  = 0;

	m_projection[8]  = 0;
	m_projection[9]  = 0;
	m_projection[10] = q;
	m_projection[11] = 1.0f;

	m_projection[12] = 0;
	m_projection[13] = 0;
	m_projection[14] = -q * pNear;
	m_projection[15] = 0;*/
	m_projection.SetProjection(pAspect,pFov,pNear,pFar);
}

void View::SetCamera(float x,float y,float z, Angle pitch, Angle yaw, Angle roll)
{
	m_camera.SetRotation(pitch,yaw,roll);
	m_camera[3].Set(x,y,z);

	m_invCam.InvertLP(m_camera);
}


const Matrix& View::GetProjectionCameraMatrix()
{
	m_cameraProjection.Mul(m_invCam,m_projection);

	return m_cameraProjection;
}

} /* namespace BogDog */
