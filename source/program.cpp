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
  mId = temp.mId;
  mOwned = temp.mOwned;

  temp.mId = 0;
  temp.mOwned = false;
}

Program::~Program()
{
  if (mOwned && is_valid())
    glDeleteShader(mId);
}

Program &Program::operator==(Program &temp)
{
  mId = temp.mId;
  mOwned = temp.mOwned;

  temp.mId = 0;
  temp.mOwned = false;

  return *this;
}

void Program::attach_shader()
{
}

void Program::detach_shader()
{
}

bool Program::link() const
{
  glLinkProgram(mId);
  return link_status();
}

bool Program::link_status() const
{
  int status = 0;
  glGetProgramiv(mId, GL_LINK_STATUS, &status);
  return status;
}

std::string Program::info_log() const
{
  std::string log;
  return log;
}

// Program::Program()
//     : mId(0)
// {
//   mId = glCreateProgram();
// }

// Program::~Program()
// {
//   glDeleteProgram(id());
// }

// void Program::attach_shader(Shader *s)
// {
//   if (s == nullptr)
//     return;

//   if (is_valid())
//     glAttachShader(id(), s->id());

//   mShaders.insert(s);
// }

// void Program::detach_shader(Shader *s)
// {
//   if (s == nullptr)
//     return;

//   if (is_valid())
//     glDetachShader(id(), s->id());

//   mShaders.erase(s);
// }

// bool Program::link() const
// {
//   glLinkProgram(id());
//   bool status = is_linked();
//   if (!status)
//     print_info_log();
//   return status;
// }

// bool Program::is_linked() const
// {
//   int status = 0;
//   if (is_valid())
//     glGetProgramiv(id(), GL_LINK_STATUS, &status);
//   return status;
// }

// void Program::activate() const
// {
//   glUseProgram(id());
// }

// void Program::desactivate() const
// {
//   glUseProgram(0);
// }

// int Program::getAttributeLocation(const char *name) const
// {
//   return glGetAttribLocation(id(), name);
// }

// int Program::getUniformLocation(const char *name) const
// {
//   return glGetUniformLocation(id(), name);
// }

// void Program::print_info_log() const
// {
//   int length = 0;
//   glGetProgramiv(id(), GL_INFO_LOG_LENGTH, &length);

//   if (length > 0)
//   {
//     std::vector<char> log(length);
//     glGetProgramInfoLog(id(), length, &length, log.data());
//   }
// }