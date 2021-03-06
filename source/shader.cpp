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

#include <gltoolbox/shader.h>
using namespace gltoolbox;

#include <fstream>
#include <iostream>

std::string Shader::src_from_file(const std::string &filename)
{
  std::ifstream stream(filename);
  return std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
}

Shader::Shader()
    : mId(0), mOwned(false), mFilename(""), mIsFromFile(false)
{
}

Shader::Shader(const std::string &src, GLenum type)
    : mId(0), mOwned(false), mFilename(""), mIsFromFile(false)
{
  create(type);
  set_source(src);
}

Shader::Shader(Shader &&temp)
{
  destroy();

  mId = temp.mId;
  mOwned = temp.mOwned;
  mFilename = temp.mFilename;
  mIsFromFile = temp.mIsFromFile;

  temp.mId = 0;
  temp.mOwned = false;
}

Shader::~Shader()
{
  destroy();
}

bool Shader::compile() const
{
  if (is_valid())
    glCompileShader(mId);

  return compile_status();
}

std::string Shader::type_as_str() const
{
  switch (type())
  {
  case GL_FRAGMENT_SHADER:
    return "Fragment Shader";
    break;
  case GL_VERTEX_SHADER:
    return "Vertex Shader";
    break;
  case GL_GEOMETRY_SHADER:
    return "Geometry Shader";
    break;
  case GL_TESS_CONTROL_SHADER:
    return "Tesselation Control Shader";
    break;
  case GL_TESS_EVALUATION_SHADER:
    return "Teselation Evaluation Shader";
    break;
  case GL_COMPUTE_SHADER:
    return "Compute Shader";
    break;
  default:
    return "Unknow Type Shader";
    break;
  }
}

std::string Shader::info_log() const
{
  std::string log;
  log.resize(info_log_length());
  glGetShaderInfoLog(id(), static_cast<GLsizei>(log.size()), 0, log.data());

  return log;
}

std::string Shader::source() const
{
  std::string src;
  src.resize(source_length());
  glGetShaderSource(id(), static_cast<GLsizei>(src.size()), 0, src.data());

  return src;
}

void Shader::set_source(const std::string &src) const
{
  const GLchar *_src = src.c_str();
  glShaderSource(mId, 1, (const GLchar **)&_src, 0);

  bool success = compile();
  if (!success)
    std::cerr << "[shader::set_source()] : unable to compile " << type_as_str() << std::endl
              << info_log() << std::endl;
}

void Shader::set_source_file(const std::string &filename)
{
  mFilename = filename;
  mIsFromFile = true;
}

void Shader::create(GLenum type)
{
  if (!mOwned || !is_valid())
  {
    mId = glCreateShader(type);
    mOwned = true;
  }
}

void Shader::destroy()
{
  if (mOwned && is_valid())
  {
    glDeleteShader(mId);
    mId = 0;
    mOwned = false;
  }
}

GLint Shader::get_parameter(const GLenum param) const
{
  GLint result;
  glGetShaderiv(id(), param, &result);
  return result;
}