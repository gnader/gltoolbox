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

#ifndef __GLTOOLBOX_VERTEXATTRIB_H__
#define __GLTOOLBOX_VERTEXATTRIB_H__

#include "gl.h"
#include "buffer.h"

namespace gltoolbox
{
  class Program; //forward declaration of program class

  class VertexAttribute
  {
  public:
    VertexAttribute(Program *prog, GLint location);
    VertexAttribute(Program *prog, const std::string &name);
    VertexAttribute(Program *prog, GLint location, const std::string &name);

    virtual ~VertexAttribute();

    inline GLint location() const;
    inline const std::string &name() const;

    void enable() const;
    void disable() const;

  protected:
    Program *mProg;

    GLint mLocation;
    std::string mName;
  };
}

#endif