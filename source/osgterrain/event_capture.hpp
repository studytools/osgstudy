#ifndef _HS_GRAPHICS_GRAPHICS_OSG_EVENTCAPTURE_HPP_
#define _HS_GRAPHICS_GRAPHICS_OSG_EVENTCAPTURE_HPP_

#include <cstring>

#include <osg/NodeVisitor>
#include <osgViewer/Viewer>

#include <osgGA/GUIEventHandler>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>

class EventCapture;

class EventCaptureHandler : public osgGA::GUIEventHandler
{
public:
  EventCaptureHandler(EventCapture* event_capture);

  bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

private:
  EventCapture* event_capture_;
};

class GetNameVisitor : public ::osg::NodeVisitor
{
public:
  GetNameVisitor(const std::string& name);
  virtual void apply(::osg::Geode& node);
  virtual void apply(::osg::Group& node);

  const ::osg::Node* node() const;
  bool found() const;

protected:
private:
  std::string name_;
  bool found_;
  ::osg::Node* node_;
};

class EventCapture
{
public:
  typedef unsigned int NodeMask;

  EventCapture(const std::string& node_name,NodeMask node_mask = 0xff);

  void set_is_activate(bool val = true);
  bool is_activate() const;

  virtual ::osg::Node* root();
  virtual bool EventResponse(osgViewer::Viewer* view,
    const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) { return false; }

protected:
  ::osg::Group* root_;
  std::string node_name_;
  NodeMask node_mask_;

private:
  bool is_activate_;

};

#endif

