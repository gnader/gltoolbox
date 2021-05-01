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
  public:
    struct BufferBinding
    {
      BaseBuffer *buffer;

      GLint size;
      GLenum type;
      GLboolean normalized;
      GLsizei stride;
      GLsizei pointer;
    };

  public:
    VertexArray();

    VertexArray(const VertexArray &other) = delete;
    VertexArray(const VertexArray &&temp);

    virtual ~VertexArray();

    VertexArray &operator=(const VertexArray &other) = delete;

    //=====================================================
    // Information
    //=====================================================
    inline GLuint id() const { return mId; }
    inline GLboolean is_valid() const { return glIsVertexArray(mId); }

    //=====================================================
    // Drawcalls
    //=====================================================
    inline GLenum &mode() { return mMode; }
    inline GLenum mode() const { return mMode; }

    inline void set_mode(GLenum mode) { mMode = mode; }

    void DrawElements();
    void DrawElements(GLsizei count);
    void DrawElements(GLuint start, GLuint end);

    //=====================================================
    // Index Buffer
    //=====================================================

    bool has_index_buffer() const;
    void set_index_buffer();

    //=====================================================
    // Vertex Buffers
    //=====================================================
    bool has_vertex_buffer() const;

    void attach_vertex_buffer(const std::string &name);
    void detach_vertex_buffer(const std::string &name);

    void vertex_buffer(const std::string &name);

  protected:
    GLuint mId;
    bool mOwned;

    GLenum mMode;

    //vertex buffers
  };
}

#endif