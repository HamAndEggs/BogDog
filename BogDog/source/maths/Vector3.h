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


#pragma once

#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

namespace BogDog{
struct Matrix;

// ---------------------------------------------------------------------------
struct Vector3
{
	float x,y,z;

	//Constructors
	Vector3(){};//So if you alloc a big array you don't get loads of fmov's of zero to x,y,z.
	Vector3(float __v);
	Vector3(float __x,float __y,float __z);
	Vector3(const Vector3 &);

	bool IsNotNULL()const{if( this != NULL )return true;return false;}
	bool IsValid()const{return IsNotNULL();}//will do NaN test.


    // casting
// casting
//	operator float* (){return (float *) &x;}
//	operator const float* () const{return (const float *) &x;}

	//Copies.
    Vector3& operator = ( const Vector3& );
    Vector3& operator = ( float );
	float operator [](int pIndex)const{assert( pIndex > -1 && pIndex < 3 );return (&this->x)[pIndex];}
	float &operator [](int pIndex){assert( pIndex > -1 && pIndex < 3 );return (&this->x)[pIndex];}

    // assignment operators
    Vector3& operator += ( const Vector3& );
    Vector3& operator -= ( const Vector3& );
    Vector3& operator *= ( const Vector3& );
    Vector3& operator /= ( const Vector3& );
    Vector3& operator *= ( float );
    Vector3& operator /= ( float );

    // unary operators
    Vector3 operator + () const;
    Vector3 operator - () const;

    // binary operators
    Vector3 operator + ( const Vector3& ) const;
    Vector3 operator - ( const Vector3& ) const;
    Vector3 operator * ( const Vector3& ) const;
    Vector3 operator * ( float ) const;
    Vector3 operator / ( float ) const;

    friend Vector3 operator * ( float, const Vector3& );

    int operator == ( const Vector3& ) const;
    int operator != ( const Vector3& ) const;

	Vector3 *Set(float pX,float pY,float pZ)
	{
		x = pX;y = pY;z = pZ;
		return this;
	}

//Standard maths things. Saves on the temp object thats needed for the operators.
	Vector3 *Add(float pX,float pY,float pZ);
	Vector3 *Add(const Vector3 &pVec);
	Vector3 *Add(const Vector3 &pA,const Vector3 &pB);
	Vector3 *AddScale(const Vector3 &pVec,float pScale);//Scales vec then adds it on.
	Vector3 *AddScale(const Vector3 &pA,const Vector3 &pB,float pScale);//Scales pB then adds it on to pA

	Vector3 *Sub(const Vector3 &pVec);
	Vector3 *Sub(const Vector3 &pA,const Vector3 &pB);
	Vector3 *Mul(const Vector3 &pVec);
	Vector3 *Mul(const Vector3 &pA,const Vector3 &pB);
	Vector3 *MulScale(const Vector3 &pVec,float pScale);//Scales pVec then muls 'this' by it.
	Vector3 *MulScale(const Vector3 &pA,const Vector3 &pB,float pScale);//Scales pB first then muls pA by the result.
	Vector3 *Div(const Vector3 &pVec);
	Vector3 *Div(const Vector3 &pA,const Vector3 &pB);

//Matrix transform maths.
	//does this = this * pMatrix
	Vector3 *MatrixMul(const Matrix *pMatrix);//Returns the 'this' pointer.
	//does this = pIn * pMatrix
	Vector3 *MatrixMul(const Vector3 *pIn,const Matrix *pMatrix);//returns this.
	//does this = (x,y,z) * pMatrix
	Vector3 *MatrixMul(float pX,float pY,float pZ,const Matrix *pMatrix);//returns this.
	//does this = this * inv pMatrix
	Vector3 *MatrixMulInverse(const Matrix *pMatrix);//Returns the 'this' pointer.
	//does this = pIn * inv pMatrix
	Vector3 *MatrixMulInverse(const Vector3 *pIn,const Matrix *pMatrix);//returns this.
	//does this = (x,y,z) * inv pMatrix
	Vector3 *MatrixMulInverse(float pX,float pY,float pZ,const Matrix *pMatrix);//returns this.

//Matrix rotations, the matrix does not translate it.
	//does this = this * pMatrix
	Vector3 *MatrixRot(const Matrix *pMatrix);//Returns the 'this' pointer.
	//does this = pIn * pMatrix
	Vector3 *MatrixRot(const Vector3 *pIn,const Matrix *pMatrix);//returns this.
	//does this = (x,y,z) * pMatrix
	Vector3 *MatrixRot(float pX,float pY,float pZ,const Matrix *pMatrix);//returns this.
	//does this = this * inv pMatrix
	Vector3 *MatrixRotInverse(const Matrix *pMatrix);//Returns the 'this' pointer.
	//does this = pIn * inv pMatrix
	Vector3 *MatrixRotInverse(const Vector3 *pIn,const Matrix *pMatrix);//returns this.
	//does this = (x,y,z) * inv pMatrix
	Vector3 *MatrixRotInverse(float pX,float pY,float pZ,const Matrix *pMatrix);//returns this.

//Other types of matrix muls.
	Vector3 *MatrixMulCatmullRom(const Matrix &pCoefficients,const float pTime);

//Algibra
	float Dot( const Vector3* v)const;
	float Dot( const Vector3& v)const;
	float LengthSq()const;
	float Length()const;
	float Norm();
	float Norm(const Vector3 &pV);
	float Norm(const Vector3 &pFrom,const Vector3 &pTo);//Makes a norm that points towards pTo from pFrom.
	float Norm(const Vector3 &pA,const Vector3 &pB,const Vector3 &pC);//Makes a norm from this face.
	float Norm(float pX,float pY,float pZ);
	//Makes a norm from this face defined by a list of xyz, the stride is there so you can use it on vertex formats.
	//Stride has to be DWORD(four byte) aligned.
	float Norm(float *pFace,int pVertex_stride = sizeof(float)*3 );

