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
#include <float.h>
#include "./maths/Matrix.h"
#include "./maths/Quaternion.h"
#include "./maths/Maths.h"

namespace BogDog{
// ---------------------------------------------------------------------------


Matrix Matrix::identity(1,0,0,0,	0,1,0,0,	0,0,1,0,	0,0,0,1);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
Matrix::Matrix(float _00,float _01,float _02,float _03,
				 float _10,float _11,float _12,float _13,
				 float _20,float _21,float _22,float _23,
				 float _30,float _31,float _32,float _33)
{
	m[0][0] = _00;	m[0][1] = _01;	m[0][2] = _02;	m[0][3] = _03;
	m[1][0] = _10;	m[1][1] = _11;	m[1][2] = _12;	m[1][3] = _13;
	m[2][0] = _20;	m[2][1] = _21;	m[2][2] = _22;	m[2][3] = _23;
	m[3][0] = _30;	m[3][1] = _31;	m[3][2] = _32;	m[3][3] = _33;
}

Matrix &Matrix::Set(const Matrix &pSource)
{
	assert( pSource.IsValid() );
	for( int i = 0 ; i < 16 ; ((float*)m)[i] = ((float*)pSource.m)[i] , i++);
	return *this;
}

Matrix &Matrix::Set(const float pSource[4][3])
{
	m[0][0] = pSource[0][0];
	m[0][1] = pSource[0][1];
	m[0][2] = pSource[0][2];
	m[0][3] = 0.0f;

	m[1][0] = pSource[1][0];
	m[1][1] = pSource[1][1];
	m[1][2] = pSource[1][2];
	m[1][3] = 0.0f;

	m[2][0] = pSource[2][0];
	m[2][1] = pSource[2][1];
	m[2][2] = pSource[2][2];
	m[2][3] = 0.0f;

	m[3][0] = pSource[3][0];
	m[3][1] = pSource[3][1];
	m[3][2] = pSource[3][2];
	m[3][3] = 1.0f;

	return *this;
}

//pSource is asumed to be float[4][3], pIs_zup_rhs is for 3DSMax/OGl style matrices.
Matrix &Matrix::Set(const float *pSource,int pIs_zup_rhs)
{
	if( pIs_zup_rhs )
	{
		m[0][0] = pSource[0];
		m[0][1] = pSource[2];
		m[0][2] = pSource[1];
		m[0][3] = 0.0f;
		pSource += 3;

		//NOTE switched MAX Z-up RHS to D3D Z-front LHS by swapping row 1 and 2 and swapping y and z.
		m[2][0] = pSource[0];
		m[2][1] = pSource[2];
		m[2][2] = pSource[1];
		m[2][3] = 0.0f;
		pSource += 3;

		//And the final switch.
		m[1][0] = pSource[0];
		m[1][1] = pSource[2];
		m[1][2] = pSource[1];
		m[1][3] = 0.0f;
		pSource += 3;

		m[3][0] = pSource[0];
		m[3][1] = pSource[2];
		m[3][2] = pSource[1];
		m[3][3] = 1.0f;
		pSource += 3;
	}
	else
	{
		m[0][0] = pSource[0];
		m[0][1] = pSource[1];
		m[0][2] = pSource[2];
		m[0][3] = 0.0f;
		pSource += 3;

		m[1][0] = pSource[0];
		m[1][1] = pSource[1];
		m[1][2] = pSource[2];
		m[1][3] = 0.0f;
		pSource += 3;

		m[2][0] = pSource[0];
		m[2][1] = pSource[1];
		m[2][2] = pSource[2];
		m[2][3] = 0.0f;
		pSource += 3;

		m[3][0] = pSource[0];
		m[3][1] = pSource[1];
		m[3][2] = pSource[2];
		m[3][3] = 1.0f;
		pSource += 3;
	}

	return *this;
}


Matrix &Matrix::Set(float pX,float pY,float pZ)
{
	m[0][0] = 1.0f;
	m[0][1] = 0.0f;
	m[0][2] = 0.0f;
	m[0][3] = 0.0f;

	m[1][0] = 0.0f;
	m[1][1] = 1.0f;
	m[1][2] = 0.0f;
	m[1][3] = 0.0f;

	m[2][0] = 0.0f;
	m[2][1] = 0.0f;
	m[2][2] = 1.0f;
	m[2][3] = 0.0f;

	m[3][0] = pX;
	m[3][1] = pY;
	m[3][2] = pZ;
	m[3][3] = 1.0f;
	return *this;
}

void Matrix::SetIdentity()
{
	m[0][0] = 1.0f;
	m[0][1] = 0.0f;
	m[0][2] = 0.0f;
	m[0][3] = 0.0f;

	m[1][0] = 0.0f;
	m[1][1] = 1.0f;
	m[1][2] = 0.0f;
	m[1][3] = 0.0f;

	m[2][0] = 0.0f;
	m[2][1] = 0.0f;
	m[2][2] = 1.0f;
	m[2][3] = 0.0f;

	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = 0.0f;
	m[3][3] = 1.0f;

}

void Matrix::SetScale(float pX,float pY,float pZ)
{
	m[0][0] = pX;
	m[0][1] = 0.0f;
	m[0][2] = 0.0f;
	m[0][3] = 0.0f;

	m[1][0] = 0.0f;
	m[1][1] = pY;
	m[1][2] = 0.0f;
	m[1][3] = 0.0f;

	m[2][0] = 0.0f;
	m[2][1] = 0.0f;
	m[2][2] = pZ;
	m[2][3] = 0.0f;

	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = 0.0f;
	m[3][3] = 1.0f;
}

void Matrix::SetRotation(Angle pitch,Angle yaw,Angle roll)
{
/*
float sinX, cosX, sinY, cosY, sinZ, cosZ;

	pX *= DEGTORAD;
	pY *= DEGTORAD;
	pZ *= DEGTORAD;

	sinX = sinf(pX);
	cosX = cosf(pX);

	sinY = sinf(pY);
	cosY = cosf(pY);

	sinZ = sinf(pZ);
	cosZ = cosf(pZ);


	m[0][0] = cosX * cosZ + sinX * sinY * sinZ;
	m[0][1] = sinZ * cosY;
	m[0][2] = cosX * sinY * sinZ - sinX * cosZ;
	m[0][3] = 0.0f;

	m[1][0] = sinX * sinY * cosZ - cosX * sinZ;
	m[1][1] = cosZ * cosY;
	m[1][2] = sinZ * sinX + cosZ * cosX * sinY;
	m[1][3] = 0.0f;

	m[2][0] = cosY * sinX;
	m[2][1] = -sinY;
	m[2][2] = cosY * cosX;
	m[2][3] = 0.0f;

	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = 0.0f;
	m[3][3] = 1.0f;
*/
	SetRotationX(pitch);
	ApplyRotationYPost(yaw);
	ApplyRotationZPost(roll);

}

void Matrix::SetRotation(const Quaternion &pQuat)
{
//	D3DXMatrixRotationQuaternion((D3DXMATRIX*)m,(D3DXQUATERNION*)pQuat.q);
}

void Matrix::SetRotationX(Angle pitch)
{
	float sinX = Sin(pitch);
	float cosX = Cos(pitch);

	m[0][0] = 1.0f;
	m[0][1] = 0.0f;
	m[0][2] = 0.0f;
	m[0][3] = 0.0f;

	m[1][0] = 0.0f;
	m[1][1] = cosX;
	m[1][2] = sinX;
	m[1][3] = 0.0f;

	m[2][0] = 0.0f;
	m[2][1] = -sinX;
	m[2][2] = cosX;
	m[2][3] = 0.0f;

	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = 0.0f;
	m[3][3] = 1.0f;
}

void Matrix::SetRotationY(Angle yaw)
{
	float sinY = Sin(yaw);
	float cosY = Cos(yaw);

	m[0][0] = cosY;
	m[0][1] = 0.0f;
	m[0][2] = -sinY;
	m[0][3] = 0.0f;

	m[1][0] = 0.0f;
	m[1][1] = 1.0f;
	m[1][2] = 0.0f;
	m[1][3] = 0.0f;

	m[2][0] = sinY;
	m[2][1] = 0.0f;
	m[2][2] = cosY;
	m[2][3] = 0.0f;

	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = 0.0f;
	m[3][3] = 1.0f;

}

void Matrix::SetRotationZ(Angle roll)
{
	float sinZ = Sin(roll);
	float cosZ = Cos(roll);

	m[0][0] = cosZ;
	m[0][1] = sinZ;
	m[0][2] = 0.0f;
	m[0][3] = 0.0f;

	m[1][0] = -sinZ;
	m[1][1] = cosZ;
	m[1][2] = 0.0f;
	m[1][3] = 0.0f;

	m[2][0] = 0.0f;
	m[2][1] = 0.0f;
	m[2][2] = 1.0f;
	m[2][3] = 0.0f;

	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = 0.0f;
	m[3][3] = 1.0f;

}

void Matrix::SetZYOrientation(const Vector3 &pLook,const Vector3 &pUp,int pClear_transform/*= 1*/)
{
Vector3	Temp,TempY, look_norm;
float Dot;

	assert( pLook.IsValid() );
	assert( pUp.IsValid() );

	look_norm.Norm(pLook);

	Dot = look_norm.Dot(pUp);

	TempY.x = pUp.x - (Dot * look_norm.x);
	TempY.y = pUp.y - (Dot * look_norm.y);
	TempY.z = pUp.z - (Dot * look_norm.z);

	Dot = TempY.LengthSq();
	if( Dot > FLT_EPSILON && Dot < FLT_EPSILON )
	{
		Temp.RotateX(&look_norm,90.0f);
		Dot = Temp.Dot(&look_norm);
		TempY.x = Temp.x - (Dot * look_norm.x);
		TempY.y = Temp.y - (Dot * look_norm.y);
		TempY.z = Temp.z - (Dot * look_norm.z);
	}

	Temp.Norm(TempY);

	m[2][0] = look_norm.x;
	m[2][1] = look_norm.y;
	m[2][2] = look_norm.z;
	m[2][3] = 0;

	m[1][0] = Temp.x;
	m[1][1] = Temp.y;
	m[1][2] = Temp.z;
	m[1][3] = 0;

	TempY.Cross(Temp,look_norm);
	m[0][0] = TempY.x;
	m[0][1] = TempY.y;
	m[0][2] = TempY.z;
	m[0][3] = 0;

	if( pClear_transform )
	{
		m[3][0] = 0;
		m[3][1] = 0;
		m[3][2] = 0;
		m[3][3] = 1;
	}
}

void Matrix::SetZYLookAt(const Vector3 &pFrom,const Vector3 &pTo)
{
Vector3 dir,up(0,1,0);

	assert( pFrom.IsValid() && pTo.IsValid() );

	dir = pTo - pFrom;
	SetZYOrientation(dir,up);
	SetAxis(3,pFrom);
}

void Matrix::SetAxisFromCrossProduct(int pDest_axis,int pAxis_a,int pAxis_b)
{
	assert( pDest_axis > -1 && pDest_axis < 4 );
	assert( pAxis_a > -1 && pAxis_a < 4 );
	assert( pAxis_b > -1 && pAxis_b < 4 );

	m[pDest_axis][0] = (m[pAxis_a][1] * m[pAxis_b][2]) - (m[pAxis_a][2] * m[pAxis_b][1]);
	m[pDest_axis][1] = (m[pAxis_a][2] * m[pAxis_b][0]) - (m[pAxis_a][0] * m[pAxis_b][2]);
	m[pDest_axis][2] = (m[pAxis_a][0] * m[pAxis_b][1]) - (m[pAxis_a][1] * m[pAxis_b][0]);
	if( pDest_axis == 3 )
		m[pDest_axis][3] = 1;
	else
		m[pDest_axis][3] = 0;
}

//Makes a matrix that then when mul'd with a vector of (t*t*t,t*t,t) gives a point on a curve from pB to pC.
void Matrix::SetCatmullRomCoefficients(const Vector3 *pA,const Vector3 *pB,const Vector3 *pC,const Vector3 *pD)
{
Vector3 in,out;

	out.x = (pC->x - pA->x) * 0.5f;
	out.y = (pC->y - pA->y) * 0.5f;
	out.z = (pC->z - pA->z) * 0.5f;

	in.x = (pD->x - pB->x) * 0.5f;
	in.y = (pD->y - pB->y) * 0.5f;
	in.z = (pD->z - pB->z) * 0.5f;

	//Now make the coefficents matrix.
	m[0][0] = (2.0f*pB->x) + (-2.0f*pC->x) + (out.x) + (in.x);
	m[0][1] = (2.0f*pB->y) + (-2.0f*pC->y) + (out.y) + (in.y);
	m[0][2] = (2.0f*pB->z) + (-2.0f*pC->z) + (out.z) + (in.z);
	m[0][3] = 0.0f;

	m[1][0] = (-3.0f*pB->x) + (3.0f*pC->x) + (-2.0f*out.x) - (in.x);
	m[1][1] = (-3.0f*pB->y) + (3.0f*pC->y) + (-2.0f*out.y) - (in.y);
	m[1][2] = (-3.0f*pB->z) + (3.0f*pC->z) + (-2.0f*out.z) - (in.z);
	m[0][3] = 0.0f;

	m[2][0] = out.x;
	m[2][1] = out.y;
	m[2][2] = out.z;
	m[0][3] = 0.0f;

	m[3][0] = pB->x;
	m[3][1] = pB->y;
	m[3][2] = pB->z;
	m[3][3] = 0.0f;
}

void Matrix::SetCatmullRomCoefficients(const Vector3 &pA,const Vector3 &pB,const Vector3 &pC,const Vector3 &pD)
{
	SetCatmullRomCoefficients(&pA,&pB,&pC,&pD);
}

//Not 100%, depends on how rotations are gotton to, 180y is the same as 180x, 180z but the matrix looks different.
float Matrix::GetAxisRotation(int pAxis)
{
float r;

	if( pAxis == AXIS_X_ROT )
		r = atan2f(m[1][2],m[1][1])*RADTODEG;
	else if( pAxis == AXIS_Z_ROT )
		r = atan2f(m[0][1],m[0][0])*RADTODEG;
	else//Default to y rotation on bad pAxis number.
		r = atan2f(m[2][0],m[2][2])*RADTODEG;


	if( r > (180.0f - 0.0001f) )
		return 0;

	if( r < (-180.0f + 0.0001f) )
		return 0;

	return r;
}

//Does this = pA * pB
Matrix &Matrix::Mul(const Matrix &pA,const Matrix &pB)
{
	assert( &pA != this );
	assert( &pB != this );

	m[0][0] = (pA.m[0][0]*pB.m[0][0]) + (pA.m[0][1]*pB.m[1][0]) + (pA.m[0][2]*pB.m[2][0]) + (pA.m[0][3]*pB.m[3][0]);
	m[0][1] = (pA.m[0][0]*pB.m[0][1]) + (pA.m[0][1]*pB.m[1][1]) + (pA.m[0][2]*pB.m[2][1]) + (pA.m[0][3]*pB.m[3][1]);
	m[0][2] = (pA.m[0][0]*pB.m[0][2]) + (pA.m[0][1]*pB.m[1][2]) + (pA.m[0][2]*pB.m[2][2]) + (pA.m[0][3]*pB.m[3][2]);
	m[0][3] = (pA.m[0][0]*pB.m[0][3]) + (pA.m[0][1]*pB.m[1][3]) + (pA.m[0][2]*pB.m[2][3]) + (pA.m[0][3]*pB.m[3][3]);

	m[1][0] = (pA.m[1][0]*pB.m[0][0]) + (pA.m[1][1]*pB.m[1][0]) + (pA.m[1][2]*pB.m[2][0]) + (pA.m[1][3]*pB.m[3][0]);
	m[1][1] = (pA.m[1][0]*pB.m[0][1]) + (pA.m[1][1]*pB.m[1][1]) + (pA.m[1][2]*pB.m[2][1]) + (pA.m[1][3]*pB.m[3][1]);
	m[1][2] = (pA.m[1][0]*pB.m[0][2]) + (pA.m[1][1]*pB.m[1][2]) + (pA.m[1][2]*pB.m[2][2]) + (pA.m[1][3]*pB.m[3][2]);
	m[1][3] = (pA.m[1][0]*pB.m[0][3]) + (pA.m[1][1]*pB.m[1][3]) + (pA.m[1][2]*pB.m[2][3]) + (pA.m[1][3]*pB.m[3][3]);

	m[2][0] = (pA.m[2][0]*pB.m[0][0]) + (pA.m[2][1]*pB.m[1][0]) + (pA.m[2][2]*pB.m[2][0]) + (pA.m[2][3]*pB.m[3][0]);
	m[2][1] = (pA.m[2][0]*pB.m[0][1]) + (pA.m[2][1]*pB.m[1][1]) + (pA.m[2][2]*pB.m[2][1]) + (pA.m[2][3]*pB.m[3][1]);
	m[2][2] = (pA.m[2][0]*pB.m[0][2]) + (pA.m[2][1]*pB.m[1][2]) + (pA.m[2][2]*pB.m[2][2]) + (pA.m[2][3]*pB.m[3][2]);
	m[2][3] = (pA.m[2][0]*pB.m[0][3]) + (pA.m[2][1]*pB.m[1][3]) + (pA.m[2][2]*pB.m[2][3]) + (pA.m[2][3]*pB.m[3][3]);

	m[3][0] = (pA.m[3][0]*pB.m[0][0]) + (pA.m[3][1]*pB.m[1][0]) + (pA.m[3][2]*pB.m[2][0]) + (pA.m[3][3]*pB.m[3][0]);
	m[3][1] = (pA.m[3][0]*pB.m[0][1]) + (pA.m[3][1]*pB.m[1][1]) + (pA.m[3][2]*pB.m[2][1]) + (pA.m[3][3]*pB.m[3][1]);
	m[3][2] = (pA.m[3][0]*pB.m[0][2]) + (pA.m[3][1]*pB.m[1][2]) + (pA.m[3][2]*pB.m[2][2]) + (pA.m[3][3]*pB.m[3][2]);
	m[3][3] = (pA.m[3][0]*pB.m[0][3]) + (pA.m[3][1]*pB.m[1][3]) + (pA.m[3][2]*pB.m[2][3]) + (pA.m[3][3]*pB.m[3][3]);

	return *this;
}

//Does this = this * pIn
Matrix &Matrix::Mul(const Matrix &pIn)
{
Matrix temp;

	temp.Mul(*this,pIn);

	*this = temp;

	return *this;
}

//Does this = pIn * this
Matrix &Matrix::MulPre(const Matrix &pIn)
{
Matrix temp;

	temp.Mul(pIn,*this);

	*this = temp;

	return *this;
}

//The same as above, but the result is transposed, handy for running throw SIMD maths. m4x4*Vec is quicker if matrix is transposed.
Matrix &Matrix::MulTranspose(const Matrix &pA,const Matrix &pB)
{
	assert( &pA != this );
	assert( &pB != this );

	m[0][0] = (pA.m[0][0]*pB.m[0][0]) + (pA.m[0][1]*pB.m[1][0]) + (pA.m[0][2]*pB.m[2][0]) + (pA.m[0][3]*pB.m[3][0]);
	m[1][0] = (pA.m[0][0]*pB.m[0][1]) + (pA.m[0][1]*pB.m[1][1]) + (pA.m[0][2]*pB.m[2][1]) + (pA.m[0][3]*pB.m[3][1]);
	m[2][0] = (pA.m[0][0]*pB.m[0][2]) + (pA.m[0][1]*pB.m[1][2]) + (pA.m[0][2]*pB.m[2][2]) + (pA.m[0][3]*pB.m[3][2]);
	m[3][0] = (pA.m[0][0]*pB.m[0][3]) + (pA.m[0][1]*pB.m[1][3]) + (pA.m[0][2]*pB.m[2][3]) + (pA.m[0][3]*pB.m[3][3]);

	m[0][1] = (pA.m[1][0]*pB.m[0][0]) + (pA.m[1][1]*pB.m[1][0]) + (pA.m[1][2]*pB.m[2][0]) + (pA.m[1][3]*pB.m[3][0]);
	m[1][1] = (pA.m[1][0]*pB.m[0][1]) + (pA.m[1][1]*pB.m[1][1]) + (pA.m[1][2]*pB.m[2][1]) + (pA.m[1][3]*pB.m[3][1]);
	m[2][1] = (pA.m[1][0]*pB.m[0][2]) + (pA.m[1][1]*pB.m[1][2]) + (pA.m[1][2]*pB.m[2][2]) + (pA.m[1][3]*pB.m[3][2]);
	m[3][1] = (pA.m[1][0]*pB.m[0][3]) + (pA.m[1][1]*pB.m[1][3]) + (pA.m[1][2]*pB.m[2][3]) + (pA.m[1][3]*pB.m[3][3]);

	m[0][2] = (pA.m[2][0]*pB.m[0][0]) + (pA.m[2][1]*pB.m[1][0]) + (pA.m[2][2]*pB.m[2][0]) + (pA.m[2][3]*pB.m[3][0]);
	m[1][2] = (pA.m[2][0]*pB.m[0][1]) + (pA.m[2][1]*pB.m[1][1]) + (pA.m[2][2]*pB.m[2][1]) + (pA.m[2][3]*pB.m[3][1]);
	m[2][2] = (pA.m[2][0]*pB.m[0][2]) + (pA.m[2][1]*pB.m[1][2]) + (pA.m[2][2]*pB.m[2][2]) + (pA.m[2][3]*pB.m[3][2]);
	m[3][2] = (pA.m[2][0]*pB.m[0][3]) + (pA.m[2][1]*pB.m[1][3]) + (pA.m[2][2]*pB.m[2][3]) + (pA.m[2][3]*pB.m[3][3]);

	m[0][3] = (pA.m[3][0]*pB.m[0][0]) + (pA.m[3][1]*pB.m[1][0]) + (pA.m[3][2]*pB.m[2][0]) + (pA.m[3][3]*pB.m[3][0]);
	m[1][3] = (pA.m[3][0]*pB.m[0][1]) + (pA.m[3][1]*pB.m[1][1]) + (pA.m[3][2]*pB.m[2][1]) + (pA.m[3][3]*pB.m[3][1]);
	m[2][3] = (pA.m[3][0]*pB.m[0][2]) + (pA.m[3][1]*pB.m[1][2]) + (pA.m[3][2]*pB.m[2][2]) + (pA.m[3][3]*pB.m[3][2]);
	m[3][3] = (pA.m[3][0]*pB.m[0][3]) + (pA.m[3][1]*pB.m[1][3]) + (pA.m[3][2]*pB.m[2][3]) + (pA.m[3][3]*pB.m[3][3]);

	return *this;
}

Matrix &Matrix::MulTranspose(const Matrix &pIn)
{
Matrix temp;

	temp.MulTranspose(*this,pIn);

	*this = temp;

	return *this;
}

//Does this = pIn * this
Matrix &Matrix::MulPreTranspose(const Matrix &pIn)
{
Matrix temp;

	temp.MulTranspose(pIn,*this);

	*this = temp;

	return *this;
}


//This treats the matrix as a 3x3 not a 4x4 so no translation.
//This means that any translation that there is, is not changed or overwritten
//Does this = pA * pB
//Returns this.
Matrix &Matrix::Mul3x3(const Matrix &pA,const Matrix &pB)
{
	assert( &pA != this );
	assert( &pB != this );

	m[0][0] = (pA.m[0][0]*pB.m[0][0]) + (pA.m[0][1]*pB.m[1][0]) + (pA.m[0][2]*pB.m[2][0]);
	m[0][1] = (pA.m[0][0]*pB.m[0][1]) + (pA.m[0][1]*pB.m[1][1]) + (pA.m[0][2]*pB.m[2][1]);
	m[0][2] = (pA.m[0][0]*pB.m[0][2]) + (pA.m[0][1]*pB.m[1][2]) + (pA.m[0][2]*pB.m[2][2]);

	m[1][0] = (pA.m[1][0]*pB.m[0][0]) + (pA.m[1][1]*pB.m[1][0]) + (pA.m[1][2]*pB.m[2][0]);
	m[1][1] = (pA.m[1][0]*pB.m[0][1]) + (pA.m[1][1]*pB.m[1][1]) + (pA.m[1][2]*pB.m[2][1]);
	m[1][2] = (pA.m[1][0]*pB.m[0][2]) + (pA.m[1][1]*pB.m[1][2]) + (pA.m[1][2]*pB.m[2][2]);

	m[2][0] = (pA.m[2][0]*pB.m[0][0]) + (pA.m[2][1]*pB.m[1][0]) + (pA.m[2][2]*pB.m[2][0]);
	m[2][1] = (pA.m[2][0]*pB.m[0][1]) + (pA.m[2][1]*pB.m[1][1]) + (pA.m[2][2]*pB.m[2][1]);
	m[2][2] = (pA.m[2][0]*pB.m[0][2]) + (pA.m[2][1]*pB.m[1][2]) + (pA.m[2][2]*pB.m[2][2]);

	return *this;
}

Matrix &Matrix::Mul3x3(const Matrix &pIn)//Does this = this * pIn
{
Matrix temp;

	temp.Mul3x3(*this,pIn);

	m[0][0] = temp.m[0][0];
	m[0][1] = temp.m[0][1];
	m[0][2] = temp.m[0][2];

	m[1][0] = temp.m[1][0];
	m[1][1] = temp.m[1][1];
	m[1][2] = temp.m[1][2];

	m[2][0] = temp.m[2][0];
	m[2][1] = temp.m[2][1];
	m[2][2] = temp.m[2][2];

	return *this;
}

Matrix &Matrix::MulPre3x3(const Matrix &pIn)	//Does this = pIn * this
{
Matrix temp;

	temp.Mul3x3(pIn,*this);

	m[0][0] = temp.m[0][0];
	m[0][1] = temp.m[0][1];
	m[0][2] = temp.m[0][2];

	m[1][0] = temp.m[1][0];
	m[1][1] = temp.m[1][1];
	m[1][2] = temp.m[1][2];

	m[2][0] = temp.m[2][0];
	m[2][1] = temp.m[2][1];
	m[2][2] = temp.m[2][2];

	return *this;
}

Matrix &Matrix::InvertLP()
{
Matrix t;

	t.InvertLP(*this);
	Set(t);
	return *this;
}

Matrix &Matrix::InvertLP(const Matrix &pM)
{
Vector3 v(pM.m[3][0],pM.m[3][1],pM.m[3][2]);

	m[0][0] = pM.m[0][0];
	m[0][1] = pM.m[1][0];
	m[0][2] = pM.m[2][0];
	m[0][3] = 0.0f;

	m[1][0] = pM.m[0][1];
	m[1][1] = pM.m[1][1];
	m[1][2] = pM.m[2][1];
	m[1][3] = 0.0f;

	m[2][0] = pM.m[0][2];
	m[2][1] = pM.m[1][2];
	m[2][2] = pM.m[2][2];
	m[2][3] = 0.0f;

	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = 0.0f;
	m[3][3] = 1.0f;

	v.MatrixMul(this);

	m[3][0] = -v.x;
	m[3][1] = -v.y;
	m[3][2] = -v.z;

	return *this;
}

Matrix &Matrix::Transpose()
{
Matrix t;

	return Set(t.Transpose(*this));
}

Matrix &Matrix::Transpose(const Matrix &pM)
{
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			m[i][j] = pM.m[j][i];
		}
	}

	return *this;
}

