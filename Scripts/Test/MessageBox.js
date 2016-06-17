//
// message boxes test
// 2016.06.15 neoarc (neoarcturus@gmail.com)
//
var k = system.keyboard;
var c = consoleWindow;
include("../Addon/VirtualKey.js");

function main() {
    try {
        if (0) {
            var ret = prompt("Input anything", "I am prompt");
            alert("Your input: " + ret);
        }
        if (1) {
            c.visible = true;
            bindHotkey();
            return;
        }
    } catch(e) {
        alert(e, "Exception!");
    }
    exit();
}

function bindHotkey() {
    print("Press a key to test. (esc: exit)");
    print("1: alert");
    print("2: confirm");
    print("3: prompt");

    var k = system.keyboard;
    var ret;
	k.on(VK["1"], function() {
        ret = alert("alert");
        print("-> " + ret);
	});

    k.on(VK["2"], function() {
        ret = confirm("confirm");
        print("-> " + ret);
    });

    k.on(VK["3"], function() {
        ret = prompt("prompt");
        print("-> " + ret);
	});

    k.on(VK.esc, function() {
		exit();
	});
}
