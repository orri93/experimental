#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QtCharts/QAbstractSeries>
#include <QtQml/QQmlContext>

#include <configuration.h>

//QT_BEGIN_NAMESPACE
//class QQuickView;
//QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class Backend : public QObject {
  Q_OBJECT
public:
  explicit Backend(QObject* parent = nullptr);
  ~Backend();

  Q_PROPERTY(bool isConnected READ isConnected NOTIFY isConnectedChanged)
  Q_PROPERTY(int refreshInterval READ refreshInterval WRITE setRefreshInterval NOTIFY refreshIntervalChanged)

  Q_PROPERTY(bool boolean READ boolean WRITE setBoolean NOTIFY booleanChanged)
  Q_PROPERTY(double real READ real WRITE setReal NOTIFY realChanged)
  Q_PROPERTY(int integer READ integer WRITE setInteger NOTIFY integerChanged)
  Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

  bool initialize(/* QQmlContext* context */);

  Q_INVOKABLE bool commandA();
  Q_INVOKABLE bool commandB();

  Q_INVOKABLE int update(
    QAbstractSeries* output,
    QAbstractSeries* temperature,
    QAbstractSeries* setpoints);

  const bool& isConnected() const;
  const int& refreshInterval() const;

  const bool& boolean() const;
  const double& real() const;
  const int& integer() const;
  const QString& text() const;

signals:
  void isConnectedChanged();
  void refreshIntervalChanged();

  void booleanChanged();
  void realChanged();
  void integerChanged();
  void textChanged();

Q_SIGNALS:
//  void quit();
//  void exit(int retCode);

public Q_SLOTS:
//  bool close();

public slots:
  void setRefreshInterval(const int& value);

  void setBoolean(const bool& value);
  void setReal(const double& value);
  void setInteger(const int& value);
  void setText(const QString& value);

private:
  typedef std::unique_ptr<Configuration> ConfigurationPointer;

  void setIsConnected(const bool& value);

  ConfigurationPointer configuration_;
  int refreshInterval_;

  bool isConnected_;

  bool boolean_;
  double real_;
  int integer_;
  QString text_;
};

#endif // BACKEND_H
