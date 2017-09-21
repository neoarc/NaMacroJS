
function main() {
  test();
}

function test() {
  consoleWindow.visible = true;

  // Find image from whole screen
  let cnt = 100;
  ///*
  for (var i=0; i<cnt; i++) {
      var img = system.screen.capture(0, 0, system.screen.width, system.screen.height);
      img = null;
  }
  //gc();
  //*/

  /*
  for (var i=0; i<cnt; i++) {
      var img = new Image("./Image/KakaoTray.bmp");
      delete img;
      img = null;
  }
  //*/

  /*
  for (var i=0; i<1000000; i++) {
      var f = new File("./dummy.ext");
      delete f;
      f = null;
  }
  //*/
}
