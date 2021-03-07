#ifndef GOS_EXP_QT_SELECT_H_
#define GOS_EXP_QT_SELECT_H_

#include <QtWidgets>

class SelectFileLayout : public QHBoxLayout {
  Q_OBJECT
public:
  SelectFileLayout(const QString& label, const QString& value, QWidget* parent = nullptr);

signals:
  void valueChanged(const QString& newValue);

private slots:
  void onBrowseClicked();
  void onSetClicked();

private:
  void initialize(const QString& label);

  QString _value;

  QLabel* _label;
  QLineEdit* _edit;

  QPushButton* _browse;
  QPushButton* _set;
};

#endif
