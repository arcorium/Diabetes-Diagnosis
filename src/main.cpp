#include <expected>
#include <iostream>

#include <QApplication>
#include <QPushButton>
#include <QLocalSocket>

#include "ui/main_window.h"
#include "ui/opening.h"

int main(int argc, char* argv[])
{
  using namespace Qt::Literals;

  QApplication app(argc, argv);
  app.setStyle("fusion");

  ar::MainWindow main_window{"/tmp/cbr_socket.s"};
  ar::Opening opening{main_window};

  opening.show();

  return app.exec();
}
