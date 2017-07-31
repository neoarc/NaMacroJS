//
// Control test
// 2016.06.21 neoarc (neoarcturus@gmail.com)
//
function main() {
  test();
}

function test() {
  consoleWindow.visible = true;

  var w = findWindows("카카오톡");
  if (w.length == 0)
    return;
  w = w[0];

  // Add Control to another program
  w.addControl("Button", 100, 3, 100, 25, "Button", true, function() {
    // but callback is not working
    alert("haha!");
  });
}
