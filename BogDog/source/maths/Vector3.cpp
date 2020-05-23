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

#include "./maths/Vector3.h"
#include "./maths/Matrix.h"
#include "./maths/Maths.h"

namespace BogDog{
// ---------------------------------------------------------------------------
float Vector3::Norm()
{
float work,length;

	work = Dot(this);
	length = sqrtf(work);
	if( length > FLT_MIN )
	{
		work = 1.0f / length;

		x *= work;
		y *= work;
		z *= work;
	}

	return length;
}

float Vector3::Norm(const Vector3 &pV)
{
float length,work;

	assert(pV.IsValid());

	length = pV.Length();
	if( length > FLT_MIN )
	{
		work = 1.0f / length;

		x = pV.x * work;
		y = pV.y * work;
		z = pV.z * work;
	}

	return length;
}

float Vector3::Norm(const Vector3 &pFrom,const Vector3 &pTo)
{
	*this = pTo - pFrom;
	return Norm();
}

float Vector3::Norm(const Vector3 &pA,const Vector3 &pB,const Vector3 &pC)
{
Vector3 pba,pca;

	pba = pB - pA;
	pca = pC - pA;

	//Make the norm.
	Cross(pba,pca);

	//Done.
	return Norm();
}

float Vector3::Norm(float pX,float pY,float pZ)
{
float length,work;

	length = sqrtf( (pX*pX) + (pY*pY) + (pZ*pZ) );
	if( length > FLT_MIN )
	{
		work = 1.0f / length;

		x = pX * work;
		y = pY * work;
		z = pZ * work;
	}

	return length;
}

float Vector3::Norm(float *pFace,int pVertex_stride)
{
Vector3 a,b,c;

	assert( (pVertex_stride&3) == 0 );
	assert( pVertex_stride >= (int)(sizeof(float)*3) );
	pVertex_stride >>= 2;

	a.Set(pFace[0],pFace[1],pFace[2]);

	pFace += pVertex_stride;
	b.Set(pFace[0],pFace[1],pFace[2]);

	pFace += pVertex_stride;
	c.Set(pFace[0],pFace[1],pFace[2]);

	b = b - a;
	c = c - a;

	//Make the norm.
	Cross(b,c);

	//Done.
	return Norm();
}

Vector3 &Vector3::Cross(const Vector3 &pA,const Vector3 &pB)
{
	assert( pA.IsValid() );
	assert( pB.IsValid() );

	x = (pA.y * pB.z) - (pA.z * pB.y);
	y = (pA.z * pB.x) - (pA.x * pB.z);
	z = (pA.x * pB.y) - (pA.y * pB.x);

	return *this;
}

Vector3 &Vector3::Cross(const Vector3 &pV)
{
	assert( pV.IsValid() );

	float _x = (y * pV.z) - (z * pV.y);
	float _y = (z * pV.x) - (x * pV.z);
		   z = (x * pV.y) - (y * pV.x);

	x = _x;
	y = _y;

	return *this;
}

Vector3 &Vector3::Cross(float pX,float pY,float pZ)
{
	float _x = (y * pZ) - (z * pY);
	float _y = (z * pX) - (x * pZ);
		   z = (x * pY) - (y * pX);

	x = _x;
	y = _y;

	return *this;
}

float Vector3::DistToSq(const Vector3 *pTo) const
{
float ox,oy,oz;

	ox = pTo->x - x;
	oy = pTo->y - y;
	oz = pTo->z - z;

	return (ox*ox)+(oy*oy)+(oz*oz);
}

float Vector3::DistTo(const Vector3 *pTo) const
{
float ox,oy,oz;

	ox = pTo->x - x;
	oy = pTo->y - y;
	oz = pTo->z - z;

	return sqrtf( (ox*ox)+(oy*oy)+(oz*oz) );
}

float Vector3::DistToSq(float pX,float pY,float pZ)const
{
float ox,oy,oz;

	ox = pX - x;
	oy = pY - y;
	oz = pZ - z;

	return (ox*ox)+(oy*oy)+(oz*oz);
}

float Vector3::DistTo(float pX,float pY,float pZ)const
{
float ox,oy,oz;

	ox = pX - x;
	oy = pY - y;
	oz = pZ - z;

	return sqrtf( (ox*ox)+(oy*oy)+(oz*oz) );
}

float Vector3::DistToSq(const Matrix *pTo)const
{
float ox,oy,oz;

	assert( pTo );

	ox = pTo->m[3][0] - x;
	oy = pTo->m[3][1] - y;
	oz = pTo->m[3][2] - z;

	return (ox*ox)+(oy*oy)+(oz*oz);
}

float Vector3::DistTo(const Matrix *pTo)const
{
float ox,oy,oz;

	assert( pTo );

	ox = pTo->m[3][0] - x;
	oy = pTo->m[3][1] - y;
	oz = pTo->m[3][2] - z;

	return sqrtf( (ox*ox)+(oy*oy)+(oz*oz) );
}

float Vector3::DistFromEdgeSq(const Vector3 &pA,const Vector3 &pB,Vector3 *rNearest_point/* = NULL */) const
{
Vector3	point,dist;

	assert( pA.IsValid() );
	assert( pB.IsValid() );
	assert( &pA != &pB );//Make sure that someone is not silly.

	if( !rNearest_point )
		rNearest_point = &point;

	GetPointOnEdge(pA,pB,rNearest_point);

	dist.Sub(*rNearest_point,*this);

	return dist.LengthSq();
}

//////////////////////////////////////////////////////////////////////
//Matrix transform maths.
Vector3 *Vector3::MatrixMul(const Matrix *pMatrix)
{
	float _x = (pMatrix->m[0][0] * x) + (pMatrix->m[1][0] * y) + (pMatrix->m[2][0] * z) + pMatrix->m[3][0];
	float _y = (pMatrix->m[0][1] * x) + (pMatrix->m[1][1] * y) + (pMatrix->m[2][1] * z) + pMatrix->m[3][1];
		   z = (pMatrix->m[0][2] * x) + (pMatrix->m[1][2] * y) + (pMatrix->m[2][2] * z) + pMatrix->m[3][2];

	x = _x;
	y = _y;

	return this;
}

//does this = pIn * pMatrix
Vector3 *Vector3::MatrixMul(const Vector3 *pIn,const Matrix *pMatrix)//returns pOut.
{
	x = (pMatrix->m[0][0] * pIn->x) + (pMatrix->m[1][0] * pIn->y) + (pMatrix->m[2][0] * pIn->z) + pMatrix->m[3][0];
	y = (pMatrix->m[0][1] * pIn->x) + (pMatrix->m[1][1] * pIn->y) + (pMatrix->m[2][1] * pIn->z) + pMatrix->m[3][1];
	z = (pMatrix->m[0][2] * pIn->x) + (pMatrix->m[1][2] * pIn->y) + (pMatrix->m[2][2] * pIn->z) + pMatrix->m[3][2];

	return this;
}

//does this = (x,y,z) * pMatrix
Vector3 *Vector3::MatrixMul(float pX,float pY,float pZ,const Matrix *pMatrix)//returns pOut.
{
	x = (pMatrix->m[0][0] * pX) + (pMatrix->m[1][0] * pY) + (pMatrix->m[2][0] * pZ) + pMatrix->m[3][0];
	y = (pMatrix->m[0][1] * pX) + (pMatrix->m[1][1] * pY) + (pMatrix->m[2][1] * pZ) + pMatrix->m[3][1];
	z = (pMatrix->m[0][2] * pX) + (pMatrix->m[1][2] * pY) + (pMatrix->m[2][2] * pZ) + pMatrix->m[3][2];

	return this;
}

Vector3 *Vector3::MatrixMulInverse(const Vector3 *pIn,const Matrix *pMatrix)
{
Vector3 temp;

	assert( pMatrix );
	assert( pIn );

	temp.x = pIn->x - pMatrix->m[3][0];
	temp.y = pIn->y - pMatrix->m[3][1];
	temp.z = pIn->z - pMatrix->m[3][2];

	x = (pMatrix->m[0][0] * temp.x) + (pMatrix->m[0][1] * temp.y) + (pMatrix->m[0][2] * temp.z);
	y = (pMatrix->m[1][0] * temp.x) + (pMatrix->m[1][1] * temp.y) + (pMatrix->m[1][2] * temp.z);
	z = (pMatrix->m[2][0] * temp.x) + (pMatrix->m[2][1] * temp.y) + (pMatrix->m[2][2] * temp.z);

	return this;
}

//does this = (x,y,z) * inv pMatrix
Vector3 *Vector3::MatrixMulInverse(float pX,float pY,float pZ,const Matrix *pMatrix)
{
	assert( pMatrix );

	pX = pX - pMatrix->m[3][0];
	pY = pY - pMatrix->m[3][1];
	pZ = pZ - pMatrix->m[3][2];

	x = (pMatrix->m[0][0] * pX) + (pMatrix->m[0][1] * pY) + (pMatrix->m[0][2] * pZ);
	y = (pMatrix->m[1][0] * pX) + (pMatrix->m[1][1] * pY) + (pMatrix->m[1][2] * pZ);
	z = (pMatrix->m[2][0] * pX) + (pMatrix->m[2][1] * pY) + (pMatrix->m[2][2] * pZ);

	return this;
}

Vector3 *Vector3::MatrixMulInverse(const Matrix *pMatrix)
{
	assert( pMatrix );

	x = x - pMatrix->m[3][0];
	y = y - pMatrix->m[3][1];
	z = z - pMatrix->m[3][2];


	float _x = (pMatrix->m[0][0] * x) + (pMatrix->m[0][1] * y) + (pMatrix->m[0][2] * z);
	float _y = (pMatrix->m[1][0] * x) + (pMatrix->m[1][1] * y) + (pMatrix->m[1][2] * z);
		   z = (pMatrix->m[2][0] * x) + (pMatrix->m[2][1] * y) + (pMatrix->m[2][2] * z);

	x = _x;
	y = _y;

	return this;
}

//////////////////////////////////////////////////////////////////////
//Matrix rotations, the matrix does not translate it.
Vector3 *Vector3::MatrixRot(const Matrix *pMatrix)
{
	float _x = (pMatrix->m[0][0] * x) + (pMatrix->m[1][0] * y) + (pMatrix->m[2][0] * z);
	float _y = (pMatrix->m[0][1] * x) + (pMatrix->m[1][1] * y) + (pMatrix->m[2][1] * z);
		   z = (pMatrix->m[0][2] * x) + (pMatrix->m[1][2] * y) + (pMatrix->m[2][2] * z);

	x = _x;
	y = _y;

	return this;
}

//does this = pIn * pMatrix
Vector3 *Vector3::MatrixRot(const Vector3 *pIn,const Matrix *pMatrix)//returns pOut.
{
	x = (pMatrix->m[0][0] * pIn->x) + (pMatrix->m[1][0] * pIn->y) + (pMatrix->m[2][0] * pIn->z);
	y = (pMatrix->m[0][1] * pIn->x) + (pMatrix->m[1][1] * pIn->y) + (pMatrix->m[2][1] * pIn->z);
	z = (pMatrix->m[0][2] * pIn->x) + (pMatrix->m[1][2] * pIn->y) + (pMatrix->m[2][2] * pIn->z);

	return this;
}

