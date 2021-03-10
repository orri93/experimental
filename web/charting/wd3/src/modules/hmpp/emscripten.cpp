#include <cstdlib>
#include <cstring>

#include <iostream>
#include <iomanip>
#include <string>
#include <memory>

#include <modules/hmpp/types.h>
#include <modules/hmpp/global.h>
#include <modules/hmpp/emscripten.h>
#include <modules/hmpp/column.h>
#include <modules/hmpp/time.h>
#include <modules/macros.h>

#define WD3_EMSCRIPTEN_RESULT_STRING_LENGTH 1024

namespace wd3 {
namespace emscripten {
typedef std::unique_ptr<wd3::column> ColumnPtr;
static ColumnPtr column;
namespace result {
namespace string {
static char wd3EmscriptenResultString[WD3_EMSCRIPTEN_RESULT_STRING_LENGTH];
void set(const std::string& text);
const char* get();
} // namespace parse
} // namespace time
} // namespace emscripten
} // namespace wd3

// void testEmscripten() { std::cout << "Test Emscripten keepalive function" << std::endl; }

/*
 *  Gradient interface
 */

void resetGradient() {
  wd3::global::gradient.reset();
}

void addGradientColorStop(int color, int size) {
  wd3::global::gradient.add(static_cast<uint32_t>(color), size);
}

bool addGradientColorStopText(const char* text, int size) {
  if (wd3::global::gradient.add(text, size)) {
    return true;
  } else {
    std::cerr << "Failed to add '" << text <<
      "' as gradient color stop text" << std::endl;
    return false;
  }
}

void stockGradient(int size) {
  wd3::global::gradient.stock(size);
}

void createGradient() {
  wd3::global::gradient.create();
}

/*
 *  Data interface
 */

bool startData(int size) {
  wd3::global::data = std::make_unique<wd3::data>(size);
  if (wd3::global::data) {
    return true;
  } else {
    std::cerr << "Failed to create WD3 data" << std::endl;
  }
  return false;
}

bool removeDate(const char* timetext) {
  if (wd3::global::data) {
    wd3::type::time time = wd3::time::parse(timetext);
    wd3::global::data->remove(time);
    return true;
  } else {
    std::cerr << "WD3 data is undefined" << std::endl;
  }
  return false;
}

/*
 *  Column interface
 */

bool startColumn(const char* timetext) {
  if (wd3::global::data) {
    wd3::type::time time = wd3::time::parse(timetext);
    wd3::emscripten::column =
      std::make_unique<wd3::column>(wd3::global::data->size(), time);
    if (wd3::emscripten::column) {
      return true;
    } else {
      std::cerr << "Failed to create WD3 column" << std::endl;
    }
  } else {
    std::cerr << "WD3 data is undefined" << std::endl;
  }
  return false;
}

bool startColumnNow() {
  if (wd3::global::data) {
    wd3::type::time time = wd3::type::clock::now();
    wd3::emscripten::column =
      std::make_unique<wd3::column>(wd3::global::data->size(), time);
    if (wd3::emscripten::column) {
      return true;
    } else {
      std::cerr << "Failed to create WD3 column" << std::endl;
    }
  } else {
    std::cerr << "WD3 data is undefined" << std::endl;
  }
  return false;
}

bool setColumn(int index, double depth, double value) {
  if (wd3::emscripten::column) {
    wd3::emscripten::column->set(index, depth, value);
    return true;
  } else {
    std::cerr << "WD3 column is undefined" << std::endl;
  }
  return false;
}

bool addColumn() {
  if (wd3::global::data) {
    if (wd3::emscripten::column) {
      wd3::global::data->add(*wd3::emscripten::column);
      return true;
    } else {
      std::cerr << "WD3 column is undefined" << std::endl;
    }
  } else {
    std::cerr << "WD3 data is undefined" << std::endl;
  }
  return false;
}

/*
 *  Scale interface
 */

bool updateScaleX() {
  if (wd3::global::data) {
    const gos::range::d1<wd3::type::time> domain =
      wd3::global::data->time();
    wd3::global::context.updatexscale(domain);
    return true;
  } else {
    std::cerr << "WD3 data is undefined" << std::endl;
  }
  return false;
}

bool updateScaleY() {
  if (wd3::global::data) {
    ::gos::range::d1<> domain;
    wd3::global::data->range(domain);
    wd3::global::context.updateyscale(domain);
    return true;
  } else {
    std::cerr << "WD3 data is undefined" << std::endl;
  }
  return false;
}

