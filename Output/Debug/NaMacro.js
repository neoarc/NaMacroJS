//
// Tester code (debug build)
// 2015.12.02 neoarc (neoarcturus@gmail.com)
//

print("init NaMacro.js");

function main()
{
    //timer_test();

    //window_activate_test();
    //consolewindow_test();
    //record_test();
    //window_test();
    //key_test();
    //tts_test();
    //mouse_test();
    //convGMacro_test();
    //screen_test();

	alert("Press a any key to exit", "End of demo script :)", 0);
	exit();
}

function timer_test()
{
    consolewindow.setVisible(true);

    // not support
    /*
    setInterval(0, function() {
        print("test.");
    }, 1000);
    */
}

function window_activate_test()
{
    var ar = findWindows("컴퓨터");
    if (ar.length == 0) {
        alert("Cannot find window.");
        return;
    }
    for (var i=0; i<10; i++) {
        ar[i%(ar.length)].activate();
        sleep(1000);
    }
}

function consolewindow_test()
{
    var c = consolewindow;
    c.setVisible(true);
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
        c.setVisible(v);
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
        win.setVisible(false);
        sleep(1000);
        win.setVisible(true);
        sleep(1000);
        win.setVisible(false);
        sleep(1000);
        win.setVisible(true);
        sleep(1000);
    } else {
        alert("Cannot find Window :(");
    }
}

function key_test()
{
    print("Open notepad and set focus in 10 seconds.");
    for (var i = 10; i > 0; i--)
    {
        print(i + "...");
        sleep(1000);
    }

    var k = system.keyboard;
    k.down(0x20); // VK_SPACE
    k.up(0x20);

    // type a to z
    for (var i=0x41; i<=0x5a; i++)
    {
        print(i);

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

        sleep(1000);
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

function record_test()
{
    // removed
}
