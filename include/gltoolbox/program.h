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

#ifndef __GLTOOLBOX_PROGRAM_H__
#define __GLTOOLBOX_PROGRAM_H__

#include "shader.h"
#include "uniform.h"

#include <unordered_map>

namespace gltoolbox
{
  class Program
  {
  public:
    Program();

    Program(const Program &other) = delete;
    Program(Program &&temp);

    virtual ~Program();

    Program &operator=(const Program &other) = delete;

    inline GLuint id() const { return mId; }
    inline bool is_valid() const { return glIsProgram(mId) != 0; }

    bool link() const;
    inline bool link_status() const { return get_parameter(GL_LINK_STATUS) != 0; }
    inline bool delete_status() const { return get_parameter(GL_DELETE_STATUS) != 0; }

    inline void use() const { glUseProgram(mId); }
    inline void unuse() const { glUseProgram(0); }

    //============================
    // Shaders
    //============================

    inline GLint num_attached_shader() const { return get_parameter(GL_ATTACHED_SHADERS); }

    bool has_shader(GLenum type);
    inline const Shader &get_shader(GLenum type) const { return mShaderList.at(type); }

    void attach_shader(Shader &shader);
    void attach_shader(Shader &&temp);

    void detach_shader(GLenum type);

    //============================
    // Vertex Attributes
    //============================

    inline GLint num_active_attributes() const { return get_parameter(GL_ACTIVE_ATTRIBUTES); }
    inline const std::unordered_map<std::string, GLint> &attributes() const { return mAttributeList; }

    bool has_attribute(const std::string &name);

    bool add_attribute(const std::string &name);
    void add_attributes(const std::vector<std::string> &names);

    void remove_attribute(const std::string &name);

    //============================
    // Uniforms
    //============================

    inline GLint num_active_uniforms() const { return get_parameter(GL_ACTIVE_UNIFORMS); }

    template <typename T>
    bool add_uniform(const std::string &name, T *ptr, GLsizei count = 1)
    {
      GLint loc = glGetUniformLocation(id(), name.c_str());
      if (loc >= 0)
      {
        auto search = mUniformList.find(name);
        if (search != mUniformList.end())
          search->second.reset();
        mUniformList[name] = std::make_unique<Uniform<T>>(this, loc, ptr, count);
        return true;
      }
      return false;
    }

    void enable_uniforms() const;
    void enable_uniform(const std::string &name) const;

    template <typename T>
    void update_uniform(const std::string &name, T *ptr, GLsizei count = 1)
    {
      const auto &search = mUniformList.find(name);
      if (search != mUniformList.end())
        dynamic_cast<Uniform<T> *>(search->second.get())->update(ptr, count);
    }

    void remove_uniform(const std::string &name);

    //============================
    // Program Info
    //============================

    std::string info_log() const;
    inline GLsizei info_log_length() const { return get_parameter(GL_INFO_LOG_LENGTH); }

  protected:
    void create();
    void destroy();

    void delete_uniforms();

    GLint get_parameter(const GLenum param) const;

  protected:
    GLuint mId;
    bool mOwned;

    std::unordered_map<GLenum, Shader> mShaderList;

    std::unordered_map<std::string, GLint> mAttributeList;
    std::unordered_map<std::string, std::unique_ptr<BaseUniform>> mUniformList;
  };
}

#endif