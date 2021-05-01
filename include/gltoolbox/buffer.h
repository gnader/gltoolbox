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

    //=====================================================
    // Buffer information
    //=====================================================

    inline GLuint id() const { return mId; }
    inline bool is_valid() const { return glIsBuffer(mId) != 0; }

    inline GLenum target() const { return mTarget; }
    inline GLenum usage() const { return GLenum(get_parameter(GL_BUFFER_USAGE)); }

    //=====================================================
    // Buffer operations
    //=====================================================

    inline void bind() const { glBindBuffer(mTarget, mId); }
    inline void unbind() const { glBindBuffer(mTarget, 0); }

    //======================================================
    // Buffer content
    //======================================================

    inline GLsizei memory_size() const { return get_parameter(GL_BUFFER_SIZE); }

    virtual GLsizei element_size() const = 0;
    virtual GLsizei num_elements() const = 0;

    virtual void update(GLenum usage = GL_STATIC_DRAW) const = 0;
    virtual void update(GLsizei offset, GLsizei num) const = 0;

    virtual void get_data() const = 0;
    virtual void get_subdata(GLsizei offset, GLsizei num) const = 0;

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
  public:
    Buffer(GLenum target)
        : BaseBuffer(target), mPtr(nullptr), mNum(-1)
    {
    }

    Buffer(GLenum target, T *data, GLsizei num)
        : BaseBuffer(target), mPtr(data), mNum(num)
    {
    }

    Buffer(const Buffer &other) = delete;

    Buffer(Buffer &&temp)
        : BaseBuffer(temp)
    {
      mPtr = temp.mPtr;
      mNum = temp.mNum;

      temp.mPtr = nullptr;
      temp.mNum = -1;
    }

    virtual ~Buffer()
    {
      mPtr = nullptr;
      mNum = -1;
    }

    Buffer &operator=(const Buffer &other) = delete;

    Buffer &operator=(Buffer &other)
    {
      //delete whatever was there
      delete_buffer();

      //move buffer ownership
      mId = other.mId;
      mOwned = other.mOwned;
      mTarget = other.mTarget;
      mPtr = other.mPtr;
      mNum = other.mNum;

      other.mId = 0;
      other.mOwned = false;
      other.mPtr = nullptr;
      other.mNum = -1;

      return *this;
    }

    virtual GLsizei element_size() const { return sizeof(T); }
    virtual GLsizei num_elements() const { return mNum; }

    //=====================================================
    // Upload data to GPU
    //=====================================================

    virtual void update(GLenum usage = GL_STATIC_DRAW) const
    {
      bind();
      glBufferData(target(), mNum * element_size(), mPtr, usage);
    }

    virtual void update(GLsizei offset, GLsizei num) const
    {
      bind();
      glBufferSubData(target(), offset * element_size(), mNum * element_size(), mPtr);
    }

    void set(T *data, GLsizei num) const
    {
      mPtr = data;
      mNum = num;

      bind();
      glBufferData(target(), mNum * sizeof(T), mPtr, usage());
    }

    //=====================================================
    // Download data from GPU
    //=====================================================

    virtual void
    get()
    {
      get(0, num_elements());
    }

    virtual void get(GLsizei offset, GLsizei num)
    {
      bind();
      glGetBufferSubData(target(), offset * element_size(), num * element_size(), mPtr);
    }

    void get(T *data)
    {
      get(data, 0, num_elements());
    }

    void get(T *data, GLsizei offset, GLsizei num)
    {
      bind();
      glGetBufferSubData(target(), offset * element_size(), num * element_size(), data);
    }

    void get(std::vector<T> &data)
    {
      get(data.data(), 0, num_elements);
    }

    void get(std::vector<T> &data, GLsizei offset, GLsizei num)
    {
      get(data.data(), offset, num);
    }

  protected:
    //=====================================================
    // Class Attributes
    //=====================================================
    // ! uniform does not have ownership of the pointer.
    // ! mPtr will be dangling if it get deleted or get out of scope
    T *mPtr;
    GLsizei mNum;
  };
}

#endif