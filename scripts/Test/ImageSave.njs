//
// Image.save test
// 2017.02.18 neoarc (neoarcturus@gmail.com)
//
function main() {
  var s = system.screen;
  var img = s.capture(0, 0, s.width, s.height);

  img.save("./Image/SavedImage.bmp");
  img.save("./Image/SavedImage.jpg");
  img.save("./Image/SavedImage.png");

  alert("bmp, jpg, png saved.");
  exit();
}
