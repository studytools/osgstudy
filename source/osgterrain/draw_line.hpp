#ifndef _HS_GRAPHICS_GRAPHICS_OSG_DRAW_LINE_HPP_
#define _HS_GRAPHICS_GRAPHICS_OSG_DRAW_LINE_HPP_

#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Array>
#include <osg/NodeCallback>

#include <osgGA/GUIEventHandler>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>

#include "event_capture.hpp"

class DrawLine : public EventCapture
{
public:
  typedef osgUtil::LineSegmentIntersector::Intersections Intersections;
  DrawLine(NodeMask node_mask = 0xff);

  virtual bool EventResponse(osgViewer::Viewer* view,
  const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);

  void set_color(::osg::Vec4 color);
  void set_line_width(float line_width);
  void set_matrix(osg::Matrix m);

private:
  void update(::osg::Vec3 p1,::osg::Vec3 p2);

private:
  bool is_first_point_finish_;
  float line_width_;
  ::osg::Vec3 point_start_;
  ::osg::Geode* geode_;
  ::osg::StateSet* stateset_;
  ::osg::Geometry* geom_;
  ::osg::Vec3Array* vertices_;
  ::osg::Vec4Array* color_;
  Intersections intersections_;
};

#endif
