#include <algorithm>
#include <ui.h>

/* UI configuration group */
#define GROUP_UI "Ui"

/* Controller input items */
#define KEY_BB_SETPOINT "SetpointAccuracy"
#define DEFAULT_BB_SETPOINT_TUNING_FROM -10.0
#define DEFAULT_BB_SETPOINT_TUNING_TO 280.0
#define DEFAULT_BB_SETPOINT_PRECISION 1

/* Controller output items */
#define KEY_BB_INTEGRAL "IntegralFormat"
#define DEFAULT_BB_INTEGRAL_PRECISION 0

/* PID configuration */
#define KEY_BB_KP "KpAccuracy"
#define DEFAULT_BB_KP_PRECISION 2

/* Tuning configuration */
#define KEY_BB_KP_TUNING "KpTuningRange"
#define DEFAULT_BB_KP_TUNING_FROM 0.0
#define DEFAULT_BB_KP_TUNING_TO 1.0
#define DEFAULT_BB_KI_TUNING_FROM 0.0
#define DEFAULT_BB_KI_TUNING_TO 10.0

namespace qs = ::qt::sandbox;
namespace qsm = ::qt::sandbox::model;

namespace qt {
namespace sandbox {

Ui::Ui(QObject* parent) :
  QObject(parent) {
}

Ui::Ui(const Ui& ui) :
  /* Controller input items */
  setpoint_(ui.setpoint_),
  /* Controller output items */
  integral_(ui.integral_),
  /* PID configuration */
  kp_(ui.kp_),
  /* Tuning configuration */
  kpTuning_(ui.kpTuning_) {
}

Ui& Ui::operator=(const Ui& ui) {
  return set(ui);
}

Ui& Ui::set(const Ui& ui) {
  if (this != &ui) {
    /* Controller input items */
    this->setpoint_ = ui.setpoint_;

    /* Controller output items */
    this->integral_ = ui.integral_;

    /* PID configuration */
    this->kp_ = ui.kp_;

    /* Tuning configuration */
    this->kpTuning_ = ui.kpTuning_;
  }
  return *this;
}

QSettings* Ui::read(QSettings* settings) {
  QVariant value;

  /* Ui configuration group */
  settings->beginGroup(GROUP_UI);

  /* Controller input items */
  qs::read(
    settings,
    KEY_BB_SETPOINT,
    setpoint_,
    make_accuracy(
      qsm::Restriction::Enum::Both,
      make_range(
        DEFAULT_BB_SETPOINT_TUNING_FROM,
        DEFAULT_BB_SETPOINT_TUNING_TO),
      DEFAULT_BB_SETPOINT_PRECISION));

  /* Controller output items */
  qs::read(
    settings,
    KEY_BB_INTEGRAL,
    integral_,
    DEFAULT_BB_INTEGRAL_PRECISION);

  /* PID configuration */
  qs::read(
    settings,
    KEY_BB_KP,
    kp_,
    DEFAULT_BB_KP_PRECISION);

  /* Tuning configuration */
  qs::read(
    settings,
    KEY_BB_KP_TUNING,
    this->kpTuning_,
    DEFAULT_BB_KP_TUNING_FROM,
    DEFAULT_BB_KP_TUNING_TO);

  /* UI configuration group */
  settings->endGroup();

  return settings;
}

QSettings* Ui::write(QSettings* settings) {
  /* Ui configuration group */
  settings->beginGroup(GROUP_UI);

  /* Controller input items */
  qs::write(settings, KEY_BB_SETPOINT, this->setpoint_);

  /* Controller output items */
  qs::write(settings, KEY_BB_INTEGRAL, this->integral_);

  /* PID configuration */
  qs::write(settings, KEY_BB_KP, kp_);

  /* Tuning configuration */
  qs::write(settings, KEY_BB_KP_TUNING, this->kpTuning_);

  /* UI configuration group */
  settings->endGroup();

  return settings;
}
/* Controller input items */
Accuracy* Ui::setpoint() { return &setpoint_; }
/* Controller output items */
Format* Ui::integral() { return &integral_; }
/* PID configuration */
Accuracy* Ui::kp() { return &kp_; }
/* Tuning configuration */
Range* Ui::kpTuning() { return &kpTuning_; }

/* Controller input items */
void Ui::setSetpoint(const Accuracy* accuracy) {
  if (accuracy != nullptr) {
    if (setpoint_ != *accuracy) {
      setpoint_ = *accuracy;
      emit setpointChanged();
    }
  }
}
/* Controller output items */
void Ui::setIntegral(const Format* format) {
  if (format != nullptr) {
    if (integral_ != *format) {
      integral_ = *format;
      emit integralChanged();
    }
  }
}
/* PID configuration */
void Ui::setKp(const Accuracy* accuracy) {
  if (accuracy != nullptr) {
    if (kp_ != *accuracy) {
      kp_ = *accuracy;
      emit kpChanged();
    }
  }
}
/* Tuning configuration */
void Ui::setKpTuning(const Range* range) {
  if (range != nullptr) {
    if (kpTuning_ != *range) {
      kpTuning_ = *range;
      emit kpTuningChanged();
    }
  }
}

}
}

bool operator==(const qs::Ui& lhs, const qs::Ui& rhs) {
  return compare(lhs, rhs) == 0;
}
bool operator!=(const qs::Ui& lhs, const qs::Ui& rhs) {
  return compare(lhs, rhs) != 0;
}

int compare(const qs::Ui& first, const qs::Ui& second) {
  return (
    /* Controller input items */
    first.setpoint_ == second.setpoint_ &&
    /* Controller output items */
    first.integral_ == second.integral_ &&
    /* PID configuration */
    first.kp_ == second.kp_ &&
    /* Tuning configuration */
    first.kpTuning_ == second.kpTuning_) ? 0 : 1;
}
