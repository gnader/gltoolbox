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

Buffer::Buffer(GLenum target, GLsizei elementsize, GLenum usage)
    : mId(0), mOwned(false), mUsage(usage), mTarget(target), mElementSize(elementsize)
{
  create();
}

Buffer::Buffer(Buffer &&temp)
{
  //delete whatever was there
  destroy();

  //move buffer ownership
  mId = temp.mId;
  mOwned = temp.mOwned;

  mUsage = temp.mUsage;
  mTarget = temp.mTarget;
  mElementSize = temp.mElementSize;

  temp.mId = 0;
  temp.mOwned = false;
}

Buffer::~Buffer()
{
  destroy();
}

Buffer &Buffer::operator=(Buffer &other)
{
  //delete whatever was there
  destroy();

  //move buffer ownership
  mId = other.mId;
  mOwned = other.mOwned;

  mUsage = other.mUsage;
  mTarget = other.mTarget;
  mElementSize = other.mElementSize;

  other.mId = 0;
  other.mOwned = false;

  return *this;
}

void Buffer::upload(void *ptr, GLsizei count) const
{
  bind();
  glBufferData(target(), count * element_size(), ptr, usage());
}

void Buffer::upload(void *ptr, GLsizei offset, GLsizei count) const
{
  bind();
  glBufferSubData(target(), offset * element_size(), count * element_size(), ptr);
}

void Buffer::download(void *ptr, GLsizei size) const
{
  bind();
  glGetBufferSubData(target(), 0, size, ptr);
}

void Buffer::download(void *ptr, GLsizei offset, GLsizei size) const
{
  bind();
  glGetBufferSubData(target(), offset, size, ptr);
}

void Buffer::create()
{
  if (!mOwned || !is_valid())
  {
    glCreateBuffers(1, &mId);
    bind(); // this will actually trigger the buffer creation
    mOwned = true;
  }
}

void Buffer::destroy()
{
  if (mOwned && is_valid())
  {
    glDeleteBuffers(1, &mId);
    mId = 0;
    mOwned = false;
  }
}

GLint Buffer::get_parameter(const GLenum param) const
{
  GLint result;
  bind();
  glGetBufferParameteriv(target(), param, &result);
  return result;
}