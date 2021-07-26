import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';

export interface Page {
  name: string;
  routerLink: string;
}

@Component({
  selector: 'app-ph',
  templateUrl: './ph.component.html',
  styleUrls: ['./ph.component.scss']
})
export class PhComponent implements OnInit {
  pageName = 'Home Page';
  pages: Page[];

  constructor(router: Router) {
    this.pages = router.config
      .filter((r) => r.data && r.data.type === 'page')
      .map((r) => ({
        name: r.data.name,
        routerLink: r.path
      }));
  }

  ngOnInit(): void {
  }
}
