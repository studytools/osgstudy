#include <QApplication>
#include <QSurfaceFormat>

#include "mainwindow.hpp"

int main(int argc, char **argv)
{
  QApplication app(argc, argv);
//   QSurfaceFormat format;
//   if (QCoreApplication::arguments().contains(QLatin1String("--multisample")))
//     format.setSamples(4);
//   if (QCoreApplication::arguments().contains(QLatin1String("--coreprofile"))) {
//     format.setVersion(3, 3);
//     format.setProfile(QSurfaceFormat::CoreProfile);
//   }
//   format.setDepthBufferSize(24);
//   format.setStencilBufferSize(8);
//   QSurfaceFormat::setDefaultFormat(format);

  hs::graphics::example::MainWindow mw;
  mw.showMaximized();

  return app.exec();
}
