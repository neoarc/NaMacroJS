include("../Addon/Window.js")
include("../Addon/Keyboard.js")

var c = consoleWindow;
var k = system.keyboard;

function main() {
  c.visible = true;
  print("Launch notepad...");

  var w = waitAndGetFirstWindow("메모장");
  w.activate();
  for (var i = 0; i < 10; i++)
    k.typeString("[Hello world] ");

  sleep(1000);

  k.pressCompositeKey("enter,.,.,.,ctrl+a,back,h,a,h,a,h,a");
  k.pressCompositeKey("ctrl+a,back");
  k.typeString("Thank you :)");

  exit();
}
