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

  return -1;
}

Texture::Texture(GLenum target)
    : mId(0), mOwned(false), mTarget(target),
      mPtr(nullptr), mWidth(0), mHeight(0), mDepth(0)
{
  mDimention = Texture::dimention(target);
  create();
  set_texture_options(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_REPEAT);
}

Texture::Texture(GLenum target,
                 GLenum minfunc, GLenum magfunc,
                 GLenum wraps = GL_REPEAT, GLenum wrapt = GL_REPEAT, GLenum wrapr = GL_REPEAT)
    : mId(0), mOwned(false), mTarget(target),
      mPtr(nullptr), mWidth(0), mHeight(0), mDepth(0)
{
  mDimention = Texture::dimention(target);
  create();
  set_texture_options(minfunc, magfunc, wraps, wrapt, wrapr);
}

Texture::Texture(Texture &&temp)
{
  //delete whatever was there
  destroy();

  mId = temp.mId;
  mOwned = temp.mOwned;
  mTarget = temp.mTarget;

  mDimention = Texture::dimention(mTarget);
  mWidth = temp.mWidth;
  mHeight = temp.mHeight;
  mDepth = temp.mDepth;

  mTexFormat = temp.mTexFormat;
  mPixFormat = temp.mPixFormat;
  mPixType = temp.mPixType;

  mPtr = temp.mPtr;

  temp.mId = 0;
  temp.mOwned = false;
  temp.mPtr = nullptr;
  temp.mWidth = 0;
  temp.mHeight = 0;
  temp.mDepth = 0;
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
  mWidth = other.mWidth;
  mHeight = other.mHeight;
  mDepth = other.mDepth;

  mTexFormat = other.mTexFormat;
  mPixFormat = other.mPixFormat;
  mPixType = other.mPixType;

  mPtr = other.mPtr;

  other.mId = 0;
  other.mOwned = false;
  other.mPtr = nullptr;
  other.mWidth = 0;
  other.mHeight = 0;
  other.mDepth = 0;

  return *this;
}

void Texture::set_texture_options(GLenum minfunc, GLenum magfunc, GLenum wraps) const
{
  bind();

  glTexParameteri(target(), GL_TEXTURE_MIN_FILTER, minfunc);
  glTexParameteri(target(), GL_TEXTURE_MAG_FILTER, magfunc);
  glTexParameteri(target(), GL_TEXTURE_WRAP_S, wraps);
  unbind();
}

void Texture::set_texture_options(GLenum minfunc, GLenum magfunc, GLenum wraps, GLenum wrapt) const
{
  bind();

  glTexParameteri(target(), GL_TEXTURE_MIN_FILTER, minfunc);
  glTexParameteri(target(), GL_TEXTURE_MAG_FILTER, magfunc);
  glTexParameteri(target(), GL_TEXTURE_WRAP_S, wraps);
  glTexParameteri(target(), GL_TEXTURE_WRAP_T, wrapt);

  unbind();
}

void Texture::set_texture_options(GLenum minfunc, GLenum magfunc, GLenum wraps, GLenum wrapt, GLenum wrapr) const
{
  bind();

  glTexParameteri(target(), GL_TEXTURE_MIN_FILTER, minfunc);
  glTexParameteri(target(), GL_TEXTURE_MAG_FILTER, magfunc);
  glTexParameteri(target(), GL_TEXTURE_WRAP_S, wraps);
  glTexParameteri(target(), GL_TEXTURE_WRAP_T, wrapt);
  glTexParameteri(target(), GL_TEXTURE_WRAP_R, wrapr);

  unbind();
}

void Texture::generate_mipmaps() const
{
  bind();
  glGenerateTextureMipmap(id());
  unbind();
}

void Texture::attach_to(void *ptr, GLsizei width, GLenum texformat, GLenum pixformat, GLenum pixtype)
{
  mPtr = ptr;
  mWidth = width;
  mTexFormat = texformat;
  mPixFormat = pixformat;
  mPixType = pixtype;

  update();
}

void Texture::attach_to(void *ptr, GLsizei width, GLsizei height, GLenum texformat, GLenum pixformat, GLenum pixtype)
{
  mPtr = ptr;
  mWidth = width;
  mHeight = height;
  mTexFormat = texformat;
  mPixFormat = pixformat;
  mPixType = pixtype;

  update();
}

void Texture::attach_to(void *ptr, GLsizei width, GLsizei height, GLsizei depth, GLenum texformat, GLenum pixformat, GLenum pixtype)
{
  mPtr = ptr;
  mWidth = width;
  mHeight = height;
  mDepth = depth;
  mTexFormat = texformat;
  mPixFormat = pixformat;
  mPixType = pixtype;

  update();
}

void Texture::update() const
{
  if (is_attached())
  {
    bind();
    if (dim() == 1)
      glTexImage1D(target(), 0, mTexFormat, mWidth, 0, mPixFormat, mPixType, mPtr);
    else if (dim() == 2)
      glTexImage2D(target(), 0, mTexFormat, mWidth, mHeight, 0, mPixFormat, mPixType, mPtr);
    else if (dim() == 3)
      glTexImage3D(target(), 0, mTexFormat, mWidth, mHeight, mDepth, 0, mPixFormat, mPixType, mPtr);
    unbind();
  }
}

void Texture::update(void *ptr, GLsizei width, GLenum texformat, GLenum pixformat, GLenum pixtype) const
{
  if (dim() == 1)
  {
    bind();
    glTexImage1D(target(), 0, texformat, width, 0, pixformat, pixtype, ptr);
    unbind();
  }
}

void Texture::update(void *ptr, GLsizei width, GLsizei height, GLenum texformat, GLenum pixformat, GLenum pixtype) const
{
  if (dim() == 2)
  {
    bind();
    glTexImage2D(target(), 0, texformat, width, height, 0, pixformat, pixtype, ptr);
    unbind();
  }
}

void Texture::update(void *ptr, GLsizei width, GLsizei height, GLsizei depth, GLenum texformat, GLenum pixformat, GLenum pixtype) const
{
  if (dim() == 3)
  {
    bind();
    glTexImage3D(target(), 0, texformat, width, height, depth, 0, pixformat, pixtype, ptr);
    unbind();
  }
}

void Texture::get()
{
  if (is_attached())
  {
    bind();
    glGetTexImage(target(), 0, format(), type(), mPtr);
    unbind();
  }
}

void Texture::get(void *ptr)
{
  if (is_attached())
  {
    bind();
    glGetTexImage(target(), 0, format(), type(), ptr);
    unbind();
  }
}

void Texture::get(void *ptr, GLenum format, GLenum type)
{
  bind();
  glGetTexImage(target(), 0, format, type, ptr);
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

    mPtr = nullptr;
    mWidth = 0;
    mHeight = 0;
    mDepth = 0;
  }
}

GLint Texture::get_parameter(const GLenum param) const
{
  GLint value;
  glGetTexParameteriv(target(), param, &value);
  return value;
}
