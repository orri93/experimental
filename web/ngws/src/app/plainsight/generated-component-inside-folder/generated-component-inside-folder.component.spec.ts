import { ComponentFixture, TestBed } from '@angular/core/testing';

import { GeneratedComponentInsideFolderComponent } from './generated-component-inside-folder.component';

describe('GeneratedComponentInsideFolderComponent', () => {
  let component: GeneratedComponentInsideFolderComponent;
  let fixture: ComponentFixture<GeneratedComponentInsideFolderComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ GeneratedComponentInsideFolderComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(GeneratedComponentInsideFolderComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