void Matrix::ApplyRotationXPost(float pX)
{
Matrix temp;

	temp.SetRotationX(pX);
	Mul(temp);
}


void Matrix::ApplyRotationYPost(float pY)
{
Matrix temp,r;

	temp.SetRotationY(pY);
	Mul(temp);
}

void Matrix::ApplyRotationZPost(float pZ)
{
Matrix temp;

	temp.SetRotationZ(pZ);
	Mul(temp);
}

void Matrix::ApplyRotationXPre(float pX)
{
Matrix temp;

	temp.SetRotationX(pX);
	MulPre(temp);
}


void Matrix::ApplyRotationYPre(float pY)
{
Matrix temp;

	temp.SetRotationY(pY);
	MulPre(temp);
}

void Matrix::ApplyRotationZPre(float pZ)
{
Matrix temp;

	temp.SetRotationZ(pZ);
	MulPre(temp);
}

//Same as above, but the translation is not effected/changed etc.. Uses Mul3x3
void Matrix::ApplyRotationXPost3x3(float pX)
{
Matrix temp;

	temp.SetRotationX(pX);
	Mul3x3(temp);
}


void Matrix::ApplyRotationYPost3x3(float pY)
{
Matrix temp,r;

	temp.SetRotationY(pY);
	Mul3x3(temp);
}

