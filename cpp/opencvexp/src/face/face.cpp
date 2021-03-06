#include <cstdlib>

#include <iostream>

#include <QApplication>
#include <QCommandLineParser>

#include <viewer.h>

#define DEVICE_ID 0

int main(int argc, char* argv[]) {
  int device;
  QApplication application(argc, argv);
  QGuiApplication::setApplicationDisplayName(FaceViewer::tr("Face Viewer"));
  QCommandLineParser commandLineParser;
  commandLineParser.addHelpOption();
  commandLineParser.addPositionalArgument(FaceViewer::tr("[Device]"), FaceViewer::tr("Capture device to open."));
  commandLineParser.process(QCoreApplication::arguments());
  if (commandLineParser.positionalArguments().isEmpty()) {
    device = DEVICE_ID;
  } else {
    bool isok = false;
    QString front = commandLineParser.positionalArguments().front();
    device = front.toInt(&isok);
    if (!isok) {
      std::cerr << "Failed to parse '" << front.toStdString()
        << "' as the Video Capture device id" << std::endl;
      return EXIT_FAILURE;
    }
  }
  FaceViewer viewer(device);
  viewer.show();
  return application.exec();

  //capture.open(device);
  //std::cerr << "Failed to open the default Video Capture device" << std::endl;
}
