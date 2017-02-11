var k = system.keyboard;
var c = consoleWindow;

include("../Addon/VirtualKey.js");

function main()
{
    try {
        c.visible = true;
        bindHotkey();

    } catch(e) {
        alert(e, "Exception!");
    }
}

function bindHotkey() {
    c.text = "Press esc to exit.";
    k.on(VK.esc, function() {
		exit();
	});

    print("press 'f1' to turn aero on");
    k.on(VK["f1"], function() {
        system.screen.setAero(true);
    });

    print("press 'f2' to turn aero off");
    k.on(VK["f2"], function() {
        system.screen.setAero(false);
    });
}
