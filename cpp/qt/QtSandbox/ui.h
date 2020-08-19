#ifndef QT_SANDBOX_UI_H_
#define QT_SANDBOX_UI_H_

#include <memory>

#include <QObject>
#include <QSettings>
#include <QMetaType>
#include <QDebug>

#include <accuracy.h>
#include <format.h>

namespace qt { namespace sandbox { class Ui; } }

bool operator==(const ::qt::sandbox::Ui& lhs, const ::qt::sandbox::Ui& rhs);
bool operator!=(const ::qt::sandbox::Ui& lhs, const ::qt::sandbox::Ui& rhs);

int compare(const ::qt::sandbox::Ui& first, const ::qt::sandbox::Ui& second);

namespace qt {
namespace sandbox {

class Ui : public QObject {
  Q_OBJECT

  /* Controller input items */
  Q_PROPERTY(Accuracy* setpoint READ setpoint WRITE setSetpoint NOTIFY setpointChanged)

  /* Controller output items */
  Q_PROPERTY(Format* integral READ integral WRITE setIntegral NOTIFY integralChanged)

  /* PID configuration */
  Q_PROPERTY(Accuracy* kp READ kp WRITE setKp NOTIFY kpChanged)

  /* Tuning configuration */
  Q_PROPERTY(Range* kpTuning READ kpTuning WRITE setKpTuning NOTIFY kpTuningChanged)

  friend bool(::operator==) (const Ui&, const Ui&);
  friend bool(::operator!=) (const Ui&, const Ui&);
  friend int(::compare) (const Ui&, const Ui&);

public:

  explicit Ui(QObject* parent = nullptr);

  Ui(const Ui& ui);

  Ui& operator=(const Ui& ui);

  Ui& set(const Ui& ui);

  QSettings* read(QSettings* settings);
  QSettings* write(QSettings* settings);

  /* Controller input items */
  Accuracy* setpoint();

  /* Controller output items */
  Format* integral();

  /* PID configuration */
  Accuracy* kp();

  /* Tuning configuration */
  Range* kpTuning();

signals:
  /* Controller input items */
  void setpointChanged();

  /* Controller output items */
  void integralChanged();

  /* PID configuration */
  void kpChanged();

  /* Tuning configuration */
  void kpTuningChanged();


public slots:
  /* Controller input items */
  void setSetpoint(const Accuracy* accuracy);

  /* Controller output items */
  void setIntegral(const Format* number);

  /* PID configuration */
  void setKp(const Accuracy* number);

  /* Tuning configuration */
  void setKpTuning(const Range* range);

protected:
  /* Controller input items */
  Accuracy setpoint_;

  /* Controller output items */
  Format integral_;

  /* PID configuration */
  Accuracy kp_;

  /* Tuning configuration */
  Range kpTuning_;

};

typedef std::unique_ptr<Ui> UiPointer;

}
}


#endif
