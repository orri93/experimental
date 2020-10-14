import { TestBed } from '@angular/core/testing';

import { InterpolateService } from './interpolate.service';

describe('InterpolateService', () => {
  let service: InterpolateService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(InterpolateService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