void Matrix::ApplyRotationZPost3x3(float pZ)
{
Matrix temp;

	temp.SetRotationZ(pZ);
	Mul3x3(temp);
}

void Matrix::ApplyRotationXPre3x3(float pX)
{
Matrix temp;

	temp.SetRotationX(pX);
	MulPre3x3(temp);
}


void Matrix::ApplyRotationYPre3x3(float pY)
{
Matrix temp;

	temp.SetRotationY(pY);
	MulPre3x3(temp);
}

void Matrix::ApplyRotationZPre3x3(float pZ)
{
Matrix temp;

	temp.SetRotationZ(pZ);
	MulPre3x3(temp);
}

int Matrix::IsReflected()
{
Vector3 x(m[0][0],m[0][1],m[0][2]);
Vector3 y(m[1][0],m[1][1],m[1][2]);
Vector3 z(m[2][0],m[2][1],m[2][2]);
Vector3 t;

	t.Cross(x,y);

	if( t.Dot(&z) < 0.0f )//Z is reflected
		return 3;

	t.Cross(x,z);

	if( t.Dot(&y) < 0.0f )//Y is reflected
		return 2;

	t.Cross(y,z);

	if( t.Dot(&x) < 0.0f )//X is reflected
		return 1;

	return 0;
}

int Matrix::IsIdentity(int pCheck_translation/*= 1*/)
{
	if( m[0][0] != 1.0f )return 0;
	if( m[0][1] != 0.0f )return 0;
	if( m[0][2] != 0.0f )return 0;
	if( m[0][3] != 0.0f )return 0;

	if( m[1][0] != 0.0f )return 0;
	if( m[1][1] != 1.0f )return 0;
	if( m[1][2] != 0.0f )return 0;
	if( m[1][3] != 0.0f )return 0;

	if( m[2][0] != 0.0f )return 0;
	if( m[2][1] != 0.0f )return 0;
	if( m[2][2] != 1.0f )return 0;
	if( m[2][3] != 0.0f )return 0;

	if( pCheck_translation )
	{
		if( m[3][0] != 0.0f )return 0;
		if( m[3][1] != 0.0f )return 0;
		if( m[3][2] != 0.0f )return 0;
		if( m[3][3] != 1.0f )return 0;
	}

	return 1;
}

