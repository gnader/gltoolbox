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

#ifndef __GLTOOLBOX_SHADER_H__
#define __GLTOOLBOX_SHADER_H__

#include <string>

#include "gl.h"

namespace gltoolbox
{
  class Shader
  {
  public:
    Shader(GLenum type = GL_FRAGMENT_SHADER);

    Shader(const Shader &other) = delete;
    Shader(Shader &&temp);

    virtual ~Shader();

    Shader &operator=(const Shader &other) = delete;
    Shader &operator==(Shader &temp);

    void set_source(const std::string &src) const;
    void set_source_from_file(const std::string &filename) const;

    bool compile() const;

    inline GLuint id() const { return mId; }

    inline bool is_valid() const { return glIsShader(mId) != 0; }

    inline bool compile_status() const { return get_parameter(GL_COMPILE_STATUS) != 0; }

    inline bool delete_status() const { return get_parameter(GL_DELETE_STATUS) != 0; }

    inline GLenum type() const { return GLenum(get_parameter(GL_SHADER_TYPE)); }
    std::string type_as_str() const;

    std::string info_log() const;
    inline GLsizei info_log_length() const { return get_parameter(GL_INFO_LOG_LENGTH); }

    std::string source() const;
    inline GLsizei source_length() const { return get_parameter(GL_SHADER_SOURCE_LENGTH); }

  protected:
    GLint get_parameter(const GLenum param) const;

  protected:
    GLuint mId;
    bool mOwned;
  };
}

#endif
