#include <QDir>

#include <configuration.h>

/* Timer configuration */
#define GROUP_TIMER "Timer"
#define KEY_INTERVAL "Interval"
#define DEFAULT_INTERVAL 1000

/* Experiment configuration */
#define GROUP_EXPERIMENT "Experiment"
#define KEY_BOOLEAN "Boolean"
#define KEY_REAL "Real"
#define KEY_INTEGER "Integer"
#define KEY_TEXT "Text"
#define DEFAULT_BOOLEAN true
#define DEFAULT_REAL 93.418
#define DEFAULT_INTEGER 666
#define DEFAULT_TEXT "Geirmundur"

Configuration::Configuration(const QString& filepath, QObject* parent) :
  Items(parent),
  filepath_(filepath),
  mode_(configuration::mode::normal) {
  create();
}

Configuration::Configuration(QObject* parent) :
  Items(parent),
  filepath_(GOS_CONFIGURATION_FILE_PATH),
  mode_(configuration::mode::normal) {
  create();
}

QSettings* Configuration::initialize(const bool& watcher) {
  QSettings* result = nullptr;
  //QString filepath = QDir::cleanPath(path_ + QDir::separator() + filename_);
  settings_ = std::make_unique<QSettings>(filepath_, SettingsFormat);
  if (settings_) {
    setMode(configuration::mode::initializing);
    result = read(false);
    if (result != nullptr) {
      if (watcher) {
        watcher_ = std::make_unique<QFileSystemWatcher>(this);
        if (watcher_) {
          QObject::connect(
            watcher_.get(),
            &QFileSystemWatcher::fileChanged,
            this,
            &Configuration::onFileChanged);
          watcher_->addPath(settings_->fileName());
          qInfo() << "Watching Configuration from '"
            << settings_->fileName() << "'";
        } else {
          qCritical() << "Out of memory when trying to create a "
            "Qt File System Watcher";
          result = nullptr;
        }
      } else {
        qInfo() << "Reading Configuration from '"
          << settings_->fileName() << "'";
      }
    } else {
      qCritical() << "Failed to read configuration";
      result = nullptr;
    }
  } else {
    qCritical() << "Out of memory when trying to create a Qt Setting";
    result = nullptr;
  }
  setMode(configuration::mode::normal);
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

  /* Timer configuration */
  settings_->beginGroup(GROUP_TIMER);
  value = settings_->value(KEY_INTERVAL, DEFAULT_INTERVAL);
  setInterval(value.toInt());
  settings_->endGroup();

  /* Experiment configuration */
  settings_->beginGroup(GROUP_EXPERIMENT);
  value = settings_->value(KEY_BOOLEAN, DEFAULT_BOOLEAN);
  setBoolean(value.toBool());
  value = settings_->value(KEY_REAL, DEFAULT_REAL);
  setReal(value.toDouble());
  value = settings_->value(KEY_INTEGER, DEFAULT_INTEGER);
  setInteger(value.toInt());
  value = settings_->value(KEY_TEXT, DEFAULT_TEXT);
  setText(value.toString());
  settings_->endGroup();

  return settings_.get();
}

QSettings* Configuration::write(const bool& sync) {
  if (startWriting() == nullptr) {
    return nullptr;
  }

  writeTimers();
  writeExperiment();

  return completeWriting(sync);
}

void Configuration::create() {
  fWriteExperiment_ = std::bind(&Configuration::writeExperiment, this);
  fWriteTimers_ = std::bind(&Configuration::writeTimers, this);
}

/* Writing */
QSettings* Configuration::startWriting() {
  if (settings_) {
    return settings_.get();
  } else {
    return nullptr;
  }
}
void Configuration::writeTimers() {
  settings_->beginGroup(GROUP_TIMER);
  settings_->setValue(KEY_INTERVAL, interval());
  settings_->endGroup();
}
void Configuration::writeExperiment() {
  settings_->beginGroup(GROUP_EXPERIMENT);
  settings_->setValue(KEY_BOOLEAN, boolean());
  settings_->setValue(KEY_REAL, real());
  settings_->setValue(KEY_REAL, integer());
  settings_->setValue(KEY_REAL, text());
  settings_->endGroup();
}
QSettings* Configuration::completeWriting(const bool& sync) {
  if (sync) {
    settings_->sync();
  }
  return settings_.get();
}

const configuration::mode& Configuration::mode() const {
  return mode_;
}
void Configuration::setMode(const configuration::mode& mode) {
  mode_ = mode;
}

void Configuration::handle(std::function<void()>& changed) {
  switch (mode_) {
  case configuration::mode::normal:
    emit changed();
    break;
  case configuration::mode::write:
  case configuration::mode::initializing:
  default:
    break;
  }
}

void Configuration::handle(
  std::function<void()> & changed,
  std::function<void()> & write) {
  switch (mode_) {
  case configuration::mode::normal:
    qDebug() << "Configuration handling mode 'normal'";
    emit changed();
    break;
  case configuration::mode::write:
    qDebug() << "Configuration handling mode 'write'";
    if (startWriting()) {
      write();
      completeWriting(true);
      qDebug() << "Configuration writing completed";
    }
    break;
  case configuration::mode::initializing:
  default:
    break;
  }
}

/* Timer configuration */
void Configuration::setInterval(const int& value) {
  std::function<void()> changed =
    std::bind(&Configuration::intervalChanged, this);
  if (applyInterval(value)) {
    qDebug() << "Setting refresh interval to " << interval_;
    handle(changed, fWriteTimers_);
  }
}

/* Experiment configuration */
void Configuration::setBoolean(const bool& value) {
  std::function<void()> changed =
    std::bind(&Configuration::booleanChanged, this);
  if (applyBoolean(value)) {
    qDebug() << "Setting boolean to " << boolean_;
    handle(changed, fWriteExperiment_);
  }
}
void Configuration::setReal(const double& value) {
  std::function<void()> changed =
    std::bind(&Configuration::realChanged, this);
  if (applyReal(value)) {
    qDebug() << "Setting real to " << real_;
    handle(changed, fWriteExperiment_);
  }
}
void Configuration::setInteger(const int& value) {
  std::function<void()> changed =
    std::bind(&Configuration::integerChanged, this);
  if (applyInteger(value)) {
    qDebug() << "Setting integer to " << integer_;
    handle(changed, fWriteExperiment_);
  }
}
void Configuration::setText(const QString& value) {
  std::function<void()> changed =
    std::bind(&Configuration::textChanged, this);
  if (applyText(value)) {
    qDebug() << "Setting text to '" << text_ << "'";
    handle(changed, fWriteExperiment_);
  }
}

void Configuration::onFileChanged(const QString& path) {
  switch (mode_) {
  case configuration::mode::normal:
    qDebug() << "Configuration file change and in 'read' mode";
    read(true);
    break;
  case configuration::mode::write:
    qDebug() << "Ignoring Configuration file change because in "
      "'write' mode";
    qDebug() << "Configuration Setting mode back to 'normal'";
    setMode(configuration::mode::normal);
    break;
  case configuration::mode::initializing:
    qDebug() << "Ignoring Configuration file change because in "
      "'initializing' mode";
    break;
  default:
    break;
  }
}