//
// Tester code (debug build)
// 2015.12.02 neoarc (neoarcturus@gmail.com)
//

print("init NaMacro.js");

function main()
{
    key_test();

    //tts_test();
    //mouse_test();
    //convGMacro_test();
    //screen_test();

	alert("Press a any key to exit", "End :)", 0);
	exit();
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