#include <restriction.h>

#include <QQmlEngine>
#include <QDebug>

#define GOS_QML_RESTRICTION_COUNT 4

namespace qt {
namespace sandbox {
namespace model {

static const QString RestrictionTextArray[GOS_QML_RESTRICTION_COUNT + 1] = {
  "None",
  "Minimum",
  "Maximum",
  "Both",
  "" };

static Restriction::Enum RestrictionEnumArray[GOS_QML_RESTRICTION_COUNT + 1] = {
  Restriction::Enum::None,
  Restriction::Enum::Minimum,
  Restriction::Enum::Maximum,
  Restriction::Enum::Both,
  Restriction::Enum::Undefined
};

Restriction::EnumerateModel Restriction::EnumerateModel_(
  RestrictionEnumArray,
  RestrictionTextArray,
  Restriction::Enum::Undefined,
  Restriction::RestrictionRoles::ValueRole,
  Restriction::RestrictionRoles::TextRole);

Restriction::Restriction(QObject* parent) :
  QAbstractListModel(parent) {
}

int Restriction::index(const Restriction::Enum& restriction) {
  int index = EnumerateModel_.index(restriction);
  qDebug() << "Returning restriction index for '"
    << EnumerateModel_.toString(restriction) << "' as " << index;
  return index;
  //return EnumerateModel_.index(restriction);
}

Restriction::Enum Restriction::restriction(const int& index) {
  Restriction::Enum restriction = EnumerateModel_.enumerate(index);
  qDebug() << "Returning restriction '"
    << EnumerateModel_.toString(restriction) << "' for index " << index;
  return restriction;
  //return EnumerateModel_.enumerate(index);
}

int Restriction::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return EnumerateModel_.count();
}

QVariant Restriction::data(const QModelIndex& index, int role) const {
  return EnumerateModel_.data(index, role);
}

//![0]
QHash<int, QByteArray> Restriction::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[TextRole] = "text";
  roles[ValueRole] = "value";
  return roles;
}

Restriction::EnumerateModel& Restriction::enumerateModel() {
  return EnumerateModel_;
}

}
}
}
