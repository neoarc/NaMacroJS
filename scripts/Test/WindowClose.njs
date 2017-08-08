//
// 2017.01.30 neoarc (neoarcturus@gmail.com)
// Window.state test
//

function main() {
  var name = "메모장";
  var ws = findWindows(name);
  if (ws.length == 0) {
    alert("Cannot find window(s): " + name);
    exit();
    return;
  }

  var w = ws[0];
  w.close();
  alert("is " + name + " closed?");
  
  exit();
}
