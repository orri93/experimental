#ifndef GOS_EXP_QT_VIEWER_H_
#define GOS_EXP_QT_VIEWER_H_

#include <opencv2/videoio.hpp>

#include <QImage>
#include <QTimer>
#include <QLabel>
#include <QWidget>
#include <QMainWindow>
#include <QScrollArea>
#include <QtWidgets>

#include <cv/detection.h>

class FaceViewer : public QMainWindow {
  Q_OBJECT
public:
  FaceViewer(const int& device, QWidget* parent = nullptr);

private slots:
  void open();
  void zoomIn();
  void zoomOut();
  void normalSize();
  void fitToWindow();
  void about();
  void timer();

private:
  void initialize();
  void updateActions();
  void setFrame(const QImage& frame);
  void scaleImage(const double& factor);
  void adjustScrollBar(QScrollBar* scrollBar, const double& factor);

  int _device;
  double _scaleFactor;

  QImage _image;
  QTimer _timer;

  QLabel* _imageLabel;
  QScrollArea* _scrollArea;

  QVBoxLayout* _optionLayout;
  QHBoxLayout* _mainLayout;

  QAction* _zoomInAct;
  QAction* _zoomOutAct;
  QAction* _normalSizeAct;
  QAction* _fitToWindowAct;

  cv::Mat _frame;
  cv::Mat _clone;
  cv::VideoCapture _capture;

  gos::cv::detection::context _context;
};

#endif
