#ifndef QT_SANDBOX_FORMAT_H_
#define QT_SANDBOX_FORMAT_H_

#include <QObject>
#include <QSettings>

#define KEY_BB_FORMAT_PRECISION "Precision"

#define DEFAULT_BB_FORMAT_PRECISION 0

namespace qt { namespace sandbox { class Format; } }

int compare(
  const ::qt::sandbox::Format& first,
  const ::qt::sandbox::Format& second);

bool operator==(
  const ::qt::sandbox::Format& lhs,
  const ::qt::sandbox::Format& rhs);
bool operator!=(
  const ::qt::sandbox::Format& lhs,
  const ::qt::sandbox::Format& rhs);

namespace qt {
namespace sandbox {

QSettings* read(QSettings* settings, const QString& key, Format& format);
QSettings* read(
  QSettings* settings,
  const QString& key,
  Format& format,
  const int& precision);
QSettings* write(QSettings* settings, const QString& key, const Format& format);

class Format : public QObject {
  Q_OBJECT

  Q_PROPERTY(int precision READ precision WRITE setPrecision NOTIFY precisionChanged)

  friend bool(::operator==) (const Format& lhs, const Format& rhs);
  friend bool(::operator!=) (const Format& lhs, const Format& rhs);
  friend int(::compare) (const Format& first, const Format& second);

  friend QSettings* read(
    QSettings* settings,
    const QString& key,
    Format& format);
  friend QSettings* read(
    QSettings* settings,
    const QString& key,
    Format& format,
    const int& precision);
  friend QSettings* write(
    QSettings* settings,
    const QString& key,
    const Format& format);

public:
  explicit Format(QObject* parent = nullptr);
  Format(
    const int& precision,
    QObject* parent = nullptr);
  Format(const Format& format);
  Format& operator=(const Format& format);

  Format& set(const Format& format);
  virtual void set(const int& precision);

  const int& precision() const;

signals:
  void precisionChanged();

public slots:
  void setPrecision(const int& value);

protected:
  int precision_;
};

Format make_format(const int& precision);

}
}

#endif
