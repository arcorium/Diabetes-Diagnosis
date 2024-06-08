#ifndef OPENING_H
#define OPENING_H

#include <QWidget>

namespace ar
{
  QT_BEGIN_NAMESPACE

  namespace Ui
  {
    class Opening;
  }

  QT_END_NAMESPACE

  class MainWindow;

  class Opening : public QWidget
  {
    Q_OBJECT

  public:
    explicit Opening(ar::MainWindow& main_window, QWidget* parent = nullptr);
    ~Opening() override;

  private:
    Ui::Opening* ui;
    MainWindow& main_window_;
  };
} // ar

#endif //OPENING_H
