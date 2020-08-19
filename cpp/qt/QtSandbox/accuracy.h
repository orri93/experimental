#ifndef QT_SANDBOX_ACCURACY_H_
#define QT_SANDBOX_ACCURACY_H_

#include <QObject>
#include <QSettings>

#include <range.h>
#include <format.h>
#include <restriction.h>

#define KEY_BB_ACCURACY_RANGE "Range"
#define KEY_BB_ACCURACY_PRECISION "Precision"
#define KEY_BB_ACCURACY_RESTRICTION "Restriction"

#define DEFAULT_BB_ACCURACY_PRECISION 0

namespace qt { namespace sandbox { class Accuracy; } }

int compare(
  const ::qt::sandbox::Accuracy& first,
  const ::qt::sandbox::Accuracy& second);

bool operator==(
  const ::qt::sandbox::Accuracy& lhs,
  const ::qt::sandbox::Accuracy& rhs);
bool operator!=(
  const ::qt::sandbox::Accuracy& lhs,
  const ::qt::sandbox::Accuracy& rhs);

namespace qt {
namespace sandbox {

QSettings* read(QSettings* settings, const QString& key, Accuracy& accuracy);
QSettings* read(
  QSettings* settings,
  const QString& key,
  Accuracy& accuracy,
  const int& precision);
QSettings* read(
  QSettings* settings,
  const QString& key,
  Accuracy& accuracy,
  const Accuracy& default);
QSettings* write(QSettings* settings, const QString& key, const Accuracy& accuracy);

class Accuracy : public Format {
  Q_OBJECT

  Q_PROPERTY(Range* range READ range WRITE setRange NOTIFY rangeChanged)
  Q_PROPERTY(::qt::sandbox::model::Restriction::Enum restriction MEMBER restriction_ WRITE setRestriction NOTIFY restrictionChanged)

  friend bool(::operator==) (const Accuracy& lhs, const Accuracy& rhs);
  friend bool(::operator!=) (const Accuracy& lhs, const Accuracy& rhs);
  friend int(::compare) (const Accuracy& first, const Accuracy& second);

  friend QSettings* read(
    QSettings* settings,
    const QString& key,
    Accuracy& accuracy);
  friend QSettings* read(
    QSettings* settings,
    const QString& key,
    Accuracy& accuracy,
    const Accuracy& default);
  friend QSettings* read(
    QSettings* settings,
    const QString& key,
    Accuracy& accuracy,
    const int& precision);
  friend QSettings* write(
    QSettings* settings,
    const QString& key,
    const Accuracy& accuracy);

public:
  explicit Accuracy(QObject* parent = nullptr);
  Accuracy(
    const ::qt::sandbox::model::Restriction::Enum& restriction,
    const Range& range,
    const int& precision,
    QObject* parent = nullptr);
  Accuracy(
    const int& precision,
    QObject* parent = nullptr);
  Accuracy(const Accuracy& accuracy);
  Accuracy& operator=(const Accuracy& accuracy);

  Accuracy& set(const Accuracy& accuracy);
  void set(
    const ::qt::sandbox::model::Restriction::Enum& restriction,
    const Range& range,
    const int& precision);
  void set(const int& precision);

  Range* range();

signals:
  void rangeChanged();
  void restrictionChanged();

public slots:
  void setRange(const Range* value);
  void setRestriction(const ::qt::sandbox::model::Restriction::Enum& value);

private:
  void setRange(const Range& value);
  Range range_;
  ::qt::sandbox::model::Restriction::Enum restriction_;
};

Accuracy make_accuracy(
  const ::qt::sandbox::model::Restriction::Enum& restriction,
  const Range& range,
  const int& precision);

Accuracy make_accuracy(const int& precision);

}
}

#endif
