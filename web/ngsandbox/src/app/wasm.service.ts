import { Injectable } from '@angular/core';
import { HttpClient } from "@angular/common/http";
import { mergeMap } from "rxjs/operators";
import { environment } from "../environments/environment";

export type FibonacciFunction = (num: number) => number;

@Injectable({ providedIn: 'root' })
export class WasmService {

  loaded: boolean;

  fibonacciFunction: FibonacciFunction;

  constructor(private http: HttpClient) {
    // Load "pure" WebAssembly, i.e. without any Emscripten API needed to work with it
    let wasmurl: string = `${environment.wasmAssetsPath}/fibonacci.wasm`;
    this.instantiateWasm(wasmurl, {}).then((result) => {
      const wasmInstance = result.instance;
      this.fibonacciFunction = wasmInstance.exports.fibonacci as FibonacciFunction;
      this.loaded = true;
    });
  }

  private instantiateWasm(
    url: string,
    imports?: WebAssembly.Imports
  ): Promise<WebAssembly.WebAssemblyInstantiatedSource> {
    // emcc -Os src/app/wasm/fibonacci/fibonacci.c -o src/assets/wasm/fibonacci.wasm --no-entry
    return this.http
      .get(url, { responseType: "arraybuffer" })
      .pipe(mergeMap((bytes) => WebAssembly.instantiate(bytes, imports)))
      .toPromise();
  }

  public fibonacci(num: number): number {
    return this.fibonacciFunction(num);
  }
}
