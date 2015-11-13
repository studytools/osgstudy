#include "viewer_widget.hpp"

ViewerWidget::ViewerWidget(QWidget *parent)
  : OpenglWidget(parent)
{
  root_ = new osg::Group;
}

ViewerWidget::~ViewerWidget()
{
}

void ViewerWidget::initializeGL()
{
  getCamera()->setGraphicsContext(graphics_window_);
  getCamera()->setClearColor(osg::Vec4(0.3,0.3,0.6,0.8));
  getCamera()->setViewport(new osg::Viewport(0,0,width(),height()));
  getCamera()->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  getCamera()->setProjectionMatrixAsPerspective(
    30.0f,
    static_cast<double>(width()) / static_cast<double>(height()),
    1.0f,10000.0f);

  setThreadingModel(osgViewer::Viewer::SingleThreaded);

  trackball_ = new osgGA::TrackballManipulator;
  trackball_->setAllowThrow(false);
  setCameraManipulator(trackball_);

  root_->setName("GraphicsOpenglWindowQtRoot");
  root_->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
  root_->getOrCreateStateSet()->setMode(GL_DEPTH_TEST,osg::StateAttribute::ON);

  osg::ref_ptr<osg::Node> pNode =
    osgDB::readNodeFile("../../test_data/cube.osgb");
  root_->addChild(pNode);

  setSceneData(root_);

}

void ViewerWidget::paintGL()
{
  trackball_->setNode(root_);
  frame();
}

void ViewerWidget::resizeGL(int width,int height)
{
  getCamera()->setViewport(new osg::Viewport(0,0,width,height));
}
