#ifndef QT_SANDBOX_RANGE_H_
#define QT_SANDBOX_RANGE_H_

#include <QObject>
#include <QSettings>

#define KEY_BB_RANGE_FROM "From"
#define KEY_BB_RANGE_TO "To"

#define DEFAULT_BB_RANGE_FROM 0.0
#define DEFAULT_BB_RANGE_TO 10.0

namespace qt { namespace sandbox { class Range; } }

bool operator==(
  const ::qt::sandbox::Range& lhs,
  const ::qt::sandbox::Range& rhs);
bool operator!=(
  const ::qt::sandbox::Range& lhs,
  const ::qt::sandbox::Range& rhs);

int compare(
  const ::qt::sandbox::Range& first,
  const ::qt::sandbox::Range& second);


namespace qt {
namespace sandbox {

QSettings* read(QSettings* settings, const QString& key, Range& range);
QSettings* read(
  QSettings* settings,
  const QString& key,
  Range& range,
  const Range& default);
QSettings* read(
  QSettings* settings,
  const QString& key,
  Range& range,
  const double& from,
  const double& to);
QSettings* write(QSettings* settings, const QString& key, const Range& range);

class Range : public QObject {
  Q_OBJECT

  Q_PROPERTY(double from MEMBER from_ WRITE setFrom NOTIFY fromChanged)
  Q_PROPERTY(double to MEMBER to_ WRITE setTo NOTIFY toChanged)

  friend bool(::operator==) (const Range& lhs, const Range& rhs);
  friend bool(::operator!=) (const Range& lhs, const Range& rhs);
  friend int(::compare) (const Range& first, const Range& second);

  friend QSettings* read(QSettings* settings, const QString& key, Range& range);
  friend QSettings* read(
    QSettings* settings,
    const QString& key,
    Range& range,
    const Range& default);
  friend QSettings* read(
    QSettings* settings,
    const QString& key,
    Range& range,
    const double& from,
    const double& to);
  friend QSettings* write(
    QSettings* settings,
    const QString& key,
    const Range& range);

public:
  explicit Range(QObject* parent = nullptr);

  Range(
    const double& from,
    const double& to,
    QObject* parent = nullptr);
  Range(const Range& range);

  virtual Range& operator=(const Range& range);

  virtual Range& set(const Range& range);
  void set(const double& from, const double& to);

signals:
  void fromChanged();
  void toChanged();

public slots:
  void setFrom(const double& value);
  void setTo(const double& value);

protected:
  double from_;
  double to_;
};

Range make_range(const double& from, const double& to);

}
}

#endif
