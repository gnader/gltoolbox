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

#include <gltoolbox/gltoolbox.h>
#include <gltoolbox/utils/textrenderer.h>
#include <GLFW/glfw3.h>

#include "shapes.h"

// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include "stb_image_write.h"

static void glfw_error_callback(int error, const char *description)
{
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int argc, char **argv)
{
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
    return 1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

  GLFWwindow *window = glfwCreateWindow(1000, 1000, "gltoolbox demo", nullptr, nullptr);
  if (window == nullptr)
    return 1;

  // glfwHideWindow(window);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  gltoolbox::GL::initilize(glfwGetProcAddress);

  std::cout << "OpenGL version: " << gltoolbox::GL::gl_version() << std::endl;
  std::cout << "GLSL version: " << gltoolbox::GL::glsl_version() << std::endl;

  int width, height;

  gltoolbox::TextRenderer txt;
  txt.load_font("./calibri.ttf", 64);
  txt.load_font("./consolas.ttf", 64);
  txt.set_font("Consolas");

  std::vector<uint8_t> data;
  data.resize(1000 * 1000 * 3);

  gltoolbox::GL::enable_blend();
  gltoolbox::GL::set_blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  int n = 0;

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glfwMakeContextCurrent(window);
    glbinding::Binding::useCurrentContext();

    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    gltoolbox::GL::clear(GL_COLOR_BUFFER_BIT);
    gltoolbox::GL::clear_color(255.f, 1.f);

    std::string text = "gltoolbox::TextRenderer()";
    double t0 = glfwGetTime();
    txt.draw(text, 0, 100, 64);
    txt.draw("{ this works }", width / 2, 200, 128);
    double t1 = glfwGetTime();

    double dt = t1 - t0;
    // std::cout << 1. / dt << " fps" << std::endl;

    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}