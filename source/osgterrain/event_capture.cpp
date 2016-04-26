#include "event_capture.hpp"

EventCaptureHandler::EventCaptureHandler(EventCapture* event_capture)
  : event_capture_(event_capture)
{
}

bool EventCaptureHandler::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa)
{
  if (!event_capture_->is_activate())
    return false;
  
  osgViewer::Viewer* view = dynamic_cast<osgViewer::Viewer*>(&aa);
  if(!view)
    return false;

  return event_capture_->EventResponse(view,ea,aa);
}

GetNameVisitor::GetNameVisitor(const std::string& name)
  : ::osg::NodeVisitor(TRAVERSE_ALL_CHILDREN)
  , name_(name)
  , found_(false)
  , node_(nullptr)
{
}
void GetNameVisitor::apply(::osg::Geode& node)
{
  if (node.getName() == name_)
  {
    node_ = dynamic_cast<::osg::Node*>(&node);
    found_ = true;
  }
  else
    traverse(node);
}
void GetNameVisitor::apply(::osg::Group& node)
{
  if(node.getName() == name_)
  {
    node_ = dynamic_cast<::osg::Node*>(&node);
    found_ = true;
  }
  else
    traverse(node);
}

const ::osg::Node* GetNameVisitor::node() const
{
  return node_;
}

bool GetNameVisitor::found() const
{
  return found_;
}

EventCapture::EventCapture(const std::string& node_name,NodeMask node_mask)
  : is_activate_(false)
  , root_(new ::osg::Group)
  , node_name_(node_name)
  , node_mask_(node_mask)
{
  root_->setName(node_name_);
  root_->setNodeMask(node_mask_);
}

::osg::Node* EventCapture::root()
{
  return root_;
}

void EventCapture::set_is_activate(bool val)
{
  is_activate_ = val;
}

bool EventCapture::is_activate() const
{
  return is_activate_;
}
