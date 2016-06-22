//
// tts module test
// 2016.05.03 neoarc (neoarcturus@gmail.com)
//
include("../AddOn/VirtualKey.js");

function main() {
    test();
}

function test() {
    consoleWindow.visible = true;

    // create window
    print("Create window.");
    var w = new Window(0, 0, 300, 300);
    w.create();
    w.text = "Window";
    w.move("center", "middle");
    w.visible = true;

    // create control
    print("Create some control.");

    // simple control
    w.addControl("Button", 10, 10, 100, 100, "Button", true, function () {
        print("OnCommand: Button");
    });
    w.addControl("Edit", 10, 120, 100, 30, "Edit", true);
    w.addControl("Static", 10, 160, 100, 30, "Static", true);

    // image control
    var c, img;
    c = w.addControl("Button", 120, 10, 100, 100, "Button", true);
    c.image = img = system.screen.capture(0, 0, c.width, c.height);

    c = w.addControl("Button", 120, 120, 100, 100, "Button", true);
    c.image = "./Image/Question Block.png";

    c = w.addControl("Static", 120, 230, 100, 30, "Static", true);
    c.image = img = system.screen.capture(0, 0, c.width, c.height);
}
