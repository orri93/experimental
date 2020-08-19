#ifndef QT_SANDBOX_CONFIGURATION_H_
#define QT_SANDBOX_CONFIGURATION_H_

#include <memory>
#include <vector>

#include <QObject>
#include <QSettings>
#include <QMetaType>
#include <QFileSystemWatcher>
#include <QDebug>

#include <ui.h>

#define GOS_CONFIGURATION_FILE_PATH "configuration.ini"

namespace qt { namespace sandbox { class Configuration; } }

bool operator==(
  const ::qt::sandbox::Configuration& lhs, 
  const ::qt::sandbox::Configuration& rhs);
bool operator!=(
  const ::qt::sandbox::Configuration& lhs,
  const ::qt::sandbox::Configuration& rhs);

int compare(
  const ::qt::sandbox::Configuration& first,
  const ::qt::sandbox::Configuration& second);

namespace qt {
namespace sandbox {

class Configuration : public QObject {
  Q_OBJECT

    /* UI configuration */
    Q_PROPERTY(Ui* ui READ ui NOTIFY uiChanged)

protected:
  const QSettings::Format SettingsFormat = QSettings::IniFormat;

public:
  explicit Configuration(const QString& filepath, QObject* parent = nullptr);
  explicit Configuration(QObject* parent = nullptr);

  virtual QSettings* initialize();

  virtual QSettings* read(const bool& sync = false);
  virtual QSettings* write(const bool& sync = false);

  /* Ui configuration */
  Ui* ui();

  bool applyUiDialog(Ui* ui);

signals:
  void uiChanged();

public slots:

private slots:

private:
  typedef std::unique_ptr<QSettings> SettingsPointer;
  typedef std::unique_ptr<QFileSystemWatcher> WatcherPointer;

  /* Writing */
  QSettings* startWriting();
  void writeUi();
  virtual QSettings* completeWriting(const bool& sync = false);

  /* Ui configuration */
  Ui ui_;

  /* Private */
  SettingsPointer settings_;
  WatcherPointer watcher_;

  QString filepath_;
};

}
}

#endif
