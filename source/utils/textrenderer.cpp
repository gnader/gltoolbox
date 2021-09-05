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

#define BUFFSIZE 100

// Shaders //-------------------------------------------------------//
const std::string vShader = "#version 450 core \n"
                            "in vec2 vQuad; \n"
                            "in vec4 vTex; \n"
                            "in vec4 vPos; \n"
                            "out vec2 fTex; \n"
                            "uniform vec2 pos; \n"
                            "void main(void) { \n"
                            "  mat2 s1 = mat2(vPos.z, 0 , 0, vPos.w); \n"
                            "  mat2 s2 = mat2(vTex.z, 0 , 0, vTex.w); \n"
                            "  fTex = s2 * vQuad + vTex.xy; \n"
                            "  vec3 position = vec3(s1 * vQuad + vPos.xy + pos.xy, 0.f); \n"
                            "  gl_Position = vec4(position, 1.0);\n"
                            "}";

const std::string fShader = "#version 450 core \n"
                            "in vec2 fTex; \n"
                            "out vec4 colour; \n"
                            "uniform vec3 rgb; \n"
                            "uniform sampler2D atlas; \n"
                            "void main(void) { \n"
                            "  float alpha = texture(atlas, fTex).r; \n"
                            "  colour = vec4(rgb, alpha); \n"
                            "}";
;
//------------------------------------------------------------------//

std::string TextRenderer::charlist = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
int TextRenderer::padding = 4;

TextRenderer::TextRenderer(bool doInit)
    : mCurrSize(1.f), mCurrRGB{0.f, 0.f, 0.f}, mCurrFont(""), mIsInit(false), mAtlas(GL_TEXTURE_2D)
{
  mTQuad.resize(4 * BUFFSIZE, 0.f);
  mTexQuad.resize(4 * BUFFSIZE, 0.f);
  mVQuad = {0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f};
  mIQuad = {0, 1, 2, 0, 2, 3};

  if (doInit)
    init();
}

TextRenderer::~TextRenderer()
{
}

