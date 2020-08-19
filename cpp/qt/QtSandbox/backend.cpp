#include <iostream>
#include <fstream>
#include <string>
#include <memory>

#include <QDebug>

#include <QtCharts/QXYSeries>
#include <QtCharts/QAreaSeries>
#include <QtQuick/QQuickView>
#include <QtQuick/QQuickItem>
#include <QtCore/QRandomGenerator>
#include <QtCore/QtMath>

#include <backend.h>
#include <restriction.h>

#define GOS_QML_BACKEND "backend"

QT_CHARTS_USE_NAMESPACE

Q_DECLARE_METATYPE(QAbstractSeries*)
Q_DECLARE_METATYPE(QAbstractAxis*)

namespace qs = ::qt::sandbox;
namespace qsm = ::qt::sandbox::model;

namespace backend {
typedef std::unique_ptr<qs::Backend> BackendPointer;
static BackendPointer _backend;
//static PidToolkitPlugin _plugin;
bool create(QQmlContext& context) {

  //_plugin.registerTypes("");
 // qmlRegisterType<gptu::Range>(
  //  GOS_QML_TYPE_RANGE_URI, 1, 0, GOS_QML_TYPE_RANGE_NAME);

  qRegisterMetaType<qs::Range*>("::qt::sandbox::Range*");
  qRegisterMetaType<qs::Format*>("::qt::sandbox::Format*");
  qRegisterMetaType<qs::Accuracy*>("::qt::sandbox::Accuracy*");
  qRegisterMetaType<qs::Ui*>("::qt::sandbox::Ui*");
  qRegisterMetaType<qs::Configuration*>("::qt::sandbox::Configuration*");
  qRegisterMetaType<qsm::Restriction*>("::qt::sandbox::model::Restriction*");

  qmlRegisterUncreatableMetaObject(
    qsm::Restriction::staticMetaObject,
    "qt.sandbox.models", 1, 0, "Restriction",
    "Cannot create namespace Restriction in QML");
  qRegisterMetaType<qsm::Restriction::Enum>(
    "::qt::sandbox::model::Restriction::Enum");


  //qmlRegisterUncreatableType<qs::Restriction>(
  //  "qt.sandbox",
  //  1,
  //  0,
  //  GOS_QML_ENUM_RESTRICTION,
  //  "Cannot create Restriction in QML");

  _backend = std::make_unique<qs::Backend>(context);
  if (_backend) {
    context.setContextProperty(GOS_QML_BACKEND, _backend.get());
    if (_backend->initialize()) {
      return true;
    } else {
      std::cerr << "Failed to initialize the Backend" << std::endl;
    }
  } else {
    std::cerr << "Out of memory when creating the Backend" << std::endl;
  }
  return false;
}
}

namespace qt {
namespace sandbox {

Backend::Backend(QQmlContext& context, QObject* parent) :
  QObject(parent),
  context_(context),
  count_(0),
  /* Controller input items */
  setpoint_(0.0),
  /* Controller tuning items */
  kp_(0.0F),
  /* Controller output items */
  integral_(0.0F) {
  qRegisterMetaType<QAbstractSeries*>();
  qRegisterMetaType<QAbstractAxis*>();
}

Backend::~Backend() {
}

bool Backend::initialize() {
  configuration_ = std::make_unique<Configuration>(this);
  if (configuration_) {
    QSettings* settings = configuration_->initialize();
    if (settings != nullptr) {
      applyConfiguration();
      Ui* uip = configuration_->ui();
      uiForDialog_ = std::make_unique<Ui>(*uip);
      if (uiForDialog_) {
        qInfo() << "Initialize completed";
        return true;
      } else {
        qCritical() << "Failed to UI Settings for Dialog";
      }
    } else {
      qCritical() << "Failed to initialize configuration";
    }
  } else {
    qCritical() << "Failed to create configuration";
  }
  return false;
}

void Backend::panelCompleted() {
  qDebug() << "Panel Completed";
  notify();
  emit uiChanged();
  emit configurationChanged();
  emit completed();
}

void Backend::applyUiDialog() {
  if (uiForDialog_ && configuration_) {
    if (configuration_->applyUiDialog(uiForDialog_.get())) {
      emit configurationChanged();
    }
  }
}

void Backend::rejectUiDialog() {
  if (uiForDialog_ && configuration_) {
    Ui* uip = configuration_->ui();
    if (::compare(*uip, *uiForDialog_) != 0) {
      *uiForDialog_ = *uip;
      emit uiChanged();
    }
  }
}

//QString Backend::toString(const Restriction::Enum& restriction) {
//  return Restriction::toString(restriction);
//}

/* Configuration Access */
Configuration* Backend::configuration() {
  if (configuration_) {
    return configuration_.get();
  }
  return nullptr;
}

Ui* Backend::ui() {
  if (uiForDialog_) {
    return uiForDialog_.get();
  } else {
    qWarning() << "Undefined Black Box from Orchestration";
    return nullptr;
  }
}

/* Controller input items */
const double& Backend::setpoint() const {
  return setpoint_;
}

/* Controller tuning items */
const float& Backend::kp() const { return kp_; }
const QString Backend::kpText() const {
  return QString::number(static_cast<double>(kp_));
}

/* Controller output items */
const float& Backend::integral() const {
  return integral_;
}
const QString Backend::integralText() const {
  return QString::number(static_cast<double>(integral_));
}

qsm::Restriction* Backend::restrictionModel() {
  return &restrictionmodel_;
}

void Backend::setSetpoint(const double& setpoint) {
  bool result;
  float local = static_cast<float>(setpoint);
  if (setpoint_ != local && local >= 0.0F && local <= 300.0F) {
    result = writeSetpoint(local);
    if (result) {
      qDebug() << "Successfully set Setpoint to " << setpoint;
    } else {
      qWarning() << "Failed to set Setpoint to " << setpoint;
    }
    setpoint_ = setpoint;
    emit setpointChanged();
  }
}

/* Controller tuning items */
void Backend::setKp(const float& value) {
  bool result;
  float local = static_cast<float>(value);
  if (kp_ != local && local >= 0.0F && local <= 100.0F) {
    result = writeKp(local);
    if (result) {
      qDebug() << "Successfully set Kp to " << value;
    } else {
      qWarning() << "Failed to set Kp to " << value;
    }
    kp_ = value;
    emit kpChanged();
    emit kpTextChanged();
  }
}

void Backend::setKpText(const QString& value) {
  bool ok;
  float kp = value.toFloat(&ok);
  if (ok && std::isfinite(kp) && kp != kp_) {
    setKp(kp);
  }
}

void Backend::applyConfiguration() {
}

/* Controller output items */
void Backend::setIntegral(const float& value) {
  if (integral_ != value) {
    integral_ = value;
    emit integralChanged();
    emit integralTextChanged();
  }
}


/* Other items */

bool Backend::writeSetpoint(const float& setpoint) {
  qDebug() << "Write Setpoint " << setpoint;
  return true;
}

bool Backend::writeKp(const float& kp) {
  qDebug() << "Write Kp " << kp;
  return true;
}

void Backend::notify() {

}

}
}
