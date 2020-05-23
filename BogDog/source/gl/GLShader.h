/*
 * GLShader.h
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

#ifndef GLSHADER_H_
#define GLSHADER_H_

#include <stdio.h>
#include "OpenGLES20.h"
#include "maths/Matrix.h"

namespace BogDog
{

#define ATTRIB_POS 0
#define ATTRIB_COLOUR 1
#define ATTRIB_UV0 2

struct GLShader
{
	~GLShader();

	int getUniformLocation(const char* name);

	void BindAttribLocation(int location,const char* name);

	void Enable(const Matrix& projInvcam);

	void setTransform(Matrix& transform)
	{
        glUniformMatrix4fv(u_trans, 1, false,(const GLfloat*)transform.m);
        CHECK_OGL_ERRORS();
	}

	void setTransform(float x,float y,float z)
	{
		transOnly[12] = x;
		transOnly[13] = y;
		transOnly[14] = z;
        glUniformMatrix4fv(u_trans, 1, false, transOnly);
        CHECK_OGL_ERRORS();
	}

	void setTransformIdentity()
	{
		glUniformMatrix4fv(u_trans, 1, false, identity);
		CHECK_OGL_ERRORS();
	}

	void setGlobalColour(float red,float green,float blue,float alpha)
	{
		glUniform4f(u_global_colour,red,green,blue,alpha);
	}

	void setTexture(int index,GLint texture)
	{
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D,texture);
		glUniform1i(u_tex0,0);
	}

protected:
	GLShader()
	{
	}

	void Create(const char* vertex, const char* fragment);

	virtual void onGetUniformLocation();

	virtual void onBindAttribs();

	int LoadShader(int type, const char* shaderCode);



private:

	/**
	 * Used for when we want to set the transform to an identity matrix.
	 */
	static float identity[16];
	static float transOnly[16];

	GLint u_trans;
	GLint u_proj_cam;
	GLint u_global_colour;
	GLint u_tex0;
	GLint shader;


};

} /* namespace BogDog */
#endif /* GLSHADER_H_ */
