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

#include <assert.h>
#include "./maths/Maths.h"
#include "./maths/Frustrum.h"
#include "./maths/Box.h"

namespace BogDog{
// ---------------------------------------------------------------------------

Frustrum::Frustrum(float pAspectRatio,float pFov,float pNear_z,float pFar_z)
{
	SetProjection(pAspectRatio,pFov,pNear_z,pFar_z);
}

void Frustrum::SetProjection(float pAspectRatio,float pFov,float pNear_z,float pFar_z)//Persective
{
float cotangent = 1.0f / tanf(pFov*DEGTORAD);
float q = pFar_z / (pFar_z - pNear_z);

	m[0][0] = cotangent;
	m[0][1] = 0.0f;
	m[0][2] = 0.0f;
	m[0][3] = 0.0f;

	m[1][0] = 0.0f;
	m[1][1] = pAspectRatio * cotangent;
	m[1][2] = 0.0f;
	m[1][3] = 0.0f;

	m[2][0] = 0.0f;
	m[2][1] = 0.0f;
	m[2][2] = q;
	m[2][3] = 1.0f;

	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = -q * pNear_z;
	m[3][3] = 0.0f;

	//Now rebuild the clipping planes.
	ExtractPlanes();
}

void Frustrum::SetProjectionParallel(float pWidth,float pHeight,float pNear_z,float pFar_z)//Parallel
{
float q = 1.0f / (pFar_z - pNear_z);

	m[0][0] = 2.0f / pWidth;
	m[0][1] = 0.0f;
	m[0][2] = 0.0f;
	m[0][3] = 0.0f;

	m[1][0] = 0.0f;
	m[1][1] = 2.0f / pHeight;
	m[1][2] = 0.0f;
	m[1][3] = 0.0f;

	m[2][0] = 0.0f;
	m[2][1] = 0.0f;
	m[2][2] = q;
	m[2][3] = 1.0f;

	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = -pNear_z * q;
	m[3][3] = 0.0f;

	//Now rebuild the clipping planes.
	ExtractPlanes();
}

void Frustrum::ExtractPlanes()
{
	//Left clipping plane
	left.Set(	m[0][3] + m[0][0],
				m[1][3] + m[1][0],
				m[2][3] + m[2][0],
				m[3][3] + m[3][0]);
	left.Norm();

	//Right clipping plane
	right.Set(	m[0][3] - m[0][0],
				m[1][3] - m[1][0],
				m[2][3] - m[2][0],
				m[3][3] - m[3][0]);
	right.Norm();

	//Top clipping plane
	top.Set(	m[0][3] - m[0][1],
				m[1][3] - m[1][1],
				m[2][3] - m[2][1],
				m[3][3] - m[3][1]);
	top.Norm();

	//Bottom clipping plane
	bottom.Set(	m[0][3] + m[0][1],
				m[1][3] + m[1][1],
				m[2][3] + m[2][1],
				m[3][3] + m[3][1]);
	bottom.Norm();

	//Near clipping plane
	front.Set(m[0][2],
			m[1][2],
			m[2][2],
			m[3][2]);
	front.Norm();

	//Far clipping plane
	back.Set(	m[0][3] - m[0][2],
				m[1][3] - m[1][2],
				m[2][3] - m[2][2],
				m[3][3] - m[3][2]);
	back.Norm();
}

Vector3 *Frustrum::WorldToView(Vector3 *pOut,const Vector3 *pIn,float pView_width,float pView_height)const
{
float x,y,z,w;

	x = (m[0][0] * pIn->x) + (m[1][0] * pIn->y) + (m[2][0] * pIn->z) + m[3][0];
	y = (m[0][1] * pIn->x) + (m[1][1] * pIn->y) + (m[2][1] * pIn->z) + m[3][1];
	z = (m[0][2] * pIn->x) + (m[1][2] * pIn->y) + (m[2][2] * pIn->z) + m[3][2];
	w = (m[0][3] * pIn->x) + (m[1][3] * pIn->y) + (m[2][3] * pIn->z) + m[3][3];

	w = 1.0f / w;

	pOut->x = ( 1.0f + (x * w) ) * (pView_width * 0.5f);
	pOut->y = ( 1.0f - (y * w) ) * (pView_height * 0.5f);
	pOut->z = z * w;

	return pOut;
}

int Frustrum::IsInView(const Vector3 *pPoint)const//Returns 0 if not in view, 1 if in.
{
float x,y,z,w;

	w = ((m[0][3] * pPoint->x) + (m[1][3] * pPoint->y) + (m[2][3] * pPoint->z) + m[3][3]);
	x = (m[0][0] * pPoint->x) + (m[1][0] * pPoint->y) + (m[2][0] * pPoint->z) + m[3][0];
	if( -w < x && x < w )
	{
		y = (m[0][1] * pPoint->x) + (m[1][1] * pPoint->y) + (m[2][1] * pPoint->z) + m[3][1];
		if( -w < y && y < w )
		{
			z = (m[0][2] * pPoint->x) + (m[1][2] * pPoint->y) + (m[2][2] * pPoint->z) + m[3][2];
			if( 0 < z && z < w )
				return 1;
		}
	}

	return 0;
}

int Frustrum::IsInView(const Vector3 *pPoint,float pRadius)const//Returns 0 if not in view, 1 if intersecting and 2 if fully in.
{
int ret,n;

	ret = 0;
	n = left.FrustrumSphereIntersection(*pPoint,pRadius);
	if( n )
	{
		ret += n;
		n = right.FrustrumSphereIntersection(*pPoint,pRadius);
		if( n )
		{
			ret += n;
			n = top.FrustrumSphereIntersection(*pPoint,pRadius);
			if( n )
			{
				ret += n;
				n = bottom.FrustrumSphereIntersection(*pPoint,pRadius);
				if( n )
				{
					ret += n;
					n = front.FrustrumSphereIntersection(*pPoint,pRadius);
					if( n )
					{
						ret += n;
						n = back.FrustrumSphereIntersection(*pPoint,pRadius);
						if( n )
						{
							ret += n;
							if( ret == 12 )
								return 2;
							return 1;
						}
					}
				}
			}
		}
	}

	return 0;
}

int Frustrum::IsInView(const Bounds *pBounds)const//Returns 0 if not in view, 1 if intersecting and 2 if fully in.
{
int axis;
int flag_one_is_in;
int flag_one_is_out;
Vector3 p;

	assert( pBounds );
	flag_one_is_in = 0;
	flag_one_is_out = 0;
	for( axis = 0 ; axis < 8 ; axis++ )
	{
		pBounds->GetCorner(axis,&p);
		if( IsInView(&p) )
		{
			if( flag_one_is_out )
				return 1;
			flag_one_is_in = 2;//Set to two so that if all is in I return 2.
		}
		else
		{
			if( flag_one_is_in )
				return 1;
			flag_one_is_out = 1;
		}
	}

	//If one is in then all are in, if one is out then all are out. and so flag_one_is_out should be 1 when flag_one_is_in == 1
	assert( flag_one_is_in != 2 || flag_one_is_out != 1 );//Can not happen, as would have returned in the loop.
	return flag_one_is_in;
}


// ---------------------------------------------------------------------------
};//namespace BogDog{
