var Crawler = {
	
	print: function(data){
		var len = data.length
		console.log(data[len-1]);
	},

	plot: function(data) {
		var len = data.length;
		if (len <=0 )return;
		var other = data[len-1]['other'];
		var xx2   = data[len-1]['xx2'];
		var xx3   = data[len-1]['xx3'];
		var xx4   = data[len-1]['xx4'];
		var dataset = [{label:'other', count:other}, {label:'2xx', count:xx2}, {label:'3xx', count:xx3}, {label:'4xx', count:xx4}];
		console.log(JSON.stringify(dataset))
		// dataset = $.parseJSON(dataset);

		d3.select("#chart").selectAll("svg").remove();
		var chart = document.getElementById('chart');
		drawLegendChart(JSON.stringify(dataset), chart);
		d3.select("#chart").attr("align","center");
	}
}

 function drawLegendChart(dataset,selecter) {
	
	var dataset = $.parseJSON(dataset);

	var width = 360;
    var height = 360;
    var radius = Math.min(width, height) / 2;
    var donutWidth = 75;
    var legendRectSize = 18;                                  
    var legendSpacing = 4;                                    

    // var color = d3.scale.category20b();
    var color = d3.scaleOrdinal(d3.schemeCategory20b);

    var svg = d3.select(selecter)
      .append('svg')
      .attr('width', width)
      .attr('height', height)
      .append('g')
      .attr('transform', 'translate(' + (width / 2) + 
        ',' + (height / 2) + ')');

    var arc = d3.arc()
      .innerRadius(radius - donutWidth)
      .outerRadius(radius);

    var pie = d3.pie()
      .value(function(d) { return Number(d.count); })
      .sort(null);

    var path = svg.selectAll('path')
      .data(pie(dataset))
      .enter()
      .append('path')
      .attr('d', arc)
      .attr('fill', function(d, i) { 
        return color(d.data.label);
      });

    var legend = svg.selectAll('.legend')                     
      .data(color.domain())                                   
      .enter()                                                
      .append('g')                                            
      .attr('class', 'legend')                                
      .attr('transform', function(d, i) {                     
        var height = legendRectSize + legendSpacing;          
        var offset =  height * color.domain().length/2;     
        var horz = -2 * legendRectSize;                       
        var vert = i * height - offset;                       
        return 'translate(' + horz + ',' + vert + ')';        
      });                                                     

    legend.append('rect')                                     
      .attr('width', legendRectSize)                          
      .attr('height', legendRectSize)                         
      .style('fill', color)                                   
      .style('stroke', color);                                
      
    legend.append('text')                                     
      .attr('x', legendRectSize + legendSpacing)              
      .attr('y', legendRectSize - legendSpacing)              
      .text(function(d) { return d; });                       	
}
