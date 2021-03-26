import { TestBed } from '@angular/core/testing';

import { M5Service } from './m5.service';

describe('M5Service', () => {
  let service: M5Service;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(M5Service);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
