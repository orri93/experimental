import { Component, ViewChild, ElementRef, NgZone, OnDestroy } from '@angular/core';
import { environment } from './../../environments/environment';
import { HttpClientDataService } from './../http-client-data.service';
import { EmscriptenWasmComponent } from "../emscripten-wasm.component";

const RES_X = 600.0;
const RES_Y = 400.0;

const requestFullscreen =
  document.documentElement.requestFullscreen ||
  document.documentElement["webkitRequestFullscreen"] ||
  document.documentElement["msRequestFullscreen"] ||
  document.documentElement["mozRequestFullScreen"];

export function setPerformanceResult(formatted: string) {
  var resultElement = document.getElementById('custom-ws-performance-result');
  if(resultElement) {
    resultElement.innerHTML = formatted;
  }
}

export function setPerformanceIntervalResult(formatted: string) {
  var resultElement = document.getElementById('custom-ws-interval-performance-result');
  if(resultElement) {
    resultElement.innerHTML = "Interval - " + formatted;
  }
}

@Component({
  selector: 'app-custom-ws',
  templateUrl: './custom-ws.component.html',
  styleUrls: ['./custom-ws.component.css']
})
export class CustomWsComponent extends EmscriptenWasmComponent implements OnDestroy {
  @ViewChild("canvas") canvas: ElementRef;

  error: string;
  supportsFullscreen: boolean;

  constructor(
    private ngZone: NgZone) {
    super("SdlHeatmapModule", "gossdlheatmap.js");

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

  start(): void {
    console.log("Start Button Clicked");

    let loadMatrixUrl = HttpClientDataService.createGetMatrixUrl(RES_X);
    let wsUrl = environment.wsPath;
    let startFrom = RES_X;

    if(this.module) {
      console.log("Calling advanced start in module");
      this.module.ccall(
        "advanced_start",
        "void",
        ["string", "string", "number"],
        [loadMatrixUrl, wsUrl, startFrom]);
    }
  }

  stop(): void {
    console.log("Stop Button Clicked");

    if(this.module) {
      console.log("Calling advanced stop in module");
      this.module.ccall("advanced_stop", "void", [], []);
    }
  }

  ngOnDestroy(): void {
    console.log("On ng destroy");
    if(this.module) {
      console.log("Calling shutdown in module");
      this.module.ccall("shutdown", "void", [], []);
    }
  }

}
