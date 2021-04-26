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
void BaseUniform::update_value(GLsizei cnt, int *data) const
{
  glProgramUniform1iv(mProg->id(), mLocation, cnt, data);
}

void BaseUniform::update_value(GLsizei cnt, unsigned int *data) const
{
  glProgramUniform1uiv(mProg->id(), mLocation, cnt, data);
}

void BaseUniform::update_value(GLsizei cnt, float *data) const
{
  glProgramUniform1fv(mProg->id(), mLocation, cnt, data);
}

void BaseUniform::update_value(GLsizei cnt, double *data) const
{
  glProgramUniform1dv(mProg->id(), mLocation, cnt, data);
}

// glProgramUniform2{i|ui|f|d}v
void BaseUniform::update_value(GLsizei cnt, std::array<int, 2> *data) const
{
  glProgramUniform2iv(mProg->id(), mLocation, cnt, data->data());
}

void BaseUniform::update_value(GLsizei cnt, std::array<unsigned int, 2> *data) const
{
  glProgramUniform2uiv(mProg->id(), mLocation, cnt, data->data());
}

void BaseUniform::update_value(GLsizei cnt, std::array<float, 2> *data) const
{
  glProgramUniform2fv(mProg->id(), mLocation, cnt, data->data());
}

void BaseUniform::update_value(GLsizei cnt, std::array<double, 2> *data) const
{
  glProgramUniform2dv(mProg->id(), mLocation, cnt, data->data());
}

// glProgramUniform3{i|ui|f|d}v
void BaseUniform::update_value(GLsizei cnt, std::array<int, 3> *data) const
{
  glProgramUniform3iv(mProg->id(), mLocation, cnt, data->data());
}

void BaseUniform::update_value(GLsizei cnt, std::array<unsigned int, 3> *data) const
{
  glProgramUniform3uiv(mProg->id(), mLocation, cnt, data->data());
}

void BaseUniform::update_value(GLsizei cnt, std::array<float, 3> *data) const
{
  glProgramUniform3fv(mProg->id(), mLocation, cnt, data->data());
}

void BaseUniform::update_value(GLsizei cnt, std::array<double, 3> *data) const
{
  glProgramUniform3dv(mProg->id(), mLocation, cnt, data->data());
}

// glProgramUniform4{i|ui|f|d}v
void BaseUniform::update_value(GLsizei cnt, std::array<int, 4> *data) const
{
  glProgramUniform4iv(mProg->id(), mLocation, cnt, data->data());
}

void BaseUniform::update_value(GLsizei cnt, std::array<unsigned int, 4> *data) const
{
  glProgramUniform4uiv(mProg->id(), mLocation, cnt, data->data());
}

void BaseUniform::update_value(GLsizei cnt, std::array<float, 4> *data) const
{
  glProgramUniform4fv(mProg->id(), mLocation, cnt, data->data());
}

void BaseUniform::update_value(GLsizei cnt, std::array<double, 4> *data) const
{
  glProgramUniform4dv(mProg->id(), mLocation, cnt, data->data());
}

#ifdef GLTOOLBOX_ENABLE_EIGEN

// glProgramUniform2{i|ui|f|d}
void BaseUniform::update_value(GLsizei cnt, Eigen::Vector2i *data) const
{
  glProgramUniform2iv(mProg->id(), mLocation, cnt, data->data());
}

void BaseUniform::update_value(GLsizei cnt, Eigen::Vector2f *data) const
{
  glProgramUniform2fv(mProg->id(), mLocation, cnt, data->data());
}

void BaseUniform::update_value(GLsizei cnt, Eigen::Vector2d *data) const
{
  glProgramUniform2dv(mProg->id(), mLocation, cnt, data->data());
}

// glProgramUniform3{i|ui|f|d}
void BaseUniform::update_value(GLsizei cnt, Eigen::Vector3i *data) const
{
  glProgramUniform3iv(mProg->id(), mLocation, cnt, data->data());
}

void BaseUniform::update_value(GLsizei cnt, Eigen::Vector3f *data) const
{
  glProgramUniform3fv(mProg->id(), mLocation, cnt, data->data());
}

void BaseUniform::update_value(GLsizei cnt, Eigen::Vector3d *data) const
{
  glProgramUniform3dv(mProg->id(), mLocation, cnt, data->data());
}

// glProgramUniform4{i|ui|f|d}
void BaseUniform::update_value(GLsizei cnt, Eigen::Vector4i *data) const
{
  glProgramUniform4iv(mProg->id(), mLocation, cnt, data->data());
}

