#include "draw_line.hpp"

#include <iostream>
#include <osg/LineWidth>

DrawLineHandler::DrawLineHandler(DrawLine* line)
  : line_(line)
  , is_first_point_finish_(false)
{

}

bool DrawLineHandler::handle(
  const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
  if (!line_->is_activate())
  {
    return false;
  }

  osgViewer::Viewer* view = dynamic_cast<osgViewer::Viewer*>(&aa);
  if(!view)
  {
    return false;
  }

  if (ea.getEventType()  == osgGA::GUIEventAdapter::PUSH &&
      ea.getButtonMask() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
  {
    if(is_first_point_finish_ && pickpoint(view,ea,point2_))
    {
      line_->update(point1_,point2_);
      is_first_point_finish_ = false;
    }
    else if(pickpoint(view,ea,point1_))
    {
      is_first_point_finish_ = true;
    }
    return true;
  }

  if ((ea.getEventType()  == osgGA::GUIEventAdapter::DRAG ||
      ea.getEventType()  == osgGA::GUIEventAdapter::MOVE) &&
      is_first_point_finish_ && pickpoint(view,ea,point2_))
  {
    line_->update(point1_,point2_);
    return true;
  }  

  return false;
}

bool DrawLineHandler::pickpoint(
  osgViewer::Viewer* view,
  const osgGA::GUIEventAdapter& ea,
  ::osg::Vec3& point)
{
  osgUtil::LineSegmentIntersector::Intersections intersections;

  if(view->computeIntersections(ea,intersections))
  {
    for(const auto& interactor : intersections)
    {
      for (const auto& node : interactor.nodePath)
      {
        if (node->getName() == "LODTileMesh")
        {
          point =  interactor.localIntersectionPoint;
          return true;
        }
        
      }
    }
  }
  return false;
}


DrawLine::DrawLine()
  : is_activate_(false)
  , line_width_(3)
  , root_(new ::osg::Group)
  , geode_(new ::osg::Geode)
  , geom_(new ::osg::Geometry)
  , vertices_(new ::osg::Vec3Array)
  , color_(new ::osg::Vec4Array)
{
  geode_->setDataVariance(::osg::Object::DYNAMIC);
  stateset_ = geode_->getOrCreateStateSet();
  stateset_->setAttribute(new ::osg::LineWidth(line_width_));
  color_->push_back(::osg::Vec4(1.0f,0.0,0.0f,1.0f));
  geom_->setColorArray(color_,::osg::Array::BIND_OVERALL);
  geom_->addPrimitiveSet(new ::osg::DrawArrays(GL_LINES,0,2));

//   vertices_->resize(2,::osg::Vec3());
//   vertices_->push_back(osg::Vec3(-400,-192,180));
//   vertices_->push_back(osg::Vec3(200,-192,180));

//   geom_->setVertexArray(vertices_);

  geode_->addDrawable(geom_);
  root_->addChild(geode_);

}

::osg::Group* DrawLine::root()
{
  return root_;
}

void DrawLine::update(::osg::Vec3 p1, ::osg::Vec3 p2)
{
  vertices_->clear();
  vertices_->push_back(p1);
  vertices_->push_back(p2);
  geom_->setVertexArray(vertices_);
}

void DrawLine::set_color()
{

}

void DrawLine::set_is_activate(bool val)
{
  is_activate_ = val;
}

bool DrawLine::is_activate()
{
  return is_activate_;
}
