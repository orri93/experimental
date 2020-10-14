import { TestBed } from '@angular/core/testing';

import { InMemoryDataOverrideService } from './in-memory-data-override.service';

describe('InMemoryDataOverrideService', () => {
  let service: InMemoryDataOverrideService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(InMemoryDataOverrideService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