	//does this = (x,y,z) * pMatrix
Vector3 *Vector3::MatrixRot(float pX,float pY,float pZ,const Matrix *pMatrix)//returns this.
{
	x = (pMatrix->m[0][0] * pX) + (pMatrix->m[1][0] * pY) + (pMatrix->m[2][0] * pZ);
	y = (pMatrix->m[0][1] * pX) + (pMatrix->m[1][1] * pY) + (pMatrix->m[2][1] * pZ);
	z = (pMatrix->m[0][2] * pX) + (pMatrix->m[1][2] * pY) + (pMatrix->m[2][2] * pZ);

	return this;
}

Vector3 *Vector3::MatrixRotInverse(const Vector3 *pIn,const Matrix *pMatrix)
{
	assert( pMatrix );
	assert( pIn );

	x = (pMatrix->m[0][0] * pIn->x) + (pMatrix->m[0][1] * pIn->y) + (pMatrix->m[0][2] * pIn->z);
	y = (pMatrix->m[1][0] * pIn->x) + (pMatrix->m[1][1] * pIn->y) + (pMatrix->m[1][2] * pIn->z);
	z = (pMatrix->m[2][0] * pIn->x) + (pMatrix->m[2][1] * pIn->y) + (pMatrix->m[2][2] * pIn->z);

	return this;
}

//does this = (x,y,z) * inv pMatrix
Vector3 *Vector3::MatrixRotInverse(float pX,float pY,float pZ,const Matrix *pMatrix)
{
	assert( pMatrix );

	x = (pMatrix->m[0][0] * pX) + (pMatrix->m[0][1] * pY) + (pMatrix->m[0][2] * pZ);
	y = (pMatrix->m[1][0] * pX) + (pMatrix->m[1][1] * pY) + (pMatrix->m[1][2] * pZ);
	z = (pMatrix->m[2][0] * pX) + (pMatrix->m[2][1] * pY) + (pMatrix->m[2][2] * pZ);

	return this;
}

Vector3 *Vector3::MatrixRotInverse(const Matrix *pMatrix)
{
	assert( pMatrix );

	float _x = (pMatrix->m[0][0] * x) + (pMatrix->m[0][1] * y) + (pMatrix->m[0][2] * z);
	float _y = (pMatrix->m[1][0] * x) + (pMatrix->m[1][1] * y) + (pMatrix->m[1][2] * z);
		   z = (pMatrix->m[2][0] * x) + (pMatrix->m[2][1] * y) + (pMatrix->m[2][2] * z);

	x = _x;
	y = _y;

	return this;
}

Vector3 *Vector3::MatrixMulCatmullRom(const Matrix &pCoefficients,const float pTime)
{
float t_cube;
float t_square;

	t_square = pTime * pTime;
	t_cube = t_square * pTime;

	x = (pCoefficients.m[0][0] * t_cube) + (pCoefficients.m[1][0] * t_square) + (pCoefficients.m[2][0] * pTime) + pCoefficients.m[3][0];
	y = (pCoefficients.m[0][1] * t_cube) + (pCoefficients.m[1][1] * t_square) + (pCoefficients.m[2][1] * pTime) + pCoefficients.m[3][1];
	z = (pCoefficients.m[0][2] * t_cube) + (pCoefficients.m[1][2] * t_square) + (pCoefficients.m[2][2] * pTime) + pCoefficients.m[3][2];

	return this;
}

void Vector3::Tween(const Vector3 *pTo,float pAmount)
{
float dx,dy,dz;

	dx = pTo->x - x;
	dy = pTo->y - y;
	dz = pTo->z - z;

	dx *= pAmount;
	dy *= pAmount;
	dz *= pAmount;

	x += dx;
	y += dy;
	z += dz;
}

void Vector3::Tween(const Vector3 *pFrom,const Vector3 *pTo,float pAmount)
{
float dx,dy,dz;

	dx = pTo->x - pFrom->x;
	dy = pTo->y - pFrom->y;
	dz = pTo->z - pFrom->z;

	dx *= pAmount;
	dy *= pAmount;
	dz *= pAmount;

	x = pFrom->x + dx;
	y = pFrom->y + dy;
	z = pFrom->z + dz;
}

//Returns zero if the diff between the two is with in pFactor.
int Vector3::Compare(const Vector3 &pA,float pFactor/*= 0.001f*/) const
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