void TextRenderer::draw(const std::string &text, float x, float y,
                        const std::string &fontname, const float &size, const std::array<float, 3> &rgb)
{
  if (mFonts.find(fontname) == mFonts.end())
    return;

  set_font(fontname);
  mCurrSize = size;
  mCurrRGB = rgb;

  const Font &font = mFonts[mCurrFont];

  if (!mIsInit)
    init();

  GLint vp[4];
  GL::get_viewport(vp);
  float scaleX = mCurrSize / (64 * float(vp[2] - vp[0]));
  float scaleY = mCurrSize / (64 * float(vp[3] - vp[1]));
  float scaleT = 1.f / float(font.atlasres);
  float advance = 0;

  std::array<float, 2> pos{2.f * x / float(vp[2] - vp[0]) - 1.f,
                           1.f - 2.f * y / float(vp[3] - vp[1])};

  mPrg.use();
  //uniforms
  mPrg.enable_uniform("rgb", &mCurrRGB);
  mPrg.enable_uniform("pos", &pos);
  //texture
  mAtlas.bind();
  mPrg.enable_samplers();
  //attributes
  mVao.bind();
  mVao.enable_attributes(mPrg.attributes());

  int i = 0;
  while (i < text.size())
  {
    for (int j = 0; j < BUFFSIZE; ++j)
    {
      auto it = font.characterlist.find(text.at(i));
      if (it == font.characterlist.end())
        it--;

      //compute vertex and texture coords
      const Character &c = it->second;
      mTQuad[4 * j + 0] = advance + float(c.bearingX) * scaleX;
      mTQuad[4 * j + 1] = (-float(c.height) + float(c.bearingY)) * scaleY;
      mTQuad[4 * j + 2] = float(c.width) * scaleX;
      mTQuad[4 * j + 3] = float(c.height) * scaleY;

      mTexQuad[4 * j + 0] = (c.texX - 1) * scaleT;
      mTexQuad[4 * j + 1] = (c.texY - 1) * scaleT;
      mTexQuad[4 * j + 2] = (c.width + 2) * scaleT;
      mTexQuad[4 * j + 3] = (c.height + 2) * scaleT;

      advance += (float(c.advance >> 6) - 2) * scaleX;

      // std::cout << mTQuad[4 * j + 2] << ", " << mTQuad[4 * j + 3] << std::endl;

      ++i;
      if (i >= text.size())
        break;
    }

    //upload data to gpu
    mVao.attribute_buffer("vPos")->upload(mTQuad.data(), mTQuad.size());
    mVao.attribute_buffer("vTex")->upload(mTexQuad.data(), mTexQuad.size());

    //draw
    mVao.draw_elements(text.size());
  }

  //cleanup
  mVao.disable_attributes(mPrg.attributes());
  mVao.unbind();
  mAtlas.unbind();
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
  int res = (size + padding) * int(sqrt(charlist.length()) + 1);
  res = 1 << (int(log2(res)) + 1);

  mFonts[name].atlas.resize(res * res);
  mFonts[name].atlasres = res;

  int32_t texX = padding / 2;
  int32_t texY = padding / 2;
  unsigned int nY = 0;
  for (auto c : charlist)
  {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    {
      std::cerr << "[TextRenderer::loadfont()] failed to load char " << c << std::endl;
      continue;
    }

    FT_GlyphSlot glyph = face->glyph;

    //compute texture coordinates
    if (texX + glyph->bitmap.width > res)
    {
      texY += nY + padding;
      texX = padding / 2;
      nY = 0;
    }
    nY = std::max(nY, glyph->bitmap.rows);

    //create character
    Character chr = {glyph->bitmap.width, glyph->bitmap.rows,
                     glyph->bitmap_left, glyph->bitmap_top,
                     glyph->metrics.horiAdvance,
                     texX, texY};
    mFonts[name].characterlist.insert(std::pair<char, Character>(c, chr));

    //copy bitmap data
    for (int32_t j = 0; j < chr.height; ++j)
      for (int32_t i = 0; i < chr.width; ++i)
        mFonts[name].atlas[(texY + j) * res + texX + i] = glyph->bitmap.buffer[(chr.height - 1 - j) * chr.width + i];

    //increment texture
    texX += chr.width + padding;
  }

  if (mCurrFont.empty())
    set_font(name);

  //clean up
  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  return true;
}

void TextRenderer::init()
{
  //setup texture
  mAtlas.set_texture_options(GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

  //setup program
  mPrg.attach_shader(std::move(gltoolbox::Shader(vShader, GL_VERTEX_SHADER)));
  mPrg.attach_shader(std::move(gltoolbox::Shader(fShader, GL_FRAGMENT_SHADER)));
  mPrg.link();

  //add uniforms and inputs
  mPrg.add_attribute("vQuad");
  mPrg.add_attribute("vPos");
  mPrg.add_attribute("vTex");
  mPrg.add_sampler("atlas", 0);
  mPrg.add_uniform<std::array<float, 3>>("rgb");
  mPrg.add_uniform<std::array<float, 2>>("pos");

  //setup buffers
  mVao.set_index_buffer<uint8_t>(GL_TRIANGLES, mIQuad.data(), mIQuad.size(), GL_STATIC_DRAW);
  mVao.add_attribute<float>("vQuad", mVQuad.data(), mVQuad.size(), 2, GL_FLOAT, 0, 0, GL_STATIC_DRAW);
  mVao.add_attribute<float>("vPos", mTQuad.data(), mTQuad.size(), 4, GL_FLOAT, 0, 0, GL_DYNAMIC_DRAW, GL_FALSE, 1);
  mVao.add_attribute<float>("vTex", mTexQuad.data(), mTexQuad.size(), 4, GL_FLOAT, 0, 0, GL_DYNAMIC_DRAW, GL_FALSE, 1);

  mIsInit = true;
}
