include("../Addon/Window.js")
include("../Addon/Keyboard.js")

function main()
{
    var devenv_name = "Microsoft Visual Studio";
    var devenv_exe = "C:/Program Files (x86)/Microsoft Visual Studio 10.0/Common7/IDE/devenv.exe";
    var output_path = "d:\\Private";

    var w = getFirstWindow(devenv_name);
    if (w == null)
    {
        system.execute(devenv_exe);
        w = waitAndGetFirstWindow(devenv_name);
        sleep(2000);
    }

    w.activate();

    var k = system.keyboard;
    k.pressCompositeKey("alt+t,i,alt+e,alt+n,space,space,alt+n");
    sleep(500);
    k.pressCompositeKey("tab,back");
    k.typeString(output_path);
    k.pressCompositeKey("alt+f");
    sleep(500);
    k.pressCompositeKey("esc");

    exit();
}
