#ifndef _OSG_STUDY_GRAPHICSWINDOWQT_HPP_
#define _OSG_STUDY_GRAPHICSWINDOWQT_HPP_

#include "OSG_Study/config/hs_config.hpp"

#include "qopenglwidget.hpp"

#include <osgViewer/Viewer>

class GraphicsOpenglWindowQt: public OpenglWidget, public osgViewer::Viewer
{
public:
  GraphicsOpenglWindowQt(QWidget* parent = 0);
  ~GraphicsOpenglWindowQt();

protected:
  virtual void initializeGL();
  virtual void paintGL();
  virtual void timerEvent(QTimerEvent *event);

private:
  void createCamera(int x, int y, int w, int h);

private:
  osg::ref_ptr<osg::Group> root_;

};


#endif
