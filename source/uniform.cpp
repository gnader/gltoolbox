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

#include <gltoolbox/uniform.h>
#include <gltoolbox/program.h>
using namespace gltoolbox;

#include <iostream>

BaseUniform::BaseUniform(Program *prog, GLint location)
    : mProg(prog), mLocation(location), mName("")
{
}

BaseUniform::BaseUniform(Program *prog, const std::string &name)
    : mProg(prog), mLocation(-1), mName(name)
{
}

BaseUniform::BaseUniform(Program *prog, GLint location, const std::string &name)
    : mProg(prog), mLocation(location), mName(name)
{
}

BaseUniform::~BaseUniform()
{
  mProg = nullptr;
}

// glProgramUniform1{i|ui|f|d}v
void BaseUniform::update_value(GLint prog, GLint loc, GLsizei cnt, int *data) const
{
  glProgramUniform1iv(prog, loc, cnt, data);
}

void BaseUniform::update_value(GLint prog, GLint loc, GLsizei cnt, unsigned int *data) const
{
  glProgramUniform1uiv(prog, loc, cnt, data);
}

void BaseUniform::update_value(GLint prog, GLint loc, GLsizei cnt, float *data) const
{
  // glProgramUniform1fv(prog, loc, cnt, data);
  std::cout << *data << std::endl;
}

void BaseUniform::update_value(GLint prog, GLint loc, GLsizei cnt, double *data) const
{
  glProgramUniform1dv(prog, loc, cnt, data);
}

// glProgramUniform2{i|ui|f|d}v
void BaseUniform::update_value(GLint prog, GLint loc, GLsizei cnt, std::array<int, 2> *data) const
{
  glProgramUniform2iv(prog, loc, cnt, data->data());
}

void BaseUniform::update_value(GLint prog, GLint loc, GLsizei cnt, std::array<unsigned int, 2> *data) const
{
  glProgramUniform2uiv(prog, loc, cnt, data->data());
}

void BaseUniform::update_value(GLint prog, GLint loc, GLsizei cnt, std::array<float, 2> *data) const
{
  glProgramUniform2fv(prog, loc, cnt, data->data());
}

void BaseUniform::update_value(GLint prog, GLint loc, GLsizei cnt, std::array<double, 2> *data) const
{
  glProgramUniform2dv(prog, loc, cnt, data->data());
}

// glProgramUniform3{i|ui|f|d}v
void BaseUniform::update_value(GLint prog, GLint loc, GLsizei cnt, std::array<int, 3> *data) const
{
  glProgramUniform3iv(prog, loc, cnt, data->data());
}

void BaseUniform::update_value(GLint prog, GLint loc, GLsizei cnt, std::array<unsigned int, 3> *data) const
{
  glProgramUniform3uiv(prog, loc, cnt, data->data());
}

void BaseUniform::update_value(GLint prog, GLint loc, GLsizei cnt, std::array<float, 3> *data) const
{
  glProgramUniform3fv(prog, loc, cnt, data->data());
}

void BaseUniform::update_value(GLint prog, GLint loc, GLsizei cnt, std::array<double, 3> *data) const
{
  glProgramUniform3dv(prog, loc, cnt, data->data());
}

// glProgramUniform4{i|ui|f|d}v
void BaseUniform::update_value(GLint prog, GLint loc, GLsizei cnt, std::array<int, 4> *data) const
{
  glProgramUniform4iv(prog, loc, cnt, data->data());
}

void BaseUniform::update_value(GLint prog, GLint loc, GLsizei cnt, std::array<unsigned int, 4> *data) const
{
  glProgramUniform4uiv(prog, loc, cnt, data->data());
}

void BaseUniform::update_value(GLint prog, GLint loc, GLsizei cnt, std::array<float, 4> *data) const
{
  glProgramUniform4fv(prog, loc, cnt, data->data());
}

void BaseUniform::update_value(GLint prog, GLint loc, GLsizei cnt, std::array<double, 4> *data) const
{
  glProgramUniform4dv(prog, loc, cnt, data->data());
}