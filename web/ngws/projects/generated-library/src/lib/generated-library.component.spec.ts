import { ComponentFixture, TestBed } from '@angular/core/testing';

import { GeneratedLibraryComponent } from './generated-library.component';

describe('GeneratedLibraryComponent', () => {
  let component: GeneratedLibraryComponent;
  let fixture: ComponentFixture<GeneratedLibraryComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ GeneratedLibraryComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(GeneratedLibraryComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
