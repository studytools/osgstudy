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
#include <osg/LineWidth>

#include <osgDB/Registry>

#include <osgSim/OverlayNode>

#include "draw_line.hpp"

osg::Node* createLine(const osg::Vec3& p1, const osg::Vec3& p2)
{
  osg::Vec3Array* line = new osg::Vec3Array;
  line->push_back(p1);
  line->push_back(p2);
  osg::Vec4Array* colors = new osg::Vec4Array;
  colors->push_back(osg::Vec4(1.0f,0.0f,0.0f,1.0f));
  osg::Geometry* geom = new osg::Geometry;
  geom->setVertexArray(line);
  geom->setColorArray(colors,osg::Array::BIND_OVERALL);
  geom->addPrimitiveSet(new ::osg::DrawArrays(GL_LINES,0,2));

  osg::Geode* geode = new osg::Geode;
  osg::StateSet* stateset = geode->getOrCreateStateSet();
  stateset->setAttribute(new ::osg::LineWidth(3));
  geode->addDrawable(geom);

  return geode;
}

int main(int argc,char **argv)
{
  // use an ArgumentParser object to manage the program arguments.
  osg::ArgumentParser arguments(&argc,argv);

  // read the scene from the list of file specified commandline args.
  osg::ref_ptr<osg::Node> scene = osgDB::readNodeFiles(arguments);

  // if not loaded assume no arguments passed in, try use default model instead.
//   if(!scene) scene = osgDB::readNodeFile("../../test_data/cube.osgb");
  if(!scene) scene = osgDB::readNodeFile("Tile_+004_+004.osgb");
  if(!scene)
  {
    osg::notify(osg::NOTICE)<<"No model loaded"<<std::endl;
    return 1;
  }
  scene->setName("LODTileMesh");
  osg::Group* root = new osg::Group;

  osg::ref_ptr<osg::Node> line =
    createLine(osg::Vec3(-400,-192,180),osg::Vec3(300,-192,180));

  DrawLine* draw_line = new DrawLine;
  draw_line->set_is_activate(true);

  osgSim::OverlayNode* overlayNode = new osgSim::OverlayNode(
    osgSim::OverlayNode::OBJECT_DEPENDENT_WITH_ORTHOGRAPHIC_OVERLAY);
  overlayNode->setContinuousUpdate(true);
  overlayNode->setOverlaySubgraph(draw_line->root());
  overlayNode->setOverlayBaseHeight(0.99);
  overlayNode->addChild(scene);
  root->addChild(overlayNode);


  osgViewer::Viewer viewer;
//   viewer.setUpViewAcrossAllScreens();
//   osgViewer::Viewer::Windows windows;
//   viewer.getWindows(windows);
// 
//   if(windows.empty()) return 1;

  // set the scene to render
  viewer.setSceneData(root);
  viewer.addEventHandler(new DrawLineHandler(draw_line));
  viewer.setCameraManipulator(new osgGA::TrackballManipulator());

#if 1

  // use of custom simulation time.

  viewer.realize();

  double simulationTime = 0.0;

  while(!viewer.done())
  {
//     overlayNode->setOverlaySubgraph(draw_line->root());
    viewer.frame(simulationTime);
    simulationTime += 0.001;
  }

  return 0;
#else

  // normal viewer usage.
  return viewer.run();

#endif

}
