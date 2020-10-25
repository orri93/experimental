import { TestBed } from '@angular/core/testing';

import { IntervalPerformanceService } from './interval-performance.service';

describe('IntervalPerformanceService', () => {
  let service: IntervalPerformanceService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(IntervalPerformanceService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
