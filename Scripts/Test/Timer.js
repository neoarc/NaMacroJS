//
// timer test
// 2016.05.13 neoarc (neoarcturus@gmail.com)
//
var k = system.keyboard;
var c = consoleWindow;

include("../Module/VirtualKey.js");

function main() {
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

    print("press 'a' to toggle 1000ms interval.");
    k.on(VK.a, function() {
        if (this._timerA) {
            clearInterval(this._timerA);
            this._timerA = null;
        }
        else {
            this._timerA = setInterval(1000, function () {
                print("1000ms Interval handler.");
            });
        }
    });

    print("press 'b' to toggle 300ms interval.");
    k.on(VK.b, function() {
        if (this._timerB) {
            clearInterval(this._timerB);
            this._timerB = null;
        }
        else {
            this._timerB = setInterval(300, function () {
                print("300ms Interval handler.");
            });
        }
    });

    print("press 'c' to set 500ms Timeout.");
    k.on(VK.c, function() {
        setTimeout(500, function () {
            print("500ms Timeout.");
        });
    });
}
