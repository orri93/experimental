#include <select.h>

static void initializeFileDialog(QFileDialog& dialog, QFileDialog::AcceptMode acceptMode, const QStringList& nameFilters);

SelectFileLayout::SelectFileLayout(const QString& label, const QString& value, QWidget* parent) :
  QHBoxLayout(parent),
  _value(value),
  _label(nullptr),
  _edit(nullptr),
  _browse(nullptr),
  _set(nullptr) {
  initialize(label);
}

void SelectFileLayout::onBrowseClicked() {
  QFileDialog fileDialog(nullptr, tr("Open Cascade File"));
  QStringList nameFilterList;
  nameFilterList.append(tr("XML files (*.xml)"));
  nameFilterList.append(tr("Text files (*.txt)"));
  initializeFileDialog(fileDialog, QFileDialog::AcceptOpen, nameFilterList);
  int result = fileDialog.exec();
  if (result == QDialog::Accepted) {
    _edit->setText(fileDialog.selectedFiles().first());
  }
}

void SelectFileLayout::onSetClicked() {
  _value = _edit->text();
  valueChanged(_value);
}

void SelectFileLayout::initialize(const QString& label) {
  _label = new QLabel(label);
  _edit = new QLineEdit;
  _edit->setMinimumWidth(100);
  _edit->setText(_value);

  _browse = new QPushButton(tr("Browse"));
  connect(_browse, &QPushButton::clicked, this, &SelectFileLayout::onBrowseClicked);
  
  _set = new QPushButton(tr("Set"));
  connect(_set, &QPushButton::clicked, this, &SelectFileLayout::onSetClicked);

  addWidget(_label);
  addWidget(_edit);
  addWidget(_browse);
  addWidget(_set);
}

void initializeFileDialog(QFileDialog& dialog, QFileDialog::AcceptMode acceptMode, const QStringList& nameFilters) {
  static bool firstDialog_ = true;

  if (firstDialog_) {
    firstDialog_ = false;
    const QStringList locations = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
    dialog.setDirectory(locations.isEmpty() ? QDir::currentPath() : locations.last());
  }

  dialog.setFileMode(QFileDialog::AnyFile);
  dialog.setNameFilters(nameFilters);
  dialog.selectNameFilter(nameFilters.front());

}
