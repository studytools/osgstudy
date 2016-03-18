set(SOURCES_FILES 
  overlaynode.cpp
  draw_line.hpp
  draw_line.cpp) 

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)  
  
find_package(OpenSceneGraph 3.5.2 REQUIRED osgDB osgUtil osgSim osgViewer osgGA)
include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})

add_executable(overlaynode ${SOURCES_FILES})
  
target_link_libraries(overlaynode ${OPENSCENEGRAPH_LIBRARIES})