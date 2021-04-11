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

void Shader::read_source_from_file(const std::string &filename, std::string &source)
{
  //initialise source
  source = "";

  //open file
  std::fstream file(filename.c_str(), std::ios::in);
  if (!file.is_open())
  {
    return;
  }

  //read file
  std::string buffer;
  while (std::getline(file, buffer))
    source += buffer;

  //close file
  file.close();
}

Shader::Shader(GLenum type)
    : mId(0), mType(type), mFilename("")
{
  mId = glCreateShader(mType);
}

Shader::~Shader()
{
  glDeleteShader(mId);
}

std::string Shader::type_string() const
{
  switch (mType)
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

bool Shader::compile() const
{
  if (is_valid())
    glCompileShader(mId);

  bool status = is_compiled();
  if (!status)
    print_info_log();

  return status;
}

bool Shader::is_compiled() const
{
  int status = 0;

  if (is_valid())
    glGetShaderiv(mId, GL_COMPILE_STATUS, &status);

  return status;
}

bool Shader::load_source_from_file(const std::string &filename)
{
  if (filename != "")
    mFilename = filename;

  std::string source;
  Shader::read_source_from_file(mFilename, source);
  bool status = load_source_from_string(source);

  return status;
}

bool Shader::load_source_from_string(const std::string &source)
{
  if (is_valid())
  {
    const GLchar *src = source.c_str();
    glShaderSource(mId, 1, (const GLchar **)&src, 0);
  }

  return compile();
}

bool Shader::reload()
{
  return load_source_from_file(mFilename);
}

void Shader::print_info_log() const
{
  int length = 0;
  glGetShaderiv(id(), GL_INFO_LOG_LENGTH, &length);

  if (length > 0)
  {
    std::vector<char> log(length);
    glGetShaderInfoLog(id(), length, &length, log.data());
  }
}