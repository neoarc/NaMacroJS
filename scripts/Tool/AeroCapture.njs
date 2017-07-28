var k = system.keyboard;
var c = consoleWindow;

include("../Addon/VirtualKey.js");
include("../Addon/String.js");

function main()
{
  try {
    c.visible = true;
    bindHotkey();
  } catch (e) {
    alert(e, "Exception!");
  }
}

function bindHotkey()
{
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

  print("press 'f3' to capture screen");
  k.on(VK["f3"], function() {
    var img = system.screen.capture(0, 0, system.screen.width, system.screen.height);
    var currentdate = new Date();
    var dt =
      String(currentdate.getFullYear()).fillZeroForLength(4) +
      String((currentdate.getMonth() + 1)).fillZeroForLength(2) +
      String(currentdate.getDate()).fillZeroForLength(2) + "_" +
      String(currentdate.getHours()).fillZeroForLength(2) +
      String(currentdate.getMinutes()).fillZeroForLength(2) +
      String(currentdate.getSeconds()).fillZeroForLength(2);
    img.save(".\\capture_" + dt + ".bmp");
  });
}
