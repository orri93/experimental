#include <cstdlib>

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

namespace wd3 {
namespace emscripten {
typedef std::unique_ptr<wd3::column> ColumnPtr;
ColumnPtr column;
namespace time {
namespace parse {
std::string format = WD3_TIME_DEFAULT_PARSE_FORMAT;
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

#if defined(__EMSCRIPTEN__) || defined(WD3_DEVELOPE_EMSCRIPTEN)

bool startData(int size) {
  wd3::global::data = std::make_unique<wd3::data>(size);
  if (wd3::global::data) {
    return true;
  } else {
    std::cerr << "Failed to create WD3 data" << std::endl;
  }
  return false;
}

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

#endif
