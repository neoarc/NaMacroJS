//
// include test
// 2016.06.16 neoarc (neoarcturus@gmail.com)
//
include("../Addon/VirtualKey.js");

this.TestFactor = 0;

function main() {
    try {
        try {
            if (VirtualKey !== undefined)
                this.TestFactor += 1;
        } catch(e) {

        }

        include("Sub/Include_Sub1.js");
        if (this.TestFactor == 111)
            alert("Test Success.");
        else
            alert("Test Failed.");
    } catch(e) {
        alert(e, "Exception!");
    }
    exit();
}
