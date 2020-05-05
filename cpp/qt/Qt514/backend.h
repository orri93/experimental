#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QVariant>
#include <QtCharts/QAbstractSeries>
#include <QQmlApplicationEngine>
#include <QtQml/QQmlContext>

#include <configuration.h>

#include <items.h>

//QT_BEGIN_NAMESPACE
//class QQuickView;
//QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

namespace backend {
bool create(QQmlApplicationEngine& engine);
}

class Backend : public Items {
  Q_OBJECT

  Q_PROPERTY(bool isConnected READ isConnected NOTIFY isConnectedChanged)

  Q_PROPERTY(int interval READ interval WRITE setInterval NOTIFY intervalChanged)
  Q_PROPERTY(int intervalIndex READ intervalIndex WRITE setIntervalIndex NOTIFY intervalIndexChanged)

  Q_PROPERTY(bool boolean READ boolean WRITE setBoolean NOTIFY booleanChanged)
  Q_PROPERTY(double real READ real WRITE setReal NOTIFY realChanged)
  Q_PROPERTY(int integer READ integer WRITE setInteger NOTIFY integerChanged)
  Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

public:
  explicit Backend(QQmlApplicationEngine& engine, QObject* parent = nullptr);
  ~Backend();

  bool initialize(/* QQmlContext* context */);

  Q_INVOKABLE int update(
    QAbstractSeries* output,
    QAbstractSeries* temperature,
    QAbstractSeries* setpoints);
  Q_INVOKABLE void panelCompleted();
  Q_INVOKABLE bool commandA();
  Q_INVOKABLE bool commandB();

  const bool& isConnected() const;

  const int intervalIndex() const;

signals:
  void completed();

  void isConnectedChanged();
  
  void intervalChanged();
  void intervalIndexChanged();

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
  void setInterval(const int& value);
  void setIntervalIndex(const int& value);

  void setBoolean(const bool& value);
  void setReal(const double& value);
  void setInteger(const int& value);
  void setText(const QString& value);

private:
  typedef std::unique_ptr<Configuration> ConfigurationPointer;

  void setIsConnected(const bool& value);
  void apply();

  QQmlApplicationEngine& engine_;
  ConfigurationPointer configuration_;
  QVariant intervalmodel_;
  bool ispanelcompleted_;

  bool isConnected_;
};

#endif // BACKEND_H
