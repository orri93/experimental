import { TestBed } from '@angular/core/testing';

import { LcDataService } from './lc-data.service';

describe('LcDataService', () => {
  let service: LcDataService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(LcDataService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });

  it('should return index for a vector value for a row', () => {
    let row: number;
    let index: number;
    let vector: Vector = {values: [10.0, 20.0, 30.0]};

    row = 5;
    index = LcDataService.vectorIndex(vector, row);
    expect(index).toEqual(-1);

    row = 10;
    index = LcDataService.vectorIndex(vector, row);
    expect(index).toEqual(0);

    row = 11;
    index = LcDataService.vectorIndex(vector, row);
    expect(index).toEqual(0);

    row = 20;
    index = LcDataService.vectorIndex(vector, row);
    expect(index).toEqual(0);

    row = 22;
    index = LcDataService.vectorIndex(vector, row);
    expect(index).toEqual(1);

    row = 22;
    index = LcDataService.vectorIndex(vector, row);
    expect(index).toEqual(1);

    row = 30;
    index = LcDataService.vectorIndex(vector, row);
    expect(index).toEqual(1);

    row = 31;
    index = LcDataService.vectorIndex(vector, row);
    expect(index).toEqual(-1);
  });
});
