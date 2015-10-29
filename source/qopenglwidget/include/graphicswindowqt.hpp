#ifndef _OSG_STUDY_GRAPHICSWINDOWQT_HPP_
#define _OSG_STUDY_GRAPHICSWINDOWQT_HPP_

#include "OSG_Study/config/hs_config.hpp"

#include <osgViewer/GraphicsWindow>

class QInputEvent;

namespace osgQt
{

  class GraphicsOpenglWindowQt: public osgViewer::GraphicsWindow
  {
  public:
    GraphicsOpenglWindowQt(osg::GraphicsContext::Traits* traits, QWidget* parent = NULL, const QGLWidget* shareWidget = NULL, Qt::WindowFlags f = 0 );
    ~GraphicsOpenglWindowQt();
  };
}

#endif
