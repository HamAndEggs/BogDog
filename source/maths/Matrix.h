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

#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stddef.h>
#include "Vector3.h"
#include "SinCos.h"

namespace BogDog{
struct Quaternion;
// ---------------------------------------------------------------------------

struct Matrix
{
	enum{AXIS_X_ROT	= 0};
	enum{AXIS_Y_ROT	= 1};
	enum{AXIS_Z_ROT	= 2};
	enum{AXIS_TRANS	= 3};

	float m[4][4];

	static Matrix identity;

	Matrix(){};
	~Matrix(){};
	Matrix(float _00,float _01,float _02,float _03,
			float _10,float _11,float _12,float _13,
			float _20,float _21,float _22,float _23,
			float _30,float _31,float _32,float _33);

	Matrix(const Quaternion &pQuat){;SetRotation(pQuat);}
	Matrix(float pX,float pY,float pZ){;SetRotation(pX,pY,pZ);}
	Matrix(const Matrix &pMatrix){Set(pMatrix);}

	const Vector3 &operator [](int pIndex)const{assert( pIndex > -1 && pIndex < 4 );return *((const Vector3*)m[pIndex]);}

	Vector3 &operator [](int pIndex){assert( pIndex > -1 && pIndex < 4 );return *((Vector3*)m[pIndex]);}

	bool IsNotNULL()const{if( this != NULL )return true;return false;}
	bool IsValid()const{return IsNotNULL();}//will do NaN test.

	void SetIdentity();
	void SetScale(float pX,float pY,float pZ);
	void SetScale(float pScale){SetScale(pScale,pScale,pScale);}

	void SetRotation(Angle pitch,Angle yaw,Angle roll);
	void SetRotation(const Quaternion &pQuat);
	void SetRotationX(Angle pitch);
	void SetRotationY(Angle yaw);
	void SetRotationZ(Angle roll);

	Matrix &Set(const Matrix &pSource);
	Matrix &Set(const float pSource[4][3]);
	Matrix &Set(const float *pSource,int pIs_zup_rhs);//pSource is asumed to be float[4][3], pIs_zup_rhs is for 3DSMax/OGl style matrices.
	Matrix &Set(float pX,float pY,float pZ);//Sets as identity with translation set to x,y,z.

	void SetZYOrientation(const Vector3 &pLook,const Vector3 &pUp,int pClear_transform = 1);
	void SetZYLookAt(const Vector3 &pFrom,const Vector3 &pTo);
	inline void SetAxis(int pAxis_index,float pX,float pY,float pZ);
	inline void SetAxis(int pAxis_index,float pX,float pY,float pZ,float pW);
	inline void SetAxis(int pAxis_index,const Vector3 &pAxis);
	void SetAxisFromCrossProduct(int pDest_axis,int pAxis_a,int pAxis_b);
	//Makes a matrix that then when mul'd with a vector of (t*t*t,t*t,t) gives a point on a curve from pB to pC.
	void SetCatmullRomCoefficients(const Vector3 *pA,const Vector3 *pB,const Vector3 *pC,const Vector3 *pD);
	void SetCatmullRomCoefficients(const Vector3 &pA,const Vector3 &pB,const Vector3 &pC,const Vector3 &pD);

	//Some get functions.
	inline Vector3 &GetAxis(int pAxis_index,Vector3 &rAxis) const;
	inline Vector3 &GetAxis(int pAxis_index,Vector3 &rAxis,float pScale) const;
	inline Vector3 &GetAxisCrossProduct(int pAxis_a,int pAxis_b,Vector3 &rVec) const;
	float GetAxisRotation(int pAxis = AXIS_Y_ROT);//Not 100%, depends on how rotations are gotton to, 180y is the same as 180x, 180z but the matrix looks different.

	//Rotation stuff.
	void ApplyRotationXPost(float pX);
	void ApplyRotationYPost(float pY);
	void ApplyRotationZPost(float pZ);
	void ApplyRotationXPre(float pX);
	void ApplyRotationYPre(float pY);
	void ApplyRotationZPre(float pZ);
	//Same as above, but the translation is not effected/changed etc.. Uses Mul3x3
	void ApplyRotationXPost3x3(float pX);
	void ApplyRotationYPost3x3(float pY);
	void ApplyRotationZPost3x3(float pZ);
	void ApplyRotationXPre3x3(float pX);
	void ApplyRotationYPre3x3(float pY);
	void ApplyRotationZPre3x3(float pZ);


