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

#include <GLFW/glfw3.h>
#include <gltoolbox/gltoolbox.h>

#define TYPE int

int main(int argc, char **argv)
{
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

  gltoolbox::GL::initilize(glfwGetProcAddress);

  std::cout << "OpenGL version: " << gltoolbox::GL::gl_version() << std::endl;
  std::cout << "GLSL version: " << gltoolbox::GL::glsl_version() << std::endl;

  gltoolbox::Program prg;
  prg.attach_shader(std::move(gltoolbox::Shader::from_file("blinn.vert", GL_VERTEX_SHADER)));
  prg.attach_shader(std::move(gltoolbox::Shader::from_file("blinn.frag", GL_FRAGMENT_SHADER)));
  prg.link();

  float u1 = 12.;
  prg.add_uniform<float>("picking_radius", &u1);
  prg.update_uniform();

  auto display_vector = [](const std::vector<float> &vec) {
    for (auto x : vec)
      std::cout << x << " " << std::flush;
    std::cout << std::endl;
  };

  std::vector<float> data{1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f};
  std::vector<unsigned int> index{0, 1, 2, 3, 4, 5, 6};

  gltoolbox::VertexArray vao;
  vao.set_index_buffer<unsigned int>(GL_POINTS, index.data(), index.size());
  vao.add_attribute<float>("points", data.data(), data.size(), 1, GL_FLOAT, 0, 0, GL_STATIC_DRAW);

  {
    auto wp = vao.attribute_buffer("points").lock();

    std::cout << wp->memory_size() << std::endl;

    display_vector(data);
    data[4] = 40;
    display_vector(data);
    wp->get();
    display_vector(data);
  }

  std::cout << "done." << std::endl;

  return 0;
}