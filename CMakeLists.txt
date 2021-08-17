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

## OPTIONS
#--------------------------------------------------------------------
option(GLTOOLBOX_BUILD_DEMO "build the demo program" ON)

## DEPENDENCIES
#-------------------------------------------------------------------- 
# glbinding
find_package(GLBINDING REQUIRED)
include_directories(${GLBINDING_INCLUDE_DIR})

#freetype
find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

# eigen
find_package(Eigen3)
if(EIGEN3_FOUND)
  include_directories(${EIGEN3_INCLUDE_DIR})
  add_definitions(-DGLTOOLBOX_ENABLE_EIGEN)
endif()

## INCLUDES
#--------------------------------------------------------------------
include_directories(${PROJECT_SOURCE_DIR}/include)

## FILES
#--------------------------------------------------------------------
set(CPP_FOLDER ${PROJECT_SOURCE_DIR}/source)
set(H_FOLDER ${PROJECT_SOURCE_DIR}/include/gltoolbox)

set(source
    ${CPP_FOLDER}/buffer.cpp
    ${CPP_FOLDER}/framebuffer.cpp
    ${CPP_FOLDER}/program.cpp
    ${CPP_FOLDER}/shader.cpp
    ${CPP_FOLDER}/texture.cpp
    ${CPP_FOLDER}/uniform.cpp
    ${CPP_FOLDER}/vertexarray.cpp
    ${CPP_FOLDER}/utils/textrenderer.cpp)

set(header
    ${H_FOLDER}/gl.h
    ${H_FOLDER}/gltoolbox.h
    ${H_FOLDER}/buffer.h
    ${H_FOLDER}/framebuffer.h
    ${H_FOLDER}/program.h
    ${H_FOLDER}/shader.h
    ${H_FOLDER}/texture.h
    ${H_FOLDER}/uniform.h
    ${H_FOLDER}/vertexarray.h
    ${H_FOLDER}/utils/textrenderer.h
)

## GLTOOLBOX
#--------------------------------------------------------------------
add_library(gltoolbox ${source} ${header})

## EXAMPLE
# --------------------------------------------------------------------
if(GLTOOLBOX_BUILD_DEMO)
  # glfw
  find_package(GLFW REQUIRED)
  if(GLFW_FOUND)
    include_directories(${GLFW_INCLUDE_DIR})
    add_definitions(-DGLFW_INCLUDE_NONE)
  endif()
  #demo
  add_executable(demo examples/demo.cpp examples/shapes.cpp)
  target_link_libraries(demo gltoolbox ${GLBINDING_LIBRARIES} ${FREETYPE_LIBRARIES} ${GLFW_LIBRARIES})
endif()
unset(${GLTOOLBOX_BUILD_DEMO})
