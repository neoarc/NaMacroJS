//
// Control test
// 2016.06.21 neoarc (neoarcturus@gmail.com)
//
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
    w.addControl("Button", 10, 10, 100, 100, "Button", true, function (code) {
        print("OnCommand: Button: " + code);
    });
    w.addControl("Edit", 10, 120, 100, 30, "Edit", true, function (code) {
        print("OnCommand: Edit: " + code);

        // set focus 256 = 0x100
        // kill focus 512 = 0x200

        // update 1024 = 0x400
        // change 768 = 0x300

        // maxtext 1281 = 0x501
    });
    w.addControl("Static", 10, 160, 100, 30, "Static", true, function (code) {
        // not work
        print("OnCommand: Static: " + code);
    });

    // image control
    var c, img;
    c = w.addControl("Button", 120, 10, 100, 100, "Button", true);
    c.image = img = system.screen.capture(0, 0, c.width, c.height);

    c = w.addControl("Button", 120, 120, 100, 100, "Button", true);
    c.image = "./Image/Question Block.png";

    c = w.addControl("Static", 120, 230, 100, 30, "Static", true);
    c.image = img = system.screen.capture(0, 0, c.width, c.height);
}