	diff = z - pA.z;
	if( diff < -pFactor )
		return -1;

	if( diff > pFactor )
		return 1;

	return 0;
}

	//This is very handy func, makes a ray from the front clip plane out in to the scene based on screen pos.
void Vector3::MakeViewportRay(
						float pScreen_x,
						float pScreen_y,
						const Matrix *pCamera,
						float pFov,
						float pViewport_width,
						float pViewport_height)
{
float the_atan;
float x_scale,y_scale;

		the_atan = tanf(pFov*DEGTORAD);
		x_scale = the_atan * (2*pScreen_x-pViewport_width) / pViewport_width;
		y_scale = -the_atan * (2*pScreen_y-pViewport_height) / pViewport_width;// should be width to account for aspect

		AddScale( (*pCamera)[2],(*pCamera)[0],x_scale);
		AddScale( (*pCamera)[1],y_scale);
}

//Rounds the vectors xyz to the nearest parts.
void Vector3::RoundToNearest()
{
	md_Maths_RoundToNearest(&x);
	md_Maths_RoundToNearest(&y);
	md_Maths_RoundToNearest(&z);
}

void Vector3::RoundToNearest(const Vector3 *pVec)
{
	x = md_Maths_RoundToNearest(pVec->x);
	y = md_Maths_RoundToNearest(pVec->y);
	z = md_Maths_RoundToNearest(pVec->z);
}

