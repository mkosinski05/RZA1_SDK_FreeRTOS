/*****************************************************************************
* (C) ARDWare ltd. 2010-2011. All Rights Reserved.
* www.ardware.co.uk
* $Workfile: $
* $Revision: $
* $Project:	 $
* $Date		 $
******************************************************************************/
var	gMsTestFileName = "ms_test.cgi?";
var	gMsTestElementID = "ms_test";
var	gMsTestUpdateTimerStarted = 0;
var	gMsTestRequestCount = 0;
var	gMsTestTimeOut;
var	gMsTestHttpRequest;
var gTestSize = 0;
var gDriveLetter = 0;
function MsTestCreateHttpRequest()
{
	var xmlhttp = false;
	var e;
	if (window.XMLHttpRequest)
	{
		xmlhttp	= new XMLHttpRequest();
	}
	else
	{
		xmlhttp	= new ActiveXObject("Microsoft.XMLHTTP");
		try
		{
			httprequest=new	ActiveXObject("Msxml2.XMLHTTP");
		} 
		catch (e)
		{
			try
			{
				httprequest = new ActiveXObject("Microsoft.XMLHTTP");
			} catch	(e)
			{
			}
		}
	}
	return xmlhttp
}
function MsTestRequestUpdate()
{
	gMsTestHttpRequest = MsTestCreateHttpRequest();
	if (gMsTestHttpRequest)
	{
		gMsTestHttpRequest.onreadystatechange = MsTestUpdateContents;
		gMsTestHttpRequest.open('GET', gMsTestFileName + " -nocache" + Math.random(), true);
		gMsTestHttpRequest.send(null);
		gMsTestRequestCount++;
	}
}
function MsTestUpdateContents()
{
	if (gMsTestHttpRequest.readyState == 4)
	{
		if (gMsTestHttpRequest.status == 200)
		{
			document.getElementById(gMsTestElementID).innerHTML = gMsTestHttpRequest.responseText;
			gMsTestRequestCount = 0;
		}
	}
}
function MsTestUpdateTimer()
{
	var	MsTestRefreshInterval = 0;
	switch (gMsTestRequestCount)
	{
		case 0:
		{
			MsTestRefreshInterval = 1000;
			break;
		}
		case 1:
		{
			MsTestRefreshInterval = 2000;
			break;
		}
		case 2:
		{
			MsTestRefreshInterval = 5000;
			break;
		}
		case 3:
		{
			MsTestRefreshInterval = 10000;
			break;
		}
		case 4:
		{
			MsTestRefreshInterval = 20000;
			break;
		}
		case 5:
		{
			MsTestRefreshInterval = 30000;
			break;
		}
		case 6:
		{
			MsTestRefreshInterval = 60000;
			break;
		}
		case 7:
		{
			MsTestRefreshInterval = 120000;
			break;
		}
		default:
		{
			MsTestRefreshInterval = 0;
			break
		}
	}
	MsTestRequestUpdate();
	if (MsTestRefreshInterval)
	{
		gMsTestTimeOut = setTimeout("MsTestUpdateTimer()", MsTestRefreshInterval);
	}
}
function MsTestScreenUpdate()
{
	if (!gMsTestUpdateTimerStarted)
	{
		gMsTestUpdateTimerStarted = 1;
		MsTestUpdateTimer();
	}
}
function msSetTestSize(form)
{
	gTestSize = form.test_size.selectedIndex;
}
function msSetDrive(form)
{
	gDriveLetter = form.test_drive.selectedIndex;
}
function msTestStart()
{
    var testArgument = gTestSize + "," + gDriveLetter;
    callFunc("ms_test", "ms_test.cgi", testArgument); 
}
