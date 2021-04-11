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

#include <iostream>
using namespace std;

#include <GLFW/glfw3.h>
#include <gltoolbox/gltoolbox.h>

void error(int errnum, const char *errmsg)
{
  std::cerr << errnum << ": " << errmsg << std::endl;
}

int main(int argc, char **argv)
{
  glfwSetErrorCallback(error);

  if (!glfwInit())
    return 1;

  glfwDefaultWindowHints();
  glfwWindowHint(GLFW_VISIBLE, false);

  auto window = glfwCreateWindow(320, 240, "", nullptr, nullptr);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }
  glfwHideWindow(window);

  glfwMakeContextCurrent(window);

  glbinding::Binding::initialize(glfwGetProcAddress, true);

  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

  return 0;
}