#ifndef WD3_HM_CPP_STATISTICS_H_
#define WD3_HM_CPP_STATISTICS_H_

#include <modules/hmpp/data.h>
#include <modules/hmpp/column.h>

namespace wd3 {
namespace statistics {
void sum(::wd3::data& data, ::wd3::column& sum, ::wd3::column& squaresum);
void mean(::wd3::column& mean, ::wd3::column& sum, const int& size, const int& count);
void diffsquare(::wd3::data& data, ::wd3::column& mean, ::wd3::column& diffsquare);
void sd(::wd3::column& sd, ::wd3::column& diffsquare, const int& size, const int& count);
} // namespace statistics
} // namespace wd3

#endif
