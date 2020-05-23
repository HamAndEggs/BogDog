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
#include <stddef.h>
#include "./maths/Plane.h"

namespace BogDog{
// ---------------------------------------------------------------------------
void Plane::Norm()//Muls x,y,z and d buy (1/norm.Length());
{
float mag = 1.0f / Length();

	x *= mag;
	y *= mag;
	z *= mag;
	d *= mag;
}

bool Plane::RayCast(const Vector3 &pPos,const Vector3 &pDir,Vector3 &rHitPoint,float *rDist/*= NULL*/)
{
float t,norm_dot_dir;

	//See if we intersect with the plane.
	assert( pPos.IsValid() );
	assert( pDir.IsValid() );
	assert( rHitPoint.IsNotNULL() );

	//If T >=0 then we are faceing away from the plane, that is we are in the same direction.
	norm_dot_dir = Dot(pDir);
	if( norm_dot_dir >= 0 )
		return false;

	//We are pointing towards the plane, so get the intersection point.
	t = -(Dot(pPos)+d);
	if( t > 0 )//If t > 0 then pPos nearer to origen and so can't be intersecting the plane.
		return false;

	t /= norm_dot_dir;

	//Now get our hit point.
	rHitPoint.x = pPos.x + (pDir.x * t);
	rHitPoint.y = pPos.y + (pDir.y * t);
	rHitPoint.z = pPos.z + (pDir.z * t);

	if( rDist )
		*rDist = t;

	return true;
}

Vector3 &Plane::RayCastDoubleSided(const Vector3 &pPos,const Vector3 &pDir,Vector3 &rHitPoint,float *rDist/*= NULL*/)
{
float t,norm_dot_dir;

	//See if we intersect with the plane.
	assert( pPos.IsValid() );
	assert( pDir.IsValid() );
	assert( rHitPoint.IsNotNULL() );

	//If T >=0 then we are faceing away from the plane, that is we are in the same direction.
	norm_dot_dir = Dot(pDir);

	//We are pointing towards the plane, so get the intersection point.
	t = -(Dot(pPos)+d);

	t /= norm_dot_dir;

	//Now get our hit point.
	rHitPoint.AddScale(pPos,pDir,t);

	if( rDist )
		*rDist = t;

	return rHitPoint;
}

int Plane::SphereIntersection(const Vector3 &pPos,const float pRadius,Vector3 &rHitPoint,float *rDist/*= NULL*/)
{
float t;

	//See if we intersect with the plane.
	assert( pPos.IsValid() );

	t = Dot(pPos)+d;
	if( rDist )
		*rDist = t;

	if( t >= -pRadius && t <= pRadius )
	{
		//Now get our hit point.
		rHitPoint.x = pPos.x + (x * t);
		rHitPoint.y = pPos.y + (y * t);
		rHitPoint.z = pPos.z + (z * t);
		return 1;
	}

	return 0;
}

//Returns 0 if on negative side of the plane, 1 if intersects plane or 2 if on positive size and so in this halfspace of the frustrum.
int Plane::FrustrumSphereIntersection(const Vector3 &pPos,float pRadius)const
{
float distance;

	assert( pPos.IsValid() );

	distance = Dot(pPos) + d;

	if( distance < -pRadius )
		return 0;

	if( fabsf(distance) < pRadius )
		return 1;

	return 2;
}

// ---------------------------------------------------------------------------
};//namespace BogDog{
