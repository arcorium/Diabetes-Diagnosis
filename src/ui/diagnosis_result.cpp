#include "diagnosis_result.h"
#include "../cmake-build-debug/cbr_autogen/include/ui_diagnosis_result.h"

namespace ar
{
  DiagnosisResult::DiagnosisResult(QWidget* parent) :
    QDialog(parent), ui_(new Ui::DiagnosisResult)
  {
    ui_->setupUi(this);
  }

  DiagnosisResult::~DiagnosisResult()
  {
    delete ui_;
  }

  Ui::DiagnosisResult* DiagnosisResult::ui() noexcept
  {
    return ui_;
 }
} // ar
