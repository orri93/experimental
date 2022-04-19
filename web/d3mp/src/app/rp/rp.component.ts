import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';

export interface Page {
  name: string;
  routerLink: string;
}

@Component({
  selector: 'app-rp',
  templateUrl: './rp.component.html',
  styleUrls: ['./rp.component.scss']
})
export class RpComponent implements OnInit {
  pageName = 'Home Page';
  pages: Page[];

  constructor(router: Router) {
    this.pages = router.config
      .filter((r) => r.data && r.data['type'] === 'page')
      .map((r) => ( {
        name: r.data?.name,
        routerLink: r.path as string
      } ));
  }

  ngOnInit(): void {
  }

}
