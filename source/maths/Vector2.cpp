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

#include <math.h>
#include <assert.h>
#include <float.h>

#include "./maths/Vector2.h"
#include "./maths/Matrix.h"
#include "./maths/Maths.h"

namespace BogDog{
// ---------------------------------------------------------------------------

float Vector2::DistToSq(const Vector2 *pTo) const
{
float ox,oy;

	ox = pTo->x - x;
	oy = pTo->y - y;

	return (ox*ox)+(oy*oy);
}

float Vector2::DistTo(const Vector2 *pTo) const
{
float ox,oy;

	ox = pTo->x - x;
	oy = pTo->y - y;

	return sqrtf( (ox*ox)+(oy*oy));
}

float Vector2::DistToSq(float pX,float pY)const
{
float ox,oy;

	ox = pX - x;
	oy = pY - y;

	return (ox*ox)+(oy*oy);
}

float Vector2::DistTo(float pX,float pY)const
{
float ox,oy;

	ox = pX - x;
	oy = pY - y;

	return sqrtf( (ox*ox)+(oy*oy));
}

float Vector2::DistToSq(const Matrix *pTo)const
{
float ox,oy;

	assert( pTo );

	ox = pTo->m[3][0] - x;
	oy = pTo->m[3][1] - y;

	return (ox*ox)+(oy*oy);
}

float Vector2::DistTo(const Matrix *pTo)const
{
float ox,oy;

	assert( pTo );

	ox = pTo->m[3][0] - x;
	oy = pTo->m[3][1] - y;


	return sqrtf( (ox*ox)+(oy*oy));
}


//////////////////////////////////////////////////////////////////////
//Matrix transform maths.
Vector2 *Vector2::MatrixMul(const Matrix *pMatrix)
{
	float _x = (pMatrix->m[0][0] * x) + (pMatrix->m[1][0] * y) + pMatrix->m[3][0];
	float _y = (pMatrix->m[0][1] * x) + (pMatrix->m[1][1] * y) + pMatrix->m[3][1];

	x = _x;
	y = _y;

	return this;
}

//does this = pIn * pMatrix
Vector2 *Vector2::MatrixMul(const Vector2 *pIn,const Matrix *pMatrix)//returns pOut.
{
	x = (pMatrix->m[0][0] * pIn->x) + (pMatrix->m[1][0] * pIn->y) + pMatrix->m[3][0];
	y = (pMatrix->m[0][1] * pIn->x) + (pMatrix->m[1][1] * pIn->y) + pMatrix->m[3][1];

	return this;
}

//does this = (x,y) * pMatrix
Vector2 *Vector2::MatrixMul(float pX,float pY,const Matrix *pMatrix)//returns pOut.
{
	x = (pMatrix->m[0][0] * pX) + (pMatrix->m[1][0] * pY) + pMatrix->m[3][0];
	y = (pMatrix->m[0][1] * pX) + (pMatrix->m[1][1] * pY) + pMatrix->m[3][1];

	return this;
}

Vector2 *Vector2::MatrixMulInverse(const Vector2 *pIn,const Matrix *pMatrix)
{
Vector2 temp;

	assert( pMatrix );
	assert( pIn );

	temp.x = pIn->x - pMatrix->m[3][0];
	temp.y = pIn->y - pMatrix->m[3][1];

	x = (pMatrix->m[0][0] * temp.x) + (pMatrix->m[0][1] * temp.y);
	y = (pMatrix->m[1][0] * temp.x) + (pMatrix->m[1][1] * temp.y);

	return this;
}

//does this = (x,y,z) * inv pMatrix
Vector2 *Vector2::MatrixMulInverse(float pX,float pY,const Matrix *pMatrix)
{
	assert( pMatrix );

	pX = pX - pMatrix->m[3][0];
	pY = pY - pMatrix->m[3][1];

	x = (pMatrix->m[0][0] * pX) + (pMatrix->m[0][1] * pY);
	y = (pMatrix->m[1][0] * pX) + (pMatrix->m[1][1] * pY);

	return this;
}

Vector2 *Vector2::MatrixMulInverse(const Matrix *pMatrix)
{
	assert( pMatrix );

	x = x - pMatrix->m[3][0];
	y = y - pMatrix->m[3][1];


	float _x = (pMatrix->m[0][0] * x) + (pMatrix->m[0][1] * y);
	float _y = (pMatrix->m[1][0] * x) + (pMatrix->m[1][1] * y);

	x = _x;
	y = _y;

	return this;
}

//////////////////////////////////////////////////////////////////////
//Matrix rotations, the matrix does not translate it.
Vector2 *Vector2::MatrixRot(const Matrix *pMatrix)
{
	float _x = (pMatrix->m[0][0] * x) + (pMatrix->m[1][0] * y);
	float _y = (pMatrix->m[0][1] * x) + (pMatrix->m[1][1] * y);

	x = _x;
	y = _y;

	return this;
}

//does this = pIn * pMatrix
Vector2 *Vector2::MatrixRot(const Vector2 *pIn,const Matrix *pMatrix)//returns pOut.
{
	x = (pMatrix->m[0][0] * pIn->x) + (pMatrix->m[1][0] * pIn->y);
	y = (pMatrix->m[0][1] * pIn->x) + (pMatrix->m[1][1] * pIn->y);

	return this;
}

