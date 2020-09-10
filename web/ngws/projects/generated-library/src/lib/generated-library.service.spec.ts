import { TestBed } from '@angular/core/testing';

import { GeneratedLibraryService } from './generated-library.service';

describe('GeneratedLibraryService', () => {
  let service: GeneratedLibraryService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(GeneratedLibraryService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
