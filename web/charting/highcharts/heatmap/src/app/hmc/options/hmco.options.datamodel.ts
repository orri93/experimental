import { Chart } from './chart';
import { XAxis } from './xaxis';
import { YAxis } from './yaxis';
import { Boost } from './boost';
import { TextOption } from './text-option';
import { EnabledOption } from './enabled-option';
import { ColorAxis } from './color-axis';
import { Legend } from './Legend';
import { PlotOptions } from './plot-options';

export interface Hmco {
  chart?: Chart;
  tooltip?: EnabledOption;
  boost?: Boost;
  title?: TextOption;
  credits?: EnabledOption;
  exporting?: EnabledOption;
  xAxis?: XAxis;
  yAxis?: YAxis;
  colorAxis?: ColorAxis;
  legend?: Legend;
  plotOptions?: PlotOptions;
  series?: Array<any>;
}
