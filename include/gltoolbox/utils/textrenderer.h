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

#ifndef __GLTOOLBOX_TEXTRENDERER_H__
#define __GLTOOLBOX_TEXTRENDERER_H__

#include <map>
#include <unordered_map>
#include <vector>

#include <gltoolbox/program.h>
#include <gltoolbox/texture.h>
#include <gltoolbox/vertexarray.h>

namespace gltoolbox
{
  class TextRenderer
  {
  private:
    struct Character
    {
      uint32_t width, height;
      int32_t bearingX, bearingY;
      int32_t advance;
    };

    struct Font
    {
      Texture atlas;
      std::map<char, Character> characterlist;

      Font() : atlas(GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR) {} //default constructor
    };

  public:
    TextRenderer(bool doInit = false);
    virtual ~TextRenderer();

    inline void set_font_size(float size) { mCurrSize = size; }
    inline void set_font_color(const std::array<float, 3> &rgb) { mCurrRGB = rgb; }
    inline void set_font(const std::string &fontname)
    {
      if (mFonts.find(fontname) != mFonts.end())
        mCurrFont = fontname;
    }

    inline void draw(const std::string &text, float x, float y)
    {
      draw(text, x, y, mCurrFont, mCurrSize, mCurrRGB);
    }

    void draw(const std::string &text, float x, float y, const std::string &fontname, const float &size, const std::array<float, 3> &color);

    //load font data from .ttf file, sets loaded font to current
    bool load_font(const std::string &filename, unsigned int size = 48);

  protected:
    void init();

  protected:
    //current rendering
    float mCurrSize;
    std::array<float, 3> mCurrRGB;
    std::string mCurrFont;

    //font database
    std::unordered_map<std::string, Font> mFonts;

    //geometry
    std::vector<float> mTQuad;
    std::array<float, 8> mVQuad;
    std::array<uint8_t, 6> mIQuad;

    //rendering
    bool mIsInit;
    mutable Program mPrg;
    mutable VertexArray mVao;
  };
}

#endif