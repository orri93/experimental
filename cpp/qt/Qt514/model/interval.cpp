#include <map>

#include <QStringList>

#include <model/interval.h>

namespace model {

typedef std::map<int, int> IntervalMap;
typedef IntervalMap::value_type IntervalValue;
typedef IntervalMap::iterator IntervalIterator;

static const int Intervals[] = { 1000, 750, 500, 250, 200, 150, 100, 50, 0 };

static IntervalMap _indexmap;
static IntervalMap _valuemap;

static QStringList _list;

int index(const int& value) {
  IntervalIterator it = _valuemap.find(value);
  if (it != _valuemap.end()) {
    return it->second;
  }
  return -1;
}

int value(const int& index) {
  IntervalIterator it = _indexmap.find(index);
  if (it != _indexmap.end()) {
    return it->second;
  }
  return -1;
}

QVariant create() {
  int i = 0;
  const int* interval = Intervals;
  while (*interval != 0) {
    _list.append(QString("%1 ms").arg(*interval));
    _indexmap.insert(IntervalValue(i, *interval));
    _valuemap.insert(IntervalValue(*interval, i));
    interval++;
    i++;
  }
  return QVariant::fromValue(_list);
}

}
