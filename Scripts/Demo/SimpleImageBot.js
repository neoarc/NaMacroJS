var g_img_list = [
    "복원1",
    "복원2",
    "업데이트1",
    "업데이트2",
];
var g_img_obj = [];

var g_img_path = "./SimpleImageBot/";
var _c = consoleWindow;
var _s = system.screen;
var _m = system.mouse;
var g_img_scr = null;
var g_pt_found = null;
var g_win = null;

function main()
{
    _c.visible = true;

    loadImages();
    runBot();
}

function runBot()
{
    for ( ; ; ) {
        var ret = findImageFromList(g_img_list, g_img_obj, 0);
        if (ret) {
            trace(ret + " 찾음.");
            clickFound(2000, true);
        }
        else {
            consoleWindow.activate();
        }

        sleep(1000);
    }
}

function loadImages()
{
    trace("Load Images begin.");

    // filelist, obj
    loadImageToBuffer(g_img_list, g_img_obj);
    trace("Load Images end.");
}

function loadImageToBuffer(list, buffer)
{
    for (var i=0; i<list.length; i++)
    {
        var filename = list[i];
        var img = new Image(g_img_path + filename + ".bmp");
        img._id = filename;
        buffer[i] = img;
        buffer[filename] = img;
        trace("> " + img._id + " / " + img.width + " x " + img.height);
    }

    buffer.length = list.length;
}

function captureScreen()
{
    if (g_img_scr)
        g_img_scr.reset();
    g_img_scr = _s.capture(0, 0, _s.width, _s.height);
}

function findImageFromList(img_list, img_obj, acc_factor)
{
    captureScreen();

    var is_found = false;
    for (var i = 0; i < img_list.length; i++) {
        if (findImage(img_obj[i]._id, acc_factor)) {
            return img_obj[i]._id;
        }
    }

    return null;
}

function findImageAndClick(img_name, delay_after_click, delay_when_rescan, not_found_msg)
{
    for (; ;) {
        captureScreen();
        if (findImage(img_name, 50, from_y, to_y)) {
            clickFound(delay_after_click, true);
            return true;
        }
        if (not_found_msg !== undefined)
            trace(not_found_msg);

        sleep(delay_when_rescan);
    }
}

function findImage(img_name, acc_factor)
{
    if (!g_img_scr)
        throw "g_img_scr is null";

    if (acc_factor === undefined)
        acc_factor = 0;
    g_pt_found = g_img_scr.findImage(g_img_obj[img_name], acc_factor);

    return g_pt_found;
}

function clickFound(sleep_time, recapture)
{
    if (!g_pt_found)
        throw "g_pt_found is null";

    //postClick(g_pt_found.x, g_pt_found.y);
    _m.click(g_pt_found.x, g_pt_found.y);
    sleep(sleep_time);

    if (recapture === true)
        captureScreen();
}

function postClick(x, y)
{
    _m.postLbuttonDown(x, y);
    sleep(10);
    _m.postLbuttonUp(x, y);
}
