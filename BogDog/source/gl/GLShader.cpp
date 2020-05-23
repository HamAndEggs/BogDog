/*
 * GLShader.cpp
 *
 *  Created on: 9 Jun 2012
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

#include "gl/OpenGLES20.h"
#include "gl/GLShader.h"
#include "maths/Matrix.h"

namespace BogDog
{

/**
 * Used for when we want to set the transform to an identity matrix.
 */
float GLShader::identity[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
float GLShader::transOnly[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};

GLShader::~GLShader()
{
}

int GLShader::getUniformLocation(const char* name)
{
	int location = glGetUniformLocation(shader,name);
	CHECK_OGL_ERRORS();

	if( location < 0 )
	{
		printf("Failed to find UniformLocation %s\n",name);
	}

	return location;
}

void GLShader::BindAttribLocation(int location,const char* name)
{
	glBindAttribLocation(shader, location,name);
	CHECK_OGL_ERRORS();
	printf("AttribLocation(%s = %d)\n",name,location);
}

void GLShader::Enable(const Matrix& projInvcam)
{
    glUseProgram(shader);
    CHECK_OGL_ERRORS();

    glUniformMatrix4fv(u_proj_cam, 1, false,(const float*)projInvcam.m);
    CHECK_OGL_ERRORS();

    if( u_tex0 > 0 )
    {
    	glActiveTexture(GL_TEXTURE0);
    	glUniform1i(u_tex0,0);
    }
}

void GLShader::Create(const char* vertex, const char* fragment)
{
	//"precision highp float;\n"
	//"precision mediump float;\n"

	int vertexShader = LoadShader(GL_VERTEX_SHADER,vertex);
	int fragmentShader = LoadShader(GL_FRAGMENT_SHADER,fragment);

	printf("GLShader::Create: :vertexShader(%d) fragmentShader(%d)\n",vertexShader,fragmentShader);

	shader = glCreateProgram(); // create empty OpenGL Program
	CHECK_OGL_ERRORS();

	glAttachShader(shader, vertexShader); // add the vertex shader to program
	CHECK_OGL_ERRORS();

	glAttachShader(shader, fragmentShader); // add the fragment shader to program
	CHECK_OGL_ERRORS();
	//Set the input stream numbers.
	//Has to be done before linking.
	onBindAttribs();

	glLinkProgram(shader); // creates OpenGL program executables
	CHECK_OGL_ERRORS();

	GLint compiled;
	glGetProgramiv(shader,GL_LINK_STATUS,&compiled);
	CHECK_OGL_ERRORS();
	if ( compiled == GL_FALSE )
	{
		printf("Failed to compile shader\n");
		GLint infoLen = 0;
		glGetProgramiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );
		printf("infoLen(%d)\n",infoLen);
		if ( infoLen > 1 )
		{
			char* error_message = new char[infoLen];

			glGetProgramInfoLog(shader,infoLen,&infoLen,error_message);
			printf("Error compiling shader:\n%s\n",error_message);

			delete []error_message;
		}
		glDeleteShader ( shader );
		shader = 0;
	}

	//Get the bits for the variables in the shader.
	onGetUniformLocation();
}

void GLShader::onGetUniformLocation()
{
	u_proj_cam = getUniformLocation("u_proj_cam");
	u_trans = getUniformLocation("u_trans");
	u_global_colour = getUniformLocation("u_global_colour");
	u_tex0 = getUniformLocation("u_tex0");
}

void GLShader::onBindAttribs()
{
	//Has to be done before linking.
	BindAttribLocation(ATTRIB_POS, "a_xyz");
	BindAttribLocation(ATTRIB_COLOUR, "a_col");
	BindAttribLocation(ATTRIB_UV0, "a_uv0");
}

int GLShader::LoadShader(int type, const char* shaderCode)
{
	// create a vertex shader type (GLES20.GL_VERTEX_SHADER)
	// or a fragment shader type (GLES20.GL_FRAGMENT_SHADER)
	int shaderFrag = glCreateShader(type);

	// add the source code to the shader and compile it
	glShaderSource(shaderFrag,1,&shaderCode,NULL);
	glCompileShader(shaderFrag);
	CHECK_OGL_ERRORS();
	// Check the compile status
	GLint compiled;
	glGetShaderiv(shaderFrag,GL_COMPILE_STATUS,&compiled);
	if ( compiled == GL_FALSE )
	{
		printf("Failed to compile shader\n");
		GLint infoLen = 0;
		glGetShaderiv ( shaderFrag, GL_INFO_LOG_LENGTH, &infoLen );
		printf("infoLen(%d)\n",infoLen);
		if ( infoLen > 1 )
		{
			char* error_message = new char[infoLen];

			glGetShaderInfoLog(shaderFrag,infoLen,&infoLen,error_message);
			printf("Error compiling shader:\n%s\n",error_message);

			delete []error_message;
		}
		glDeleteShader ( shaderFrag );
		printf("\n\n%s\n\n",shaderCode);
	}
	CHECK_OGL_ERRORS();

	return shaderFrag;
}


} /* namespace BogDog */
