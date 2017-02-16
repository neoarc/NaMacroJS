//
// ImageBot Addon Test
// 2017.02.16 neoarc
//
include("../Addon/ImageBot.js");

function main()
{
    try {
        var _c = consoleWindow;
        _c.visible = true;
        
        var b = new ImageBot(["VisualStudio2010_AboutOk"], "../Test/Image/");
        b.run();
    } catch (e) {
        alert("Exception: " + e);
    }
}
