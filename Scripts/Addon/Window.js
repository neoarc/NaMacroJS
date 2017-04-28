function getFirstWindow(name)
{
	var ws;
	for ( ; ; )
	{
		ws = findWindows(name);
		if (ws.length > 0)
			break;

		else
			return null;
	}
	return ws[0];
}

function waitAndGetFirstWindow(name, print_log)
{
	var ws;
	for ( ; ; )
	{
		ws = findWindows(name);
		if (ws.length > 0)
			break;

		if (print_log === true)
			trace("findWindow: " + name);
		sleep(100);
	}
	return ws[0];
}