	//Addition/subtraction.
	inline void AddAxis(int pAxis_index,const Vector3 &pAdd);
	inline void SubAxis(int pAxis_index,const Vector3 &pSub);
	inline void AddAxis(int pAxis_index,const Vector3 &pAdd,float pScale);//Does axis += (pA * pScale)
	inline void SubAxis(int pAxis_index,const Vector3 &pSub,float pScale);
	inline void AddAxis(int pDest_axis,int pSource_axis,float pScale);
	inline void SubAxis(int pDest_axis,int pSource_axis,float pScale);

	//Does this = pA * pB
	//Returns this.
	Matrix &Mul(const Matrix &pA,const Matrix &pB);
	Matrix &Mul(const Matrix &pIn);//Does this = this * pIn
	Matrix &MulPre(const Matrix &pIn);	//Does this = pIn * this
	//The same as above, but the result is transposed, handy for running throw SIMD maths. m4x4*Vec is quicker if matrix is transposed.
	Matrix &MulTranspose(const Matrix &pA,const Matrix &pB);
	Matrix &MulTranspose(const Matrix &pIn);//Does this = this * pIn
	Matrix &MulPreTranspose(const Matrix &pIn);	//Does this = pIn * this


	//This treats the matrix as a 3x3 not a 4x4 so no translation.
	//This means that any translation that there is, is not changed or overwritten
	//Does this = pA * pB
	//Returns this.
	Matrix &Mul3x3(const Matrix &pA,const Matrix &pB);
	Matrix &Mul3x3(const Matrix &pIn);//Does this = this * pIn
	Matrix &MulPre3x3(const Matrix &pIn);	//Does this = pIn * this

	//Util stuff.
	//Returns pOut.
	Matrix &Normalise();//Returns this.
	Matrix &Normalise(const Matrix &pMatrix);//Returns this.
	Matrix &ResetZeroRows();
	void DecomposeMatrix(Quaternion &pRotation,Vector3 &pScale,Vector3 &pTranslation);
	void Scale(float pScale);
	void Scale(float pScaleX,float pScaleY,float pScaleZ);
	void Scale(const Vector3 &pScale){Scale(pScale.x,pScale.y,pScale.z);}
	void Rand(float pX_min,float pX_max,float pY_min,float pY_max,float pZ_min,float pZ_max);
	void Rand(float pX,float pY,float pZ);
	void GetOffset(const Matrix &pTo,Vector3 &rOffset);// Does pTo - this from the translation.
	void GetOffset(const Vector3 &pTo,Vector3 &rOffset);// Does pTo - this from the translation.

	//Dist funtions.
	float DistToSq(const Vector3 &pTo)const;
	float DistTo(const Vector3 &pTo)const;
	float DistToSq(float pX,float pY,float pZ)const;
	float DistTo(float pX,float pY,float pZ)const;
	float DistToSq(const Matrix &pTo)const;
	float DistTo(const Matrix &pTo)const;
	float DistFromEdgeSq(const Vector3 &pA,const Vector3 &pB,Vector3 *rNearest_point = 0 )const;//A and B make the edge.

	//Operators;
	Matrix operator = (const Matrix &pIn){ for( int i = 0 ; i < 16 ; ((float*)m)[i] = ((float*)pIn.m)[i] , i++); return *this;}

	//Inversions.
	Matrix &InvertLP();
	Matrix &InvertLP(const Matrix &pM);
	//Slower but deals with scaled matrices.
	Matrix &Invert();
	Matrix &Invert(const Matrix &pM);

	Matrix &Transpose();
	Matrix &Transpose(const Matrix &pM);

