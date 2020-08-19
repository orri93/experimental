#ifndef QT_SANDBOX_RESTRICTION_H_
#define QT_SANDBOX_RESTRICTION_H_

#include <QObject>
#include <QtGlobal>
#include <QAbstractListModel>

#include <model.h>

#define GOS_QML_MODEL_RESTRICTION "restrictionModel"

namespace qt {
namespace sandbox {

namespace model {

class Restriction : public QAbstractListModel {
  Q_OBJECT
public:
  enum class Enum : quint8 {
    Undefined,
    None,
    Minimum,
    Maximum,
    Both
  };
  enum RestrictionRoles {
    TextRole = Qt::UserRole + 1,
    ValueRole
  };
  typedef ::qt::sandbox::model::Enumerate<Enum, RestrictionRoles> EnumerateModel;
  Q_ENUM(Enum)
  Q_ENUM(RestrictionRoles)

  Restriction(QObject* parent = nullptr);

  Q_INVOKABLE int index(const Enum& restriction);
  Q_INVOKABLE Enum restriction(const int& index);
  
  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
  
  static EnumerateModel& enumerateModel();

protected:
  QHash<int, QByteArray> roleNames() const;
private:

  static EnumerateModel EnumerateModel_;
};

}
}
}

#endif
