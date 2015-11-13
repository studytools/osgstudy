#ifndef _OSG_STUDY_GRAPHICSWINDOWQT_HPP_
#define _OSG_STUDY_GRAPHICSWINDOWQT_HPP_

#include "OSG_Study/config/hs_config.hpp"

#include "qopenglwidget.hpp"
#include <osgViewer/GraphicsWindow>

class OpenglWidget;

class GraphicsOpenglWindowQt: public osgViewer::GraphicsWindow
{
public:
  GraphicsOpenglWindowQt(
    ::osg::GraphicsContext::Traits* traits,
    OpenglWidget* widget);
  ~GraphicsOpenglWindowQt();

private:
  bool init();

  virtual bool setWindowRectangleImplementation(int x,int y,int width,int height);
  virtual bool setWindowDecorationImplementation(bool windowDecoration);
  virtual void grabFocus();
  virtual void grabFocusIfPointerInWindow();
  virtual void raiseWindow();
  virtual void setWindowName(const std::string& name);
  virtual void setCursor(MouseCursor cursor);
  virtual void requestWarpPointer(float x,float y);

  // dummy implementations, assume that graphics context is *always* current and valid.
  virtual bool valid() const { return true; }
  virtual bool realizeImplementation() { return true; }
  virtual bool isRealizedImplementation() const  { return true; }
  virtual void closeImplementation() {}
  virtual bool makeCurrentImplementation() { return true; }
  virtual bool makeContextCurrentImplementation() { return true; }
  virtual bool releaseContextImplementation() { return true; }
  virtual void swapBuffersImplementation() {}

private:
  friend class OpenglWidget;
  OpenglWidget* widget_;

};


#endif
