#include <QtWidgets/QApplication>

#include <QQmlApplicationEngine>

#include <backend.h>

int main(int argc, char *argv[])
{
  // Qt Charts uses Qt Graphics View Framework for drawing, therefore QApplication must be used.
  QApplication app(argc, argv);

  QQmlApplicationEngine engine;

  Backend backend/*( &engine )*/;

  engine.rootContext()->setContextProperty("backend", &backend);

  const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
  QObject::connect(
    &engine,
    &QQmlApplicationEngine::objectCreated,
    &app,
    [url](QObject *obj, const QUrl &objUrl) {
      if (!obj && url == objUrl)
        QCoreApplication::exit(-1);
    },
    Qt::QueuedConnection);
  engine.load(url);

  return app.exec();
}
