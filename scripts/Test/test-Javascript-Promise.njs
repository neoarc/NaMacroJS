// 17.08.14 neoarc

function main()
{
	// prepare
    consoleWindow.visible = true;
    var c = consoleWindow;
	
	// test
	var k = 0; // key for test (0 or 1)
    var p = new Promise(function (resolve, reject) {
		trace("please wait...");
		//trace("please wait 5 seconds...");
		//setTimeout(function() {
			var i = k;
			i++;
			if (i == 1)
				//resolve({});
				//resolve(new Object);
				resolve("yay");
			else
				reject("oops");
		//}, 5000);
	});
	
	p.then(function (res) {
		trace("resolved!");
		trace("response: " + res);
	}, function (res) {
		trace("rejected!");
		trace("response: " + res);
	});
}
