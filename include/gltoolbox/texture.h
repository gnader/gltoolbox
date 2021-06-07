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

    inline GLsizei width() const { return mWidth; }
    inline GLsizei height() const { return mHeight; }
    inline GLsizei depth() const { return mDepth; }

    inline GLenum internal_format() const { return mTexFormat; }
    inline GLenum format() const { return mPixFormat; }
    inline GLenum type() const { return mPixType; }

    inline void bind() const { glBindTexture(target(), id()); }
    inline void unbind() const { glBindTexture(target(), 0); }

    inline bool is_attached() const { return mPtr != nullptr; }

    void set_texture_options(GLenum minfunc, GLenum magfunc, GLenum wraps) const;
    void set_texture_options(GLenum minfunc, GLenum magfunc, GLenum wraps, GLenum wrapt) const;
    void set_texture_options(GLenum minfunc, GLenum magfunc, GLenum wraps, GLenum wrapt, GLenum wrapr) const;

    void generate_mipmaps() const;

    void attach_to(void *ptr, GLsizei width, GLenum texformat, GLenum pixformat, GLenum pixtype);
    void attach_to(void *ptr, GLsizei width, GLsizei height, GLenum texformat, GLenum pixformat, GLenum pixtype);
    void attach_to(void *ptr, GLsizei width, GLsizei height, GLsizei depth, GLenum texformat, GLenum pixformat, GLenum pixtype);

    void update() const;

    void update(void *ptr, GLsizei width, GLenum texformat, GLenum pixformat, GLenum pixtype) const;
    void update(void *ptr, GLsizei width, GLsizei height, GLenum texformat, GLenum pixformat, GLenum pixtype) const;
    void update(void *ptr, GLsizei width, GLsizei height, GLsizei depth, GLenum texformat, GLenum pixformat, GLenum pixtype) const;

    void get();
    void get(void *ptr);
    void get(void *ptr, GLenum format, GLenum type);

  protected:
    void create();
    void destroy();

    GLint get_parameter(const GLenum param) const;

  protected:
    GLuint mId;
    bool mOwned;

    GLenum mTarget;

    GLuint mDimention;
    GLsizei mWidth;
    GLsizei mHeight; // if texture is 1D -> mHeight and mDepth have value 0
    GLsizei mDepth;  // if texture is 2D -> mDepth have value 0

    GLenum mTexFormat;
    GLenum mPixFormat;
    GLenum mPixType;

    // ! texture does not have ownership of the pointer.
    // ! mPtr will be dangling if it get deleted or get out of scope
    void *mPtr;
  };
}

#endif