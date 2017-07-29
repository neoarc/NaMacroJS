//
// message boxes test
// 2016.06.15 neoarc (neoarcturus@gmail.com)
//
var k = system.keyboard;
var c = consoleWindow;
include("../Addon/VirtualKey.js");

function main() {
  try {
    if (0) {
      var ret = prompt("Input anything", "I am prompt");
      alert("Your input: " + ret);
    }
    if (1) {
      c.visible = true;
      bindHotkey();
      return;
    }
  } catch (e) {
    alert(e, "Exception!");
  }
  exit();
}

function bindHotkey() {
  print("Press a key to test. (esc: exit)");
  print("F1: alert");
  print("F2: confirm");
  print("F3: prompt");

  var k = system.keyboard;
  var ret;
  k.on(VK["f1"], function() {
    ret = alert("alert message", "alert title");
    print("-> " + ret);
  });

  k.on(VK["f2"], function() {
    ret = confirm("confirm message", "confirm title");
    print("-> " + ret);
  });

  k.on(VK["f3"], function() {
    ret = prompt("prompt message", "prompt title");
    print("-> " + ret);
  });

  k.on(VK.esc, function() {
    exit();
  });
}
