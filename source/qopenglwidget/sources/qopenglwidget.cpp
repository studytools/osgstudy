#include <osgUtil/Optimizer>
#include <osgDB/ReadFile>

#include <osg/MatrixTransform>
#include <osgGA/TrackballManipulator>
#include <osgGA/StateSetManipulator>

#include "qopenglwidget.hpp"

namespace osgQt
{

OSGOpenglWidget::OSGOpenglWidget(QWidget *parent)
  : QOpenGLWidget(parent)
{
  setMinimumSize(300,250);
  setMouseTracking(true);
}

OSGOpenglWidget::~OSGOpenglWidget()
{
}

void OSGOpenglWidget::initializeGL()
{
  root_ = new osg::Group;
  root_->setName("Root");

  this->setCamera(CreateCamera(0,0,width(),height()));
  osg::ref_ptr<osgGA::TrackballManipulator> pManipulator =
      new osgGA::TrackballManipulator;
  this->setCameraManipulator(pManipulator);
  this->setThreadingModel(osgViewer::ViewerBase::SingleThreaded);

  root_->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
  root_->getOrCreateStateSet()->setMode(GL_DEPTH_TEST,osg::StateAttribute::ON);
  this->setSceneData(root_);

  // 添加cube.osgb作为测试
  osg::ref_ptr<osg::Node> pNode = osgDB::readNodeFile("cube.osgb");
  root_->addChild(pNode);

  startTimer(10);
}

void OSGOpenglWidget::paintGL()
{
  frame();
}

void OSGOpenglWidget::resizeGL(int w,int h)
{
  getCamera()->setViewport(0,0,w,h);
}

void OSGOpenglWidget::timerEvent(QTimerEvent *event)
{ 
  update();
}

void OSGOpenglWidget::mousePressEvent(QMouseEvent* event)
{
//   QPoint pos = event->pos();
//   QList<QGraphicsItem*> listItems = items(mapToScene(pos.x(),pos.y()).toPoint());
//   if(listItems.size() > 0)
//   {
//     QGraphicsView::mousePressEvent(event);
//   }
//   else
//   {
//     int button = 0;
//     switch(event->button())
//     {
//     case Qt::LeftButton: button = 1; break;
//     case Qt::MidButton: button = 2; break;
//     case Qt::RightButton: button = 3; break;
//     case Qt::NoButton: button = 0; break;
//     default: button = 0; break;
//     }
//     setKeyboardModifiers(event);
//     m_pGraphicsWindow->getEventQueue()->mouseButtonPress(event->x(),event->y(),button);
//   }
}

void OSGOpenglWidget::setKeyboardModifiers(QInputEvent* event)
{
  int modkey = event->modifiers() & (Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier);
  unsigned int mask = 0;
  if(modkey & Qt::ShiftModifier) mask |= osgGA::GUIEventAdapter::MODKEY_SHIFT;
  if(modkey & Qt::ControlModifier) mask |= osgGA::GUIEventAdapter::MODKEY_CTRL;
  if(modkey & Qt::AltModifier) mask |= osgGA::GUIEventAdapter::MODKEY_ALT;
  _gw->getEventQueue()->getCurrentEventState()->setModKeyMask(mask);
}

void OSGOpenglWidget::mouseReleaseEvent(QMouseEvent* event)
{
  int button = 0;
  switch(event->button())
  {
  case Qt::LeftButton: button = 1; break;
  case Qt::MidButton: button = 2; break;
  case Qt::RightButton: button = 3; break;
  case Qt::NoButton: button = 0; break;
  default: button = 0; break;
  }
  setKeyboardModifiers(event);
  m_pGraphicsWindow->getEventQueue()->mouseButtonRelease(event->x(),event->y(),button);

  QGraphicsView::mouseReleaseEvent(event);
}


osg::ref_ptr<osg::Camera> OSGOpenglWidget::CreateCamera(int x,int y,int w,int h)
{
  osgViewer::GraphicsWindowEmbedded* m_pGraphicsWindow =
    new osgViewer::GraphicsWindowEmbedded(x,y,w,h);
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

  osg::ref_ptr<osg::Camera> camera = new osg::Camera;
  camera->setGraphicsContext(m_pGraphicsWindow);
  camera->setClearColor(osg::Vec4(0.3,0.3,0.6,0.8));
  camera->setViewport(new osg::Viewport(0,0,traits->width,traits->height));
  camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  camera->setProjectionMatrixAsPerspective(
      30.0f,
      static_cast<double>(traits->width) / 
      static_cast<double>(traits->height),
      1.0f,10000.0f);

  return camera.release();
}

}