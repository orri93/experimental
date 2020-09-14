export interface Unit {
  value: number;
  unit: string;
  unit_abbr: string;
}

export interface Configuration {
  ecd_upper_limit: Unit
  ecd_lower_limit: Unit;
  ecd_sensor_yellow_limit: Unit;
  ecd_sensor_green_limit: Unit;
  ecd_shoe_red_limit: Unit;
  ecd_shoe_yellow_limit: Unit;
  ecd_lot_value: Unit;
  shoe_depth: Unit;
}
