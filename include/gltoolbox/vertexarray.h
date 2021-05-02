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

#ifndef __GLTOOLBOX_VERTEXARRAY_H__
#define __GLTOOLBOX_VERTEXARRAY_H__

#include "gl.h"
#include "buffer.h"

#include <unordered_map>

namespace gltoolbox
{
  class VertexArray
  {
  protected:
    //=====================================================
    // member types
    //=====================================================
    struct IndexBuffer
    {
      std::shared_ptr<BaseBuffer> buffer;

      GLenum mode;
      GLenum type;
    };

    struct AttributeBuffer
    {
      std::shared_ptr<BaseBuffer> buffer; //a buffer can be shared by multiple vertex arrays

      GLint size;
      GLenum type;
      GLboolean normalized;
      GLsizei stride;
      GLsizei pointer;
    };

  public:
    //=====================================================
    // Initialisation
    //=====================================================
    VertexArray();

    VertexArray(const VertexArray &other) = delete;
    VertexArray(VertexArray &&temp);

    virtual ~VertexArray();

    VertexArray &operator=(const VertexArray &other) = delete;

    //=====================================================
    // Information
    //=====================================================
    inline GLuint id() const { return mId; }
    inline GLboolean is_valid() const { return glIsVertexArray(mId); }

    //=====================================================
    // bind/unbind
    //=====================================================
    inline void bind() const { glBindVertexArray(mId); }
    inline void unbind() const { glBindVertexArray(0); }

    //=====================================================
    // Drawcalls
    //=====================================================
    void DrawElements() const;
    void DrawElements(GLsizei inum) const;
    void DrawElements(GLuint start, GLuint end) const;

    //=====================================================
    // Index Buffer
    //=====================================================
    bool has_index_buffer() const;

    template <typename T>
    void set_index_buffer(GLenum mode, T *indices, GLsizei count, GLenum usage = GL_STATIC_DRAW)
    {
      mIndices.buffer.reset(new Buffer<T>(GL_ELEMENT_ARRAY_BUFFER, indices, count, usage));
      mIndices.mode = mode;
      switch (sizeof(T))
      {
      case 8:
        mIndices.type = GL_UNSIGNED_BYTE;
        break;
      case 16:
        mIndices.type = GL_UNSIGNED_SHORT;
        break;
      case 32:
        mIndices.type = GL_UNSIGNED_INT;
        break;
      }
    }

    template <typename T>
    std::weak_ptr<Buffer<T>> index_buffer() const
    {
      return std::weak_ptr<Buffer<T>>(dynamic_cast<Buffer<T>>(mIndices.buffer.get()));
    }

    //=====================================================
    // Vertex Buffers
    //=====================================================
    bool has_buffer(const std::string &name) const;

    void add_buffer(const std::string &name);
    void remove_buffer(const std::string &name);

    template <typename T>
    std::weak_ptr<Buffer<T>> buffer(const std::string &name) const
    {
      return std::weak_ptr<Buffer<T>>(dynamic_cast<Buffer<T>>(mAttributes.at(name).buffer.get()));
    }

  protected:
    void delete_vertexarray();

  protected:
    GLuint mId;
    bool mOwned;

    // index buffer
    IndexBuffer mIndices;

    // vertex attribute buffers
    std::unordered_map<std::string, AttributeBuffer> mAttributes;
  };
}

#endif