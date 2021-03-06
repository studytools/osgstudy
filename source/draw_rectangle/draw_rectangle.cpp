﻿#include <osgUtil/Optimizer>
#include <osgDB/ReadFile>

#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgViewer/GraphicsWindow>

#include <osgGA/TrackballManipulator>

#include <osg/Material>
#include <osg/Geode>
#include <osg/BlendFunc>
#include <osg/Depth>
#include <osg/PolygonOffset>
#include <osg/MatrixTransform>
#include <osg/Camera>
#include <osg/RenderInfo>

#include <osgDB/Registry>

USE_GRAPHICSWINDOW() //静态链接手动调用该宏.
USE_OSGPLUGIN(jpeg)
USE_OSGPLUGIN(osg2)

USE_SERIALIZER_WRAPPER_LIBRARY(osg)


class DrawLine
{
public:

protected:
private:


};



int main(int argc,char **argv)
{
  osg::ref_ptr<osg::Node> scene = osgDB::readNodeFile("../../test_data/cube.osgb");
  if(!scene)
  {
    osg::notify(osg::NOTICE)<<"No model loaded"<<std::endl;
    return 1;
  }
  osgViewer::Viewer viewer;
  viewer.setUpViewAcrossAllScreens();
  osgViewer::Viewer::Windows windows;
  viewer.getWindows(windows);

  if(windows.empty()) return 1;

  // set the scene to render
  viewer.setSceneData(scene.get());
  viewer.realize();

  return viewer.run();
}
