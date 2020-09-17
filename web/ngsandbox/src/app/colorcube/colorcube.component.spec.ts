import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ColorcubeComponent } from './colorcube.component';

describe('ColorcubeComponent', () => {
  let component: ColorcubeComponent;
  let fixture: ComponentFixture<ColorcubeComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ ColorcubeComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(ColorcubeComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
