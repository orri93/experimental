#include <size.h>

SizeLayout::SizeLayout(const QString& label, const QSize& size, QWidget* parent) :
  QHBoxLayout(parent),
  _size(size),
  _label(nullptr),
  _widthSpinBox(nullptr),
  _heightSpinBox(nullptr) {
  initialize(label);
}

void SizeLayout::extra(bool enable) {
  _widthSpinBox->setEnabled(enable);
  _heightSpinBox->setEnabled(enable);
}

void SizeLayout::onWidthChanged(int value) {
  _size.setWidth(value);
  sizeChanged(_size);
}

void SizeLayout::onHeightChanged(int value) {
  _size.setHeight(value);
  sizeChanged(_size);
}

void SizeLayout::initialize(const QString& label) {
  _label = new QLabel(label);
  _widthSpinBox = new QSpinBox;
  _widthSpinBox->setMinimum(1);
  _widthSpinBox->setMaximum(100);
  _widthSpinBox->setValue(_size.width());
  connect(_widthSpinBox, &QSpinBox::valueChanged, this, &SizeLayout::onWidthChanged);
  _heightSpinBox = new QSpinBox;
  _heightSpinBox->setMinimum(1);
  _heightSpinBox->setMaximum(100);
  _heightSpinBox->setValue(_size.height());
  connect(_heightSpinBox, &QSpinBox::valueChanged, this, &SizeLayout::onHeightChanged);
  addWidget(_label);
  addWidget(_widthSpinBox);
  addWidget(_heightSpinBox);
}
