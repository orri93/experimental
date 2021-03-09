#include <cstdlib>

#include <iostream>
#include <iomanip>
#include <string>
#include <memory>

#include <modules/hmpp/global.h>
#include <modules/hmpp/emscripten.h>
#include <modules/macros.h>

#include <modules/hmpp/column.h>

namespace wd3 {
namespace emscripten {
typedef std::unique_ptr<wd3::column> ColumnPtr;
ColumnPtr column;
namespace time {
namespace parse {
std::string format = "%Y-%m-%d %H:%M:%S";
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

#if defined(WD3_DEVELOPE_EMSCRIPTEN)
#ifndef EMSCRIPTEN_KEEPALIVE
#define EMSCRIPTEN_KEEPALIVE
#endif
#endif

EMSCRIPTEN_KEEPALIVE int change(int count, bool copy) {
  return 0;
}

EMSCRIPTEN_KEEPALIVE bool startData(int size) {
  wd3::global::data = std::make_unique<wd3::data>(size);
  if (wd3::global::data) {
    return true;
  } else {
    std::cerr << "Failed to create WD3 data" << std::endl;
    return false;
  }
}

EMSCRIPTEN_KEEPALIVE bool startColumn() {
  if (wd3::global::data) {
    wd3::emscripten::column =
      std::make_unique<wd3::column>(wd3::global::data->size());
    if (wd3::emscripten::column) {
      return true;
    } else {
      std::cerr << "Failed to create WD3 column" << std::endl;
      return false;
    }
  } else {
    std::cerr << "WD3 data is undefined" << std::endl;
    return false;
  }
}

EMSCRIPTEN_KEEPALIVE bool setColumn(int index, double depth, double value) {
  if (wd3::emscripten::column) {
    wd3::emscripten::column->set(index, depth, value);
    return true;
  } else {
    std::cerr << "WD3 column is undefined" << std::endl;
    return false;
  }
}

EMSCRIPTEN_KEEPALIVE bool addColumn() {

}

#endif
