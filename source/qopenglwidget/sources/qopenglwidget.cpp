#include "qopenglwidget.hpp"

OpenglWidget::OpenglWidget(QWidget *parent)
  : QOpenGLWidget(parent)
{
}

OpenglWidget::~OpenglWidget()
{
}

void OpenglWidget::setKeyboardModifiers(QInputEvent* event)
{
  int modkey = event->modifiers() & 
    (Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier);
  unsigned int mask = 0;
  if(modkey & Qt::ShiftModifier) mask |= osgGA::GUIEventAdapter::MODKEY_SHIFT;
  if(modkey & Qt::ControlModifier) mask |= osgGA::GUIEventAdapter::MODKEY_CTRL;
  if(modkey & Qt::AltModifier) mask |= osgGA::GUIEventAdapter::MODKEY_ALT;
  graphics_window_->getEventQueue()->getCurrentEventState()->setModKeyMask(mask);
}

void OpenglWidget::keyPressEvent(QKeyEvent* event)
{
  QPoint pos = QCursor::pos();
  setKeyboardModifiers(event);
  graphics_window_->getEventQueue()->keyPress(
    (osgGA::GUIEventAdapter::KeySymbol)*(event->text().toLatin1().data()));
  QOpenGLWidget::keyPressEvent(event);
}

void OpenglWidget::keyReleaseEvent(QKeyEvent* event)
{
  setKeyboardModifiers(event);
  graphics_window_->getEventQueue()->keyRelease(
    (osgGA::GUIEventAdapter::KeySymbol)*(event->text().toLatin1().data()));
  QOpenGLWidget::keyReleaseEvent(event);
}

void OpenglWidget::mousePressEvent(QMouseEvent* event)
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
  graphics_window_->getEventQueue()->mouseButtonPress(
    event->x(),event->y(),button);
}

void OpenglWidget::mouseReleaseEvent(QMouseEvent* event)
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
  graphics_window_->getEventQueue()->mouseButtonRelease(
    event->x(),event->y(),button);

  QOpenGLWidget::mouseReleaseEvent(event);
}

void OpenglWidget::mouseDoubleClickEvent(QMouseEvent* event)
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
  graphics_window_->getEventQueue()->mouseDoubleButtonPress(
    event->x(),event->y(),button);

  QOpenGLWidget::mouseDoubleClickEvent(event);
}

void OpenglWidget::mouseMoveEvent(QMouseEvent* event)
{
  setKeyboardModifiers(event);
  graphics_window_->getEventQueue()->mouseMotion(event->x(),event->y());
  QOpenGLWidget::mouseMoveEvent(event);
}

void OpenglWidget::wheelEvent(QWheelEvent* event)
{
  setKeyboardModifiers(event);
  graphics_window_->getEventQueue()->mouseScroll(
    event->orientation() == Qt::Vertical ?
    (event->delta() > 0 ? osgGA::GUIEventAdapter::SCROLL_UP : 
    osgGA::GUIEventAdapter::SCROLL_DOWN) :
    (event->delta() > 0 ? osgGA::GUIEventAdapter::SCROLL_LEFT : 
    osgGA::GUIEventAdapter::SCROLL_RIGHT));
  QOpenGLWidget::wheelEvent(event);
}

void OpenglWidget::resizeGL(int width,int height)
{
  graphics_window_->getEventQueue()->windowResize(0,0,width,height);
  graphics_window_->resized(0,0,width,height);
}