#include <QDir>

#include <configuration.h>

/* UI configuration */
#define GROUP_UI "Ui"

namespace qt {
namespace sandbox {

Configuration::Configuration(const QString& filepath, QObject* parent) :
  QObject(parent),
  filepath_(filepath) {
}

Configuration::Configuration(QObject* parent) :
  QObject(parent),
  filepath_(GOS_CONFIGURATION_FILE_PATH) {
}

QSettings* Configuration::initialize() {
  QSettings* result = nullptr;
  settings_ = std::make_unique<QSettings>(filepath_, SettingsFormat);
  if (settings_) {
    //ui_.initialize();
    result = read();
    if (!QFile::exists(settings_->fileName())) {
      qInfo() << "Creating a default Configuration file at '"
        << settings_->fileName() << "'";
      result = write(true);
      if (result == nullptr) {
        qCritical() << "Failed to write default configuration file";
        return result;
      }
    }
    if (result != nullptr) {

    } else {
      qCritical() << "Failed to read configuration";
      result = nullptr;
    }
  } else {
    qCritical() << "Out of memory when trying to create a Qt Setting";
    result = nullptr;
  }
  //ui_.setNormal();
  return result;
}

QSettings* Configuration::read(const bool& sync) {
  if (!settings_) {
    return nullptr;
  }
  if (sync) {
    settings_->sync();
  }

  QVariant value;

  ui_.read(settings_.get());

  return settings_.get();
}

QSettings* Configuration::write(const bool& sync) {
  if (startWriting() == nullptr) {
    return nullptr;
  }

  writeUi();

  return completeWriting(sync);
}

/* Writing */
QSettings* Configuration::startWriting() {
  if (settings_) {
    return settings_.get();
  } else {
    return nullptr;
  }
}
void Configuration::writeUi() {
  ui_.write(settings_.get());
}
QSettings* Configuration::completeWriting(const bool& sync) {
  if (sync) {
    settings_->sync();
  }
  return settings_.get();
}

/* Ui configuration */
Ui* Configuration::ui() {
  return &ui_;
}

bool Configuration::applyUiDialog(Ui* ui) {
  if (&ui_ != ui) {
    if (::compare(ui_, *ui) != 0) {
      ui_ = *ui;
      write(true);
      emit uiChanged();
      return true;
    }
  }
  return false;
}

}
}
