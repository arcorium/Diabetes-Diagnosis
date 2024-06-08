#include "main_window.h"

#include <QMessageBox>

#include <iostream>
#include <ranges>

#include "bmi.h"
#include "diagnosis_result.h"
#include "ui_main_window.h"
#include "ui_diagnosis_result.h"
#include "message/payload.h"

namespace ar
{
  using namespace Qt::StringLiterals;

  MainWindow::MainWindow(const QString& host, QWidget* parent)
    : QMainWindow(parent), wait_reply_{false}, socket_{new QLocalSocket{this}}
  {
    ui_.setupUi(this);

    QStringList header_labels = {
      "Pregnancies", "Glucose", "Blood Pressure", "Skin Thickness", "Insulin", "BMI", "DPF", "Age",
      "Result"
    };
    ui_.tableWidget->setColumnCount(header_labels.size());
    ui_.tableWidget->setHorizontalHeaderLabels(header_labels);
    QStringList diagnosed_header_labels = {
      "Name", "Age", "Gender", "Pregnancies", "Glucose", "Blood Pressure", "Skin Thickness", "Insulin", "BMI",
      "DPF",
      "Result"
    };
    ui_.result_table->setColumnCount(diagnosed_header_labels.size());
    ui_.result_table->setHorizontalHeaderLabels(diagnosed_header_labels);

    ui_.spinbox_pregnancies->setDisabled(true);
    ui_.spinbox_pregnancies->setMinimum(0);
    ui_.spinbox_pregnancies->setMaximum(24);
    ui_.checkbox_pregnant->setDisabled(ui_.combobox_gender->currentIndex() == 0);
    ui_.dspinbox_weight->setMaximum(std::numeric_limits<double>::max());
    ui_.spinbox_height->setMaximum(std::numeric_limits<int>::max());

    ui_.spinbox_glucose->setMinimum(0);
    ui_.spinbox_glucose->setMaximum(std::numeric_limits<int>::max());
    ui_.spinbox_blood->setMinimum(0);
    ui_.spinbox_blood->setMaximum(std::numeric_limits<int>::max());
    ui_.spinbox_skin->setMinimum(0);
    ui_.spinbox_skin->setMaximum(std::numeric_limits<int>::max());
    ui_.spinbox_insulin->setMinimum(0);
    ui_.spinbox_insulin->setMaximum(std::numeric_limits<int>::max());

    ui_.spinbox_pregnancies->setValue(6);
    ui_.spinbox_glucose->setValue(148);
    ui_.spinbox_blood->setValue(72);
    ui_.spinbox_skin->setValue(35);
    ui_.spinbox_insulin->setValue(0);
    ui_.dspinbox_bmi->setValue(33.6);
    ui_.dspinbox_dpf->setValue(0.627);
    ui_.spinbox_age->setValue(50);

    setup_connection();

    socket_->connectToServer(host);
  }

  MainWindow::~MainWindow()
  {
  }

  void MainWindow::setup_connection() noexcept
  {
    // Socket
    connect(socket_, &QLocalSocket::readyRead, this, &MainWindow::on_new_data);
    connect(socket_, &QLocalSocket::errorOccurred, this, &MainWindow::on_error_data);
    connect(socket_, &QLocalSocket::connected, this, [&] {
      std::cout << "Connected" << std::endl;
      auto msg = create_message(DataPayload{});
      auto header_msg = msg.header.serialize();
      socket_->write(header_msg);
      socket_->write(msg.body);
      wait_reply_ = true;
    });

    // Tab
    connect(ui_.tabWidget, &QTabWidget::tabBarClicked, this, &MainWindow::on_tab_changed);

    // Pregnancy
    connect(ui_.checkbox_pregnant, &QCheckBox::clicked, this, [&](bool checked) {
      ui_.spinbox_pregnancies->setDisabled(!checked);
      ui_.label_pregnancies->setDisabled(!checked);
      if (!checked)
        ui_.spinbox_pregnancies->setValue(0);
    });

    // BMI
    connect(ui_.spinbox_height, &QSpinBox::valueChanged, this, &MainWindow::on_bmi_value_changed);
    connect(ui_.dspinbox_weight, &QDoubleSpinBox::valueChanged, this, &MainWindow::on_bmi_value_changed);

    // DPF
    connect(ui_.spinbox_grandparents, &QSpinBox::valueChanged, this,
            &MainWindow::on_family_diseases_changed);
    connect(ui_.spinbox_parents, &QSpinBox::valueChanged, this, &MainWindow::on_family_diseases_changed);
    connect(ui_.spinbox_siblings, &QSpinBox::valueChanged, this, &MainWindow::on_family_diseases_changed);

    // Start diagnosis button
    connect(ui_.btn_start_diagnosis, &QPushButton::clicked, this, &MainWindow::on_start_diagnosis);

    // Clear button
    connect(ui_.btn_clear, &QPushButton::clicked, this, &MainWindow::clear_forms);

    // Gender
    connect(ui_.combobox_gender, &QComboBox::currentIndexChanged, this, [&](int index) {
      ui_.checkbox_pregnant->setDisabled(index == 0);
    });
  }

