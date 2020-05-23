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
#include "./maths/Quaternion.h"
#include "./maths/Matrix.h"
#include "./maths/Maths.h"

namespace BogDog{
// ---------------------------------------------------------------------------
Quaternion::Quaternion(float pX,float pY,float pZ)
{
Matrix m;
	m.SetRotation(pX,pY,pZ);
	*this = m;
}

Quaternion::Quaternion(float pX,float pY,float pZ,float pTheta)
{
float s;

	pTheta *= DEGTORAD * 0.5f;

	s = sin(pTheta);
	w = cos(pTheta);

	x = s * pX;
	y = s * pY;
	z = s * pZ;
}


Quaternion::Quaternion(Matrix *pMatrix)
{
	*this = *pMatrix;
}

//Does this = pA * pB
Quaternion *Quaternion::Mul(const Quaternion *pA,const Quaternion *pB)
{
	w = pA->w * pB->w - pA->x * pB->x - pA->y * pB->y - pA->z * pB->z;
	x = pA->w * pB->x + pA->x * pB->w + pA->y * pB->z - pA->z * pB->y;
	y = pA->w * pB->y + pA->y * pB->w + pA->z * pB->x - pA->x * pB->z;
	z = pA->w * pB->z + pA->z * pB->w + pA->x * pB->y - pA->y * pB->x;

	return this;
}

Quaternion& Quaternion::operator = ( const Matrix& pMatrix)
{
float		tr, s;
int			i, j, k;
int			n[3] = {1,2,0};

	tr = pMatrix.m[0][0] + pMatrix.m[1][1] + pMatrix.m[2][2];

	if (tr >= 0)
	{
		s = sqrtf( tr + 1 );
		w = s / 2;

		s = 0.5F / s;

		x = (pMatrix.m[1][2] - pMatrix.m[2][1]) * s;
		y = (pMatrix.m[2][0] - pMatrix.m[0][2]) * s;
		z = (pMatrix.m[0][1] - pMatrix.m[1][0]) * s;
	}
	else
	{
		i = 0;
		if(pMatrix.m[1][1] > pMatrix.m[0][0])
			i = 1;
		if(pMatrix.m[2][2] > pMatrix.m[i][i])
			i = 2;

		j = n[i];
		k = n[j];

		s = sqrtf(pMatrix.m[i][i] - (pMatrix.m[j][j] + pMatrix.m[k][k]) + 1);
		q[i] = s / 2;

		s = 0.5F / s;

		w  = (pMatrix.m[j][k] - pMatrix.m[k][j]) * s;
		q[j] = (pMatrix.m[i][j] + pMatrix.m[j][i]) * s;
		q[k] = (pMatrix.m[i][k] + pMatrix.m[k][i]) * s;
	}

	return *this;
}

//Returns this.
//Does this = this * pIn
Quaternion *Quaternion::Mul(const Quaternion *pIn)
{
Quaternion t;

	t.Mul(this,pIn);

	x = t.x;
	y = t.y;
	z = t.z;
	w = t.w;

	return this;
}

//Does this = pIn * this
Quaternion *Quaternion::MulPre(const Quaternion *pIn)
{
Quaternion t;

	t.Mul(pIn,this);

	x = t.x;
	y = t.y;
	z = t.z;
	w = t.w;

	return this;
}

float Quaternion::Dot(const Quaternion *pQ)const
{
	return (x * pQ->x) + (y * pQ->y) + (z * pQ->z) + (w * pQ->w);
}

//The cool stuff, interpolulation.
void Quaternion::Slerp(Quaternion *pA,float pFraction)
{
Quaternion t;

	t.Slerp(this,pA,pFraction);
	*this = t;
}

void Quaternion::Slerp(Quaternion *pA,Quaternion *pB,float pFraction)
{
float omega,s_omega,c_omega,scale_l,scale_r;

    assert( pA != NULL );
    assert( pB != NULL );


	c_omega = pA->Dot(pB);

	// Take shortest route
	if (c_omega < 0)
	{
		x = -pB->x;
		y = -pB->y;
		z = -pB->z;
		w = -pB->w;
		c_omega = -c_omega;
	}
	else
	{
		x = pB->x;
		y = pB->y;
		z = pB->z;
		w = pB->w;
	}

	if (1 - c_omega > 1E-5F)
	{
		// normal case
		omega = acosf(c_omega);

		s_omega = 1.0f/sinf( omega );
		scale_l = sinf( (1.0f - pFraction) * omega ) * s_omega;
		scale_r = sinf( pFraction * omega) * s_omega;
	}
	else
	{
		// Ends very close, use small angle approx
		scale_l = 1 - pFraction*c_omega;
		scale_r = pFraction;
	}

	x = scale_l * pA->x + scale_r * x;
	y = scale_l * pA->y + scale_r * y;
	z = scale_l * pA->z + scale_r * z;
	w = scale_l * pA->w + scale_r * w;
}

//Set funtions
void Quaternion::SetRotationX(float pX)
{
	pX *= DEGTORAD * 0.5f;

	x = sin(pX);
	y = 0;
	z = 0;
	w = cos(pX);
}

void Quaternion::SetRotationY(float pY)
{
	pY *= DEGTORAD * 0.5f;

	x = 0;
	y = sin(pY);
	z = 0;
	w = cos(pY);
}

void Quaternion::SetRotationZ(float pZ)
{
	pZ *= DEGTORAD * 0.5f;

	x = 0;
	y = 0;
	z = sin(pZ);
	w = cos(pZ);
}

//Rotation stuff.
void Quaternion::ApplyRotationXPost(float pX)
{
Quaternion t;

	t.SetRotationX(pX);

	Mul(&t);
}

void Quaternion::ApplyRotationYPost(float pY)
{
Quaternion t;

	t.SetRotationY(pY);

	Mul(&t);
}

void Quaternion::ApplyRotationZPost(float pZ)
{
Quaternion t;

	t.SetRotationZ(pZ);

	Mul(&t);
}

void Quaternion::ApplyRotationXPre(float pX)
{
Quaternion t;

	t.SetRotationX(pX);

	MulPre(&t);
}

void Quaternion::ApplyRotationYPre(float pY)
{
Quaternion t;

	t.SetRotationY(pY);

	MulPre(&t);
}

void Quaternion::ApplyRotationZPre(float pZ)
{
Quaternion t;

	t.SetRotationZ(pZ);

	MulPre(&t);
}

// ---------------------------------------------------------------------------
};//namespace BogDog{
