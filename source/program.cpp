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

#include <gltoolbox/program.h>
using namespace gltoolbox;

Program::Program()
    : mId(0), mOwned(true)
{
  mId = glCreateProgram();
}

Program::Program(Program &&temp)
{
  delete_program();
  delete_uniforms();

  mId = temp.mId;
  mOwned = temp.mOwned;
  mShaderList = std::move(temp.mShaderList);
  mUniformList = std::move(temp.mUniformList);

  temp.mId = 0;
  temp.mOwned = false;
}

Program::~Program()
{
  delete_program();
  delete_uniforms();
}

bool Program::link() const
{
  if (is_valid())
    glLinkProgram(mId);

  return link_status();
}

void Program::attach_shader(Shader &shader)
{
  GLenum type = shader.type();

  // ! If mShaderList[type] is already assigned, the shader will be deleted.
  // ! Input shader will be no longer be valid
  mShaderList[type] = shader;
  glAttachShader(id(), mShaderList[type].id());
}

void Program::attach_shader(Shader &&temp)
{
  GLenum type = temp.type();

  mShaderList[type] = temp;
  glAttachShader(id(), mShaderList[type].id());
}

void Program::detach_shader(GLenum type)
{
  glDetachShader(id(), mShaderList.at(type).id());
  mShaderList.erase(type);
}

void Program::update_uniform() const
{
  for (const auto &[name, ptr] : mUniformList)
    ptr.get()->update();
}

void Program::update_uniform(const std::string &name) const
{
  const auto &search = mUniformList.find(name);
  if (search != mUniformList.end())
    search->second.get()->update();
}

void Program::remove_uniform(const std::string &name)
{
  auto search = mUniformList.find(name);
  if (search != mUniformList.end())
    search->second.reset();
  mUniformList.erase(name);
}

void Program::delete_program()
{
  if (mOwned && is_valid())
  {
    glDeleteProgram(mId);
    mId = 0;
  }
}

void Program::delete_uniforms()
{
  for (auto &[name, ptr] : mUniformList)
    ptr.reset();
}

GLint Program::get_parameter(const GLenum param) const
{
  GLint value;
  glGetProgramiv(id(), param, &value);
  return value;
}
