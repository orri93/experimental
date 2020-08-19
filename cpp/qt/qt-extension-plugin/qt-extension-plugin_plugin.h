#ifndef QTEXTENSIONPLUGIN_PLUGIN_H
#define QTEXTENSIONPLUGIN_PLUGIN_H

#include <QQmlExtensionPlugin>

class Qt-extension-pluginPlugin : public QQmlExtensionPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
  void registerTypes(const char *uri) override;
};

#endif // QTEXTENSIONPLUGIN_PLUGIN_H
