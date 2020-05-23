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

#include "./maths/Box.h"
#include "./maths/Matrix.h"

namespace BogDog
{

// ---------------------------------------------------------------------------

//Makes the box big enough to encompuss the point.
Bounds::Bounds(const Vector3 &pMin,const Vector3 &pMax)
{
	assert( pMin.IsValid() );
	assert( pMax.IsValid() );

	min = pMin;
	max = pMax;
}

Bounds::Bounds(const Vector3 &pCenter,float pSize)
{
	assert( pCenter.IsValid() );
	Make(pCenter,pSize);
}

Bounds::Bounds(const Vector3 &pSize)
{
	Make(pSize);
}

Bounds::Bounds(float pSize)
{
	Make(pSize);
}

bool Bounds::IsNotNULL()const
{
	if( this != NULL )
		return true;
	return false;
}

bool Bounds::IsValid()const
{
	if( IsNotNULL() && min.IsValid() && max.IsValid() )
		return true;

	return false;
}

void Bounds::Grow(const Vector3 *pPoint)
{
	Grow( pPoint->x,pPoint->y,pPoint->z);
}

void Bounds::Grow(const Bounds *pBounds)
{
	//Make sure the box comming in is valid.
	assert( pBounds );
	assert( pBounds->min.x <= pBounds->max.x );
	assert( pBounds->min.y <= pBounds->max.y );
	assert( pBounds->min.z <= pBounds->max.z );

	if( min.x > pBounds->min.x )
		min.x = pBounds->min.x;

	if( min.y > pBounds->min.y )
		min.y = pBounds->min.y;

	if( min.z > pBounds->min.z )
		min.z = pBounds->min.z;

	if( max.x < pBounds->max.x )
		max.x = pBounds->max.x;

	if( max.y < pBounds->max.y )
		max.y = pBounds->max.y;

	if( max.z < pBounds->max.z )
		max.z = pBounds->max.z;

}

void Bounds::Grow(float pX,float pY,float pZ)
{
	if( min.x > pX )
		min.x = pX;
	else if( max.x < pX )
		max.x = pX;

	if( min.y > pY )
		min.y = pY;
	else if( max.y < pY )
		max.y = pY;

	if( min.z > pZ )
		min.z = pZ;
	else if( max.z < pZ )
		max.z = pZ;
}

void Bounds::Scale(float pScale)
{
Vector3 p,s;

	GetCenter(&p);
	GetSize(&s);
	s *= pScale;
	Make(p,s);
}

void Bounds::Scale(float pX_scale,float pY_scale,float pZ_scale)
{
Vector3 p,s;

	GetCenter(&p);
	GetSize(&s);
	s.x *= pX_scale;
	s.y *= pY_scale;
	s.z *= pZ_scale;
	Make(p,s);
}

//Sets min max to the first point and calls Grow on the rest.
void Bounds::Make(const Vector3 *pPoints,int pNum_points)
{
	assert( pPoints );
	assert( pNum_points );

	min = pPoints[0];
	max = pPoints[0];
	while( pNum_points-- )
	{
		Grow(pPoints + pNum_points);
	};
}

void Bounds::Make(const Vector3 &pCenter,const Vector3 &pSize)
{
	assert( pCenter.IsValid() );
	assert( pSize.IsValid() );

	min = pCenter;
	max = pCenter;

	min -= (pSize * 0.5f);
	max += (pSize * 0.5f);
}

void Bounds::Make(const Vector3 &pCenter,float pSize)
{
Vector3 size(pSize,pSize,pSize);

	Make(pCenter,size);
}

void Bounds::Make(const Vector3 &pSize)
{
Vector3 c(0,0,0);
	Make(c,pSize);
}

void Bounds::Make(float pCenter_x,float pCenter_y,float pCenter_z,float pSize_x,float pSize_y,float pSize_z)
{
	pSize_x *= 0.5f;
	pSize_y *= 0.5f;
	pSize_z *= 0.5f;

	min.x = pCenter_x - pSize_x;
	min.y = pCenter_y - pSize_y;
	min.z = pCenter_z - pSize_z;

	max.x = pCenter_x + pSize_x;
	max.y = pCenter_y + pSize_y;
	max.z = pCenter_z + pSize_z;
}

void Bounds::Make(float pSize_x,float pSize_y,float pSize_z)
{
	pSize_x *= 0.5f;
	pSize_y *= 0.5f;
	pSize_z *= 0.5f;

	min.x = -pSize_x;
	min.y = -pSize_y;
	min.z = -pSize_z;

	max.x = +pSize_x;
	max.y = +pSize_y;
	max.z = +pSize_z;
}

void Bounds::Make(float pSize)
{
Vector3 size(pSize,pSize,pSize),c(0,0,0);
	Make(c,size);
}

//Get functions
void Bounds::GetFacePosX(Vector3 *rTop_left,Vector3 *rTop_right,Vector3 *rBottom_right,Vector3 *rBottom_left)const
{
	assert( rTop_left );
	assert( rTop_right );
	assert( rBottom_right );
	assert( rBottom_left );

	rTop_left->Set(		max.x, max.y, min.z );
	rTop_right->Set(	max.x, max.y, max.z );
	rBottom_right->Set(	max.x, min.y, max.z );
	rBottom_left->Set(	max.x, min.y, min.z );
}

void Bounds::GetFaceNegX(Vector3 *rTop_left,Vector3 *rTop_right,Vector3 *rBottom_right,Vector3 *rBottom_left)const
{
	assert( rTop_left );
	assert( rTop_right );
	assert( rBottom_right );
	assert( rBottom_left );

	rTop_left->Set(		min.x, max.y, max.z );
	rTop_right->Set(	min.x, max.y, min.z );
	rBottom_right->Set(	min.x, min.y, min.z );
	rBottom_left->Set(	min.x, min.y, max.z );
}

void Bounds::GetFacePosY(Vector3 *rTop_left,Vector3 *rTop_right,Vector3 *rBottom_right,Vector3 *rBottom_left)const
{
	assert( rTop_left );
	assert( rTop_right );
	assert( rBottom_right );
	assert( rBottom_left );

	rTop_left->Set(		min.x, max.y, max.z );
	rTop_right->Set(	max.x, max.y, max.z );
	rBottom_right->Set(	max.x, max.y, min.z );
	rBottom_left->Set(	min.x, max.y, min.z );
}

void Bounds::GetFaceNegY(Vector3 *rTop_left,Vector3 *rTop_right,Vector3 *rBottom_right,Vector3 *rBottom_left)const
{
	assert( rTop_left );
	assert( rTop_right );
	assert( rBottom_right );
	assert( rBottom_left );

	rTop_left->Set(		min.x, min.y, min.z );
	rTop_right->Set(	max.x, min.y, min.z );
	rBottom_right->Set(	max.x, min.y, max.z );
	rBottom_left->Set(	min.x, min.y, max.z );
}

void Bounds::GetFacePosZ(Vector3 *rTop_left,Vector3 *rTop_right,Vector3 *rBottom_right,Vector3 *rBottom_left)const
{
	assert( rTop_left );
	assert( rTop_right );
	assert( rBottom_right );
	assert( rBottom_left );

	rTop_left->Set(		max.x, max.y, max.z );
	rTop_right->Set(	min.x, max.y, max.z );
	rBottom_right->Set(	min.x, min.y, max.z );
	rBottom_left->Set(	max.x, min.y, max.z );
}

void Bounds::GetFaceNegZ(Vector3 *rTop_left,Vector3 *rTop_right,Vector3 *rBottom_right,Vector3 *rBottom_left)const
{
	assert( rTop_left );
	assert( rTop_right );
	assert( rBottom_right );
	assert( rBottom_left );

	rTop_left->Set(		min.x, max.y, min.z );
	rTop_right->Set(	max.x, max.y, min.z );
	rBottom_right->Set(	max.x, min.y, min.z );
	rBottom_left->Set(	min.x, min.y, min.z );
}

void Bounds::GetSize(Vector3 *rSize)const
{
	assert( rSize );
	*rSize = max - min;
}

void Bounds::GetCenter(Vector3 *rCenter)const
{
	assert( rCenter );

	*rCenter = min + ((max-min)*0.5f);
}

void Bounds::GetCorner(int pCorner,Vector3 *rCorner)const
{
	assert( rCorner );

	if( pCorner&BOX_CORNER_MAX_X )
		rCorner->x = max.x;
	else
		rCorner->x = min.x;

	if( pCorner&BOX_CORNER_MAX_Y )
		rCorner->y = max.y;
	else
		rCorner->y = min.y;

	if( pCorner&BOX_CORNER_MAX_Z )
		rCorner->z = max.z;
	else
		rCorner->z = min.z;
}

void Bounds::GetCorner(int pCorner,Vector3 *rCorner,Vector3 *rX_dir,Vector3 *rY_dir,Vector3 *rZ_dir)const
{
int f;
	GetCorner(pCorner,rCorner);

	f = pCorner;
	if( f&BOX_CORNER_MAX_X )
		f &= ~BOX_CORNER_MAX_X;
	else
		f |= BOX_CORNER_MAX_X;
	GetCorner(f,rX_dir);

	f = pCorner;
	if( f&BOX_CORNER_MAX_Y )
		f &= ~BOX_CORNER_MAX_Y;
	else
		f |= BOX_CORNER_MAX_Y;
	GetCorner(f,rY_dir);

	f = pCorner;
	if( f&BOX_CORNER_MAX_Z )
		f &= ~BOX_CORNER_MAX_Z;
	else
		f |= BOX_CORNER_MAX_Z;
	GetCorner(f,rZ_dir);

	(*rX_dir) = (*rX_dir) - (*rCorner);
	(*rY_dir) = (*rY_dir) - (*rCorner);
	(*rZ_dir) = (*rZ_dir) - (*rCorner);
}

//Matrix mul stuff, used to make axis aligned boxes.
Bounds *Bounds::MatrixMul(const Matrix *pMatrix)//Returns the 'this' pointer.
{
float temp;
Vector3 a;
int j,k;

	assert( pMatrix );

	GetSize(&a);

	min.MatrixMul(pMatrix);
	max = min;


	for( j = 0 ; j < 3 ; j++ )
	{
		for( k = 0 ; k < 3 ; k++ )
		{
			temp = pMatrix->m[k][j] * a[k];
			if( temp < 0 )
				min[j] += temp;
			else
				max[j] += temp;
		}
	}

	return this;
}

Bounds *Bounds::MatrixMul(const Matrix *pMatrix,const Bounds *pBounds)//Returns the 'this' pointer.
{
float temp;
Vector3 a;
int j,k;

	assert( pMatrix );
	assert( pBounds );

	pBounds->GetSize(&a);

	min.MatrixMul(&pBounds->min,pMatrix);
	max = min;

	for( j = 0 ; j < 3 ; j++ )
	{
		for( k = 0 ; k < 3 ; k++ )
		{
			temp = pMatrix->m[k][j] * a[k];
			if( temp < 0 )
				min[j] += temp;
			else
				max[j] += temp;
		}
	}

	return this;
}

//Some intersection functions.
int Bounds::IntersectsAA(const Bounds &pBounds)const//Seperating axis test.
{
Vector3 T,center_a,center_b,size_a,size_b;

	assert( pBounds.IsValid() );

	GetCenter(&center_a);
	GetSize(&size_a);
	size_a *= 0.5f;

	pBounds.GetCenter(&center_b);
	pBounds.GetSize(&size_b);
	size_b *= 0.5f;

	T = center_b - center_a;

	return fabsf(T.x) < (size_a.x + size_b.x) && fabsf(T.y) < (size_a.y + size_b.y) && fabsf(T.z) < (size_a.z + size_b.z);
}

int Bounds::IntersectsAA(const Bounds &pBounds,Vector3 &rSeperating_axis)const
{
Vector3 T,center_a,center_b,size_a,size_b;

	assert( pBounds.IsValid() );
	assert( rSeperating_axis.IsValid() );

	GetCenter(&center_a);
	GetSize(&size_a);
	size_a *= 0.5f;

	pBounds.GetCenter(&center_b);
	pBounds.GetSize(&size_b);
	size_b *= 0.5f;

	T = center_b - center_a;

	rSeperating_axis.x = (fabsf(T.x) - (size_a.x + size_b.x)) / (size_a.x + size_b.x);
	rSeperating_axis.y = (fabsf(T.y) - (size_a.y + size_b.y)) / (size_a.y + size_b.y);
	rSeperating_axis.z = (fabsf(T.z) - (size_a.z + size_b.z)) / (size_a.z + size_b.z);


	if( rSeperating_axis.x < 0.0f &&
		rSeperating_axis.y < 0.0f &&
		rSeperating_axis.z < 0.0f )
		return 1;

	return 0;
}
// ---------------------------------------------------------------------------
};//namespace BogDog{
