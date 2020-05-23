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
struct Vector2
{
	float x,y;

	//Constructors
	Vector2(){};//So if you alloc a big array you don't get loads of fmov's of zero to x,y,z.
	Vector2(float __v);
	Vector2(float __x,float __y);
	Vector2(const Vector2 &);

	bool IsNotNULL()const{if( this != NULL )return true;return false;}
	bool IsValid()const{return IsNotNULL();}//will do NaN test.


    // casting
// casting
//	operator float* (){return (float *) &x;}
//	operator const float* () const{return (const float *) &x;}

	//Copies.
    Vector2& operator = ( const Vector2& );
    Vector2& operator = ( float );
	float operator [](int pIndex)const{assert( pIndex > -1 && pIndex < 3 );return (&this->x)[pIndex];}
	float &operator [](int pIndex){assert( pIndex > -1 && pIndex < 3 );return (&this->x)[pIndex];}

    // assignment operators
    Vector2& operator += ( const Vector2& );
    Vector2& operator -= ( const Vector2& );
    Vector2& operator *= ( const Vector2& );
    Vector2& operator /= ( const Vector2& );
    Vector2& operator *= ( float );
    Vector2& operator /= ( float );

    // unary operators
    Vector2 operator + () const;
    Vector2 operator - () const;

    // binary operators
    Vector2 operator + ( const Vector2& ) const;
    Vector2 operator - ( const Vector2& ) const;
    Vector2 operator * ( const Vector2& ) const;
    Vector2 operator * ( float ) const;
    Vector2 operator / ( float ) const;

    friend Vector2 operator * ( float, const Vector2& );

    int operator == ( const Vector2& ) const;
    int operator != ( const Vector2& ) const;

	Vector2 *Set(float pX,float pY)
	{
		x = pX;y = pY;
		return this;
	}

//Standard maths things. Saves on the temp object thats needed for the operators.
	Vector2 *Add(float pX,float pY,float pZ);
	Vector2 *Add(const Vector2 &pVec);
	Vector2 *Add(const Vector2 &pA,const Vector2 &pB);
	Vector2 *AddScale(const Vector2 &pVec,float pScale);//Scales vec then adds it on.
	Vector2 *AddScale(const Vector2 &pA,const Vector2 &pB,float pScale);//Scales pB then adds it on to pA

	Vector2 *Sub(const Vector2 &pVec);
	Vector2 *Sub(const Vector2 &pA,const Vector2 &pB);
	Vector2 *Mul(const Vector2 &pVec);
	Vector2 *Mul(const Vector2 &pA,const Vector2 &pB);
	Vector2 *MulScale(const Vector2 &pVec,float pScale);//Scales pVec then muls 'this' by it.
	Vector2 *MulScale(const Vector2 &pA,const Vector2 &pB,float pScale);//Scales pB first then muls pA by the result.
	Vector2 *Div(const Vector2 &pVec);
	Vector2 *Div(const Vector2 &pA,const Vector2 &pB);

//Matrix transform maths.
	//does this = this * pMatrix
	Vector2 *MatrixMul(const Matrix *pMatrix);//Returns the 'this' pointer.
	//does this = pIn * pMatrix
	Vector2 *MatrixMul(const Vector2 *pIn,const Matrix *pMatrix);//returns this.
	//does this = (x,y,z) * pMatrix
	Vector2 *MatrixMul(float pX,float pY,const Matrix *pMatrix);//returns this.
	//does this = this * inv pMatrix
	Vector2 *MatrixMulInverse(const Matrix *pMatrix);//Returns the 'this' pointer.
	//does this = pIn * inv pMatrix
	Vector2 *MatrixMulInverse(const Vector2 *pIn,const Matrix *pMatrix);//returns this.
	//does this = (x,y,z) * inv pMatrix
	Vector2 *MatrixMulInverse(float pX,float pY,const Matrix *pMatrix);//returns this.

//Matrix rotations, the matrix does not translate it.
	//does this = this * pMatrix
	Vector2 *MatrixRot(const Matrix *pMatrix);//Returns the 'this' pointer.
	//does this = pIn * pMatrix
	Vector2 *MatrixRot(const Vector2 *pIn,const Matrix *pMatrix);//returns this.
	//does this = (x,y,z) * pMatrix
	Vector2 *MatrixRot(float pX,float pY,const Matrix *pMatrix);//returns this.
	//does this = this * inv pMatrix
	Vector2 *MatrixRotInverse(const Matrix *pMatrix);//Returns the 'this' pointer.
	//does this = pIn * inv pMatrix
	Vector2 *MatrixRotInverse(const Vector2 *pIn,const Matrix *pMatrix);//returns this.
	//does this = (x,y,z) * inv pMatrix
	Vector2 *MatrixRotInverse(float pX,float pY,const Matrix *pMatrix);//returns this.

//Other types of matrix muls.
	Vector2 *MatrixMulCatmullRom(const Matrix &pCoefficients,const float pTime);

//Algibra
	float Dot( const Vector2* v)const;
	float Dot( const Vector2& v)const;
	float LengthSq()const;
	float Length()const;

