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

#include <gltoolbox/shapes.h>

using namespace gltoolbox;

std::unique_ptr<Shapes::PolygonRenderer> Shapes::mPoly;

bool Shapes::mIsInit = false;

void Shapes::set_color(float r, float g, float b)
{
  if (!mIsInit)
    init();

  mPoly->color(r, g, b);
}

void Shapes::draw_ngon(int n, float x)
{
  if (!mIsInit)
    init();

  mPoly->render();
}

void Shapes::init()
{
  mPoly = std::unique_ptr<PolygonRenderer>(new PolygonRenderer());
  mPoly->init(360);
  mIsInit = true;
}

Shapes::PolygonRenderer::PolygonRenderer()
{
  mColor = {0.f, 0.f, 0.f};
}

Shapes::PolygonRenderer::~PolygonRenderer()
{
}

void Shapes::PolygonRenderer::color(float r, float g, float b)
{
  mColor[0] = r;
  mColor[1] = g;
  mColor[2] = b;
}

void Shapes::PolygonRenderer::init(int npts)
{
  std::string vert = "#version 450 core \n in vec2 vtx_pos; \n void main(void) { gl_Position = vec4(vtx_pos.x, vtx_pos.y, 0., 1.0); }";
  std::string frag = "#version 450 core \n uniform vec3 rgb; \n out vec4 colour; \n void main(void) { colour = vec4(rgb.r, rgb.g, rgb.b, 1.0); }";

  //setup program
  mPrg.attach_shader(std::move(gltoolbox::Shader(vert, GL_VERTEX_SHADER)));
  mPrg.attach_shader(std::move(gltoolbox::Shader(frag, GL_FRAGMENT_SHADER)));
  mPrg.link();

  mPrg.add_attribute("vtx_pos");
  mPrg.add_uniform<std::array<float, 3>>("rgb", &mColor);

  //init coord vector
  mCoords.resize((npts + 1) * 2);
  float dtheta = 2 * M_PI / float(npts);
  for (int i = 0; i < npts; ++i)
  {
    mCoords[2 * (i + 1) + 0] = cos(i * dtheta);
    mCoords[2 * (i + 1) + 1] = sin(i * dtheta);
  }

  //init index vector
  mIndices.resize(npts * 3, 0);
  for (int i = 0; i < npts; ++i)
  {
    mIndices[3 * i + 0] = 0;
    mIndices[3 * i + 1] = i + 1;
    mIndices[3 * i + 2] = i + 2;
  }
  mIndices.back() = 1;

  //setup vao
  mVao.set_index_buffer<unsigned short>(GL_TRIANGLES, mIndices.data(), mIndices.size(), GL_DYNAMIC_DRAW);
  mVao.add_attribute<float>("vtx_pos", mCoords.data(), mCoords.size(), 2, GL_FLOAT, 0, 0, GL_STATIC_DRAW);
}

void Shapes::PolygonRenderer::render()
{
  mPrg.use();

  mPrg.enable_uniform("rgb");

  mVao.bind();
  mVao.enable_attributes(mPrg.attributes());
  mVao.draw_elements();
  mVao.disable_attributes(mPrg.attributes());
  mVao.unbind();

  mPrg.unuse();
}