Matrix &Matrix::Normalise()
{
Vector3 row_0;
Vector3 row_1(m[1][0],m[1][1],m[1][2]);
Vector3 row_2(m[2][0],m[2][1],m[2][2]);

	row_0.Norm(m[0][0],m[0][1],m[0][2]);

	row_2.Cross(row_0,row_1);
	row_1.Cross(row_2,row_0);

	row_1.Norm();
	row_2.Norm();

	m[0][0] = row_0.x;
	m[0][1] = row_0.y;
	m[0][2] = row_0.z;
	m[0][3] = 0;

	m[1][0] = row_1.x;
	m[1][1] = row_1.y;
	m[1][2] = row_1.z;
	m[1][3] = 0;

	m[2][0] = row_2.x;
	m[2][1] = row_2.y;
	m[2][2] = row_2.z;
	m[2][3] = 0;

	m[3][3] = 1;

	return *this;
}

Matrix &Matrix::Normalise(const Matrix &pMatrix)
{
Vector3 row_0;
Vector3 row_1(pMatrix.m[1][0],pMatrix.m[1][1],pMatrix.m[1][2]);
Vector3 row_2(pMatrix.m[2][0],pMatrix.m[2][1],pMatrix.m[2][2]);

	row_0.Norm(pMatrix.m[0][0],pMatrix.m[0][1],pMatrix.m[0][2]);

	row_2.Cross(row_0,row_1);
	row_1.Cross(row_2,row_0);

	row_1.Norm();
	row_2.Norm();

	m[0][0] = row_0.x;
	m[0][1] = row_0.y;
	m[0][2] = row_0.z;
	m[0][3] = 0;

	m[1][0] = row_1.x;
	m[1][1] = row_1.y;
	m[1][2] = row_1.z;
	m[1][3] = 0;

	m[2][0] = row_2.x;
	m[2][1] = row_2.y;
	m[2][2] = row_2.z;
	m[2][3] = 0;

	m[3][3] = 1;

	return *this;
}

