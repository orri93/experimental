#include <emscripten.h>

/*
 * Using Web Assembly to speed up your Angular Application
 * 
 * See https://malcoded.com/posts/web-assembly-angular/
 * 
 * Compile
 *
 * Windows 10
 * > emcc src\wasm\fibonacci.c -Os -s WASM=1 -s MODULARIZE=1 -o src\wasm\fibonacci.js
 * 
 * Linux
 * $ emcc src/wasm/fibonacci.c -Os -s WASM=1 -s MODULARIZE=1 -o src/wasm/fibonacci.js
 * 
 */

int EMSCRIPTEN_KEEPALIVE fibonacci(int n) {
  if (n == 0 || n == 1)
    return n;
  else
    return (fibonacci(n - 1) + fibonacci(n - 2));
}
