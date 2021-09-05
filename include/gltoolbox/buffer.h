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
#include <vector>

namespace gltoolbox
{
  class Buffer
  {
  public:
    Buffer(GLenum target, GLsizei elementsize, GLenum usage = GL_STATIC_DRAW);

    Buffer(const Buffer &other) = delete;
    Buffer(Buffer &&temp);

    virtual ~Buffer();

    Buffer &operator=(const Buffer &other) = delete;
    Buffer &operator=(Buffer &other);

    //=====================================================
    // Buffer information
    //=====================================================

    inline GLuint id() const { return mId; }
    inline bool is_valid() const { return (glIsBuffer(mId) == GL_TRUE); }

    inline GLenum target() const { return mTarget; }

    inline GLenum usage() const { return mUsage; }
    inline void set_usage(GLenum usage) { mUsage = usage; }

    inline GLsizei element_size() const { return mElementSize; }
    inline GLsizei buffer_size() const { return get_parameter(GL_BUFFER_SIZE); }

    //=====================================================
    // Buffer operations
    //=====================================================

    inline void bind() const { glBindBuffer(mTarget, mId); }
    inline void unbind() const { glBindBuffer(mTarget, 0); }

    //======================================================
    // Buffer content
    //======================================================
    //send data from CPU memory to GPU memory
    void upload(void *ptr, GLsizei count) const;
    void upload(void *ptr, GLsizei offset, GLsizei count) const;

    //send data from GPU memory to CPU memory
    void download(void *ptr, GLsizei size) const;
    void download(void *ptr, GLsizei offset, GLsizei size) const;

  protected:
    void create();
    void destroy();

    GLint get_parameter(const GLenum param) const;

  protected:
    GLuint mId;
    bool mOwned;

    GLenum mUsage;
    GLenum mTarget;
    GLsizei mElementSize;
  };
}

#endif