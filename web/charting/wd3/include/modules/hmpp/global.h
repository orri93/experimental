#ifndef WD3_HM_CPP_GLOBAL_H_
#define WD3_HM_CPP_GLOBAL_H_

#include <memory>

#include <modules/hmpp/data.h>
#include <modules/hmpp/context.h>
#include <modules/hmpp/gradient.h>

namespace wd3 {
namespace global {

typedef ::std::unique_ptr<::wd3::data> DataPtr;

extern DataPtr data;
extern wd3::context context;
extern wd3::gradient gradient;

} // namespace global
} // namespace wd3

#endif
