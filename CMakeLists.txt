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

# eigen
find_package(Eigen3)
if(EIGEN3_FOUND)
  include_directories(${EIGEN3_INCLUDE_DIR})
  add_definitions(-DGLTOOLBOX_ENABLE_EIGEN)
  message("eigen:")
  message("${EIGEN3_INCLUDE_DIR}")
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
    ${CPP_FOLDER}/program.cpp
    ${CPP_FOLDER}/shader.cpp
    ${CPP_FOLDER}/uniform.cpp
    ${CPP_FOLDER}/vertexattrib.cpp)

set(header
    ${H_FOLDER}/gl.h
    ${H_FOLDER}/gltoolbox.h
    ${H_FOLDER}/buffer.h
    ${H_FOLDER}/program.h
    ${H_FOLDER}/shader.h
    ${H_FOLDER}/uniform.h
    ${H_FOLDER}/vertexattrib.h)

## GLTOOLBOX
#--------------------------------------------------------------------
add_library(gltoolbox ${source} ${header})

## EXAMPLES
#--------------------------------------------------------------------
add_executable(gl examples/gl.cpp)
target_link_libraries(gl gltoolbox ${GLBINDING_LIBRARIES} ${GLFW_LIBRARIES})