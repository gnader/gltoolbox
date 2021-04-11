project(GLToolBox)

cmake_minimum_required(VERSION 3.2.0)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -stdlib=libc++")
elseif(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

## DEPENDENCIES
#-------------------------------------------------------------------- 
# glfw
find_package(GLFW REQUIRED)
if(GLFW_FOUND)
  include_directories(${GLFW_INCLUDE_DIR})
  add_definitions(-DGLFW_INCLUDE_NONE)
  message("glfw:")
  message("${GLFW_INCLUDE_DIR}")
  message("${GLFW_LIBRARIES}")
endif()

# glbinding
find_package(GLBINDING REQUIRED)
if(GLBINDING_FOUND)
  include_directories(${GLBINDING_INCLUDE_DIR})
  message("glbinding:")
  message("${GLBINDING_INCLUDE_DIR}")
  message("${GLBINDING_LIBRARIES}")
endif()

## INCLUDES
#--------------------------------------------------------------------
include_directories(${PROJECT_SOURCE_DIR}/source/include)

## FILES
#--------------------------------------------------------------------
set(source
    ${PROJECT_SOURCE_DIR}/source/source/buffer.cpp
    ${PROJECT_SOURCE_DIR}/source/source/program.cpp
    ${PROJECT_SOURCE_DIR}/source/source/shader.cpp)

set(header
    ${PROJECT_SOURCE_DIR}/source/include/gltoolbox/gl.h
    ${PROJECT_SOURCE_DIR}/source/include/gltoolbox/gltoolbox.h
    ${PROJECT_SOURCE_DIR}/source/include/gltoolbox/buffer.h
    ${PROJECT_SOURCE_DIR}/source/include/gltoolbox/program.h
    ${PROJECT_SOURCE_DIR}/source/include/gltoolbox/shader.h)

## GLTOOLBOX
#--------------------------------------------------------------------
add_library(gltoolbox ${source} ${header})

## EXAMPLES
#--------------------------------------------------------------------
add_executable(gl examples/gl.cpp)
target_link_libraries(gl gltoolbox ${GLBINDING_LIBRARIES} ${GLFW_LIBRARIES})