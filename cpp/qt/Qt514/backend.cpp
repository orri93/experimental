#include <backend.h>

Backend::Backend(QObject* parent) :
  QObject(parent),
  isConnected_(false),
  refreshInterval_(1000),
  boolean_(false),
  real_(0.0),
  integer_(0) {
}

Backend::~Backend() {
}

bool Backend::initialize(/* QQmlContext* context */) {
  bool result = false;
  configuration_ = std::make_unique<Configuration>(this);
  if (configuration_) {
    //QSettings* settings = configuration_->read();
    result = initialize::configuration(*configuration_);
    if (result) {
      setRefreshInterval(configuration_->refreshInterval());
      setBoolean(configuration_->boolean());
      setReal(configuration_->real());
      setInteger(configuration_->integer());
      setText(configuration_->text());
    } else {
      qCritical() << "Failed to initialize configuration";
    }
  } else {
    qCritical() << "Failed to create configuration";
  }
  return result;
}


bool Backend::commandA() {
  qInfo() << "Command A";
  return true;
}
bool Backend::commandB() {
  qInfo() << "Command B";
  return true;
}

int Backend::update(
  QAbstractSeries* output,
  QAbstractSeries* temperature,
  QAbstractSeries* setpoints) {
  if(output != nullptr && temperature != nullptr && setpoints != nullptr) {
    qInfo() << "Update";
  }
  return 0;
}

const bool& Backend::isConnected() const {
  return isConnected_;
}

const int& Backend::refreshInterval() const {
  return refreshInterval_;
}

const bool& Backend::boolean() const {
  return boolean_;
}
const double& Backend::real() const {
  return real_;
}
const int& Backend::integer() const {
  return integer_;
}
const QString& Backend::text() const {
  return text_;
}

void Backend::setRefreshInterval(const int& value) {
  if (refreshInterval_ != value) {
    refreshInterval_ = value;
    qDebug() << "Setting refresh interval to " << refreshInterval_;
    emit refreshIntervalChanged();
  }
}

void Backend::setBoolean(const bool& value) {
  if (boolean_ != value) {
    boolean_ = value;
    qDebug() << "Setting boolean to " << boolean_;
    emit booleanChanged();
  }
}
void Backend::setReal(const double& value) {
  if (real_ != value) {
    real_ = value;
    qDebug() << "Setting real to " << real_;
    emit realChanged();
  }
}
void Backend::setInteger(const int& value) {
  if (integer_ != value) {
    integer_ = value;
    qDebug() << "Setting integer to " << integer_;
    emit integerChanged();
  }
}
void Backend::setText(const QString& value) {
  if (text_ != value) {
    text_ = value;
    qDebug() << "Setting text to '" << text_ << "'";
    emit textChanged();
  }
}

void Backend::setIsConnected(const bool& value) {
  if (isConnected_ != value) {
    isConnected_ = value;
    qDebug() << "Setting connected to " << isConnected_;
    emit isConnectedChanged();
  }
}
