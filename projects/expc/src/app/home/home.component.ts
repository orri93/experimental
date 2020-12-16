import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';

export interface Demo {
  name: string;
  routerLink: string;
}

@Component({
  selector: 'app-home',
  templateUrl: './home.component.html',
  styleUrls: ['./home.component.css']
})
export class HomeComponent implements OnInit {
  demos: Demo[];

  constructor(router: Router) {
    this.demos = router.config
      .filter((r) => r.data && r.data.demo)
      .map((r) => ({
        name: r.data.name,
        routerLink: r.path,
      }));
  }

  ngOnInit(): void {
  }
}