inline float GetSqrt(float pValue)
{
	if( pValue > 0 )
		pValue = sqrtf(pValue);
	else if( pValue < 0 )
		pValue = -sqrtf(-pValue);

	return pValue;
}

Matrix &Matrix::ResetZeroRows()
{
int row;

	for( row = 0 ; row < 3 ; row++ )
	{
		//If there is only one row that is zero then we can do a cross product to get the other.
		if( m[row][0] == 0 && m[row][1] == 0 && m[row][2] == 0 )
		{
			//Make the missing axis.
			SetAxisFromCrossProduct(row,(row+1)%3,(row+2)%3);

			//If the matrix is scaled then the new row could be squared.
			m[row][0] = GetSqrt(m[row][0]);
			m[row][1] = GetSqrt(m[row][1]);
			m[row][2] = GetSqrt(m[row][2]);
			break;
		}
	}

	return *this;
}

void Matrix::DecomposeMatrix(Quaternion &rRotation,Vector3 &rScale,Vector3 &rTranslation)
{
Matrix work;
Vector3 axis;

	work.Normalise(*this);
	rRotation = work;

	work.InvertLP();
	work.Mul(*this);

	work.GetAxis(0,axis);
	rScale.x = axis.Length();

	work.GetAxis(1,axis);
	rScale.y = axis.Length();

	work.GetAxis(2,axis);
	rScale.z = axis.Length();

	GetAxis(3,rTranslation);
}

