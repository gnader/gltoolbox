/**
  * This file is part of gltoolbox
  *
  * MIT License
  *
  * Copyright (c) 2021 Georges Nader
  *
  * Permission is hereby granted, free of charge, to any person obtaining a copy
  * of this software and associated documentation files (the "Software"), to deal
  * in the Software without restriction, including without limitation the rights
  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  * copies of the Software, and to permit persons to whom the Software is
  * furnished to do so, subject to the following conditions:
  * 
  * The above copyright notice and this permission notice shall be included in all
  * copies or substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  * SOFTWARE.
  */

#ifndef __GL_H__
#define __GL_H__

#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>
using namespace gl;

namespace gltoolbox
{
  class GL
  {
  public:
    // all member functions are static, constructor is not needed
    GL() = delete;

  public:
    static void initilize(glbinding::GetProcAddress functionPointer, bool resolve = true)
    {
      glbinding::Binding::initialize(functionPointer, resolve);
    }

    static const GLubyte *gl_version()
    {
      return glGetString(GL_VERSION);
    }

    static const GLubyte *glsl_version()
    {
      return glGetString(GL_SHADING_LANGUAGE_VERSION);
    }
  };
}

#endif // __GL_H__