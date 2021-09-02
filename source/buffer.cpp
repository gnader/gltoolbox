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
    : mId(0), mOwned(false), mTarget(target),
      mIsAttached(false), mPtr(nullptr), mElementNum(-1), mElementSize(-1)
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
  mTarget = temp.mTarget;

  mIsAttached = temp.mIsAttached;
  mPtr = temp.mPtr;
  mElementNum = temp.mElementNum;
  mElementSize = temp.mElementSize;

  temp.mId = 0;
  temp.mOwned = false;
  temp.detach();
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
  mTarget = other.mTarget;

  mIsAttached = other.mIsAttached;
  mPtr = other.mPtr;
  mElementNum = other.mElementNum;
  mElementSize = other.mElementSize;

  other.mId = 0;
  other.mOwned = false;
  other.detach();

  return *this;
}

void Buffer::upload(void *ptr, GLsizei size, GLenum usage) const
{
  bind();
  glBufferData(target(), size, ptr, usage);
}

void Buffer::upload(void *ptr, GLsizei offset, GLsizei size) const
{
  bind();
  glBufferSubData(target(), offset, size, ptr);
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

void Buffer::detach()
{
  mPtr = nullptr;
  mElementNum = 0;
  mElementSize = 0;
  mIsAttached = false;
}

void Buffer::update(GLenum usage) const
{
  upload(mPtr, num_elements() * element_size(), usage);
}

void Buffer::update(GLsizei offset, GLsizei num) const
{
  upload(mPtr, element_size() * offset, element_size() * num);
}

void Buffer::download()
{
  download(mPtr, num_elements() * element_size());
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

  if (is_attached())
    detach();
}

GLint Buffer::get_parameter(const GLenum param) const
{
  GLint result;
  bind();
  glGetBufferParameteriv(target(), param, &result);
  return result;
}