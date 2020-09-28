import { ComponentFixture, TestBed } from '@angular/core/testing';

import { SdlCubeComponent } from './sdl-cube.component';

describe('SdlCubeComponent', () => {
  let component: SdlCubeComponent;
  let fixture: ComponentFixture<SdlCubeComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ SdlCubeComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(SdlCubeComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
