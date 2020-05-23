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
#include "./maths/Maths.h"
#include "./maths/Vector3.h"

namespace BogDog{
// ---------------------------------------------------------------------------

void md_Maths_GetDimensionsInCameraSpace(float pFov,float pAspect,float pDepth,float *rXScale,float *rYScale)
{
	assert( pAspect > 0 );
	assert( pDepth > 0 );
	assert( pFov > 1 );

	assert( rXScale );
	assert( rYScale );

	//Width in camera space at the required depth
	*rXScale = 2 * tanf( pFov*DEGTORAD ) * pDepth;
	*rYScale = (*rXScale) / pAspect;
}

int md_Maths_RayIntersectTriangle(const Vector3 *pPos,const Vector3 *pDir,const Vector3 *pV0,const Vector3 *pV1,const Vector3 *pV2)
{
Vector3 edge1,edge2,tvec,pvec,qvec;
float det;
float u,  v;

	//find vectors for two edges sharing vert0
	edge1 = *pV1 - *pV0;
	edge2 = *pV2 - *pV0;

	//begin calculating determinant - also used to calculate U parameter
	pvec.Cross(*pDir,edge2);

	//if determinant is near zero, ray lies in plane of triangle
	det = edge1.Dot(pvec);

	//calculate distance from vert0 to ray origin
	tvec = *pPos - *pV0;

	//calculate U parameter and test bounds
	u = tvec.Dot(pvec);
	if( u < 0.0 || u > det )
		return 0;

	//prepare to test V parameter
	qvec.Cross(tvec,edge1);

	//calculate V parameter and test bounds
	v = pDir->Dot(qvec);
	if( v < 0.0 || u + v > det )
		return 0;

	return 1;
}

//Done like this so that I can test it in debug, release should compile away ok.
//Whilst doing SIMD in the past i've broken this sort of thing. Can cause bugs.
inline float RoundToNearest(float pValue)
{
	if( pValue > 0.0f )
	{
		if( fmodf(pValue,1.0f) < 0.5f )
			return floor(pValue);

		return ceilf(pValue);
	}
	else
	{
		if( fmodf(pValue,1.0f) < -0.5f )
			return floor(pValue);

		return ceilf(pValue);
	}
}

float md_Maths_RoundToNearest(float pValue)
{
//Just make sure its working.
	assert( 3.0f == RoundToNearest(3.2f) );
	assert( 4.0f == RoundToNearest(3.9f) );
	assert( -3.0f == RoundToNearest(-3.2f) );
	assert( -4.0f == RoundToNearest(-3.9f) );

	return RoundToNearest(pValue);
}


//Calculate the line segment PaPb that is the shortest route between
//two lines P1P2 and P3P4. Calculate also the values of mua and mub where
//    Pa = P1 + mua (P2 - P1)
//    Pb = P3 + mub (P4 - P3)
//Return FALSE if no solution exists.
int md_Maths_LineLineIntersect(Vector3 *p1,Vector3 *p2,Vector3 *p3,Vector3 *p4,Vector3 *pa,Vector3 *pb)
{
Vector3 p13,p43,p21;
float d1343,d4321,d1321,d4343,d2121;
float numer,denom;
float mua,mub;

	p13.x = p1->x - p3->x;
	p13.y = p1->y - p3->y;
	p13.z = p1->z - p3->z;
	p43.x = p4->x - p3->x;
	p43.y = p4->y - p3->y;
	p43.z = p4->z - p3->z;

	//return 0 on very short line.
	if( fabs(p43.x) < FLT_EPSILON &&
		fabs(p43.y) < FLT_EPSILON &&
		fabs(p43.z) < FLT_EPSILON)
	{
		return 0;
	}

	p21.x = p2->x - p1->x;
	p21.y = p2->y - p1->y;
	p21.z = p2->z - p1->z;
	//return 0 on very short line.
	if( fabs(p21.x) < FLT_EPSILON &&
		fabs(p21.y) < FLT_EPSILON &&
		fabs(p21.z) < FLT_EPSILON)
	{
		return 0;
	}

	d1343 = p13.x * p43.x + p13.y * p43.y + p13.z * p43.z;
	d4321 = p43.x * p21.x + p43.y * p21.y + p43.z * p21.z;
	d1321 = p13.x * p21.x + p13.y * p21.y + p13.z * p21.z;
	d4343 = p43.x * p43.x + p43.y * p43.y + p43.z * p43.z;
	d2121 = p21.x * p21.x + p21.y * p21.y + p21.z * p21.z;

	denom = d2121 * d4343 - d4321 * d4321;
	if( fabs(denom) < FLT_EPSILON )
		return 0;

	numer = d1343 * d4321 - d1321 * d4343;

	mua = numer / denom;
	mub = (d1343 + d4321 * (mua)) / d4343;

	if( mua < 0 || mua > 1 || mub < 0 || mub > 1 )
		return 0;

	pa->x = p1->x + mua * p21.x;
	pa->y = p1->y + mua * p21.y;
	pa->z = p1->z + mua * p21.z;
	pb->x = p3->x + mub * p43.x;
	pb->y = p3->y + mub * p43.y;
	pb->z = p3->z + mub * p43.z;

	return 1;
}

int md_Maths_LineLineIntersect2d(
					float p1_x,float p1_y,
					float p2_x,float p2_y,
					float p3_x,float p3_y,
					float p4_x,float p4_y,
					float *rA_x,float *rA_y,
					float *rB_x,float *rB_y)
{
float p13_x,p43_x,p21_x;
float p13_y,p43_y,p21_y;
float d1343,d4321,d1321,d4343,d2121;
float numer,denom;
float mua,mub;

	p21_x = p2_x - p1_x;
	p21_y = p2_y - p1_y;
	//return 0 on very short line.
	if( fabs(p21_x) < FLT_EPSILON &&
		fabs(p21_y) < FLT_EPSILON )
	{
		return 0;
	}

	p43_x = p4_x - p3_x;
	p43_y = p4_y - p3_y;
	//return 0 on very short line.
	if( fabs(p43_x) < FLT_EPSILON &&
		fabs(p43_y) < FLT_EPSILON )
	{
		return 0;
	}

	p13_x = p1_x - p3_x;
	p13_y = p1_y - p3_y;

	d4321 = p43_x * p21_x + p43_y * p21_y;
	d4343 = p43_x * p43_x + p43_y * p43_y;
	d2121 = p21_x * p21_x + p21_y * p21_y;

	denom = d2121 * d4343 - d4321 * d4321;
	if( fabs(denom) < FLT_EPSILON )
		return 0;

	d1343 = p13_x * p43_x + p13_y * p43_y;
	d1321 = p13_x * p21_x + p13_y * p21_y;

	numer = d1343 * d4321 - d1321 * d4343;

	mua = numer / denom;
	mub = (d1343 + d4321 * (mua)) / d4343;

	if( mua < 0 || mua > 1 || mub < 0 || mub > 1 )
		return 0;

	if( rB_y )
	{
		assert( rA_x && rA_y && rB_x && rB_y );
		*rA_x = p1_x + mua * p21_x;
		*rA_y = p1_y + mua * p21_y;

		*rB_x = p3_x + mub * p43_x;
		*rB_y = p3_y + mub * p43_y;
	}

	return 1;
}

int md_Maths_FacePointIntersect2d(float pFace[3][2],float pX,float pY)
{
float a,c,b;
float norm[3][2];
float s;
int n;

	for( n = 0 ; n < 3 ; n++ )
	{
		norm[n][0] = pX - pFace[n][0];
		norm[n][1] = pY - pFace[n][1];

		s = sqrtf( (norm[n][0]*norm[n][0])+(norm[n][1]*norm[n][1]) );
		assert( s > 0 );
		s = 1.0f / s;
		norm[n][0] *= s;
		norm[n][1] *= s;
	}

	a = (norm[0][0] * norm[1][0]) + (norm[0][1] * norm[1][1]);
	b = (norm[1][0] * norm[2][0]) + (norm[1][1] * norm[2][1]);
	c = (norm[2][0] * norm[0][0]) + (norm[2][1] * norm[0][1]);

	if( (a+b+c) < -1 )
		return 1;

	return 0;
}

int md_Maths_FaceBoxIntersect2d(//Tested.
					float pFace[3][2],
					float pLeft,
					float pTop,
					float pRight,
					float pBottom)
{
int i;

	//See if any verts in box.
	for( i = 0 ; i < 3 ; i++ )
	{
		if( pFace[i][0] > pLeft && pFace[i][0] < pRight &&
			pFace[i][1] > pTop && pFace[i][1] < pBottom )
		{
			return 1;
		}
	}

	//See if box points in tri.
	if( md_Maths_FacePointIntersect2d(pFace,pLeft,pTop) )
		return 1;

	if( md_Maths_FacePointIntersect2d(pFace,pRight,pTop) )
		return 1;

	if( md_Maths_FacePointIntersect2d(pFace,pLeft,pBottom) )
		return 1;

	if( md_Maths_FacePointIntersect2d(pFace,pRight,pBottom) )
		return 1;

	return 0;
}

int md_Maths_ColourLerp(int pColourA,int pColourB,float pFrac)
{
int res,t;
float a,b;

	if( pFrac > 1.0f )
		pFrac = 1.0f;
	else if( pFrac < 0.0f )
		pFrac = 0.0f;

	//Do alpha.
	a = (float)((pColourA&0xff000000)>>24);
	b = (float)((pColourB&0xff000000)>>24);

	a += (b-a)*pFrac;
	t = (int)(a);
	assert( t > -1 && t < 256 );//Make sure 've not fucked it up.
	res = t<<24;

	//Do red.
	a = (float)((pColourA&0x00ff0000)>>16);
	b = (float)((pColourB&0x00ff0000)>>16);

	a += (b-a)*pFrac;
	t = (int)(a);
	assert( t > -1 && t < 256 );//Make sure 've not fucked it up.
	res |= t<<16;

	//Do green.
	a = (float)((pColourA&0x0000ff00)>>8);
	b = (float)((pColourB&0x0000ff00)>>8);

	a += (b-a)*pFrac;
	t = (int)(a);
	assert( t > -1 && t < 256 );//Make sure 've not fucked it up.
	res |= t<<8;

	//Do blue.
	a = (float)(pColourA&0x000000ff);
	b = (float)(pColourB&0x000000ff);

	a += (b-a)*pFrac;
	t = (int)(a);
	assert( t > -1 && t < 256 );//Make sure 've not fucked it up.
	res |= t;

	return res;
}

int md_Maths_RotateViaShortestDistance(const float pTarget,const float pStep,float &rAngle,const float pMax/*= 360.0f*/)
{
float cw_diff,ccw_diff;
int target_less_angle;

	if( pTarget > rAngle )
	{
		cw_diff = pTarget - rAngle;
		ccw_diff = rAngle + (pMax - pTarget);
		target_less_angle = 0;
	}
	else
	{
		cw_diff = pTarget + (pMax - rAngle);
		ccw_diff = rAngle - pTarget;
		target_less_angle = 1;
	}

	if( cw_diff < ccw_diff )
	{
		rAngle += pStep;
	}
	else
	{
		rAngle -= pStep;
	}

	if( target_less_angle )
	{
		if( pTarget > rAngle )
			rAngle = pTarget;
	}
	else
	{
		if( pTarget < rAngle )
			rAngle = pTarget;
	}

	if( rAngle > pMax )
		rAngle -= pMax;
	else if( rAngle < 0.0f )
		rAngle += pMax;

	return ( pTarget != rAngle );
}
// ---------------------------------------------------------------------------
};//namespace BogDog{
