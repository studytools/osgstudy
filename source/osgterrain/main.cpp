#include <osg/ArgumentParser>
#include <osgDB/ReadFile>
#include <osgDB/ReaderWriter>

#include <osgViewer/Viewer>

#include <osgGA/TrackballManipulator>

#include <osgSim/OverlayNode>


#include "osgterrain.hpp"
#include "draw_line.hpp"

int main(int argc,char** argv)
{
  osg::ArgumentParser arguments(&argc,argv);

  // create the window to draw to.
  osg::ref_ptr<osg::GraphicsContext::Traits> traits = 
    new osg::GraphicsContext::Traits;
  traits->x = 200;
  traits->y = 200;
  traits->width = 800;
  traits->height = 600;
  traits->windowDecoration = true;
  traits->doubleBuffer = true;
  traits->sharedContext = 0;
  osg::ref_ptr<osg::GraphicsContext> gc = 
    osg::GraphicsContext::createGraphicsContext(traits.get());
  osgViewer::GraphicsWindow* gw = 
    dynamic_cast<osgViewer::GraphicsWindow*>(gc.get());
  if(!gw)
  {
    osg::notify(osg::NOTICE)
      <<"Error: unable to create graphics window."<<std::endl;
    return 1;
  }

  osgViewer::Viewer viewer(arguments);
  viewer.getCamera()->setGraphicsContext(gc.get());
  viewer.getCamera()->setViewport(0,0,800,600);
  viewer.setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
  viewer.setKeyEventSetsDone(0);
  viewer.setRunFrameScheme(osgViewer::ViewerBase::CONTINUOUS);


  DEMTerrain* dem_terrain = new DEMTerrain(0x01);
  dem_terrain->ReadDEMFile("dem.tif");

  osg::ref_ptr<osg::Group> root = new osg::Group;
  root->setName("ViewerRoot");
  root->setDataVariance(::osg::Object::DYNAMIC);

//   root->addChild(dem_terrain->terrain_tile());

  DrawLine* draw_line = new DrawLine(0x01); 
  draw_line->set_is_activate(true);
  draw_line->set_terrain(dem_terrain);
  draw_line->set_line_width(10);
//   root->addChild(draw_line->root());

  osg::BoundingSphere bb = root->getBound();

  osgSim::OverlayNode* overlayNode = new osgSim::OverlayNode(
  osgSim::OverlayNode::OBJECT_DEPENDENT_WITH_ORTHOGRAPHIC_OVERLAY);
  overlayNode->setContinuousUpdate(true);
  overlayNode->setOverlaySubgraph(draw_line->root());
  overlayNode->setOverlayBaseHeight(0.99);
//   overlayNode->setOverlayTextureSizeHint(2.0);
  //   osg::ref_ptr<osg::Node> cube = osgDB::readNodeFile("cube.osgb");
  overlayNode->addChild(dem_terrain->terrain_tile());
//   osg::ref_ptr<osg::Group> root = new osg::Group;
  root->addChild(overlayNode);
//   root->addChild(line);



  viewer.setSceneData(root);

  osgGA::TrackballManipulator *trackball_ = new osgGA::TrackballManipulator();
  trackball_->setAllowThrow(false);

  viewer.addEventHandler(new EventCaptureHandler(draw_line));
  viewer.setCameraManipulator(trackball_);
  viewer.realize();

  while(!viewer.done())
  {
    viewer.frame();
  }

  return 0;
}

