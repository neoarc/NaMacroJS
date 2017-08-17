//
// 2017.08.17 neoarc
//
var network_interface = "이더넷";

function main() {
  try {
    var w = new Window(0, 0, 210, 100);
    w.create();
    w.text = "Lan Toggler v0.1";
    w.visible = true;

    var c;
    c = w.addControl("Button", 10, 10, 80, 40, "Off", true, function() {
      switchLan(false);
    });
    c = w.addControl("Button", 100, 10, 80, 40, "On", true, function() {
      switchLan(true);
    });
  } catch (e) {
    alert("Error: " + e + " / " + typeof(e));
  }
}

function switchLan(enable)
{
    var op = enable ? "enable" : "disable";
    system.execute("netsh", "interface set interface \"" + network_interface + "\" " + op, false);
}
