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

#include <gltoolbox/vertexarray.h>
using namespace gltoolbox;

VertexArray::VertexArray()
    : mId(0), mOwned(false)
{
  create();
}

VertexArray::VertexArray(VertexArray &&temp)
{
  // delete whatever there is here
  destroy();

  mId = temp.mId;
  mOwned = temp.mOwned;

  // clear the information from temp
  temp.mId = 0;
  temp.mOwned = false;
}

VertexArray::~VertexArray()
{
  destroy();
}

void VertexArray::draw_arrays(GLenum mode, GLint first, GLsizei count) const
{
  glDrawArrays(mode, first, count);
}

void VertexArray::draw_arrays(GLenum mode, GLint first, GLsizei count, GLsizei inum) const
{
  glDrawArraysInstanced(mode, first, count, inum);
}

void VertexArray::draw_elements() const
{
  mIndices.buffer->bind();
  glDrawElements(mIndices.mode, mIndices.count, mIndices.type, (GLvoid *)0);
}

void VertexArray::draw_elements(GLsizei inum) const
{
  mIndices.buffer->bind();
  glDrawElementsInstanced(mIndices.mode, mIndices.count, mIndices.type, (GLvoid *)0, inum);
}

void VertexArray::draw_elements(GLuint start, GLuint end) const
{
  mIndices.buffer.get()->bind();
  glDrawRangeElements(mIndices.mode, start, end, end - start, mIndices.type, (GLvoid *)0);
}

bool VertexArray::has_index_buffer() const
{
  if (mIndices.buffer)
    return mIndices.buffer->is_valid();

  return false;
}

bool VertexArray::has_attribute(const std::string &name) const
{
  auto search = mAttributes.find(name);
  if (search != mAttributes.end())
    if (mAttributes.at(name).buffer)
      return mAttributes.at(name).buffer.get()->is_valid();

  return false;
}

void VertexArray::enable_attribute(const std::string &name, GLint loc) const
{
  if (has_attribute(name))
  {
    const auto &format = attribute_format(name);
    const auto &divisor = attribute_divisor(name);

    mAttributes.at(name).buffer->bind();
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, format.size, format.type, format.normalized, format.stride, 0);
    if (divisor > 0)
      glVertexAttribDivisor(loc, divisor);
  }
}

void VertexArray::enable_attributes(const std::unordered_map<std::string, GLint> &attributes) const
{
  for (const auto &[name, loc] : attributes)
    enable_attribute(name, loc);
}

void VertexArray::disable_attribute(const std::string &name, GLint loc) const
{
  if (has_attribute(name))
  {
    glDisableVertexAttribArray(loc);
    mAttributes.at(name).buffer->unbind();
  }
}

void VertexArray::disable_attributes(const std::unordered_map<std::string, GLint> &attributes) const
{
  for (const auto &[name, loc] : attributes)
    disable_attribute(name, loc);
}

void VertexArray::create()
{
  if (!mOwned || !is_valid())
  {
    glCreateVertexArrays(1, &mId);
    bind(); // this will actually trigger the buffer creation
    mOwned = true;
  }
}

void VertexArray::destroy()
{
  if (mOwned && is_valid())
  {
    glDeleteVertexArrays(1, &mId);
    mId = 0;
    mOwned = false;
  }
}