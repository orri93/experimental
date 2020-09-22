import { ComponentFixture, TestBed } from '@angular/core/testing';

import { SebastianComponent } from './sebastian.component';

describe('SebastianComponent', () => {
  let component: SebastianComponent;
  let fixture: ComponentFixture<SebastianComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ SebastianComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(SebastianComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
