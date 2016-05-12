//
// Tester code (debug build)
// 2015.12.02 neoarc (neoarcturus@gmail.com)
//

function main()
{
	try {
		consoleWindow.visible = true;

		//complex_test();
		//include_test();
		//object_wrap_test();
		//garbage_collection_test();
		//accessor_test(); // <-- window spy
	    //window_activate_test();
	    //consoleWindow_test();
	    //window_test();
	    //key_test();
	    //tts_test();
		//mouse_test();
	    //convGMacro_test();
	    //screen_test();

		alert("Press a any key to exit", "End of demo script :)", 0);

        // TODO timer
	} catch(e) {
		alert("Error: " + e + " / " + typeof(e));
	}
	exit();
}

function include_test()
{
	include("ModuleNeoarc.js");

	var a = new Neoarc();
	a.func();
}

function complex_test()
{
	function _random(_min, _max) {
		var r = Math.random() * (_max - _min);
		r = r % (_max - _min) + _min;
		r = parseInt(r);
		//print(_min + " ~ " + _max + " : " + r);
		return r;
	}

	var max_cnt = 10;
	var max_speed = 5;
    var s = system.screen;
	var ar = [];
	for (var i=0; i<max_cnt; i++) {
		var w = new Window(
			_random(0, s.width/3),
			_random(0, s.height/3),
			_random(100, s.width/2),
			_random(100, s.height/2)
		    );
		w.create();
		w.visible = true;
		w.text = "윈도우";
		w.xs = _random(1,max_speed) * (_random(0,10) > 5 ? 1 : -1);
		w.ys = _random(1,max_speed) * (_random(0,10) > 5 ? 1 : -1);

		ar.push(w);
	}

	for (var j=0; j<10000; j++ ) {
		for (var i=0; i<ar.length; i++) {
			var w = ar[i];

			w.x += w.xs
			w.y += w.ys;
			if (w.x + w.width >= s.width)
				w.xs *= -1;
			if (w.x < 0)
				w.xs *= -1;
			if (w.y + w.height >= s.height)
				w.ys *= -1;
			if (w.y < 0)
				w.ys *= -1;
		}
		sleep(1);
	}
}

function object_wrap_test()
{
	var ar = [];
	for (var i=0; i<5; i++) {
		var w = new Window();
		w.create(i*100, i*100, 500, 500);
		w.visible = true;
		w.text = "윈도우 " + i;

		//showObjProperties(w);
		ar.push(w);
	}

	var i=5;
	while(i>0) {
		print(i + "...");
		sleep(1000);
		--i;
	}
}

function garbage_collection_test()
{
	for (var i=0; i<1000; i++)
		make_garbage();

	var i=5;
	while(i>0) {
		print(i + "...");
		sleep(1000);
		--i;
	}
}

function make_garbage()
{
	var ran = parseInt(Math.random()*100) % 4;
	print("make a random garbage: " + ran);
	switch(ran) {
		case 0: {
			var img = system.screen.capture(0, 0, 1000, 1000);
		}
		break;
		case 1: {
			var aa = {name:"aa", year:35};
			var bb = {name:"bb", year:34};
		}
		break;
		case 2: {
			var ar = [];
			for (var i=0; i<10; i++) {
				ar[i] = [];
				for (var j=0; j<10; j++){
					ar[i][j] = {name:"dummy"};
				}
			}
		}
		break;
		case 3: {
			var win = findWindows("");
		}
		break;
	}
}

function accessor_test()
{
	var m = system.mouse;

    /*
	for (var i = 0; i < 1000; i++) {
	    consoleWindow.x--;
	    consoleWindow.text = consoleWindow.x;
	    sleep(1);
	}
    */

	///*
	var old = {};
	while (true)
	{
	    if (m.x != old.x ||
            m.y != old.y) {
	        print("mouse pos: " + m.x + ", " + m.y);

	        var w = getWindow(m.x, m.y);
	        if (old.w == null || w.text != old.w.text) {
				print("---------------------------------------------");
	            showObjProperties(w);

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

function consoleWindow_test()
{
    var c = consoleWindow;
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
	var k = system.keyboard;

	k.down(0x5b); // l win
	for (var i=0; i<30; i++) {
		k.down(0x09); // tab
		k.up(0x09);
		sleep(100);
	}
	k.down(0x1b); // esc
	k.up(0x1b);
	sleep(100);

	k.up(0x5b);

	/*
    var win = findWindows("메모장");
    if (win.length == 0) {
        print("Open notepad to test.");
        while (win.length == 0) {
            win = findWindows("메모장");
            sleep(1000);
        }
    }
    win[0].activate();

    k.down(0x20); // VK_SPACE
    k.up(0x20);

    // type a to z
    for (var i=0x41; i<=0x5a; i++) {
        print(String.fromCharCode(i));

        k.down(i);
        k.up(i);
    }
	*/
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
    // pick a pixel from point x,y
    var m = system.mouse;
    var s = system.screen;
	print("screen metrics: " + s.width + "x" + s.height);

	/*
    for (var i = 1; i < 100; i++) {
        var xy = i * 10;

        m.move(xy, xy);
        var col = s.getPixel(xy, xy);
        var r = parseInt(col / 256 / 256 % 256);
        var g = parseInt(col / 256 % 256);
        var b = parseInt(col % 256);

        print(xy + ", " + xy + ": " + r + "," + g + "," + b);

        sleep(10);
    }
	*/
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

        //if (isFunction(obj[prop])) continue;
        print(space + prop + " : " + obj[prop]);

        try {
            if (typeof obj[prop] == "object") {
                showObjProperties(obj[prop], level + 1);
            }
        } catch (e) {
            print(space + "   !!Exception!!");
        }
    }
}
