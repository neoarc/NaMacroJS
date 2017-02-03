function waitAndGetFirstWindow(name)
{
	var ws;
	for ( ; ; )
	{
		ws = findWindows(name);
		if (ws.length > 0)
			break;

		sleep(100);
	}
	return ws[0];
}
