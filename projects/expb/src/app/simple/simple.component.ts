import { Component, ViewChild, ElementRef, NgZone, AfterViewInit, OnDestroy } from '@angular/core';
import { EmscriptenWasmComponent } from '../emscripten-wasm.component';

const requestFullscreen =
  document.documentElement.requestFullscreen ||
  document.documentElement["webkitRequestFullscreen"] ||
  document.documentElement["msRequestFullscreen"] ||
  document.documentElement["mozRequestFullScreen"];

@Component({
  selector: 'app-simple',
  templateUrl: './simple.component.html',
  styleUrls: ['./simple.component.css']
})
export class SimpleComponent extends EmscriptenWasmComponent implements OnDestroy, AfterViewInit {
  @ViewChild("canvas") canvas: ElementRef;

  title = 'Simple';

  error: string;
  supportsFullscreen: boolean;

  canvasNativeElement: HTMLCanvasElement;

  constructor(private ngZone: NgZone) {
    super("SimpleModule", "simple.js");
    this.supportsFullscreen = !!requestFullscreen;
  }

  ngAfterViewInit(): void {
    super.ngAfterViewInit();
    this.canvasNativeElement = super.getCanvasNativeElement(this.canvas);
    if(this.canvasNativeElement) {
      this.moduleDecorator = (mod) => {
        mod.arguments = ['testing1', 'testing2'];
        mod.preRun = [];
        mod.canvas = this.canvasNativeElement;
        mod.printErr = (what: string) => {
          if (!what.startsWith("WARNING")) {
            this.ngZone.run(() => (this.error = what));
          }
        };
      };
    }
  }

  toggleFullscreen() {
    if (requestFullscreen) {
      requestFullscreen.bind(this.canvasNativeElement)();
    }
  }

  renderRandomA(): void {
    console.log("Render Random A");
    if(this.module) {
      this.module.ccall("erandom", "void", [], []);
    }
  }

  renderRandomB(): void {
    console.log("Render Random B");
    if(this.module) {
      this.module.ccall("erandommax", "void", [], []);
    }
  }

  enableLoop(): void {
    if(this.module) {
      this.module.ccall("enable", "void", [], []);
    }
  }

  disabledLoop(): void {
    if(this.module) {
      this.module.ccall("disabled", "void", [], []);
    }
  }

  ngOnDestroy(): void {
    console.log("On ng destroy");
    if(this.module) {
      console.log("Calling shutdown in module");
      this.module.ccall("eshutdown", "void", [], []);
    }
  }
}
