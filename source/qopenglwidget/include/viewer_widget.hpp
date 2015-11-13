#ifndef _OSG_STUDY_VIEWER_WIDGET_HPP_
#define _OSG_STUDY_VIEWER_WIDGET_HPP_

#include "qopenglwidget.hpp"

#include <osg/Group>
#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/GraphicsWindow>
#include <osgViewer/Viewer>

USE_OSGPLUGIN(osg2)
USE_OSGPLUGIN(jpeg)
USE_OSGPLUGIN(png)
USE_OSGPLUGIN(tiff)

USE_SERIALIZER_WRAPPER_LIBRARY(osg)

class ViewerWidget: public OpenglWidget, public osgViewer::Viewer
{
public:
  ViewerWidget(QWidget *parent = 0);
  ~ViewerWidget();

  virtual void initializeGL();
  virtual void paintGL();
  virtual void resizeGL(int width, int height);

private:
  osg::ref_ptr<osg::Group> root_;
  osg::ref_ptr<osgGA::TrackballManipulator> trackball_;

};

#endif