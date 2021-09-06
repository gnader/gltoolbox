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

#ifndef __GLTOOLBOX_TEXTURE_H__
#define __GLTOOLBOX_TEXTURE_H__

#include "gl.h"

namespace gltoolbox
{
  class Texture
  {
  public:
    inline static void activate(GLuint unit = 0)
    {
      glActiveTexture(GL_TEXTURE0 + unit);
    }

    inline static void unpack_alignment(GLint value)
    {
      glPixelStorei(GL_UNPACK_ALIGNMENT, value);
    }

    static GLuint dimention(GLenum target);

  public:
    Texture(GLenum target);
    Texture(GLenum target,
            GLenum minfunc, GLenum magfunc,
            GLenum wraps = GL_REPEAT, GLenum wrapt = GL_REPEAT, GLenum wrapr = GL_REPEAT);

    Texture(const Texture &other) = delete;
    Texture(Texture &&temp);

    virtual ~Texture();

    Texture &operator=(const Texture &other) = delete;
    Texture &operator=(Texture &other);

    inline GLint id() const { return mId; }
    inline bool is_valid() const { return glIsTexture(mId) == GL_TRUE; }

    inline GLenum target() const { return mTarget; }
    inline GLuint dim() const { return mDimention; }

    inline GLenum internal_format() const { return mTexFormat; }
    inline GLenum format() const { return mPixFormat; }
    inline GLenum type() const { return mPixType; }

    inline void bind() const { glBindTexture(target(), id()); }
    inline void unbind() const { glBindTexture(target(), 0); }

    void set_type(GLenum type);
    void set_format(GLenum format);
    void set_format(GLenum internal, GLenum pixel);

    void set_options(GLenum minfunc, GLenum magfunc, GLenum wraps) const;
    void set_options(GLenum minfunc, GLenum magfunc, GLenum wraps, GLenum wrapt) const;
    void set_options(GLenum minfunc, GLenum magfunc, GLenum wraps, GLenum wrapt, GLenum wrapr) const;

    void generate_mipmaps() const;

    void upload(void *ptr, GLsizei width) const;
    void upload(void *ptr, GLsizei width, GLsizei height) const;
    void upload(void *ptr, GLsizei width, GLsizei height, GLsizei depth) const;

    void download(void *ptr);

  protected:
    void create();
    void destroy();

    GLint get_parameter(const GLenum param) const;

  protected:
    GLuint mId;
    bool mOwned;

    GLenum mTarget;
    GLuint mDimention;

    GLenum mPixType;
    GLenum mPixFormat;
    GLenum mTexFormat;
  };
}

#endif