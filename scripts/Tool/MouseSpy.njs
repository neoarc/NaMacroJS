//
// mouse spy
// 2015.12.02 neoarc (neoarcturus@gmail.com)
//
include("../Addon/VirtualKey.js");

var g_static;
var g_static_color; // TODO color box (color picker)
var g_static_hotkey;
var g_boost_click_timer = null;

function main()
{
  try {
    createLogWindow();
    initHotkey();
    spy();
  } catch (e) {
    alert("Error: " + e + " / " + typeof(e));
  }
}

function createLogWindow()
{
  var width = 550;
  var height = 320;
  var logger = new Window(0, 0, width, height);
  logger.create();
  logger.topmost = true;
  logger.text = "MouseSpy v3 by neoarc";
  logger.move("right", "top");
  logger.visible = true;

  g_static = logger.addControl("Static", 0, 0, width, height, "", true);
  //_color = logger.addControl("Static", width, 0, 100, 100, "Static", true);

  var inc = 120;
  logger.height = logger.height + inc;
  g_static_hotkey = logger.addControl("Static", 0, height, width, inc, "", true);
  g_static_hotkey.text =
    "4) Helper hotkeys\n" +
    "   - F9: click\n" +
    "   - F10: postClick\n" +
    "   - F11: boost click (toggle)\n" +
    "   - F12: tag current info (TODO not impl)";
}

function initHotkey()
{
  var k = system.keyboard;
  var m = system.mouse;
  k.on(VK["f9"], function() {
    m.lbuttonDown(m.x, m.y);
    sleep(10);
    m.lbuttonUp(m.x, m.y);
  });

  k.on(VK["f10"], function() {
    m.postLbuttonDown(m.x, m.y);
    sleep(10);
    m.postLbuttonUp(m.x, m.y);
  });

  k.on(VK["f11"], function() {
    if (g_boost_click_timer === null) {
      g_boost_click_timer = setInterval(30, function() {
        system.mouse.click(system.mouse.x, system.mouse.y);
      });
    } else {
      clearInterval(g_boost_click_timer);
      g_boost_click_timer = null;
    }
  });
}

function spy()
{
  var m = system.mouse;
  var old = {};

  setInterval(10, function() {
    var str = "";
    if (m.x != old.x || m.y != old.y) {
      var w = getWindow(m.x, m.y);
      var wa = getActiveWindow();

      str += ("1) Mouse info\n");
      str += ("   - in screen: " + m.x + ", " + m.y + "\n");
      str += ("   - in active window: " + (m.x - wa.x) + ", " + (m.y - wa.y) + "\n");
      str += ("   - in hover window: " + (m.x - w.x) + ", " + (m.y - w.y) + "\n");

      var color = system.screen.getPixel(m.x, m.y);
      var rgb = to_rgb(color);
      str += ("   - Color(bgr): " + rgb.hex +
        " (r:" + rgb.rh + ", g:" + rgb.gh + ", b:" + rgb.bh + ") / " +
        rgb.value + " (r:" + rgb.r + ", g:" + rgb.g + ", b:" + rgb.b + ") \n"
      );

      str += ("2) Hover Window info\n");
      str += getWindowProperties(w) + "\n";

      str += ("3) Active Window info\n");
      str += getWindowProperties(wa) + "\n";

      g_static.text = str;
      //_color.image = system.screen.capture(m.x, m.y, 100, 100);
    }

    old.x = m.x;
    old.y = m.y;
    sleep(10);
  });
}

function getWindowProperties(w)
{
  var str = "";
  if (!w) {
    return "";
  }

  str += ("   - text: " + w.text + "\n");
  str += ("   - class: " + w.class + "\n");
  str += ("   - position: " + w.x + "," + w.y + " (" + w.width + "x" + w.height + ") client: (" + w.clientWidth + "x" + w.clientHeight + ")\n");
  str += ("   - handle: " + to_hex8(w.handle) + " (" + w.handle + ")\n");
  str += ("   - topmost: " + w.topmost + "\n");
  str += ("   - visible: " + w.visible);
  str += ("   - state: " + w.state);

  return str;
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
