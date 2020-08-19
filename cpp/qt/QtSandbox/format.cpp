#include <format.h>

namespace qs = ::qt::sandbox;

namespace qt {
namespace sandbox {

Format::Format(QObject* parent) :
  QObject(parent),
  precision_(0) {
}

Format::Format(
  const int& precision,
  QObject* parent) :
  QObject(parent),
  precision_(precision) {
}

Format::Format(const Format& format) :
  precision_(format.precision_) {
}

Format& Format::operator=(const Format& format) {
  return set(format);
}

Format& Format::set(const Format& format) {
  if (this != &format) {
    setPrecision(format.precision_);
  }
  return *this;
}

void Format::set(const int& precision) {
  setPrecision(precision);
}

const int& Format::precision() const { return precision_; }

void Format::setPrecision(const int& value) {
  if (precision_ != value) {
    precision_ = value;
    emit precisionChanged();
  }
}

Format make_format(const int& precision) {
  return Format(precision);
}

QSettings* read(QSettings* settings, const QString& key, Format& format) {
  return read(
    settings,
    key,
    format,
    DEFAULT_BB_FORMAT_PRECISION);
}

QSettings* read(
  QSettings* settings,
  const QString& key,
  Format& format,
  const int& precision) {
  QVariant value;
  value = settings->value(key + KEY_BB_FORMAT_PRECISION, precision);
  format.setPrecision(value.toInt());
  return settings;
}

QSettings* write(
  QSettings* settings,
  const QString& key,
  const Format& format) {
  settings->setValue(key + KEY_BB_FORMAT_PRECISION, format.precision_);
  return settings;
}

}
}

bool operator==(const qs::Format& lhs, const qs::Format& rhs) {
  return compare(lhs, rhs) == 0;
}

bool operator!=(const qs::Format& lhs, const qs::Format& rhs) {
  return compare(lhs, rhs) != 0;
}

int compare(const qs::Format& first, const qs::Format& second) {
  return (first.precision_ == second.precision_) ? 0 : 1;
}
