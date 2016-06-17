//
// include test
// 2016.06.16 neoarc (neoarcturus@gmail.com)
//
var k = system.keyboard;
var c = consoleWindow;
c.visible = true;

include("../Addon/VirtualKey.js");

this.TestFactor = 0;
this.TestFactor += 1;

function main() {
    try {
        include("Sub/Include_Sub1.js");
        if (this.TestFactor == 111)
            alert("Success");
        alert("done.");
    } catch(e) {
        alert(e, "Exception!");
    }
    exit();
}
