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
    Buffer(GLenum target);

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

    //send data from CPU memory to GPU memory
    void upload(void *ptr, GLsizei size, GLenum usage = GL_STATIC_DRAW) const;
    void upload(void *ptr, GLsizei offset, GLsizei size) const;

    void download(void *ptr, GLsizei size) const;
    void download(void *ptr, GLsizei offset, GLsizei size) const;

    // functions used in case the buffer is attached to some memory
    inline bool is_attached() const { return mIsAttached; }

    inline GLsizei num_elements() const { return mElementNum; }
    inline GLsizei element_size() const { return mElementSize; }

    template <typename T>
    void attach_to(T *ptr, GLsizei num)
    {
      mPtr = ptr;
      mElementNum = num;
      mElementSize = sizeof(T);
      mIsAttached = true;

      update();
    }

    void detach();

    void update(GLenum usage = GL_STATIC_DRAW) const;
    void update(GLsizei offset, GLsizei num) const;

    void download();

  protected:
    void create();
    void destroy();

    GLint get_parameter(const GLenum param) const;

  protected:
    GLuint mId;
    bool mOwned;

    GLenum mTarget;

    bool mIsAttached;
    // ! buffer does not have ownership of the pointer.
    // ! mPtr will be dangling if it get deleted or get out of scope
    void *mPtr;
    GLsizei mElementNum;
    GLsizei mElementSize;
  };

  // template <typename T>
  // class Buffer : public BaseBuffer
  // {
  // public:
  //   Buffer(GLenum target)
  //       : BaseBuffer(target), mPtr(nullptr), mIsLinked(false), mNum(-1)
  //   {
  //   }

  //   Buffer(GLenum target, T *data, GLsizei num, GLenum usage = GL_STATIC_DRAW)
  //       : BaseBuffer(target), mPtr(data), mNum(num)
  //   {
  //     mIsLinked = (data == nullptr);
  //     update(usage);
  //   }

  //   Buffer(const Buffer &other) = delete;

  //   Buffer(Buffer &&temp)
  //       : BaseBuffer(temp)
  //   {
  //     mPtr = temp.mPtr;
  //     mNum = temp.mNum;

  //     temp.mPtr = nullptr;
  //     temp.mNum = -1;
  //   }

  //   virtual ~Buffer()
  //   {
  //     mPtr = nullptr;
  //     mNum = -1;
  //   }

  //   Buffer &operator=(const Buffer &other) = delete;

  //   Buffer &operator=(Buffer &other)
  //   {
  //     //delete whatever was there
  //     destroy();

  //     //move buffer ownership
  //     mId = other.mId;
  //     mOwned = other.mOwned;
  //     mTarget = other.mTarget;
  //     mPtr = other.mPtr;
  //     mNum = other.mNum;

  //     other.mId = 0;
  //     other.mOwned = false;
  //     other.mPtr = nullptr;
  //     other.mNum = -1;

  //     return *this;
  //   }

  //   virtual GLsizei element_size() const { return sizeof(T); }
  //   virtual GLsizei num_elements() const { return mNum; }

  //   virtual bool is_linked() const { return mIsLinked; }

  //   //=====================================================
  //   // update data (cpu -> gpu)
  //   //=====================================================

  //   virtual void update(GLenum usage = GL_STATIC_DRAW) const
  //   {
  //     upload(mPtr, mNum * element_size(), usage);
  //   }

  //   virtual void update(GLsizei offset, GLsizei num) const
  //   {
  //     upload(mPtr, offset * element_size(), mNum * element_size());
  //   }

  //   void link_to(T *data, GLsizei num) const
  //   {
  //     mPtr = data;
  //     mNum = num;

  //     upload(mPtr, mNum * element_size(), usage();
  //   }

  //   //=====================================================
  //   // updata data (gpu -> cpu)
  //   //=====================================================

  //   virtual void get() const
  //   {
  //     get(0, num_elements());
  //   }

  //   virtual void get(GLsizei offset, GLsizei num) const
  //   {
  //     bind();
  //     glGetBufferSubData(target(), offset * element_size(), num * element_size(), mPtr);
  //   }

  //   void get(T *data)
  //   {
  //     get(data, 0, num_elements());
  //   }

  //   void get(T *data, GLsizei offset, GLsizei num)
  //   {
  //     bind();
  //     glGetBufferSubData(target(), offset * element_size(), num * element_size(), data);
  //   }

  //   void get(std::vector<T> &data)
  //   {
  //     get(data.data(), 0, num_elements());
  //   }

  //   void get(std::vector<T> &data, GLsizei offset, GLsizei num)
  //   {
  //     get(data.data(), offset, num);
  //   }

  // protected:
  //   //=====================================================
  //   // Class Attributes
  //   //=====================================================
  //   // ! buffer does not have ownership of the pointer.
  //   // ! mPtr will be dangling if it get deleted or get out of scope
  //   T *mPtr;
  //   bool mIsLinked;
  //   GLsizei mNum;
  // };
}

#endif