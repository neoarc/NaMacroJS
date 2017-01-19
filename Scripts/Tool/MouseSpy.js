//
// mouse spy
// 2015.12.02 neoarc (neoarcturus@gmail.com)
//
var _static;

function main()
{
	try {
		createLogWindow();
		spy();
	} catch(e) {
		alert("Error: " + e + " / " + typeof(e));
	}
}

function createLogWindow()
{
	var width = 550;
	var height = 360;
	var logger = new Window(0, 0, width, height);
	logger.create();
	logger.topmost = true;
	logger.text = "MouseSpy v2 by neoarc";
	logger.move("right", "top");
	logger.visible = true;

	_static = logger.addControl("Static", 0, 0, width, height, "", true);
}

function spy()
{
	var m = system.mouse;
	var old = {};

	setInterval(10, function () {
		var str = "";
	    if (m.x != old.x || m.y != old.y) {
			var w = getWindow(m.x, m.y);
			str += ("1) Mouse info\n");
	        str += ("   - in screen: " + m.x + ", " + m.y + "\n");
			str += ("   - in window: " + (m.x-w.x) + ", " + (m.y-w.y) + "\n");

			str += ("2) Hover info\n");
			var color = system.screen.getPixel(m.x, m.y);
			var rgb = to_rgb(color);
			str += ("   - color: (BGR)" + rgb.hex_bgr +
				" (r:" + rgb.rh + ", g:" + rgb.gh + ", b:" + rgb.bh + ") / " +
				rgb.value + " (r:" + rgb.r + ", g:" + rgb.g + ", b:" + rgb.b + ") \n"
			);
			str += ("   - color: (RGB)" + rgb.hex_rgb + "\n");

			str += ("3) Hover Window info\n");
			str += sprintWindowProperties(w) + "\n";

			var wa = getActiveWindow();
			str += ("4) Active Window info\n");
			str += sprintWindowProperties(wa) + "\n";

			_static.text = str;
	    }

	    old.x = m.x;
	    old.y = m.y;
		sleep(10);
	});
}

function sprintWindowProperties(w)
{
	var str = "";
	if (!w) {
		return "";
	}

	str += ("   - text: " + w.text + "\n");
	str += ("   - class: " + w.class + "\n");
	str += ("   - position: " + w.x + "," + w.y + " (" + w.width + "x" + w.height + ")\n");
	str += ("   - handle: " + to_hex8(w.handle) + " (" + w.handle + ")\n");
	str += ("   - topmost: " + w.topmost + "\n");
	str += ("   - visible: " + w.visible);

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
	rh = ('00'+rh).substring(rh.length);
	gh = ('00'+gh).substring(gh.length);
	bh = ('00'+bh).substring(bh.length);
	var hex_bgr = "0x" + bh + gh + rh;
	var hex_rgb = "0x" + rh + gh + bh;

	return {
		value: rgb,
		hex_bgr: hex_bgr,
		hex_rgb: hex_rgb,
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
	hex = ("000000"+hex).substring(hex.length);
	return "0x" + hex;
}
