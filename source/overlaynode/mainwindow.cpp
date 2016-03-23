#include "mainwindow.hpp"
#include "ui_widget.h"

MainWindow::MainWindow(QWidget *parent):
QMainWindow(parent),
    ui(new Ui::MyWidget)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}