void Matrix::Scale(float pScale)
{
Matrix m;

	m.SetScale(pScale);
	Mul(m);
}

void Matrix::Scale(float pScaleX,float pScaleY,float pScaleZ)
{
Matrix m;

	m.SetScale(pScaleX,pScaleY,pScaleZ);
	Mul(m);
}


void Matrix::GetOffset(const Matrix &pTo,Vector3 &rOffset)
{
	assert( pTo.IsValid() );
	assert( rOffset.IsNotNULL() );

	rOffset.x = pTo.m[3][0] - m[3][0];
	rOffset.y = pTo.m[3][1] - m[3][1];
	rOffset.z = pTo.m[3][2] - m[3][2];
}

void Matrix::GetOffset(const Vector3 &pTo,Vector3 &rOffset)
{
	assert( pTo.IsValid() );
	assert( rOffset.IsNotNULL() );

	rOffset.x = pTo.x - m[3][0];
	rOffset.y = pTo.y - m[3][1];
	rOffset.z = pTo.z - m[3][2];
}


float Matrix::DistToSq(const Vector3 &pTo) const
{
float ox,oy,oz;

	ox = pTo.x - m[3][0];
	oy = pTo.y - m[3][1];
	oz = pTo.z - m[3][2];

	return (ox*ox)+(oy*oy)+(oz*oz);
}

