#include <osgUtil/Optimizer>
#include <osgDB/ReadFile>

#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgQt/GraphicsWindowQt>

#include <osgGA/TrackballManipulator>
#include <osgGA/MultiTouchTrackballManipulator>

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

#include <QWidget>
#include <QTimer>
#include <QApplication>
#include <QGridLayout>

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

// int main(int argc,char **argv)
// {
//   // use an ArgumentParser object to manage the program arguments.
//   osg::ArgumentParser arguments(&argc,argv);
// 
//   // read the scene from the list of file specified commandline args.
//   osg::ref_ptr<osg::Node> scene = osgDB::readNodeFiles(arguments);
// 
//   // if not loaded assume no arguments passed in, try use default model instead.
// //   if(!scene) scene = osgDB::readNodeFile("../../test_data/cube.osgb");
//   if(!scene) scene = osgDB::readNodeFile("Tile_+004_+004.osgb");
//   if(!scene)
//   {
//     osg::notify(osg::NOTICE)<<"No model loaded"<<std::endl;
//     return 1;
//   }
//   scene->setName("LODTileMesh");
//   osg::Group* root = new osg::Group;
//   root->setName("ViewerRoot");
//   root->setDataVariance(::osg::Object::DYNAMIC);
// 
//   osg::Group* mesh_group = new osg::Group;
//   mesh_group->addChild(scene);
// 
//   osg::ref_ptr<osg::Node> line =
//     createLine(osg::Vec3(-400,-192,180),osg::Vec3(300,-192,180));
// 
//   DrawLine* draw_line = new DrawLine;
//   draw_line->set_is_activate(true);
// 
//   osgSim::OverlayNode* overlayNode = new osgSim::OverlayNode(
//     osgSim::OverlayNode::OBJECT_DEPENDENT_WITH_ORTHOGRAPHIC_OVERLAY);
//   overlayNode->setContinuousUpdate(true);
//   overlayNode->setOverlaySubgraph(draw_line->root());
//   overlayNode->setOverlayBaseHeight(0.99);
//   overlayNode->addChild(mesh_group);
//   root->addChild(mesh_group);
// 
// 
//   osgViewer::Viewer viewer;
//   viewer.setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
//   viewer.setRunFrameScheme(osgViewer::ViewerBase::CONTINUOUS);
//   viewer.setSceneData(root);
//   viewer.addEventHandler(new DrawLineHandler(draw_line));
// //   viewer.setCameraManipulator(new osgGA::TrackballManipulator());
//   osgGA::TrackballManipulator* trackball_ = new osgGA::TrackballManipulator();
//   trackball_->setAllowThrow(false);
//   viewer.setCameraManipulator(trackball_);
// 
// #if 1
// 
//   // use of custom simulation time.
// 
//   viewer.realize();
// 
//   double simulationTime = 0.0;
// 
//   while(!viewer.done())
//   {
//     trackball_->setNode(root);
//     viewer.frame();
//     simulationTime += 0.001;
//   }
// 
//   return 0;
// #else
// 
//   // normal viewer usage.
//   return viewer.run();
// 
// #endif
// 
// }

class ViewerWidget: public QWidget,public osgViewer::CompositeViewer
{
public:
  ViewerWidget(QWidget* parent = 0,Qt::WindowFlags f = 0,osgViewer::ViewerBase::ThreadingModel threadingModel=osgViewer::CompositeViewer::SingleThreaded): QWidget(parent,f)
  {
    setThreadingModel(threadingModel);

    // disable the default setting of viewer.done() by pressing Escape.
    setKeyEventSetsDone(0);


    osg::ref_ptr<osg::Node> line = createLine(osg::Vec3(-2,0,2),osg::Vec3(2,0,2));
    osgSim::OverlayNode* overlayNode = new osgSim::OverlayNode(
      osgSim::OverlayNode::OBJECT_DEPENDENT_WITH_ORTHOGRAPHIC_OVERLAY);
    overlayNode->setContinuousUpdate(true);
    overlayNode->setOverlaySubgraph(line);
    overlayNode->setOverlayBaseHeight(0.99);
    osg::ref_ptr<osg::Node> cube = osgDB::readNodeFile("cube.osgb");
    overlayNode->addChild(cube);
    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild(overlayNode);
    root->addChild(line);

    QWidget* widget1 = addViewWidget( createGraphicsWindow(0,0,100,100), osgDB::readRefNodeFile("cow.osgt") );
//     QWidget* widget1 = addViewWidget(createGraphicsWindow(0,0,100,100),root);
    QWidget* widget2 = addViewWidget(createGraphicsWindow(0,0,100,100),osgDB::readRefNodeFile("glider.osgt"));
    QWidget* widget3 = addViewWidget(createGraphicsWindow(0,0,100,100),osgDB::readRefNodeFile("axes.osgt"));
    QWidget* widget4 = addViewWidget(createGraphicsWindow(0,0,100,100),osgDB::readRefNodeFile("fountain.osgt"));
    QWidget* popupWidget = addViewWidget(createGraphicsWindow(900,100,320,240,"Popup window",true),osgDB::readRefNodeFile("dumptruck.osgt"));
    popupWidget->show();

    QGridLayout* grid = new QGridLayout;
    grid->addWidget(widget1,0,0);
    grid->addWidget(widget2,0,1);
    grid->addWidget(widget3,1,0);
    grid->addWidget(widget4,1,1);
    setLayout(grid);

    connect(&_timer,SIGNAL(timeout()),this,SLOT(update()));
    _timer.start(10);
  }

  QWidget* addViewWidget(osgQt::GraphicsWindowQt* gw,osg::ref_ptr<osg::Node> scene)
  {
    osgViewer::View* view = new osgViewer::View;
    addView(view);

    osg::Camera* camera = view->getCamera();
    camera->setGraphicsContext(gw);

    const osg::GraphicsContext::Traits* traits = gw->getTraits();

    camera->setClearColor(osg::Vec4(0.2,0.2,0.6,1.0));
    camera->setViewport(new osg::Viewport(0,0,traits->width,traits->height));
    camera->setProjectionMatrixAsPerspective(30.0f,static_cast<double>(traits->width)/static_cast<double>(traits->height),1.0f,10000.0f);

    view->setSceneData(scene);
    view->addEventHandler(new osgViewer::StatsHandler);
    view->setCameraManipulator(new osgGA::MultiTouchTrackballManipulator);
    gw->setTouchEventsEnabled(true);
    return gw->getGLWidget();
  }

  osgQt::GraphicsWindowQt* createGraphicsWindow(int x,int y,int w,int h,const std::string& name="",bool windowDecoration=false)
  {
    osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->windowName = name;
    traits->windowDecoration = windowDecoration;
    traits->x = x;
    traits->y = y;
    traits->width = w;
    traits->height = h;
    traits->doubleBuffer = true;
    traits->alpha = ds->getMinimumNumAlphaBits();
    traits->stencil = ds->getMinimumNumStencilBits();
    traits->sampleBuffers = ds->getMultiSamples();
    traits->samples = ds->getNumMultiSamples();

    return new osgQt::GraphicsWindowQt(traits.get());
  }

  virtual void paintEvent(QPaintEvent* event)
  { frame(); }

protected:

  QTimer _timer;
};

int main(int argc,char** argv)
{

  QApplication app(argc,argv);
  ViewerWidget* viewWidget = new ViewerWidget(0,Qt::Widget,osgViewer::ViewerBase::SingleThreaded);
  viewWidget->setGeometry(100,100,800,600);
  viewWidget->show();
  return app.exec();
}