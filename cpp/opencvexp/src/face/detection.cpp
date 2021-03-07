#include <mutex>

#include <QSize>

#include <lock.h>
#include <detection.h>

DetectionGroup::DetectionGroup(
  const QString& title,
  ::gos::cv::detection::classification& classification,
  QWidget* parent) :
  QGroupBox(title, parent),
  _selectCascadeFile(nullptr),
  _sizeLayout(nullptr),
  _factorLabel(nullptr),
  _factorSpinBox(nullptr),
  _factorLayout(nullptr),
  _minimumLabel(nullptr),
  _minimumSpinBox(nullptr),
  _minimumLayout(nullptr),
  _detectionLayout(nullptr),
  _classification(classification) {
  initialize();
}

void DetectionGroup::extra(bool enable) {
  _sizeLayout->extra(enable);
  _factorSpinBox->setEnabled(enable);
  _minimumSpinBox->setEnabled(enable);
}

void DetectionGroup::onFactorChanged(double value) {
  ::std::lock_guard guard(::gos::qt::lock::context_);
  _classification.factor = value;
}

void DetectionGroup::onMinimumChanged(int value) {
  ::std::lock_guard guard(::gos::qt::lock::context_);
  _classification.minimum = value;
}

void DetectionGroup::initialize() {
  QString cascadeFile = QString::fromStdString(_classification.file);
  _selectCascadeFile = new SelectFileLayout(tr("Cascade File:"), cascadeFile);

  QSize size(_classification.size.width, _classification.size.height);
  _sizeLayout = new SizeLayout(tr("Size:"), size);

  _factorLabel = new QLabel(tr("Factor:"));
  _factorSpinBox = new QDoubleSpinBox;
  _factorSpinBox->setMinimum(0.1);
  _factorSpinBox->setMaximum(2.0);
  _factorSpinBox->setDecimals(1);
  _factorSpinBox->setSingleStep(0.1);
  _factorSpinBox->setValue(_classification.factor);
  connect(_factorSpinBox, &QDoubleSpinBox::valueChanged, this, &DetectionGroup::onFactorChanged);
  _factorLayout = new QHBoxLayout;
  _factorLayout->addWidget(_factorLabel);
  _factorLayout->addWidget(_factorSpinBox);

  _minimumLabel = new QLabel(tr("Minimum:"));
  _minimumSpinBox = new QSpinBox;
  _minimumSpinBox->setMinimum(1);
  _minimumSpinBox->setMaximum(10);
  _minimumSpinBox->setValue(_classification.minimum);
  connect(_minimumSpinBox, &QSpinBox::valueChanged, this, &DetectionGroup::onMinimumChanged);
  _minimumLayout = new QHBoxLayout;
  _minimumLayout->addWidget(_minimumLabel);
  _minimumLayout->addWidget(_minimumSpinBox);

  _detectionLayout = new QVBoxLayout;
  _detectionLayout->addLayout(_selectCascadeFile);
  _detectionLayout->addLayout(_sizeLayout);
  _detectionLayout->addLayout(_factorLayout);
  _detectionLayout->addLayout(_minimumLayout);

  setLayout(_detectionLayout);
}
