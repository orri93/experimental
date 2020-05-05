#ifndef ITEMS_H
#define ITEMS_H

#include <QtCore/QObject>

class Items : public QObject {
  Q_OBJECT
public:
  Items(QObject* parent = nullptr);

  virtual ~Items();

  /* Timer items */
  virtual const int& interval() const;

  /* Experiment items */
  virtual const bool& boolean() const;
  virtual const double& real() const;
  virtual const int& integer() const;
  virtual const QString& text() const;

protected:

  /* Timer items */
  virtual bool applyInterval(const int& value);

  /* Experiment items */
  virtual bool applyBoolean(const bool& value);
  virtual bool applyReal(const double& value);
  virtual bool applyInteger(const int& value);
  virtual bool applyText(const QString& value);

  /* Timer items */
  int interval_;

  /* Experiment items */
  bool boolean_;
  double real_;
  int integer_;
  QString text_;
};

#endif
