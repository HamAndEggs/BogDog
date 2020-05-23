/*
 * main.cpp
 *
 *  Created on: 8 Jun 2012
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


#include <stdio.h>
#include "BogDog.h"

int main(void)
{
  printf("\n**************** Starting app ****************\n");
  BogDog::OpenGLES_2_0 gl;
  if(gl.Create(false) == false )
  {
	  return 1;
  }
  CHECK_OGL_ERRORS();

  BogDog::View theView;

  theView.SetFrustum(40.0f,gl.GetAspectRatio(),0.1f,100.0f);
  theView.SetCamera(0,0,-40,0,0,0);

  BogDog::GLShaderColour* colourShader = BogDog::GLShaderColour::Allocate();
  BogDog::Mesh* box = BogDog::ShapeBuilder::MakeBox(0.7f,0.7f,0.7f)->BuildMesh(true,false);

  BogDog::Matrix boxRot;

  CHECK_OGL_ERRORS();

  BogDog::Timer timer;
  timer.Start();

  BogDog::Matrix ma,mb;

  int numDrawn = 0;
  int n = 0;
  Angle a = 0;
  float time = 0;
  while(BogDog::OpenGLES_2_0::ApplicationRunning())
  {
	  usleep(10000);
	  a += 200;
	  theView.SetCamera(BogDog::Sin(a) * -40,0,BogDog::Cos(a) * -40,0,a,0);

	  const BogDog::Matrix& projectionInvCamera = theView.GetProjectionCameraMatrix();

	  gl.Clear(0);

	  n++;
	  if( n == 60 )
	  {
		  timer.Stop();
		  timer.Start();
		  n = 0;
		  printf("FPS = %f numDrawn(%d)\n",60.0f / timer.GetSeconds(),numDrawn);
	  }

	  colourShader->Enable(projectionInvCamera);

	  box->Enable();

	  boxRot.SetRotation(a,a*3,a*2);
	  colourShader->setTransform(boxRot);
	  colourShader->setGlobalColour(1,1,1,1);

	  numDrawn = 0;
	  for( float x = -15 ; x <= 15 ; x += 1.0f )
	  {
		  for( float y = -15 ; y <= 15 ; y += 1.0f )
		  {
			  float cs = (BogDog::Sin((Angle)(int)((x + y + time)*2000.0f)) + 1.0f) * 0.5f;

			  colourShader->setGlobalColour(cs,cs,cs,0.2f);

			  boxRot[3].x = x;
			  boxRot[3].y = y;
			  boxRot[3].z = cs * 10.0f;
			  colourShader->setTransform(boxRot);
			  box->Draw();
			  numDrawn++;
		  }
	  }
	  time += 1.0f;

	  gl.Update();
  };

  delete colourShader;

  return 0;
}
