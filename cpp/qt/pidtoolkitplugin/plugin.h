#ifndef GOS_PID_TUNING_UI_PLUGIN_H_
#define GOS_PID_TUNING_UI_PLUGIN_H_

#include <QtQml/QQmlExtensionPlugin>

class GosTryingPlugin : public QQmlExtensionPlugin {
  Q_OBJECT
    Q_PLUGIN_METADATA(IID "Gos.Trying/1.0")
public:

  /**
   * @brief registerTypes Overrided function that should register all
   * C++ classes exported by this plugin.
   * @param uri           Plugin uri.
   */
  void registerTypes(const char* uri) override;
};

#endif
