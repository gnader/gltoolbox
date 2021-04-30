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

BaseBuffer::BaseBuffer(GLenum target)
    : mId(0), mOwned(true), mTarget(target)
{
  glCreateBuffers(1, &mId);
  bind(); // this will actually trigger the buffer creation
}

BaseBuffer::BaseBuffer(BaseBuffer &&temp)
{
  //delete whatever was there
  delete_buffer();

  //move buffer ownership
  mId = temp.mId;
  mOwned = temp.mOwned;
  mTarget = temp.mTarget;

  temp.mId = 0;
  temp.mOwned = false;
}

BaseBuffer::~BaseBuffer()
{
  delete_buffer();
}

BaseBuffer &BaseBuffer::operator=(BaseBuffer &other)
{
  //delete whatever was there
  delete_buffer();

  //move buffer ownership
  mId = other.mId;
  mOwned = other.mOwned;
  mTarget = other.mTarget;

  other.mId = 0;
  other.mOwned = false;

  return *this;
}

void BaseBuffer::set_data(GLvoid *data, GLsizei size, GLenum usage) const
{
  bind();
  glBufferData(target(), size, data, usage);
}

void BaseBuffer::set_subdata(GLvoid *data, GLintptr offset, GLsizei size) const
{
  bind();
  glBufferSubData(target(), offset, size, data);
}

void BaseBuffer::get_data(GLvoid *data, GLsizei size) const
{
  get_subdata(data, 0, size);
}

void BaseBuffer::get_subdata(GLvoid *data, GLintptr offset, GLsizei size) const
{
  bind();
  glGetBufferSubData(target(), offset, size, data);
}

void BaseBuffer::delete_buffer()
{
  if (mOwned && is_valid())
  {
    glDeleteBuffers(1, &mId);
    mId = 0;
  }
}

GLint BaseBuffer::get_parameter(const GLenum param) const
{
  GLint result;
  bind();
  glGetBufferParameteriv(target(), param, &result);
  return result;
}