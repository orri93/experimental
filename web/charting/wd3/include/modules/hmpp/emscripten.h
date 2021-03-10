#ifndef WD3_HM_CPP_EMSCRIPTEN_H_
#define WD3_HM_CPP_EMSCRIPTEN_H_

#ifdef __EMSCRIPTEN__
#include <emscripten.h>

namespace wd3 {
namespace emscripten {
int main(int argc, char** argv);
} // namespace emscripten
} // namespace wd3

#ifdef __cplusplus
extern "C" {
#endif

// EMSCRIPTEN_KEEPALIVE void testEmscripten();

/*
 *  Gradient interface
 */
EMSCRIPTEN_KEEPALIVE void resetGradient();
EMSCRIPTEN_KEEPALIVE void addGradientColorStop(int color, int size);
EMSCRIPTEN_KEEPALIVE bool addGradientColorStopText(const char* text, int size);
EMSCRIPTEN_KEEPALIVE void stockGradient(int size);
EMSCRIPTEN_KEEPALIVE void createGradient();

/*
 *  Data interface
 */
EMSCRIPTEN_KEEPALIVE bool startData(int size);
EMSCRIPTEN_KEEPALIVE bool removeDate(const char* timetext);

/*
 *  Column interface
 */
EMSCRIPTEN_KEEPALIVE bool startColumn(const char* timetext);
EMSCRIPTEN_KEEPALIVE bool startColumnNow();
EMSCRIPTEN_KEEPALIVE bool setColumn(int index, double depth, double value);
EMSCRIPTEN_KEEPALIVE bool addColumn();

/*
 *  Scale interface
 */
EMSCRIPTEN_KEEPALIVE bool updateScaleX();
EMSCRIPTEN_KEEPALIVE bool updateScaleY();
EMSCRIPTEN_KEEPALIVE bool updateScaleYZ();
EMSCRIPTEN_KEEPALIVE void setScaleZ(double from, double to);

/*
 *  Render interface
 */
EMSCRIPTEN_KEEPALIVE bool render();
EMSCRIPTEN_KEEPALIVE bool clear(int value);
EMSCRIPTEN_KEEPALIVE bool resize(int width, int height, int value);

#ifdef __cplusplus
}
#endif

#endif

#endif
