import { Injectable } from '@angular/core';

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

@Injectable({providedIn: 'root'})
export class SebastianService {

  constructor() { }

  run() {
    const number = 30;
    this.fibonacciJS(number);
    this.fibonacciWASM(number);
  }

  fibonacciJS(number: number) {
    function fib(n: number) {
      if (n === 0 || n === 1) {
        return n;
      } else {
        return (fib(n - 1) + fib(n - 2));
      }
    }

    console.group('JS');
    console.time('time');
    const jsFib = fib(number);
    console.log('result: ', jsFib);
    console.timeEnd('time');
    console.groupEnd();
  }

  fibonacciWASM(number: number) {
    import('./../wasm/fibonacci.wasm').then((module) => {
      console.group('WASM');
      console.time('time');
      const wasmFib = module._fibonacci(number);
      console.log('result: ', wasmFib);
      console.timeEnd('time');
      console.groupEnd();
    });
  }

}
