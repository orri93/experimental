import { Component, ViewChild, ElementRef, NgZone, OnDestroy } from '@angular/core';
import { interval, Subscription } from 'rxjs';
import { HttpClient } from "@angular/common/http";
import { HttpClientDataService } from './../http-client-data.service';
import { WebSocketService, TYPE_START, TYPE_STOP, TYPE_UPDATE } from './../web-socket.service';
import { EmscriptenWasmComponent } from "../emscripten-wasm.component";
import { InterpolateService } from './../interpolate.service';
import { AppConfiguration } from '../app.configuration';
import { DataService } from './../data.service';

const RES_X = 600.0;
const RES_Y = 400.0;

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

  constructor(
    private webSocketService: WebSocketService,
    private dataService: HttpClientDataService,
    private httpClient: HttpClient,
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

    this.webSocketService.connect();
  }

  toggleFullscreen() {
    if (requestFullscreen) {
      requestFullscreen.bind(this.canvas.nativeElement)();
    }
  }

  showMatrix() {
    this.dataService.getMatrix(RES_X).subscribe(m => {
      console.log("Got Matrix");
      let v, n, mu, x1: number;

      let yRange: DataRange = m.r.y;

      this.module.ccall("lock", "void", [], []);

      for(let x = 0; x < RES_X; x++) {
        let vector: Vector = x < m.v.length ? m.v[x] : null;
        for(let y = 0; y < RES_Y; y++) {
          let index = DataService.vectorIndex(vector, y);
          if(index >= 0) {
            x1 = vector.p[index].x;
            mu = (y - x1) / (vector.p[index + 1].x - x1);
            v = InterpolateService.interpolateLinear(vector.p[index].y, vector.p[index + 1].y, mu);
            n = (v - yRange.f) / (yRange.t - yRange.f);
            this.module.ccall(
              "set_pixel",
              "void",
              ["number", "number", "number"],
              [x, y, n]);
          } else {
            this.module.ccall(
              "unset_pixel",
              "void",
              ["number", "number"],
              [x, y]);
          }
        }
      }

      this.module.ccall("unlock_and_flip", "void", [], []);

      this.webSocketService.dataUpdates().subscribe( (message: WsMessage) => {
        if(message.t === TYPE_UPDATE && message.u) {
          let update: WsUpdate = message.u;
          let vector: Vector = update.v;

          this.module.ccall("lock", "void", [], []);
          this.module.ccall("shift", "void", [], []);

          for(let y = 0; y < RES_Y; y++) {
            let index = DataService.vectorIndex(vector, y);
            if(index >= 0) {
              x1 = vector.p[index].x;
              mu = (y - x1) / (vector.p[index + 1].x - x1);
              v = InterpolateService.interpolateLinear(vector.p[index].y, vector.p[index + 1].y, mu);
              n = (v - yRange.f) / (yRange.t - yRange.f);
              this.module.ccall(
                "set_pixel_last_column",
                "void",
                ["number", "number"],
                [y, n]);
            } else {
              this.module.ccall(
                "unset_pixel_last_column",
                "void",
                ["number"],
                [y]);
            }
          }

          this.module.ccall("unlock_and_flip", "void", [], []);
        }
      });

      let wsStart: WsStart = { f: RES_X };
      let wsMessage: WsMessage = { t: TYPE_START, s: wsStart };
      this.webSocketService.sendMessage(wsMessage);
  
    });

  }

  start(): void {
    console.log("Start Button Clicked");

    let getMatrixUrl = HttpClientDataService.createGetMatrixUrl(RES_X);
    let wsUrl = "";

    this.module.ccall(
      "start",
      "void",
      ["string", "string"],
      [getMatrixUrl, wsUrl]);
  }

  stop(): void {
    console.log("Stop Button Clicked");

    let wsMessage: WsMessage = { t: TYPE_STOP };
    this.webSocketService.sendMessage(wsMessage);
  }

  ngOnDestroy(): void {
    console.log("On ng destroy");
    this.webSocketService.close();
    if(this.module) {
      console.log("Calling shutdown in module");
      this.module.ccall("shutdown", "void", [], []);
    }
  }
}
