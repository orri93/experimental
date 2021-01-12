const SIZE = { width: 250, height: 200 };

const RANGE = {
  x: { min: 0, max: 100 },
  y: { min: 0, max: 100 },
  z: { min: 0, max: 10 }
};

const DUMMY_DATA = [
  { x: 10, y: 30, z: 10 },
  { x: 20, y: 10, z: 5 }
];

var data = [];

const rw = SIZE.width / (RANGE.x.max - RANGE.x.min);
const rh = SIZE.height / (RANGE.y.max - RANGE.y.min);

const xScale = d3.scaleLinear()
  .domain([RANGE.x.min, RANGE.x.max])
  .range([0, SIZE.width]);
const yScale = d3.scaleLinear()
  .domain([RANGE.y.min, RANGE.y.max])
  .range([SIZE.height, 0]);

const zScale = d3.scaleSequential(d3.interpolateBlues)
  .domain([RANGE.z.min, RANGE.z.max]);

const gosd3hm = d3.select('#gosd3hm')
  .classed('gosd3hms', true)
  .style('width', SIZE.width + 'px')
  .style('height', SIZE.height + 'px');

const pixels = gosd3hm
  .selectAll('.gosd3hmp')
  .data(DUMMY_DATA)
  .enter()
  .append('rect')
  .classed('gosd3hmp', true)
  .attr('fill', (data) => zScale(data.z))
  .attr('width', rw)
  .attr('height', rh)
  .attr('x', (data) => xScale(data.x))
  .attr('y', (data) => yScale(data.y));
