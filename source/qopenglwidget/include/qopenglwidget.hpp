#ifndef _OSG_STUDY_QOPENGLWIDGET_HPP_
#define _OSG_STUDY_QOPENGLWIDGET_HPP_

#include "OSG_Study/config/hs_config.hpp"

#include <QOpenGLWidget>

#include <osg/Group>
#include <osg/ref_ptr>
#include <osgViewer/Viewer>

#include <QWidget>
#include <QOpenGLWidget>
#include <QMouseEvent>

class QInputEvent;

namespace osgQt
{

class OSGOpenglWidget : public QOpenGLWidget, public osgViewer::Viewer
{
  Q_OBJECT
public:
  OSGOpenglWidget(QWidget *parent = 0);
  ~OSGOpenglWidget();

protected:
  virtual void resizeGL(int w,int h) Q_DECL_OVERRIDE;
  virtual void paintGL() Q_DECL_OVERRIDE;
  virtual void initializeGL() Q_DECL_OVERRIDE;
  virtual void timerEvent(QTimerEvent *event);

  void setKeyboardModifiers(QInputEvent* event);

  virtual void mousePressEvent(QMouseEvent* event);
  virtual void mouseReleaseEvent(QMouseEvent* event);

private:
  osg::ref_ptr<osg::Camera> CreateCamera(int x, int y, int w, int h);

private:
  osg::ref_ptr<osg::Group> root_;
  osg::ref_ptr<osg::Camera> camera_;

};

class GraphicsOpenglWindowQt: public osgViewer::GraphicsWindow
{
public:
  GraphicsOpenglWindowQt(
    osg::GraphicsContext::Traits* traits,
    QWidget* parent = NULL,
    const QGLWidget* shareWidget = NULL,
    Qt::WindowFlags f = 0);
  GraphicsOpenglWindowQt( OSGOpenglWidget* widget );
  ~GraphicsOpenglWindowQt();
};


}

#endif
