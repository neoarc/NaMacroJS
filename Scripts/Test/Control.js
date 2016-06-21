//
// tts module test
// 2016.05.03 neoarc (neoarcturus@gmail.com)
//
include("../AddOn/VirtualKey.js");
var c = consoleWindow;

function main() {
    test();
}

function test() {
    c.visible = true;

    // create window
    print("Create window.");
    var w = new Window();
    w.create();
    w.x = 0;
    w.y = 0;
    w.width = 300;
    w.height = 300;
    w.text = "Window";
    w.visible = true;

    // create control
    print("Create some control.");
    /*
    var ctrl = new Control("Button", 0, 0, 100, 100, w);
    ctrl.parent = w;
    ctrl.create();
    ctrl.text = "Control";
    ctrl.visible = true;
    w.addControl(ctrl);
    */

    var ctrl = w.addControl("Button", 0, 0, 100, 100, "Control", true);
    setInterval(1000, function () {
        var d = new Date();
        ctrl.text = d.getHours() + ":" + d.getMinutes() + ":" + d.getSeconds();
    });
}
