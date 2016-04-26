#ifndef _HS_GRAPHICS_GRAPHICS_OSG_DRAW_LINE_HPP_
#define _HS_GRAPHICS_GRAPHICS_OSG_DRAW_LINE_HPP_

#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Array>
#include <osg/NodeCallback>
#include <osg/Plane>

#include <osgGA/GUIEventHandler>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>

#include "event_capture.hpp"
#include "osgterrain.hpp"

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
  void set_terrain(DEMTerrain* dem);

private:
  void update(::osg::Vec3 p1,::osg::Vec3 p2);
  void ComputeDEMPathVertices(const osg::Vec3& startPt,const osg::Vec3& endPt);

private:
  bool is_first_point_finish_;
  float line_width_;
  ::osg::Vec3 point_start_;
  ::osg::Geode* geode_;
  ::osg::StateSet* stateset_;
  ::osg::Geometry* geom_;
  ::osg::Vec3Array* vertices_;
  ::osg::Vec3Array* dem_path_vertices_;
  ::osg::Vec4Array* color_;
  Intersections intersections_;

  DEMTerrain* dem_;
};

#endif
