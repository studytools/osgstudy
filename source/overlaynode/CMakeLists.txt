set(SOURCES_FILES 
  overlaynode.cpp
  draw_line.hpp
  draw_line.cpp
  main.cpp
  mainwindow.hpp
  mainwindow.cpp) 
  
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5OpenGL REQUIRED)
get_target_property(QtCore_location Qt5::Core LOCATION)

qt5_wrap_ui(UI_FILES widget.ui)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
  
find_package(OpenSceneGraph 3.5.2 REQUIRED osgDB osgUtil osgSim osgViewer osgGA osgQt)
include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})

add_executable(overlaynode ${SOURCES_FILES} ${UI_FILES})
  
target_link_libraries(overlaynode ${OPENSCENEGRAPH_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5OpenGL_LIBRARIES})