import { ComponentFixture, TestBed } from '@angular/core/testing';

import { WaterDropComponent } from './water-drop.component';

describe('WaterDropComponent', () => {
  let component: WaterDropComponent;
  let fixture: ComponentFixture<WaterDropComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ WaterDropComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(WaterDropComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