	Vector3 &Cross(const Vector3 &pV);
	Vector3 &Cross(const Vector3 &pA,const Vector3 &pB);
	Vector3 &Cross(float pX,float pY,float pZ);
	void  Tween(const Vector3 *pTo,float pAmount);
	void  Tween(const Vector3 *pFrom,const Vector3 *pTo,float pAmount);
	//The default is that they are the same is diff <= 1mm
	//Returns zero if the diff between the two is within pFactor.
	int Compare(const Vector3 &pA,float pFactor = 0.001f)const;
	//This is very handy func, makes a ray from the front clip plane out in to the scene based on screen pos.
	void MakeViewportRay(
						float pScreen_x,
						float pScreen_y,
						const Matrix *pCamera,
						float pFov,
						float pViewport_width,
						float pViewport_height);
	void RoundToNearest();//Rounds the vectors xyz to the nearest parts.
	void RoundToNearest(const Vector3 *pVec);//Rounds the vectors xyz to the nearest parts.
	void Round(float pMultiple);//Does this = this - this.Mod(pMultiple);
	void Round(float pMultiple,const Vector3 *pVec);//Does this = pVec - pVec.Mod(pMultiple);
	void Floor();
	void Floor(const Vector3 *pVec);
	void Ceiling();
	void Ceiling(const Vector3 *pVec);
	void SetToSphereSurface(const Vector3 *pSphere_center,const float pSphere_radious);

	void Debug_DisplayOnScreen(char *pMessage = 0,int pLine = -1);//-1 means from last line displayed.
	//Returns alpha, which is the percent that the vec lies along the edge, zero of one then point is not along edge.
	//   p										x
	//	  \										|
	//     x--------x   p not on edge,		x---n---x	p on edge, rNearest_point == n
	float GetPointOnEdge(const Vector3 &pA,const Vector3 &pB,Vector3 *rNearest_point)const;//A and B make the edge.
	void Mod(float pDiv);
	void Mod(float pDiv,const Vector3 *pVec);

//Dist funtions.
	float DistToSq(const Vector3 *pTo)const;
	float DistTo(const Vector3 *pTo)const;
	float DistToSq(float pX,float pY,float pZ)const;
	float DistTo(float pX,float pY,float pZ)const;
	float DistToSq(const Matrix *pTo)const;
	float DistTo(const Matrix *pTo)const;
	float DistFromEdgeSq(const Vector3 &pA,const Vector3 &pB,Vector3 *rNearest_point = 0 )const;//A and B make the edge.

//Rotation funcs.
	Vector3 *RotateX(Vector3 *pVec,float pX);
	Vector3 *RotateX(float pX);
	Vector3 *RotateY(Vector3 *pVec,float pY);
	Vector3 *RotateY(float pY);
	Vector3 *RotateZ(Vector3 *pVec,float pZ);
	Vector3 *RotateZ(float pZ);

//Static util funtions.
	//The passed indices array should have the same number of members as the points.
	//The indices are mappings to the unique verts. So say vector[2] and vector[3] are the same
	//Index[2] and Index[3] will == 2.
	static int MakeUniqueIndexArray(Vector3 *pPoints,int pNum_points,int *rIndices);

