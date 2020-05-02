#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <memory>

#include <QObject>
#include <QSettings>
#include <QMetaType>
#include <QDebug>

#define GOS_CONFIGURATION_FILE_PATH "Configuration.ini"

class Configuration : public QObject {
  Q_OBJECT

  /* Timer */
  Q_PROPERTY(int refreshInterval READ refreshInterval WRITE setRefreshInterval NOTIFY refreshIntervalChanged)

  /* Experiment */
  Q_PROPERTY(bool boolean READ boolean WRITE setBoolean NOTIFY booleanChanged)
  Q_PROPERTY(double real READ real WRITE setReal NOTIFY realChanged)
  Q_PROPERTY(int integer READ integer WRITE setInteger NOTIFY integerChanged)
  Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

public:
  Configuration();

  explicit Configuration(const QString& filepath, QObject* parent = nullptr);
  explicit Configuration(QObject* parent = nullptr);

  virtual QSettings* read();
  virtual QSettings* write(const bool& sync);

  /*
   * Value access methods
   */

   /* Timer configuration */
  const int& refreshInterval() const;

   /* Experiment configuration */
  const bool& boolean() const;
  const double& real() const;
  const int& integer() const;
  const QString& text() const;

signals:
  /* Timer configuration */
  void refreshIntervalChanged();

  /* Experiment configuration */
  void booleanChanged();
  void realChanged();
  void integerChanged();
  void textChanged();

public slots:
  /* Timer configuration */
  void setRefreshInterval(const int& value);

  /* Experiment configuration */
  void setBoolean(const bool& value);
  void setReal(const double& value);
  void setInteger(const int& value);
  void setText(const QString& value);

protected:
  const QSettings::Format SettingsFormat = QSettings::IniFormat;

private:
  typedef std::unique_ptr<QSettings> SettingsPointer;

  bool create();

  SettingsPointer settings_;

  QString filepath_;

  /* Timer configuration */
  int refreshInterval_;

  /* Experiment configuration */
  bool boolean_;
  double real_;
  int integer_;
  QString text_;

};

namespace initialize {
bool configuration(Configuration& configuration);
}

#endif // CONFIGURATION_H
