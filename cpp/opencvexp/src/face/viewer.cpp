#include <iostream>

#include <QApplication>
#include <QGuiApplication>
#include <QColorSpace>
#include <QMessageBox>
#include <QMenuBar>
#include <QScreen>

#include <viewer.h>

#define GOS_CV_VIEWER_TIMER 100

// #define GOS_CV_DEFAULT_FACE_CASCADE_FILE "C:\\lib\\opencv\\build\\etc\\haarcascades\\haarcascade_frontalcatface.xml"
#define GOS_CV_DEFAULT_FACE_CASCADE_FILE "C:\\lib\\opencv\\build\\etc\\haarcascades\\haarcascade_frontalface_alt.xml"
#define GOS_CV_DEFAULT_EYES_CASCADE_FILE "C:\\lib\\opencv\\build\\etc\\haarcascades\\haarcascade_eye_tree_eyeglasses.xml"

FaceViewer::FaceViewer(const int& device, QWidget* parent) :
  QMainWindow(parent),
  _device(device),
  _scaleFactor(1.0),
  _imageLabel(new QLabel),
  _scrollArea(new QScrollArea),
  _optionLayout(new QVBoxLayout), 
  _mainLayout(new QHBoxLayout) {
  initialize();
}

void FaceViewer::open() {
  if (!_capture.isOpened()) {
    _capture.open(_device);
    if (!_capture.isOpened()) {
      QMessageBox::critical(this, tr("Open Video Capture Device"),
        tr("<p>Failed to open <b>Video Capture Device</b></p>"));
    }
  } else {
    QMessageBox::warning(this, tr("Open Video Capture Device"),
      tr("<p>A <b>Video Capture Device</b> is already opened</p>"));
  }
}

void FaceViewer::zoomIn() {
  scaleImage(1.25);
}

void FaceViewer::zoomOut() {
  scaleImage(0.8);
}

void FaceViewer::normalSize() {
  _imageLabel->adjustSize();
  _scaleFactor = 1.0;
}

void FaceViewer::fitToWindow() {
  bool fitToWindow = _fitToWindowAct->isChecked();
  _scrollArea->setWidgetResizable(fitToWindow);
  if (!fitToWindow) {
    normalSize();
  }
  updateActions();
}

void FaceViewer::about() {
  QMessageBox::about(this, tr("About Image Viewer"),
    tr("<p>The <b>Image Viewer</b> example shows how to combine QLabel "
      "and QScrollArea to display an image. QLabel is typically used "
      "for displaying a text, but it can also display an image. "
      "QScrollArea provides a scrolling view around another widget. "
      "If the child widget exceeds the size of the frame, QScrollArea "
      "automatically provides scroll bars. </p><p>The example "
      "demonstrates how QLabel's ability to scale its contents "
      "(QLabel::scaledContents), and QScrollArea's ability to "
      "automatically resize its contents "
      "(QScrollArea::widgetResizable), can be used to implement "
      "zooming and scaling features. </p><p>In addition the example "
      "shows how to use QPainter to print an image.</p>"));
}

void FaceViewer::timer() {
  if (_capture.isOpened()) {
    _capture >> _frame;

    if (!_frame.empty()) {
      _clone = _frame.clone();

      if (!_context.face.classifier.empty()) {
        //::gos::cv::detection::detecta(_clone, _context);
        ::gos::cv::detection::detectb(_clone, _context);
      }

      QImage frame = QImage(
        (uchar*)_clone.data,
        _clone.cols,
        _clone.rows,
        _clone.step,
        QImage::Format_BGR888);

      setFrame(frame);

    }
  }
}

void FaceViewer::initialize() {
  gos::cv::detection::initialize(_context);

  _context.face.classifier.load(GOS_CV_DEFAULT_FACE_CASCADE_FILE);
  _context.eyes.classifier.load(GOS_CV_DEFAULT_EYES_CASCADE_FILE);

  _imageLabel->setBackgroundRole(QPalette::Base);
  _imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  _imageLabel->setScaledContents(true);

  _scrollArea->setBackgroundRole(QPalette::Dark);
  _scrollArea->setWidget(_imageLabel);
  _scrollArea->setVisible(false);

  _mainLayout->addLayout(_optionLayout);
  _mainLayout->addWidget(_scrollArea);

  setCentralWidget(_scrollArea);
  //setLayout(_mainLayout);

  QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
  QAction* openAct = fileMenu->addAction(tr("&Open..."), this, &FaceViewer::open);
  openAct->setShortcut(QKeySequence::Open);

  QMenu* viewMenu = menuBar()->addMenu(tr("&View"));

  _zoomInAct = viewMenu->addAction(tr("Zoom &In (25%)"), this, &FaceViewer::zoomIn);
  _zoomInAct->setShortcut(QKeySequence::ZoomIn);
  _zoomInAct->setEnabled(false);

  _zoomOutAct = viewMenu->addAction(tr("Zoom &Out (25%)"), this, &FaceViewer::zoomOut);
  _zoomOutAct->setShortcut(QKeySequence::ZoomOut);
  _zoomOutAct->setEnabled(false);

  _normalSizeAct = viewMenu->addAction(tr("&Normal Size"), this, &FaceViewer::normalSize);
  _normalSizeAct->setShortcut(tr("Ctrl+S"));
  _normalSizeAct->setEnabled(false);

  viewMenu->addSeparator();

  _fitToWindowAct = viewMenu->addAction(tr("&Fit to Window"), this, &FaceViewer::fitToWindow);
  _fitToWindowAct->setEnabled(false);
  _fitToWindowAct->setCheckable(true);
  _fitToWindowAct->setShortcut(tr("Ctrl+F"));

  QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));

  helpMenu->addAction(tr("&About"), this, &FaceViewer::about);
  helpMenu->addAction(tr("About &Qt"), &QApplication::aboutQt);

  resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);

  connect(&_timer, &QTimer::timeout, this, &FaceViewer::timer);
  _timer.start(GOS_CV_VIEWER_TIMER);

  open();
}

void FaceViewer::updateActions() {
  _zoomInAct->setEnabled(!_fitToWindowAct->isChecked());
  _zoomOutAct->setEnabled(!_fitToWindowAct->isChecked());
  _normalSizeAct->setEnabled(!_fitToWindowAct->isChecked());
}

void FaceViewer::setFrame(const QImage& frame) {
  _image = frame;

  if (_image.colorSpace().isValid()) {
    _image.convertToColorSpace(QColorSpace::SRgb);
  }

  _imageLabel->setPixmap(QPixmap::fromImage(_image));
  _scaleFactor = 1.0;

  _scrollArea->setVisible(true);
  _fitToWindowAct->setEnabled(true);
  updateActions();

  if (!_fitToWindowAct->isChecked()) {
    _imageLabel->adjustSize();
  }
}

void FaceViewer::scaleImage(const double& factor) {
  _scaleFactor *= factor;

  _imageLabel->resize(_scaleFactor * _imageLabel->pixmap(Qt::ReturnByValue).size());

  adjustScrollBar(_scrollArea->horizontalScrollBar(), factor);
  adjustScrollBar(_scrollArea->verticalScrollBar(), factor);

  _zoomInAct->setEnabled(_scaleFactor < 3.0);
  _zoomOutAct->setEnabled(_scaleFactor > 0.333);
}

void FaceViewer::adjustScrollBar(QScrollBar* scrollBar, const double& factor) {

}