	//does this = (x,y,z) * pMatrix
Vector2 *Vector2::MatrixRot(float pX,float pY,const Matrix *pMatrix)//returns this.
{
	x = (pMatrix->m[0][0] * pX) + (pMatrix->m[1][0] * pY);
	y = (pMatrix->m[0][1] * pX) + (pMatrix->m[1][1] * pY);

	return this;
}

Vector2 *Vector2::MatrixRotInverse(const Vector2 *pIn,const Matrix *pMatrix)
{
	assert( pMatrix );
	assert( pIn );

	x = (pMatrix->m[0][0] * pIn->x) + (pMatrix->m[0][1] * pIn->y);
	y = (pMatrix->m[1][0] * pIn->x) + (pMatrix->m[1][1] * pIn->y);

	return this;
}

//does this = (x,y,z) * inv pMatrix
Vector2 *Vector2::MatrixRotInverse(float pX,float pY,const Matrix *pMatrix)
{
	assert( pMatrix );

	x = (pMatrix->m[0][0] * pX) + (pMatrix->m[0][1] * pY);
	y = (pMatrix->m[1][0] * pX) + (pMatrix->m[1][1] * pY);
	return this;
}

Vector2 *Vector2::MatrixRotInverse(const Matrix *pMatrix)
{
	assert( pMatrix );

	float _x = (pMatrix->m[0][0] * x) + (pMatrix->m[0][1] * y);
	float _y = (pMatrix->m[1][0] * x) + (pMatrix->m[1][1] * y);

	x = _x;
	y = _y;

	return this;
}

Vector2 *Vector2::MatrixMulCatmullRom(const Matrix &pCoefficients,const float pTime)
{
float t_cube;
float t_square;

	t_square = pTime * pTime;
	t_cube = t_square * pTime;

	x = (pCoefficients.m[0][0] * t_cube) + (pCoefficients.m[1][0] * t_square) + (pCoefficients.m[2][0] * pTime) + pCoefficients.m[3][0];
	y = (pCoefficients.m[0][1] * t_cube) + (pCoefficients.m[1][1] * t_square) + (pCoefficients.m[2][1] * pTime) + pCoefficients.m[3][1];

	return this;
}

void Vector2::Tween(const Vector2 *pTo,float pAmount)
{
float dx,dy;

	dx = pTo->x - x;
	dy = pTo->y - y;

	dx *= pAmount;
	dy *= pAmount;

	x += dx;
	y += dy;
}

void Vector2::Tween(const Vector2 *pFrom,const Vector2 *pTo,float pAmount)
{
float dx,dy;

	dx = pTo->x - pFrom->x;
	dy = pTo->y - pFrom->y;

	dx *= pAmount;
	dy *= pAmount;

	x = pFrom->x + dx;
	y = pFrom->y + dy;

}

//Returns zero if the diff between the two is with in pFactor.
int Vector2::Compare(const Vector2 &pA,float pFactor/*= 0.001f*/) const
{
float diff;

	diff = x - pA.x;
	if( diff < -pFactor )
		return -1;

	if( diff > pFactor )
		return 1;

	diff = y - pA.y;
	if( diff < -pFactor )
		return -1;

	if( diff > pFactor )
		return 1;

	return 0;
}


//Rounds the vectors xyz to the nearest parts.
void Vector2::RoundToNearest()
{
	md_Maths_RoundToNearest(&x);
	md_Maths_RoundToNearest(&y);
}

void Vector2::RoundToNearest(const Vector2 *pVec)
{
	x = md_Maths_RoundToNearest(pVec->x);
	y = md_Maths_RoundToNearest(pVec->y);
}

//Static util funtions.
//The passed indices array should have the same number of members as the points.
//The indices are mappings to the unique verts. So say vector[2] and vector[3] are the same
//Index[2] and Index[3] will == 2.
static Vector2 *pSort_array = 0;
static int sort_vectors_on_indices(const void *pA,const void *pB)
{
	return pSort_array[*((int*)pA)].Compare(pSort_array[*((int*)pB)]);
}

int Vector2::MakeUniqueIndexArray(Vector2 *pPoints,int pNum_points,int *rIndices)
{
int n,n2;
int *sorted_indices;
int num_unique_vertices;

	assert( pPoints && pNum_points > 0 && rIndices );

	sorted_indices = new int[pNum_points];

	//Fill the index list and use it for the qsort that speeds up the process.
	for( n = 0 ; n < pNum_points ; n++ )
	{
		sorted_indices[n] = n;
	}

	pSort_array = pPoints;
	qsort(sorted_indices,pNum_points,sizeof(int),sort_vectors_on_indices);
	pSort_array = 0;

	//Now the indices are in vector order, now overwrite the duplicates.
	num_unique_vertices = 0;
	for( n = 0 ; n < pNum_points ; n = n2 )
	{
		rIndices[sorted_indices[n]] = sorted_indices[n];
		num_unique_vertices++;
		n2 = n+1;
		for( ; n2 < pNum_points ; n2++ )
		{
			if( pPoints[sorted_indices[n]].Compare(pPoints[sorted_indices[n2]]) == 0 )
			{
				rIndices[sorted_indices[n2]] = sorted_indices[n];
			}
		}
	}

	delete []sorted_indices;

	return num_unique_vertices;
}

//This func will alloc a new array of vectors for you from the data comming from above.
void Vector2::MakeUniqueVectorArray(Vector2 *pPoints,int pNum_points,
									Vector2 **rUnique,int *rNum_unique,
									int **rIndex_list)//rIndex_list will take you bake to the orginal list.
{
int *indices;
int n,n2;

	*rIndex_list = new int[pNum_points];

	indices = new int[pNum_points];

	*rNum_unique = MakeUniqueIndexArray(pPoints,pNum_points,indices);
	assert( *rNum_unique > 0 && *rNum_unique <= pNum_points );

	//Now make the unique vertices.
	*rUnique = new Vector2[*rNum_unique];

	n2 = 0;
	(*rUnique)[0] = pPoints[indices[0]];
	*rIndex_list[indices[0]] = 0;
	n2++;
	for( n = 1 ; n < pNum_points ; n++ )
	{
		*rIndex_list[indices[n]] = n2-1;
		if( indices[n] != indices[n2-1] )//The n2-1 is so we test against the last added indexed vector.
		{
			(*rUnique)[n2] = pPoints[indices[n]];
			n2++;
		}
	}

	delete []indices;
}

// ---------------------------------------------------------------------------
};//namespace BogDog{
