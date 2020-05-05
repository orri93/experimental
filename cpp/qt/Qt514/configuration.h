#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <memory>
#include <functional>

#include <QObject>
#include <QSettings>
#include <QMetaType>
#include <QFileSystemWatcher>
#include <QDebug>

#include <items.h>
#include <types.h>

#define GOS_CONFIGURATION_FILE_PATH "Configuration.ini"

namespace configuration {
enum class mode { normal, write, initializing };
}

class Configuration : public Items {
  Q_OBJECT

  /* Timer */
  Q_PROPERTY(int interval READ interval WRITE setInterval NOTIFY intervalChanged)

  /* Experiment */
  Q_PROPERTY(bool boolean READ boolean WRITE setBoolean NOTIFY booleanChanged)
  Q_PROPERTY(double real READ real WRITE setReal NOTIFY realChanged)
  Q_PROPERTY(int integer READ integer WRITE setInteger NOTIFY integerChanged)
  Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

public:
  explicit Configuration(const QString& filepath, QObject* parent = nullptr);
  explicit Configuration(QObject* parent = nullptr);

  virtual QSettings* initialize(const bool& watcher);

  virtual QSettings* read(const bool& sync = false);
  virtual QSettings* write(const bool& sync = false);

  const configuration::mode& mode() const;
  void setMode(const configuration::mode& mode);

  /*
   * Value access methods
   */

signals:
  /* Timer configuration */
  void intervalChanged();

  /* Experiment configuration */
  void booleanChanged();
  void realChanged();
  void integerChanged();
  void textChanged();

public slots:
  /* Timer configuration */
  void setInterval(const int& value);

  /* Experiment configuration */
  void setBoolean(const bool& value);
  void setReal(const double& value);
  void setInteger(const int& value);
  void setText(const QString& value);

protected:
  const QSettings::Format SettingsFormat = QSettings::IniFormat;

private slots:
  void onFileChanged(const QString& path);

private:
  typedef std::unique_ptr<QSettings> SettingsPointer;
  typedef std::unique_ptr<QFileSystemWatcher> WatcherPointer;

  void handle(std::function<void()> & changed);
  void handle(std::function<void()> & changed, std::function<void()> & write);

  void create();

  /* Writing */
  QSettings* startWriting();
  void writeTimers();
  void writeExperiment();
  virtual QSettings* completeWriting(const bool& sync = false);

  std::function<void()> fWriteTimers_;
  std::function<void()> fWriteExperiment_;

  SettingsPointer settings_;
  WatcherPointer watcher_;

  QString filepath_;
  configuration::mode mode_;
};

#endif // CONFIGURATION_H
