import { Component, ViewChild, ElementRef, NgZone, OnDestroy } from '@angular/core';
import { interval, Subscription } from 'rxjs';
import { HttpClient } from "@angular/common/http";
import { EmscriptenWasmComponent } from "../emscripten-wasm.component";
import { AppConfiguration } from '../app.configuration';

const requestFullscreen =
  document.documentElement.requestFullscreen ||
  document.documentElement["webkitRequestFullscreen"] ||
  document.documentElement["msRequestFullscreen"] ||
  document.documentElement["mozRequestFullScreen"];

@Component({
  selector: 'app-custom',
  templateUrl: './custom.component.html',
  styleUrls: ['./custom.component.css']
})
export class CustomComponent extends EmscriptenWasmComponent implements OnDestroy {
  @ViewChild("canvas") canvas: ElementRef;

  error: string;
  supportsFullscreen: boolean;

  constructor(private httpClient: HttpClient, private ngZone: NgZone) {
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

  ngOnDestroy(): void {
    console.log("On ng destroy");
    if(this.module) {
      console.log("Calling shutdown in module");
      this.module.ccall("shutdown", "void", [], []);
    }
  }
}
