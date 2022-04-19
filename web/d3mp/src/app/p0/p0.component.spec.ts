import { ComponentFixture, TestBed } from '@angular/core/testing';

import { P0Component } from './p0.component';

describe('P0Component', () => {
  let component: P0Component;
  let fixture: ComponentFixture<P0Component>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ P0Component ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(P0Component);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
