#include <cstdlib>

#include <memory>

#include <QtWidgets/QApplication>

#include <QQmlApplicationEngine>

#include <backend.h>

#include <validation/float.h>

int main(int argc, char *argv[])
{
  // Qt Charts uses Qt Graphics View Framework for drawing, therefore QApplication must be used.
  QApplication app(argc, argv);

  qmlRegisterType<validation::FloatValidator>(
    GOS_QML_FLOAT_VALIDATOR, 1, 0, GOS_QML_FLOAT_VALIDATOR);

  QQmlApplicationEngine engine;

  if (backend::create(engine)) {
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(
      &engine,
      &QQmlApplicationEngine::objectCreated,
      &app,
      [url](QObject* obj, const QUrl& objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);
    engine.load(url);
    return app.exec();
  } else {
    return EXIT_FAILURE;
  }
}
