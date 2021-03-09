#include <iostream>
#include <sstream>

#include <date/date.h>

#include <modules/hmpp/time.h>

namespace wd3 {
namespace time {

wd3::type::time parse(const std::string& text, const std::string& format) {
  wd3::type::time time;
  std::istringstream in{ text };
  in >> ::date::parse(format, time);
  return time;
}

std::string format(const wd3::type::time& time, const std::string& format) {
  return ::date::format(format, time);
}

} // namespace time
} // namespace wd3
