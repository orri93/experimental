#include <QDebug>

#include <items.h>

Items::Items(QObject* parent) :
  QObject(parent),
  interval_(1000),
  boolean_(false),
  real_(0.0),
  integer_(0) {
}

Items::~Items() {
}

/* Timer items */
const int& Items::interval() const {
  return interval_;
}

/* Experiment items */
const bool& Items::boolean() const {
  return boolean_;
}
const double& Items::real() const {
  return real_;
}
const int& Items::integer() const {
  return integer_;
}
const QString& Items::text() const {
  qDebug() << "Item text with value '" << text_ << "' requested";
  return text_;
}

/* Timer items */
bool Items::applyInterval(const int& value) {
  if (interval_ != value) {
    interval_ = value;
    return true;
  }
  return false;
}

/* Experiment items */
bool Items::applyBoolean(const bool& value) {
  if (boolean_ != value) {
    boolean_ = value;
    return true;
  }
  return false;
}

bool Items::applyReal(const double& value) {
  if (real_ != value) {
    real_ = value;
    return true;
  }
  return false;
}

bool Items::applyInteger(const int& value) {
  if (integer_ != value) {
    integer_ = value;
    return true;
  }
  return false;
}

bool Items::applyText(const QString& value) {
  if (text_ != value) {
    text_ = value;
    return true;
  }
  return false;
}
