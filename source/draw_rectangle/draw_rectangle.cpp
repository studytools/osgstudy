#include <osgUtil/Optimizer>
#include <osgDB/ReadFile>

#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>

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

int main(int argc,char **argv)
{
  // use an ArgumentParser object to manage the program arguments.
  osg::ArgumentParser arguments(&argc,argv);

  // read the scene from the list of file specified commandline args.
  osg::ref_ptr<osg::Node> scene = osgDB::readNodeFiles(arguments);

  // if not loaded assume no arguments passed in, try use default model instead.
  if(!scene) scene = osgDB::readNodeFile("cube.osgb");
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
