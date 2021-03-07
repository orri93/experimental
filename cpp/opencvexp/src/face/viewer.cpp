#include <mutex>

#include <QApplication>
#include <QGuiApplication>
#include <QColorSpace>
#include <QMessageBox>
#include <QMenuBar>
#include <QScreen>

#include <lock.h>
#include <viewer.h>

FaceViewer::FaceViewer(const int& device, QWidget* parent) :
  QMainWindow(parent),
  _device(device),
  _scaleFactor(1.0),
  _imageLabel(nullptr),
  _scrollArea(nullptr),
  _statusBar(nullptr),
  _controller(nullptr) {
  initialize();
}

void FaceViewer::open() {
  if (!_capture.isOpened()) {
    _capture.open(_device);
    if (!_capture.isOpened()) {
      QMessageBox::critical(this, tr("Open Video Capture Device"),
        tr("<p>Failed to open the <b>Video Capture Device</b></p>"));
    }
  } else {
    QMessageBox::warning(this, tr("Open Video Capture Device"),
      tr("<p>A <b>Video Capture Device</b> is already opened</p>"));
  }
}

void FaceViewer::close() {
  if (_capture.isOpened()) {
    _capture.release();
    if (_capture.isOpened()) {
      QMessageBox::critical(this, tr("Close Video Capture Device"),
        tr("<p>Failed to close the <b>Video Capture Device</b></p>"));
    }
  } else {
    QMessageBox::warning(this, tr("Close Video Capture Device"),
      tr("<p>A <b>Video Capture Device</b> is already closed</p>"));
  }
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
  QMessageBox::about(this, tr("About Face Viewer"),
    tr("<p>The <b>Face Viewer</b> experiment uses "
      "OpenCV to capture and detect faces and eyes</p>"));
}

void FaceViewer::timer() {
  QString message;
  if (_capture.isOpened()) {
    _capture >> _frame;

    if (!_frame.empty()) {
      _clone = _frame.clone();

      {
        ::std::lock_guard guard(::gos::qt::lock::context_);
        if (!_context.face.classifier.empty()) {
          int faceCount;
          switch (_context.method) {
          case ::gos::cv::detection::method::a:
            ::gos::cv::detection::detecta(_clone, _context);
            break;
          case ::gos::cv::detection::method::b:
            ::gos::cv::detection::detectb(_clone, _context);
            break;
          }
          faceCount = ::gos::cv::detection::detectb(_clone, _context);
          if (faceCount > 0) {
            message = QString("Detecting %1 faces").arg(QString::number(faceCount));
          } else {
            message = tr("No faces detected");
          }
        } else {
          message = tr("No classifier has been loaded");
        }
      }

      QImage frame = QImage(
        (uchar*)_clone.data,
        _clone.cols,
        _clone.rows,
        _clone.step,
        QImage::Format_BGR888);

      setFrame(frame);
    } else {
      message = tr("Frame is empty");
    }
  } else {
    message = tr("The capture is not open");
  }
  _statusBar->showMessage(message);
}

void FaceViewer::onTimerChanged(int value) {
  _timer.setInterval(value);
}

void FaceViewer::closeEvent(QCloseEvent* event) {
  if (_timer.isActive()) {
    _timer.stop();
  }
  QWidget::closeEvent(event);
  if (_controller->isVisible()) {
    _controller->close();
  }
}

void FaceViewer::initialize() {
  gos::cv::detection::initialize(_context);

  _context.face.classifier.load(_context.face.file);
  _context.eyes.classifier.load(_context.eyes.file);

  _imageLabel = new QLabel;
  _imageLabel->setBackgroundRole(QPalette::Base);
  _imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  _imageLabel->setScaledContents(true);

  _scrollArea = new QScrollArea;
  _scrollArea->setBackgroundRole(QPalette::Dark);
  _scrollArea->setWidget(_imageLabel);
  _scrollArea->setVisible(false);

  _statusBar = new QStatusBar;
  _statusBar->showMessage(tr("Ready"));

  setCentralWidget(_scrollArea);
  setStatusBar(_statusBar);

  QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
  QAction* openAct = fileMenu->addAction(tr("&Open..."), this, &FaceViewer::open);
  openAct->setShortcut(QKeySequence::Open);
  QAction* closeAct = fileMenu->addAction(tr("&Close"), this, &FaceViewer::close);
  closeAct->setShortcut(QKeySequence::Close);

  QMenu* viewMenu = menuBar()->addMenu(tr("&View"));

  _fitToWindowAct = viewMenu->addAction(tr("&Fit to Window"), this, &FaceViewer::fitToWindow);
  _fitToWindowAct->setEnabled(false);
  _fitToWindowAct->setCheckable(true);
  _fitToWindowAct->setShortcut(tr("Ctrl+F"));

  QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));

  helpMenu->addAction(tr("&About"), this, &FaceViewer::about);
  helpMenu->addAction(tr("About &Qt"), &QApplication::aboutQt);

  resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);

  _controller = new FaceDetectionController(_context);
  connect(_controller, &FaceDetectionController::timerChanged, this, &FaceViewer::onTimerChanged);
  _controller->show();

  connect(&_timer, &QTimer::timeout, this, &FaceViewer::timer);
  _timer.start(_context.timer);

  open();
}

void FaceViewer::updateActions() {
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
}

void FaceViewer::adjustScrollBar(QScrollBar* scrollBar, const double& factor) {
  scrollBar->setValue(
    int(factor * scrollBar->value() + ((factor - 1) * scrollBar->pageStep() / 2)));
}
