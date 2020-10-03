import { ComponentFixture, TestBed } from '@angular/core/testing';

import { SdlHeatmapComponent } from './sdl-heatmap.component';

describe('SdlHeatmapComponent', () => {
  let component: SdlHeatmapComponent;
  let fixture: ComponentFixture<SdlHeatmapComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ SdlHeatmapComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(SdlHeatmapComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