	//State functions.
	int IsReflected();
	int IsIdentity(int pCheck_translation = 1);
};

inline void Matrix::SetAxis(int pAxis_index,float pX,float pY,float pZ)
{
	assert( pAxis_index > -1 && pAxis_index < 4 );

	m[pAxis_index][0] = pX;
	m[pAxis_index][1] = pY;
	m[pAxis_index][2] = pZ;
	if( pAxis_index == 3 )
		m[pAxis_index][3] = 1;
	else
		m[pAxis_index][3] = 0;
}

inline void Matrix::SetAxis(int pAxis_index,const Vector3 &pAxis)
{
	assert( pAxis_index > -1 && pAxis_index < 4 );
	assert( pAxis.IsValid() );

	m[pAxis_index][0] = pAxis.x;
	m[pAxis_index][1] = pAxis.y;
	m[pAxis_index][2] = pAxis.z;
	if( pAxis_index == 3 )
		m[pAxis_index][3] = 1;
	else
		m[pAxis_index][3] = 0;
}

inline void Matrix::AddAxis(int pAxis_index,const Vector3 &pAdd)
{
	assert( pAxis_index > -1 && pAxis_index < 4 );
	assert( pAdd.IsValid() );

	m[pAxis_index][0] += pAdd.x;
	m[pAxis_index][1] += pAdd.y;
	m[pAxis_index][2] += pAdd.z;
}

inline void Matrix::SubAxis(int pAxis_index,const Vector3 &pSub)
{
	assert( pAxis_index > -1 && pAxis_index < 4 );
	assert( pSub.IsValid() );

	m[pAxis_index][0] -= pSub.x;
	m[pAxis_index][1] -= pSub.y;
	m[pAxis_index][2] -= pSub.z;
}

inline void Matrix::AddAxis(int pAxis_index,const Vector3 &pAdd,float pScale)
{
	assert( pAxis_index > -1 && pAxis_index < 4 );
	assert( pAdd.IsValid() );

	m[pAxis_index][0] += (pAdd.x*pScale);
	m[pAxis_index][1] += (pAdd.y*pScale);
	m[pAxis_index][2] += (pAdd.z*pScale);
}

inline void Matrix::SubAxis(int pAxis_index,const Vector3 &pSub,float pScale)
{
	assert( pAxis_index > -1 && pAxis_index < 4 );
	assert( pSub.IsValid() );

	m[pAxis_index][0] -= (pSub.x*pScale);
	m[pAxis_index][1] -= (pSub.y*pScale);
	m[pAxis_index][2] -= (pSub.z*pScale);
}

inline void Matrix::AddAxis(int pDest_axis,int pSource_axis,float pScale)
{
	assert( pDest_axis > -1 && pDest_axis < 4 );
	assert( pSource_axis > -1 && pSource_axis < 4 );

	m[pDest_axis][0] += (m[pSource_axis][0]*pScale);
	m[pDest_axis][1] += (m[pSource_axis][1]*pScale);
	m[pDest_axis][2] += (m[pSource_axis][2]*pScale);
}

inline void Matrix::SubAxis(int pDest_axis,int pSource_axis,float pScale)
{
	assert( pDest_axis > -1 && pDest_axis < 4 );
	assert( pSource_axis > -1 && pSource_axis < 4 );

	m[pDest_axis][0] -= (m[pSource_axis][0]*pScale);
	m[pDest_axis][1] -= (m[pSource_axis][1]*pScale);
	m[pDest_axis][2] -= (m[pSource_axis][2]*pScale);
}

//Some get functions.
inline Vector3 &Matrix::GetAxis(int pAxis_index,Vector3 &rAxis) const
{
	assert( pAxis_index > -1 && pAxis_index < 4 );
	assert( rAxis.IsValid() );

	rAxis.x = m[pAxis_index][0];
	rAxis.y = m[pAxis_index][1];
	rAxis.z = m[pAxis_index][2];

	return rAxis;
}

inline Vector3 &Matrix::GetAxis(int pAxis_index,Vector3 &rAxis,float pScale) const
{
	assert( pAxis_index > -1 && pAxis_index < 4 );
	assert( rAxis.IsValid() );

	rAxis.x = m[pAxis_index][0]*pScale;
	rAxis.y = m[pAxis_index][1]*pScale;
	rAxis.z = m[pAxis_index][2]*pScale;

	return rAxis;
}

inline Vector3 &Matrix::GetAxisCrossProduct(int pAxis_a,int pAxis_b,Vector3 &rVec) const
{
	assert( pAxis_a > -1 && pAxis_a < 4 );
	assert( pAxis_b > -1 && pAxis_b < 4 );
	assert( rVec.IsValid() );

	rVec.x = (m[pAxis_a][1] * m[pAxis_b][2]) - (m[pAxis_a][2] * m[pAxis_b][1]);
	rVec.y = (m[pAxis_a][2] * m[pAxis_b][0]) - (m[pAxis_a][0] * m[pAxis_b][2]);
	rVec.z = (m[pAxis_a][0] * m[pAxis_b][1]) - (m[pAxis_a][1] * m[pAxis_b][0]);

	return rVec;
}
// ---------------------------------------------------------------------------
};//namespace BogDog{
#endif//#ifndef __MATRIX_H__
