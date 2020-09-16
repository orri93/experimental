import { Component, OnInit } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { interval, Subscription } from 'rxjs';

@Component({
  selector: 'app-sandbox',
  templateUrl: './sandbox.component.html',
  styleUrls: ['./sandbox.component.css']
})
export class SandboxComponent implements OnInit {

  subscription: Subscription;

  constructor(private http: HttpClient) { }

  ngOnInit(): void {
    // Sample API
    const apiLink: string = '/assets/data.json';

    const now: Date = new Date();

    this.getApiResponse(apiLink).then(
      data => {
        // Set 10 seconds interval to update data again and again
        const source = interval(10000);

        const s0 = [];
        const s0d = data['s0'];
        console.log("Getting Data from API " + apiLink);
        console.log("S0: " + s0d);
        let i: number = 0;
        s0d.forEach(element => {
          let ts: Date = new Date();
          ts.setHours(now.getHours() + i++);
          const temrow = [
            ts, element
          ];
          s0.push(temrow);
        });
        console.log("S0: " + s0);
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
