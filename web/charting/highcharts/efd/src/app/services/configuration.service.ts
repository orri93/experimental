import { Injectable, OnDestroy } from '@angular/core';
import { Configuration, Unit } from '../models/configuration';

@Injectable({
  providedIn: 'root'
})
export class ConfigurationService implements Configuration {

  ecd_upper_limit: Unit;
  ecd_lower_limit: Unit;
  ecd_sensor_yellow_limit: Unit;
  ecd_sensor_green_limit: Unit;
  ecd_shoe_red_limit: Unit;
  ecd_shoe_yellow_limit: Unit;
  ecd_lot_value: Unit;
  shoe_depth: Unit;

  constructor() { }

  
}
