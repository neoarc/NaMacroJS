//
// Tester code
// 2015.12.02 neoarc (neoarcturus@gmail.com)
//

print("init NaMacro.js");

function main()
{
    //mouse_test();
    convGMacro_test();
	
	alert("Press a any key to exit", "End :)", 0);
	exit();
}

function convGMacro_test()
{
    var str = convGMacroToNaMacro("GMacro.gmc");
    alert(str);
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