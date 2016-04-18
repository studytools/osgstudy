###############################################################################
#osgterrain
set(SOURCES_FILES "osgterrain.cpp") 

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(GDAL REQUIRED)

find_package(OpenSceneGraph 3.5.2 REQUIRED osgDB osgUtil osgViewer osgGA osgTerrain)
include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})

add_executable(osgterrain ${SOURCES_FILES})

target_link_libraries(osgterrain ${OPENSCENEGRAPH_LIBRARIES} ${GDAL_LIBRARY})