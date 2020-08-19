#ifndef QT_SANDBOX_MODEL_H_
#define QT_SANDBOX_MODEL_H_

#include <QAbstractListModel>
#include <QString>
#include <QMap>

namespace qt {
namespace sandbox {
namespace model {

template<typename E, typename R> class Enumerate {
public:
  Enumerate(
    const E* enumerate,
    const QString* text,
    const E& undefined,
    const R& valuerole,
    const R& textrole) :
    count_(0),
    undefined_(undefined),
    valuerole_(valuerole),
    textrole_(textrole) {
    if (text != nullptr) {
      while (text->length() > 0) {
        enumtextmap_.insert(*enumerate, *text);
        textenummap_.insert(*text, *enumerate);
        enumindexmap_.insert(*enumerate, count_);
        indexenummap_.insert(count_, *enumerate);
        enumerate++;
        count_++;
        text++;
      }
    }
  }

  const int count() const {
    return count_;
  }

  QString toString(const E& enumerate) {
    return enumtextmap_.value(enumerate, "Unknown");
  }
  E parse(const QString& text) {
    return textenummap_.value(text, undefined_);
  }
  int index(const E& restriction) {
    return enumindexmap_.value(restriction, -1);
  }
  E enumerate(const int& index) {
    return indexenummap_.value(index, undefined_);
  }

  QVariant data(const QModelIndex& index, int role) {
    E e = enumerate(index.row());
    if (e != undefined_) {
      if (role == valuerole_) {
        return QVariant::fromValue(e);
      } else if (role == textrole_) {
        return toString(e);
      }
    }
    return QVariant();
  }

private:
  typedef QMap<QString, E> TextEnumerateMap;
  typedef QMap<E, QString> EnumerateTextMap;
  typedef QMap<int, E> IndexEnumerateMap;
  typedef QMap<E, int> EnumerateIndexMap;

  int count_;
  EnumerateTextMap enumtextmap_;
  TextEnumerateMap textenummap_;
  EnumerateIndexMap enumindexmap_;
  IndexEnumerateMap indexenummap_;

  E undefined_;
  R valuerole_;
  R textrole_;
};

}
}
}

#endif
