#ifndef WD3_HM_CPP_TIME_H_
#define WD3_HM_CPP_TIME_H_

#include <chrono>
#include <string>

#include <gos/range.h>

#include <modules/hmpp/types.h>

#define WD3_TIME_DEFAULT_PARSE_FORMAT "%Y-%m-%d %T"
#define WD3_TIME_DEFAULT_FORMAT "%Y-%m-%d %T"

namespace wd3 {
namespace time {

template<typename T = double>
class scale {
public:
  scale() {}
  scale(const scale& scale) : _domain(scale._domain), _range(scale._range) {}
  scale(const ::gos::range::d1 <::wd3::type::time>& domain, const ::gos::range::d1<T>& range) :
    _domain(domain),
    _range(range) {
  }

  scale& operator=(const scale& scale) {
    if (this != &scale) {
      _domain = scale._domain;
      _range = scale._range;
    }
    return *this;
  }

  void setdomain(const ::gos::range::d1<::wd3::type::time>& domain) { _domain = domain; }
  void setrange(const ::gos::range::d1<T>& range) { _range = range; }

  void setdomain(const ::wd3::type::time& from, const ::wd3::type::time& to) { _domain.setfrom(from); _domain.setto(to); }
  void setrange(const T& from, const T& to) { _range.setfrom(from); _range.setto(to); }

  const ::gos::range::d1<::wd3::type::time>& domain() const { return _domain; }
  const ::gos::range::d1<T>& range() const { return _range; }

  T value(const ::wd3::type::time& value) {
    ::wd3::type::duration dd = _domain.to() - _domain.from();
    ::wd3::type::duration td = value - _domain.from();
    T rd = _range.difference();
    ::std::chrono::microseconds ddm = ::std::chrono::duration_cast<::std::chrono::microseconds>(dd);
    ::std::chrono::microseconds tdm = ::std::chrono::duration_cast<::std::chrono::microseconds>(td);
    double r = static_cast<double>(tdm.count()) / ddm.count();
    return _range.from() + static_cast<T>(r * rd);
  }

  ::wd3::type::time reverse(const T& value) {
    ::wd3::type::duration dd = _domain.to() - _domain.from();
    T rd = _range.difference();
    ::std::chrono::microseconds ddm = ::std::chrono::duration_cast<::std::chrono::microseconds>(dd);
    double r = static_cast<double>(value - _range.from()) / rd;
    ::wd3::type::duration td = ::std::chrono::microseconds(static_cast<long long>(r * ddm.count()));
    return _domain.from() + td;
  }

private:
  ::gos::range::d1<::wd3::type::time> _domain;
  ::gos::range::d1<T> _range;
};

template<typename T = double>
scale<T> make_scale(const ::gos::range::d1<::wd3::type::time>& domain, const ::gos::range::d1<T>& range) {
  return scale<T>(domain, range);
}

template<typename T = double>
scale<T> make_scale(const ::wd3::type::time& domain_from, const ::wd3::type::time& domain_to, const T& range_from, const T& range_to) {
  return scale<T>(::gos::range::make_d1(domain_from, domain_to), ::gos::range::make_d1(range_from, range_to));
}

::wd3::type::time parse(const ::std::string& time, const ::std::string& format = WD3_TIME_DEFAULT_PARSE_FORMAT);
::std::string format(const ::wd3::type::time& time, const ::std::string& format = WD3_TIME_DEFAULT_FORMAT);

} // namespace time
} // namespace wd3

#endif
