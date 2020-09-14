import { ComponentFixture, TestBed } from '@angular/core/testing';

import { HmcChartComponent } from './hmc-chart.component';

describe('HmcChartComponent', () => {
  let component: HmcChartComponent;
  let fixture: ComponentFixture<HmcChartComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ HmcChartComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(HmcChartComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
