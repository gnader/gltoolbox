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

#include <gltoolbox/utils/textrenderer.h>
using namespace gltoolbox;

#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H

// Shaders //-------------------------------------------------------//
const std::string vShader = "#version 450 core \n"
                            "in vec2 vert; \n"
                            "uniform vec4 T; \n"
                            "void main(void) { \n"
                            "  mat2 s = mat2(T.z, 0 , 0, T.w); \n"
                            "  vec3 position = vec3(s*vert+T.xy, 0.f); \n"
                            "  gl_Position = vec4(position, 1.0);\n"
                            "}";

const std::string fShader = "#version 450 core \n"
                            "out vec4 colour; \n"
                            "uniform vec3 rgb; \n"
                            "void main(void) { \n"
                            "  colour = vec4(rgb, 1.f); \n"
                            "}";
;
//------------------------------------------------------------------//

TextRenderer::TextRenderer(bool doInit)
    : mCurrSize(3.f), mCurrRGB{0.f, 0.f, 0.f}, mCurrFont(""), mIsInit(false)
{
  mVQuad = {0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f};
  mIQuad = {0, 1, 2, 0, 2, 3};

  if (doInit)
    init();
}

TextRenderer::~TextRenderer()
{
}

void TextRenderer::draw(const std::string &text, float x, float y,
                        const std::string &fontname, const float &size, const std::array<float, 3> &color)
{
  if (mFonts.find(fontname) == mFonts.end())
    return;

  const Font &font = mFonts[fontname];

  if (!mIsInit)
    init();

  GLint vp[4];
  GL::get_viewport(vp);
  float scaleX = size / float(vp[2] - vp[0]);
  float scaleY = size / float(vp[3] - vp[1]);
  float advance = 0;

  mPrg.use();
  for (const char &c : text)
  {
    const Character &glyph = font.characterlist.at(c);

    std::array<float, 4> T = {x + advance + glyph.bearingX * scaleX,
                              y - (glyph.height - glyph.bearingY) * scaleY,
                              glyph.width * scaleX,
                              glyph.height * scaleY};
    std::array<float, 3> rgb = color;

    mPrg.enable_uniform("T", &T);
    mPrg.enable_uniform("rgb", &rgb);

    mVao.bind();
    mVao.enable_attributes(mPrg.attributes());
    mVao.draw_elements();
    mVao.disable_attributes(mPrg.attributes());
    mVao.unbind();

    advance += float(glyph.advance >> 6) * scaleX;
  }
  mPrg.unuse();
}

bool TextRenderer::load_font(const std::string &filename, unsigned int size)
{
  if (!mIsInit)
    init();

  FT_Library ft;
  FT_Face face;

  if (FT_Init_FreeType(&ft))
  {
    std::cerr << "[TextRenderer::loadfont()] Could not initialize FreeType Lib" << std::endl;
    return false;
  }

  if (FT_New_Face(ft, filename.c_str(), 0, &face))
  {
    std::cerr << "[TextRenderer::loadfont()] Could not load font from " << filename << std::endl;
    FT_Done_FreeType(ft); //clean up
    return false;
  }

  FT_Set_Pixel_Sizes(face, 0, size);

  //get font name
  std::string name(face->family_name);
  mFonts.insert({name, Font()});

  //load font characters, only ascii are supported for now
  std::string ascii = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
  for (auto c : ascii)
  {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    {
      std::cerr << "[TextRenderer::loadfont()] failed to load char " << c << std::endl;
      continue;
    }

    Character chr = {face->glyph->bitmap.width,
                     face->glyph->bitmap.rows,
                     face->glyph->bitmap_left,
                     face->glyph->bitmap_top,
                     face->glyph->metrics.horiAdvance};

    mFonts[name].characterlist.insert(std::pair<char, Character>(c, chr));
  }

  mCurrFont = name;

  //clean up
  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  return true;
}

void TextRenderer::init()
{
  //setup program
  mPrg.attach_shader(std::move(gltoolbox::Shader(vShader, GL_VERTEX_SHADER)));
  mPrg.attach_shader(std::move(gltoolbox::Shader(fShader, GL_FRAGMENT_SHADER)));
  mPrg.link();

  //add uniforms and inputs
  mPrg.add_attribute("vert");
  mPrg.add_uniform<std::array<float, 4>>("T");
  mPrg.add_uniform<std::array<float, 3>>("rgb");

  //setup buffers
  mVao.set_index_buffer<uint8_t>(GL_TRIANGLES, mIQuad.data(), mIQuad.size(), GL_STATIC_DRAW);
  mVao.add_attribute<float>("vert", mVQuad.data(), mVQuad.size(), 2, GL_FLOAT, 0, 0, GL_STATIC_DRAW);

  mIsInit = true;
}
