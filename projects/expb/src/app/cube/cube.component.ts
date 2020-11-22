import { Component, ViewChild, ElementRef, NgZone, OnDestroy } from '@angular/core';
import { HttpClient } from "@angular/common/http";
import { EmscriptenWasmComponent } from '../emscripten-wasm.component';

const getFileName = (filePath: string) => filePath.split("/").reverse()[0];

const allowedMimeTypes = ["image/bmp", "image/x-windows-bmp", "image/jpeg", "image/pjpeg", "image/png"];

const defaultImage = "assets/img/angular.png";

const requestFullscreen =
  document.documentElement.requestFullscreen ||
  document.documentElement["webkitRequestFullscreen"] ||
  document.documentElement["msRequestFullscreen"] ||
  document.documentElement["mozRequestFullScreen"];

@Component({
  selector: 'app-cube',
  templateUrl: './cube.component.html',
  styleUrls: ['./cube.component.css']
})
export class CubeComponent extends EmscriptenWasmComponent implements OnDestroy {
  @ViewChild("canvas") canvas: ElementRef;

  title = '3D Cube';

  error: string;
  supportsFullscreen: boolean;

  predefinedImages: string[];
  fileUploadAccept: string;

  constructor(private httpClient: HttpClient, private ngZone: NgZone) {
    super("CubeModule", "cube.js");

    this.supportsFullscreen = !!requestFullscreen;
    this.fileUploadAccept = allowedMimeTypes.join(",");
    this.predefinedImages = [defaultImage, "assets/img/cat.png", "assets/img/embroidery.png"];

    this.moduleDecorator = (mod) => {
      mod.arguments = [getFileName(defaultImage)];
      mod.preRun = [
        () => {
          mod.FS_createPreloadedFile("/", getFileName(defaultImage), defaultImage, true);
        }
      ];
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

  selectPredefinedImage(index: number) {
    this.error = null;

    const imageUrl: string = this.predefinedImages[index];
    this.httpClient
      .get(imageUrl, { responseType: "arraybuffer" })
      .subscribe((imageBytes) => this.setTexture(getFileName(imageUrl), new Uint8Array(imageBytes)));
  }

  onFileUploaded(files: FileList) {
    if (!files.length) {
      return;
    }

    this.error = null;

    const file = files[0];
    if (allowedMimeTypes.indexOf(file.type) < 0) {
      this.error = `Unsupported mime type ${file.type}`;
      return;
    }

    const fileName = file.name;

    const reader = new FileReader();
    reader.onload = () => {
      const inputArray = new Uint8Array(<ArrayBuffer>reader.result);
      this.setTexture(fileName, inputArray);
    };
    reader.readAsArrayBuffer(file);
  }

  private setTexture(fileName: string, inputArray: Uint8Array) {
    const isDefaultImage = fileName === getFileName(defaultImage);

    // Default image is always there
    if (!isDefaultImage) {
      this.module.FS_createDataFile("/", fileName, inputArray, true);
    }

    this.module.ccall("settexture", "void", ["string"], [fileName]);

    // Delete the file afterwards to free memory
    if (!isDefaultImage) {
      this.module.FS_unlink(fileName);
    }
  }

  ngOnDestroy(): void {
    console.log("On ng destroy");
  }
}