bool updateScaleYZ() {
  if (wd3::global::data) {
    gos::range::d1<> ydomain, zdomain;
    wd3::global::data->ranges(ydomain, zdomain);
    wd3::global::context.updateyscale(ydomain);
    int gc = static_cast<int>(wd3::global::gradient.get().size());
    wd3::global::context.updatezscale(zdomain, gc);
    return true;
  } else {
    std::cerr << "WD3 data is undefined" << std::endl;
  }
  return false;
}

void setScaleZ(double from, double to) {
  gos::range::d1 domain = gos::range::make_d1(from, to);
  int gc = static_cast<int>(wd3::global::gradient.get().size());
  wd3::global::context.updatezscale(domain, gc);
}

const char* getScaleXDomainFrom() {
  const ::wd3::time::scale<int>& scale = wd3::global::context.getxscale();
  std::string text = ::wd3::time::format(scale.domain().from());
  wd3::emscripten::result::string::set(text);
  return wd3::emscripten::result::string::get();
}

const char* getScaleXDomainTo() {
  const ::wd3::time::scale<int>& scale = wd3::global::context.getxscale();
  std::string text = ::wd3::time::format(scale.domain().to());
  wd3::emscripten::result::string::set(text);
  return wd3::emscripten::result::string::get();
}

double getScaleYDomainFrom() {
  const ::gos::scale<double, int>& scale = wd3::global::context.getyscale();
  return scale.domain().from();
}

double getScaleYDomainTo() {
  const ::gos::scale<double, int>& scale = wd3::global::context.getyscale();
  return scale.domain().to();
}

double getScaleZDomainFrom() {
  const ::gos::scale<double, int>& scale = wd3::global::context.getzscale();
  return scale.domain().from();
}

double getScaleZDomainTo() {
  const ::gos::scale<double, int>& scale = wd3::global::context.getzscale();
  return scale.domain().to();
}

/*
 *  Render interface
 */

bool render() {
  bool result = false;
  if (wd3::global::data) {
    if (wd3::global::context.begin()) {
      result = wd3::global::context.render(
        wd3::global::gradient,
        *wd3::global::data);
      if (!result) {
        std::cerr << "WD3 render failed" << std::endl;
      }
      if (wd3::global::context.complete()) {
        return result;
      } else {
        std::cerr << "WD3 complete render failed" << std::endl;
      }
    } else {
      std::cerr << "WD3 begin render failed" << std::endl;
    }
  } else {
    std::cerr << "WD3 data is undefined" << std::endl;
  }
  return result;
}

bool clear(int value) {
  if (wd3::global::context.begin()) {
    wd3::global::context.clear(value);
    if (wd3::global::context.complete()) {
      return true;
    } else {
      std::cerr << "WD3 complete render failed" << std::endl;
    }
  } else {
    std::cerr << "WD3 begin render failed" << std::endl;
  }
  return false;
}

bool resize(int width, int height, int value) {
  wd3::global::context.set(width, height);
  if (wd3::global::context.create()) {
    if (wd3::global::data) {
      return render();
    } else {
      return clear(value);
    }
  } else {
    std::cerr << "WD3 create failed" << std::endl;
  }
  return false;
}

namespace wd3 {
namespace emscripten {
namespace result {
namespace string {
void set(const std::string& text) {
#ifdef __EMSCRIPTEN__
  if (text.length() + 1 < WD3_EMSCRIPTEN_RESULT_STRING_LENGTH) {
    ::strncpy(
      wd3EmscriptenResultString,
      text.c_str(),
      text.length() + 1);
} else {
    wd3EmscriptenResultString[0] = '\0';
  }
#else
  ::strncpy_s(
    wd3EmscriptenResultString,
    WD3_EMSCRIPTEN_RESULT_STRING_LENGTH,
    text.c_str(),
    text.length() + 1);
#endif
}
const char* get() {
  return ::wd3::emscripten::result::string::wd3EmscriptenResultString;
}
} // namespace parse
} // namespace time
int main(int argc, char** argv) {
  wd3::global::context.set(WD3_DEFAULT_WIDTH, WD3_DEFAULT_HEIGHT);
  wd3::global::context.parse(argc, argv);
  wd3::global::gradient.stock();
  wd3::global::gradient.create();
  if (wd3::global::context.initialize()) {
    return wd3::global::context.create() ? EXIT_SUCCESS : EXIT_FAILURE;
  }
  return EXIT_FAILURE;
}
} // namespace emscripten
} // namespace wd3
