#include <QDir>

#include <configuration.h>

/* Timer configuration */
#define GROUP_TIMER "Timer"
#define KEY_REFRESH_INTERVAL "RefreshInterval"
#define DEFAULT_REFRESH_INTERVAL 1000

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

Configuration::Configuration(
    const QString& filepath,
    QObject* parent) 
  : QObject(parent), filepath_(filepath) {
}

Configuration::Configuration(QObject* parent) :
  QObject(parent),
  filepath_(GOS_CONFIGURATION_FILE_PATH) {
}

QSettings* Configuration::read() {
  if (!settings_) {
    if (!create()) {
      return nullptr;
    }
  }
  settings_->sync();

  QVariant value;

  /* Timer configuration */
  settings_->beginGroup(GROUP_TIMER);
  value = settings_->value(KEY_REFRESH_INTERVAL, DEFAULT_REFRESH_INTERVAL);
  setRefreshInterval(value.toInt());
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
  if (!settings_) {
    if (!create()) {
      return nullptr;
    }
  }

  /* Timer configuration */
  settings_->beginGroup(GROUP_TIMER);
  settings_->setValue(KEY_REFRESH_INTERVAL, refreshInterval_);
  settings_->endGroup();

  /* Experiment configuration */
  settings_->beginGroup(GROUP_EXPERIMENT);
  settings_->setValue(KEY_BOOLEAN, boolean_);
  settings_->setValue(KEY_REAL, real_);
  settings_->setValue(KEY_INTEGER, integer_);
  settings_->setValue(KEY_TEXT, text_);
  settings_->endGroup();

  if (sync) {
    settings_->sync();
  }
  return settings_.get();
}

bool Configuration::create() {
  //QString filepath = QDir::cleanPath(path_ + QDir::separator() + filename_);
  settings_.reset(new QSettings(filepath_, SettingsFormat));
  if (settings_) {
    return true;
  } else {
    qCritical() << "Out of memory when trying to create a Qt Setting";
    return false;
  }
}

/* Timer configuration */
const int& Configuration::refreshInterval() const {
  return refreshInterval_;
}

/* Experiment configuration */
const bool& Configuration::boolean() const {
  return boolean_;
}
const double& Configuration::real() const {
  return real_;
}
const int& Configuration::integer() const {
  return integer_;
}
const QString& Configuration::text() const {
  return text_;
}

/* Timer configuration */
void Configuration::setRefreshInterval(const int& value) {
  if (refreshInterval_ != value) {
    refreshInterval_ = value;
    qDebug() << "Setting refresh interval to " << refreshInterval_;
    emit refreshIntervalChanged();
  }
}

/* Experiment configuration */
void Configuration::setBoolean(const bool& value) {
  if (boolean_ != value) {
    boolean_ = value;
    qDebug() << "Setting boolean to " << boolean_;
    emit booleanChanged();
  }
}
void Configuration::setReal(const double& value) {
  if (real_ != value) {
    real_ = value;
    qDebug() << "Setting real to " << real_;
    emit realChanged();
  }
}
void Configuration::setInteger(const int& value) {
  if (integer_ != value) {
    integer_ = value;
    qDebug() << "Setting integer to " << integer_;
    emit integerChanged();
  }
}
void Configuration::setText(const QString& value) {
  if (text_ != value) {
    text_ = value;
    qDebug() << "Setting text to '" << text_ << "'";
    emit textChanged();
  }
}

namespace initialize {
bool configuration(Configuration& configuration) {
  QSettings* settings = configuration.read();
  if (settings != nullptr) {
    qInfo() << "Read the configuration successfully";
    return true;
  } else {
    qCritical() << "Failed to read the configuration";
    return false;
  }
}
}
