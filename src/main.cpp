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

  QApplication a(argc, argv);
  // QApplication::setStyle("Fusion");

  ar::MainWindow main_window{"/tmp/cbr_socket.s"};
  ar::Opening opening{main_window};

  // Create connection


  opening.show();

  return QApplication::exec();
}
