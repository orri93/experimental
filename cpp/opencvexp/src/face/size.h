#ifndef GOS_EXP_QT_SIZE_H_
#define GOS_EXP_QT_SIZE_H_

#include <QSize>
#include <QtWidgets>

class SizeLayout : public QHBoxLayout {
  Q_OBJECT
public:
  SizeLayout(const QString& label, const QSize& size, QWidget* parent = nullptr);
  void extra(bool enable);

signals:
  void sizeChanged(const QSize& newValue);

private slots:
  void onWidthChanged(int value);
  void onHeightChanged(int value);

private:
  void initialize(const QString& label);

  QSize _size;

  QLabel* _label;
  QSpinBox* _widthSpinBox;
  QSpinBox* _heightSpinBox;
};

#endif
