#ifndef MODEL_VALIDATION_FLOAT_H
#define MODEL_VALIDATION_FLOAT_H

#include <QValidator>
#include <QString>

#define GOS_QML_FLOAT_VALIDATOR "FloatValidator"

namespace validation {
class FloatValidator : public QValidator {
  Q_OBJECT
public:
  explicit FloatValidator(QObject* parent = nullptr);
  virtual State validate(QString& input, int& pos) const;
};
}

#endif
