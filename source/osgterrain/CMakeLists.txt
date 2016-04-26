###############################################################################
#osgterrain
set(SOURCES_FILES
	"main.cpp"
	"osgterrain.hpp"	
	"osgterrain.cpp" 
	"draw_line.hpp"	
	"draw_line.cpp" 
	"event_capture.hpp"	
	"event_capture.cpp") 

find_package(GDAL REQUIRED)

find_package(OpenSceneGraph 3.5.2 REQUIRED osgDB osgUtil osgViewer osgGA osgTerrain osgSim)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})
include_directories(${GDAL_INCLUDE_DIR})

add_executable(osgterrain ${SOURCES_FILES})

target_link_libraries(osgterrain ${OPENSCENEGRAPH_LIBRARIES} ${GDAL_LIBRARY})