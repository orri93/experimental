import { Component, OnInit } from '@angular/core';
import { SebastianService } from './../sebastian.service';

@Component({
  selector: 'app-sebastian',
  templateUrl: './sebastian.component.html',
  styleUrls: ['./sebastian.component.css']
})
export class SebastianComponent implements OnInit {

  constructor(private sebastianService: SebastianService) { }

  ngOnInit(): void {
    
  }

}
