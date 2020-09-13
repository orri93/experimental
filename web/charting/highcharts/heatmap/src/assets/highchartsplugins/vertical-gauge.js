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

        // create shortcuts
        var defaultOptions = H.getOptions(),
            extendClass = H.extendClass,
            merge = H.merge,
            seriesTypes = H.seriesTypes;
        /**
         * The series type factory. This will be included in HC5.
         */
        H.seriesType = function (type, parent, options, props) {

            // Merge the options
            defaultOptions.plotOptions[type] = merge(
                defaultOptions.plotOptions[parent],
                options
            );

            // Create the class
            seriesTypes[type] = extendClass(seriesTypes[parent], props);
        };

        H.seriesType('marker', 'scatter', null, {
            drawPoints: function () {
                var series = this,
                    chart = this.chart,
                    renderer = chart.renderer;

                H.each(this.points, function (point) {

                    if (!point.graphic) {
                        point.graphic = renderer.g()
                            .attr({
                                zIndex: 9
                            })
                            .add();
                        point.line = renderer.path([
                            'M', 0, 0,
                            'L', chart.plotWidth, 0,
                            'z'
                        ]).attr({
                            'stroke-width': 1,
                            'stroke': 'red'
                        }).add(point.graphic);
                        point.wings = renderer.rect(-10, -10, chart.plotWidth + 20, 20, 5)
                            .attr({
                                fill: 'none',
                                'stroke': 'gray',
                                'stroke-width': 2
                            })
                            .add(point.graphic)
                    }

                    point.graphic['attr']({
                        translateX: chart.plotLeft,
                        translateY: chart.plotTop + point.plotY
                    });

                });
            }
        });
    }(Highcharts));
}));
