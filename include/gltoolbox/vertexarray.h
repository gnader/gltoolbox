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
    //=====================================================
    // member types
    //=====================================================
  public:
    struct AttributeFormat
    {
      GLint size;
      GLenum type;
      GLboolean normalized;
      GLsizei stride;
      GLuint offset;
    };

  protected:
    struct IndexBuffer
    {
      std::shared_ptr<BaseBuffer> buffer;

      GLenum mode;
      GLenum type;
    };

    struct AttributeBuffer
    {
      std::shared_ptr<BaseBuffer> buffer;
      AttributeFormat format;
      GLuint divisor;
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
    void draw_arrays(GLenum mode, GLint first, GLsizei count) const;
    void draw_arrays(GLenum mode, GLint first, GLsizei count, GLsizei inum) const;

    void draw_elements() const;
    void draw_elements(GLsizei inum) const;
    void draw_elements(GLuint start, GLuint end) const;

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
      case 1:
        mIndices.type = GL_UNSIGNED_BYTE;
        break;
      case 2:
        mIndices.type = GL_UNSIGNED_SHORT;
        break;
      case 4:
        mIndices.type = GL_UNSIGNED_INT;
        break;
      }
    }

    std::weak_ptr<BaseBuffer> index_buffer() const
    {
      std::weak_ptr<BaseBuffer> wp = mIndices.buffer;
      return wp;
    }

    //=====================================================
    // Attribute Buffers
    //=====================================================
    size_t num_attributes() const { return mAttributes.size(); }

    bool has_attribute(const std::string &name) const;

    template <typename T>
    bool add_attribute(const std::string &name,
                       T *data, GLsizei count, GLsizei size,
                       GLenum type, GLsizei stride, GLsizei offset,
                       GLenum usage = GL_STATIC_DRAW, GLboolean normalized = GL_FALSE, GLuint divisor = 0)
    {
      auto search = mAttributes.find(name);
      if (search == mAttributes.end())
      {
        AttributeBuffer attr;
        attr.buffer.reset(new Buffer<T>(GL_ARRAY_BUFFER, data, count, usage));
        attr.format = {size, type, stride, offset};
        attr.divisor = divisor;

        mAttributes.insert({name, std::move(attr)});

        return true;
      }

      return false;
    }

    void remove_attribute(const std::string &name)
    {
      auto search = mAttributes.find(name);
      if (search != mAttributes.end())
      {
        //delete the buffer
        mAttributes[name].buffer.reset();
        mAttributes.erase(name);
      }
    }

    std::weak_ptr<BaseBuffer> attribute_buffer(const std::string &name) const
    {
      std::weak_ptr<BaseBuffer> wp = mAttributes.at(name).buffer;
      return wp;
    }

    const AttributeFormat &attribute_format(const std::string &name) const
    {
      return mAttributes.at(name).format;
    }

    AttributeFormat &attribute_format(const std::string &name)
    {
      return mAttributes.at(name).format;
    }

    const GLuint &attribute_divisor(const std::string &name) const
    {
      return mAttributes.at(name).divisor;
    }

    GLuint &attribute_divisor(const std::string &name)
    {
      return mAttributes.at(name).divisor;
    }

    void enable_attribute(const std::string &name, GLint loc) const;
    void enable_attributes(const std::unordered_map<std::string, GLint> &attributes) const;

    void disable_attribute(const std::string &name, GLint loc) const;
    void disable_attributes(const std::unordered_map<std::string, GLint> &attributes) const;

  protected:
    void create();
    void destroy();

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