  void MainWindow::clear_forms() noexcept
  {
    ui_.line_name->clear();
    ui_.line_address->clear();
    ui_.spinbox_age->setValue(0);
    ui_.checkbox_pregnant->setCheckState(Qt::CheckState::Unchecked);
    ui_.dspinbox_weight->setValue(0);
    ui_.spinbox_height->setValue(0);
    ui_.spinbox_pregnancies->setValue(0);
    ui_.spinbox_pregnancies->setDisabled(true);
    ui_.spinbox_glucose->setValue(0);
    ui_.spinbox_blood->setValue(0);
    ui_.spinbox_skin->setValue(0);
    ui_.spinbox_insulin->setValue(0);
    ui_.dspinbox_bmi->setValue(0.0);
    ui_.dspinbox_dpf->setValue(0.0);
    ui_.spinbox_grandparents->setValue(0);
    ui_.spinbox_parents->setValue(0);
    ui_.spinbox_siblings->setValue(0);
  }

  void MainWindow::add_data_test(DataResponse::Data&& data, size_t i) noexcept
  {
    auto pregnancies = new QTableWidgetItem{QString::number(data.pregnancies)};
    auto glucose = new QTableWidgetItem{QString::number(data.glucose)};
    auto blood = new QTableWidgetItem{QString::number(data.blood_pressure)};
    auto skin = new QTableWidgetItem{QString::number(data.skin_thickness)};
    auto insulin = new QTableWidgetItem{QString::number(data.insulin)};
    auto bmi = new QTableWidgetItem{QString::number(data.bmi)};
    auto dpf = new QTableWidgetItem{QString::number(data.dpf)};
    auto age = new QTableWidgetItem{QString::number(data.age)};
    auto result = new QTableWidgetItem{QString::number(data.result)};

    data_test_.push_back(std::forward<DataResponse::Data>(data));

    if (data_test_.size() == 1)
      ui_.tableWidget->setRowCount(1); // Create empty row
    else
      ui_.tableWidget->insertRow(data_test_.size() - 1);

    if (i == std::numeric_limits<size_t>::max())
      i = data_test_.size() - 1;

    ui_.tableWidget->setItem(i, 0, pregnancies);
    ui_.tableWidget->setItem(i, 1, glucose);
    ui_.tableWidget->setItem(i, 2, blood);
    ui_.tableWidget->setItem(i, 3, skin);
    ui_.tableWidget->setItem(i, 4, insulin);
    ui_.tableWidget->setItem(i, 5, bmi);
    ui_.tableWidget->setItem(i, 6, dpf);
    ui_.tableWidget->setItem(i, 7, age);
    ui_.tableWidget->setItem(i, 8, result);
  }

