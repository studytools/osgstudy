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
#include <osg/ShapeDrawable>

#include <osgDB/Registry>

int main(int argc,char **argv)
{
  // use an ArgumentParser object to manage the program arguments.
  osg::ArgumentParser arguments(&argc,argv);

  // read the scene from the list of file specified commandline args.
  osg::ref_ptr<osg::Node> scene = osgDB::readNodeFiles(arguments);

  // if not loaded assume no arguments passed in, try use default model instead.
  if(!scene) scene = osgDB::readNodeFile("../../test_data/cube.osgb");
  if(!scene)
  {
    osg::notify(osg::NOTICE)<<"No model loaded"<<std::endl;
    return 1;
  }

  osg::Group* root = new osg::Group;

  osg::Sphere* sp = new osg::Sphere(osg::Vec3(0,0,0),0.5f);
  osg::Geode* geode = new osg::Geode;
  osg::ShapeDrawable* sphere_drawable = new osg::ShapeDrawable(sp);
  geode->addDrawable(sphere_drawable);



  root->addChild(geode);
  root->addChild(scene.get());

  sp->setCenter(osg::Vec3(0,0,0));

  osgViewer::Viewer viewer;
  viewer.setUpViewAcrossAllScreens();
  osgViewer::Viewer::Windows windows;
  viewer.getWindows(windows);

  if(windows.empty()) return 1;

  // set the scene to render
  viewer.setSceneData(root);
  viewer.realize();

  return viewer.run();
}
