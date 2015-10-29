###############################################################################
#qopenglwidget
set(PROJECT_NAME "qopenglwidget")
set(OSG_EXAMPLE_SOURCES
  "include/mainwindow.hpp"
  "include/qopenglwidget.hpp"
  "include/graphicswindowqt.hpp"
  "sources/main.cpp"
  "sources/mainwindow.cpp"
  "sources/qopenglwidget.cpp"
  "sources/graphicswindowqt.cpp"
  )
set(OSG_EXAMPLE_QT_MODULES
  "Widgets"
  "Core"
  "OpenGL"
  )
set(OSG_EXAMPLE_RESOURCES
  "resource/qopenglwidget.qrc"
  )
  
include_directories(include) 

hslib_add_qt_executable(${PROJECT_NAME}
  SOURCES ${OSG_EXAMPLE_SOURCES}
  QT_MODULES ${OSG_EXAMPLE_QT_MODULES}
  RESOURCES ${OSG_EXAMPLE_RESOURCES}
  )
  
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
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
hslib_add_executable_end(${PROJECT_NAME})