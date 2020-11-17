import { AfterViewInit, Directive, ElementRef } from "@angular/core";
import { loadScript } from "./tools";
import { environment } from "./../environments/environment";
import wasmCacheBusting from "../wasm-cache-busting.json";
import { stringify } from 'querystring';

type EmscriptenModuleDecorator<M extends EmscriptenModule> = (module: M) => void;

const noopModuleDecorator = (mod: EmscriptenModule) => mod;

@Directive()
export abstract class EmscriptenWasmComponent<M extends EmscriptenModule = EmscriptenModule> implements AfterViewInit {
  private resolvedModule: M;
  protected moduleDecorator: EmscriptenModuleDecorator<M>;

  protected constructor(
    private moduleExportName: string,
    private wasmJavaScriptLoader: string
  ) { }

  get module(): M {
    return this.resolvedModule;
  }

  ngAfterViewInit(): void {
    this.resolveModule();
  }

  protected resolveModule(): void {
    const jsVersion = wasmCacheBusting[this.wasmJavaScriptLoader]
      ? `?v=${wasmCacheBusting[this.wasmJavaScriptLoader]}`
      : "";
    loadScript(
      this.moduleExportName,
      `${environment.wasmAssetsPath}/${this.wasmJavaScriptLoader}${jsVersion}`)
      .then(() => {
        const module = <M>{
          locateFile: (file: string) => {
            const fileVersion = wasmCacheBusting[file] ?
              `?v=${wasmCacheBusting[file]}` : "";
            return `${environment.wasmAssetsPath}/${file}${fileVersion}`;
          },
        };
        const moduleDecorator: EmscriptenModuleDecorator<M> =
          this.moduleDecorator || noopModuleDecorator;
        moduleDecorator(module);

        return window[this.moduleExportName](module);
      })
      .then((mod) => {
        this.resolvedModule = mod;
      });
  }

  protected getCanvasNativeElement(canvas: ElementRef): HTMLCanvasElement {
    let canvasElement: HTMLCanvasElement;
    if(canvas) {
      if(canvas.nativeElement) {
        canvasElement = <HTMLCanvasElement>canvas.nativeElement;
        if(!canvasElement) {
          console.error("Canvas native element is not a HTML Canvas Element");
        }
      } else {
        console.error("Undefined canvas native element");
      }
    } else {
      console.error("Undefined canvas");
    }
    return canvasElement;
  }
}
