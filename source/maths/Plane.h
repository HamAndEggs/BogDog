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

#ifndef __PLANE_H__
#define __MATRIX_H__

#include <float.h>
#include "Vector3.h"

namespace BogDog{
// ---------------------------------------------------------------------------
struct Plane : public Vector3
{
	float d;
	//Constructors
	Plane(){};//Default construtor, so if you alloc a big array you don't get loads of fmov's of zero to x,y,z.
	Plane(float __x,float __y,float __z,float __d){Set(__x,__y,__z,__d);}
	Plane(const Vector3 &pNorm,float __d);
	Plane(const Vector3 &pA,const Vector3 &pB,const Vector3 &pC){Set(pA,pB,pC);}//Make the plane form the passed triangle.

	Plane& operator = ( const Plane &pSource )
	{
		x = pSource.x;
		y = pSource.y;
		z = pSource.z;
		return *this;
	}

	void Norm();//Muls x,y,z and d buy (1/norm.Length());

	//Get funtions.
	inline int GetMinAxis();//Returns the smallest axis.

	//set funtions.
	inline void Set(float __x,float __y,float __z,float __d);
	inline float Set(const Vector3 &pA,const Vector3 &pB,const Vector3 &pC);
	inline void Set(const Vector3 &pPoint,const Vector3 &pNorm);


	//Ray casting.0
	//Returns pHitPoint if the ray hits the plane, else returns NULL.
	bool RayCast(const Vector3 &pPos,const Vector3 &pDir,Vector3 &rHitPoint,float *rDist = NULL);
	Vector3 &RayCastDoubleSided(const Vector3 &pPos,const Vector3 &pDir,Vector3 &rHitPoint,float *rDist = NULL);
	int SphereIntersection(const Vector3 &pPos,const float pRadius,Vector3 &rHitPoint,float *rDist = NULL);
	//Returns 0 if on negative side of the plane, 1 if intersects plane or 2 if on positive size and so in this halfspace of the frustrum.
	int FrustrumSphereIntersection(const Vector3 &pPos,float pRadius) const;
};

//Constructors

inline Plane::Plane(const Vector3 &pNorm,float __d)
{
	assert( pNorm.IsValid() );
	assert( pNorm.LengthSq() <= 1.0f );//Make sure its normalised.
	assert( pNorm.LengthSq() > FLT_EPSILON );//Make sure its not really small.

	x = pNorm.x;
	y = pNorm.y;
	z = pNorm.z;
	d = __d;
}

//This func is used when doing point in tri claculations in barycentric coord space.
inline int Plane::GetMinAxis()
{
	if( fabsf(y) > fabsf(x) )
	{
		if( fabsf(y) > fabsf(z) )
		{
			return 1;
		}
		return 2;
	}
	else if( fabsf(z) > fabsf(x) )
	{
		return 2;
	}
	return 0;
}

inline void Plane::Set(float __x,float __y,float __z,float __d)
{
	x = __x;
	y = __y;
	z = __z;
	d = __d;
}

//Make the plane form the passed triangle.
inline float Plane::Set(const Vector3 &pA,const Vector3 &pB,const Vector3 &pC)
{
	//Make the
	float r = Vector3::Norm(pA,pB,pC);

	//Make the dist from the origin.
	d = -Dot(pA);

	return r;
}

inline void Plane::Set(const Vector3 &pPoint,const Vector3 &pNorm)
{
	assert( pPoint.IsValid() );
	assert( pNorm.IsValid() );
	x = pNorm.x;
	x = pNorm.y;
	x = pNorm.z;
	d = -Dot(pPoint);
}
// ---------------------------------------------------------------------------
};//namespace BogDog{
#endif//#ifndef __MATRIX_H__