void Vector3::SetToSphereSurface(const Vector3 *pSphere_center,const float pSphere_radious)
{
Vector3 offset;
float d;

	assert( pSphere_center );
	d = offset.Norm(*pSphere_center,*this);
	if( d < 0.1f )
	{
		x = pSphere_center->x + pSphere_radious;
		y = pSphere_center->y + pSphere_radious;
		z = pSphere_center->z + pSphere_radious;
	}
	else
	{
		x = pSphere_center->x + ( offset.x * pSphere_radious);
		y = pSphere_center->y + ( offset.y * pSphere_radious);
		z = pSphere_center->z + ( offset.z * pSphere_radious);
	}
}

//Returns alpha, which is the percent that the vec lies along the edge, zero of one then point is not along edge.
//   p										x
//	  \										|
//     x--------x   p not on edge,		x---n---x	p on edge, rNearest_point == n
float Vector3::GetPointOnEdge(const Vector3 &pA,const Vector3 &pB,Vector3 *rNearest_point)const
{
Vector3	dir, sep, dist, point;
float alpha;

	assert( pA.IsValid() );
	assert( pB.IsValid() );
	assert( &pA != &pB );//Make sure that someone is not silly.

	dir.Sub(pB,pA);

	sep.Sub(*this,pA);

	alpha = sep.Dot(&dir) / dir.LengthSq();
	if( alpha <= 0 )
	{
		*rNearest_point = pA;
	}
	else if( alpha >= 1 )
	{
		*rNearest_point = pB;
	}
	else
	{
		rNearest_point->AddScale(pA,dir,alpha);
	}

	return alpha;
}