	void  Tween(const Vector2 *pTo,float pAmount);
	void  Tween(const Vector2 *pFrom,const Vector2 *pTo,float pAmount);
	//The default is that they are the same is diff <= 1mm
	//Returns zero if the diff between the two is within pFactor.
	int Compare(const Vector2 &pA,float pFactor = 0.001f)const;

	void RoundToNearest();//Rounds the vectors xyz to the nearest parts.
	void RoundToNearest(const Vector2 *pVec);//Rounds the vectors xyz to the nearest parts.
	void Round(float pMultiple);//Does this = this - this.Mod(pMultiple);
	void Round(float pMultiple,const Vector2 *pVec);//Does this = pVec - pVec.Mod(pMultiple);
	void Floor();
	void Floor(const Vector2 *pVec);
	void Ceiling();
	void Ceiling(const Vector2 *pVec);
	void SetToSphereSurface(const Vector2 *pSphere_center,const float pSphere_radious);

	void Debug_DisplayOnScreen(char *pMessage = 0,int pLine = -1);//-1 means from last line displayed.

	void Mod(float pDiv);
	void Mod(float pDiv,const Vector2 *pVec);

//Dist funtions.
	float DistToSq(const Vector2 *pTo)const;
	float DistTo(const Vector2 *pTo)const;
	float DistToSq(float pX,float pY)const;
	float DistTo(float pX,float pY)const;
	float DistToSq(const Matrix *pTo)const;
	float DistTo(const Matrix *pTo)const;

//Rotation funcs.
	Vector2 *RotateX(Vector2 *pVec,float pX);
	Vector2 *RotateX(float pX);
	Vector2 *RotateY(Vector2 *pVec,float pY);
	Vector2 *RotateY(float pY);
	Vector2 *RotateZ(Vector2 *pVec,float pZ);
	Vector2 *RotateZ(float pZ);

//Static util funtions.
	//The passed indices array should have the same number of members as the points.
	//The indices are mappings to the unique verts. So say vector[2] and vector[3] are the same
	//Index[2] and Index[3] will == 2.
	static int MakeUniqueIndexArray(Vector2 *pPoints,int pNum_points,int *rIndices);

	//This func will alloc a new array of vectors and indices for you using the func above.
	static void MakeUniqueVectorArray(Vector2 *pPoints,int pNum_points,
									Vector2 **rUnique,int *rNum_unique,
									int **rIndex_list);//rIndex_list will take you bake to the orginal list.

	const char *GetAsString()const;
};

//Constructors
inline Vector2::Vector2(float __v)
{
	x = __v;y = __v;
}

inline Vector2::Vector2(float __x,float __y)
{
	x = __x;y = __y;
}

inline Vector2::Vector2(const Vector2 &pVector)
{
	x = pVector.x;y = pVector.y;
}



// assignment operators
inline Vector2& Vector2::operator = ( const Vector2 &v )
{
	x = v.x;
	y = v.y;

	return *this;
}

inline Vector2& Vector2::operator = ( float f)
{
	x = f;
	y = f;
	return *this;
}

inline Vector2& Vector2::operator += ( const Vector2& v )
{
    x += v.x;
    y += v.y;
    return *this;
}

inline Vector2& Vector2::operator -= ( const Vector2& v )
{
    x -= v.x;
    y -= v.y;
    return *this;
}

inline Vector2& Vector2::operator *= ( const Vector2& v )
{
    x *= v.x;
    y *= v.y;
    return *this;
}

inline Vector2& Vector2::operator /= ( const Vector2& v )
{
    x /= v.x;
    y /= v.y;
    return *this;
}


inline Vector2& Vector2::operator *= ( float f )
{
    x *= f;
    y *= f;
    return *this;
}

inline Vector2& Vector2::operator /= ( float f )
{
    float fInv = 1.0f / f;
    x *= fInv;
    y *= fInv;
    return *this;
}


// unary operators
inline Vector2 Vector2::operator + () const
{
    return *this;
}

inline Vector2 Vector2::operator - () const
{
    return Vector2(-x, -y);
}


// binary operators
inline Vector2 Vector2::operator + ( const Vector2& v ) const
{
    return Vector2(x + v.x, y + v.y);
}

inline Vector2 Vector2::operator - ( const Vector2& v ) const
{
    return Vector2(x - v.x, y - v.y);
}

inline Vector2 Vector2::operator * ( const Vector2& v ) const
{
    return Vector2 (x * v.x, y * v.y);
}

inline Vector2 Vector2::operator * ( float f ) const
{
    return Vector2(x * f, y * f);
}

