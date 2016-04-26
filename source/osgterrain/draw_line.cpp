#include "draw_line.hpp"

#include <iostream>
#include <iomanip>
#include <osg/LineWidth>
#include <osg/Point>

#include <osgDB/ReadFile>

#include <osgUtil/PlaneIntersector>

DrawLine::DrawLine(NodeMask node_mask)
  : EventCapture("DrawLine",node_mask)
  , line_width_(4)
  , geom_(new ::osg::Geometry)
  , geode_(new ::osg::Geode)
  , vertices_(new ::osg::Vec3Array)
  , dem_path_vertices_(new osg::Vec3Array)
  , color_(new ::osg::Vec4Array)
  , is_first_point_finish_(false)
{
  geode_->setDataVariance(::osg::Object::DYNAMIC);
  stateset_ = geode_->getOrCreateStateSet();
  stateset_->setAttribute(new ::osg::LineWidth(line_width_));
  color_->push_back(::osg::Vec4(0.48f,0.4,0.93f,1.0f));
  geom_->setColorArray(color_,::osg::Array::BIND_OVERALL);
  geom_->addPrimitiveSet(new ::osg::DrawArrays(GL_LINES,0,2));
  geode_->addDrawable(geom_);
  root_->addChild(geode_);
}

void DrawLine::update(::osg::Vec3 p1, ::osg::Vec3 p2)
{
  vertices_->clear();
  vertices_->push_back(p1);
  vertices_->push_back(p2);
  geom_->setVertexArray(vertices_); 
}

void DrawLine::set_color(::osg::Vec4 color)
{
  color_->clear();
  color_->push_back(color);
  geom_->setColorArray(color_,::osg::Array::BIND_OVERALL);
}

void DrawLine::set_line_width(float line_width)
{
  line_width_ = line_width;
  stateset_->setAttribute(new ::osg::LineWidth(line_width_));
}

void DrawLine::set_terrain(DEMTerrain* dem)
{
  dem_ = dem;
}

void DrawLine::ComputeDEMPathVertices(
  const osg::Vec3& startPt,const osg::Vec3& endPt)
{
  osg::Plane plane;
  osg::Vec3d upVector(0.0,0.0,1.0);
  osg::Vec3f planeNormal = (endPt-startPt)^upVector;
  planeNormal.normalize();
  plane.set(planeNormal,startPt);

  osg::Vec3f startPlaneNormal = upVector^planeNormal;
  startPlaneNormal.normalize();

  osg::Polytope polytope;
  polytope.add(osg::Plane(startPlaneNormal,startPt));
  osg::Vec3f endPlaneNormal = planeNormal^upVector;
  endPlaneNormal.normalize();
  polytope.add(osg::Plane(endPlaneNormal,endPt));

  osg::ref_ptr<osgUtil::PlaneIntersector>intersector =
    new osgUtil::PlaneIntersector(plane,polytope);
  osgUtil::IntersectionVisitor iv(intersector.get());
  dem_->terrain_tile()->accept(iv);

  if(intersector->containsIntersections())
  {
    dem_path_vertices_->clear();
    osgUtil::PlaneIntersector::Intersections& intersections = 
      intersector->getIntersections();
    osgUtil::PlaneIntersector::Intersections::iterator itr;
    for(const auto& intersection : intersections)
    {
      for(const auto& polyline : intersection.polyline)
      {
        dem_path_vertices_->push_back(polyline* (*intersection.matrix));
      }
    }
  }
}

bool DrawLine::EventResponse(osgViewer::Viewer* view,
  const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa)
{
  if(ea.getEventType()  == osgGA::GUIEventAdapter::PUSH &&
     ea.getButtonMask() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
  {
    intersections_.clear();
    view->computeIntersections(ea,intersections_,node_mask_);

    if(intersections_.empty())
      return true;
    const auto& intersection = *intersections_.begin();
    osg::Vec3Array* linepoint = new osg::Vec3Array;

    if(is_first_point_finish_)
    {
      update(point_start_,intersection.getWorldIntersectPoint());
      ComputeDEMPathVertices(point_start_,intersection.getWorldIntersectPoint());
      is_first_point_finish_ = false;
    }
    else
    {
      point_start_ = intersection.getWorldIntersectPoint();
      is_first_point_finish_ = true;
    }
    return true;
  }

  if (is_first_point_finish_ && 
      (ea.getEventType() == osgGA::GUIEventAdapter::DRAG ||
       ea.getEventType() == osgGA::GUIEventAdapter::MOVE))
  {
    intersections_.clear();
    view->computeIntersections(ea,intersections_,node_mask_);
    if(intersections_.empty())
      return true;
    const auto& intersection = *intersections_.begin();
    update(point_start_,intersection.getWorldIntersectPoint());
    return true;
  }

  return false;
}