  void MainWindow::add_diagnosed_data(UserDiagnosed&& data, size_t i) noexcept
  {
    auto name = new QTableWidgetItem{QString::fromStdString(data.name)};
    auto gender = new QTableWidgetItem{QString::fromStdString(std::string{gender_str(data.gender)})};
    auto pregnancies = new QTableWidgetItem{QString::number(data.pregnancies)};
    auto glucose = new QTableWidgetItem{QString::number(data.glucose)};
    auto blood = new QTableWidgetItem{QString::number(data.blood_pressure)};
    auto skin = new QTableWidgetItem{QString::number(data.skin_thickness)};
    auto insulin = new QTableWidgetItem{QString::number(data.insulin)};
    auto bmi = new QTableWidgetItem{QString::number(data.bmi)};
    auto dpf = new QTableWidgetItem{QString::number(data.dpf)};
    auto age = new QTableWidgetItem{QString::number(data.age)};
    auto result = new QTableWidgetItem{QString::number(data.result)};

    diagnosed_users_.push_back(std::forward<UserDiagnosed>(data));

    if (diagnosed_users_.size() == 1)
      ui_.result_table->setRowCount(1); // Create empty row
    else
      ui_.result_table->insertRow(diagnosed_users_.size() - 1);


    if (i == std::numeric_limits<size_t>::max())
      i = diagnosed_users_.size() - 1;

    ui_.result_table->setItem(i, 0, name);
    ui_.result_table->setItem(i, 1, age);
    ui_.result_table->setItem(i, 2, gender);
    ui_.result_table->setItem(i, 3, pregnancies);
    ui_.result_table->setItem(i, 4, glucose);
    ui_.result_table->setItem(i, 5, blood);
    ui_.result_table->setItem(i, 6, skin);
    ui_.result_table->setItem(i, 7, insulin);
    ui_.result_table->setItem(i, 8, bmi);
    ui_.result_table->setItem(i, 9, dpf);
    ui_.result_table->setItem(i, 10, result);
  }

  void MainWindow::on_bmi_value_changed(double) const noexcept
  {
    auto height = ui_.spinbox_height->value();
    if (height == 0.0)
      return;

    auto height_meter = static_cast<float>(height) / 100.f;

    auto bmi = calculate_bmi(ui_.dspinbox_weight->value(), height_meter);
    ui_.dspinbox_bmi->setValue(bmi);
  }

  void MainWindow::on_family_diseases_changed(int) const noexcept
  {
    std::array values{
      ValueWeight<uint8_t>{ui_.spinbox_grandparents->value(), 0.25},
      ValueWeight<uint8_t>{ui_.spinbox_parents->value(), 0.0625},
      ValueWeight<uint8_t>{ui_.spinbox_siblings->value(), 0.125},
    };

    auto dpf = calculate_dpf(values);
    ui_.dspinbox_dpf->setValue(dpf);
  }

