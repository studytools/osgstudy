cmake_minimum_required(VERSION 3.0)

if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" VERSION_GREATER  3.1)
cmake_policy(SET CMP0054 OLD)
endif("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" VERSION_GREATER  3.1)

include("cmake/yong_injector/injector.cmake")
include("cmake/yong_hsprj.cmake")
include("cmake/yong_hslib.cmake")

set(SOLUTION_NAME "OSG_Study")
hs_project(${SOLUTION_NAME} CXX)

###############################################################################
#Switching Nodes
option(BUILD_SWITCHING_NODES "build Switching Nodes." OFF)
if(BUILD_SWITCHING_NODES)
add_subdirectory(./source/switching_nodes)
endif()

hs_project_end(${SOLUTION_NAME})
