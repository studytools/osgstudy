#include "mainwindow.hpp"

#include <QMessageBox>
#include <QFileDialog>

namespace hs
{
namespace graphics
{
namespace example
{

MainWindow::MainWindow(QWidget *parent,Qt::WindowFlags flags)
  : QMainWindow(parent,flags)
{

  setMinimumSize(800,600);
  CreateActions();
  CreateMenus();
  CreateToolBar();
  CreateStatusBar();

  viewer_widget_ = new ViewerWidget(this);
  setCentralWidget(viewer_widget_);
}

void MainWindow::CreateActions()
{
  action_open_ = new QAction(QIcon(":/resource/open"),tr("&Open"),this);
  action_open_->setShortcuts(QKeySequence::Open);
  action_open_->setStatusTip(tr("Open an existing file"));
  connect(action_open_,&QAction::triggered,this,&MainWindow::Open);

  action_new_ = new QAction(QIcon(":/resource/new"),tr("&New"),this);
  action_new_->setShortcuts(QKeySequence::Open);
  action_new_->setStatusTip(tr("Open an existing file"));
  connect(action_new_,&QAction::triggered,this,&MainWindow::New);

  action_exit_ = new QAction(tr("E&xit"),this);
  action_exit_->setShortcuts(QKeySequence::Quit);
  action_exit_->setStatusTip(tr("Exit the application"));
  connect(action_exit_,&QAction::triggered,this,&MainWindow::close);

  action_about_ = new QAction(QIcon(":/resource/info"),tr("&About"),this);
  action_about_->setStatusTip(tr("Show the application's About box"));
  connect(action_about_,&QAction::triggered,this,&MainWindow::About);

}

void MainWindow::CreateMenus()
{
  menu_file_ = menuBar()->addMenu(tr("&File"));
  menu_file_->addAction(action_open_);
  menu_file_->addAction(action_new_);
  menu_file_->addSeparator();
  menu_file_->addAction(action_exit_);

  menu_render_ = menuBar()->addMenu(tr("Render"));

  menuBar()->addSeparator();
  menu_help_ = menuBar()->addMenu(tr("&Help"));
  menu_help_->addAction(action_about_);
}

void MainWindow::CreateToolBar()
{
  toolbar_file_ = addToolBar(tr("File"));
  toolbar_file_->addAction(action_open_);
  toolbar_file_->addAction(action_new_);
  toolbar_file_->addSeparator();
}

void MainWindow::CreateStatusBar()
{
  statusBar()->showMessage(tr("Ready"));
}

void MainWindow::Open()
{
  QString file_name = QFileDialog::getOpenFileName(this,tr("Open Model File"),
    ".",tr("mesh files (*.bin *osg *osgb);;  \
    ply files (*.ply);;  \
    obj files (*.obj);;  \
    All files (*.*)"));

  if(file_name.isEmpty())
    return;
}

void MainWindow::New()
{
}

void MainWindow::About()
{
  QMessageBox::about(this,tr("About Application"),
    tr("The <b>Application</b> osg example."));
}

}//namespace example
}
}
