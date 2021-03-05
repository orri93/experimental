#ifndef WD3_HM_CPP_EXCEPTION_H_
#define WD3_HM_CPP_EXCEPTION_H_

#include <string>
#include <exception>

namespace wd3 {

class exception : public ::std::exception {
public:
  exception(const char* what);
#ifdef _MSC_VER
#if _MSC_VER >= 1910
  const char* what() const noexcept override;
#else
  const char* what() const;
#endif
#else
  const char* what() const noexcept override;
#endif

private:
  std::string what_;
};

} // namespace wd3

#endif
