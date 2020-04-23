var	gGraphUpdateTimerStarted = 0;
var	gGraphRequestCount	= 0;
var	gGraphTimeOut;
var	gGraphHttpRequest;
var	gGraphType = 0;
var gPlotCalled = 0;
var gPlot;
function GraphRequestUpdate()
{
	var URL;
	switch (gGraphType)
	{
		case 0:
		{
			URL = "sri_cpu.json -nocache" + Math.random();
			break;
		}
		case 1:
		{
			URL = "sri_events.json -nocache" + Math.random();
			break;
		}
		case 2:
		{
			URL = "sri_files.json -nocache" + Math.random();
			break;
		}
		case 3:
		{
			URL = "sri_timers.json -nocache" + Math.random();
			break;
		}
		default:
		{
			URL = "sri_heap" + (gGraphType - 4) + ".json -nocache" + Math.random();
			break;
		}
	}
	$.ajax({
		url: URL,
		method: 'GET',
		dataType: 'json',
		success: onCpuReceived
	});
}
function onCpuReceived(series)
{
	var options = {
		lines: { show: true, lineWidth: 1 },
		points: { show: false },
		xaxis: { tickDecimals: 0, tickSize: 1 },
		yaxis: { tickDecimals: 0, tickSize: 10, min: 0, max: 100 }
	};
	if (gPlotCalled)
	{
		gPlot.setData([ series ]);
		gPlot.setupGrid();
		gPlot.draw();
	}
	else
	{
		var	data = [ series	];
		gPlot = $.plot($("#cpuUsage"), data, options);
		gPlotCalled = 1;
	}
	gGraphRequestCount = 0;
}
function GraphUpdateTimer()
{
	var	GraphRefreshInterval = 0;
	switch (gGraphRequestCount)
	{
		case 0:
		{
			GraphRefreshInterval = 1000;
			break;
		}
		case 1:
		{
			GraphRefreshInterval = 2000;
			break;
		}
		case 2:
		{
			GraphRefreshInterval = 5000;
			break;
		}
		case 3:
		{
			GraphRefreshInterval = 10000;
			break;
		}
		case 4:
		{
			GraphRefreshInterval = 20000;
			break;
		}
		case 5:
		{
			GraphRefreshInterval = 30000;
			break;
		}
		case 6:
		{
			GraphRefreshInterval = 60000;
			break;
		}
		case 7:
		{
			GraphRefreshInterval = 120000;
			break;
		}
		default:
		{
			GraphRefreshInterval = 0;
			break
		}
	}
	GraphRequestUpdate();
	if (GraphRefreshInterval)
	{
		gGraphTimeOut = setTimeout("GraphUpdateTimer()", GraphRefreshInterval);
	}
}
function setGraphType(form)
{
	gGraphType = form.graph_select.selectedIndex;
}
function StartGraphUpdate()
{
	if (!gGraphUpdateTimerStarted)
	{
		gGraphUpdateTimerStarted = 1;
		GraphUpdateTimer();
	}
}