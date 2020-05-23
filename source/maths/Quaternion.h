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


#ifndef __QUATERNION_H__
#define __QUATERNION_H__


#include "Vector3.h"

namespace BogDog{

struct Matrix;
// ---------------------------------------------------------------------------
struct Quaternion
{
public:
	union
	{
		struct
		{
			float x,y,z,w;
		};
		float q[4];
	};

	Quaternion(){}
	Quaternion(float pX,float pY,float pZ);
	Quaternion(float pX,float pY,float pZ,float pTheta);
	Quaternion(Vector3 *pAxis,float pTheta);
	Quaternion(Matrix *pMatrix);

	~Quaternion(){};

	//Does this = pA * pB
	Quaternion *Mul(const Quaternion *pA,const Quaternion *pB);

	//Returns this.
	//Does this = this * pIn
	Quaternion *Mul(const Quaternion *pIn);
	//Does this = pIn * this
	Quaternion *MulPre(const Quaternion *pIn);

	float Dot(const Quaternion *pQ)const;

	//The cool stuff, interpolulation.
	void Slerp(Quaternion *pA,float pFraction);	//this = 'this Slerp pA' using a temp var.
	void Slerp(Quaternion *pA,Quaternion *pB,float pFraction); //this = pA Slerp pB

	//Copies.
	Quaternion& operator = ( const Matrix& pMatrix);
	Quaternion& operator = ( const Quaternion& pQuat);


	//Set funtions
	void SetIdentity();
	void SetRotationX(float pX);
	void SetRotationY(float pY);
	void SetRotationZ(float pZ);

	//Rotation stuff.
	void ApplyRotationXPost(float pX);
	void ApplyRotationYPost(float pY);
	void ApplyRotationZPost(float pZ);
	void ApplyRotationXPre(float pX);
	void ApplyRotationYPre(float pY);
	void ApplyRotationZPre(float pZ);
};

//Inline code, only inline stuff that is one or two lines, anymore and it would start to bloat the exec killing code cache.

//Constructors.
inline Quaternion::Quaternion(Vector3 *pAxis,float pTheta)
{
	Quaternion(pAxis->x,pAxis->y,pAxis->z,pTheta);
}

inline Quaternion& Quaternion::operator = ( const Quaternion& pQuat)
{
	x = pQuat.x;
	y = pQuat.y;
	z = pQuat.z;
	w = pQuat.w;
	return *this;
}

//Set funtions
inline void Quaternion::SetIdentity()
{
	x = 0;
	y = 0;
	z = 0;
	w = 1;
}
// ---------------------------------------------------------------------------
};//namespace BogDog{
#endif //#ifndef __QUATERNION_H__
