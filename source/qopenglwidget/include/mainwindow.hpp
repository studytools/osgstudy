#ifndef _MAIN_WINDOW_HPP_
#define _MAIN_WINDOW_HPP_

#include <QMainWindow>
#include <QWidget>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>

#include "qopenglwidget.hpp"

namespace hs
{
namespace graphics
{
namespace example
{

class MainWindow: public QMainWindow
{
  Q_OBJECT
public:
  typedef osgQt::OSGOpenglWidget ViewerWidget;

public:
  MainWindow(QWidget* parent = 0,Qt::WindowFlags flags = 0);

  private slots:
  void Open();
  void New();
  void About();

private:
  void CreateActions();
  void CreateMenus();
  void CreateToolBar();
  void CreateStatusBar();

private:
  QMenu *menu_file_;
  QMenu *menu_render_;
  QMenu *menu_help_;
  QAction *action_open_;
  QAction *action_new_;
  QAction *action_exit_;
  QAction *action_about_;
  QToolBar *toolbar_file_;

  ViewerWidget* viewer_widget_;
};

}
}
}


#endif