	//This func will alloc a new array of vectors and indices for you using the func above.
	static void MakeUniqueVectorArray(Vector3 *pPoints,int pNum_points,
									Vector3 **rUnique,int *rNum_unique,
									int **rIndex_list);//rIndex_list will take you bake to the orginal list.

	const char *GetAsString()const;
};

//Constructors
inline Vector3::Vector3(float __v)
{
	x = __v;y = __v;z = __v;
}

inline Vector3::Vector3(float __x,float __y,float __z)
{
	x = __x;y = __y;z = __z;
}

inline Vector3::Vector3(const Vector3 &pVector)
{
	x = pVector.x;y = pVector.y;z = pVector.z;
}



// assignment operators
inline Vector3& Vector3::operator = ( const Vector3 &v )
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

inline Vector3& Vector3::operator = ( float f)
{
	x = f;
	y = f;
	z = f;
	return *this;
}

inline Vector3& Vector3::operator += ( const Vector3& v )
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

inline Vector3& Vector3::operator -= ( const Vector3& v )
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

inline Vector3& Vector3::operator *= ( const Vector3& v )
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
}

inline Vector3& Vector3::operator /= ( const Vector3& v )
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    return *this;
}


inline Vector3& Vector3::operator *= ( float f )
{
    x *= f;
    y *= f;
    z *= f;
    return *this;
}

inline Vector3& Vector3::operator /= ( float f )
{
    float fInv = 1.0f / f;
    x *= fInv;
    y *= fInv;
    z *= fInv;
    return *this;
}


// unary operators
inline Vector3 Vector3::operator + () const
{
    return *this;
}

inline Vector3 Vector3::operator - () const
{
    return Vector3(-x, -y, -z);
}


// binary operators
inline Vector3 Vector3::operator + ( const Vector3& v ) const
{
    return Vector3(x + v.x, y + v.y, z + v.z);
}

inline Vector3 Vector3::operator - ( const Vector3& v ) const
{
    return Vector3(x - v.x, y - v.y, z - v.z);
}

inline Vector3 Vector3::operator * ( const Vector3& v ) const
{
    return Vector3 (x * v.x, y * v.y, z * v.z);
}

inline Vector3 Vector3::operator * ( float f ) const
{
    return Vector3(x * f, y * f, z * f);
}

inline Vector3 Vector3::operator / ( float f ) const
{
    float fInv = 1.0f / f;
    return Vector3(x * fInv, y * fInv, z * fInv);
}


inline Vector3 operator * ( float f, const Vector3& v )
{
    return Vector3(f * v.x, f * v.y, f * v.z);
}

inline int Vector3::operator == ( const Vector3& v ) const
{
    return (x == v.x && y == v.y && z == v.z);
}

inline int Vector3::operator != ( const Vector3& v ) const
{
    return x != v.x || y != v.y || z != v.z;
}

//Algibra
inline float Vector3::Dot( const Vector3* v) const
{
	return (x * v->x) + (y * v->y) + (z * v->z);
}

inline float Vector3::Dot( const Vector3& v) const
{
	return (x * v.x) + (y * v.y) + (z * v.z);
}


inline float Vector3::LengthSq() const
{
	return (x*x)+(y*y)+(z*z);
}

inline float Vector3::Length() const
{
	return sqrtf((x*x)+(y*y)+(z*z));
}

//Standard maths things. Saves on the temp object thats needed for the operators.
//Addition
inline Vector3 *Vector3::Add(const Vector3 &pVec)
{
	x += pVec.x;
	y += pVec.y;
	z += pVec.z;

	return this;
}

inline Vector3 *Vector3::Add(float pX,float pY,float pZ)
{
	x += pX;
	y += pY;
	z += pZ;
	return this;
}

