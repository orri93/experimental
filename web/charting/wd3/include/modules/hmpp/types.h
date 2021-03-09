#ifndef WD3_HM_CPP_TYPES_H_
#define WD3_HM_CPP_TYPES_H_

#include <chrono>

// #include <date/date.h>

namespace wd3 {
namespace type {

typedef std::chrono::system_clock clock;
typedef clock::duration duration;
typedef clock::time_point time;

} // namespace type
} // namespace wd3

#endif
