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

#include <gltoolbox/framebuffer.h>
using namespace gltoolbox;

FrameBuffer::FrameBuffer(GLenum target)
    : mId(0), mOwned(false), mTarget(target)
{
  create();
}

FrameBuffer::FrameBuffer(FrameBuffer &&temp)
{
  //delete whatever was there
  destroy();

  mId = temp.mId;
  mOwned = temp.mOwned;
  mTarget = temp.mTarget;
  for (auto &att : temp.mAttachments)
    mAttachments.insert({att.first, std::move(att.second)});

  temp.mId = 0;
  temp.mOwned = false;
}

FrameBuffer::~FrameBuffer()
{
  destroy();
}

GLenum FrameBuffer::status() const
{
  GLenum result;
  bind();
  result = glCheckFramebufferStatus(target());
  unbind();

  return result;
}

std::string FrameBuffer::status_as_string() const
{
  GLenum fbo_status = status();
  switch (fbo_status)
  {
  case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
    return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
  case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
    return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
  case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
    return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
  case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
    return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
  case GL_FRAMEBUFFER_UNSUPPORTED:
    return "GL_FRAMEBUFFER_UNSUPPORTED";
  }

  return "UNKNOWN_ERROR";
}

bool FrameBuffer::has_attachment(GLenum attachment) const
{
  auto search = mAttachments.find(attachment);
  return search != mAttachments.end();
}

bool FrameBuffer::attach(GLenum attachment,
                         GLenum textarget, GLsizei width, GLsizei height,
                         GLenum texformat, GLenum pixformat, GLenum pixtype)
{
  if (has_attachment(attachment)) //attachment already allocated
    return false;

  std::array<GLenum, 9> supported{GL_TEXTURE_2D, GL_TEXTURE_RECTANGLE,
                                  GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                                  GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
                                  GL_TEXTURE_2D_MULTISAMPLE};
  bool is_supported = std::find(supported.begin(), supported.end(), textarget) != supported.end();

  if (!is_supported) //target is not supported
    return false;

  bind();

  mAttachments[attachment] = std::make_shared<Texture>(textarget, GL_LINEAR, GL_LINEAR);
  mAttachments[attachment]->attach_to(nullptr, width, height, texformat, pixformat, pixtype); //allocate memory on GPU
  glFramebufferTexture2D(target(), attachment, textarget, mAttachments[attachment]->id(), 0);

  unbind();

  return true;
}

bool FrameBuffer::remove_attachment(GLenum attachement)
{
  if (has_attachment(attachement))
  {
    mAttachments[attachement].reset();
    mAttachments.erase(attachement);
  }

  return false;
}

std::shared_ptr<Texture> FrameBuffer::texture(GLenum attachment) const
{
  return mAttachments.at(attachment);
}

void FrameBuffer::create()
{
  if (!mOwned || !is_valid())
  {
    glCreateFramebuffers(1, &mId);
    mOwned = true;
  }
}

void FrameBuffer::destroy()
{
  if (mOwned && is_valid())
  {
    glDeleteFramebuffers(1, &mId);
    mId = 0;
    mOwned = false;

    for (auto &att : mAttachments)
      att.second.reset();
    mAttachments.clear();
  }
}
