#include <iostream>

#include "graphicswindowqt.hpp"

#include <QOpenGLContext>

GraphicsOpenglWindowQt::GraphicsOpenglWindowQt(
  ::osg::GraphicsContext::Traits* traits,OpenglWidget* widget)
{
  _traits = traits;
  widget_ = widget;
  init();
}
GraphicsOpenglWindowQt::~GraphicsOpenglWindowQt()
{
}

bool GraphicsOpenglWindowQt::init()
{
  if (!widget_)
  {
    return false;
  }
  
  // initialize State
  setState(new osg::State);
  getState()->setGraphicsContext(this);

  // initialize contextID
  if(_traits.valid() && _traits->sharedContext.valid())
  {
    getState()->setContextID(_traits->sharedContext->getState()->getContextID());
    incrementContextIDUsageCount(getState()->getContextID());
  }
  else
  {
    getState()->setContextID(osg::GraphicsContext::createNewContextID());
  }

  // make sure the event queue has the correct window rectangle size and input range
  getEventQueue()->syncWindowRectangleWithGraphicsContext();

  return true;


}

bool GraphicsOpenglWindowQt::setWindowRectangleImplementation(int x,int y,int width,int height)
{
  if(widget_ == NULL)
      return false;

  widget_->setGeometry(x,y,width,height);
  return true;
}


bool GraphicsOpenglWindowQt::setWindowDecorationImplementation(bool windowDecoration)
{
  Qt::WindowFlags flags = Qt::Window|Qt::CustomizeWindowHint;//|Qt::WindowStaysOnTopHint;
  if(windowDecoration)
      flags |= Qt::WindowTitleHint|Qt::WindowMinMaxButtonsHint|Qt::WindowSystemMenuHint;
  _traits->windowDecoration = windowDecoration;

  if(widget_)
  {
    widget_->setWindowFlags(flags);

    return true;
  }

  return false;
}

void GraphicsOpenglWindowQt::grabFocus()
{
  if(widget_)
      widget_->setFocus(Qt::ActiveWindowFocusReason);
}

void GraphicsOpenglWindowQt::grabFocusIfPointerInWindow()
{
  if(widget_->underMouse())
      widget_->setFocus(Qt::ActiveWindowFocusReason);
}

void GraphicsOpenglWindowQt::raiseWindow()
{
  if(widget_)
      widget_->raise();
}

void GraphicsOpenglWindowQt::setWindowName(const std::string& name)
{
  if(widget_)
      widget_->setWindowTitle(name.c_str());
}

void GraphicsOpenglWindowQt::setCursor(MouseCursor cursor)
{
  if(cursor==InheritCursor && widget_)
  {
    widget_->unsetCursor();
  }

  QCursor current_cursor;

  switch(cursor)
  {
  case NoCursor: current_cursor = Qt::BlankCursor; break;
  case RightArrowCursor: case LeftArrowCursor: current_cursor = Qt::ArrowCursor; break;
  case InfoCursor: current_cursor = Qt::SizeAllCursor; break;
  case DestroyCursor: current_cursor = Qt::ForbiddenCursor; break;
  case HelpCursor: current_cursor = Qt::WhatsThisCursor; break;
  case CycleCursor: current_cursor = Qt::ForbiddenCursor; break;
  case SprayCursor: current_cursor = Qt::SizeAllCursor; break;
  case WaitCursor: current_cursor = Qt::WaitCursor; break;
  case TextCursor: current_cursor = Qt::IBeamCursor; break;
  case CrosshairCursor: current_cursor = Qt::CrossCursor; break;
  case HandCursor: current_cursor = Qt::OpenHandCursor; break;
  case UpDownCursor: current_cursor = Qt::SizeVerCursor; break;
  case LeftRightCursor: current_cursor = Qt::SizeHorCursor; break;
  case TopSideCursor: case BottomSideCursor: current_cursor = Qt::UpArrowCursor; break;
  case LeftSideCursor: case RightSideCursor: current_cursor = Qt::SizeHorCursor; break;
  case TopLeftCorner: current_cursor = Qt::SizeBDiagCursor; break;
  case TopRightCorner: current_cursor = Qt::SizeFDiagCursor; break;
  case BottomRightCorner: current_cursor = Qt::SizeBDiagCursor; break;
  case BottomLeftCorner: current_cursor = Qt::SizeFDiagCursor; break;
  default: break;
  };
  if(widget_) widget_->setCursor(current_cursor);
}

void GraphicsOpenglWindowQt::requestWarpPointer(float x,float y)
{
  if(widget_)
      QCursor::setPos(widget_->mapToGlobal(QPoint((int)x,(int)y)));
}



