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

#include <gltoolbox/buffer.h>

using namespace gltoolbox;

Buffer::Buffer(GLenum target)
    : mId(0), mTarget(target)
{
  glCreateBuffers(1, &mId);
}

Buffer::~Buffer()
{
  glDeleteBuffers(1, &mId);
}

GLint Buffer::size() const
{
  GLint sz = 0;
  bind();
  glGetBufferParameteriv(mTarget, GL_BUFFER_SIZE, &sz);
  return sz;
}

// Binds the buffer to target.
void Buffer::bind() const
{
  glBindBuffer(mTarget, mId);
}

// Unbinds a specific target, i.e. binds a 0 id to the target.
void Buffer::unbind() const
{
  glBindBuffer(mTarget, 0);
}

// Creates video memory for the buffer.
void Buffer::set_data(GLsizeiptr size, const GLvoid *data, GLenum usage)
{
  bind();
  glBufferData(mTarget, size, data, usage);
}

// Writes data only to a defined area of the memory.
void Buffer::set_subdata(GLintptr offset, GLsizeiptr size, const GLvoid *data)
{
  bind();
  glBufferSubData(mTarget, offset, size, data);
}