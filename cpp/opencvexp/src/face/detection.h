#ifndef GOS_EXP_QT_DETECTION_H_
#define GOS_EXP_QT_DETECTION_H_

#include <QtWidgets>

#include <cv/detection.h>

#include <select.h>
#include <size.h>

class DetectionGroup : public QGroupBox {
  Q_OBJECT
public:
  DetectionGroup(
    const QString& title,
    ::gos::cv::detection::classification& classification,
    QWidget* parent = nullptr);
  void extra(bool enable);

signals:

private slots:
  void onFactorChanged(double value);
  void onMinimumChanged(int value);

private:
  void initialize();

  SelectFileLayout* _selectCascadeFile;

  SizeLayout* _sizeLayout;

  QLabel* _factorLabel;
  QDoubleSpinBox* _factorSpinBox;
  QHBoxLayout* _factorLayout;

  QLabel* _minimumLabel;
  QSpinBox* _minimumSpinBox;
  QHBoxLayout* _minimumLayout;

  QVBoxLayout* _detectionLayout;

  ::gos::cv::detection::classification& _classification;
};

#endif
