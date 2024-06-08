#include "opening.h"
#include "../cmake-build-debug/cbr_autogen/include/ui_opening.h"
#include "main_window.h"

namespace ar
{
  Opening::Opening(ar::MainWindow& main_window, QWidget* parent) :
    QWidget(parent), ui(new Ui::Opening), main_window_{main_window}
  {
    ui->setupUi(this);

    connect(ui->pushButton_2, &QPushButton::clicked, this, [&](bool val) {
      main_window_.show();
      close();
    });

    connect(ui->pushButton, &QPushButton::clicked, this, [&](bool) {
      main_window_.show();
      close();
    });
  }

  Opening::~Opening()
  {
    delete ui;
  }
} // ar
