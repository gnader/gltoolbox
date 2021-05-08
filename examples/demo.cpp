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
#include <gltoolbox/shapes.h>
#include <GLFW/glfw3.h>

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

  GLFWwindow *window = glfwCreateWindow(640, 640, "gltoolbox demo", nullptr, nullptr);
  if (window == nullptr)
    return 1;

  // glfwHideWindow(window);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  gltoolbox::GL::initilize(glfwGetProcAddress);

  std::cout << "OpenGL version: " << gltoolbox::GL::gl_version() << std::endl;
  std::cout << "GLSL version: " << gltoolbox::GL::glsl_version() << std::endl;

  int width, height;

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glfwMakeContextCurrent(window);
    glbinding::Binding::useCurrentContext();

    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1.f, 1.f, 1.f, 1.f);

    gltoolbox::Shapes::color(0.7, 0.3, 0.1);
    gltoolbox::Shapes::draw_quad(0, 00, 100, 100);
    gltoolbox::Shapes::draw_quad(200, 200, 100, 100);
    gltoolbox::Shapes::draw_quad(400, 400, 100, 100);

    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}