#ifndef _OSG_STUDY_QOPENGLWIDGET_HPP_
#define _OSG_STUDY_QOPENGLWIDGET_HPP_

#include "OSG_Study/config/hs_config.hpp"

#include <QtGui/QInputEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QWheelEvent>
#include <QtWidgets/QOpenGLWidget>

#include <osgViewer/GraphicsWindow>

class OpenglWidget : public QOpenGLWidget
{
  Q_OBJECT
public:
  OpenglWidget(QWidget *parent = 0);
  ~OpenglWidget();

protected:
  virtual void keyPressEvent(QKeyEvent* event);
  virtual void keyReleaseEvent(QKeyEvent* event);
  virtual void mousePressEvent(QMouseEvent* event);
  virtual void mouseReleaseEvent(QMouseEvent* event);
  virtual void mouseDoubleClickEvent(QMouseEvent* event);
  virtual void mouseMoveEvent(QMouseEvent* event);
  virtual void wheelEvent(QWheelEvent* event);
  virtual void resizeGL(int width,int height);

private:
  void setKeyboardModifiers(QInputEvent* event);

protected:
  osgViewer::GraphicsWindow* graphics_window_;
};

#endif
