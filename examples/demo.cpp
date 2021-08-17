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

  gltoolbox::TextRenderer txt_renderer;
  txt_renderer.load_font("./Arial_italic.ttf");

  // gltoolbox::FrameBuffer fbo(GL_FRAMEBUFFER);
  // fbo.attach(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 1000, 1000, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);

  // std::cout << fbo.status_as_string() << std::endl;

  std::vector<uint8_t> data;
  data.resize(1000 * 1000 * 3);

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glfwMakeContextCurrent(window);
    glbinding::Binding::useCurrentContext();

    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // std::cout << "rendering into framebuffer" << std::endl;

    // fbo.bind();
    gltoolbox::GL::clear(GL_COLOR_BUFFER_BIT);
    gltoolbox::GL::clear_color(255.f, 1.f);

    gltoolbox::Shape2D::color(0.8, 0.1, 0.1);
    gltoolbox::Shape2D::draw_quad(0, 0, 600, 200);

    gltoolbox::Shape2D::color(0.1, 0.8, 0.1);
    gltoolbox::Shape2D::draw_quad(200, 200, 400, 600);
    // fbo.unbind();

    // fbo.texture(GL_COLOR_ATTACHMENT0)->download(data.data());

    // stbi_flip_vertically_on_write(1);
    // stbi_write_png("test.png", 1000, 1000, 3, data.data(), 0);

    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}