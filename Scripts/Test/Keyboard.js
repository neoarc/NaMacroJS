//
// keyboard module test
// 2016.04.29 neoarc (neoarcturus@gmail.com)
//
var k = system.keyboard;
var c = consoleWindow;

include("../Addon/VirtualKey.js");

function main() {
    try {
        c.visible = true;

        // test1: typeString
        if (0) {
            var w = findNotepad();
            w.activate();

            // automatic down,up keyboard
            k.typeString("Hello world?", 100);
        }

        // test2: bind event
        if (0) {
            bindHotkey();
            return;
        }

        // test3: hotkey in hotkey
        if (1) {
            bindHotkeyInHotkey();
            return;
        };

        alert("Done.");
    } catch(e) {
        alert(e, "Exception!");
    }
    exit();
}

function bindHotkeyInHotkey() {
    // press registered hotkey in hotkey handler
    print("Space key will typing '[space] ' (contain space)");
    var k = system.keyboard;
	k.on(VK.space, function() {
		k.typeString(" [space] ");
	});

    k.on(VK.esc, function() {
		exit();
	});
}

function bindHotkey() {
    print("Space key will typing '[space]'");
    var k = system.keyboard;
	k.on(VK.space, function() {
        // TODO hard to push "ctrl+a"
        k.down(VK.lcontrol);
        k.down(VK.a);
        k.up(VK.a);
        k.up(VK.lcontrol);

        k.down(VK.backSpace);
        k.up(VK.backSpace);

		k.typeString("[space]");
	});

    k.on(VK.esc, function() {
		exit();
	});

    // bad example: loop in handler causes blocking script.
    k.on(VK["5"], function() {
        stop = false;
        k.on(VK["6"], function() {
            print("stop!");
    	    stop = true;
    	});

		while(!stop) {
            print("5");
            sleep(1000);
        }
	});
}

function findNotepad() {
    var ar = findWindows("메모장");
    if (ar.length == 0) {
        throw ("Cannot find notepad");
    }

    return ar[0];
}
