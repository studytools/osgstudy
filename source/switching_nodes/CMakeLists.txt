###############################################################################
#Switching Nodes
set(PROJECT_NAME "Switching_Nodes")
set(PROJECT_SOURCES "switching_nodes.cpp") 

hslib_add_executable(${PROJECT_NAME} WIN32
                     SOURCES ${PROJECT_SOURCES})

yong_add_dependence(${SOLUTION_NAME} ${PROJECT_NAME}
  DEPENDENT_PROJECT OpenSceneGraph
    DEPENDENT_LIBS osg OpenThreads osgDB osgUtil osgViewer osgGA osgQt
                   osgWidget osgText
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