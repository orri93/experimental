#include <emscripten.h>

/*
 * 5 Easy steps to run your C code from Angular app
 * By Sebastian Superczyński
 * 
 * See https://espeo.eu/blog/5-easy-steps-to-run-your-c-code-from-angular-app/
 * 
 * Web Assembly Compilation
 * emcc src/wasm/fibonacci.c -Os -s WASM=1 -s MODULARIZE=1 -o src/wasm/fibonacci.js
 * 
 */

int EMSCRIPTEN_KEEPALIVE fibonacci(int n) {
  if(n == 0 || n == 1) {
    return n;
  } else {
    return fibonacci(n - 1) + fibonacci(n - 2);
  }
}