  void MainWindow::on_new_data() noexcept
  {
    if (!wait_reply_)
      return;
    wait_reply_ = false;

    auto packet = socket_->readAll();
    auto type = static_cast<MessageType>(packet[0]);
    auto size = *reinterpret_cast<uint32_t*>(&packet[1]);
    auto payload = packet.sliced(5);
    if (payload.size() != size)
      std::cout << "Payload has different size with header body_size field\n";

    switch (type)
    {
    case MessageType::Diagnose:
      {
        auto deser = DiagnoseResponse::deserialize(std::move(payload));
        if (!deser)
        {
          std::cout << deser.error() << std::endl;
          break;
        }

        if (deser->index >= data_test_.size())
        {
          std::cout << "Index exceed the maximum data index" << std::endl;
          break;
        }

        // Save in result and data test
        GenderType gender = GenderType::Male;
        if (ui_.combobox_gender->currentIndex() == 1)
          gender = GenderType::Female;

        UserDiagnosed user_diagnosed{
          ui_.line_name->text().toStdString(),
          ui_.line_address->text().toStdString(),
          gender,
          ui_.spinbox_age->value(),
          ui_.spinbox_pregnancies->value(),
          ui_.spinbox_blood->value(),
          ui_.spinbox_skin->value(),
          ui_.spinbox_insulin->value(),
          ui_.spinbox_glucose->value(),
          ui_.dspinbox_bmi->value(),
          ui_.dspinbox_dpf->value(),
          deser->predict,
        };

        add_diagnosed_data(std::move(user_diagnosed));

        DataResponse::Data new_data_test{
          ui_.spinbox_pregnancies->value(),
          ui_.spinbox_blood->value(),
          ui_.spinbox_skin->value(),
          ui_.spinbox_age->value(),
          deser->predict,
          ui_.dspinbox_bmi->value(),
          ui_.dspinbox_dpf->value(),
          ui_.spinbox_insulin->value(),
          ui_.spinbox_glucose->value()
        };
        add_data_test(std::move(new_data_test));

        // Show result
        auto dialogue = new DiagnosisResult{this};
        dialogue->ui()->line_test_id->setText(QString::number(deser->index));
        dialogue->ui()->line_similarity->setText(QString::number(deser->similarity) + " %");
        if (!deser->predict)
          dialogue->ui()->label->setText("You Don't Have");

        auto& data = data_test_[deser->index];
        dialogue->ui()->spinbox_age->setValue(data.age);
        dialogue->ui()->spinbox_pregnancies->setValue(data.pregnancies);
        dialogue->ui()->spinbox_glucose->setValue(data.glucose);
        dialogue->ui()->spinbox_blood->setValue(data.blood_pressure);
        dialogue->ui()->spinbox_skin->setValue(data.skin_thickness);
        dialogue->ui()->spinbox_insulin->setValue(data.insulin);
        dialogue->ui()->spinbox_bmi->setValue(data.bmi);
        dialogue->ui()->spinbox_dpf->setValue(data.dpf);
        dialogue->show();
        break;
      }
    case MessageType::Data:
      {
        auto deser = DataResponse::deserialize(std::move(payload));
        data_test_ = std::move(deser.data);
        ui_.tableWidget->setRowCount(data_test_.size());

        // Show on data tab
        for (const auto& [i, data] : data_test_ | std::views::enumerate)
        {
          // auto model = ui_.tableWidget->model();
          auto pregnancies = new QTableWidgetItem{QString::number(data.pregnancies)};
          auto glucose = new QTableWidgetItem{QString::number(data.glucose)};
          auto blood = new QTableWidgetItem{QString::number(data.blood_pressure)};
          auto skin = new QTableWidgetItem{QString::number(data.skin_thickness)};
          auto insulin = new QTableWidgetItem{QString::number(data.insulin)};
          auto bmi = new QTableWidgetItem{QString::number(data.bmi)};
          auto dpf = new QTableWidgetItem{QString::number(data.dpf)};
          auto age = new QTableWidgetItem{QString::number(data.age)};
          auto result = new QTableWidgetItem{QString::number(data.result)};

          ui_.tableWidget->setItem(i, 0, pregnancies);
          ui_.tableWidget->setItem(i, 1, glucose);
          ui_.tableWidget->setItem(i, 2, blood);
          ui_.tableWidget->setItem(i, 3, skin);
          ui_.tableWidget->setItem(i, 4, insulin);
          ui_.tableWidget->setItem(i, 5, bmi);
          ui_.tableWidget->setItem(i, 6, dpf);
          ui_.tableWidget->setItem(i, 7, age);
          ui_.tableWidget->setItem(i, 8, result);
        }
        break;
      }
    }
  }

  void MainWindow::on_error_data(const QLocalSocket::LocalSocketError& err) noexcept
  {
    std::cout << err << std::endl;
  }

  void MainWindow::on_start_diagnosis() noexcept
  {
    // Check field
    if (ui_.line_name->text().isEmpty()
      || ui_.line_address->text().isEmpty()
      || ui_.spinbox_age->value() == 0
      || ui_.dspinbox_bmi->value() == 0.0
      || ui_.dspinbox_weight->value() == 0.0
      || ui_.spinbox_height->value() == 0
      || ui_.dspinbox_dpf->value() == 0.0)
    {
      QMessageBox msg_box{};
      msg_box.setWindowTitle(u"Diagnosis Failed"_s);
      msg_box.setText(u"Failed to start diagnosis due to some fields are empty"_s);
      msg_box.exec();
      return;
    }

    // Send data to server
    if (wait_reply_)
      return;

    wait_reply_ = true;

    DiagnosePayload payload{
      .age = ui_.spinbox_age->value(),
      .pregnancies = ui_.spinbox_pregnancies->value(),
      .blood_pressure = ui_.spinbox_blood->value(),
      .skin_thickness = ui_.spinbox_skin->value(),
      .insulin = ui_.spinbox_insulin->value(),
      .glucose = ui_.spinbox_glucose->value(),
      .bmi = ui_.dspinbox_bmi->value(),
      .dpf = ui_.dspinbox_dpf->value()
    };

    auto msg = create_message(std::move(payload));
    auto header = msg.header.serialize();

    socket_->write(header);
    socket_->write(msg.body);
  }

  void MainWindow::on_tab_changed(int index) noexcept
  {
    // 0 = Diagnose
    // 1 = Result
    // 2 = Data
  }
} // ar
