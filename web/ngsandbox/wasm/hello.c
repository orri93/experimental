#include <stdlib.h>
#include <stdio.h>

/*
 * Hello World Web Assembly
 * 
 * Compile
 *   emcc hello.c
 * 
 * Test with node
 *   node a.out.js
 * 
 * Generate HTML
 *   emcc hello.c -o hello.html
 * 
 * Pythone HTTP Server
 *   python2 -m SimpleHTTPServer 8080
 *   python3 -m http.server
 * 
 */

int main() {
  printf("hello, world!\n");
  return EXIT_SUCCESS;
}
