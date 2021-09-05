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

#ifndef __GLTOOLBOX_UNIFORM_H__
#define __GLTOOLBOX_UNIFORM_H__

#include "gl.h"

#include <array>

#ifdef GLTOOLBOX_ENABLE_EIGEN
#include <Eigen/Dense>
#endif

namespace gltoolbox
{
  class Program; //forward declaration of program class

  class BaseUniform
  {
  public:
    BaseUniform(Program *prog, GLint location);
    BaseUniform(Program *prog, const std::string &name);
    BaseUniform(Program *prog, GLint location, const std::string &name);

    virtual ~BaseUniform();

    inline GLint location() const { return mLocation; }
    inline const std::string &name() const { return mName; }

    virtual bool is_attached() const = 0;
    virtual void update() const = 0;

  protected:
    // glProgramUniform1{i|ui|f|d}
    void update_value(GLsizei cnt, int *data) const;
    void update_value(GLsizei cnt, unsigned int *data) const;
    void update_value(GLsizei cnt, float *data) const;
    void update_value(GLsizei cnt, double *data) const;

    // glProgramUniform2{i|ui|f|d}
    void update_value(GLsizei cnt, std::array<int, 2> *data) const;
    void update_value(GLsizei cnt, std::array<unsigned int, 2> *data) const;
    void update_value(GLsizei cnt, std::array<float, 2> *data) const;
    void update_value(GLsizei cnt, std::array<double, 2> *data) const;

    // glProgramUniform3{i|ui|f|d}
    void update_value(GLsizei cnt, std::array<int, 3> *data) const;
    void update_value(GLsizei cnt, std::array<unsigned int, 3> *data) const;
    void update_value(GLsizei cnt, std::array<float, 3> *data) const;
    void update_value(GLsizei cnt, std::array<double, 3> *data) const;

    // glProgramUniform4{i|ui|f|d}
    void update_value(GLsizei cnt, std::array<int, 4> *data) const;
    void update_value(GLsizei cnt, std::array<unsigned int, 4> *data) const;
    void update_value(GLsizei cnt, std::array<float, 4> *data) const;
    void update_value(GLsizei cnt, std::array<double, 4> *data) const;

#ifdef GLTOOLBOX_ENABLE_EIGEN
    // glProgramUniform2{i|ui|f|d}
    void update_value(GLsizei cnt, Eigen::Vector2i *data) const;
    void update_value(GLsizei cnt, Eigen::Vector2f *data) const;
    void update_value(GLsizei cnt, Eigen::Vector2d *data) const;

    // glProgramUniform3{i|ui|f|d}
    void update_value(GLsizei cnt, Eigen::Vector3i *data) const;
    void update_value(GLsizei cnt, Eigen::Vector3f *data) const;
    void update_value(GLsizei cnt, Eigen::Vector3d *data) const;

    // glProgramUniform4{i|ui|f|d}
    void update_value(GLsizei cnt, Eigen::Vector4i *data) const;
    void update_value(GLsizei cnt, Eigen::Vector4f *data) const;
    void update_value(GLsizei cnt, Eigen::Vector4d *data) const;

    // glProgramUniformMatrix2{f|d}
    void update_value(GLsizei cnt, Eigen::Matrix2f *data) const;
    void update_value(GLsizei cnt, Eigen::Matrix2d *data) const;

    // glProgramUniformMatrix3{f|d}
    void update_value(GLsizei cnt, Eigen::Matrix3f *data) const;
    void update_value(GLsizei cnt, Eigen::Matrix3d *data) const;

    // glProgramUniformMatrix4{f|d}
    void update_value(GLsizei cnt, Eigen::Matrix4f *data) const;
    void update_value(GLsizei cnt, Eigen::Matrix4d *data) const;

    // glProgramUniformMatrix{2x3}{f|d}
    void update_value(GLsizei cnt, Eigen::Matrix<float, 3, 2> *data) const;
    void update_value(GLsizei cnt, Eigen::Matrix<double, 3, 2> *data) const;

    // glProgramUniformMatrix{2x3}{f|d}
    void update_value(GLsizei cnt, Eigen::Matrix<float, 2, 3> *data) const;
    void update_value(GLsizei cnt, Eigen::Matrix<double, 2, 3> *data) const;

    // glProgramUniformMatrix{2x3}{f|d}
    void update_value(GLsizei cnt, Eigen::Matrix<float, 4, 2> *data) const;
    void update_value(GLsizei cnt, Eigen::Matrix<double, 4, 2> *data) const;

    // glProgramUniformMatrix{2x3}{f|d}
    void update_value(GLsizei cnt, Eigen::Matrix<float, 2, 4> *data) const;
    void update_value(GLsizei cnt, Eigen::Matrix<double, 2, 4> *data) const;

    // glProgramUniformMatrix{2x3}{f|d}
    void update_value(GLsizei cnt, Eigen::Matrix<float, 4, 3> *data) const;
    void update_value(GLsizei cnt, Eigen::Matrix<double, 4, 3> *data) const;

    // glProgramUniformMatrix{2x3}{f|d}
    void update_value(GLsizei cnt, Eigen::Matrix<float, 3, 4> *data) const;
    void update_value(GLsizei cnt, Eigen::Matrix<double, 3, 4> *data) const;
#endif

  protected:
    Program *mProg;

    GLint mLocation;
    std::string mName;
  };

  template <typename T>
  class Uniform : public BaseUniform
  {
  public:
    Uniform(Program *prog, GLint location, T *data = nullptr, GLsizei count = 1)
        : BaseUniform(prog, location), mPtr(data), mCount(count)
    {
      mIsAttached = (data == nullptr);
    }

    Uniform(Program *prog, const std::string &name, T *data = nullptr, GLsizei count = 1)
        : BaseUniform(prog, name), mPtr(data), mCount(count)
    {
      mIsAttached = (data == nullptr);
    }

    Uniform(Program *prog, GLint location, const std::string &name, T *data = nullptr, GLsizei count = 1)
        : BaseUniform(prog, location, name), mPtr(data), mCount(count)
    {
      mIsAttached = (data == nullptr);
    }

    virtual ~Uniform()
    {
      detach();
    }

    virtual bool is_attached() const
    {
      return mIsAttached;
    }

    void attach_to(T *data, GLsizei count = 1)
    {
      mPtr = data; // mPtr is not deleted, uniform class does not own mPtr
      mCount = count;
      mIsAttached = true;
    }

    void detach()
    {
      mPtr = nullptr;
      mCount = -1;
      mIsAttached = false;
    }

    virtual void update() const
    {
      if (is_attached())
        update_value(mCount, mPtr);
    }

    void update(T *data, GLsizei count = 1)
    {
      update_value(count, data);
    }

  protected:
    // ! uniform does not have ownership of the pointer.
    // ! mPtr will be dangling if it get deleted or gets out of scope
    T *mPtr;
    GLsizei mCount;
    bool mIsAttached;
  };
}

#endif