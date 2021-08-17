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

TextRenderer::TextRenderer()
{
}

TextRenderer::~TextRenderer()
{
}

bool TextRenderer::load_font(const std::string &filename, unsigned int size)
{
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

  //load font data
  for (unsigned char c = 0; c < 255; ++c)
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
                     face->glyph->advance.x};

    mFonts[name].characterlist.insert(std::pair<char, Character>(c, chr));
  }

  //clean up
  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  return true;
}
