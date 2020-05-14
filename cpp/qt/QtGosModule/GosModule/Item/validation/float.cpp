#include <cmath>

#include <validation/float.h>

namespace validation {
FloatValidator::FloatValidator(QObject* parent) : QValidator(parent) {
};

QValidator::State FloatValidator::validate(QString& input, int& pos) const {
  Q_UNUSED(pos)
    if (input.isEmpty()) {
      return Acceptable;
    }
  bool ok;
  float f = input.toFloat(&ok);
  return (ok && std::isfinite(f)) ? Acceptable : Invalid;
}

}
