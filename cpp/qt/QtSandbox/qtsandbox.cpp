#include <cstdlib>

#include <memory>
#include <iostream>

#include <QtWidgets/QApplication>

#include <QQmlApplicationEngine>

#include <backend.h>

int main(int argc, char *argv[])
{
  // Qt Charts uses Qt Graphics View Framework for drawing, therefore QApplication must be used.
  QApplication app(argc, argv);

  app.setOrganizationName("Geirmundur Orri Sigurdsson");
  app.setOrganizationDomain("none.is");
  app.setApplicationName("Qt Sandbox");

  QQmlApplicationEngine engine;
  engine.addImportPath("qrc:/");
  QQmlContext* context = engine.rootContext();
  if (context != nullptr) {
    if(backend::create(*context)) {
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
    }
  } else {
    std::cerr << "The QML Root Context is undefined";
  }
  return EXIT_FAILURE;
}
