/**
 * license N/A
 * Vertical Gauge module
 *
 * (c) 2017 NOV
 *
 * License: N/A
 */
'use strict';
(function (factory) {
    if (typeof module === 'object' && module.exports) {
        module.exports = factory;
    } else {
        factory(Highcharts);
    }
}(function (Highcharts) {
    (function (H) {
        /* eslint indent:0 */

        H.SVGRenderer.prototype.symbols.pointer = function (x, y, w, h) {
            return ['M', x - w / 2, y + h / 2,
                'L', x - w / 2, y - h / 2, x + w * 2, y - h / 2, x + w * 2, y + h / 2,
                'z'
            ];

        };
        if (H.VMLRenderer) {
            H.VMLRenderer.prototype.symbols.pointer = H.SVGRenderer.prototype.symbols.pointer;
        }
}(Highcharts));
}));
