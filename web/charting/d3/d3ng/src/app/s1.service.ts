import { Injectable } from '@angular/core';

const webkitRequestFullscreen = 'webkitRequestFullscreen';
const msRequestFullscreen = 'msRequestFullscreen';
const mozRequestFullScreen = 'mozRequestFullScreen';

@Injectable({
  providedIn: 'root'
})
export class S1Service {

  static getRequestFullscreen(doc: Document): any {
    return doc.documentElement.requestFullscreen ||
    doc.documentElement[webkitRequestFullscreen] ||
    doc.documentElement[msRequestFullscreen] ||
    doc.documentElement[mozRequestFullScreen];
  }

  constructor() { }
}
