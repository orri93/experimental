#ifndef GOS_EXP_QT_CONTROLLER_H_
#define GOS_EXP_QT_CONTROLLER_H_

#include <QDialog>
#include <QtWidgets>

#include <cv/detection.h>

#include <detection.h>

class FaceDetectionController : public QDialog {
  Q_OBJECT
public:
  FaceDetectionController(::gos::cv::detection::context& context, QWidget* parent = nullptr);

signals:
  void timerChanged(const int& newTimer);

private slots:
  void onTimerChanged(int value);
  void onMethodChanged(int index);
  void onScaleChanged(double value);

private:
  void initialize();
  void update();

  QLabel* _timerLabel;
  QSpinBox* _timerSpinBox;
  QHBoxLayout* _timerLayout;

  QLabel* _methodLabel;
  QComboBox* _methodBox;
  QHBoxLayout* _methodLayout;

  QLabel* _scaleLabel;
  QDoubleSpinBox* _scaleSpinBox;
  QHBoxLayout* _scaleLayout;

  DetectionGroup* _detectionFaceGroup;
  DetectionGroup* _detectionEyesGroup;

  QVBoxLayout* _optionLayout;

  ::gos::cv::detection::context& _context;
};

#endif
