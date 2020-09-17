import { TestBed } from '@angular/core/testing';

import { WaterDropGeneratorService } from './water-drop-generator.service';

describe('WaterDropGeneratorService', () => {
  let service: WaterDropGeneratorService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(WaterDropGeneratorService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