void BaseUniform::update_value(GLsizei cnt, Eigen::Vector4f *data) const
{
  glProgramUniform4fv(mProg->id(), mLocation, cnt, data->data());
}

void BaseUniform::update_value(GLsizei cnt, Eigen::Vector4d *data) const
{
  glProgramUniform4dv(mProg->id(), mLocation, cnt, data->data());
}

void BaseUniform::update_value(GLsizei cnt, Eigen::Matrix2f *data) const
{
  glProgramUniformMatrix2fv(mProg->id(), mLocation, cnt, GL_FALSE, data->data());
}

void BaseUniform::update_value(GLsizei cnt, Eigen::Matrix2d *data) const
{
  glProgramUniformMatrix2dv(mProg->id(), mLocation, cnt, GL_FALSE, data->data());
}

void BaseUniform::update_value(GLsizei cnt, Eigen::Matrix3f *data) const
{
  glProgramUniformMatrix3fv(mProg->id(), mLocation, cnt, GL_FALSE, data->data());
}

void BaseUniform::update_value(GLsizei cnt, Eigen::Matrix3d *data) const
{
  glProgramUniformMatrix3dv(mProg->id(), mLocation, cnt, GL_FALSE, data->data());
}

void BaseUniform::update_value(GLsizei cnt, Eigen::Matrix4f *data) const
{
  glProgramUniformMatrix4fv(mProg->id(), mLocation, cnt, GL_FALSE, data->data());
}

void BaseUniform::update_value(GLsizei cnt, Eigen::Matrix4d *data) const
{
  glProgramUniformMatrix4dv(mProg->id(), mLocation, cnt, GL_FALSE, data->data());
}

void BaseUniform::update_value(GLsizei cnt, Eigen::Matrix<float, 3, 2> *data) const
{
  glProgramUniformMatrix2x3fv(mProg->id(), mLocation, cnt, GL_FALSE, data->data());
}

void BaseUniform::update_value(GLsizei cnt, Eigen::Matrix<double, 3, 2> *data) const
{
  glProgramUniformMatrix2x3dv(mProg->id(), mLocation, cnt, GL_FALSE, data->data());
}

void BaseUniform::update_value(GLsizei cnt, Eigen::Matrix<float, 2, 3> *data) const
{
  glProgramUniformMatrix3x2fv(mProg->id(), mLocation, cnt, GL_FALSE, data->data());
}

void BaseUniform::update_value(GLsizei cnt, Eigen::Matrix<double, 2, 3> *data) const
{
  glProgramUniformMatrix3x2dv(mProg->id(), mLocation, cnt, GL_FALSE, data->data());
}

void BaseUniform::update_value(GLsizei cnt, Eigen::Matrix<float, 4, 2> *data) const
{
  glProgramUniformMatrix2x4fv(mProg->id(), mLocation, cnt, GL_FALSE, data->data());
}

void BaseUniform::update_value(GLsizei cnt, Eigen::Matrix<double, 4, 2> *data) const
{
  glProgramUniformMatrix2x4dv(mProg->id(), mLocation, cnt, GL_FALSE, data->data());
}

void BaseUniform::update_value(GLsizei cnt, Eigen::Matrix<float, 2, 4> *data) const
{
  glProgramUniformMatrix4x2fv(mProg->id(), mLocation, cnt, GL_FALSE, data->data());
}

void BaseUniform::update_value(GLsizei cnt, Eigen::Matrix<double, 2, 4> *data) const
{
  glProgramUniformMatrix4x2dv(mProg->id(), mLocation, cnt, GL_FALSE, data->data());
}

void BaseUniform::update_value(GLsizei cnt, Eigen::Matrix<float, 4, 3> *data) const
{
  glProgramUniformMatrix3x4fv(mProg->id(), mLocation, cnt, GL_FALSE, data->data());
}

void BaseUniform::update_value(GLsizei cnt, Eigen::Matrix<double, 4, 3> *data) const
{
  glProgramUniformMatrix3x4dv(mProg->id(), mLocation, cnt, GL_FALSE, data->data());
}

void BaseUniform::update_value(GLsizei cnt, Eigen::Matrix<float, 3, 4> *data) const
{
  glProgramUniformMatrix4x3fv(mProg->id(), mLocation, cnt, GL_FALSE, data->data());
}

void BaseUniform::update_value(GLsizei cnt, Eigen::Matrix<double, 3, 4> *data) const
{
  glProgramUniformMatrix4x3dv(mProg->id(), mLocation, cnt, GL_FALSE, data->data());
}
#endif