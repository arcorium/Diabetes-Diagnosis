#ifndef DIAGNOSIS_RESULT_H
#define DIAGNOSIS_RESULT_H

#include <QDialog>

namespace ar
{
  QT_BEGIN_NAMESPACE

  namespace Ui
  {
    class DiagnosisResult;
  }

  QT_END_NAMESPACE

  class DiagnosisResult : public QDialog
  {
    Q_OBJECT

  public:
    explicit DiagnosisResult(QWidget* parent = nullptr);
    ~DiagnosisResult() override;

    Ui::DiagnosisResult* ui() noexcept;

  private:
    Ui::DiagnosisResult* ui_;
  };
} // ar

#endif //DIAGNOSIS_RESULT_H
