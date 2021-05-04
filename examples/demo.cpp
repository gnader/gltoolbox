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
#include <GLFW/glfw3.h>

std::string vert = "#version 450 core \n in vec2 vtx_pos; \n void main(void) { gl_Position = vec4(vtx_pos.x, vtx_pos.y, 0., 1.0); }";
std::string frag = "#version 450 core \n uniform float grey; \n out vec4 colour; \n void main(void) { colour = vec4(grey, 0.0, 0.0, 1.0); }";

static void glfw_error_callback(int error, const char *description)
{
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int argc, char **argv)
{
  std::vector<float> pos = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
  std::vector<unsigned int> indices = {0, 3, 2, 0, 2, 1};
  float grey = 0.8;

  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
    return 1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

  GLFWwindow *window = glfwCreateWindow(640, 480, "gltoolbox demo", nullptr, nullptr);
  if (window == nullptr)
    return 1;

  // glfwHideWindow(window);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  gltoolbox::GL::initilize(glfwGetProcAddress);

  std::cout << "OpenGL version: " << gltoolbox::GL::gl_version() << std::endl;
  std::cout << "GLSL version: " << gltoolbox::GL::glsl_version() << std::endl;

  gltoolbox::Program prg;
  prg.attach_shader(std::move(gltoolbox::Shader(vert, GL_VERTEX_SHADER)));
  prg.attach_shader(std::move(gltoolbox::Shader(frag, GL_FRAGMENT_SHADER)));
  prg.link();

  prg.add_uniform<float>("grey", &grey);
  prg.add_attribute("vtx_pos");

  gltoolbox::VertexArray vao;
  vao.set_index_buffer<unsigned int>(GL_TRIANGLES, indices.data(), indices.size(), GL_STATIC_DRAW);
  vao.add_attribute<float>("vtx_pos", pos.data(), pos.size(), 2, GL_FLOAT, 0, 0, GL_STATIC_DRAW);

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

    prg.use();

    prg.enable_uniform("grey");

    vao.bind();
    vao.enable_attribute(prg.attributes());
    vao.drawElements();
    vao.disable_attribute(prg.attributes());
    vao.unbind();

    prg.unuse();

    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}