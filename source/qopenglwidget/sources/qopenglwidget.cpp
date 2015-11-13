#include "qopenglwidget.hpp"

OpenglWidget::OpenglWidget(QWidget *parent)
  : QOpenGLWidget(parent)
{
  //set the widget accepts keyboard focus
  setFocusPolicy(Qt::FocusPolicy::StrongFocus);

  graphics_window_ = new GraphicsOpenglWindowQt(createTraits(),this);
}

OpenglWidget::~OpenglWidget()
{
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
  QOpenGLWidget::mousePressEvent(event);

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

void OpenglWidget::moveEvent(QMoveEvent* event)
{
  const QPoint& pos = event->pos();
  int scaled_width = static_cast<int>(width());
  int scaled_height = static_cast<int>(height());
  graphics_window_->resized(pos.x(),pos.y(),scaled_width,scaled_height);
  graphics_window_->getEventQueue()->windowResize(pos.x(),pos.y(),scaled_width,scaled_height);
  QOpenGLWidget::moveEvent(event);
}

void OpenglWidget::resizeEvent(QResizeEvent *event)
{
  const QSize& size = event->size();

  int scaled_width = static_cast<int>(size.width());
  int scaled_height = static_cast<int>(size.height());
  graphics_window_->resized(x(),y(),scaled_width,scaled_height);
  graphics_window_->getEventQueue()->windowResize(
    x(),y(),scaled_width,scaled_height);
  graphics_window_->requestRedraw();
  QOpenGLWidget::resizeEvent(event);
}

bool OpenglWidget::event(QEvent* event)
{
  bool handled = QOpenGLWidget::event(event);

  switch(event->type())
  {
  case QEvent::KeyPress:
  case QEvent::KeyRelease:
  case QEvent::MouseButtonDblClick:
  case QEvent::MouseButtonPress:
  case QEvent::MouseButtonRelease:
  case QEvent::MouseMove:
  case QEvent::Wheel:
  this->update();
  break;

  default:
  break;
  }

  return handled;
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

osg::GraphicsContext::Traits* OpenglWidget::createTraits()
{
  osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
  osg::GraphicsContext::Traits *traits = new osg::GraphicsContext::Traits(ds);

  qsurfaceFormat2traits(format(),traits);

  QRect r = geometry();
  traits->x = r.x();
  traits->y = r.y();
  traits->width = r.width();
  traits->height = r.height();

  traits->windowName = windowTitle().toLocal8Bit().data();
  Qt::WindowFlags f = windowFlags();
  traits->windowDecoration = (f & Qt::WindowTitleHint) &&
                          (f & Qt::WindowMinMaxButtonsHint) &&
                          (f & Qt::WindowSystemMenuHint);
  QSizePolicy sp = sizePolicy();
  traits->supportsResize = sp.horizontalPolicy() != QSizePolicy::Fixed ||
                          sp.verticalPolicy() != QSizePolicy::Fixed;

  return traits;
}

void OpenglWidget::qsurfaceFormat2traits(
  const QSurfaceFormat& format,::osg::GraphicsContext::Traits* traits)
{
  traits->red = format.redBufferSize();
  traits->green = format.greenBufferSize();
  traits->blue = format.blueBufferSize();
  traits->alpha = format.hasAlpha() ? format.alphaBufferSize() : 0;
  //   traits->depth = format.depth() ? format.depthBufferSize() : 0;
  traits->depth = format.depthBufferSize();
  //   traits->stencil = format.stencil() ? format.stencilBufferSize() : 0;
  traits->stencil = format.stencilBufferSize();

  //   traits->sampleBuffers = format.sampleBuffers() ? 1 : 0;
  traits->samples = format.samples();

  traits->quadBufferStereo = format.stereo();
  //   traits->doubleBuffer = format.doubleBuffer();

  traits->vsync = format.swapInterval() >= 1;
}

