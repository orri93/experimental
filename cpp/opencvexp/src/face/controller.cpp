#include <mutex>
#include <iostream>

#include <QString>

#include <lock.h>
#include <controller.h>

static int method2index(const ::gos::cv::detection::method& method);
static ::gos::cv::detection::method index2method(const int& index);

FaceDetectionController::FaceDetectionController(gos::cv::detection::context& context, QWidget* parent) :
  QDialog(parent),
  _timerLabel(nullptr),
  _timerSpinBox(nullptr),
  _timerLayout(nullptr),
  _methodLabel(nullptr),
  _methodBox(nullptr),
  _methodLayout(nullptr),
  _scaleLabel(nullptr),
  _scaleSpinBox(nullptr),
  _scaleLayout(nullptr),
  _detectionFaceGroup(nullptr),
  _detectionEyesGroup(nullptr),
  _optionLayout(nullptr),
  _context(context) {
  initialize();
}

void FaceDetectionController::onTimerChanged(int value) {
  _context.timer = value;
  timerChanged(value);
}

void FaceDetectionController::onMethodChanged(int index) {
  ::std::lock_guard guard(::gos::qt::lock::context_);
  _context.method = index2method(index);
  update();
}

void FaceDetectionController::onScaleChanged(double value) {
  ::std::lock_guard guard(::gos::qt::lock::context_);
  _context.scale = value;
}

void FaceDetectionController::initialize() {
  _timerLabel = new QLabel(tr("Timer:"));
  _timerSpinBox = new QSpinBox;
  _timerSpinBox->setMinimum(1);
  _timerSpinBox->setMaximum(1000);
  _timerSpinBox->setValue(_context.timer);
  connect(_timerSpinBox, &QSpinBox::valueChanged, this, &FaceDetectionController::onTimerChanged);
  _timerLayout = new QHBoxLayout;
  _timerLayout->addWidget(_timerLabel);
  _timerLayout->addWidget(_timerSpinBox);

  _methodLabel = new QLabel(tr("Method:"));
  _methodBox = new QComboBox;
  _methodBox->addItem("A");
  _methodBox->addItem("B");
  _methodBox->setCurrentIndex(method2index(_context.method));
  connect(_methodBox, &QComboBox::currentIndexChanged, this, &FaceDetectionController::onMethodChanged);
  _methodLayout = new QHBoxLayout;
  _methodLayout->addWidget(_methodLabel);
  _methodLayout->addWidget(_methodBox);

  _scaleLabel = new QLabel(tr("Scale:"));
  _scaleSpinBox = new QDoubleSpinBox;
  _scaleSpinBox->setMinimum(0.1);
  _scaleSpinBox->setMaximum(2.0);
  _scaleSpinBox->setDecimals(1);
  _scaleSpinBox->setSingleStep(0.1);
  _scaleSpinBox->setValue(_context.scale);
  connect(_scaleSpinBox, &QDoubleSpinBox::valueChanged, this, &FaceDetectionController::onScaleChanged);
  _scaleLayout = new QHBoxLayout;
  _scaleLayout->addWidget(_scaleLabel);
  _scaleLayout->addWidget(_scaleSpinBox);

  _detectionFaceGroup = new DetectionGroup(tr("Face Detection"), _context.face);
  _detectionEyesGroup = new DetectionGroup(tr("Eyes Detection"), _context.eyes);

  update();

  _optionLayout = new QVBoxLayout;
  _optionLayout->addLayout(_timerLayout);
  _optionLayout->addLayout(_methodLayout);
  _optionLayout->addLayout(_scaleLayout);
  _optionLayout->addWidget(_detectionFaceGroup);
  _optionLayout->addWidget(_detectionEyesGroup);

  setLayout(_optionLayout);
}

void FaceDetectionController::update() {
  _scaleSpinBox->setEnabled(_methodBox->currentIndex() > 0);
  _detectionFaceGroup->extra(_methodBox->currentIndex() > 0);
  _detectionEyesGroup->extra(_methodBox->currentIndex() > 0);
}

int method2index(const ::gos::cv::detection::method& method) {
  switch (method) {
  case ::gos::cv::detection::method::a:
    return 0;
  case ::gos::cv::detection::method::b:
    return 1;
  default:
    return - 1;
  }
}

::gos::cv::detection::method index2method(const int& index) {
  switch (index) {
  case 0:
    return ::gos::cv::detection::method::a;
  case 1:
    return ::gos::cv::detection::method::b;
  default:
    return ::gos::cv::detection::method::unknown;
  }
}
