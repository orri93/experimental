import { ComponentFixture, TestBed } from '@angular/core/testing';

import { M1c1Component } from './m1c1.component';

describe('M1c1Component', () => {
  let component: M1c1Component;
  let fixture: ComponentFixture<M1c1Component>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ M1c1Component ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(M1c1Component);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
