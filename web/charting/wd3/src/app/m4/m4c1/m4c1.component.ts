import { Component, Input, ViewChild, ElementRef, NgZone,  OnDestroy } from '@angular/core';
import { Observable } from 'rxjs';
import * as _ from 'lodash';
import { loadScript } from './../../tools';
import { environment } from './../../../environments/environment';
import wasmCacheBusting from './../../../wasm-cache-busting.json';
import { DebugService } from './../../shared/services/debug.service';

type EmscriptenModuleDecorator<M extends EmscriptenModule> = (module: M) => void;

const noopModuleDecorator = (mod: EmscriptenModule) => mod;

const MODULE_EXPORT_NAME = 'DemoModule';
const WASM_JAVA_SCRIPT_LOADER = 'demo.js';

@Component({
  selector: 'app-m4c1',
  templateUrl: './m4c1.component.html',
  styleUrls: ['./m4c1.component.scss']
})
export class M4c1Component<M extends EmscriptenModule = EmscriptenModule> implements OnDestroy {
  @ViewChild('canvas') canvas: ElementRef;
  @ViewChild('demodiv') demo: ElementRef;
  
  size: ChartSize;

  componentName = 'Module no. 4 - Component no. 1';

  error: string;

  private resolvedModule: M;
  private moduleDecorator: EmscriptenModuleDecorator<M>;
  private observable: Observable<M>;
  private webAssemblyOriginalHeight: number;
 
  constructor(private ngZone: NgZone) {
  }

  private resolveModule(): void {
    const jsVersion = wasmCacheBusting[WASM_JAVA_SCRIPT_LOADER]
      ? `?v=${wasmCacheBusting[WASM_JAVA_SCRIPT_LOADER]}`
      : '';
    loadScript(
      MODULE_EXPORT_NAME,
      `${environment.wasmAssetsPath}/${WASM_JAVA_SCRIPT_LOADER}${jsVersion}`)
      .then(() => {
        const module = {
          locateFile: (file: string) => {
            const fileVersion = wasmCacheBusting[file] ?
              `?v=${wasmCacheBusting[file]}` : '';
            return `${environment.wasmAssetsPath}/${file}${fileVersion}`;
          },
        } as M;
        const moduleDecorator: EmscriptenModuleDecorator<M> =
          this.moduleDecorator || noopModuleDecorator;
        moduleDecorator(module);

        return window[MODULE_EXPORT_NAME](module);
      })
      .then((mod) => {
        this.resolvedModule = mod;
        this.moduleResolved();
      });
  }

  private getCanvasNativeElement(canvas: ElementRef): HTMLCanvasElement {
    let canvasElement: HTMLCanvasElement;
    if (canvas) {
      if (canvas.nativeElement) {
        canvasElement = canvas.nativeElement as HTMLCanvasElement;
        if (!canvasElement) {
          console.error('Canvas native element is not a HTML Canvas Element');
        }
      } else {
        console.error('Undefined canvas native element');
      }
    } else {
      console.error('Undefined canvas');
    }
    return canvasElement;
  }

  private moduleResolved(): void {
    console.log('WebAssembly module ' + MODULE_EXPORT_NAME + ' has been resolved');
  }

  handleSizing(newSize: ChartSize): ChartSize {
    if (this.resolvedModule) {
      DebugService.consoleLogSize(newSize, 'Handling sizing for resizing');
      this.size = _.clone(newSize);
      this.size.height = this.webAssemblyOriginalHeight;
      DebugService.consoleLogSize(this.size, 'Handling sizing for resizing using');
      this.resizeWebAssembly(this.size);
    } else {
      DebugService.consoleLogSize(newSize, 'Handling sizing for the first time');
      this.size = _.clone(newSize);
      this.webAssemblyOriginalHeight = this.size.height;
      const self = this;
      this.moduleDecorator = (mod) => {
        const args = [
          this.size.width.toString(),
          this.size.height.toString()
        ];
        mod.arguments = args;
        mod.preRun = [];
        mod.canvas = this.canvas.nativeElement as HTMLCanvasElement;
        mod.printErr = (what: string) => {
          if (!what.startsWith('WARNING')) {
            this.ngZone.run(() => (self.error = what));
          }
        };
      };
      this.resolveModule();
    }
    return this.size;
  }

  createWebAssembly(newSize: ChartSize): void {
    if (this.resolvedModule) {
      console.log('Create the WebAssembly to width: ' + newSize.width + ' and height: ' + newSize.height);
      this.resolvedModule.ccall('create', 'void', ['number', 'number'], [newSize.width, newSize.height]);
    }
  }

  resizeWebAssembly(newSize: ChartSize): void {
    if (this.resolvedModule) {
      // console.log('Resizing the WebAssembly to width: ' + newSize.width + ' and height: ' + newSize.height);
      this.resolvedModule.ccall('resize', 'void', ['number', 'number'], [newSize.width, newSize.height]);
    }
  }

  ngOnDestroy(): void {
    if (this.resolvedModule) {
      console.log('Calling shutdown in module');
      this.resolvedModule.ccall('shutdown', 'void', [], []);
    }
  }
}
