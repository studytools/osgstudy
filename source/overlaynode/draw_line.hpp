#ifndef _HS_GRAPHICS_EXAMPLE_GRAPHICS_OSG_DRAW_LINE_HPP_
#define _HS_GRAPHICS_EXAMPLE_GRAPHICS_OSG_DRAW_LINE_HPP_

#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Array>
#include <osg/BufferObject>
#include <osgViewer/Viewer>
#include <osgSim/OverlayNode>

#include <osgGA/GUIEventHandler>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>

class DrawLine;

class DrawLineHandler : public osgGA::GUIEventHandler
{
public:
  DrawLineHandler(DrawLine* line);

  bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

private:
   bool pickpoint(
    osgViewer::Viewer* view,
    const osgGA::GUIEventAdapter& ea,
    ::osg::Vec3& point);

private:
  DrawLine* line_;
  bool is_first_point_finish_;
  ::osg::Vec3 point1_;
  ::osg::Vec3 point2_;

};


class DrawLine
{
public:
  DrawLine();

  ::osg::Group* root();

  void update(::osg::Vec3 p1, ::osg::Vec3 p2);
  void set_color();
  void set_is_activate(bool val = true);
  bool is_activate();

private:
  bool is_activate_;
  float line_width_;
  ::osg::Vec3 point_start_;
  ::osg::Vec3 point_end_;
  ::osg::Group* root_;
  ::osg::Geode* geode_;
  ::osg::StateSet* stateset_;
  ::osg::Geometry* geom_;
  ::osg::Vec3Array* vertices_;
  ::osg::Vec4Array* color_;

};

#endif
