import { ComponentFixture, TestBed } from '@angular/core/testing';

import { HmcComponent } from './hmc.component';

describe('HmcComponent', () => {
  let component: HmcComponent;
  let fixture: ComponentFixture<HmcComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ HmcComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(HmcComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
