#include <emscripten.h>

/*
 *
 * Compilation
 * Compiling a New C/C++ Module to WebAssembly
 * Calling a custom function defined in C
 * https://developer.mozilla.org/en-US/docs/WebAssembly/C_to_wasm
 * https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html#calling-compiled-c-functions-from-javascript-using-ccall-cwrap
 * emcc -o fibonacci.js fibonacci.c -O3 -s WASM=1 -s NO_EXIT_RUNTIME=1 -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall']"
 * 
 * No Entry
 * emcc -Os fibonacci.c -o noentry.wasm --no-entry
 * 
 */

EMSCRIPTEN_KEEPALIVE
int fibonaccil(int num) {
  int a = 1;
  int b = 1;

  while (num-- > 1) {
    int t = a;
    a = b;
    b += t;
  }
  return b;
}

EMSCRIPTEN_KEEPALIVE
int fibonaccir(int num) {
  if (num <= 1)
    return 1;

  return fibonaccir(num - 1) + fibonaccir(num - 2);
}

int memo[10000];

EMSCRIPTEN_KEEPALIVE
int fibonaccim(int num) {
  if (memo[num] != -1)
    return memo[num];

  if (num == 1 || num == 2) {
    return 1;
  } else {
    return memo[num] = fibonaccim(num - 1) + fibonaccim(num - 2);
  }
}
