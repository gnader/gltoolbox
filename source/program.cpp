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
#include <gltoolbox/texture.h>
using namespace gltoolbox;

Program::Program()
    : mId(0), mOwned(false)
{
  create();
}

Program::Program(Program &&temp)
{
  destroy();
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
  destroy();
  delete_uniforms();
}

bool Program::link() const
{
  if (is_valid())
    glLinkProgram(mId);

  return link_status();
}

bool Program::has_shader(GLenum type)
{
  auto search = mShaderList.find(type);
  return (search != mShaderList.end());
}

void Program::attach_shader(const std::string &src, GLenum type)
{
  if (has_shader(type))
    detach_shader(type);

  mShaderList[type] = std::make_shared<Shader>(src, type);
  glAttachShader(id(), mShaderList[type]->id());
}

void Program::attach_shader(const std::shared_ptr<Shader> &shader)
{
  GLenum type = shader->type();

  if (has_shader(type))
    detach_shader(type);

  mShaderList[type] = shader;
  glAttachShader(id(), mShaderList[type]->id());
}

void Program::detach_shader(GLenum type)
{
  glDetachShader(id(), mShaderList.at(type)->id());
  mShaderList.erase(type);
}

bool Program::has_attribute(const std::string &name) const
{
  auto search = mAttributeList.find(name);
  return search != mAttributeList.end();
}

bool Program::add_attribute(const std::string &name)
{
  int loc = glGetAttribLocation(id(), name.c_str());
  bool success = (loc >= 0);

  if (success)
    mAttributeList[name] = loc;

  return success;
}

void Program::add_attributes(const std::vector<std::string> &names)
{
  for (const auto &name : names)
    add_attribute(name);
}

void Program::remove_attribute(const std::string &name)
{
  mAttributeList.erase(name);
}

bool Program::has_sampler(const std::string &name) const
{
  auto search = mSamplerList.find(name);
  return search != mSamplerList.end();
}

bool Program::add_sampler(const std::string &name, GLint unit)
{
  int loc = glGetAttribLocation(id(), name.c_str());
  bool success = (loc >= 0);
  if (success)
    mSamplerList[name] = {loc, unit};
  return success;
}

void Program::enable_samplers() const
{
  for (const auto &[name, sampler] : mSamplerList)
  {
    Texture::activate(sampler.second);
    glProgramUniform1i(id(), sampler.first, sampler.second);
  }
}

void Program::enable_sampler(const std::string &name) const
{
  const auto &sampler = mSamplerList.at(name);
  Texture::activate(sampler.second);
  glProgramUniform1i(id(), sampler.first, sampler.second);
}

void Program::remove_sampler(const std::string &name)
{
  mSamplerList.erase(name);
}

void Program::enable_uniforms() const
{
  for (const auto &[name, ptr] : mUniformList)
    ptr.get()->update();
}

void Program::enable_uniform(const std::string &name) const
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

std::string Program::info_log() const
{
  std::string log;
  log.resize(info_log_length());
  glGetProgramInfoLog(id(), static_cast<GLsizei>(log.size()), 0, log.data());

  return log;
}

void Program::create()
{
  if (!mOwned || !is_valid())
  {
    mId = glCreateProgram();
    mOwned = true;
  }
}

void Program::destroy()
{
  if (mOwned && is_valid())
  {
    glDeleteProgram(mId);
    mId = 0;
    mOwned = false;
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
