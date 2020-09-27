import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ThreeDeeCubeComponent } from './three-dee-cube.component';

describe('ThreeDeeCubeComponent', () => {
  let component: ThreeDeeCubeComponent;
  let fixture: ComponentFixture<ThreeDeeCubeComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ ThreeDeeCubeComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(ThreeDeeCubeComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
