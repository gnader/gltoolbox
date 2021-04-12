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

  class Buffer
  {
  public:
    Buffer(GLenum target);
    virtual ~Buffer();

    inline const GLuint &id() const { return mId; }
    inline const GLenum &target() const { return mTarget; }

    inline bool is_created() const { return mId != 0; }

    GLint size() const;

    // Binds the buffer to target.
    void bind() const;

    // Unbinds a specific target, i.e. binds a 0 id to the target.
    void unbind() const;

    // Creates video memory for the buffer.
    void set_data(GLsizeiptr size, const GLvoid *data, GLenum usage);

    template <typename T>
    void set_data(const T &data, GLenum usage)
    {
      set_data(static_cast<GLsizeiptr>(sizeof(T)), &data, usage);
    }

    template <typename T>
    void set_data(const std::vector<T> &data, GLenum usage)
    {
      set_data(static_cast<GLsizeiptr>(data.size() * sizeof(T)), data.data(), usage);
    }

    template <typename T, std::size_t Count>
    void set_data(const std::array<T, Count> &data, GLenum usage)
    {
      set_data(static_cast<GLsizeiptr>(Count * sizeof(T)), data.data(), usage);
    }

    // Writes data only to a defined area of the memory.
    void set_subdata(GLintptr offset, GLsizeiptr size, const GLvoid *data = nullptr);

    template <typename T>
    void set_subdata(const T &data, gl::GLintptr offset = 0)
    {
      set_subdata(offset, static_cast<GLsizeiptr>(sizeof(T)), &data);
    }

    template <typename T>
    void set_subdata(const std::vector<T> &data, gl::GLintptr offset = 0)
    {
      set_subdata(offset, static_cast<GLsizeiptr>(data.size() * sizeof(T)), data.data());
    }

    template <typename T, std::size_t Count>
    void set_subdata(const std::array<T, Count> &data, gl::GLintptr offset = 0)
    {
      set_subdata(offset, static_cast<GLsizeiptr>(Count * sizeof(T)), data.data());
    }

  protected:
    GLuint mId;
    GLenum mTarget;
  };

}

#endif