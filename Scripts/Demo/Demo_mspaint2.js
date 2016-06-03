//
// mspaint painting demo 2
// 2016.05.13 neoarc (neoarcturus@gmail.com)
//

include("../Addon/VirtualKey.js");

var m = system.mouse;
var k = system.keyboard;
k.du = function(vk, repeat) {
    if (repeat === undefined)
        repeat = 1;
    for (var i=0; i<repeat; i++) {
        k.down(vk);
        k.up(vk);
    }
};
var painter_win;
var cur_mode = -1;
var cur_brush_size = -1;
var img;
var simplify_factor = 32; // 1=original

function main() {
    consoleWindow.visible = true;
    try {
        painter_win = launch_mspaint();
        painter_win.click = function (x, y) {
            m.click(this.x + x, this.y + y);
        };

        initialize();
        calibrate(painter_win);
    } catch(e) {
        alert(e, "Exception");
    }
}

function launch_mspaint() {
    var ar = findWindows("그림판");
    if (ar.length == 0) {
        m.click(22, 1177); // start button
        k.typeString("mspaint");
        sleep(2000);

        k.down(VK.enter);
        k.up(VK.enter);

        ar = findWindowsWait("그림판");
    }
    return ar[0];
}

function findWindowsWait(name) {
    var ar;
    for ( ; ; ) {
        ar = findWindows(name);
        if (ar.length > 0)
            break;

        sleep(10);
    }
    return ar;
}

function initialize() {
    print("press 'esc' to exit");
    k.on(VK.esc, function() {
        exit();
    });

    print("press 'f1' to capture image");
    k.on(VK.f1, function() {
        captureImage();
    });

    print("press 'f2' to draw captured image");
    k.on(VK.f2, function() {
        drawCapturedImage();
    });

    k.on(VK.t, function() {
        test();
    });
}

function calibrate(w) {
    w.move(0, 0, 800, 600);
    w.activate();
}

function test() {
    ;
}

function changeCanvasSize(w, h) {
    // alt+f, e
    k.down(VK.lalt);
    k.du(VK.f);
    k.up(VK.lalt);
    k.du(VK.e);
    sleep(100);

    k.typeString(w.toString());
    k.du(VK.tab);
    k.typeString(h.toString());
    k.du(VK.enter);
}

function changeColor(rgb) {
    m.click(737, 88);
    var ar = findWindowsWait("색 편집");
    var w = ar[0];
    sleep(200);

    var _x = w.x + 521;
    var _y = w.y + 222;
    for (var i=0; i<3; i++) {
        m.click(_x, _y);
        sleep(100);

        k.du(VK.backSpace, 3);
        sleep(100);

        k.typeString(rgb[i].toString());
        _y += 25;
    }
    k.du(VK.enter);

    while (true) {
        ar = findWindows("색 편집");
        if (ar.length == 0)
            break;
    }
}

function captureImage() {
    var w = 50;
    var h = 50;

    img = system.screen.capture(m.x - (w/2), m.y - (h/2), w, h);
}

function drawCapturedImage() {
    if (!img)
        return;

    var colors = [];
    for (var y=0; y<img.height; y++) {
        for (var x=0; x<img.width; x++) {
            var color = img.getPixel(x, y);
            var rgb = color_to_rgb(color);
            rgb = simplify_rgb(rgb);
            color = rgb_to_color(rgb);

            if (colors.indexOf(color) < 0)
                colors.push(color);
        }
    }
    print("Colors: " + colors.length);

    var progress_max = colors.length * img.width * img.height;
    var progress = 0;
    var jump = 1;
    for (var c=0; c<colors.length; c++) {
        var color = colors[c];
        var rgb = color_to_rgb(color);
        print("color #" + (c+1) + "/" + colors.length + ": " + color);
        changeColor(rgb);

        for (var y=0; y<img.height; y+=jump) {
            var old_color;
            var combo = 0;
            for (var x=0; x<img.width; x+=jump) {
                progress += (jump*jump);
                consoleWindow.text = (progress*100.0) / (progress_max) + "%";

                var color2 = img.getPixel(x, y);
                var rgb2 = color_to_rgb(color2);
                rgb2 = simplify_rgb(rgb2);
                color2 = rgb_to_color(rgb2);

                if (color != color2) {
                    if (x != 0 && old_color == color && combo>0) {
                        if (combo == 1) {
                            drawPixel(x-1, y);
                        }
                        else {
                            drawLine(x-combo, y, x-1, y, 1);
                        }
                    }
                    old_color = color;
                    combo = 0;
                    continue;
                }

                old_color = color;
                combo++;


                if (x+jump >= img.width && combo>1) {
                    drawLine(x-(combo-1), y, x, y, 2);
                }
            }
        }
    }
}

function drawLine(x, y, x2, y2, test) {
    if (cur_mode != 1) {
        // alt+h, s, h, enter
        k.down(VK.lalt);
        k.du(VK.h);
        k.up(VK.lalt);
        k.du(VK.s);
        k.du(VK.h);
        k.du(VK.enter);

        cur_mode = 1;
    }

    if (cur_brush_size != 1) {
        k.down(VK.lalt);
        k.du(VK.h);
        k.up(VK.lalt);
        k.du(VK.s);
        k.du(VK.z);
        k.du(VK.space);

        cur_brush_size = 1;
    }

    var client_x = painter_win.x + 13;
    var client_y = painter_win.y + 157;

    m.lbuttonDown(client_x + x, client_y + y);
    sleep(10);
    m.lbuttonUp(client_x + x2, client_y + y2);
    sleep(10);

    // to exit line drawing mode
    //m.click(client_x + x2 + 30, client_y + y2);
    {
        k.down(VK.lalt);
        k.du(VK.h);
        k.up(VK.lalt);
        k.du(VK.p);
        cur_mode = 0;
    }
}

function drawPixel(x, y) {
    if (cur_mode != 0) {
        // alt+h, p
        k.down(VK.lalt);
        k.du(VK.h);
        k.up(VK.lalt);
        k.du(VK.p);

        cur_mode = 0;
    }

    painter_win.click(13 + x, 157 + y);
    painter_win.click(13 + x, 157 + y);
}

function color_to_rgb(color) {
    var b = parseInt(color / 256 / 256 % 256);
    var g = parseInt(color / 256 % 256);
    var r = parseInt(color % 256);

    return [r, g, b];
}

function simplify_rgb(rgb) {
    var f = simplify_factor;

    rgb[0] = parseInt(rgb[0] / f) * f;
    rgb[1] = parseInt(rgb[1] / f) * f;
    rgb[2] = parseInt(rgb[2] / f) * f;

    return rgb;
}

function rgb_to_color(rgb) {
    var color = 0;
    color += rgb[0];
    color += rgb[1]*256;
    color += rgb[2]*(256*256);

    return color;
}