float Matrix::DistTo(const Vector3 &pTo) const
{
float ox,oy,oz;

	ox = pTo.x - m[3][0];
	oy = pTo.y - m[3][1];
	oz = pTo.z - m[3][2];

	return sqrtf( (ox*ox)+(oy*oy)+(oz*oz) );
}

float Matrix::DistToSq(float pX,float pY,float pZ)const
{
float ox,oy,oz;

	ox = pX - m[3][0];
	oy = pY - m[3][1];
	oz = pZ - m[3][2];

	return (ox*ox)+(oy*oy)+(oz*oz);
}

float Matrix::DistTo(float pX,float pY,float pZ)const
{
float ox,oy,oz;

	ox = pX - m[3][0];
	oy = pY - m[3][1];
	oz = pZ - m[3][2];

	return sqrtf( (ox*ox)+(oy*oy)+(oz*oz) );
}

float Matrix::DistToSq(const Matrix &pTo)const
{
float ox,oy,oz;

assert( pTo.IsValid() );

	ox = pTo.m[3][0] - m[3][0];
	oy = pTo.m[3][1] - m[3][1];
	oz = pTo.m[3][2] - m[3][2];

	return (ox*ox)+(oy*oy)+(oz*oz);
}

float Matrix::DistTo(const Matrix &pTo)const
{
float ox,oy,oz;

	assert( pTo.IsValid() );

	ox = pTo.m[3][0] - m[3][0];
	oy = pTo.m[3][1] - m[3][1];
	oz = pTo.m[3][2] - m[3][2];

	return sqrtf( (ox*ox)+(oy*oy)+(oz*oz) );
}

float Matrix::DistFromEdgeSq(const Vector3 &pA,const Vector3 &pB,Vector3 *rNearest_point/* = NULL */) const
{
Vector3	p(m[3][0],m[3][1],m[3][2]);
	return p.DistFromEdgeSq(pA,pB,rNearest_point);
}
};//namespace BogDog{
