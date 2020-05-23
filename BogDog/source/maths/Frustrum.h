/*
 *
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

#ifndef __FRUSTRUM_H__
#define __FRUSTRUM_H__

#include "Matrix.h"
#include "Plane.h"
namespace BogDog{
struct Bounds;
// ---------------------------------------------------------------------------
struct Frustrum : public Matrix
{
	Plane left,right,top,bottom,front,back;

public:
	Frustrum(){SetProjection(640.0f / 480.0f,35.0f,1.0f,100.0f);}
	Frustrum(float pAspectRatio,float pFov,float pNear_z,float pFar_z);

	void SetProjection(float pAspectRatio,float pFov,float pNear_z,float pFar_z);//Persective
	void SetProjectionParallel(float pWidth,float pHeight,float pNear_z,float pFar_z);//Parallel

	//Transforms from world to screen space.
	Vector3 *WorldToView(Vector3 *pOut,const Vector3 *pIn,float pView_width,float pView_height)const;
	int IsInView(const Vector3 *pPoint)const;//Returns 0 if not in view, 1 if in.
	int IsInView(const Vector3 *pPoint,float pRadius)const;//Returns 0 if not in view, 1 if intersecting and 2 if fully in.
	int IsInView(const Bounds *pBounds)const;//Returns 0 if not in view, 1 if intersecting and 2 if fully in.

	//Updates the planes, needed for sphere clip test, SetProjection calls this for you.
	//If you do any matrix op to the frustrum then your need to call this for the clip test code to work.
	void ExtractPlanes();
};
// ---------------------------------------------------------------------------
};//namespace BogDog{
#endif //#ifndef __FRUSTRUM_H__
