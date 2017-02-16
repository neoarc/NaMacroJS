//
// ImageBot Addon Test
// 2017.02.16 neoarc
//
include("../Addon/ImageBot.js");

var g_image_list = [
    "Vs2010_AboutOk",
    "Vs2015_AboutOk",
];

function main()
{
    try {
        var _c = consoleWindow;
        _c.visible = true;
        
        // Create ImageBot
        var bot = new ImageBot(g_image_list, "../Test/Image/");

        // Run. (Find and Click)
        bot.run();

        // Is it simple?
    } catch (e) {
        alert("Exception: " + e);
    }
}