inline Vector2 Vector2::operator / ( float f ) const
{
    float fInv = 1.0f / f;
    return Vector2(x * fInv, y * fInv);
}


inline Vector2 operator * ( float f, const Vector2& v )
{
    return Vector2(f * v.x, f * v.y);
}

inline int Vector2::operator == ( const Vector2& v ) const
{
    return (x == v.x && y == v.y);
}

inline int Vector2::operator != ( const Vector2& v ) const
{
    return x != v.x || y != v.y;
}

//Algibra
inline float Vector2::Dot( const Vector2* v) const
{
	return (x * v->x) + (y * v->y);
}

inline float Vector2::Dot( const Vector2& v) const
{
	return (x * v.x) + (y * v.y);
}


inline float Vector2::LengthSq() const
{
	return (x*x)+(y*y);
}

inline float Vector2::Length() const
{
	return sqrtf((x*x)+(y*y));
}

//Standard maths things. Saves on the temp object thats needed for the operators.
//Addition
inline Vector2 *Vector2::Add(const Vector2 &pVec)
{
	x += pVec.x;
	y += pVec.y;

	return this;
}

inline Vector2 *Vector2::Add(float pX,float pY,float pZ)
{
	x += pX;
	y += pY;

	return this;
}

inline Vector2 *Vector2::Add(const Vector2 &pA,const Vector2 &pB)
{
	x = pA.x + pB.x;
	y = pA.y + pB.y;

	return this;
}
//Scales vec then adds it on.
inline Vector2 *Vector2::AddScale(const Vector2 &pVec,float pScale)
{
	x += (pVec.x * pScale);
	y += (pVec.y * pScale);
	return this;
}

//Scales pB then adds it on to pA
inline Vector2 *Vector2::AddScale(const Vector2 &pA,const Vector2 &pB,float pScale)
{
	x = pA.x + (pB.x * pScale);
	y = pA.y + (pB.y * pScale);
	return this;
}


//Subtraction
inline Vector2 *Vector2::Sub(const Vector2 &pVec)
{
	x -= pVec.x;
	y -= pVec.y;

	return this;
}

inline Vector2 *Vector2::Sub(const Vector2 &pA,const Vector2 &pB)
{
	x = pA.x - pB.x;
	y = pA.y - pB.y;

	return this;
}

//Multiplication
inline Vector2 *Vector2::Mul(const Vector2 &pVec)
{
	x *= pVec.x;
	y *= pVec.y;

	return this;
}
inline Vector2 *Vector2::Mul(const Vector2 &pA,const Vector2 &pB)
{
	x = pA.x * pB.x;
	y = pA.y * pB.y;

	return this;
}

inline Vector2 *Vector2::MulScale(const Vector2 &pVec,float pScale)
{
	x *= (pVec.x * pScale);
	y *= (pVec.y * pScale);

	return this;
}

inline Vector2 *Vector2::MulScale(const Vector2 &pA,const Vector2 &pB,float pScale)
{
	x = pA.x * (pB.x * pScale);
	y = pA.y * (pB.y * pScale);

	return this;
}

//Division
inline Vector2 *Vector2::Div(const Vector2 &pVec)
{
	x /= pVec.x;
	y /= pVec.y;

	return this;
}

inline Vector2 *Vector2::Div(const Vector2 &pA,const Vector2 &pB)
{
	x = pA.x / pB.x;
	y = pA.y / pB.y;

	return this;
}

inline void Vector2::Round(float pMultiple)
{
	assert( pMultiple != 0.0f );
	x = x - fmodf(x,pMultiple);
	y = y - fmodf(y,pMultiple);
}

inline void Vector2::Round(float pMultiple,const Vector2 *pVec)
{
	assert( pMultiple != 0.0f );
	assert( pVec );
	x = pVec->x - fmodf(pVec->x,pMultiple);
	y = pVec->y - fmodf(pVec->y,pMultiple);
}

inline void Vector2::Floor()
{
	x = floorf(x);
	y = floorf(y);
}

inline void Vector2::Floor(const Vector2 *pVec)
{
	assert( pVec );
	x = floorf(pVec->x);
	y = floorf(pVec->y);
}

inline void Vector2::Ceiling()
{
	x = ceilf(x);
	y = ceilf(y);
}

inline void Vector2::Ceiling(const Vector2 *pVec)
{
	assert( pVec );
	x = ceilf(pVec->x);
	y = ceilf(pVec->y);
}

inline void Vector2::Mod(float pDiv)
{
	assert( pDiv != 0.0f );
	x = fmodf(x,pDiv);
	y = fmodf(y,pDiv);
}

inline void Vector2::Mod(float pDiv,const Vector2 *pVec)
{
	assert( pDiv != 0.0f );
	assert( pVec );
	x = fmodf(pVec->x,pDiv);
	y = fmodf(pVec->y,pDiv);
}
// ---------------------------------------------------------------------------
};//namespace BogDog{
