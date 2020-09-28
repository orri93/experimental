import { ComponentFixture, TestBed } from '@angular/core/testing';

import { SdlChartComponent } from './sdl-chart.component';

describe('SdlChartComponent', () => {
  let component: SdlChartComponent;
  let fixture: ComponentFixture<SdlChartComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ SdlChartComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(SdlChartComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
