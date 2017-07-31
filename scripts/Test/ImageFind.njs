//
// Find image
// 2017.01.21 neoarc (neoarcturus@gmail.com)
//
function main() {
  test();
}

function test() {
  consoleWindow.visible = true;

  // create window
  var w = new Window(0, 0, 500, 500);
  w.create();
  w.text = "Window";
  w.move("right", "top");
  w.visible = true;

  // Find image in image
  /*
  var img1 = system.screen.capture(150, 150, 100, 100);
  var img2 = system.screen.capture(0, 0, 300, 300);

  var c;
  c = w.addControl("Static", 0, 0, 100, 100, "", true);
  c.image = img1;

  c = w.addControl("Static", 0, 110, 300, 300, "", true);
  c.image = img2;
  */

  // Find image from whole screen
  var img1 = new Image("./Image/KakaoTray.bmp");
  var img2 = system.screen.capture(0, 0, system.screen.width, system.screen.height);

  var c;
  c = w.addControl("Static", 0, 0, 100, 100, "", true);
  c.image = img1;

  c = w.addControl("Static", 0, 110, 500, 500, "", true);
  c.image = img2;

  var time1 = Date.now();
  var ret = img2.findImage(img1);
  var time2 = Date.now();
  if (ret.x == -1 && ret.y == -1) {
    alert("Fail!");
  } else {
    system.mouse.move(ret.x + img1.width / 2, ret.y + img1.height / 2);

    // logic1: 4000ms
    // logic2: 40ms
    alert(
      "Found! " +
      ret.x + "," + ret.y + " : " +
      (time2 - time1) + "ms");
  }
}
