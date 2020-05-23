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

#ifndef __MATHS_H__
#define __MATHS_H__

#include <stddef.h>
#include <math.h>

namespace BogDog{
struct Vector3;
// ---------------------------------------------------------------------------

static const float PI = 3.141592654f;
static const float DEGTORAD = 0.017453292f;//(PI / 180.0f);
static const float RADTODEG = 57.29577951f;//(180.0f / PI);


// MACROS --------------------------------------------------------------------
#define SINF(pAngle)		sinf( (pAngle)*DEGTORAD )
#define COSF(pAngle)		cosf( (pAngle)*DEGTORAD )
#define SINF_POS(pAngle)	((SINF(pAngle)*0.5f)+0.5f)	//Ranges from 0 -> 1
#define COSF_POS(pAngle)	((COSF(pAngle)*0.5f)+0.5f)

#define MD_SIGN(__VALUE)	((__VALUE)<0?-1:1)
#define MD_SWAP(__a,__b)	{(__a) ^= (__b);(__b) ^= (__a);(__a) ^= (__b);}
#define MD_SWAPF(__a,__b)	{float __t = (__a);(__a) = (__b);(__b) = __t;}

inline float md_clamp(const float pVal,const float pMin,const float pMax)
{
	if( pVal < pMin )
		return pMin;

	if( pVal > pMax )
		return pMax;

	return pVal;
}


// EXTERNAL FUNCTION PROTOTYPES ----------------------------------------------
//Takes the fov, aspect (w/h), cam depth and returns in rXScale and rYScale
extern void md_Maths_GetDimensionsInCameraSpace(float pFov,float pAspect,float pDepth,float *rXScale,float *rYScale);
extern int md_Maths_RayIntersectTriangle(const Vector3 *pPos,const Vector3 *pDir,const Vector3 *pV0,const Vector3 *pV1,const Vector3 *pV2);
extern float md_Maths_RoundToNearest(float pValue);
inline void md_Maths_RoundToNearest(float *pValue){*pValue = md_Maths_RoundToNearest(*pValue);}
inline float md_Maths_ClampValue(float pValue,float pMin,float pMax){return ( (pValue < pMin) ? pMin : (pValue > pMax) ? pMax : pValue ); }
inline int md_Maths_Compare(float pA,float pB,float pFactor){return ((pA-pB) > pFactor) ? -1 : ((pB-pA) > pFactor) ? 1 : 0;}

extern int md_Maths_LineLineIntersect(Vector3 *p1,Vector3 *p2,Vector3 *p3,Vector3 *p4,Vector3 *pa,Vector3 *pb);//Untested.
extern int md_Maths_LineLineIntersect2d(//Tested.
					float p1_x,float p1_y,
					float p2_x,float p2_y,
					float p3_x,float p3_y,
					float p4_x,float p4_y,
					float *rA_x = 0,float *rA_y = 0,
					float *rB_x = 0,float *rB_y = 0);

extern int md_Maths_FaceBoxIntersect2d(
					float pFace[3][2],
					float pLeft,
					float pTop,
					float pRight,
					float pBottom);

extern int md_Maths_FacePointIntersect2d(float pFace[3][2],float pX,float pY);//Tested.

extern int md_Maths_ColourLerp(int pColourA,int pColourB,float pFrac);//Could'nt think of anywhere else to put this.

//increments or decrements rAngle by pStep depending which is the shortest distance, rAngle 0 -> pMax
//Returns TRUE is rAngle changed, else FALSE.
extern int md_Maths_RotateViaShortestDistance(const float pTarget,const float pStep,float &rAngle,const float pMax = 360.0f);

// ---------------------------------------------------------------------------
};//namespace BogDog{
#endif//#ifndef __MATHS_H__
