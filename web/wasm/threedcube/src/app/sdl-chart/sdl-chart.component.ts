import { Component, ViewChild, ElementRef, NgZone } from '@angular/core';
import { HttpClient } from "@angular/common/http";
import { EmscriptenWasmComponent } from "../emscripten-wasm.component";

const requestFullscreen =
  document.documentElement.requestFullscreen ||
  document.documentElement["webkitRequestFullscreen"] ||
  document.documentElement["msRequestFullscreen"] ||
  document.documentElement["mozRequestFullScreen"];
  
@Component({
  selector: 'app-sdl-chart',
  templateUrl: './sdl-chart.component.html',
  styleUrls: ['./sdl-chart.component.css']
})
export class SdlChartComponent extends EmscriptenWasmComponent {
  @ViewChild("canvas") canvas: ElementRef;
  error: string;
  supportsFullscreen: boolean;
  
  constructor(private httpClient: HttpClient, private ngZone: NgZone) {
    super("ChartSdlModule", "sdl-chart.js");

    this.supportsFullscreen = !!requestFullscreen;

    this.moduleDecorator = (mod) => {
      mod.arguments = [];
      mod.preRun = [];
      mod.canvas = <HTMLCanvasElement>this.canvas.nativeElement;
      mod.printErr = (what: string) => {
        if (!what.startsWith("WARNING")) {
          this.ngZone.run(() => (this.error = what));
        }
      };
    };
  }

  toggleFullscreen() {
    if (requestFullscreen) {
      requestFullscreen.bind(this.canvas.nativeElement)();
    }
  }
}
