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

std::unique_ptr<Shape2D::PolygonRenderer> Shape2D::mPoly;

bool Shape2D::mIsInit = false;

void Shape2D::color(float r, float g, float b, float a)
{
  if (!mIsInit)
    init();
  mPoly->color(r, g, b, a);
}

void Shape2D::zvalue(float z)
{
  if (!mIsInit)
    init();
  mPoly->zvalue(z);
}

void Shape2D::draw_ngon(int n, float x, float y, float w, float h, float theta)
{
  if (!mIsInit)
    init();

  mPoly->render(n, x, y, w, h, theta);
}

void Shape2D::draw_quad(float x, float y, float w, float h, float theta)
{
  //rotate the 4-gon by 45deg
  //scale by 2/sqrt(2) and adjust position accordingly to account for rotation
  draw_ngon(4, x - w * 0.5 * 0.41421356237, y - h * 0.5 * 0.41421356237, w * 1.41421356237, h * 1.41421356237, theta - M_PI_4);
}

void Shape2D::draw_line(float xa, float ya, float xb, float yb, float thicknes)
{
  float angle = std::atan2(yb - ya, xb - xa);
  draw_quad(xa, ya - 0.5 * thicknes, xb - xa, yb + 0.5 * thicknes - ya, angle);
}

void Shape2D::init()
{
  mPoly.reset(new PolygonRenderer());
  mPoly->init(360);
  mIsInit = true;
}

Shape2D::PolygonRenderer::PolygonRenderer()
{
  mColor = {0.f, 0.f, 0.f, 1.f};
  mZindex = 0;
}

Shape2D::PolygonRenderer::~PolygonRenderer()
{
}

void Shape2D::PolygonRenderer::color(float r, float g, float b, float a)
{
  mColor[0] = r;
  mColor[1] = g;
  mColor[2] = b;
  mColor[3] = a;
}

void Shape2D::PolygonRenderer::zvalue(float z)
{
  mZindex = z;
}

void Shape2D::PolygonRenderer::init(int npts)
{
  mNumSamples = npts;
  mSides = npts;

  std::string vert = "#version 450 core \n"
                     "uniform vec2 scale; \n"
                     "uniform vec2 pos; \n"
                     "uniform float theta; \n"
                     "uniform float zindex; \n"
                     "in vec2 vert; \n"
                     "void main(void) {\n"
                     "  vec2 coord; \n"
                     "  coord.x = (scale.x*vert.x*cos(theta) - scale.y*vert.y*sin(theta));\n"
                     "  coord.y = (scale.x*vert.x*sin(theta) + scale.y*vert.y*cos(theta));\n"
                     "  coord.x = coord.x - pos.x; \n"
                     "  coord.y = coord.y + pos.y; \n"
                     "  gl_Position = vec4(coord.x, coord.y, zindex, 1.0); \n"
                     "}";

  std::string frag = "#version 450 core \n"
                     "out vec4 colour; \n"
                     "uniform vec4 rgba; \n"
                     "void main(void) {\n"
                     " colour = rgba; \n"
                     "}";

  //setup program
  mPrg.attach_shader(std::move(gltoolbox::Shader(vert, GL_VERTEX_SHADER)));
  mPrg.attach_shader(std::move(gltoolbox::Shader(frag, GL_FRAGMENT_SHADER)));
  mPrg.link();

  mPrg.add_attribute("vert");

  mPrg.add_uniform<std::array<float, 2>>("scale", nullptr);
  mPrg.add_uniform<std::array<float, 2>>("pos", nullptr);
  mPrg.add_uniform<float>("theta", nullptr);
  mPrg.add_uniform<float>("zindex", &mZindex);

  mPrg.add_uniform<std::array<float, 4>>("rgba", &mColor);

  //init coord vector
  mCoords.resize((npts + 1) * 2);
  float dtheta = 2 * M_PI / float(npts);
  mCoords[0] = 0;
  mCoords[1] = 0;
  for (int i = 0; i < npts; ++i)
  {
    mCoords[2 * (i + 1) + 0] = sin(i * dtheta);
    mCoords[2 * (i + 1) + 1] = cos(i * dtheta);
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
  mVao.add_attribute<float>("vert", mCoords.data(), mCoords.size(), 2, GL_FLOAT, 0, 0, GL_STATIC_DRAW);
}

void Shape2D::PolygonRenderer::render(int n, float x, float y, float w, float h, float theta)
{
  //if the number of sides changed
  //compute new index buffer
  n = std::min(n, mNumSamples);
  if (mSides != n)
  {
    mSides = n;
    update_indices();
  }

  GLint vp[4];
  GL::get_viewport(vp);

  //compute viewport aspect ratio
  float W = float(vp[2] - vp[0]);
  float H = float(vp[3] - vp[1]);

  //orientation
  mPrg.update_uniform("theta", &theta);

  //compute object scale
  std::array<float, 2> scale;
  scale[0] = w / W;
  scale[1] = h / H;
  mPrg.update_uniform("scale", &scale);

  //compute object position
  std::array<float, 2> pos;
  pos[0] = (W - (w + 2.f * x)) / W;
  pos[1] = (H - (h + 2.f * y)) / H;
  mPrg.update_uniform("pos", &pos);

  //do the rendering
  mPrg.use();
  mPrg.enable_uniforms();
  mVao.bind();
  mVao.enable_attributes(mPrg.attributes());
  mVao.draw_elements(0, 3 * mSides);
  mVao.disable_attributes(mPrg.attributes());
  mVao.unbind();
  mPrg.unuse();
}

void Shape2D::PolygonRenderer::update_indices()
{
  int offset = mNumSamples / mSides;

  for (int i = 0; i < mSides; ++i)
  {
    mIndices[3 * i + 0] = 0;
    mIndices[3 * i + 1] = i * offset + 1;
    mIndices[3 * i + 2] = (i + 1) * offset + 1;
  }
  mIndices[3 * (mSides - 1) + 2] = 1;

  mVao.index_buffer().lock()->update(0, 3 * mSides);
}