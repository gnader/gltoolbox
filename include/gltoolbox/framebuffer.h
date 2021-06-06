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

#ifndef __GLTOOLBOX_FRAMEBUFFER_H__
#define __GLTOOLBOX_FRAMEBUFFER_H__

#include "gl.h"
#include "texture.h"

namespace gltoolbox
{
  class FrameBuffer
  {
  public:
    FrameBuffer(GLenum target);

    FrameBuffer(const FrameBuffer &other) = delete;
    FrameBuffer(FrameBuffer &&temp);

    virtual ~FrameBuffer();

    FrameBuffer &operator=(const FrameBuffer &other) = delete;
    FrameBuffer &operator=(FrameBuffer &other);

    inline GLuint id() const { return mId; }
    inline bool is_valid() const { return (glIsFramebuffer(mId) == GL_TRUE); }

    inline GLenum target() const { return mTarget; }

    inline void bind() const { glBindFramebuffer(mTarget, mId); }
    inline void unbind() const { glBindFramebuffer(mTarget, 0); }

    GLenum status() const;
    std::string status_as_string() const;

    void attach(GLenum target);

  protected:
    void create();
    void destroy();

  protected:
    GLuint mId;
    bool mOwned;

    GLenum mTarget;

    std::unordered_map<GLenum, std::shared_ptr<Texture>> mAttachements;
  };
}

#endif