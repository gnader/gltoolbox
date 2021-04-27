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
    : mId(0), mOwned(true), mTarget(target), mData(nullptr), mSize(-1)
{
  glCreateBuffers(1, &mId);
  bind(); // this will actually trigger the buffer creation
}

Buffer::Buffer(GLenum target, GLvoid *data, GLsizei size, GLenum usage)
    : mId(0), mOwned(true), mTarget(target), mData(data), mSize(size)
{
  glCreateBuffers(1, &mId);
  update(usage); // update will call bind() and buffer will be created
}

Buffer::Buffer(Buffer &&temp)
{
  //delete what ever was there
  delete_buffer();

  mId = temp.mId;
  mOwned = temp.mOwned;
  mTarget = temp.mTarget;
  mData = temp.mData;
  mSize = temp.mSize;

  temp.mId = 0;
  temp.mOwned = false;
  temp.mData = nullptr;
  temp.mSize = -1;
}

Buffer::~Buffer()
{
  delete_buffer();
}

Buffer &Buffer::operator=(Buffer &other)
{
  //delete whatever was there
  delete_buffer();

  mId = other.mId;
  mOwned = other.mOwned;
  mTarget = other.mTarget;
  mData = other.mData;
  mSize = other.mSize;

  other.mId = 0;
  other.mOwned = false;
  other.mData = nullptr;
  other.mSize = -1;

  return *this;
}

void Buffer::set_data(GLvoid *data, GLsizei size, GLenum usage)
{
  mData = data;
  mSize = size;
  update(usage);
}

void Buffer::update(GLenum usage) const
{
  bind();
  glBufferData(target(), mSize, mData, usage);
}

void Buffer::update_subdata(GLintptr offset, GLsizei size) const
{
  bind();
  glBufferSubData(target(), offset, size, mData);
}

void Buffer::delete_buffer()
{
  if (mOwned && is_valid())
  {
    glDeleteBuffers(1, &mId);
    mId = 0;
  }

  mData = nullptr;
  mSize = -1;
}

GLint Buffer::get_parameter(const GLenum param) const
{
  GLint result;
  bind();
  glGetBufferParameteriv(target(), param, &result);
  return result;
}