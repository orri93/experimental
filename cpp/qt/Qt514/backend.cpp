#include <iostream>

#include <backend.h>
#include <types.h>

#include <model/interval.h>
#include <validation/float.h>

#define GOS_QML_BACKEND "backend"
#define GOS_QML_MODEL_INTERVAL "intervalModel"

namespace backend {
typedef std::unique_ptr<Backend> BackendPointer;
static BackendPointer _backend;
bool create(QQmlApplicationEngine& engine) {
  _backend = std::make_unique<Backend>(engine);
  if (_backend) {
    QQmlContext* context = engine.rootContext();
    if (context) {
      context->setContextProperty(GOS_QML_BACKEND, _backend.get());
      if (_backend->initialize()) {
        return true;
      } else {
        std::cerr << "Failed to initialize the Backend" << std::endl;
        return false;
      }
    } else {
      std::cerr << "QML Context is undefined" << std::endl;
      return false;
    }
  } else {
    std::cerr << "Out of memory when creating the Backend" << std::endl;
    return false;
  }
}
}

namespace backend {
void handle(
  Configuration& configuration,
  std::function<void(const int&)> & setting,
  const int& value) {
  qDebug() << "Setting configuration to 'write' mode";
  configuration.setMode(configuration::mode::write);
  setting(value);
}
}

Backend::Backend(QQmlApplicationEngine& engine, QObject* parent) :
  Items(parent),
  engine_(engine),
  isConnected_(false),
  intervalmodel_(model::create()),
  ispanelcompleted_(false) {
}

Backend::~Backend() {
}

bool Backend::initialize(/* QQmlContext* context */) {
  bool result = false;
  QQmlContext* context = engine_.rootContext();
  if (context != nullptr) {
    context->setContextProperty(GOS_QML_MODEL_INTERVAL, intervalmodel_);
  }
  configuration_ = std::make_unique<Configuration>(this);
  if (configuration_) {
    //QSettings* settings = configuration_->read();
    result = configuration_->initialize(true);
    if (result) {
      return result;
    } else {
      qCritical() << "Failed to initialize configuration";
    }
  } else {
    qCritical() << "Failed to create configuration";
  }
  return result;
}

int Backend::update(
  QAbstractSeries* output,
  QAbstractSeries* temperature,
  QAbstractSeries* setpoints) {
  if (output != nullptr && temperature != nullptr && setpoints != nullptr) {
    qInfo() << "Update";
  }
  return 0;
}

void Backend::panelCompleted() {
  interval_ = configuration_->interval();
  boolean_ = configuration_->boolean();
  real_ = configuration_->real();
  integer_ = configuration_->integer();
  text_ = configuration_->text();
  emit intervalChanged();
  emit intervalIndexChanged();
  emit booleanChanged();
  emit textChanged();
  emit completed();
  qDebug() << "Applying from configuration completed";
  ispanelcompleted_ = true;
}

bool Backend::commandA() {
  qInfo() << "Command A";
  return true;
}
bool Backend::commandB() {
  qInfo() << "Command B";
  return true;
}

const bool& Backend::isConnected() const {
  return isConnected_;
}

const int Backend::intervalIndex() const {
  return model::index(interval());
}

void Backend::setInterval(const int& value) {
  std::function<void(const int&)> setter = std::bind(
    &Configuration::setInterval,
    configuration_.get(),
    std::placeholders::_1);
  if (ispanelcompleted_ && applyInterval(value)) {
    qDebug() << "Setting interval to " << value;
    backend::handle(*configuration_, setter, value);
    intervalIndexChanged();
    intervalChanged();
  }
}

void Backend::setIntervalIndex(const int& value) {
  setInterval(model::value(value));
}

void Backend::setBoolean(const bool& value) {
  std::function<void(const bool&)> setter = std::bind(
    &Configuration::setBoolean,
    configuration_.get(),
    std::placeholders::_1);
  if (ispanelcompleted_ && applyBoolean(value)) {
    qDebug() << "Setting boolean to " << value;
    //backend::handle(*configuration_, setter, value);
    booleanChanged();
  }
}
void Backend::setReal(const double& value) {
  std::function<void(const double&)> setter = std::bind(
    &Configuration::setReal,
    configuration_.get(),
    std::placeholders::_1);
  if (ispanelcompleted_ && applyReal(value)) {
    qDebug() << "Setting real to " << value;
    //backend::handle(*configuration_, setter, value);
    realChanged();
  }
}
void Backend::setInteger(const int& value) {
  std::function<void(const int&)> setter = std::bind(
    &Configuration::setInteger,
    configuration_.get(),
    std::placeholders::_1);
  if (ispanelcompleted_ && applyInteger(value)) {
    qDebug() << "Setting integer to " << value;
    backend::handle(*configuration_, setter, value);
    integerChanged();
  }
}
void Backend::setText(const QString& value) {
  std::function<void(const QString&)> setter = std::bind(
    &Configuration::setText,
    configuration_.get(),
    std::placeholders::_1);
  if (ispanelcompleted_ && applyText(value)) {
    qDebug() << "Setting text to '" << value << "'";
    //backend::handle(*configuration_, setter, value);
    textChanged();
  }
}

void Backend::setIsConnected(const bool& value) {
  if (isConnected_ != value) {
    isConnected_ = value;
    qDebug() << "Setting connected to " << isConnected_;
    emit isConnectedChanged();
  }
}

void Backend::apply() {
  interval_ = configuration_->interval();
  boolean_ = configuration_->boolean();
  real_ = configuration_->real();
  integer_ = configuration_->integer();
  text_ = configuration_->text();
  emit intervalChanged();
  emit intervalIndexChanged();
  emit booleanChanged();
  emit textChanged();
  emit completed();
  qDebug() << "Applying from configuration completed";
}