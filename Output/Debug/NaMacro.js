//
// Tester code (debug build)
// 2015.12.02 neoarc (neoarcturus@gmail.com)
//

print("init NaMacro.js");

function main()
{
	try {
		accessor_test();

	    //window_activate_test();
	    //consolewindow_test();
	    //window_test();
	    //key_test();
	    //tts_test();
		//mouse_test();
	    //convGMacro_test();
	    //screen_test();

		alert("Press a any key to exit", "End of demo script :)", 0);
		exit();

        // TODO timer
	} catch(e) {
		alert("Error: " + e + " / " + typeof(e));
	}
}

function accessor_test()
{
    consolewindow.visible = true;

	var m = system.mouse;

	for (var i = 0; i < 1000; i++) {
	    consolewindow.x--;
	    consolewindow.text = consolewindow.x;
	    sleep(1);
	}

	/*
	var old = {};
	while (true)
	{
	    if (m.x != old.x ||
            m.y != old.y) {
	        print("mouse pos: " + m.x + ", " + m.y);

	        var w = getWindow(m.x, m.y);
	        if (old.w == null ||
                w.text != old.w.text) {
	            print("-> text: " + w.text);
	            print("-> x,y: " + w.x + "," + w.y);
	            print("-> w,h: " + w.width + "x" + w.height);

	            old.w = {};
	            old.w.text = w.text;
	        }
	    }

	    old.x = m.x;
	    old.y = m.y;
		sleep(10);
	}
	//*/

    /*
	for (var i=0; i<1000; i++) {
		m.x = i;
		sleep(1);
	}
    */
}

function window_activate_test()
{
    consolewindow.visible = true;

    var ar = findWindows("컴퓨터");
    if (ar.length == 0) {
        alert("Cannot find window.");
        return;
    }

    print("found window: " + ar.length);
    for (var i = 0; i < ar.length; i++) {
        print(i + ") " + ar[i]._unique_id);
    }
    for (var i=0; i<10; i++) {
        ar[i%(ar.length)].activate();
        sleep(1000);
    }
}

function consolewindow_test()
{
    var c = consolewindow;
    c.visible = true;
    c.move(0, 0);

    print("this is console window.");
    print("1.moving test");
    print("2.toggle visible test");

    for (var i = 0; i < 400; i++)
    {
        sleep(1);
        c.move(i, i);
    }

    var v = false;
    for (var i = 0; i < 5; i++)
    {
        c.visible = v;
        v = !v;

        sleep(500);
    }
}

function window_test()
{
    var name = "Microsoft Visual Studio";
    //var name = "Firefox";

    var results = findWindows(name);
    if (results.length > 0) {
        var win = results[0];

        // move test
        //win.move(0, 0);

        // visible teset
        win.visible = false;
        sleep(1000);
        win.visible = true;
        sleep(1000);
        win.visible = false;
        sleep(1000);
        win.visible = true;
        sleep(1000);
    } else {
        alert("Cannot find Window :(");
    }
}

function key_test()
{
    consolewindow.visible = true;

    var win = findWindows("메모장");
    if (win.length == 0) {
        print("Open notepad to test.");
        while (win.length == 0) {
            win = findWindows("메모장");
            sleep(1000);
        }
    }
    win[0].activate();

    var k = system.keyboard;
    k.down(0x20); // VK_SPACE
    k.up(0x20);

    // type a to z
    for (var i=0x41; i<=0x5a; i++)
    {
        print(String.fromCharCode(i));

        k.down(i);
        sleep(10);

        k.up(i);
        sleep(10);
    }
}

function convGMacro_test()
{
    // Convert GMacro data to NaMacro script
    var str = convGMacroToNaMacro("GMacro.gmc");
    print(str);
}

function tts_test()
{
    //ttsSpeak("Hello World?");

    /*
    for (var i = 0; i < 3; i++)
    {
        var str = "... " + (1 + i) + " ...";
        ttsSpeak(str);
    }
    */

    //ttsSpeak("<rate speed='-4'>Press a any key to exit");

    /*
    for (var i=-10; i<10; i++)
    {
        ttsSpeak("<rate speed='" + i + "'>Hello ");
    }
    */

    // Async TTS Test
    var speakAsync = function (str) {
        print(str);
        ttsSpeak("<rate speed='0'/>" + str, true);
    }

    // count seconds
    var old, cur;
    for (var i = 1; i <= 10; i++) {
        old = Date.now();
        speakAsync(i);
        cur = Date.now();

        print(" > elapsed time for speak: " + (cur - old));
        var sleeptime = 1000 - (cur - old);
        if (sleeptime > 0)
            sleep(sleeptime);
    }
}

function mouse_test()
{
    // mouse api test
    var mouse = system.mouse;
    for (var i = 1; i < 100; i++) {
        var xy = i * 10;

        print(xy + ", " + xy);
        mouse.move(xy, xy);

        sleep(10);
    }
}


function screen_test()
{
    consolewindow.visible = true;

    // pick a pixel from point x,y
    var mouse = system.mouse;
    var screen = system.screen;
    for (var i = 1; i < 100; i++) {
        var xy = i * 10;

        mouse.move(xy, xy);
        var col = screen.getPixel(xy, xy);
        var r = parseInt(col / 256 / 256 % 256);
        var g = parseInt(col / 256 % 256);
        var b = parseInt(col % 256);

        print(xy + ", " + xy + ": " + r + "," + g + "," + b);

        sleep(10);
    }
}

// for debug
function isFunction(obj)
{
    return !!(obj && obj.constructor && obj.call && obj.apply);
}

// for debug
function showObjProperties(obj, level)
{
    if (!obj) return;
    if (level === undefined) level = 0;
    if (level > 1) return;

    for (var prop in obj) {
        for (var i = 0, space = ""; i < level; i++) space += "   ";

        if (this.isFunction(obj[prop])) continue;
        trace(space + prop + " : " + obj[prop]);

        try {
            if (typeof obj[prop] == "object") {
                this.showObjProperties(obj[prop], level + 1);
            }
        } catch (e) {
            trace(space + "   !!Exception!!");
        }
    }
}
