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

#ifndef __GLTOOLBOX_BUFFER_H__
#define __GLTOOLBOX_BUFFER_H__

#include "gl.h"

namespace gltoolbox
{
  class BaseBuffer
  {
  public:
    BaseBuffer(GLenum target);

    //? copy constructor can just take control of the buffer in other without taking ownership
    BaseBuffer(const BaseBuffer &other) = delete;
    BaseBuffer(BaseBuffer &&temp);

    virtual ~BaseBuffer();

    BaseBuffer &operator=(const BaseBuffer &other) = delete;
    BaseBuffer &operator=(BaseBuffer &other);

    inline GLuint id() const { return mId; }
    inline bool is_valid() const { return glIsBuffer(mId) != 0; }

    inline GLenum target() const { return mTarget; }
    inline GLenum usage() const { return GLenum(get_parameter(GL_BUFFER_USAGE)); }

    inline GLsizei memory_size() const { return get_parameter(GL_BUFFER_SIZE); }

    inline void bind() const { glBindBuffer(mTarget, mId); }
    inline void unbind() const { glBindBuffer(mTarget, 0); }

    void set_data(GLvoid *data, GLsizei size, GLenum usage = GL_STATIC_DRAW) const;
    void set_subdata(GLvoid *data, GLintptr offset, GLsizei size) const;

    void get_data(GLvoid *data, GLsizei size) const;
    void get_subdata(GLvoid *data, GLintptr offset, GLsizei size) const;

  protected:
    void delete_buffer();

    GLint get_parameter(const GLenum param) const;

  protected:
    GLuint mId;
    bool mOwned;

    GLenum mTarget;
  };

  template <typename T>
  class Buffer : public BaseBuffer
  {
  };
}

#endif