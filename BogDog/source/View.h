/*
 * View.h
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

#ifndef VIEW_H_
#define VIEW_H_

#include "maths/Matrix.h"
#include "maths/Frustrum.h"
#include "maths/SinCos.h"

namespace BogDog
{

struct View
{

	View();

	// Frustum
	/*!
		Set the viewing frustum for transformation from 3D to 2D.
		@param	pFov	Field of view, in degrees.
		@param	pAspect	Aspect ratio of frustum..
		@param	pNear	Distance to near clip plane
		@param	pFar	Distance to far clip plane.
	*/
	void SetFrustum(float pFov, float pAspect, float pNear, float pFar);

	void SetCamera(float x,float y,float z, Angle pitch, Angle yaw, Angle roll);
	void SetCamera(Vector3& pos, Angle pitch, Angle yaw, Angle roll)
	{
		SetCamera(pos.x,pos.y,pos.z,pitch,yaw,roll);
	}

	/*!
	 * Transform a point from world space to view space.
	 */
	const Matrix& GetProjectionCameraMatrix();


private:
	Matrix m_cameraProjection;
	Frustrum m_projection;
	Matrix m_camera;				//!<Camera in world space before inversion.
	Matrix m_invCam;					//!<Camera after inversion.


};

} /* namespace BogDog */
#endif /* VIEW_H_ */
