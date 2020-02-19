// https://stackoverflow.com/questions/36975619/how-to-call-a-rest-web-service-api-from-javascript

function drawLineChart(data) {
  var svgWidth = 600, svgHeight = 400;
  var margin = { top: 20, right: 20, bottom: 30, left: 50 };
  var width = svgWidth - margin.left - margin.right;
  var height = svgHeight - margin.top - margin.bottom;

  var svg = d3.select('svg').attr("width", svgWidth).attr("height", svgHeight);
  
  var g = svg.append("g").attr("transform", "translate(" + margin.left + "," + margin.top + ")");

  var x = d3.scaleTime().rangeRound([0, width]);
  var y = d3.scaleLinear().rangeRound([height, 0]);

  var line = d3.line().x(function(d) { return x(d.x); }).y(function(d) { return y(d.y) });
  
  x.domain(d3.extent(data, function(d) { return d.x }));
  y.domain(d3.extent(data, function(d) { return d.y }));
  
  g.append("g").attr("transform", "translate(0," + height + ")")
    .call(d3.axisBottom(x)).select(".domain").remove();

  g.append("g").call(d3.axisLeft(y)).append("text").attr("fill", "#000")
    .attr("transform", "rotate(-90)").attr("y", 6).attr("dy", "0.71em")
    .attr("text-anchor", "end").text("Price ($)");

  g.append("path").datum(data).attr("fill", "none").attr("stroke", "steelblue")
    .attr("stroke-linejoin", "round").attr("stroke-linecap", "round")
    .attr("stroke-width", 1.5).attr("d", line);

  return {l: line, dx: x, dy: y};
}

const onfirst = async () => {
  const response = await fetch("/api/v1");
  const rjson = await response.json();

  chart = drawLineChart(rjson.records);

  var clickmediv = document.getElementById("clickme");
  clickmediv.style.display = "none";

  var hostname = document.location.hostname;
  var wsurl = 'ws://' + hostname + ':3002/ws';

  var wsc = new WebSocket(wsurl);
  if (wsc) {
    wsc.onmessage = function(message) {
      var wsdata = JSON.parse(message.data);
      rjson.records.push(wsdata.record);
      chart.dx.domain(d3.extent(rjson.records, function(d) { return d.x }));
      chart.dy.domain(d3.extent(rjson.records, function(d) { return d.y }));
      var svg = d3.select("svg").transition();
      svg.select(".line")   // change the line
            .duration(750)
            .attr("d", chart.l(rjson.records));
    }
  }
}

const restpostexample = async () => {
  const response = await fetch('http://example.com/movies.json', {
    method: 'POST',
    body: myBody, // string or object
    headers: {
      'Content-Type': 'application/json'
    }
  });
  const myJson = await response.json(); //extract JSON from the http response
  // do something with myJson
}
