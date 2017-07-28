 //
// mouse spy
// 2015.12.02 neoarc (neoarcturus@gmail.com)
//

function main()
{
  try {
    spy();
  } catch (e) {
    alert("Error: " + e + " / " + typeof(e));
  }
}

function spy()
{
  var c = consoleWindow;
  c.visible = true;
  c.topmost = true;
  c.text = "MouseSpy by neoarc";

  var m = system.mouse;

  var old = {};
  while (true) {
    if (m.x != old.x || m.y != old.y) {
      var w = getWindow(m.x, m.y);
      print("---------------------------------------------");
      print("1) Mouse info");
      print("   - in screen: " + m.x + ", " + m.y);
      print("   - in window: " + (m.x - w.x) + ", " + (m.y - w.y));

      print("2) Hover info");
      var color = system.screen.getPixel(m.x, m.y);
      var rgb = to_rgb(color);
      print("   - color: " + rgb.hex +
        " (r:" + rgb.rh + ", g:" + rgb.gh + ", b:" + rgb.bh + ") / " +
        rgb.value + " (r:" + rgb.r + ", g:" + rgb.g + ", b:" + rgb.b + ") "
      );

      print("3) Hover Window info");
      printWindowProperties(w);

      var wa = getActiveWindow();
      print("4) Active Window info");
      printWindowProperties(wa);
    }

    old.x = m.x;
    old.y = m.y;
    sleep(10);
  }
}

function printWindowProperties(w)
{
  if (!w) {
    return;
  }

  print("   - text: " + w.text);
  print("   - class: " + w.class);
  print("   - position: " + w.x + "," + w.y + " (" + w.width + "x" + w.height + ")");
  print("   - handle: " + to_hex8(w.handle) + " (" + w.handle + ")");
  print("   - topmost: " + w.topmost);
  print("   - visible: " + w.visible);
}

function to_rgb(rgb)
{
  var r = parseInt(rgb % 256);
  var g = parseInt(rgb / 256 % 256);
  var b = parseInt(rgb / 256 / 256 % 256);

  var rh = r.toString(16);
  var gh = g.toString(16);
  var bh = b.toString(16);
  rh = ('00' + rh).substring(rh.length);
  gh = ('00' + gh).substring(gh.length);
  bh = ('00' + bh).substring(bh.length);
  var hex = "0x" + bh + gh + rh;

  return {
    value: rgb,
    hex: hex,
    r: r,
    g: g,
    b: b,
    rh: rh,
    gh: gh,
    bh: bh
  };
}

function to_hex8(value)
{
  var hex = value.toString(16);
  hex = ("000000" + hex).substring(hex.length);
  return "0x" + hex;
}