inline Vector3 *Vector3::Add(const Vector3 &pA,const Vector3 &pB)
{
	x = pA.x + pB.x;
	y = pA.y + pB.y;
	z = pA.z + pB.z;

	return this;
}
//Scales vec then adds it on.
inline Vector3 *Vector3::AddScale(const Vector3 &pVec,float pScale)
{
	x += (pVec.x * pScale);
	y += (pVec.y * pScale);
	z += (pVec.z * pScale);
	return this;
}

//Scales pB then adds it on to pA
inline Vector3 *Vector3::AddScale(const Vector3 &pA,const Vector3 &pB,float pScale)
{
	x = pA.x + (pB.x * pScale);
	y = pA.y + (pB.y * pScale);
	z = pA.z + (pB.z * pScale);
	return this;
}


//Subtraction
inline Vector3 *Vector3::Sub(const Vector3 &pVec)
{
	x -= pVec.x;
	y -= pVec.y;
	z -= pVec.z;

	return this;
}
inline Vector3 *Vector3::Sub(const Vector3 &pA,const Vector3 &pB)
{
	x = pA.x - pB.x;
	y = pA.y - pB.y;
	z = pA.z - pB.z;

	return this;
}

//Multiplication
inline Vector3 *Vector3::Mul(const Vector3 &pVec)
{
	x *= pVec.x;
	y *= pVec.y;
	z *= pVec.z;

	return this;
}
inline Vector3 *Vector3::Mul(const Vector3 &pA,const Vector3 &pB)
{
	x = pA.x * pB.x;
	y = pA.y * pB.y;
	z = pA.z * pB.z;

	return this;
}

inline Vector3 *Vector3::MulScale(const Vector3 &pVec,float pScale)
{
	x *= (pVec.x * pScale);
	y *= (pVec.y * pScale);
	z *= (pVec.z * pScale);

	return this;
}

inline Vector3 *Vector3::MulScale(const Vector3 &pA,const Vector3 &pB,float pScale)
{
	x = pA.x * (pB.x * pScale);
	y = pA.y * (pB.y * pScale);
	z = pA.z * (pB.z * pScale);

	return this;
}

//Division
inline Vector3 *Vector3::Div(const Vector3 &pVec)
{
	x /= pVec.x;
	y /= pVec.y;
	z /= pVec.z;

	return this;
}

inline Vector3 *Vector3::Div(const Vector3 &pA,const Vector3 &pB)
{
	x = pA.x / pB.x;
	y = pA.y / pB.y;
	z = pA.z / pB.z;

	return this;
}

inline void Vector3::Round(float pMultiple)
{
	assert( pMultiple != 0.0f );
	x = x - fmodf(x,pMultiple);
	y = y - fmodf(y,pMultiple);
	z = z - fmodf(z,pMultiple);
}

inline void Vector3::Round(float pMultiple,const Vector3 *pVec)
{
	assert( pMultiple != 0.0f );
	assert( pVec );
	x = pVec->x - fmodf(pVec->x,pMultiple);
	y = pVec->y - fmodf(pVec->y,pMultiple);
	z = pVec->z - fmodf(pVec->z,pMultiple);
}

inline void Vector3::Floor()
{
	x = floorf(x);
	y = floorf(y);
	z = floorf(z);
}

inline void Vector3::Floor(const Vector3 *pVec)
{
	assert( pVec );
	x = floorf(pVec->x);
	y = floorf(pVec->y);
	z = floorf(pVec->z);
}

inline void Vector3::Ceiling()
{
	x = ceilf(x);
	y = ceilf(y);
	z = ceilf(z);
}

inline void Vector3::Ceiling(const Vector3 *pVec)
{
	assert( pVec );
	x = ceilf(pVec->x);
	y = ceilf(pVec->y);
	z = ceilf(pVec->z);
}

inline void Vector3::Mod(float pDiv)
{
	assert( pDiv != 0.0f );
	x = fmodf(x,pDiv);
	y = fmodf(y,pDiv);
	z = fmodf(z,pDiv);
}

inline void Vector3::Mod(float pDiv,const Vector3 *pVec)
{
	assert( pDiv != 0.0f );
	assert( pVec );
	x = fmodf(pVec->x,pDiv);
	y = fmodf(pVec->y,pDiv);
	z = fmodf(pVec->z,pDiv);
}
// ---------------------------------------------------------------------------
};//namespace BogDog{
