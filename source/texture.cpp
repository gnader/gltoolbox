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

#include <gltoolbox/texture.h>
using namespace gltoolbox;

GLuint Texture::dimention(GLenum target)
{
  switch (target)
  {
  case GL_TEXTURE_1D:
  case GL_TEXTURE_BUFFER:
    return 1;
  case GL_TEXTURE_2D:
  case GL_TEXTURE_RECTANGLE:
  case GL_TEXTURE_2D_MULTISAMPLE:
    return 2;
  case GL_TEXTURE_3D:
    return 3;
  }

  return -1; // should not arrive here
}

Texture::Texture(GLenum target)
    : mId(0), mOwned(false), mTarget(target)
{
  mDimention = Texture::dimention(target);
  create();
  set_options(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_REPEAT);
  set_type(GL_UNSIGNED_BYTE);
  set_format(GL_RGB);
}

Texture::Texture(GLenum target,
                 GLenum minfunc, GLenum magfunc,
                 GLenum wraps, GLenum wrapt, GLenum wrapr)
    : mId(0), mOwned(false), mTarget(target)
{
  mDimention = Texture::dimention(target);
  create();
  set_options(minfunc, magfunc, wraps, wrapt, wrapr);
  set_type(GL_UNSIGNED_BYTE);
  set_format(GL_RGB);
}

Texture::Texture(Texture &&temp)
{
  //delete whatever was there
  destroy();

  mId = temp.mId;
  mOwned = temp.mOwned;
  mTarget = temp.mTarget;

  mDimention = Texture::dimention(mTarget);

  mTexFormat = temp.mTexFormat;
  mPixFormat = temp.mPixFormat;
  mPixType = temp.mPixType;

  temp.mId = 0;
  temp.mOwned = false;
}

Texture::~Texture()
{
  destroy();
}

Texture &Texture::operator=(Texture &other)
{
  //delete whatever was there
  destroy();

  mId = other.mId;
  mOwned = other.mOwned;
  mTarget = other.mTarget;

  mDimention = Texture::dimention(mTarget);

  mTexFormat = other.mTexFormat;
  mPixFormat = other.mPixFormat;
  mPixType = other.mPixType;

  other.mId = 0;
  other.mOwned = false;

  return *this;
}

void Texture::set_options(GLenum minfunc, GLenum magfunc, GLenum wraps) const
{
  bind();

  glTexParameteri(target(), GL_TEXTURE_MIN_FILTER, minfunc);
  glTexParameteri(target(), GL_TEXTURE_MAG_FILTER, magfunc);
  glTexParameteri(target(), GL_TEXTURE_WRAP_S, wraps);
  unbind();
}

void Texture::set_options(GLenum minfunc, GLenum magfunc, GLenum wraps, GLenum wrapt) const
{
  bind();

  glTexParameteri(target(), GL_TEXTURE_MIN_FILTER, minfunc);
  glTexParameteri(target(), GL_TEXTURE_MAG_FILTER, magfunc);
  glTexParameteri(target(), GL_TEXTURE_WRAP_S, wraps);
  glTexParameteri(target(), GL_TEXTURE_WRAP_T, wrapt);

  unbind();
}

void Texture::set_options(GLenum minfunc, GLenum magfunc, GLenum wraps, GLenum wrapt, GLenum wrapr) const
{
  bind();

  glTexParameteri(target(), GL_TEXTURE_MIN_FILTER, minfunc);
  glTexParameteri(target(), GL_TEXTURE_MAG_FILTER, magfunc);
  glTexParameteri(target(), GL_TEXTURE_WRAP_S, wraps);
  glTexParameteri(target(), GL_TEXTURE_WRAP_T, wrapt);
  glTexParameteri(target(), GL_TEXTURE_WRAP_R, wrapr);

  unbind();
}

void Texture::set_format(GLenum format)
{
  mTexFormat = format;
  mPixFormat = format;
}

void Texture::set_format(GLenum internal, GLenum pixel)
{
  mTexFormat = internal;
  mTexFormat = pixel;
}

void Texture::set_type(GLenum type)
{
  mPixType = type;
}

void Texture::generate_mipmaps() const
{
  bind();
  glGenerateTextureMipmap(id());
  unbind();
}

void Texture::upload(void *ptr, GLsizei width) const
{
  if (dim() == 1)
  {
    bind();
    glTexImage1D(target(), 0, internal_format(), width, 0, format(), type(), ptr);
    unbind();
  }
}

void Texture::upload(void *ptr, GLsizei width, GLsizei height) const
{
  if (dim() == 2)
  {
    bind();
    glTexImage2D(target(), 0, internal_format(), width, height, 0, format(), type(), ptr);
    unbind();
  }
}

void Texture::upload(void *ptr, GLsizei width, GLsizei height, GLsizei depth) const
{
  if (dim() == 3)
  {
    bind();
    glTexImage3D(target(), 0, internal_format(), width, height, depth, 0, format(), type(), ptr);
    unbind();
  }
}

void Texture::download(void *ptr)
{
  bind();
  glGetTexImage(target(), 0, format(), type(), ptr);
  unbind();
}

void Texture::create()
{
  if (!mOwned || !is_valid())
  {
    glCreateTextures(target(), 1, &mId);
    mOwned = true;
  }
}

void Texture::destroy()
{
  if (mOwned && is_valid())
  {
    glDeleteTextures(1, &mId);
    mId = 0;
    mOwned = false;
  }
}

GLint Texture::get_parameter(const GLenum param) const
{
  GLint value;
  glGetTexParameteriv(target(), param, &value);
  return value;
}
