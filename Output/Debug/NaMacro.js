//
// Tester code
// 2015.12.02 neoarc (neoarcturus@gmail.com)
//

print("init NaMacro.js");

function main()
{
    tts_test();

    mouse_test();

    //convGMacro_test();

	alert("Press a any key to exit", "End :)", 0);
	exit();
}

function convGMacro_test()
{
    var str = convGMacroToNaMacro("GMacro.gmc");
    print(str);
}

function tts_test()
{
    ttsSpeak("Hello World?");

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