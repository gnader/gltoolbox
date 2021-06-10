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

#ifndef __GL_H__
#define __GL_H__

#include <iostream>

#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>
using namespace gl;

namespace gltoolbox
{
  typedef glbinding::GetProcAddress ProcAddress;

  class GL
  {
  public:
    // all member functions are static, constructor is not needed
    GL() = delete;

  public:
    static void initilize(ProcAddress functionPointer, bool resolve = true)
    {
      glbinding::Binding::initialize(functionPointer, resolve);
    }

    //=====================================================
    // openGL info
    //=====================================================

    static const GLubyte *gl_version() { return glGetString(GL_VERSION); }
    static const GLubyte *glsl_version() { return glGetString(GL_SHADING_LANGUAGE_VERSION); }

    //=====================================================
    // info
    //=====================================================

    static void get_viewport(GLint *vp) { glGetIntegerv(GL_VIEWPORT, vp); }

    //=====================================================
    // Whole Framebuffer Operations
    //=====================================================

    static void clear(ClearBufferMask bitfield) { glClear(bitfield); }

    static void clear_color(float r, float g, float b, float a) { glClearColor(r, g, b, a); }
    static void clear_color(float l, float a = 1.f) { glClearColor(l, l, l, a); }
    static void clear_color(float *rgba) { glClearColor(rgba[0], rgba[1], rgba[2], rgba[3]); }

    static void clear_depth(float d) { glClearDepth(d); }
    static void clear_stencil(int s) { glClearStencil(s); }

    //=====================================================
    // Per-Fragment Operations
    //=====================================================

    static void enable_scissor() { glEnable(GL_SCISSOR_TEST); }
    static void disbale_scissor() { glDisable(GL_SCISSOR_TEST); }
    static GLboolean is_scissor_enabled() { return glIsEnabled(GL_SCISSOR_TEST); }

    static void enable_stencil() { glEnable(GL_STENCIL_TEST); }
    static void disbale_stencil() { glDisable(GL_STENCIL_TEST); }
    static GLboolean is_stencil_enabled() { return glIsEnabled(GL_STENCIL_TEST); }

    static void enable_depth() { glEnable(GL_DEPTH_TEST); }
    static void disbale_depth() { glDisable(GL_DEPTH_TEST); }
    static GLboolean is_depth_enabled() { return glIsEnabled(GL_DEPTH_TEST); }

    static void enable_blend() { glEnable(GL_BLEND); }
    static void disbale_blend() { glDisable(GL_BLEND); }
    static GLboolean is_blend_enabled() { return glIsEnabled(GL_BLEND); }

    static void enable_dither() { glEnable(GL_DITHER); }
    static void disbale_dither() { glDisable(GL_DITHER); }
    static GLboolean is_dither_enabled() { return glIsEnabled(GL_DITHER); }
  };
}

#endif // __GL_H__