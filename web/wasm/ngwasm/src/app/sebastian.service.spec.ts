import { TestBed } from '@angular/core/testing';

import { SebastianService } from './sebastian.service';

describe('SebastianService', () => {
  let service: SebastianService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(SebastianService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
