###############################################################################
#Intersection Test
set(PROJECT_NAME "intersection_test")
set(PROJECT_SOURCES "intersection_test.cpp") 

hslib_add_executable(${PROJECT_NAME} WIN32
                     SOURCES ${PROJECT_SOURCES})

yong_add_dependence(${SOLUTION_NAME} ${PROJECT_NAME}
  DEPENDENT_PROJECT OpenSceneGraph
    DEPENDENT_LIBS osg OpenThreads osgDB osgUtil osgViewer osgGA
  DEPENDENT_PROJECT tiff
    DEPENDENT_LIBS tiff
  DEPENDENT_PROJECT zlib
    DEPENDENT_LIBS zlib               
  DEPENDENT_PROJECT jpeg_turbo
    DEPENDENT_LIBS jpeg_turbo
  DEPENDENT_PROJECT png
    DEPENDENT_LIBS png
)

hslib_add_executable_end(${PROJECT_NAME})