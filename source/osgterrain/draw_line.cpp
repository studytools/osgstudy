#include "draw_line.hpp"

#include <iostream>
#include <osg/LineWidth>

#include <osgDB/ReadFile>

DrawLine::DrawLine(NodeMask node_mask)
  : EventCapture("DrawLine",node_mask)
  , line_width_(4)
  , geom_(new ::osg::Geometry)
  , geode_(new ::osg::Geode)
  , vertices_(new ::osg::Vec3Array)
  , color_(new ::osg::Vec4Array)
  , is_first_point_finish_(false)
{
  geode_->setDataVariance(::osg::Object::DYNAMIC);
  stateset_ = geode_->getOrCreateStateSet();
  stateset_->setAttribute(new ::osg::LineWidth(line_width_));
  color_->push_back(::osg::Vec4(0.48f,0.4,0.93f,0.5f));
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

    if(is_first_point_finish_)
    {
      update(point_start_,intersection.getWorldIntersectPoint());
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

