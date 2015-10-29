cmake_minimum_required(VERSION 3.0)

if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" VERSION_GREATER  3.1)
cmake_policy(SET CMP0054 OLD)
endif("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" VERSION_GREATER  3.1)

include("cmake/yong_injector/injector.cmake")
include("cmake/yong_hsprj.cmake")
include("cmake/yong_hslib.cmake")

set(SOLUTION_NAME "OSG_Study")
hs_project(${SOLUTION_NAME} CXX)

find_package(OpenGL)
add_definitions(-DOSG_LIBRARY_STATIC -DOT_LIBRARY_STATIC)

###############################################################################
#Switching Nodes
option(BUILD_SWITCHING_NODES "build Switching Nodes." OFF)
if(BUILD_SWITCHING_NODES)
add_subdirectory(./source/switching_nodes)
endif()

###############################################################################
#INTERSECTION_TEST
option(BUILD_INTERSECTION_TEST "build Intersection Test." OFF)
if(BUILD_INTERSECTION_TEST)
add_subdirectory(./source/intersection_test)
endif()

###############################################################################
#Draw Rectangle
option(BUILD_DRAW_RECTANGLE_TEST "build Draw Rectangle." OFF)
if(BUILD_DRAW_RECTANGLE_TEST)
add_subdirectory(./source/draw_rectangle)
endif()

###############################################################################
#Draw Rectangle
option(BUILD_THUMBATCK_TEST "build thumbatck." OFF)
if(BUILD_THUMBATCK_TEST)
add_subdirectory(./source/thumbatck)
endif()

###############################################################################
#qopenglwidget
option(BUILD_QOPENGLWIDGET_TEST "build qopenglwidget." OFF)
if(BUILD_QOPENGLWIDGET_TEST)
add_subdirectory(./source/qopenglwidget)
endif()


hs_project_end(${SOLUTION_NAME})
