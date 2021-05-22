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

#ifndef __GLTOOLBOX_SHAPES_H__
#define __GLTOOLBOX_SHAPES_H_

#include "program.h"
#include "vertexarray.h"

namespace gltoolbox
{
  class Shape2D
  {
  public:
    Shape2D() = delete; //is a pure static function

    static void color(float r, float g, float b, float a = 1.f);
    static void zvalue(float z);

    static void draw_ngon(int n, float x, float y, float w, float h, float theta = 0.f);

    static void draw_quad(float x, float y, float w, float h, float theta = 0.f);
    // static void draw_line(float xa, float ya, float xb, float yb, float thickness);

  public:
    static void init();

  protected:
    class PolygonRenderer
    {
    public:
      PolygonRenderer();
      virtual ~PolygonRenderer();

      void color(float r, float g, float b, float a = 1.f);
      void zvalue(float z);

      void init(int npts);
      void render(int n, float x, float y, float w, float h, float theta1 = 0.f, float theta2 = 0.f);

    protected:
      void update_vertices();

    protected:
      int mNumSamples;
      int mSides;

      std::array<float, 4> mColor;

      float mZindex;
      std::vector<float> mCoords;
      std::vector<unsigned short> mIndices;

      Program mPrg;
      VertexArray mVao;
    };

  protected:
    static std::unique_ptr<PolygonRenderer> mPoly;
    static bool mIsInit;
  };

} // namespace gltoolbox

#endif