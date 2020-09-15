import { Component, OnInit } from '@angular/core';
import { HttpClient } from '@angular/common/http';


@Component({
  selector: 'app-sandbox',
  templateUrl: './sandbox.component.html',
  styleUrls: ['./sandbox.component.css']
})
export class SandboxComponent implements OnInit {

  constructor(private http: HttpClient) { }

  ngOnInit(): void {
    // Sample API
    const apiLink = '/assets/data.json';

    this.getApiResponse(apiLink).then(
      data => {
        console.log("Getting Data from API " + apiLink + ": " + data);
      },
      error => {
        console.log("Error getting API response for " + apiLink + ": " + error);
      });
  }

  getApiResponse(url) {
    return this.http.get(url, {}).toPromise().then(res => {
      return res;
    });
  }

}
