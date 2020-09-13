import { enableProdMode } from '@angular/core';
import { platformBrowserDynamic } from '@angular/platform-browser-dynamic';

import { AppModule } from './app/app.module';
import { environment } from './environments/environment';


require('highcharts/highcharts-more')(Highcharts);
require('highcharts/modules/solid-gauge')(Highcharts);
require('highcharts/modules/heatmap')(Highcharts);
require('highcharts/modules/exporting')(Highcharts);
require('highcharts/modules/offline-exporting')(Highcharts);
require('highcharts/modules/export-data')(Highcharts);
require('highcharts/modules/no-data-to-display')(Highcharts);

require('highcharts/modules/map')(Highcharts);
require('highcharts/modules/boost')(Highcharts);
require('highcharts/modules/boost-canvas')(Highcharts)
let Delaunay = require('delaunay-fast/delaunay');
require('../assets/highchartsplugins/vertical-gauge')(Highcharts);
require('../assets/highchartsplugins/highcharts-contour')(Highcharts, Delaunay);
require('../assets/highchartsplugins/highcharts-coloraxis-bands')(Highcharts);
require('../assets/highchartsplugins/pattern-fill-v2')(Highcharts);
require('../assets/highchartsplugins/customSymbol')(Highcharts);

// Global defaults
Highcharts.setOptions({
  lang: {
      thousandsSep: ',',
      noData: null
  }
});

if (environment.production) {
  enableProdMode();
}

platformBrowserDynamic().bootstrapModule(AppModule)
  .catch(err => console.error(err));