//Vector rotation.
Vector3 *Vector3::RotateX(Vector3 *pVec,float pX)
{
float cA,sA;

	assert(pVec);

	pX *= DEGTORAD;
	cA = cosf(pX);
	sA = sinf(pX);

	x = pVec->x;
	y = (cA * pVec->y) + (-sA * pVec->z);
	z = (sA * pVec->y) + ( cA * pVec->z);

	return this;
}

Vector3 *Vector3::RotateX(float pX)
{
float cA,sA;

	pX *= DEGTORAD;
	cA = cosf(pX);
	sA = sinf(pX);

	//Have to reserve y as its used in the next bit.
	float _y = (cA * y) + (-sA * z);
		   z = (sA * y) + ( cA * z);

	y = _y;

	return this;
}


Vector3 *Vector3::RotateY(Vector3 *pVec,float pY)
{
float cA,sA;

	assert(pVec);

	pY *= DEGTORAD;
	cA = cosf(pY);
	sA = sinf(pY);

	x = ( cA * pVec->x) + (sA * pVec->z);
	y = pVec->y;
	z = (-sA * pVec->x) + (cA * pVec->z);

	return this;
}

Vector3 *Vector3::RotateY(float pY)
{
float cA,sA;

	pY *= DEGTORAD;
	cA = cosf(pY);
	sA = sinf(pY);

	float _x = ( cA * x) + (sA * z);
	       z = (-sA * x) + (cA * z);

	x = _x;

	return this;
}

Vector3 *Vector3::RotateZ(Vector3 *pVec,float pZ)
{
float cA,sA;

	assert(pVec);

	pZ *= DEGTORAD;
	cA = cosf(pZ);
	sA = sinf(pZ);

	x = (cA * pVec->x) + (-sA * pVec->y);
	y = (sA * pVec->x) + ( cA * pVec->y);
	z = pVec->z;

	return this;
}

Vector3 *Vector3::RotateZ(float pZ)
{
float cA,sA;


	pZ *= DEGTORAD;
	cA = cosf(pZ);
	sA = sinf(pZ);

	float _x = (cA * x) + (-sA * y);
		   y = (sA * x) + ( cA * y);

	x = _x;

	return this;
}

//Static util funtions.
//The passed indices array should have the same number of members as the points.
//The indices are mappings to the unique verts. So say vector[2] and vector[3] are the same
//Index[2] and Index[3] will == 2.
static Vector3 *pSort_array = 0;
static int sort_vectors_on_indices(const void *pA,const void *pB)
{
	return pSort_array[*((int*)pA)].Compare(pSort_array[*((int*)pB)]);
}

int Vector3::MakeUniqueIndexArray(Vector3 *pPoints,int pNum_points,int *rIndices)
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
void Vector3::MakeUniqueVectorArray(Vector3 *pPoints,int pNum_points,
									Vector3 **rUnique,int *rNum_unique,
									int **rIndex_list)//rIndex_list will take you bake to the orginal list.
{
int *indices;
int n,n2;

	*rIndex_list = new int[pNum_points];

	indices = new int[pNum_points];

	*rNum_unique = MakeUniqueIndexArray(pPoints,pNum_points,indices);
	assert( *rNum_unique > 0 && *rNum_unique <= pNum_points );

	//Now make the unique vertices.
	*rUnique = new Vector3[*rNum_unique];

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
