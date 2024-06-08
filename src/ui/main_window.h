#pragma once

#include <QMainWindow>
#include <QLocalSocket>
#include <ui_main_window.h>

#include "user.h"
#include "message/payload.h"

namespace ar
{
  class MainWindow : public QMainWindow
  {
    Q_OBJECT

  public:
    explicit MainWindow(const QString& host, QWidget* parent = nullptr);
    ~MainWindow() override;

  private:
    void setup_connection() noexcept;
    void clear_forms() noexcept;

    void add_data_test(DataResponse::Data&& data, size_t i = std::numeric_limits<size_t>::max()) noexcept;
    void add_diagnosed_data(UserDiagnosed&& data, size_t i = std::numeric_limits<size_t>::max()) noexcept;

    // slot
    void on_bmi_value_changed(double) const noexcept;
    void on_family_diseases_changed(int) const noexcept;

    void on_new_data() noexcept;
    void on_error_data(const QLocalSocket::LocalSocketError& err) noexcept;
    void on_start_diagnosis() noexcept;

    void on_tab_changed(int index) noexcept;

  private:
    bool wait_reply_;
    Ui::MainWindow ui_;
    QLocalSocket* socket_;

    std::vector<UserDiagnosed> diagnosed_users_;
    std::vector<DataResponse::Data> data_test_;
  };
} // ar
