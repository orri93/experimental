import { Component } from '@angular/core';
import { Router } from '@angular/router';

import { Page } from './../shared/models/page';

@Component({
  selector: 'app-ph',
  templateUrl: './ph.component.html',
  styleUrls: ['./ph.component.scss']
})
export class PhComponent {
  pageName = 'Home Page';
  pages: Page[];

  constructor(private router: Router) {
    this.pages = router.config
      .filter((r) => r.data && r.data.type === 'page')
      .map((r) => ({
        name: r.data.name,
        routerLink: r.path
      }));
  }
}
