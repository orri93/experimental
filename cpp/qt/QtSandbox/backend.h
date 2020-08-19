#ifndef QT_SANDBOX_BACKEND_H_
#define QT_SANDBOX_BACKEND_H_

#include <memory>

#include <QTimer>
#include <QtCore/QObject>
#include <QtCharts/QAbstractSeries>
#include <QtQml/QQmlContext>

#include <configuration.h>

QT_BEGIN_NAMESPACE
class QQuickView;
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

namespace backend {
bool create(QQmlContext& context);
}

namespace qt {
namespace sandbox {

class Backend : public QObject {
  Q_OBJECT
public:
  explicit Backend(QQmlContext& context, QObject* parent = nullptr);
  ~Backend();

  /* Configuration Access */
  Q_PROPERTY(Configuration* configuration READ configuration NOTIFY configurationChanged)
  Q_PROPERTY(Ui* ui READ ui NOTIFY uiChanged)

  /* Controller input items */
  Q_PROPERTY(double setpoint READ setpoint WRITE setSetpoint NOTIFY setpointChanged)

  /* Controller tuning items */
  Q_PROPERTY(float kp READ kp WRITE setKp NOTIFY kpChanged)
  Q_PROPERTY(QString kpText READ kpText WRITE setKpText NOTIFY kpTextChanged)

  /* Controller output items */
  Q_PROPERTY(float integral READ integral NOTIFY integralChanged)
  Q_PROPERTY(QString integralText READ integralText NOTIFY integralTextChanged)

  /* Models */
  Q_PROPERTY(::qt::sandbox::model::Restriction* restrictionModel READ restrictionModel NOTIFY restrictionModelChanged)

  /* Other items */

  bool initialize();

  //Q_INVOKABLE int update(
  //  QAbstractSeries* output,
  //  QAbstractSeries* temperature,
  //  QAbstractSeries* setpoints);
  Q_INVOKABLE void panelCompleted();
  Q_INVOKABLE void applyUiDialog();
  Q_INVOKABLE void rejectUiDialog();
  //Q_INVOKABLE QString toString(const Restriction::Enum& restriction);

  /* Configuration Access */
  Configuration* configuration();
  Ui* ui();

  /* Controller input items */
  const double& setpoint() const;

  /* Controller tuning items */
  const float& kp() const;
  const QString kpText() const;

  /* Controller output items */
  const float& integral() const;
  const QString integralText() const;

  /* Models */
  ::qt::sandbox::model::Restriction* restrictionModel();

  /* Other items */

signals:
  void completed();

  /* Configuration Access */
  void configurationChanged();
  void uiChanged();

  /* Controller input items */
  void setpointChanged();

  /* Controller tuning items */
  void kpChanged();
  void kpTextChanged();

  /* Controller output items */
  void integralChanged();
  void integralTextChanged();

  /* Models */
  void restrictionModelChanged();

  /* Other items */

Q_SIGNALS:
  //  void quit();
  //  void exit(int retCode);

public Q_SLOTS:
  //  bool close();

public slots:

  /* Controller input items */
  void setSetpoint(const double& value);
  /* Controller tuning items */
  void setKp(const float& value);
  void setKpText(const QString& value);

private slots:

private:
  typedef QVector<QPointF> VectorList;
  typedef std::unique_ptr<Configuration> ConfigurationPointer;
  typedef std::unique_ptr<std::ofstream> OutputFilePointer;

  void applyConfiguration();

  /* Controller output items */
  void setIntegral(const float& value);

  /* Controller input items */
  bool writeSetpoint(const float& setpoint);

  /* Controller tuning items */
  bool writeKp(const float& kp);

  /* Other items */

  void notify();

  QQmlContext& context_;
  VectorList setpointsList_;
  VectorList temperatureList_;
  VectorList outputsList_;
  int count_;

  ConfigurationPointer configuration_;
  UiPointer uiForDialog_;

  /* Controller input items */
  double setpoint_;

  /* Controller tuning items */
  float kp_;

  /* Controller output items */
  float integral_;

  /* Models */
  ::qt::sandbox::model::Restriction restrictionmodel_;

};

}
}

#endif
