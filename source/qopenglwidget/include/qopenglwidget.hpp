#ifndef _OSG_STUDY_QOPENGLWIDGET_HPP_
#define _OSG_STUDY_QOPENGLWIDGET_HPP_

#include "OSG_Study/config/hs_config.hpp"

#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QMoveEvent>
#include <QResizeEvent>
#include <QOpenGLWidget>

#include <osg/GraphicsContext>

#include "graphicswindowqt.hpp"

class OpenglWidget : public QOpenGLWidget
{
public:
  OpenglWidget(QWidget *parent = 0);
  virtual ~OpenglWidget();

protected:
  void keyPressEvent(QKeyEvent* event);
  void keyReleaseEvent(QKeyEvent* event);
  void mousePressEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);
  void mouseDoubleClickEvent(QMouseEvent* event);
  void mouseMoveEvent(QMouseEvent* event);
  void wheelEvent(QWheelEvent* event);
  void moveEvent(QMoveEvent* event);
  void resizeEvent(QResizeEvent *event);
  bool event(QEvent* event);

  virtual void initializeGL(){};
  virtual void paintGL(){};
  virtual void resizeGL(int width, int height){};

  friend class GraphicsOpenglWindowQt;
  GraphicsOpenglWindowQt* graphics_window_;

private:
  void setKeyboardModifiers(QInputEvent* event);
  void createCamera(int x,int y,int w,int h);
  static void qsurfaceFormat2traits(
    const QSurfaceFormat& format,
    osg::GraphicsContext::Traits* traits);
  osg::GraphicsContext::Traits* createTraits();

};

#endif
