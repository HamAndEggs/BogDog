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


#include <iostream>
#include "BogDog.h"

/*
 * Make a texture texture to check my pixel shader.
 */
const GLint MakeTexture(BogDog::OpenGLES_2_0 &gl,int width,int height)
{
	uint32_t* pixels = new uint32_t[width * height];

	//Fill with white for a start.
	memset(pixels,0,width * height * 4);

	//Now make some one pixel borders. Good test for UV's.
	for(int x = 0 ; x < width ; x++ )
	{
		pixels[x] = 0x0000ff;
		pixels[(width*height) - x - 1] = 0xff0000;
	}

	for(int y = 0 ; y < height ; y++ )
	{
		pixels[y * width] = 0x00ff00;
		pixels[(y * width) + (width-1)] = 0xffffff;
	}

	GLint tex = gl.CreateTexture(BogDog::TEX_R8G8B8A8,width,height,pixels,false,true,true);

	delete[] pixels;

	return tex;
}

const GLint LoadTexture(BogDog::OpenGLES_2_0 &gl)
{
	GLint tex = 0;
	BogDog::ImageLoader loader;
	const BogDog::LoadedImage& image = loader.Load("./Examples/data/textures/test256.tga");
	if( image.image != NULL )
	{
	  std::cout << "Texture loaded " << tex << std::endl;
	  tex = gl.CreateTexture(image,false,true,true);
	  std::cout << "Texture created " << tex << std::endl;
	}
	else
	{
	  std::cout << "Texture load failed!" << std::endl;
	}
	loader.FreeupMemory();
	return tex;
}

/*
 * Main function of the application, return from this and the application will exit.
 */
int main(void)
{
  std::cout << "**************** Starting app ****************" << std::endl;
  BogDog::OpenGLES_2_0 gl;
  if(gl.Create(false) == false )
  {
	  return 1;
  }
  CHECK_OGL_ERRORS();

  BogDog::View theView;

  theView.SetFrustum(40.0f,gl.GetAspectRatio(),0.1f,100.0f);
  theView.SetCamera(0,0,-40,0,0,0);

  BogDog::GLShaderColourTex* colourShader = BogDog::GLShaderColourTex::Allocate();
  BogDog::ShapeBuilder* boxBuilder = BogDog::ShapeBuilder::MakeBox(1.0f,1.0f,1.0f);
  for(size_t n = 0 ; n < boxBuilder->faces.GetSize() ; n++ )
  {
	  boxBuilder->faces[n].colour = -1;
  }
  BogDog::Mesh* box = boxBuilder->BuildMesh(true,true);

  BogDog::Matrix boxRot;

  CHECK_OGL_ERRORS();

  BogDog::Timer timer;
  timer.Start();

//  GLint tex = MakeTexture(gl,256,256);
  GLint tex = LoadTexture(gl);

  Angle a = 0;
  float time = 0;
  while(BogDog::OpenGLES_2_0::ApplicationRunning())
  {
	  a += 7;
	  theView.SetCamera(0,0,-2.0f,0,0,0);

	  const BogDog::Matrix& projectionInvCamera = theView.GetProjectionCameraMatrix();

	  gl.Clear(0x404040);

	  colourShader->Enable(projectionInvCamera);
	  colourShader->setTexture(0,tex);
	  box->Enable();

	  boxRot.SetRotation(a,a*3,a*2);
	  colourShader->setTransform(boxRot);
	  colourShader->setGlobalColour(1,1,1,1);
	  box->Draw();
	  time += 1.0f;

	  gl.Update();
  };

  std::cout << "Application exiting" << std::endl;

  delete colourShader;

  return 0;
}


