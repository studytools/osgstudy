#include "graphicswindowqt.hpp"

#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/api/Win32/GraphicsWindowWin32>

GraphicsOpenglWindowQt::GraphicsOpenglWindowQt(QWidget* parent)
  : OpenglWidget(parent)
{
  setMouseTracking(true);
}
GraphicsOpenglWindowQt::~GraphicsOpenglWindowQt()
{
}

void GraphicsOpenglWindowQt::initializeGL()
{
  root_ = new osg::Group;
  root_->setName("GraphicsOpenglWindowQtRoot");

  createCamera(0,0,width(),height());
  this->setThreadingModel(osgViewer::Viewer::SingleThreaded);

  root_->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
  root_->getOrCreateStateSet()->setMode(GL_DEPTH_TEST,osg::StateAttribute::ON);
  this->setSceneData(root_);

  osg::ref_ptr<osgGA::TrackballManipulator> pManipulator =
    new osgGA::TrackballManipulator;
  this->setCameraManipulator(pManipulator);

  osg::ref_ptr<osg::Node> pNode = 
    osgDB::readNodeFile("../../test_data/cube.osgb");
  root_->addChild(pNode);

  startTimer(10);
}

void GraphicsOpenglWindowQt::paintGL()
{
  frame();
}
void GraphicsOpenglWindowQt::timerEvent(QTimerEvent *event)
{ 
  update();
}

void GraphicsOpenglWindowQt::createCamera(int x,int y,int w,int h)
{
  osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
  osg::ref_ptr<osg::GraphicsContext::Traits> traits = 
    new osg::GraphicsContext::Traits;
  traits->windowDecoration = true;
  traits->x = x;
  traits->y = y;
  traits->width = w;
  traits->height = h;
  traits->doubleBuffer = true;
  traits->sharedContext = 0;

  graphics_window_ = new osgViewer::GraphicsWindowEmbedded(traits.get());

//    osg::ref_ptr<osg::GraphicsContext> gc =
//      osg::GraphicsContext::createGraphicsContext(traits.get());
//     
//   graphics_window_ = dynamic_cast<osgViewer::GraphicsWindow*>(gc.get());

  getCamera()->setGraphicsContext(graphics_window_);
  getCamera()->setClearColor(osg::Vec4(0.3,0.3,0.6,0.8));
  getCamera()->setViewport(new osg::Viewport(0,0,traits->width,traits->height));
  getCamera()->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  getCamera()->setProjectionMatrixAsPerspective(
    30.0f,
    static_cast<double>(traits->width) / static_cast<double>(traits->height),
    1.0f,10000.0f);

}

